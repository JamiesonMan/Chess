#include <iostream>
#include "Game.h"

int main(){

    unsigned int option;
    do{
        std::cout << "Welcome to chess, \n";
        std::cout << "\tSelect an option: \n";
        std::cout << "\t1. Start Game\n";
        std::cout << "\t2. Test Game\n";
        std::cout << "\t3. Exit\n";
        try{   
            std::cout << "Option: ";
            std::cin >> option;
            std::cin.ignore();
        } catch (const std::exception& e){
            std::cout << e.what() << std::endl;
            option = 4;
        }
        
    } while (option > 3 || option == 0);
    if(option == 3){
        exit(0);
    } else if(option == 2){
        std::string fenInput;
        std::cout << "Enter a fen or type 's' for starting fen: ";
        std::getline(std::cin, fenInput);
        if(fenInput == "s"){
            fenInput = FEN_STARTING_POS.getFen();
        }
        try{
            FENString fen{fenInput};
            Game game{fen};
            game.test();
        } catch (const std::exception& e){
            std::cout << e.what() << std::endl;
            return 0;
        }
    } else if (option == 1) {
        std::string fenInput;
        std::cout << "Enter a fen or type 's' for starting fen: ";
        std::getline(std::cin, fenInput);
        if(fenInput == "s"){
            fenInput = FEN_STARTING_POS.getFen();
        }
        try{
            FENString fen{fenInput};
            Game game{fen};
            game.start();
        } catch (const std::exception& e){
            std::cout << e.what() << std::endl;
            return 0;
        }
    }

    return 0;
}