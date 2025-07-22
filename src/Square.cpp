#include "Square.h"
#include <stdexcept>

// Constructor
Square::Square(Color_T squareColor, unsigned int squareRow, unsigned int squareCol)
    : color{squareColor}, row{squareRow}, col{squareCol}, occupied{false} {
        
        // Validate row and col
        if(row > 7){
            throw std::invalid_argument("Error: Row cannot be greater than 7.");
        } else if (col > 7) {
            throw std::invalid_argument("Error: Col cannot be greater than 7.");
        } else if (row < 0) {
            throw std::invalid_argument("Error: Row cannot be less than 0.");
        } else if (col < 0) {
            throw std::invalid_argument("Error: Col cannot be less than 0.");
        }
    }

unsigned int Square::rowToRank(unsigned int row){
    unsigned int arr[8] = { 8, 7, 6, 5, 4, 3, 2, 1 }; // Map the row to correct rank.

    if(row <= 1) {
        throw std::invalid_argument("Error: Row can't be <= 1.");
    } else if (row >= 8) {
        throw std::invalid_argument("Error: Row can't be >= 8.");
    }

    return arr[row];
}

char Square::colToFile(unsigned int col) {

    switch(col){
        case 0:
            return 'A';
        case 1:
            return 'B';
        case 2:
            return 'C';
        case 3:
            return 'D';
        case 4:
            return 'E';
        case 5:
            return 'F';
        case 6:
            return 'G';
        case 7:
            return 'H';
        default:
            throw std::invalid_argument("Error: col must be between 0 and 7 inclusive.");
    }
}

bool Square::isOccupied() const { return occupied; }
void Square::setOccupied(bool newOccupiedValue) {
    occupied = newOccupiedValue;
}

Color_T Square::getSquareColor() const { return color; }
unsigned int Square::getRow() const { return row; }
unsigned int Square::getCol() const { return col; }

bool Square::operator==(const Square& right) const {

    if(this->row != right.getRow() || this->col != right.getCol()) {
        return false;
    }
    return true;
}

// Just print square position, e.g. "E4".
std::ostream& operator<<(std::ostream& output, const Square& square){
    output << square.colToFile(square.getCol()) << square.rowToRank(square.getRow());
    return output;
}