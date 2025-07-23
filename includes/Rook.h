#pragma once
#include "Piece.h"
#include "Square.h"
#include "Colors.h"

class Rook final : public Piece {

    public:
        Rook(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef); // Same as base but need to set has moved.

        virtual bool isValidMove(const Square& otherSquare) const override;
};