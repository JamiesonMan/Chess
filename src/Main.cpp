#include <iostream>
#include "Game.h"
#include "chess_engine/ChessEngine.h"

int main(int argc, char* argv[]){
    // Check for menu mode command line argument
    if (argc > 1 && std::string(argv[1]) == "dev") {
        // Show interactive menu
    } else {
        // Default: Start in UCI mode for GUI compatibility
        ChessEngine engine{FEN_STARTING_POS};
        engine.uciStart();
        return 0;
    }

    unsigned int option;
    do{
        std::cout << "Welcome to chess, \n";
        std::cout << "\tSelect an option: \n";
        std::cout << "\t1. Start Game\n";
        std::cout << "\t2. Perft\n";
        std::cout << "\t3. UCI Protocol Test\n";
        std::cout << "\t4. Exit\n";
        try{   
            std::cout << "Option: ";
            std::cin >> option;
            std::cin.ignore();
        } catch (const std::exception& e){
            std::cout << e.what() << std::endl;
            option = 5;
        }
        
    } while (option > 4 || option == 0);
    if(option == 4){
        exit(0);
    } else if (option == 3){
        ChessEngine engine{FEN_STARTING_POS};
        engine.uciStart();
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