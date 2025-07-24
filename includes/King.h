#pragma once
#include "Piece.h"
#include "Square.h"
#include "Colors.h"

class King final : public Piece {

    public:
        King(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef); // Same as base but need to set has moved.

        virtual bool isValidMove(const Square& otherSquare) const override;
};