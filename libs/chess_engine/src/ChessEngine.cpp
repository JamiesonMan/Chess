#include "chess_engine/ChessEngine.h"
#include "Board.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <future>

ChessEngine::ChessEngine(FENString fen) : m_fen{fen}, m_board{std::make_unique<Board>(fen)} {}
ChessEngine::~ChessEngine() = default;

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

unsigned long int ChessEngine::perft(unsigned int depth) {
    return _perft(depth, true);
}

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
    std::cout << "Starting...\n" << std::endl;
    
    // Collect all valid moves first
    std::vector<std::tuple<MoveCoordsData, std::string, Piece_T>> validMoves;
    
    // Generate all possible moves for the current player
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
    std::vector<std::future<std::pair<std::string, unsigned long int>>> futures;
    std::string currentFen = m_fen.getFen();
    
    for(const auto& moveData : validMoves) {
        const auto& move = std::get<0>(moveData);
        const auto& notation = std::get<1>(moveData);
        const auto& promotionPiece = std::get<2>(moveData);
        
        futures.push_back(std::async(std::launch::async, [this, move, notation, promotionPiece, currentFen, depth]() -> std::pair<std::string, unsigned long int> {
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
