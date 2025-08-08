#pragma once
#include "FENString.h"
#include <memory>
#include "chess_engine/Types.h"

class Board;
class Square;
class Piece;

class ChessEngine {

    public:
        explicit ChessEngine(FENString fen);
        ~ChessEngine();
        std::string getFenStr() const;
        Game_Status isValidMove(MoveCoordsData move);
        unsigned long int perft(unsigned int depth);
    private:
        FENString m_fen;
        std::unique_ptr<Board> m_board;
};