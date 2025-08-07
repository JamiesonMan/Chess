#include "TestManager.h"
#include <iostream>
#include <stdexcept>

TestManager::TestManager(const std::array<std::array<char, MAX_COLS>, MAX_ROWS>& boardInit)
    : board(boardInit) {}

const Piece* TestManager::findPawn(Color_T color) const {
    for(size_t row = 0; row < MAX_ROWS; row++){
        for(size_t col = 0; col < MAX_COLS; col++){
            const Piece* piece = board.getPieceAt(row, col);
            if(piece && piece->getType() == Piece_T::PAWN && piece->getColor() == color){
                return piece;
            }
        }
    }
    throw std::runtime_error("No pawn of specified color found on board");
}

void TestManager::printTestResult(const std::string& testName, bool result) const {
    std::cout << testName << ": " << std::boolalpha << result << std::endl;
}

void TestManager::runTwoStepMoveTest(int testPart) {
    const Piece* whitePawn = findPawn(Color_T::WHITE);
    const Square& pawnPos = whitePawn->getSquarePosition();
    size_t pawnRow = pawnPos.getRow();
    size_t pawnCol = pawnPos.getCol();

    std::cout << "Pawn found at position: (" << pawnRow << ", " << pawnCol << ")" << std::endl;
    
    // Test specific two-step scenarios
    bool twoStepMove = whitePawn->isValidMove(board.getBoardAt(pawnRow - 2, pawnCol));
    bool threeStepMove = whitePawn->isValidMove(board.getBoardAt(pawnRow - 3, pawnCol));
    bool diagonalRight = whitePawn->isValidMove(board.getBoardAt(pawnRow - 2, pawnCol + 1));
    bool diagonalLeft = whitePawn->isValidMove(board.getBoardAt(pawnRow - 2, pawnCol - 1));
    
    printTestResult("Two-step move valid", twoStepMove);
    printTestResult("Three-step move valid", threeStepMove);
    printTestResult("Diagonal right move valid", diagonalRight);
    printTestResult("Diagonal left move valid", diagonalLeft);

    switch(testPart){
        case 1:
            // Validate test results
            if(!twoStepMove || threeStepMove || diagonalRight || diagonalLeft) {
                throw std::runtime_error("Two-step move test 1 failed!");
            }
            break;
        case 2:
            if(twoStepMove || threeStepMove || diagonalRight || diagonalLeft){
                throw std::runtime_error("Two-step move test 2 failed!");
            }
            break;
        case 3:
            if(twoStepMove || threeStepMove || diagonalRight || diagonalLeft){
                throw std::runtime_error("Two-step move test 2 failed!");
            }
            break;
    }
    
    std::cout << "Test for TwoStepPawnMove: Part " << testPart << " Passed!\n" << std::endl;
}