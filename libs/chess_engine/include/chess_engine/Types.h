#pragma once
#include <cstddef>
#include "chess_engine/FENString.h"

static const size_t MAX_ROWS{8};
static const size_t MAX_COLS{8};

enum class UCICommand_T { INVALID, QUIT, ISREADY, UCINEWGAME, POSITION, GO, STOP, PONDERHIT, SETOPTION, UCI };

enum class Color_T : bool { WHITE = true, BLACK = false };

enum class Piece_T : unsigned int { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

// Display Piece Types.
enum class DPiece_T : char { NULL_PIECE = '0',
                                B_PAWN = 'p', B_KNIGHT = 'n', B_BISHOP = 'b', B_ROOK = 'r', B_QUEEN = 'q', B_KING = 'k', 
                                W_PAWN = 'P', W_KNIGHT = 'N', W_BISHOP = 'B', W_ROOK = 'R', W_QUEEN = 'Q', W_KING = 'K' };

struct MoveCoordsData {
    size_t fromRow, fromCol;
    size_t toRow, toCol;
};

static const FENString FEN_STARTING_POS{std::string{FENString::INIT_FEN}};

enum class Game_Status : unsigned int { INVALID, CONTINUE, CHECKMATE_END, DRAW_END, IN_CHECK }; // Different statuses to be returned to Game for processing.