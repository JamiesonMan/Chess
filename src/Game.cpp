#include "Game.h"
#include "chess_engine/ColorUtil.h"
#include "chess_engine/Types.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#endif

static void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

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
            if(STATUS != Game_Status::INVALID){
                updateFENAfterMove(engine.getFenStr());
            }
            switch(STATUS){
                case Game_Status::INVALID:
                    throw std::invalid_argument("Invalid Move!");

                case Game_Status::DRAW_END:
                    this->m_dboard.updateBoard(getCurrentFEN().getBoardStr());
                    _flipTurn();

                    clearScreen();
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
                    this->m_dboard.updateBoard(getCurrentFEN().getBoardStr());
                    _flipTurn();

                    clearScreen();
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
                    this->m_dboard.updateBoard(getCurrentFEN().getBoardStr());
                    _flipTurn();

                    clearScreen();
                    std::cout << this->m_dboard << std::endl;

                    if(pieceColor == Color_T::BLACK){
                        error << "White is in check!" << std::endl; 
                    } else {
                        error << "Black is in check!" << std::endl; 
                    }
                    break;
                }

                case Game_Status::CONTINUE: {
                    this->m_dboard.updateBoard(getCurrentFEN().getBoardStr());
                    _flipTurn();

                    clearScreen();
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
    
    clearScreen();
    std::cout << this->m_dboard << std::endl;
    std::cout << this->m_currentFEN.getFen() << std::endl;

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

void Game::test() {
    unsigned int depth{0};
    std::cout << "Enter a perft val: ";
    std::cin >> depth;

    ChessEngine engine{m_currentFEN};
    engine.perft(depth);
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

void Game::updateFENAfterMove(const std::string& newFen) {
    m_currentFEN = FENString{newFen};
}