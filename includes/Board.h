#pragma once
#include "Square.h"
#include "Piece.h"
#include <array>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <ostream>
#include <memory>
#include <sstream>

class Board final {
    friend std::ostream& operator<<(std::ostream& output, const Board& board);

    public:
        Board(); // Eventually include option to start at a position, FEN.

        static const size_t MAX_ROWS{8};
        static const size_t MIN_ROWS{1};
        static const size_t MAX_COLS{8};
        static const size_t MIN_COLS{1};

        const std::array<std::array<Square, MAX_COLS>, MAX_ROWS>& getBoard();

        Square& getBoardAt(size_t row, size_t col);
        const Square& getBoardAt(size_t row, size_t col) const;

        const Piece* getPieceAt(size_t row, size_t col) const;

        void printBoard() const;
        std::string boardToString() const;

    private:
        std::array<std::array<Square, MAX_COLS>, MAX_ROWS> board;
        std::array<std::array<std::unique_ptr<Piece>, MAX_COLS>, MAX_ROWS> pieces;
        
        Piece_T _charToPieceType(char c);
        std::unique_ptr<Piece> _createPiece(char pieceChar, Color_T color, const Square& square);
};