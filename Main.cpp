#include <iostream>
#include "Board.h"

void startGame(Board&);

int main(){

    Board board;

    
    unsigned int option;
    do{
        std::cout << "Welcome to chess, \n";
        std::cout << "\tSelect an option: \n";
        std::cout << "\t1. Start Game\n";
        std::cout << "\t2. Exit\n";
        try{
            std::cin >> option;
            std::cin.ignore(); // Clear the newline from input buffer
        } catch (const std::exception& e){
            std::cout << e.what() << std::endl;
            option = 3;
        }
        
    } while (option > 2 || option == 0);
    
    if(option == 2){
        exit(0);
    } else if (option == 1) {
        startGame(board);
    }

    return 0;
}

void startGame(Board& b){
    std::string sqr1;
    std::string sqr2;
    unsigned int fromRow, fromCol, toRow, toCol;

    std::cout << b << std::endl;

    do{
        try{
            std::cout << "\n\nSelect A Square to Move:\n";
            std::cout << "\tSquare from: ";
            std::getline(std::cin, sqr1);
            std::cout << "\n\t Square to: ";
            std::getline(std::cin, sqr2);
            std::cout << std::endl;
            b.notationToCoords(sqr1, fromRow, fromCol);
            b.notationToCoords(sqr2, toRow, toCol);
        } catch(const std::exception& e){
            std::cout << e.what() << "\n" << std::endl;
        }
    } while (fromRow > 7 || fromCol > 7 || toRow > 7 || fromRow > 7);


    try{
        Square& from = b.getBoardAt(fromRow, fromCol);
        Square& to = b.getBoardAt(toRow, toCol);

        b.moveTo(from, to);
    } catch(const std::exception& e){
        std::cout << e.what() << "\n" << std::endl;
    }
    
    startGame(b);

}