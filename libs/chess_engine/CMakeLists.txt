cmake_minimum_required(VERSION 3.10.0)
project(chess_engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create the chess_game library
add_library(chess_engine
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
)

# Set include directories for the library
target_include_directories(chess_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# Test executable
add_executable(TwoStepPawnMove
    tests/TestManager.cpp
    tests/pawn_tests/two_step_pawn_moves/Main.cpp
)

target_link_libraries(TwoStepPawnMove chess_engine)

enable_testing()
add_test(NAME PawnTest COMMAND TwoStepPawnMove)