#include "Pawn.h"
#include "Board.h" // To verify pawn moves need to know board state
#include <string>
#include <sstream>
#include <iostream>

Pawn::Pawn(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {
        setHasMoved(false);
    }

void Pawn::setHasMoved(bool newHasMoved){
    hasMoved = newHasMoved;
}

bool Pawn::getHasMoved() const { return hasMoved; }

bool Pawn::isValidMove(const Square& toSquare) const {
    return getBoard().canPawnMoveTo(getSquarePosition(), toSquare, getColor(), getHasMoved());
} 
 
std::string Pawn::toString() const {
    std::ostringstream output;
    output << "Piece: Pawn\n";
    output << "Color: " << colorToString(this->getColor());
    output << "\nCurrent Square: " << this->getSquarePosition(); // Overloaded.
    output << "\nHas Moved: " << std::boolalpha << this->getHasMoved();

    return output.str();
}