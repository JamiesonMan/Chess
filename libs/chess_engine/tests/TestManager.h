#pragma once
#include "../src/Board.h"
#include "../src/Piece.h"
#include "chess_engine/ColorUtil.h"
#include <array>
#include <string>

class TestManager {
public:
    TestManager(const std::array<std::array<char, MAX_COLS>, MAX_ROWS>& boardInit);
    
    void runTwoStepMoveTest(int testPart);
private:
    Board board;
    
    const Piece* findPawn(Color_T color) const;
    void printTestResult(const std::string& testName, bool result) const;
};