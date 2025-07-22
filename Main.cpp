#include <iostream>
#include "Board.h"

int main(){
    
    Board b;

    try{
        Square& sqr = b.getBoardAt(-1, -1);
    } catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
    
    std::cout << "YOU DIDNT EXIT";
    
    return 0;
}