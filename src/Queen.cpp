#include "Queen.h"
#include "Board.h" // To verify pawn moves need to know board state

Queen::Queen(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {}

bool Queen::isValidMove(const Square& toSquare) const {
    return getBoard().validQueenMove(getSquarePosition(), toSquare, getColor());
} 

void Queen::updateAttacking() {
    _clearAttacking();
    for(size_t row = 0; row < Board::MAX_ROWS; ++row) {
        for(size_t col = 0; col < Board::MAX_COLS; ++col) {
            const Square& to = getBoard().getBoardAt(row, col);
            if(to.isOccupied()){
                const Piece* attackedPiece = getBoard().getPieceAt(row, col);
                if(!attackedPiece){continue;}
                if(isValidMove(to)){
                    addToAttacking(attackedPiece);
                }
            }
        }
    }
}