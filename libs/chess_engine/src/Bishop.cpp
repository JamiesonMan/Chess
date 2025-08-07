#include "Bishop.h"
#include "Board.h" // To verify pawn moves need to know board state

Bishop::Bishop(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {}

bool Bishop::isValidMove(const Square& toSquare) const {
    return getBoard().validBishopMove(getSquarePosition(), toSquare, getColor());
} 

void Bishop::updateAttacking() {
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