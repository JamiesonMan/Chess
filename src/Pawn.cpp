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

bool Pawn::isValidMove(const Square& otherSquare) const {
    const Square& thisSquare{this->getSquarePosition()};
    Color_T thisColor = this->getColor();

    const Board& boardRef = this->getBoard();

    MoveCoordsData data{thisSquare.getRow(), thisSquare.getCol(), otherSquare.getRow(), otherSquare.getCol()};
    
    if(data.fromCol > 8 || data.toCol > 8 || data.fromCol < 1 || data.toCol < 1) {
        std::cout << "Invalid column data: fromCol=" << data.fromCol << ", toCol=" << data.toCol << std::endl;
        return false;
    }

    bool otherSquareOccupied{otherSquare.isOccupied()};

    // Cannot move to the current position.
    if(otherSquare == this->getSquarePosition()){ return false; }

    // Check if it's a two step move and if it's valid
    if(_isTwoStepMove(thisSquare, thisColor, boardRef, data, otherSquareOccupied)){
        return _isValidTwoStepMove(thisSquare, thisColor, boardRef, data, otherSquareOccupied);
    }

    return false;
}

bool Pawn::_isValidTwoStepMove(const Square& thisSquare, Color_T thisColor, const Board& boardRef, const MoveCoordsData& data, bool otherSquareOccupied) const {

    if(this->getHasMoved()) { 
        std::cout << "Pawn has already moved" << std::endl;
        return false; 
    } // Can only two step once.
    switch(thisColor){
        case Color_T::BLACK: {
            // Check bounds and get middle square (1-8 indexing)
            if(data.fromRow + 1 > 8 || data.fromCol > 8) return false;
            const Piece* midPiece = boardRef.getPieceAt(data.fromRow, data.fromCol - 1);
            bool midSquareOccupied = (midPiece != nullptr);

            if((data.toRow == (data.fromRow + 2)) && (data.toCol == data.fromCol)){ // It's a two step move.
                if(!otherSquareOccupied && !midSquareOccupied){ // The two spaces are free to move in.
                    // Need to still implement a function to determine if it would put the kind in check.
                    return true; 
                }
            }
            return false;
        }
        
        case Color_T::WHITE: {
            // Check bounds and get middle square (1-8 indexing)
            if(data.fromRow < 2 || data.fromCol > 8) {
                std::cout << "White pawn bounds check failed: fromRow=" << data.fromRow << ", fromCol=" << data.fromCol << std::endl;
                return false;
            }
            const Piece* midPiece = boardRef.getPieceAt(data.fromRow - 2, data.fromCol - 1);
            bool midSquareOccupied = (midPiece != nullptr);

            if((data.toRow == (data.fromRow - 2)) && (data.toCol == data.fromCol)){ // It's a two step move.
                if(!otherSquareOccupied && !midSquareOccupied){ // The two spaces are free to move in.
                    // *** Need to still implement a function to determine if it would put the kind in check. ***
                    return true; 
                }
            }
            return false;
        }
    }
}

bool Pawn::_isTwoStepMove(const Square& thisSquare, Color_T thisColor, const Board& boardRef, const MoveCoordsData& data, bool otherSquareOccupied) const{
    if(thisColor == Color_T::BLACK) {
        // Black pawns move down (increasing row)
        return (data.toRow == (data.fromRow + 2)) && (data.toCol == data.fromCol);
    } else {
        // White pawns move up (decreasing row)
        return (data.toRow == (data.fromRow - 2)) && (data.toCol == data.fromCol);
    }
}   
 
std::string Pawn::toString() const {
    std::ostringstream output;
    output << "Piece: Pawn\n";
    output << "Color: " << colorToString(this->getColor());
    output << "\nCurrent Square: " << this->getSquarePosition(); // Overloaded.
    output << "\nHas Moved: " << std::boolalpha << this->getHasMoved();

    return output.str();
}