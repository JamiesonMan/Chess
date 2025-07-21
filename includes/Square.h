#pragma once
#include "Colors.h"
#include <ostream>

class Square final {
    friend std::ostream& operator<<(std::ostream& output, const Square& square);

    public:
        Square(Color_T squareColor = Color_T::WHITE, unsigned int row = 1, unsigned int col = 1);

        Color_T getSquareColor() const;
        unsigned int getRow() const;
        unsigned int getCol() const;

        bool isOccupied() const;
        void setOccupied(bool);

        static unsigned int rowToRank(unsigned int); // e.g. 1 = 1, 2 = 2, etc for symmetry.
        static char colToFile(unsigned int); // e.g. 1 = 'a', 2 = 'b', etc.

        bool operator==(const Square& right) const;

    private:
        Color_T color;
        unsigned int row;
        unsigned int col;

        bool occupied; // Piece exists in this square or not.
};