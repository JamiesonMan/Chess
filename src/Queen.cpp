#include "Queen.h"
#include "Board.h" // To verify pawn moves need to know board state

Queen::Queen(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {}

bool Queen::isValidMove(const Square& toSquare) const {
    return getBoard().validQueenMove(getSquarePosition(), toSquare, getColor());
} 