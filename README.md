# Chess Engine Project

A modular chess application with separate libraries for game logic and AI engine development.
Works with Universal Chess Interface allowing it to attach to many prebuilt chess GUI's.
Computationally controllable with number of threads.
Has a dev mode the application can be launched into allowing for base game access. Default will be launched into UCI mode. 

## Project Structure

- **Main Application** (`src/`) - Interactive chess game interface
- **chess_game** (`libs/chess_game/`) - Core chess game logic and rules
- **chess_engine** (`libs/chess_engine/`) - AI engine (future development)

## Features

- Complete chess game implementation with all rules
- Modular library architecture for extensibility
- Cross-platform build support (Windows, Linux, macOS)
- Comprehensive test suite
- Prepared for AI engine integration via FEN strings

## Building the Project

### Prerequisites
- CMake 3.10 or higher
- C++20 compatible compiler

### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make
```

### Windows

#### Visual Studio (Recommended)
```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

#### Visual Studio IDE
```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
```
Then open the generated `.sln` file in Visual Studio.

#### MinGW/MSYS2
```cmd
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

## Running the Application

After building, the executable will be in:
- Linux/macOS: `build/bin/Chess`
- Windows: `build/bin/Chess.exe`

## Development Roadmap

1. **Basic chess game implementation** - Complete with all rules
2. **Modular architecture** - Separated into reusable libraries
3. **Cross-platform support** - Windows, Linux, macOS compatibility
4. **Chess engine development** - AI with evaluation and search
5. **Performance optimization** - Bitboards and efficient algorithms
6. **GUI development** - Enhanced user interface

## Libraries

- [chess_game](libs/chess_game/) - Core game logic and piece movement
- [chess_engine](libs/chess_engine/) - AI engine (planned)

## Testing

Run tests with:
```bash
cd build
ctest
```

## Contributing

This project uses a modular design where libraries can be developed independently and communicate via FEN strings for maximum flexibility.
