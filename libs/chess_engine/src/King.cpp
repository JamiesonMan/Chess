#include "King.h"
#include "Board.h" // To verify pawn moves need to know board state

King::King(Piece_T pieceType, Color_T pieceColor, const Square& pieceSquareRef, const Board& pieceBoardRef)
    : Piece(pieceType, pieceColor, pieceSquareRef, pieceBoardRef) {
        setHasMoved(false);
        setCanCastleLong(true);
        setCanCastleShort(true);
    }

void King::setHasMoved(bool newHasMoved){
    hasMoved = newHasMoved;
}

bool King::getHasMoved() const { return hasMoved; }

bool King::isValidMove(const Square& toSquare) const {
    return getBoard().validKingMove(getSquarePosition(), toSquare, getColor());
} 

bool King::getCanCastleShort() const { return m_canCastleShort;}
void King::setCanCastleShort(bool canCastleShort) { m_canCastleShort = canCastleShort;}

bool King::getCanCastleLong() const {return m_canCastleLong;}
void King::setCanCastleLong(bool canCastleLong) {m_canCastleLong = canCastleLong;}

void King::updateAttacking() {
    _clearAttacking();
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        for(size_t col = 0; col < MAX_COLS; ++col) {
            const Square& to = getBoard().getBoardAt(row, col);
            if(to.isOccupied()){
                const Piece* attackedPiece = getBoard().getPieceAt(row, col);
                if(isValidMove(to)){
                    addToAttacking(attackedPiece);
                }
            }
        }
    }
}