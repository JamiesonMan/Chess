#include <iostream>
#include "../../TestManager.h"

int main(){
    // Test case: White pawn initial two-step move
    const std::array<std::array<char, TestManager::MAX_COLS>, TestManager::MAX_ROWS> boardInit = {{
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'P', ' ', ' ', ' ', ' '},  // White pawn at initial position
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    }};

    try {
        TestManager testManager(boardInit);
        testManager.runTwoStepMoveTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}