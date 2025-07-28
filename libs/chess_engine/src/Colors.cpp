#include "Colors.h"
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