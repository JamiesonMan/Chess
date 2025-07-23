#include "Rook.h"
#include "Board.h" // To verify pawn moves need to know board state

Rook::Rook(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {}

bool Rook::isValidMove(const Square& toSquare) const {
    return getBoard().validRookMove(getSquarePosition(), toSquare, getColor());
} 