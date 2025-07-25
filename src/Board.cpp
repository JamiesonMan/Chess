#include "Board.h"
#include "Colors.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <cmath>
#include <limits>

#include "Pawn.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"


Board::Board() 
    : Board({{
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
        }}) {}

Board::Board(const std::array<std::array<char, MAX_COLS>, MAX_ROWS>& initBoardMapping) 
    : m_checkToResetEnPassant{false} 
{
    board = std::array<std::array<Square, MAX_COLS>, MAX_ROWS>();

    // Initialize pieces array to nullptr
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        for(size_t col = 0; col < MAX_COLS; ++col) {
            pieces[row][col] = nullptr;
        }
    }

    // Single loop for both squares and pieces
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        for(size_t col = 0; col < MAX_COLS; ++col) {
            // Create square with alternating colors
            Color_T squareColor = ((row + col) % 2 == 0) ? Color_T::WHITE : Color_T::BLACK;
            board[row][col] = Square{squareColor, static_cast<unsigned int>(row), static_cast<unsigned int>(col)};
            
            // Get piece character from mapping
            char pieceChar = initBoardMapping[row][col];
            
            // Create piece if square is not empty
            if(pieceChar != ' ') {
                Color_T pieceColor = std::isupper(pieceChar) ? Color_T::WHITE : Color_T::BLACK;
                pieces[row][col] = _createPiece(pieceChar, pieceColor, board[row][col]);
                getBoardAt(row, col).setOccupied(true);
            }
        }
    }
}

Square& Board::getBoardAt(size_t row, size_t col) {
    
    if(row >= MAX_ROWS || col >= MAX_COLS){
        throw std::invalid_argument("Error: Attempted to get a square off the board.");
    } else {
        return board[row][col];
    }
}

const Square& Board::getBoardAt(size_t row, size_t col) const {
    if(row >= MAX_ROWS || col >= MAX_COLS){
        throw std::invalid_argument("Error: Attempted to get a square off the board.");
    } else {
        return board[row][col];
    }
}

bool Board::getCheckToResetEnPassant() const{ return m_checkToResetEnPassant; }
void Board::setCheckToResetEnPassant(bool val) { m_checkToResetEnPassant = val; }

const std::array<std::array<Square, Board::MAX_COLS>, Board::MAX_ROWS>& Board::getBoard() const{
    return board;
}

std::array<std::array<Square, Board::MAX_COLS>, Board::MAX_ROWS>& Board::getBoard() {
    return board;
}

const Piece* Board::getPieceAt(size_t row, size_t col) const {
    if(row >= MAX_ROWS || col >= MAX_COLS) {
        return nullptr;
    }
    return pieces[row][col].get();
}

// Actual move execution
void Board::moveTo(Square& from, Square& to) {
    size_t fromRow = from.getRow();
    size_t fromCol = from.getCol();
    size_t toRow = to.getRow();
    size_t toCol = to.getCol();

    // Get the piece to move
    std::unique_ptr<Piece>& movingPiece = pieces[fromRow][fromCol];
    if(!movingPiece) {
        throw std::invalid_argument("Error: No piece at source square.");
    }

    // Validate the move
    if(!movingPiece->isValidMove(to)){
        throw std::invalid_argument("Invalid Move!");
    }

    // Check for en passant capture
    bool isEnPassant = false;
    if(movingPiece->getType() == Piece_T::PAWN) {
        // Check if this is a diagonal move to an empty square (potential en passant)
        bool isAttackMove = (fromCol != toCol);
        bool destinationEmpty = !to.isOccupied();
        
        if(isAttackMove && destinationEmpty) {
            // Look for enemy pawn on the same row as moving pawn that can be captured via en passant
            const Piece* potentialTarget = getPieceAt(fromRow, toCol);
            if(potentialTarget && potentialTarget->getType() == Piece_T::PAWN) {
                const Pawn* targetPawn = dynamic_cast<const Pawn*>(potentialTarget);
                if(targetPawn->getEnPassantCaptureStatus() && 
                   targetPawn->getColor() != movingPiece->getColor()) {
                    isEnPassant = true;
                    
                    if(!getCheckToResetEnPassant()) {
                        throw std::invalid_argument("Invalid En Passant Move!");
                    }

                    // Remove the en passant target piece
                    pieces[fromRow][toCol].reset();
                    getBoardAt(fromRow, toCol).setOccupied(false);
                }
            }
        }
    }

    // Check if destination has a piece (normal capture)
    std::unique_ptr<Piece>& targetPiece = pieces[toRow][toCol];
    if(targetPiece && !isEnPassant) {
        // Normal capture: remove the target piece
        targetPiece.reset(); // This deletes the captured piece
    }

    // Move the piece
    pieces[toRow][toCol] = std::move(movingPiece); // Transfer ownership
    pieces[fromRow][fromCol] = nullptr;            // Clear source

    // Update the piece's position reference
    pieces[toRow][toCol]->setSquarePosition(to);

    // Update square occupancy
    from.setOccupied(false);
    to.setOccupied(true);
    
    // After any move occurs, if we should reset en passant, we will.
    if(getCheckToResetEnPassant()){
        for(size_t row{0}; row < 8; ++row){
            for(size_t col{0}; col < 8; ++col){
                if(pieces[toRow][toCol]->getType() == Piece_T::PAWN){
                    Pawn* pawn = dynamic_cast<Pawn*>(pieces[toRow][toCol].get());
                    if(pawn->getEnPassantCaptureStatus()){
                        pawn->setEnPassantCaptureStatus(false);
                    }
                }
            }
        }
        setCheckToResetEnPassant(false);
    }

    if(pieces[toRow][toCol]->getType() == Piece_T::PAWN){
        Pawn* pawn = dynamic_cast<Pawn*>(pieces[toRow][toCol].get());

        if(pawn->getEnPassantCaptureStatus()){
            pawn->setEnPassantCaptureStatus(false);
        } else {
            if(pawn->getColor() == Color_T::BLACK && toRow == 3 && pawn->getHasMoved() == false){ // If we just moved 2.
                pawn->setEnPassantCaptureStatus(true);
                setCheckToResetEnPassant(true);
            } else if (pawn->getColor() == Color_T::WHITE && toRow == 4 && pawn->getHasMoved() == false){
                pawn->setEnPassantCaptureStatus(true);
                setCheckToResetEnPassant(true);
            }
        }

        pawn->setHasMoved(true);

        // Handle pawn promotion
        if(pawnCanPromote(to, pieces[toRow][toCol]->getColor())){

            Color_T pawnColor = pieces[toRow][toCol]->getColor();

            Piece_T promotionValue = _promptForPromotion(pawnColor);

            pieces[toRow][toCol].reset(); // Free the pawn
            to.setOccupied(false); // Reset square occupied status

            switch(promotionValue){
                case Piece_T::QUEEN:
                    pieces[toRow][toCol] = std::make_unique<Queen>(Piece_T::QUEEN, pawnColor, to, *this);
                    break;
                case Piece_T::ROOK:
                    pieces[toRow][toCol] = std::make_unique<Rook>(Piece_T::ROOK, pawnColor, to, *this);
                    break;
                case Piece_T::BISHOP:
                    pieces[toRow][toCol] = std::make_unique<Bishop>(Piece_T::BISHOP, pawnColor, to, *this);
                    break;
                case Piece_T::KNIGHT:
                    pieces[toRow][toCol] = std::make_unique<Knight>(Piece_T::KNIGHT, pawnColor, to, *this);
                    break;
                default:
                    pieces[toRow][toCol] = std::make_unique<Queen>(Piece_T::QUEEN, pawnColor, to, *this);
            }
            
            to.setOccupied(true); // Set it back to occupied
        }
    }
}   

Piece_T Board::_promptForPromotion(Color_T pawnColor) const {
    std::ostringstream error{};
    unsigned int option{0};
    while(true){
        std::system("clear");
        std::cout << error.str() << std::endl;
        error.str(""); // reset error log.
        do {
            std::cout << "Select a piece type to promote to: \n\n";
            std::cout << "\t1. Queen\n";
            std::cout << "\t2. Rook\n";
            std::cout << "\t3. Bishop\n";
            std::cout << "\t4. Knight\n";

            try {
                std::cin >> option;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
                if(option > 4){error << "Option must be less than 4." << std::endl;}
            } catch (const std::exception& e){
                error << e.what() << "\n" << std::endl;
            }
            
        } while (option > 4);
        break;
    }

    switch(option){
        case 1:
            return Piece_T::QUEEN;
        case 2:
            return Piece_T::ROOK;
        case 3:
            return Piece_T::BISHOP;
        case 4:
            return Piece_T::KNIGHT;
        default:
            return Piece_T::QUEEN;
    }
}

// Move logic
bool Board::validPawnMove(const Square& from, const Square& to, Color_T pawnColor, bool hasMoved) const {

    MoveCoordsData moveData{from.getRow(), from.getCol(), to.getRow(), to.getCol()};
    
    if(!_prelimMoveCheck(moveData)){ return false; }

    if(_isTwoStepMove(pawnColor, moveData)){
        return _isValidTwoStepMove(pawnColor, moveData, to.isOccupied(), hasMoved);
    }

    if(_isOneStepMove(pawnColor, moveData)){
        return _isValidOneStepMove(pawnColor, moveData, to.isOccupied());
    }

    bool attackDirectionRight = false;

    if(_isAttackRightMove(pawnColor, moveData)){
        attackDirectionRight = true;
        if(_isValidEnPassant(pawnColor, moveData, to.isOccupied(), attackDirectionRight)){
            return true;
        } else if (_isValidAttackMove(pawnColor, moveData, to.isOccupied())) {
            return true;
        }
    } else if (_isAttackLeftMove(pawnColor, moveData)){
        attackDirectionRight = false;
        if(_isValidEnPassant(pawnColor, moveData, to.isOccupied(), attackDirectionRight)){
            return true;
        } else if (_isValidAttackMove(pawnColor, moveData, to.isOccupied())) {
            return true;
        }
    }
    return false;
}

bool Board::validBishopMove(const Square& from, const Square& to, Color_T bishopColor) const {
    MoveCoordsData moveData{from.getRow(), from.getCol(), to.getRow(), to.getCol()};

    if (!_prelimMoveCheck(moveData)) { return false; } // Check bounds and if moving to same spot.

    int deltaRow = static_cast<int>(moveData.toRow) - static_cast<int>(moveData.fromRow);
    int deltaCol = static_cast<int>(moveData.toCol) - static_cast<int>(moveData.fromCol);

    if(abs(deltaRow) != abs(deltaCol)){ return false; } // Not on diagonal

    const Piece* p = getPieceAt(moveData.toRow, moveData.toCol);
    if(p){
        if(p->getColor() == bishopColor){ return false; } // Cant take a friendly piece.
    }

    if(_checkDiagBlocked(moveData, deltaRow, deltaCol)){ return false; } // Something was in between.
     
    return true;
}

bool Board::validKnightMove(const Square& from, const Square& to, Color_T knightColor) const {
    MoveCoordsData moveData{from.getRow(), from.getCol(), to.getRow(), to.getCol()};

    if (!_prelimMoveCheck(moveData)) { return false; } // Check bounds and if moving to same spot.

    switch(from.getSquareColor()){
        case Color_T::BLACK:
            if(to.getSquareColor() != Color_T::WHITE) { return false; }
            break;
        case Color_T::WHITE:
            if(to.getSquareColor() != Color_T::BLACK) { return false; }
            break;
    }

    int deltaRow = static_cast<int>(moveData.toRow) - static_cast<int>(moveData.fromRow);
    int deltaCol = static_cast<int>(moveData.toCol) - static_cast<int>(moveData.fromCol); 

    if((abs(deltaCol) == 2 && abs(deltaRow) == 1)
    || (abs(deltaCol) == 1 && abs(deltaRow) == 2)){
        const Piece* p = getPieceAt(moveData.toRow, moveData.toCol);

        if(p){
            if(p->getColor() == knightColor){ return false; } // Cant take a friendly piece.
        }

        return true;
    }

    return false;
}

bool Board::validRookMove(const Square& from, const Square& to, Color_T rookColor) const {
    MoveCoordsData moveData{from.getRow(), from.getCol(), to.getRow(), to.getCol()};

    if (!_prelimMoveCheck(moveData)) { return false; } // Check bounds and if moving to same spot.

    int deltaRow = static_cast<int>(moveData.toRow) - static_cast<int>(moveData.fromRow);
    int deltaCol = static_cast<int>(moveData.toCol) - static_cast<int>(moveData.fromCol);

    if(!((moveData.fromCol == moveData.toCol) || (moveData.fromRow == moveData.toRow))){ return false; } // Not on rank or file

    const Piece* p = getPieceAt(moveData.toRow, moveData.toCol);
    if(p){
        if(p->getColor() == rookColor){ return false; } // Cant take a friendly piece.
    }

    if(_checkRankFileBlocked(moveData, deltaRow, deltaCol)){ return false; } // Something was in between.
     
    return true;
}

bool Board::validQueenMove(const Square& from, const Square& to, Color_T queenColor) const {
    MoveCoordsData moveData{from.getRow(), from.getCol(), to.getRow(), to.getCol()};

    if (!_prelimMoveCheck(moveData)) { return false; } // Check bounds and if moving to same spot.

    int deltaRow = static_cast<int>(moveData.toRow) - static_cast<int>(moveData.fromRow);
    int deltaCol = static_cast<int>(moveData.toCol) - static_cast<int>(moveData.fromCol);

    if(!((moveData.fromCol == moveData.toCol) 
        || (moveData.fromRow == moveData.toRow)
        || (abs(deltaRow) == abs(deltaCol))))
        { return false; } // Not on rank or file

    const Piece* p = getPieceAt(moveData.toRow, moveData.toCol);
    if(p){
        if(p->getColor() == queenColor){ return false; } // Cant take a friendly piece.
    }

    if(_checkRankFileBlocked(moveData, deltaRow, deltaCol) && _checkDiagBlocked(moveData, deltaRow, deltaCol)){ return false; } // Something was in between.
     
    return true;
}

bool Board::validKingMove(const Square& from, const Square& to, Color_T kingColor) const {
    MoveCoordsData moveData{from.getRow(), from.getCol(), to.getRow(), to.getCol()};

    if (!_prelimMoveCheck(moveData)) { return false; } // Check bounds and if moving to same spot.

    int deltaRow = static_cast<int>(moveData.toRow) - static_cast<int>(moveData.fromRow);
    int deltaCol = static_cast<int>(moveData.toCol) - static_cast<int>(moveData.fromCol);

    if(!(abs(deltaRow) <= 1 && abs(deltaCol) <= 1)) { // King can only move 1 square in any direction
        return false; 
    } 

    const Piece* p = getPieceAt(moveData.toRow, moveData.toCol);
    if(p){
        if(p->getColor() == kingColor){ return false; } // Cant take a friendly piece.
    }
     
    return true;
}

bool Board::_checkRankFileBlocked(const MoveCoordsData& moveData, int deltaRow, int deltaCol) const {
    int rankDir{0};
    int fileDir{0};

    if(deltaRow == 0){
        if(deltaCol > 0){
            ++fileDir; // Right
        } else {
            --fileDir; // Left
        }
    } else {
        if(deltaRow > 0){
            ++rankDir; // Down
        } else {
            --rankDir; // Up
        }
    }
    
    int row = static_cast<int>(moveData.fromRow) + rankDir;
    int col = static_cast<int>(moveData.fromCol) + fileDir;
    int toRow = static_cast<int>(moveData.toRow);
    int toCol = static_cast<int>(moveData.toCol);
    
    // Check path until we reach destination (exclusive)
    if(deltaRow == 0){
        while (col != toCol) {
            const Piece* p = getPieceAt(static_cast<size_t>(row), static_cast<size_t>(col));
            if (p) {
                return true; // Path is blocked
            }
            col += fileDir;
        }
    } else {
        while (row != toRow) {
            const Piece* p = getPieceAt(static_cast<size_t>(row), static_cast<size_t>(col));
            if (p) {
                return true; // Path is blocked
            }
            row += rankDir;
        }
    }

    
    
    return false; // Path is clear
}

bool Board::_checkDiagBlocked(const MoveCoordsData& moveData, int deltaRow, int deltaCol) const {
    // Determine direction
    int rowStep = (deltaRow > 0) ? 1 : -1;
    int colStep = (deltaCol > 0) ? 1 : -1;
    
    int row = static_cast<int>(moveData.fromRow) + rowStep;
    int col = static_cast<int>(moveData.fromCol) + colStep;
    int toRow = static_cast<int>(moveData.toRow);
    int toCol = static_cast<int>(moveData.toCol);
    
    // Check path until we reach destination (exclusive)
    while (row != toRow && col != toCol) {
        const Piece* p = getPieceAt(static_cast<size_t>(row), static_cast<size_t>(col));
        if (p) {
            return true; // Path is blocked
        }
        row += rowStep;
        col += colStep;
    }
    
    return false; // Path is clear
}

bool Board::_prelimMoveCheck(const MoveCoordsData& moveData) const {
    if(moveData.fromCol > 7 || moveData.fromRow > 7 || moveData.toRow > 7 || moveData.toCol > 7) {
        return false;
    }

    // Cannot move to the current position.
    if(moveData.fromCol == moveData.toCol && moveData.fromRow == moveData.toRow){ return false; }

    return true;
}


bool Board::pawnCanPromote(const Square& to, Color_T color) const {
    switch(color){
        case Color_T::BLACK:
            if(to.getRow() != (MAX_ROWS - 1)){
                return false;
            }
            break;
        case Color_T::WHITE:
            if(to.getRow() != 0){
                return false;
            }
            break;
    }

    return true;
}

bool Board::_isValidTwoStepMove(Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied, bool pawnHasMoved) const{  

    if(pawnHasMoved) { // Start row for pawn black pawn
        std::cout << "Pawn has already moved" << std::endl;
        return false; 
    } // Can only two step once.

    switch(pawnColor){
        case Color_T::BLACK: {
            // Check bounds of to square and get middle square (0-7 indexing)
            if(moveData.toRow > 7 || moveData.toCol != moveData.fromCol) return false;

            const Piece* midPiece = getPieceAt(moveData.fromRow + 1, moveData.fromCol);
            bool midSquareOccupied = (midPiece != nullptr);

            
            if(!toSquareOccupied && !midSquareOccupied){ // The two spaces are free to move in.
                // Need to still implement a function to determine if it would put the king in check.
                return true; 
            }
        
            return false;
        }
        
        case Color_T::WHITE: {
            // Check bounds for to square and get middle square (0-7 indexing)
            if(moveData.toRow > 4 || moveData.toCol != moveData.fromCol) { return false; }
            const Piece* midPiece = getPieceAt(moveData.fromRow - 1, moveData.fromCol);
            bool midSquareOccupied = (midPiece != nullptr);

            
            if(!toSquareOccupied && !midSquareOccupied){ // The two spaces are free to move in.
                // *** Need to still implement a function to determine if it would put the kind in check. ***
                return true; 
            }
            
            return false;
        }
    }
}

// Confirm that we are moving 2 rows up or down (black or white) and we remain in same col.
bool Board::_isTwoStepMove(Color_T pawnColor, const MoveCoordsData& moveData) const{
    if(pawnColor == Color_T::BLACK) {
        // Black pawns move down (increasing row)
        return (moveData.toRow == (moveData.fromRow + 2)) && (moveData.toCol == moveData.fromCol);
    } else {
        // White pawns move up (decreasing row)
        return (moveData.toRow == (moveData.fromRow - 2)) && (moveData.toCol == moveData.fromCol);
    }
}

bool Board::_isValidOneStepMove(Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied) const{  

    switch(pawnColor){
        case Color_T::BLACK: {
            // Check bounds of to square and get middle square (0-7 indexing)
            if(moveData.toRow > 7 || moveData.toCol != moveData.fromCol) return false;
            
            if(!toSquareOccupied){
                // Need to still implement a function to determine if it would put the king in check.
                return true; 
            }
        
            return false;
        }
        
        case Color_T::WHITE: {
            // Check bounds for to square (0-7 indexing)
            if(moveData.toRow > 7 || moveData.toCol != moveData.fromCol) { return false; }
            
            if(!toSquareOccupied){ 
                // *** Need to still implement a function to determine if it would put the kind in check. ***
                return true; 
            }
            
            return false;
        }
    }
}

// Confirm that we are moving 2 rows up or down (black or white) and we remain in same col.
bool Board::_isOneStepMove(Color_T pawnColor, const MoveCoordsData& moveData) const{
    if(pawnColor == Color_T::BLACK) {
        // Black pawns move down (increasing row)
        return (moveData.toRow == (moveData.fromRow + 1)) && (moveData.toCol == moveData.fromCol);
    } else {
        // White pawns move up (decreasing row)
        return (moveData.toRow == (moveData.fromRow - 1)) && (moveData.toCol == moveData.fromCol);
    }
}

// Attack moves
bool Board::_isValidAttackMove(Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied) const{

    switch(pawnColor){
        case Color_T::BLACK: {
            // Check bounds of to square (0-7 indexing)
            if(moveData.toRow > 7 || moveData.toCol > 7) return false;
            
            if(!toSquareOccupied){ 
                // Need to still implement a function to determine if it would put the king in check.
                return false; 
            }

            const Piece* toPiece = getPieceAt(moveData.toRow, moveData.toCol);
            if(toPiece->getColor() == Color_T::BLACK){ // Friendly fire will not be tolerated :(.
                return false;
            }

            return true;
        }
        
        case Color_T::WHITE: {
            // Check bounds for to square and get middle square (0-7 indexing)
            if(moveData.toRow > 5 || moveData.toCol > 7 ) { return false; }
            
            if(!toSquareOccupied){ 
                // Need to still implement a function to determine if it would put the king in check.
                return false; 
            }

            const Piece* toPiece = getPieceAt(moveData.toRow, moveData.toCol);
            if(toPiece->getColor() == Color_T::WHITE){ // Friendly fire will not be tolerated :(.
                return false;
            }

            return true;
        }
    }
}

bool Board::_isAttackRightMove(Color_T pawnColor, const MoveCoordsData& moveData) const{

    if(pawnColor == Color_T::BLACK) {
        // Black pawns move down (increasing row)
        return (moveData.toRow == (moveData.fromRow + 1)) && (moveData.toCol == moveData.fromCol - 1);  
    } else {
        // White pawns move up (decreasing row)
        return (moveData.toRow == (moveData.fromRow - 1)) && (moveData.toCol == moveData.fromCol + 1);
    }
}

bool Board::_isAttackLeftMove(Color_T pawnColor, const MoveCoordsData& moveData) const {
    if(pawnColor == Color_T::BLACK) {
        // Black pawns move down (increasing row)
        return (moveData.toRow == (moveData.fromRow + 1)) && (moveData.toCol == moveData.fromCol + 1);  
    } else {
        // White pawns move up (decreasing row)
        return (moveData.toRow == (moveData.fromRow - 1)) && (moveData.toCol == moveData.fromCol - 1);
    }
}

bool Board::_isValidEnPassant(Color_T pawnColor, const MoveCoordsData& moveData, bool toSquareOccupied, bool directionRight) const {
    
    switch(pawnColor){
        case Color_T::BLACK: {
            // Check bounds of to square (0-7 indexing)
            if(moveData.toRow > 7 || moveData.toCol > 7) { return false; }
            
            // Impossible to en passant if a piece is on landing square.
            if(toSquareOccupied){ 
                return false; 
            }

            const Piece* p = getPieceAt(moveData.toRow - 1, moveData.toCol); // Piece to possibly capture.

            if(p){ // If p is a piece.
                if(p->getType() == Piece_T::PAWN){ // If p is a pawn.
                    const Pawn* pawnToCapture = dynamic_cast<const Pawn*>(p);
                    if(pawnToCapture->getEnPassantCaptureStatus() && pawnToCapture->getColor() == Color_T::WHITE){
                        return true;
                    }
                }
            }

            return false;
        }
        
        case Color_T::WHITE: {
            // Check bounds of to square (0-7 indexing)
            if(moveData.toRow > 7 || moveData.toCol > 7) { return false; }
            
            // Impossible to en passant if a piece is on landing square.
            if(toSquareOccupied){ return false; }

            const Piece* p = getPieceAt(moveData.toRow + 1, moveData.toCol); // Piece to possibly capture.

            if(p){ // If p is a piece.
                if(p->getType() == Piece_T::PAWN){ // If p is a pawn.
                    const Pawn* pawnToCapture = dynamic_cast<const Pawn*>(p);
                    if(pawnToCapture->getEnPassantCaptureStatus() && pawnToCapture->getColor() == Color_T::BLACK){
                        return true;
                    }
                }
            }

            return false;
        }
    }
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

    unsigned int row = square.getRow();
    switch(type) {
        case Piece_T::ROOK:   
            return std::make_unique<Rook>(type, color, square, *this);
        case Piece_T::KNIGHT: 
            return std::make_unique<Knight>(type, color, square, *this);
        case Piece_T::BISHOP:
            return std::make_unique<Bishop>(type, color, square, *this);
        case Piece_T::QUEEN: 
            return std::make_unique<Queen>(type, color, square, *this); 
        case Piece_T::KING:   
            return std::make_unique<King>(type, color, square, *this); 
        case Piece_T::PAWN:
            if(color == Color_T::BLACK && row != 1){
                std::unique_ptr<Pawn> p = std::make_unique<Pawn>(type, color, square, *this);
                p->setHasMoved(true);
                return p;
            } else if (color == Color_T::WHITE && row != 6){
                std::unique_ptr<Pawn> p = std::make_unique<Pawn>(type, color, square, *this);
                p->setHasMoved(true);
                return p;
            } else {
                return std::make_unique<Pawn>(type, color, square, *this);
            }
            
        default: throw std::invalid_argument("Invalid piece type");
    }
}

void Board::printBoard() const {
    std::cout << this->boardToString();
}

std::string Board::boardToString() const {
    std::ostringstream output;
    output << "                BLACK\n\n";
    output << "  +---+---+---+---+---+---+---+---+\n";
    
    // Traverse board.
    for(size_t row = 0; row < MAX_ROWS; ++row) {
        output << (MAX_ROWS - row) << " ";
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
            
            output << "| " << pieceChar << " ";
        }
        output << "|\n";
        output << "  +---+---+---+---+---+---+---+---+\n";
    }
    output << "    a   b   c   d   e   f   g   h\n\n";
    output << "                WHITE\n";
    return output.str();
}

void Board::notationToCoords(const std::string& notation, unsigned int& row, unsigned int& col) const {
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

std::ostream& operator<<(std::ostream& output, const Board& board){
    output << board.boardToString();
    return output;
}