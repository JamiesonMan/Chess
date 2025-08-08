#pragma once
#include "chess_engine/Types.h"
#include <array>
#include <string>

// Display board
class DBoard final {
    friend std::ostream& operator<<(std::ostream& output, const DBoard& dboard);
    public:
        DBoard();
        DBoard(const FENString& fen);

        DPiece_T getBoardAt(size_t row, size_t col) const;

        void updateBoard(const std::string& fen);
        void notationToCoords(const std::string& notation, size_t& row, size_t& col) const;

        std::string dboardToString() const;

    private:
        std::array<std::array<DPiece_T, MAX_COLS>, MAX_ROWS> m_dboard;
};