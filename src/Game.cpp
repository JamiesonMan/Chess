#include "Game.h"
#include <iostream>
#include <sstream>

Game::Game() : board{} {};

void Game::start(){
    std::string error{};
    std::string square1{};
    std::string square2{};
    unsigned int fromRow{0}, fromCol{0}, toRow{0}, toCol{0};

    _loop(square1, square2, fromRow, fromCol, toRow, toCol);
}

void Game::_loop(std::string square1, std::string square2, unsigned int fromRow, unsigned int fromCol, unsigned int toRow, unsigned int toCol) {

    std::ostringstream error{};

    while(true){
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
                std::cout << "\n\t Square to: ";
                std::getline(std::cin, square2);
                std::cout << std::endl;
                this->board.notationToCoords(square1, fromRow, fromCol);
                this->board.notationToCoords(square2, toRow, toCol);
            } catch(const std::exception& e){
                error << e.what() << "\n" << std::endl;
            }
        } while (fromRow > 7 || fromCol > 7 || toRow > 7 || fromRow > 7);

        try{
            Square& from = this->board.getBoardAt(fromRow, fromCol);
            Square& to = this->board.getBoardAt(toRow, toCol);

            this->board.moveTo(from, to);
        } catch(const std::exception& e){
            error << e.what() << "\n" << std::endl;
        }
    }
}