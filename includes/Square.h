#pragma once
#include "Colors.h"
#include <ostream>

// Maps itself as row: [0-7] and col: [0-7]
class Square final {
    friend std::ostream& operator<<(std::ostream& output, const Square& square);

    public:
        Square(Color_T squareColor = Color_T::WHITE, unsigned int row = 0, unsigned int col = 0);

        Color_T getSquareColor() const;
        unsigned int getRow() const;
        unsigned int getCol() const;

        bool isOccupied() const;
        void setOccupied(bool);

        static unsigned int rowToRank(unsigned int);
        static char colToFile(unsigned int); // e.g. 0 = 'A', 1 = 'B', etc.

        bool operator==(const Square& right) const;

    private:
        Color_T color;
        unsigned int row;
        unsigned int col;

        bool occupied; // Piece exists in this square or not.
};