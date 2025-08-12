#pragma once
#include "FENString.h"
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <queue>
#include "chess_engine/Types.h"

class Board;
class Square;
class Piece;

class ChessEngine {
    struct EngineID {
        std::string name;
        std::string author;
    };

    struct EngineOptionNames {
        std::string threads;
    };

    static const EngineID engineID;
    static const EngineOptionNames engineOptionNames;

    public:
        explicit ChessEngine(FENString fen);
        ~ChessEngine();
        std::string getFenStr() const;
        Game_Status isValidMove(MoveCoordsData move);
        Game_Status isValidMove(MoveCoordsData move, Piece_T promotionPiece);
        void uciStart();

        unsigned long int perft(unsigned int depth);
    private:
        FENString m_fen;
        std::unique_ptr<Board> m_board;
        
        // Threading support for UCI
        std::atomic<bool> m_shouldStop{false};
        std::atomic<bool> m_isPondering{false};
        std::thread m_signalThread;
        std::mutex m_commandMutex;
        std::condition_variable m_commandCV;
        std::queue<std::string> m_commandQueue;
        
        // UCI logging
        mutable std::ofstream m_uciLog;
        
        unsigned long int _perft(unsigned int depth, bool showMoves);
        unsigned long int _perftSingleThreaded(unsigned int depth);

        std::string _collectSignal() const;
        void _executeSignal(std::string signal);
        void _makeUciMove(const std::string& uciMove);
        std::string _getRandomValidMove() const;
        
        void _signalListener();
        void _processCommand(const std::string& command);
        void _logOutput(const std::string& output) const;

        void _printIdentity() const;
        void _printOptions() const;
        
};