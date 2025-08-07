#pragma once
#include "chess_engine/ChessEngine.h"
#include "chess_engine/Types.h"
#include "chess_engine/FENString.h"
#include "DBoard.h"
#include <sstream>

class Game {

    public:
        Game();
        Game(const FENString& fen);
        void start();

    private:
        FENString m_currentFEN;
        Color_T m_turn;
        DBoard m_dboard;
        bool m_gameActive;

        void _getUserInput(std::string& square1, std::string& square2, 
                           size_t& fromRow, size_t& fromCol,
                           size_t& toRow, size_t& toCol, std::ostringstream& error);

        void _flipTurn();
        void _reset();
        bool _askPlayAgain();
        
        // FEN management
        const FENString& getCurrentFEN() const;
        void setCurrentFEN(const FENString& fen);
        void updateFENAfterMove(const MoveCoordsData& move);
};