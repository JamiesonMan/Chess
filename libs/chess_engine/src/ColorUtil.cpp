#include "chess_engine/ColorUtil.h"
#include <stdexcept>

std::string colorToString(Color_T color){
    switch(color){
        case Color_T::WHITE:
            return "WHITE";
        case Color_T::BLACK:
            return "BLACK";
        default:
            throw std::invalid_argument("Error: This color type is not convertible to a string.");
    }
}

Color_T DPieceColor(DPiece_T p) {
    if(p == DPiece_T::W_PAWN || p == DPiece_T::W_BISHOP || p == DPiece_T::W_KNIGHT || p == DPiece_T::W_ROOK || 
        p == DPiece_T::W_QUEEN || p == DPiece_T::W_KING ) {
            return Color_T::WHITE;
        } else {
            return Color_T::BLACK;
        }
}