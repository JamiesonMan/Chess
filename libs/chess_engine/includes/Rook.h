#pragma once
#include "Piece.h"
#include "Square.h"
#include "Colors.h"

class Rook final : public Piece {

    public:
        Rook(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef); // Same as base but need to set has moved.

        void setHasMoved(bool);
        bool getHasMoved() const;

        bool getRookLong() const;
        void setRookLong(bool);

        bool getRookShort() const;
        void setRookShort(bool);

        virtual bool isValidMove(const Square& otherSquare) const override;
        virtual void updateAttacking() override;
    
    private:
        bool hasMoved;
        bool rookShort;
        bool rookLong;

};