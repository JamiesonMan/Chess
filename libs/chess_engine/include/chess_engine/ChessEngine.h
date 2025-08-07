#pragma once
#include "FENString.h"
#include <memory>
#include "chess_engine/Types.h"

class Board;
class Square;
class Piece;

class ChessEngine {

    public:
        explicit ChessEngine(const FENString& fen);
        ~ChessEngine();
        Game_Status isValidMove(MoveCoordsData move) const;
    private:
        FENString m_originPosition;
        std::unique_ptr<Board> m_board;
};