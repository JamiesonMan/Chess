#include "TestManager.h"
#include <iostream>
#include <stdexcept>

TestManager::TestManager(const std::array<std::array<char, Board::MAX_COLS>, Board::MAX_ROWS>& boardInit)
    : board(boardInit) {}

const Piece* TestManager::findPawn(Color_T color) const {
    for(size_t row = 0; row < Board::MAX_ROWS; row++){
        for(size_t col = 0; col < Board::MAX_COLS; col++){
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

void TestManager::runTwoStepMoveTests() {
    const Piece* whitePawn = findPawn(Color_T::WHITE);
    const Square& pawnPos = whitePawn->getSquarePosition();
    
    long int pawnRow = pawnPos.getRow();
    int pawnCol = pawnPos.getCol();
    
    std::cout << "Pawn found at position: (" << pawnRow << ", " << pawnCol << ")" << std::endl;
    
    // Test specific two-step scenarios
    bool twoStepMove = whitePawn->isValidMove(board.getBoardAt(pawnRow - 2, pawnCol));
    bool threeStepMove = whitePawn->isValidMove(board.getBoardAt(pawnRow - 3, pawnCol));
    bool diagonalMove = whitePawn->isValidMove(board.getBoardAt(pawnRow - 2, pawnCol + 1));
    
    printTestResult("Two-step move valid", twoStepMove);
    printTestResult("Three-step move valid", threeStepMove);
    printTestResult("Diagonal move valid", diagonalMove);
    
    // Validate test results
    if(!twoStepMove || threeStepMove || diagonalMove) {
        throw std::runtime_error("Two-step move tests failed");
    }
    
    std::cout << "All two-step move tests passed!" << std::endl;
}