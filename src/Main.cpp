#include <iostream>
#include "Game.h"

int main(){

    unsigned int option;
    do{
        std::cout << "Welcome to chess, \n";
        std::cout << "\tSelect an option: \n";
        std::cout << "\t1. Start Game\n";
        std::cout << "\t2. Exit\n";
        try{   
            std::cout << "Option: ";
            std::cin >> option;
            std::cin.ignore();
        } catch (const std::exception& e){
            std::cout << e.what() << std::endl;
            option = 3;
        }
        
    } while (option > 2 || option == 0);
    
    if(option == 2){
        exit(0);
    } else if (option == 1) {
        Game game;
        game.start();
    }

    return 0;
}