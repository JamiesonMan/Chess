# Chess Engine Library

AI chess engine for analyzing positions and suggesting optimal moves. **[Under Development]**

## Planned Features

### Core Engine Capabilities
- **Position Evaluation** - Static evaluation of chess positions
- **Move Search** - Minimax with alpha-beta pruning (I am not planning to incorporate a Neural Network, although that would be cool!)
- **Opening Book** - Fast table of opening moves
- **Endgame Tables** - Perfect play in simple endgames

### Communication Interface
- **FEN String I/O** - Import/export positions via FEN (example located below)
- **Move Notation** - Standard algebraic notation support
- **Engine Protocol** - Compatible with chess GUIs via UCI similare to stockfish. I may also build my own, specific to this engine.

## Planned API

```cpp
#include "ChessEngine.h"

// Initialize engine
ChessEngine engine;
engine.setDepth(6);  // Search depth

// Get best move from FEN position
std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
Move bestMove = engine.getBestMove(fen);

// Validate user moves (replaces chess_game validation)
bool isValid = engine.isValidMove(fen, "e2e4");

// Get position evaluation  
int evaluation = engine.evaluatePosition(fen);
// Positive = white advantage, negative = black advantage
```

## Architecture Design

### Complete Independence from chess_game
```
Application
├── chess_game (display only) 
└── chess_engine (all chess logic)
```

**chess_engine is completely self-contained:**
- Receives FEN strings as input
- Contains ALL chess rules and move validation
- Returns best moves or validates proposed moves
- No dependency on chess_game library

**chess_game is just a display layer:**
- Shows board positions to users
- Updates display when moves are made
- Exports/imports FEN strings
- NO chess rule knowledge

### Planned Components

- **Board Representation** - Bitboards for performance
- **Move Generation** - Fast legal move generation
- **Search Algorithm** - Minimax with optimizations
- **Evaluation Function** - Material + positional assessment
- **Transposition Tables** - Cache evaluated positions

## Development Roadmap

1. **Basic Engine Structure** - Core classes and interfaces
2. **FEN Parser** - Import chess positions from strings
3. **Move Generator** - Generate all legal moves efficiently
4. **Simple Evaluation** - Material counting + basic positional factors
5. **Minimax Search** - Basic search with depth limit
6. **Alpha-Beta Pruning** - Search optimization
7. **Advanced Evaluation** - Piece-square tables, pawn structure
8. **Performance Optimization** - Bitboards, move ordering
9. **Opening Book** - Common opening sequences
10. **Engine Protocol** - UCI or custom protocol support

## Building (Future)

```bash
# Build as part of main project
mkdir build && cd build
cmake ..
make

# Or build standalone
cd libs/chess_engine
mkdir build && cd build
cmake ..
make
```

## Testing (Planned)

- **Position Tests** - Known tactical puzzles
- **Performance Tests** - Nodes per second benchmarks
- **Game Tests** - Engine vs engine matches
- **Regression Tests** - Ensure improvements don't break functionality

## Engine vs Engine Example

```cpp
#include "Game.h"        // chess_game (display only)
#include "ChessEngine.h" // chess_engine (all logic)

Game display;           // Just for visualization
ChessEngine engine;     // Contains all chess knowledge

// Initialize starting position
display.loadFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

while (!engine.isGameOver(display.toFEN())) {
    // Engine calculates best move from current FEN
    std::string currentFEN = display.toFEN();
    Move bestMove = engine.getBestMove(currentFEN);
    
    // Update display (no validation - engine is trusted)
    display.makeMove(bestMove.toString());
    display.displayBoard();
}
```

## Human vs Engine Example

```cpp
Game display;
ChessEngine engine;

// User inputs move
std::string userMove = getUserInput();  // e.g., "e2e4"
std::string currentFEN = display.toFEN();

// Engine validates move (replaces chess_game validation)
if (engine.isValidMove(currentFEN, userMove)) {
    display.makeMove(userMove);
    
    // Engine responds
    Move response = engine.getBestMove(display.toFEN());
    display.makeMove(response.toString());
}
```

## Performance Goals

- **Speed** - 100K+ nodes/second on modern hardware
- **Strength** - 1800+ ELO rating
- **Memory** - Efficient memory usage for mobile platforms
- **Responsiveness** - Real-time move suggestions

## Dependencies

- C++20 standard library (maybe can be changed to only require C++14?)
- **NO dependency on chess_game library** - completely independent
- No external dependencies planned

**Key:** chess_engine contains ALL chess logic and never calls chess_game functions