cmake_minimum_required(VERSION 3.10.0)
project(chess_game VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../chess_engine ${CMAKE_CURRENT_BINARY_DIR}/chess_engine)

# Create the chess_game library
add_library(chess_game
    src/Game.cpp
)

# Set include directories for the library
target_include_directories(chess_game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(chess_game chess_engine)
