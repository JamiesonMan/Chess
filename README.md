CHESS:
START: 7/20/25

README.md last updated: 7/25/2025

Description:
    Currently in the early stages of creating what I hope to eventually be a chess engine.
    First step was to simply create the game of chess.
    Secondly we want to make this much much more memory efficient as to support higher depth thus speed of a simple engine.
    Thirdly begin using simple pruning algorithms/ hard coded (No Neural Network) evaluation for good or bad moves.
    Fourth continue improving till satisfied with strength.

Building this Application (via CMake):
    C++14 is required as a minimum, but for developmental ease of mind I am keeping it at C++20.

    Currently compiles test binaries too, but remove this if you are running on toaster, however as I write this now (7/25/2025), even a toaster can compile quick enough.

    Compile Command (Linux Distro):
        mkdir -p build
        cd build
        cmake ..
        make

    Windows:
        With Visual Studio:
            mkdir build
            cd build
            cmake ..
            cmake --build .

        With MinGW/MSYS2:
            mkdir build
            cd build
            cmake .. -G "MinGW Makefiles"
            mingw32-make

        With Visual Studio IDE:
            mkdir build
            cd build
            cmake .. -G "Visual Studio 17 2022"
            Then open the generated .sln file in Visual Studio.

Major Inefficiencies:
    1. Not using bitboards to describe board state of pieces etc.
    2. toMove() is pretty bad, a better approach I overlooked until I did more research would've been to just return a list of all possible moves in one function call! Now I have to call a crap ton if I want to create a list of possible moves. Currently, there doesn't even exist a list of possible moves to utilize.

Current developmental plan
    1. Finish basic (inefficient) game.
    2. Refactor inefficient game without any major data structure changes (just checking over current algorithms and testing everything).
    3. Implement bitboards, move class, etc. (Major change! One that I discovered needs to happen after research)
    4. Make it chess GUI compatible or create my own simple GUI. (Optional but nice)
    