#include "chess_engine/ChessEngine.h"
#include "Board.h"
#include <iostream>

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

unsigned long int ChessEngine::perft(unsigned int depth) {
    if (depth == 0) {
        return 1;
    }
    
    unsigned long int nodes = 0;
    
    // Save current state
    std::string currentFen = m_fen.getFen();
    char activePlayer = m_fen.getActiveTurn();
    Color_T currentPlayerColor = (activePlayer == 'w') ? Color_T::WHITE : Color_T::BLACK;
    
    // Generate all possible moves for the current player
    for(size_t fromRow = 0; fromRow < MAX_ROWS; ++fromRow) {
        for(size_t fromCol = 0; fromCol < MAX_COLS; ++fromCol) {
            const Piece* piece = m_board->getPieceAt(fromRow, fromCol);
            if(!piece || piece->getColor() != currentPlayerColor) {
                continue; // Skip empty squares or opponent pieces
            }
            
            // Try all possible destination squares for this piece
            for(size_t toRow = 0; toRow < MAX_ROWS; ++toRow) {
                for(size_t toCol = 0; toCol < MAX_COLS; ++toCol) {
                    
                    MoveCoordsData move = {fromRow, fromCol, toRow, toCol};
                    // Always create a fresh engine for each move to avoid board state corruption
                    ChessEngine moveEngine{FENString(currentFen)};
                    
                    try {
                        Game_Status moveResult = moveEngine.isValidMove(move);
                        if(moveResult != Game_Status::INVALID) {

                            std::cout << nodes << ". Valid Move: \n\tFROM: " << move.fromRow << ", " << move.fromCol << "\n\tTO: " << move.toRow << ", " << move.toCol << std::endl;
                            if(depth == 1) {
                                ++nodes; // At depth 1, just count valid moves
                            } else {
                                // Create a fresh engine from the updated FEN for recursion
                                std::string updatedFen = moveEngine.getFenStr();
                                ChessEngine recursiveEngine{FENString(updatedFen)};
                                nodes += recursiveEngine.perft(depth - 1);
                            }
                        }
                    } catch (const std::exception& e) {
                        std::string error{e.what()};
                        if(error != "Invalid Move!"){
                            std::cout << error << std::endl;
                        }
                        continue;
                    }
                }
            }
        }
    }
    
    return nodes;
}

std::string ChessEngine::getFenStr() const{
    return m_fen.getFen();
}
