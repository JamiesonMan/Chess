#include "Pawn.h"
#include "Board.h" // To verify pawn moves need to know board state
#include <string>
#include <sstream>

Pawn::Pawn(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {
        setHasMoved(false);
    }

void Pawn::setHasMoved(bool newHasMoved){
    hasMoved = newHasMoved;
}

bool Pawn::getHasMoved() const { return hasMoved; }

bool Pawn::isValidMove(const Square& otherSquare) const {

    // Cannot move to same position.
    if(otherSquare == this->getSquarePosition()){ return false; }

    // Determine if 2 steps forward is allowed. e.g. e4-e6 (white) or e7-e5 (black)
    if(_isValidTwoStepMove(otherSquare) == false){
        return false;
    }

    return true;
}

bool Pawn::_isValidTwoStepMove(const Square& otherSquare) const {
    const Square& thisSquare{this->getSquarePosition()};
    Color_T thisColor = this->getColor();
    unsigned int thisRow{thisSquare.getRow()};
    unsigned int thisCol{thisSquare.getCol()};

    unsigned int otherRow{otherSquare.getRow()};
    unsigned int otherCol{otherSquare.getCol()};

    if(this->getHasMoved()) { return false; } // Can only two step once.

    switch(thisColor){
        case Color_T::BLACK:
            
            if(((otherRow == (thisRow + 2)) && (otherCol == thisCol)) && (!otherSquare.isOccupied() && 1)){ return true; }

        case Color_T::WHITE:
            if(otherRow != (thisRow - 2)) { return false; }
    }

    return true;
}

std::string Pawn::toString() const {
    std::ostringstream output;
    output << "Piece: Pawn\n";
    output << "Color: " << colorToString(this->getColor());
    output << "\nCurrent Square: " << this->getSquarePosition(); // Overloaded.
    output << "\nHas Moved: " << std::boolalpha << this->getHasMoved();

    return output.str();
}