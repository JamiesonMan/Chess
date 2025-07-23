#include "Piece.h"
#include <stdexcept>
#include <sstream>


Piece::Piece(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : type{pieceType}, color{pieceColor}, positionRef{&pieceSquareRef}, boardRef{pieceBoardRef} {
        
        if(pieceSquareRef.isOccupied()){
            throw std::invalid_argument("Error: A piece cannot be initialized where another piece already exists.");
        }
}; 

Piece_T Piece::getType() const { return type; }
void Piece::setType(Piece_T type){
    this->type = type;
}

Color_T Piece::getColor() const{ return color; }
void Piece::setColor(Color_T){
    this->color = color;
}

std::string Piece::typeToString(Piece_T type) {
    switch(type) {
        case Piece_T::PAWN:
            return "Pawn";
        case Piece_T::BISHOP:
            return "Bishop";
        case Piece_T::KNIGHT:
            return "Knight";
        case Piece_T::ROOK:
            return "Rook";
        case Piece_T::QUEEN:
            return "Queen";
        case Piece_T::KING:
            return "King";
        default:
            return "";
    }
}

const Board& Piece::getBoard() const {
    return boardRef;
}

const Square& Piece::getSquarePosition() const {
    return *positionRef;
}

void Piece::setSquarePosition(const Square& newPosition) {
    positionRef = &newPosition;
}

unsigned int Piece::getValue() const {
    return static_cast<unsigned int>(type);
}

std::string Piece::toString() const {
    std::ostringstream output;
    output << "Piece: " << Piece::typeToString(getType());
    output << "\nColor: " << colorToString(this->getColor());
    output << "\nCurrent Square: " << this->getSquarePosition(); // Overloaded.

    return output.str();
}
