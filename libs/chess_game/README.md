# Chess Game Library

A lightweight C++ chess board representation and display library for user interface and game state management.

## Overview

This library provides chess board visualization and state management:
- Chess board representation and display
- Game state tracking (FEN string import/export)
- User interface for move input and board visualization
- Application-level game management (no chess rule logic)

## API Usage

### New Architecture - Display Only (TODO: Move game logic from chess_game lib to chess_engine lib leave board representation.)

```cpp
#include "Game.h"

// Initialize board from FEN string
Game game;
game.loadFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

// Display current position
game.displayBoard();

// Update board with engine's move
game.makeMove("e2e4");  // Move from engine

// Export current position
std::string currentFEN = game.toFEN();
```

### Integration with Chess Engine

```cpp
#include "Game.h"
#include "ChessEngine.h"  // From chess_engine library

Game game;
ChessEngine engine;

// 1. Get current position
std::string fen = game.toFEN();

// 2. Engine calculates best move
Move bestMove = engine.getBestMove(fen);

// 3. Validate user moves (engine does validation)
bool isValid = engine.isValidMove(fen, userMove);
if (isValid) {
    game.makeMove(userMove);
}

// 4. Update board with engine move
game.makeMove(bestMove.toString());
```

### Key Principle

**chess_game does NOT validate moves** - all chess logic is handled by chess_engine:

```cpp
// OLD: chess_game validates moves
if (piece->canMoveTo(board, x, y)) { ... }

// NEW: chess_engine validates moves  
if (engine.isValidMove(currentFEN, proposedMove)) {
    game.makeMove(proposedMove);  // Just update display
}
```

## Building as Library

This library is designed to be used as a static library in larger projects.

### CMake Integration

```cmake
# Add as subdirectory
add_subdirectory(path/to/chess_game)

# Link to your target
target_link_libraries(your_target chess_game)
```

### Manual Compilation

```bash
# Compile library
g++ -std=c++20 -c src/*.cpp -Iincludes/
ar rcs libchess_game.a *.o

# Use in your project
g++ -std=c++20 your_main.cpp -L. -lchess_game -Ipath/to/includes/
```

## Key Classes

- **Game** - Main game controller and interface
- **Board** - 8x8 chess board representation
- **Piece** - Base class for all chess pieces
- **Square** - Individual board square with coordinates
- **Colors** - Color utilities and definitions

## Game Rules Implemented

- **Standard Movement** - All pieces move according to chess rules  
- **Castling** - Both kingside and queenside castling  
- **En Passant** - Pawn captures en passant  
- **Pawn Promotion** - Pawns promote to any piece  
- **Check Detection** - Identifies when king is in check  
- **Game State** - Tracks turns, moves, and game status  

## Testing

The library includes comprehensive tests:

```bash
# Build and run tests
mkdir build && cd build
cmake ..
make
./TwoStepPawnMove  # Run pawn movement tests
```

## Engine vs Engine Example

```cpp
Game game;
ChessEngine engine;

// Start from initial position
game.loadFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

while (!game.isGameOver()) {
    // Get current position
    std::string fen = game.toFEN();
    
    // Engine calculates best move
    Move bestMove = engine.getBestMove(fen);
    
    // Update board (no validation needed - engine is trusted)
    game.makeMove(bestMove.toString());
    
    // Display updated position
    game.displayBoard();
}
```

## Dependencies

- C++20 standard library
- No external dependencies required
- **Does NOT depend on chess_engine** (separation of concerns)