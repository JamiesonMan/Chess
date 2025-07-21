#include "Board.h"
#include "Colors.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

#include "Pawn.h"


Board::Board(){
    board = std::make_unique<std::array<std::array<Square, MAX_COLS>, MAX_ROWS>>();
    // Initialize pieces array to nullptr
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        for(size_t col = 0; col < MAX_COLS; ++col) {
            pieces[row][col] = nullptr;
        }
    }


    std::array<std::array<char, MAX_COLS>, MAX_ROWS> initBoardMapping = {{
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    }};


    // Single loop for both squares and pieces
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        for(size_t col = 0; col < MAX_COLS; ++col) {
            // Create square with alternating colors
            Color_T squareColor = ((row + col) % 2 == 0) ? Color_T::WHITE : Color_T::BLACK;
            (*board)[row][col] = Square{squareColor, static_cast<unsigned int>(row + 1), static_cast<unsigned int>(col + 1)};
            
            // Get piece character from mapping
            char pieceChar = initBoardMapping[row][col];
            
            // Create piece if square is not empty
            if(pieceChar != ' ') {
                Color_T pieceColor = std::isupper(pieceChar) ? Color_T::WHITE : Color_T::BLACK;
                pieces[row][col] = _createPiece(pieceChar, pieceColor, (*board)[row][col]);
                (*board)[row][col].setOccupied(true);
            }
        }
    }
}

const std::unique_ptr<std::array<std::array<Square, Board::MAX_COLS>, Board::MAX_ROWS>>& Board::getBoard(){
    return board;
}

const Piece* Board::getPieceAt(size_t row, size_t col) const {
    if(row >= MAX_ROWS || col >= MAX_COLS) {
        return nullptr;
    }
    return pieces[row][col].get();
}

Piece_T Board::_charToPieceType(char c) {
    switch(std::toupper(c)) {
        case 'R': return Piece_T::ROOK;
        case 'N': return Piece_T::KNIGHT;
        case 'B': return Piece_T::BISHOP;
        case 'Q': return Piece_T::QUEEN;
        case 'K': return Piece_T::KING;
        case 'P': return Piece_T::PAWN;
        default: throw std::invalid_argument("Invalid piece character");
    }
}

std::unique_ptr<Piece> Board::_createPiece(char pieceChar, Color_T color, const Square& square) {
    Piece_T type = _charToPieceType(pieceChar);
    switch(type) {
        case Piece_T::ROOK:   
        case Piece_T::KNIGHT: 
        case Piece_T::BISHOP: 
        case Piece_T::QUEEN:  
        case Piece_T::KING:   
        case Piece_T::PAWN:   
            return std::make_unique<Pawn>(type, color, square, *this);
        default: throw std::invalid_argument("Invalid piece type");
    }
}

void Board::printBoard() const {
    std::cout << this->boardToString();
}

std::string Board::boardToString() const {
    std::ostringstream output;

    output << "  -----------------\n";
    // Traverse board.
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        output << (8 - row) << "|" << " ";
        for(size_t col = 0; col < MAX_COLS; ++col) {
            char pieceChar = ' ';
            
            // Get piece at this position (O(1) lookup)
            const auto& piece = pieces[row][col];
            if(piece) {
                // Get piece type and color to determine character
                Piece_T type = piece->getType();
                Color_T color = piece->getColor();
                
                switch(type) {
                    case Piece_T::PAWN:   pieceChar = 'P'; break;
                    case Piece_T::ROOK:   pieceChar = 'R'; break;
                    case Piece_T::KNIGHT: pieceChar = 'N'; break;
                    case Piece_T::BISHOP: pieceChar = 'B'; break;
                    case Piece_T::QUEEN:  pieceChar = 'Q'; break;
                    case Piece_T::KING:   pieceChar = 'K'; break;
                }
                
                if(color == Color_T::BLACK) {
                    pieceChar = std::tolower(pieceChar);
                }
            }
            
            output << pieceChar << " ";
        }
        output << "|" << "\n";
    }
    output << "  -----------------\n";
    output << "   a b c d e f g h\n";

    return output.str();
}

std::ostream& operator<<(std::ostream& output, const Board& board){
    output << board.boardToString();
    return output;
}