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
        static const size_t MAX_ROWS{8};
        static const size_t MAX_COLS{8};

        Board(); // Eventually include option to start at a position, FEN.
        Board(const std::array<std::array<char, MAX_COLS>, MAX_ROWS>& initBoardMapping);

        const std::array<std::array<Square, MAX_COLS>, MAX_ROWS>& getBoard();

        Square& getBoardAt(size_t row, size_t col);
        const Square& getBoardAt(size_t row, size_t col) const;

        const Piece* getPieceAt(size_t row, size_t col) const;

        void printBoard() const;
        std::string boardToString() const;

        // Move member function
        bool canPawnMoveTo(const Square& from, const Square& to, Color_T pawnColor, bool hasMoved) const;

    private:
        std::array<std::array<Square, MAX_COLS>, MAX_ROWS> board;
        std::array<std::array<std::unique_ptr<Piece>, MAX_COLS>, MAX_ROWS> pieces;
        
        struct MoveCoordsData {
            unsigned int fromRow, fromCol;
            unsigned int toRow, toCol;
        };

        bool _isValidTwoStepMove(const Square& from, Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied, bool pawnHasMoved) const;
        bool _isTwoStepMove(const Square& from, Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied) const;

        Piece_T _charToPieceType(char c);
        std::unique_ptr<Piece> _createPiece(char pieceChar, Color_T color, const Square& square);
};