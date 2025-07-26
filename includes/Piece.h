#pragma once
#include "Square.h" // reference this piece's position.
#include "Colors.h" // Color_T - White and Black.
#include <string>
#include <memory>
#include <vector>

class Board;

enum class Piece_T : unsigned int { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}; // values correspond to the numeric value of that piece, KING is invaluable.

// abstract class - not an initializable object.
class Piece {
    public:
        Piece(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef);
        virtual ~Piece() = default;

        Piece_T getType() const;
        void setType(Piece_T);

        Color_T getColor() const;
        void setColor(Color_T);

        const std::vector<const Piece*>& getAttacking() const;

        void addToAttacking(const Piece*);
        void deleteFromAttacking(size_t indexToErase);

        static std::string typeToString(Piece_T type);
    
        const Board& getBoard() const;

        const Square& getSquarePosition() const;
        void setSquarePosition(const Square& newPosition);

        unsigned int getValue() const; // return enum class value for the piece type.

        virtual bool isValidMove(const Square& toSquare) const = 0; // pure virtual. Based on derived class implementation.
        virtual void updateAttacking() = 0;
        virtual std::string toString() const; // Interpret the Piece_T in their own implementation.

        void _clearAttacking();

    private:
        Piece_T type;
        Color_T color;
        const Square* positionRef;
        const Board& boardRef;

        std::vector<const Piece*> attacking;
};