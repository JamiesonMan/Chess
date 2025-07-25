cmake_minimum_required(VERSION 3.10.0)
project(Chess VERSION 0.1.0 LANGUAGES C CXX)

# Set C++20 standard; C++14 build possible as of 7/25/2025
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(includes)

# Add executable with source files from src directory
add_executable(Chess
    src/Board.cpp 
    src/Colors.cpp 
    src/Pawn.cpp 
    src/Bishop.cpp
    src/Knight.cpp
    src/Rook.cpp
    src/Queen.cpp
    src/King.cpp
    src/Piece.cpp
    src/Square.cpp
    src/Game.cpp
    Main.cpp
)

add_executable(TwoStepPawnMove
    src/Board.cpp 
    src/Colors.cpp 
    src/Pawn.cpp 
    src/Piece.cpp
    src/Bishop.cpp
    src/Rook.cpp
    src/Knight.cpp
    src/Queen.cpp
    src/King.cpp
    src/Square.cpp
    src/Game.cpp
    tests/TestManager.cpp
    tests/pawn_tests/two_step_pawn_moves/Main.cpp
)
enable_testing()
add_test(NAME TwoStepPawnMove COMMAND TwoStepPawnMove)