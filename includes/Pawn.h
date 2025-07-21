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
        struct MoveCoordsData {
            unsigned int fromRow, fromCol;
            unsigned int toRow, toCol;
        };
        bool _isValidTwoStepMove(const Square& thisSquare, Color_T thisColor, const Board& boardRef, const MoveCoordsData& data, bool otherSquareOccupied) const;
        bool _isTwoStepMove(const Square& thisSquare, Color_T thisColor, const Board& boardRef, const MoveCoordsData& data, bool otherSquareOccupied) const;

        bool hasMoved;
};