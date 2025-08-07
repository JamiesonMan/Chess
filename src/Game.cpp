#include "Game.h"
#include "chess_engine/ColorUtil.h"
#include "chess_engine/Types.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

Game::Game() : m_currentFEN{FEN_STARTING_POS}, m_turn{Color_T::WHITE}, m_dboard{}, m_gameActive{false} {};

Game::Game(const FENString& fen) : m_currentFEN{fen}, m_turn{Color_T::WHITE}, m_dboard{fen}, m_gameActive{false} {};

void Game::_flipTurn() {
    if(m_turn == Color_T::WHITE){
        m_turn = Color_T::BLACK;
    } else {
        m_turn = Color_T::WHITE;
    }
}

void Game::_reset() {
    // Reset to starting position
    m_currentFEN = FENString{FEN_STARTING_POS};
    m_dboard = DBoard();
    m_turn = Color_T::WHITE;
}

bool Game::_askPlayAgain() {
    std::string response;
    while(true) {
        std::cout << "\nWould you like to play again? (y/n): ";
        std::getline(std::cin, response);
        
        if(response == "y" || response == "Y" || response == "yes" || response == "YES") {
            return true;
        } else if(response == "n" || response == "N" || response == "no" || response == "NO") {
            return false;
        } else {
            std::cout << "Please enter 'y' for yes or 'n' for no." << std::endl;
        }
    }
}

void Game::start(){
    m_gameActive = true;
    std::ostringstream error{};
    
    while(m_gameActive) {
        std::string square1{};
        std::string square2{};
        size_t fromRow{0}, fromCol{0}, toRow{0}, toCol{0};

        // Get user input for move
        _getUserInput(square1, square2, fromRow, fromCol, toRow, toCol, error);

        try{
            MoveCoordsData move{fromRow, fromCol, toRow, toCol};

            DPiece_T p = m_dboard.getBoardAt(fromRow, fromCol);
            Color_T pieceColor = DPieceColor(p);
            if(p != DPiece_T::NULL_PIECE){
                if(pieceColor != m_turn){
                    std::ostringstream errorTurn;
                    errorTurn << "It is " << colorToString(m_turn) << "'s turn!" << std::endl;
                    throw std::invalid_argument(errorTurn.str());
                }
            }

            // Create engine with current FEN state for move validation
            ChessEngine engine{m_currentFEN};
            Game_Status STATUS = engine.isValidMove(move);

            switch(STATUS){
                case Game_Status::INVALID:
                    throw std::invalid_argument("Invalid Move!");

                case Game_Status::DRAW_END:
                    this->m_dboard.moveTo(move);
                    updateFENAfterMove(move);
                    _flipTurn();

                    std::system("clear");
                    std::cout << this->m_dboard << std::endl;
                    std::cout << "DRAW" << std::endl;
                    if(_askPlayAgain()) {
                        _reset();
                        // Continue to next game loop iteration
                    } else {
                        std::cout << "Thanks for playing!" << std::endl;
                        m_gameActive = false;
                    }
                    break;
                
                case Game_Status::CHECKMATE_END: {
                    this->m_dboard.moveTo(move);
                    updateFENAfterMove(move);
                    _flipTurn();

                    std::system("clear");
                    std::cout << this->m_dboard << std::endl;
                    
                    Color_T winningColor = pieceColor;
                    std::cout << "CHECKMATE! " << colorToString(winningColor) << " Wins!" << std::endl;
                    if(_askPlayAgain()) {
                        _reset();
                        // Continue to next game loop iteration
                    } else {
                        m_gameActive = false;
                    }
                    break;
                }
                    
                case Game_Status::IN_CHECK: {
                    if(pieceColor == Color_T::BLACK){
                        error << "White is in check!" << std::endl; 
                    } else {
                        error << "Black is in check!" << std::endl; 
                    }
                    break;
                }

                case Game_Status::CONTINUE: {
                    this->m_dboard.moveTo(move);
                    updateFENAfterMove(move);
                    _flipTurn();

                    std::system("clear");
                    std::cout << this->m_dboard << std::endl;
                    break;
                }

                default:
                    break;
            }
        } catch(const std::exception& e){
            error << e.what() << std::endl;
        }
    }
}

void Game::_getUserInput(std::string& square1, std::string& square2, size_t& fromRow, size_t& fromCol, size_t& toRow, size_t& toCol, std::ostringstream& error) {
    
    square1.clear();
    square2.clear();
    fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
    
    std::system("clear");
    std::cout << this->m_dboard << std::endl;

    do{
        try{
            std::cout << error.str() << std::endl;
            error.str(""); // reset

            std::cout << "Select A Square to Move:\n";
            std::cout << "\tSquare from: ";
            std::getline(std::cin, square1);
            this->m_dboard.notationToCoords(square1, fromRow, fromCol);
            std::cout << "\n\t Square to: ";
            std::getline(std::cin, square2);
            std::cout << std::endl;
            this->m_dboard.notationToCoords(square2, toRow, toCol);
        } catch(const std::exception& e){
            error << e.what() << std::endl;
            square1.clear();
            square2.clear();
            fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
        }
    } while (fromRow >= MAX_ROWS || fromCol >= MAX_COLS || toRow >= MAX_ROWS || toCol >= MAX_COLS);
}

// FEN management functions
const FENString& Game::getCurrentFEN() const {
    return m_currentFEN;
}

void Game::setCurrentFEN(const FENString& fen) {
    m_currentFEN = fen;
    // Update display board to match new FEN
    m_dboard = DBoard{fen};
}

void Game::updateFENAfterMove(const MoveCoordsData& move) {
    // Reconstruct FEN string from current DBoard state
    std::string boardString = "";
    
    // 1. Board position - traverse DBoard and convert to FEN notation
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        size_t emptySquares = 0;
        
        for(size_t col = 0; col < MAX_COLS; ++col) {
            DPiece_T piece = m_dboard.getBoardAt(row, col);
            
            if(piece == DPiece_T::NULL_PIECE) {
                ++emptySquares;
            } else {
                // Add any accumulated empty squares
                if(emptySquares > 0) {
                    boardString += std::to_string(emptySquares);
                    emptySquares = 0;
                }
                
                // Add piece character
                switch(piece) {
                    case DPiece_T::B_PAWN:   boardString += 'p'; break;
                    case DPiece_T::B_ROOK:   boardString += 'r'; break;
                    case DPiece_T::B_KNIGHT: boardString += 'n'; break;
                    case DPiece_T::B_BISHOP: boardString += 'b'; break;
                    case DPiece_T::B_QUEEN:  boardString += 'q'; break;
                    case DPiece_T::B_KING:   boardString += 'k'; break;
                    case DPiece_T::W_PAWN:   boardString += 'P'; break;
                    case DPiece_T::W_ROOK:   boardString += 'R'; break;
                    case DPiece_T::W_KNIGHT: boardString += 'N'; break;
                    case DPiece_T::W_BISHOP: boardString += 'B'; break;
                    case DPiece_T::W_QUEEN:  boardString += 'Q'; break;
                    case DPiece_T::W_KING:   boardString += 'K'; break;
                    default: break;
                }
            }
        }
        
        // Add any remaining empty squares at end of rank
        if(emptySquares > 0) {
            boardString += std::to_string(emptySquares);
        }
        
        // Add rank separator (except for last rank)
        if(row < MAX_ROWS - 1) {
            boardString += '/';
        }
    }
    
    // 2. Active color (current turn)
    char activeColor = (m_turn == Color_T::WHITE) ? 'w' : 'b';
    
    // 3. Castling rights (simplified - assume all rights lost after any move for now)
    // TODO: Implement proper castling rights tracking
    std::string castlingRights = "-";
    
    // 4. En passant target square (simplified - no en passant tracking for now)
    // TODO: Implement proper en passant tracking
    std::string enPassantTarget = "-";
    
    // 5. Halfmove clock (simplified - always 0 for now)
    // TODO: Implement proper halfmove clock tracking
    std::string halfmoveClock = "0";
    
    // 6. Fullmove number (simplified - always 1 for now)  
    // TODO: Implement proper fullmove number tracking
    std::string fullmoveNumber = "1";
    
    // Construct complete FEN string
    std::string newFENString = boardString + " " + activeColor + " " + 
                              castlingRights + " " + enPassantTarget + " " + 
                              halfmoveClock + " " + fullmoveNumber;
    
    // Update current FEN
    m_currentFEN = FENString{newFENString};
}