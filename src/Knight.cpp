#include "Knight.h"
#include "Board.h" // To verify pawn moves need to know board state

Knight::Knight(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {}

bool Knight::isValidMove(const Square& toSquare) const {
    return getBoard().validKnightMove(getSquarePosition(), toSquare, getColor());
} 