#pragma once
#include "Piece.h"
#include "Square.h"
#include "Colors.h"
#include <string>

class Pawn final : public Piece {

    public:
        Pawn(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef); // Same as base but need to set has moved.

        void setHasMoved(bool);
        bool getHasMoved() const;

        virtual bool isValidMove(const Square& otherSquare) const override;
        virtual std::string toString() const override;

    private:
        bool _isValidTwoStepMove(const Square& otherSquare) const;

        bool hasMoved;
};