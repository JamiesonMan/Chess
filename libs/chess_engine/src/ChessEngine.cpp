#include "chess_engine/ChessEngine.h"
#include "Board.h"

ChessEngine::ChessEngine(const FENString& fen) : m_originPosition{fen}, m_board{std::make_unique<Board>(fen)} {}

ChessEngine::~ChessEngine() = default;

Game_Status ChessEngine::isValidMove(MoveCoordsData move) const {
    Square& from = m_board->getBoardAt(move.fromRow, move.fromCol);
    Square& to = m_board->getBoardAt(move.toRow, move.toCol);

    const Piece* p = m_board->getPieceAt(move.fromRow, move.fromCol);
    
    if(m_board->isLegalMove(from, to, p)){
        return m_board->moveTo(from, to); // Make Move to get Game Status
    } else {
        return Game_Status::INVALID;
    }
}
