#include "Bishop.h"
#include "Board.h" // To verify pawn moves need to know board state

Bishop::Bishop(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {}

bool Bishop::isValidMove(const Square& toSquare) const {
    return getBoard().validBishopMove(getSquarePosition(), toSquare, getColor());
} 