#include "DBoard.h"
#include <stdexcept>
#include <sstream>

DBoard::DBoard() : DBoard(FEN_STARTING_POS){}

//"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"
DBoard::DBoard(const FENString& fen) {
    std::string fenBoard = fen.getBoardStr();
    size_t row{0};
    size_t col{0};
    
    // Initialize board to empty
    for(size_t i = 0; i < MAX_ROWS; ++i) {
        for(size_t j = 0; j < MAX_COLS; ++j) {
            m_dboard[i][j] = DPiece_T::NULL_PIECE;
        }
    }
    
    for(char p : fenBoard){
        if(row >= MAX_ROWS) break; // Safety check
        
        switch(p){
            case 'r':
                m_dboard[row][col] = DPiece_T::B_ROOK;
                ++col;
                break;
            case 'n':
                m_dboard[row][col] = DPiece_T::B_KNIGHT;
                ++col;
                break;
            case 'b':
                m_dboard[row][col] = DPiece_T::B_BISHOP;
                ++col;
                break;
            case 'q':
                m_dboard[row][col] = DPiece_T::B_QUEEN;
                ++col;
                break;
            case 'k':
                m_dboard[row][col] = DPiece_T::B_KING;
                ++col;
                break;
            case 'p':
                m_dboard[row][col] = DPiece_T::B_PAWN;
                ++col;
                break;
            case 'R':
                m_dboard[row][col] = DPiece_T::W_ROOK;
                ++col;
                break;
            case 'N':
                m_dboard[row][col] = DPiece_T::W_KNIGHT;
                ++col;
                break;
            case 'B':
                m_dboard[row][col] = DPiece_T::W_BISHOP;
                ++col;
                break;
            case 'Q':
                m_dboard[row][col] = DPiece_T::W_QUEEN;
                ++col;
                break;
            case 'K':
                m_dboard[row][col] = DPiece_T::W_KING;
                ++col;
                break;
            case 'P':
                m_dboard[row][col] = DPiece_T::W_PAWN;
                ++col;
                break;
            case '/':
                ++row;
                col = 0;
                break;
            default:
                if(std::isdigit(p)){
                    size_t num = p - '0';
                    col += num; // Skip empty squares
                }
                break;
        }
    }
}

DPiece_T DBoard::getBoardAt(size_t row, size_t col) const {
    if(row >= MAX_ROWS || col >= MAX_COLS){
        throw std::invalid_argument("Error: Invalid row/col argument.");
    } else {
        return m_dboard[row][col];
    }
}

// Prevalidated by engine.
void DBoard::moveTo(MoveCoordsData move) {
    m_dboard[move.toRow][move.toCol] = m_dboard[move.fromRow][move.fromCol];
    m_dboard[move.fromRow][move.fromCol] = DPiece_T::NULL_PIECE;
}

std::string DBoard::dboardToString() const {
    std::ostringstream output;
    output << "                BLACK\n\n";
    output << "  +---+---+---+---+---+---+---+---+\n";
    
    // Traverse board.
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        output << (MAX_ROWS - row) << " ";
        for(size_t col = 0; col < MAX_COLS; ++col) {
            char pieceChar = ' ';
            
            // Get piece at this position (O(1) lookup)
            const auto& piece = m_dboard[row][col];
            
            switch(piece) {
                case DPiece_T::NULL_PIECE: break;
                case DPiece_T::W_PAWN:   pieceChar = 'P'; break;
                case DPiece_T::W_ROOK:   pieceChar = 'R'; break;
                case DPiece_T::W_KNIGHT: pieceChar = 'N'; break;
                case DPiece_T::W_BISHOP: pieceChar = 'B'; break;
                case DPiece_T::W_QUEEN:  pieceChar = 'Q'; break;
                case DPiece_T::W_KING:   pieceChar = 'K'; break;

                case DPiece_T::B_PAWN:   pieceChar = 'p'; break;
                case DPiece_T::B_ROOK:   pieceChar = 'r'; break;
                case DPiece_T::B_KNIGHT: pieceChar = 'n'; break;
                case DPiece_T::B_BISHOP: pieceChar = 'b'; break;
                case DPiece_T::B_QUEEN:  pieceChar = 'q'; break;
                case DPiece_T::B_KING:   pieceChar = 'k'; break;
            }
            
            output << "| " << pieceChar << " ";
        }
        output << "|\n";
        output << "  +---+---+---+---+---+---+---+---+\n";
    }
    output << "    a   b   c   d   e   f   g   h\n\n";
    output << "                WHITE\n";
    return output.str();
}

void DBoard::notationToCoords(const std::string& notation, size_t& row, size_t& col) const {
    if(notation.length() != 2) {
        throw std::invalid_argument("Invalid notation: must be 2 characters (e.g., 'E4')");
    }

    char colChar = std::toupper(notation[0]);
    char rowChar = notation[1];

    // Convert column (A-H to 0-7)
    if(colChar < 'A' || colChar > 'H') {
        throw std::invalid_argument("Invalid column: must be A-H");
    }
    col = colChar - 'A';

    // Convert row (1-8 to 7-0, since board array is 0-indexed from top)
    if(rowChar < '1' || rowChar > '8') {
        throw std::invalid_argument("Invalid row: must be 1-8");
    }

    row = 8 - (rowChar - '0');
}

std::ostream& operator<<(std::ostream& output, const DBoard& dboard){
    output << dboard.dboardToString();
    return output;
}