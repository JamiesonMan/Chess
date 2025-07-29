#pragma once
#include "Square.h"
#include "King.h"
#include "Piece.h"
#include <array>
#include <stdexcept>
#include <cctype>
#include <ostream>
#include <memory>
#include <sstream>
#include "Rook.h"

class Board final {
    friend std::ostream& operator<<(std::ostream& output, const Board& board);

    // Hypothetical board state for move simulation without modifying actual board
    struct HypotheticalMove {
        size_t fromRow, fromCol;
        size_t toRow, toCol;
        
        HypotheticalMove(size_t fRow, size_t fCol, size_t tRow, size_t tCol);
        bool isSquareOccupied(size_t row, size_t col, const Board& board) const;
        const Piece* getPieceAt(size_t row, size_t col, const Board& board) const;
        std::pair<size_t, size_t> getKingPosition(Color_T color, const Board& board) const;
    };

    struct MoveCoordsData {
        unsigned int fromRow, fromCol;
        unsigned int toRow, toCol;
    };

    // Struct to keep track of castling rights.
    struct CastleRights {
        bool whiteLong, whiteShort, blackLong, blackShort, blackCastled, whiteCastled;
    };

    public:
        enum class Castle_T : unsigned int { BLACK_SHORT, BLACK_LONG, WHITE_SHORT, WHITE_LONG }; // To determine a type of castle move.
        enum class Game_Status : unsigned int { CONTINUE, CHECKMATE_END, DRAW_END, IN_CHECK }; // Different statuses to be returned to Game for processing.
        
        static const size_t MAX_ROWS{8};
        static const size_t MAX_COLS{8};

        Board(); // Eventually include option to start at a position, FEN.
        Board(const std::array<std::array<char, MAX_COLS>, MAX_ROWS>& initBoardMapping);

        const std::array<std::array<Square, MAX_COLS>, MAX_ROWS>& getBoard() const;

        const Square& getBoardAt(size_t row, size_t col) const;
        Square& getBoardAt(size_t row, size_t col);

        const Piece* getPieceAt(size_t row, size_t col) const;

        const King* getBlackKing() const;
        const King* getWhiteKing() const;

        bool getBlackKingInCheck() const;
        bool getWhiteKingInCheck() const;

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

        // if a move is legal returns true, else false.
        bool isLegalMove(const Square& from, const Square& to, const std::unique_ptr<Piece>& movingPiece) const;

        // Updates board and pieces.
        Game_Status moveTo(Square& from, Square& to);

        // Convert chess notation (e.g. "E4") to row/col coordinates
        void notationToCoords(const std::string& notation, unsigned int& row, unsigned int& col) const;

    private:
        std::array<std::array<Square, MAX_COLS>, MAX_ROWS> board;
        std::array<std::array<std::unique_ptr<Piece>, MAX_COLS>, MAX_ROWS> pieces;

        CastleRights m_castleRights;
        bool m_checkToResetEnPassant;
        bool m_whiteKingInCheck;
        bool m_blackKingInCheck;

        const King* m_whiteKing;
        const King* m_blackKing;
        
        void _setBlackKingInCheck(bool);
        void _setWhiteKingInCheck(bool);
        
        // Helper function to simulate the board state after a move, then checks if it leaves the king in check.
        bool _wouldLeaveKingInCheck(const Square& from, const Square& to, Color_T movingPieceColor) const;

        void _setBlackKing(King*);
        void _setWhiteKing(King*);

        bool _kingInCheck(const Piece* king) const;
        bool _kingInCheck(const Piece* king, const std::array<std::array<std::unique_ptr<Piece>, MAX_COLS>, MAX_ROWS>& piecesCopy) const;
        bool _checkCheckmate() const;
        bool _checkDraw(Color_T) const;

        struct HypotheticalMoveValidator {
            static bool canPieceAttackSquare(const Piece* piece, size_t pieceRow, size_t pieceCol, 
                                           size_t targetRow, size_t targetCol, 
                                           const HypotheticalMove& move, const Board& board);
            
            static bool isPathClear(size_t fromRow, size_t fromCol, size_t toRow, size_t toCol,
                                  const HypotheticalMove& move, const Board& board);
        };

        // Automatically moves rook to correct location
        void _castleRookMove(Castle_T);
        void _updateRookData(Rook* rook);
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

        bool _isValidCastleMove(const MoveCoordsData& moveData, Color_T kingColor) const;
        std::array<std::array<Square, MAX_COLS>, MAX_ROWS>& getBoard();

        Piece_T _promptForPromotion(Color_T pawnColor) const;
        Piece_T _charToPieceType(char c);
        std::unique_ptr<Piece> _createPiece(char pieceChar, Color_T color, const Square& square);
};