#pragma once
#include "Square.h"
#include "Piece.h"
#include <array>
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

        const std::array<std::array<Square, MAX_COLS>, MAX_ROWS>& getBoard() const;

        const Square& getBoardAt(size_t row, size_t col) const;
        Square& getBoardAt(size_t row, size_t col);

        const Piece* getPieceAt(size_t row, size_t col) const;

        bool getCheckToResetEnPassant() const;
        void setCheckToResetEnPassant(bool val);

        void printBoard() const;
        std::string boardToString() const;

        // Move validation functions.
        bool validPawnMove(const Square& from, const Square& to, Color_T pawnColor, bool hasMoved) const;
        bool validBishopMove(const Square& from, const Square& to, Color_T bishopColor) const;
        bool validKnightMove(const Square& from, const Square& to, Color_T knightColor) const;
        bool validRookMove(const Square& from, const Square& to, Color_T rookColor) const;
        bool validQueenMove(const Square& from, const Square& to, Color_T queenColor) const;
        bool validKingMove(const Square& from, const Square& to, Color_T kingColor) const;

        bool pawnCanPromote(const Square& to, Color_T color) const;

        // Updates board and pieces.
        void moveTo(Square& from, Square& to);

        // Convert chess notation (e.g. "E4") to row/col coordinates
        void notationToCoords(const std::string& notation, unsigned int& row, unsigned int& col) const;

    private:
        std::array<std::array<Square, MAX_COLS>, MAX_ROWS> board;
        std::array<std::array<std::unique_ptr<Piece>, MAX_COLS>, MAX_ROWS> pieces;

        struct MoveCoordsData {
            unsigned int fromRow, fromCol;
            unsigned int toRow, toCol;
        };

        bool m_checkToResetEnPassant;

        bool _prelimMoveCheck(const MoveCoordsData&) const;

        // Two step move
        bool _isValidTwoStepMove(Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied, bool pawnHasMoved) const;
        bool _isTwoStepMove(Color_T pawnColor, const MoveCoordsData& moveData) const;

        // One step move
        bool _isValidOneStepMove(Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied) const;
        bool _isOneStepMove(Color_T pawnColor, const MoveCoordsData& moveData) const;

        // Pawn attacking
        bool _isValidAttackMove(Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied) const;
        bool _isAttackRightMove(Color_T pawnColor, const MoveCoordsData& moveData) const;
        bool _isAttackLeftMove(Color_T pawnColor, const MoveCoordsData& moveData) const;

        bool _isValidEnPassant(Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied, bool direction) const;

        bool _checkDiagBlocked(const MoveCoordsData&, int deltaRow, int deltaCol) const;
        bool _checkRankFileBlocked(const MoveCoordsData& moveData, int deltaRow, int deltaCol) const;

        Piece_T _promptForPromotion(Color_T pawnColor) const;

        std::array<std::array<Square, MAX_COLS>, MAX_ROWS>& getBoard();

        Piece_T _charToPieceType(char c);
        std::unique_ptr<Piece> _createPiece(char pieceChar, Color_T color, const Square& square);
};