#pragma once
#include "Piece.h"
#include "Square.h"

class Queen final : public Piece {

    public:
        Queen(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef); // Same as base but need to set has moved.

        virtual bool isValidMove(const Square& otherSquare) const override;
        virtual void updateAttacking() override;
};