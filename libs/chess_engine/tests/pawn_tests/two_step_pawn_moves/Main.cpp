#include <iostream>
#include "../../TestManager.h"

int main(){
    // Test case: White pawn initial two-step move
    int testCode{1};
    const std::array<std::array<char, TestManager::MAX_COLS>, TestManager::MAX_ROWS> twoStepInit_01 = {{
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'P', ' ', ' ', ' ', ' '}, 
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    }};
    try {
        TestManager testManager_01(twoStepInit_01);
        testManager_01.runTwoStepMoveTest(testCode);
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }

    // Test case: White pawn 2 step move with a piece blocking. Should be an invalid move.
    ++testCode;
    const std::array<std::array<char, TestManager::MAX_COLS>, TestManager::MAX_ROWS> twoStepInit_02 = {{
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'r', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'P', ' ', ' ', ' ', ' '}, 
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    }};
    try {
        TestManager testManager_02(twoStepInit_02);
        testManager_02.runTwoStepMoveTest(testCode);
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }

    // Test case: White pawn 2 step move with a piece blocking. Should be an invalid move.
    ++testCode;
    const std::array<std::array<char, TestManager::MAX_COLS>, TestManager::MAX_ROWS> twoStepInit_03 = {{
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'r', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', 'P', ' ', ' ', ' ', ' '},  
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    }};
    try {
        TestManager testManager_03(twoStepInit_03);
        testManager_03.runTwoStepMoveTest(testCode);
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }


        return 0; // Pass
}