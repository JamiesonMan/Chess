#include <iostream>
#include "Board.h"

/* Testing two step logic for pawns. */
int main(){
    bool t1;

    const std::array<std::array<char, Board::MAX_COLS>, Board::MAX_ROWS> boardInit1 = {{
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
        }};

    Board b{boardInit1};

    const auto& board = b.getBoard();

    std::cout << b << std::endl;
    
    int numberOfTrue = 0;
    int numberOfFalse = 0;
    // Loop through every position of board (64)
    for(size_t row = 0; row < Board::MAX_ROWS; row++){
        for(size_t col = 0; col < Board::MAX_COLS; col++){

            // Get a pointer to current piece. nullptr if no piece at this location.
            const Piece* p = b.getPieceAt(row, col);
            if(p){ // If there is a piece.
                if(p->getType() == Piece_T::PAWN){ // If it's of type PAWN.
                    const Square& fromSquare = p->getSquarePosition();
                    
                    // Loop through entire board.
                    for(size_t rowI = 0; rowI < Board::MAX_ROWS; rowI++){
                        for(size_t colI = 0; colI < Board::MAX_COLS; colI++){
                            const Square& toSquare = board[rowI][colI];
                            
                            bool validMove = p->isValidMove(toSquare); // Determine if this pawn has a valid move.

                            if(validMove){
                                numberOfTrue++;
                            } else {
                                numberOfFalse++;
                            }
                        }
                    }
                }
            }
        }
    }
    
    std::cout << "Total Impossible Moves: " << numberOfFalse << "\n";
    std::cout << "Total Possible Moves: " << numberOfTrue << std::endl;

    if(numberOfTrue == 16) {
        t1 = true;
    } else {
        t1 = false;
    }

    if(t1){
        return 0;
    } else {
        return 1;
    }
}