#include "chess_engine/ChessEngine.h"
#include "Board.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <sstream>
#include <random>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <fstream>

#ifdef _WIN32
    #include <process.h>
    #include <windows.h>
    #define getpid _getpid
#else
    #include <unistd.h>
#endif

/*
Regarding UCI:
    Can probably get its own struct called UCI which contains helpful UCI funcs.
Regarding Perft:
    We should speed this up and then take out default threaded option.
    Check how big call stack is? Do we overflow? Like a million steps or something crazy
        each with a stack fram containing a ChessEngine......
*/

const ChessEngine::EngineID ChessEngine::engineID = {"Wazzu Engine", "Jamieson Mansker"};
const ChessEngine::EngineOptionNames ChessEngine::engineOptionNames = {"Threads"};

ChessEngine::ChessEngine(FENString fen) : m_fen{fen}, m_board{std::make_unique<Board>(fen)} {
    // Open UCI log file - overwrite for each new session
    m_uciLog.open("ucilog.txt", std::ios::out | std::ios::trunc);
    if (m_uciLog.is_open()) {
        m_uciLog << "=== New Chess Engine Session Started ===" << std::endl;
        m_uciLog.flush();
    }
}

ChessEngine::~ChessEngine() {
    if (m_uciLog.is_open()) {
        m_uciLog << "=== Chess Engine Session Ended ===" << std::endl;
        m_uciLog.close();
    }
}

// Dedicate a thread to listening, await commands, execute command.
void ChessEngine::uciStart() {
    std::cout << "Wazzu Chess Engine v1.0" << std::endl;

    // Start the signal listener thread
    m_signalThread = std::thread(&ChessEngine::_signalListener, this);
    
    // Main UCI loop - process commands
    while (!m_shouldStop.load()) {
        std::unique_lock<std::mutex> lock(m_commandMutex);
        m_commandCV.wait(lock, [this] { return !m_commandQueue.empty() || m_shouldStop.load(); });
        
        if (m_shouldStop.load()) break;
        
        if (!m_commandQueue.empty()) {
            std::string command = m_commandQueue.front();
            m_commandQueue.pop();
            lock.unlock();
            
            _processCommand(command);
        }
    }
    
    // Clean up thread
    if (m_signalThread.joinable()) {
        m_signalThread.join();
    }
}

void ChessEngine::_makeUciMove(const std::string& uciMove) {
    try {
        if (uciMove.length() < 4) return;
        
        // Parse UCI move format (e.g., "e2e4", "e7e8q" for promotion)
        char fromFile = uciMove[0];
        char fromRank = uciMove[1];
        char toFile = uciMove[2];
        char toRank = uciMove[3];
        
        // Validate coordinates
        if (fromFile < 'a' || fromFile > 'h' || toFile < 'a' || toFile > 'h' ||
            fromRank < '1' || fromRank > '8' || toRank < '1' || toRank > '8') {
            return;
        }
        
        // Convert to array indices (a1 = row 7, col 0)
        size_t fromRow = 8 - (fromRank - '0');
        size_t fromCol = fromFile - 'a';
        size_t toRow = 8 - (toRank - '0');
        size_t toCol = toFile - 'a';
        
        MoveCoordsData move = {fromRow, fromCol, toRow, toCol};
        
        // Check for promotion
        if (uciMove.length() == 5) {
            char promotionChar = uciMove[4];
            Piece_T promotionPiece;
            switch (promotionChar) {
                case 'q': promotionPiece = Piece_T::QUEEN; break;
                case 'r': promotionPiece = Piece_T::ROOK; break;
                case 'b': promotionPiece = Piece_T::BISHOP; break;
                case 'n': promotionPiece = Piece_T::KNIGHT; break;
                default: return;
            }
            isValidMove(move, promotionPiece);
        } else {
            isValidMove(move);
        }
    } catch (const std::exception& e) {
        // Log error but don't crash - probably corrupted FEN
        if (m_uciLog.is_open()) {
            m_uciLog << "ERROR in _makeUciMove: " << e.what() << " for move: " << uciMove << std::endl;
            m_uciLog << "Current FEN: " << m_fen.getFen() << std::endl;
            m_uciLog.flush();
        }
        // Don't process this move, continue with current position
        return;
    }
}

std::string ChessEngine::_getRandomValidMove() const {
    std::vector<std::string> validMoves;
    
    // Generate all possible moves for the current player
    for(size_t fromRow = 0; fromRow < MAX_ROWS; ++fromRow) {
        for(size_t fromCol = 0; fromCol < MAX_COLS; ++fromCol) {
            const Piece* piece = m_board->getPieceAt(fromRow, fromCol);
            if(!piece) continue;
            
            // Get the current active player
            char activePlayer = m_fen.getActiveTurn();
            Color_T currentPlayerColor = (activePlayer == 'w') ? Color_T::WHITE : Color_T::BLACK;
            
            if(piece->getColor() != currentPlayerColor) continue;
            
            // Try all possible destination squares for this piece
            for(size_t toRow = 0; toRow < MAX_ROWS; ++toRow) {
                for(size_t toCol = 0; toCol < MAX_COLS; ++toCol) {
                    try {
                        Square& from = m_board->getBoardAt(fromRow, fromCol);
                        Square& to = m_board->getBoardAt(toRow, toCol);
                        
                        if(m_board->isLegalMove(from, to, piece)) {
                            // Convert coordinates to UCI notation
                            char fromFile = 'a' + fromCol;
                            char fromRank = '8' - fromRow;
                            char toFile = 'a' + toCol;
                            char toRank = '8' - toRow;
                            
                            // Check if this is a pawn promotion move
                            if(piece->getType() == Piece_T::PAWN && m_board->pawnCanPromote(to, piece->getColor())) {
                                // Add all promotion options
                                std::vector<char> promotions = {'q', 'r', 'b', 'n'};
                                for(char promo : promotions) {
                                    std::string move = std::string() + fromFile + fromRank + toFile + toRank + promo;
                                    validMoves.push_back(move);
                                }
                            } else {
                                std::string move = std::string() + fromFile + fromRank + toFile + toRank;
                                validMoves.push_back(move);
                            }
                        }
                    } catch (const std::exception& e) {
                        continue;
                    }
                }
            }
        }
    }
    
    if (validMoves.empty()) {
        return "a1a1"; // No valid moves return a1a1
    }
    
    // Use time-based seed for randomness
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, validMoves.size() - 1);
    
    return validMoves[dist(gen)];
}
std::string ChessEngine::_collectSignal() const {
    std::string signal;
    std::getline(std::cin, signal);
    
    // Log the incoming command
    if (m_uciLog.is_open()) {
        m_uciLog << "GUI -> Engine: " << signal << std::endl;
        m_uciLog.flush();
    }
    
    return signal;
}

void ChessEngine::_executeSignal(std::string signal) {
    std::istringstream iss(signal);
    std::string command;
    iss >> command;
    
    if (command == "quit") {
        std::exit(0);
    } else if (command == "isready") {
        std::cout << "readyok" << std::endl;
    } else if (command == "ucinewgame") {
        // Reset to starting position
        m_fen = FEN_STARTING_POS;
        m_board = std::make_unique<Board>(m_fen);
    } else if (command == "position") {
        std::string posType;
        iss >> posType;
        
        if (posType == "startpos") {
            m_fen = FEN_STARTING_POS;
            m_board = std::make_unique<Board>(m_fen);
            
            std::string movesKeyword;
            iss >> movesKeyword;
            if (movesKeyword == "moves") {
                std::string move;
                while (iss >> move) {
                    _makeUciMove(move);
                }
            }
            
            // Log the final FEN after processing all moves
            std::string finalFen = m_board->getFenStr();
            std::string logMessage = "Final FEN after position fen: " + finalFen;
            std::cout << logMessage << std::endl;
            _logOutput(logMessage);
        } else if (posType == "fen") {
            std::string fenStr;
            std::string token;
            while (iss >> token && token != "moves") {
                if (!fenStr.empty()) fenStr += " ";
                fenStr += token;
            }
            
            m_fen = FENString(fenStr);
            m_board = std::make_unique<Board>(m_fen);
            
            if (token == "moves") {
                std::string move;
                while (iss >> move) {
                    _makeUciMove(move);
                }
            }
            
            // Log the final FEN after processing all moves
            std::string finalFen = m_board->getFenStr();
            std::string logMessage = "Final FEN after position fen: " + finalFen;
            std::cout << logMessage << std::endl;
            _logOutput(logMessage);
        }
    } else if (command == "go") {
        // Generate and return a random valid move
        std::string bestMove = _getRandomValidMove();
        if (!bestMove.empty()) {
            std::cout << "bestmove " << bestMove << std::endl;
        } else {
            std::cout << "bestmove 0000" << std::endl;
        }
    } else if (command == "setoption") {
        // Handle setoption commands (for now just acknowledge)
        // Format: setoption name <name> value <value>
        std::string name, nameValue, value, valueValue;
        iss >> name >> nameValue >> value >> valueValue;
        // For now, just acknowledge - could store thread count etc.
    }
}

void ChessEngine::_signalListener() {
    while (!m_shouldStop.load()) {
        std::string signal = _collectSignal();
        
        if (signal == "quit") {
            m_shouldStop.store(true);
            m_commandCV.notify_one();
            break;
        }
        
        // Queue the command for the main thread to process
        {
            std::lock_guard<std::mutex> lock(m_commandMutex);
            m_commandQueue.push(signal);
        }
        m_commandCV.notify_one();
    }
}

void ChessEngine::_processCommand(const std::string& signal) {
    std::istringstream iss(signal);
    std::string command;
    iss >> command;

    std::string logError;
    std::string response;
    
    switch(_commandHit(command)){
        case UCICommand_T::INVALID:
            logError = "Invalid Command: " + command;
            break;
        
        case UCICommand_T::UCI:
            _printIdentity();
            _printOptions();

            response = "uciok";
            break;
        
        case UCICommand_T::QUIT:
            m_shouldStop.store(true);
            break;
        
        case UCICommand_T::ISREADY: 
            response = "readyok";
            break;
    
        case UCICommand_T::UCINEWGAME: // Reset to starting position
            m_fen = FEN_STARTING_POS;
            m_board = std::make_unique<Board>(m_fen);
            break;
        
        case UCICommand_T::POSITION: {
            std::string posType;
            iss >> posType;
            
            if (posType == "startpos") {
                m_fen = FEN_STARTING_POS;
                m_board = std::make_unique<Board>(m_fen);
                
                std::string movesKeyword;
                iss >> movesKeyword;
                if (movesKeyword == "moves") {
                    std::string move;
                    while (iss >> move) {
                        _makeUciMove(move);
                    }
                } else if (!movesKeyword.empty()){
                    logError = "Invalid Command: " + command;
                }
                
                // Log the final FEN after processing all moves
                std::string finalFen = m_board->getFenStr();
                std::string info = "FEN Generated: " + finalFen;
                _printInfo(info);
            } else if (posType == "fen") {
                std::string fenStr;
                std::string token;
                while (iss >> token && token != "moves") {
                    if (!fenStr.empty()) { fenStr += " "; }
                    fenStr += token;
                }
                
                m_fen = FENString(fenStr);
                m_board = std::make_unique<Board>(m_fen);
                
                if (token == "moves") {
                    std::string move;
                    while (iss >> move) {
                        _makeUciMove(move);
                    }
                }
                
                // Log the final FEN after processing all moves
                std::string finalFen = m_board->getFenStr();
                std::string info = "FEN Generated: " + finalFen;
                _printInfo(info);
            } else {
                logError = "Invalid Command: position lacks an argument";
            }
            break;
        }

        case UCICommand_T::GO: {
            // Check for ponder mode
            std::string param;
            bool isPonder = false;
            while (iss >> param) {
                if (param == "ponder") {
                    isPonder = true;
                    break;
                }
            }
            
            if (isPonder) {
                m_isPondering.store(true);
                // In ponder mode, we would start thinking but wait for ponderhit or stop
                // For now, just acknowledge ponder mode
            } else {
                // For 50-move rule, still make a move and let GUI handle draw detection
                // UCI engines typically don't claim draws themselves
                
                // Generate and return a random valid move
                std::string bestMove = _getRandomValidMove();
                if (!bestMove.empty()) {
                    response = "bestmove " + bestMove;
                } else {
                    response = "bestmove 0000";
                }
            }
            break;
        }
        case UCICommand_T::STOP:
            if (m_isPondering.load()) {
                m_isPondering.store(false);
                std::string bestMove = _getRandomValidMove();
                if (!bestMove.empty()) {
                    response = "bestmove " + bestMove;
                } else {
                    response = "bestmove 0000";
                }
            }
            break;
        case UCICommand_T::PONDERHIT:
            // Ponder hit - convert from pondering to actual search
            if (m_isPondering.load()) {
                m_isPondering.store(false);
                std::string bestMove = _getRandomValidMove();
                if (!bestMove.empty()) {
                    response = "bestmove " + bestMove;
                } else {
                    response =  "bestmove 0000";
                }
            }
            break;
        case UCICommand_T::SETOPTION: {
            std::string name, nameValue, value, valueValue;
            iss >> name >> nameValue >> value >> valueValue;
            // For now, just acknowledge - could store thread count etc.
            break;
        }
    }

    if(!logError.empty()){ _printInfo(logError); }
    if(!response.empty()){ _printResponse(response); }
}

void ChessEngine::_printResponse(const std::string& response) const {
    std::cout << response << std::endl;
}

UCICommand_T ChessEngine::_commandHit(const std::string& in) const {
    // This is because switch cant work on std::string but I still want the switch because its clean.
    if(in == "quit"){
        return UCICommand_T::QUIT;
    } else if (in == "isready"){
        return UCICommand_T::ISREADY;
    } else if (in == "ucinewgame"){
        return UCICommand_T::UCINEWGAME;
    } else if (in == "position"){
        return UCICommand_T::POSITION;
    } else if (in == "go"){
        return UCICommand_T::GO;
    } else if (in == "stop"){
        return UCICommand_T::STOP;
    } else if (in == "ponderhit"){
        return UCICommand_T::PONDERHIT;
    } else if (in == "setoption"){
        return UCICommand_T::SETOPTION;
    } else if (in == "uci"){
        return UCICommand_T::UCI;
    } else {
        return UCICommand_T::INVALID;
    }
}

// Actually is modifying the m_board states... TODO
Game_Status ChessEngine::isValidMove(MoveCoordsData move) {
    Square& from = m_board->getBoardAt(move.fromRow, move.fromCol);
    Square& to = m_board->getBoardAt(move.toRow, move.toCol);

    const Piece* p = m_board->getPieceAt(move.fromRow, move.fromCol);
    
    if(m_board->isLegalMove(from, to, p)){
        Game_Status gameStatus = m_board->moveTo(from, to);
        m_fen.setFen(m_board->getFenStr());
        return gameStatus;
    } else {
        return Game_Status::INVALID;
    }
}

Game_Status ChessEngine::isValidMove(MoveCoordsData move, Piece_T promotionPiece) {
    Square& from = m_board->getBoardAt(move.fromRow, move.fromCol);
    Square& to = m_board->getBoardAt(move.toRow, move.toCol);

    const Piece* p = m_board->getPieceAt(move.fromRow, move.fromCol);
    
    if(m_board->isLegalMove(from, to, p)){
        Game_Status gameStatus = m_board->moveTo(from, to, promotionPiece);
        m_fen.setFen(m_board->getFenStr());
        return gameStatus;
    } else {
        return Game_Status::INVALID;
    }
}

void ChessEngine::_printIdentity() const {
    std::string nameOutput = "id name " + engineID.name;
    std::string authorOutput = "id author " + engineID.author + "\n";
    
    std::cout << nameOutput << std::endl;
    std::cout << authorOutput << std::endl;
    
    _logOutput(nameOutput);
    _logOutput(authorOutput);
}

void ChessEngine::_printOptions() const {
    std::string optionOutput = "option name " + engineOptionNames.threads + " type spin default 1 min 1 max 1024";
    std::cout << optionOutput << std::endl;
    _logOutput(optionOutput);
}

void ChessEngine::_logOutput(const std::string& output) const {
    if (m_uciLog.is_open()) {
        m_uciLog << "Engine -> GUI: " << output << std::endl;
        m_uciLog.flush();
    }
}

// Add a type for more than just string TODO.
void ChessEngine::_printInfo(const std::string& info) const {
    std::string infoString = "info string " + info;
    std::cout << infoString << std::endl;
    _logOutput(infoString);
    
}

// pubic version
unsigned long int ChessEngine::perft(unsigned int depth) {
    return _perft(depth, true);
}

// recursive, greatly inefficient due to us making a ChessEngine obj each step.
// Multi threaded inconsistency? TODO
unsigned long int ChessEngine::_perft(unsigned int depth, bool showMoves) {
    if (depth == 0) {
        return 1;
    }
    
    if(!showMoves) {
        // For recursive calls, use the single-threaded version
        return _perftSingleThreaded(depth);
    }
    
    // Display detected core count
    unsigned int numCores = std::thread::hardware_concurrency();
    if (numCores == 0) {
        std::cout << "Hardware concurrency detection failed, using 4 cores as fallback" << std::endl;
        numCores = 4;
    } else {
        std::cout << "Detected " << numCores << " CPU cores" << std::endl;
    }
    std::cout << "Calculating...\n" << std::endl;
    
    // Collect all valid moves first
    // TODO THIS SHOULDN"T BE A CHESS ENGINE THING. should be a member of m_board in Board.
    std::vector<std::tuple<MoveCoordsData, std::string, Piece_T>> validMoves;
    
    // Generate all possible moves for the current player.
    // TODO, add states in Board to improve.
    for(size_t fromRow = 0; fromRow < MAX_ROWS; ++fromRow) {
        for(size_t fromCol = 0; fromCol < MAX_COLS; ++fromCol) {
            const Piece* piece = m_board->getPieceAt(fromRow, fromCol);
            if(!piece) {
                continue; // Skip empty squares
            }
            
            // Get the current active player fresh each iteration
            char activePlayer = m_fen.getActiveTurn();
            Color_T currentPlayerColor = (activePlayer == 'w') ? Color_T::WHITE : Color_T::BLACK;
            
            if(piece->getColor() != currentPlayerColor) {
                continue; // Skip opponent pieces
            }
            
            // Try all possible destination squares for this piece
            for(size_t toRow = 0; toRow < MAX_ROWS; ++toRow) {
                for(size_t toCol = 0; toCol < MAX_COLS; ++toCol) {
                    MoveCoordsData move = {fromRow, fromCol, toRow, toCol};
                    
                    try {
                        Square& from = m_board->getBoardAt(fromRow, fromCol);
                        Square& to = m_board->getBoardAt(toRow, toCol);
                        
                        // Just check if move is legal without making it
                        if(m_board->isLegalMove(from, to, piece)) {
                            // Convert coordinates to algebraic notation
                            size_t fromRowCopy = fromRow, fromColCopy = fromCol;
                            size_t toRowCopy = toRow, toColCopy = toCol;
                            std::string fromSquare = m_board->coordsToNotation(fromRowCopy, fromColCopy);
                            std::string toSquare = m_board->coordsToNotation(toRowCopy, toColCopy);
                            
                            // Check if this is a pawn promotion move
                            if(piece->getType() == Piece_T::PAWN && m_board->pawnCanPromote(to, piece->getColor())) {
                                // Generate 4 separate moves for each promotion piece
                                std::vector<Piece_T> promotionPieces = {Piece_T::QUEEN, Piece_T::ROOK, Piece_T::BISHOP, Piece_T::KNIGHT};
                                std::vector<char> promotionSuffixes = {'q', 'r', 'b', 'n'};
                                
                                for(size_t i = 0; i < promotionPieces.size(); ++i) {
                                    std::string moveNotation = fromSquare + toSquare + promotionSuffixes[i];
                                    validMoves.emplace_back(move, moveNotation, promotionPieces[i]);
                                }
                            } else {
                                std::string moveNotation = fromSquare + toSquare;
                                validMoves.emplace_back(move, moveNotation, Piece_T::QUEEN); // Default, not used for non-promotions
                            }
                        }
                    } catch (const std::exception& e) {
                        continue;
                    }
                }
            }
        }
    }
    
    // Process moves in parallel
    // Each initial move will get its own thread...
    std::vector<std::future<std::pair<std::string, unsigned long int>>> futures;
    std::string currentFen = m_fen.getFen();
    
    for(const auto& moveData : validMoves) {
        const auto& move = std::get<0>(moveData);
        const auto& notation = std::get<1>(moveData);
        const auto& promotionPiece = std::get<2>(moveData);
        
        futures.push_back(std::async(std::launch::async, [this, move, notation, promotionPiece, currentFen, depth]()-> std::pair<std::string, unsigned long int> {
            try {
                ChessEngine engine{FENString(currentFen)};
                
                // Check if this is a promotion move by looking at the notation
                Game_Status moveResult;
                if(notation.length() > 4 && (notation.back() == 'q' || notation.back() == 'r' || notation.back() == 'b' || notation.back() == 'n')) {
                    moveResult = engine.isValidMove(move, promotionPiece);
                } else {
                    moveResult = engine.isValidMove(move);
                }
                
                if(moveResult != Game_Status::INVALID) {
                    unsigned long int nodes = engine._perftSingleThreaded(depth - 1);
                    return {notation, nodes};
                }
            } catch (const std::exception& e) {}
            return {notation, 0};
        }));
    }
    
    // Collect results and output them in order
    unsigned long int totalNodes = 0;
    for(size_t i = 0; i < futures.size(); ++i) {
        auto result = futures[i].get();
        if(result.second > 0) {
            std::cout << result.first << ": " << result.second << std::endl;
            totalNodes += result.second;
        }
    }
    
    std::cout << std::endl << "Nodes searched: " << totalNodes << std::endl << std::flush;
    return totalNodes;
}

// Does the recursive function for the thread.
unsigned long int ChessEngine::_perftSingleThreaded(unsigned int depth) {
    if (depth == 0) {
        return 1;
    }
    
    unsigned long int totalNodes = 0;
    
    // Generate all possible moves for the current player
    for(size_t fromRow = 0; fromRow < MAX_ROWS; ++fromRow) {
        for(size_t fromCol = 0; fromCol < MAX_COLS; ++fromCol) {
            const Piece* piece = m_board->getPieceAt(fromRow, fromCol);
            if(!piece) {
                continue;
            }
            
            // Get the current active player fresh each iteration
            char activePlayer = m_fen.getActiveTurn();
            Color_T currentPlayerColor = (activePlayer == 'w') ? Color_T::WHITE : Color_T::BLACK;
            
            if(piece->getColor() != currentPlayerColor) {
                continue;
            }
            
            // Try all possible destination squares for this piece
            for(size_t toRow = 0; toRow < MAX_ROWS; ++toRow) {
                for(size_t toCol = 0; toCol < MAX_COLS; ++toCol) {
                    MoveCoordsData move = {fromRow, fromCol, toRow, toCol};
                    
                    try {
                        Square& from = m_board->getBoardAt(fromRow, fromCol);
                        Square& to = m_board->getBoardAt(toRow, toCol);
                        
                        // Just check if move is legal without making it
                        if(m_board->isLegalMove(from, to, piece)) {
                            // Check if this is a pawn promotion move
                            if(piece->getType() == Piece_T::PAWN && m_board->pawnCanPromote(to, piece->getColor())) {
                                // Generate 4 separate moves for each promotion piece
                                std::vector<Piece_T> promotionPieces = {Piece_T::QUEEN, Piece_T::ROOK, Piece_T::BISHOP, Piece_T::KNIGHT};
                                
                                for(Piece_T promotionPiece : promotionPieces) {
                                    if(depth == 1) {
                                        totalNodes++;
                                    } else {
                                        // For recursion, create new engine and make the move
                                        std::string currentFen = m_fen.getFen();
                                        ChessEngine recursiveEngine{FENString(currentFen)};
                                    
                                        Game_Status moveResult = recursiveEngine.isValidMove(move, promotionPiece);
                                        if(moveResult != Game_Status::INVALID) {
                                            totalNodes += recursiveEngine._perftSingleThreaded(depth - 1);
                                        }
                                    }
                                }
                            } else {
                                if(depth == 1) {
                                    totalNodes++;
                                } else {
                                    // For recursion, create new engine and make the move
                                    std::string currentFen = m_fen.getFen();
                                    ChessEngine recursiveEngine{FENString(currentFen)};
                                
                                    Game_Status moveResult = recursiveEngine.isValidMove(move);
                                    if(moveResult != Game_Status::INVALID) {
                                        totalNodes += recursiveEngine._perftSingleThreaded(depth - 1);
                                    }
                                }
                            }
                        }
                    } catch (const std::exception& e) {            
                        continue;
                    }
                }
            }
        }
    }
    return totalNodes;
}

std::string ChessEngine::getFenStr() const{
    return m_fen.getFen();
}