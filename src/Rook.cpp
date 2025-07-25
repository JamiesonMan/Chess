#include "Rook.h"
#include "Board.h" // To verify pawn moves need to know board state

Rook::Rook(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {
        setHasMoved(false);
        
        if(pieceSquareRef.getCol() == Board::MAX_COLS - 1) { // If its a short sided rook
            setRookShort(true);
            setRookLong(false);
        } else { // If its a long sided rook
            setRookShort(false);
            setRookLong(true);
        }

    }

void Rook::setHasMoved(bool newHasMoved){
    hasMoved = newHasMoved;
}

bool Rook::getHasMoved() const { return hasMoved; }

bool Rook::getRookLong() const { return rookLong;}
void Rook::setRookLong(bool newRookLong) { rookLong = newRookLong; }

bool Rook::getRookShort() const { return rookShort; }
void Rook::setRookShort(bool newRookShort) { rookShort = newRookShort; }


bool Rook::isValidMove(const Square& toSquare) const {
    return getBoard().validRookMove(getSquarePosition(), toSquare, getColor());
} 