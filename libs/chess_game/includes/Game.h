#pragma once
#include "Board.h"

class Game {

    public:

        Game();

        void start();
        void _loop(std::string square1, std::string square2, unsigned int fromRow, unsigned int fromCol, unsigned int toRow, unsigned int toCol);

        void _flipTurn();
        void _reset();
        bool _askPlayAgain();

    private:
        Color_T turn;
        Board board;
};