#include "Pawn.h"
#include "Board.h" // To verify pawn moves need to know board state
#include <string>
#include <sstream>
#include <iostream>

Pawn::Pawn(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {
        setHasMoved(false);
        setEnPassantCaptureStatus(false);
    }

void Pawn::setHasMoved(bool newHasMoved){
    hasMoved = newHasMoved;
}

bool Pawn::getHasMoved() const { return hasMoved; }

void Pawn::setEnPassantCaptureStatus(bool newEnPassantCaptureStatus) {
    enPassantCaptureStatus = newEnPassantCaptureStatus;
}
bool Pawn::getEnPassantCaptureStatus() const { return enPassantCaptureStatus; }

bool Pawn::isValidMove(const Square& toSquare) const {
    return getBoard().validPawnMove(getSquarePosition(), toSquare, getColor(), getHasMoved());
} 

void Pawn::updateAttacking() {
    _clearAttacking();
    for(size_t row = 0; row < Board::MAX_ROWS; ++row) {
        for(size_t col = 0; col < Board::MAX_COLS; ++col) {
            const Square& to = getBoard().getBoardAt(row, col);
            if(to.isOccupied()){
                if(isValidMove(to)){
                    const Piece* attackedPiece = getBoard().getPieceAt(row, col);
                    addToAttacking(attackedPiece);
                }
            }
        }
    }
}

std::string Pawn::toString() const {
    std::ostringstream output;
    output << "Piece: " << Piece::typeToString(getType());
    output << "\nColor: " << colorToString(this->getColor());
    output << "\nCurrent Square: " << this->getSquarePosition(); // Overloaded.
    output << "\nHas Moved: " << std::boolalpha << this->getHasMoved();

    return output.str();
}