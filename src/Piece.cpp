#include "Piece.h"
#include <stdexcept>

Piece::Piece(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : type{pieceType}, color{pieceColor}, positionRef{pieceSquareRef}, boardRef{pieceBoardRef} {
        
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

const Board& Piece::getBoard() const {
    return boardRef;
}

const Square& Piece::getSquarePosition() const {
    return positionRef;
}

unsigned int Piece::getValue() const {
    return static_cast<unsigned int>(type);
}