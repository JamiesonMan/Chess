#pragma once
#include "Board.h"

class Game {

    public:

        Game();

        void start();
        void _loop(std::string square1, std::string square2, unsigned int fromRow, unsigned int fromCol, unsigned int toRow, unsigned int toCol);

    private:
        Board board;
};