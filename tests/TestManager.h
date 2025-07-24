#pragma once
#include "../includes/Board.h"
#include "../includes/Piece.h"
#include "../includes/Colors.h"
#include <array>
#include <string>

class TestManager {
public:
    static const size_t MAX_COLS = Board::MAX_COLS;
    static const size_t MAX_ROWS = Board::MAX_ROWS;

    TestManager(const std::array<std::array<char, Board::MAX_COLS>, Board::MAX_ROWS>& boardInit);
    
    void runTwoStepMoveTests();
    
private:
    Board board;
    
    const Piece* findPawn(Color_T color) const;
    void printTestResult(const std::string& testName, bool result) const;
};