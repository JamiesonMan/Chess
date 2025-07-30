#include "Game.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

Game::Game() : turn{Color_T::WHITE}, board{} {};

void Game::_flipTurn() {
    if(turn == Color_T::WHITE){
        turn = Color_T::BLACK;
    } else {
        turn = Color_T::WHITE;
    }
}

void Game::_reset() {
    // Reset board to starting position
    board = Board();
    // Reset turn to white
    turn = Color_T::WHITE;
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
    std::string error{};
    std::string square1{};
    std::string square2{};
    unsigned int fromRow{0}, fromCol{0}, toRow{0}, toCol{0};

    _loop(square1, square2, fromRow, fromCol, toRow, toCol);
}

void Game::_loop(std::string square1, std::string square2, unsigned int fromRow, unsigned int fromCol, unsigned int toRow, unsigned int toCol) {

    std::ostringstream error{};
    bool gameActive = true;
    
    while(gameActive){
        square1.clear();
        square2.clear();
        fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
        
        std::system("clear");

        std::cout << this->board << std::endl;

        do{
            try{
                std::cout << error.str() << std::endl;
                error.str(""); // reset

                std::cout << "Select A Square to Move:\n";
                std::cout << "\tSquare from: ";
                std::getline(std::cin, square1);
                this->board.notationToCoords(square1, fromRow, fromCol);
                std::cout << "\n\t Square to: ";
                std::getline(std::cin, square2);
                std::cout << std::endl;
                this->board.notationToCoords(square2, toRow, toCol);
            } catch(const std::exception& e){
                error << e.what() << std::endl;
                square1.clear();
                square2.clear();
                fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
            }
        } while (fromRow > 7 || fromCol > 7 || toRow > 7 || fromRow > 7);

        try{
            Square& from = this->board.getBoardAt(fromRow, fromCol);
            Square& to = this->board.getBoardAt(toRow, toCol);
            
            const Piece* p = board.getPieceAt(fromRow, fromCol);
            if(p){
                if(p->getColor() != turn){
                    std::ostringstream errorTurn;
                    errorTurn << "It is " << colorToString(turn) << "'s turn!" << std::endl;
                    throw std::invalid_argument(errorTurn.str());
                }
            }

            Board::Game_Status STATUS = this->board.moveTo(from, to);
            
            // Only flip turn AFTER successful move
            _flipTurn();

            switch(STATUS){
                case Board::Game_Status::DRAW_END:
                    std::system("clear");
                    std::cout << this->board << std::endl;
                    std::cout << "DRAW" << std::endl;
                    if(_askPlayAgain()) {
                        _reset();
                        // Continue to next game loop iteration
                    } else {
                        std::cout << "Thanks for playing!" << std::endl;
                        gameActive = false;
                    }
                    break;
                
                case Board::Game_Status::CHECKMATE_END: {
                    std::system("clear");
                    std::cout << this->board << std::endl;
                    const Piece* p = board.getPieceAt(toRow, toCol);
                    Color_T winningColor = p->getColor();
                    std::cout << "CHECKMATE! " << colorToString(winningColor) << " Wins!" << std::endl;
                    if(_askPlayAgain()) {
                        _reset();
                        // Continue to next game loop iteration
                    } else {
                        gameActive = false;
                    }
                    break;
                }
                    
                case Board::Game_Status::IN_CHECK: {
                    const Piece* p = board.getPieceAt(toRow, toCol);
                    if(p->getColor() == Color_T::BLACK){
                        error << "White is in check!" << std::endl; 
                    } else {
                        error << "Black is in check!" << std::endl; 
                    }
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