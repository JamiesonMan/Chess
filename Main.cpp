#include <iostream>
#include "Board.h"

int main(){
    
    Board b;
    
    const auto& board = b.getBoard();

    b.printBoard();
    std::cout << b << std::endl;

    for(size_t row = 0; row < 8; row++){
        for(size_t col = 0; col < 8; col++){
            const Square& squareRef = (*board)[row][col];
            std::cout << squareRef;
            std::cout << "\nOccupied: " << std::boolalpha << squareRef.isOccupied();
            std::cout << "\nSquare Color: " << colorToString(squareRef.getSquareColor()) << "\n" << std::endl;
        }
    }
    
    
    
    for(size_t row = 0; row < 8; row++){
        for(size_t col = 0; col < 8; col++){
            Piece* piece = b.getPieceAt(row, col); // returns nullptr if no piece.
            if(piece) {
                std::cout << piece->getSquarePosition() << std::endl;
            } else {
                std::cout << "Empty square at row " << row << ", col " << col << std::endl;
            }
        }
    }
    

    return 0;
}