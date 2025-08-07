#include "Knight.h"
#include "Board.h" // To verify pawn moves need to know board state

Knight::Knight(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {}

bool Knight::isValidMove(const Square& toSquare) const {
    return getBoard().validKnightMove(getSquarePosition(), toSquare, getColor());
} 

void Knight::updateAttacking() {
    _clearAttacking();
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        for(size_t col = 0; col < MAX_COLS; ++col) {
            const Square& to = getBoard().getBoardAt(row, col);
            if(to.isOccupied()){
                const Piece* attackedPiece = getBoard().getPieceAt(row, col);
                if(isValidMove(to)){
                    addToAttacking(attackedPiece);
                }
            }
        }
    }
}