#pragma once
#include <string>
#include <array>
// -------------------1----------------------- 2 --3- 4 5 6
// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
/*
    Parts 1-6 of a FEN String:  
        1. Board Position
            a. white pieces are capitalized, black is lower.
            b. defines the state of each rank from black's side to white's side.
                Any empty squares within the rank are described numerically.
                e.g. ppp1pppp means 3 pawns a single space and then 4 more pawns for that rank.
                Ranks are seperated by delim char '/'

        2. Active Turn
            a. The color of the current turn. w for white. b for black.

        3. Castling rights
            a. Determines for each color, if they could still castle in this game.
            b. K means white can castle king side (short castle). Q means white can castle queen side (long castle)
                k and q respectfully mean the same except for black.
            c. '-' is used to define that neither color can castle.

        4. En Passant Tracking
            a. Determines if there exists a target square for En Passant.
            b. Square is represented in Algebraic Notation.
            c. Only shows a target square if the last move made was a pawn two step.\
            d. If the last move wasnt a pawn two step. Then it can be left blank, '-'
        
        5. Halfmove Clock
            a. The number of halfmoves since the last capture or pawn advance, used for the fifty-move rule.
        
        6. Current Total Number of Moves
            a. Keeps track of the total number of moves that have occured.
*/
class FENString {

    friend std::ostream& operator<<(std::ostream& output, const FENString& fen);

    public:
        FENString(std::string);
        
        const std::string& getFen() const;
        void setFen(std::string);

        char getPosDelim() const;

        const std::string& getBoardStr() const;
        void setBoardStr(std::string);

        char getActiveTurn() const;
        void setActiveTurn(char);
        
        const std::string& getCastlingRightsStr() const;
        void setCastlingRights(std::string);

        const std::string& getEnPassantTarget() const;
        void setEnPassantTarget(std::string);

        unsigned int getHalfMoveClock() const;
        void setHalfMoveClock(unsigned int);

        unsigned int getTotalMoves() const;
        void setTotalMoves(unsigned int);

        static constexpr std::array<char, 12> m_charPieces{'r', 'n', 'b', 'q', 'k', 'p', 'R', 'N', 'B', 'Q', 'K', 'P'};
        static constexpr std::array<char, 5> m_castleRights{'k', 'q', 'K', 'Q', '-'};
        static constexpr std::array<char, 3> m_activeTurns{'w', 'b', '-'};
        static constexpr size_t MIN_FEN_LENGTH{19};
        static constexpr size_t MAX_FEN_LENGTH{100};
        static constexpr size_t maxNumOfSpaceDelim{5};
        static constexpr size_t MAX_HALF_MOVES{200}; // Allow games to continue past 50-move rule
        static constexpr std::string_view INIT_FEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

    private:
        std::string m_fen;

        std::string m_boardString;
        char m_activeTurn;
        std::string m_castlingRights;
        std::string m_enPassantTarget;
        unsigned int m_halfMoveClock;
        unsigned int m_totalMoves;

        size_t m_lastDelim{0};

        static constexpr char m_posDelim{'/'};
        static constexpr char m_spaceDelim{' '};
        static constexpr char m_nullChar{'-'}; 

        // determines if a fen string is valid, and populates references with the values.
        bool _isValidFen(std::string& fen, std::string& boardString, char& activeTurn,
                        std::string& castlingRights, std::string& enPassantTarget, 
                        unsigned int& halfMoves, unsigned int& totalMoves);
        
        size_t __isValidFenBoard(std::string& fen, std::string& boardString);
        size_t __isValidActiveTurn(std::string& fen, char& activeTurn);
        size_t __isValidCastleRights(std::string& fen, std::string& castlingRights);
        size_t __isValidEnPassantTarget(std::string& fen, std::string& enPassantTarget);
        size_t __isValidHalfMove(std::string& fen, unsigned int& halfMoves);
        size_t __isValidTotalMove(std::string& fen, unsigned int& totalMoves);
};  