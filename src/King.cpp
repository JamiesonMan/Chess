#include "King.h"
#include "Board.h" // To verify pawn moves need to know board state

King::King(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {}

bool King::isValidMove(const Square& toSquare) const {
    return getBoard().validKingMove(getSquarePosition(), toSquare, getColor());
} 