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
        FENString fen{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};
        Game game{fen};
        game.test();
    } else if (option == 1) {
        Game game;
        game.start();
    }

    return 0;
}