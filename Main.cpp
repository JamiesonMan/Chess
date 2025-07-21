#include <iostream>
#include "Board.h"

int main(){
    
    Board b;
    
    const auto& board = b.getBoard();

    std::cout << b << std::endl;
    
    int numberOfTrue = 0;

    for(size_t row = 0; row < Board::MAX_ROWS; row++){
        for(size_t col = 0; col < Board::MAX_COLS; col++){

            const Piece* p = b.getPieceAt(row, col);
            if(p){
                if(p->getType() == Piece_T::PAWN){
                    const Square& fromSquare = p->getSquarePosition();
                    std::cout << "Testing pawn at " << fromSquare << " (color: " << (p->getColor() == Color_T::WHITE ? "WHITE" : "BLACK") << ")" << std::endl;
                    for(size_t rowI = 0; rowI < Board::MAX_ROWS; rowI++){
                        for(size_t colI = 0; colI < Board::MAX_COLS; colI++){
                            const Square& toSquare = (*board)[rowI][colI];

                            bool validMove = p->isValidMove(toSquare);

                            if(validMove){
                                std::cout << "Valid move: " << fromSquare << " to " << toSquare << std::endl;
                                numberOfTrue++;
                            }
                        }
                    }
                }
            }
        }
    }

    std::cout << "Total Possible Moves: " << numberOfTrue;

    return 0;
}