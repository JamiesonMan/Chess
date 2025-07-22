#include "Bishop.h"
#include "Board.h" // To verify pawn moves need to know board state
#include <string>
#include <sstream>
#include <iostream>

Bishop::Bishop(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {}

bool Bishop::isValidMove(const Square& toSquare) const {
    return getBoard().validBishopMove(getSquarePosition(), toSquare, getColor());
} 

std::string Bishop::toString() const {
    std::ostringstream output;
    output << "Piece: " << Piece::typeToString(getType());
    output << "\nColor: " << colorToString(this->getColor());
    output << "\nCurrent Square: " << this->getSquarePosition(); // Overloaded.

    return output.str();
}