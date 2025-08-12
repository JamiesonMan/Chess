#include "chess_engine/FENString.h"
#include "chess_engine/Types.h"
#include <stdexcept>
#include <array>
#include <algorithm>
#include <cctype>
#include <iostream>

std::ostream& operator<<(std::ostream& output, const FENString& fen){
    output << fen.getFen();
    return output;
}

FENString::FENString(std::string fen) {
        std::string boardString;
        char activeTurn;
        std::string castlingRights;
        std::string enPassantTarget;
        unsigned int halfMoveClock;
        unsigned int totalMoves;

    if(_isValidFen(fen, boardString, activeTurn, castlingRights, enPassantTarget, halfMoveClock, totalMoves)){
        m_fen = fen;  // Store the original FEN string
        m_boardString = boardString;
        m_activeTurn = activeTurn;
        m_castlingRights = castlingRights;
        m_enPassantTarget = enPassantTarget;
        m_halfMoveClock = halfMoveClock;
        m_totalMoves = totalMoves;
    } else {
        throw std::invalid_argument("Error: Invalid FEN!");
    }
}

char FENString::getPosDelim() const {
    return m_posDelim;
}


size_t FENString::__isValidFenBoard(std::string& fen, std::string& boardString) {    
    size_t delimOccuranceCount{0};
    size_t occurance{0};
    const size_t maxNumOfPosDelim{MAX_ROWS - 1};

    while(delimOccuranceCount != maxNumOfPosDelim){
        size_t val = fen.find(m_posDelim, occurance);
        if(val == std::string::npos){
            return std::string::npos;
        } else {
            ++delimOccuranceCount;
            occurance = val;
        }
    }

    size_t numValidRanks{0};
    size_t rankCount{0};
    size_t currCharIndex{0};

    while(numValidRanks < MAX_ROWS){
        rankCount = 0; // Reset rank count for each new rank
        while(rankCount < MAX_COLS && currCharIndex < fen.length()){
            char curr = fen.at(currCharIndex);
            if(curr == m_posDelim){ // End of rank
                break;
            } else if(std::find(m_charPieces.begin(), m_charPieces.end(), curr) != m_charPieces.end()) { // Found a valid piece
                ++rankCount;
            } else if (std::isdigit(curr)){ // Valid digit
                size_t val = curr - '0';

                if(val > MAX_COLS || val < 1){
                    return std::string::npos;
                } else {
                    rankCount += val;
                }
            } else {
                return std::string::npos;
            }
            ++currCharIndex;
        }
        if(rankCount == MAX_COLS){
            ++numValidRanks;
            if(numValidRanks < MAX_ROWS && currCharIndex < fen.length() && fen.at(currCharIndex) == m_posDelim){
                ++currCharIndex; // Skip the '/' delimiter
            }
        } else {
            return std::string::npos;
        }
    }
    if(numValidRanks == MAX_ROWS){
        size_t spaceDelim = fen.find(m_spaceDelim);
        boardString = fen.substr(0, spaceDelim);
        return occurance;
    } else {
        return std::string::npos;
    } 
}

size_t FENString::__isValidActiveTurn(std::string& fen, char& activeTurn) {
    size_t val = fen.find(m_spaceDelim, m_lastDelim + 1);
    if(val == std::string::npos){ return std::string::npos; }
    if(std::find(m_activeTurns.begin(), m_activeTurns.end(), fen.at(val + 1)) != m_activeTurns.end()){
        activeTurn = fen.at(val + 1);
        return val;
    } else {
        return std::string::npos;
    }
}

size_t FENString::__isValidCastleRights(std::string& fen, std::string& castlingRights) {
    size_t val = fen.find(m_spaceDelim, m_lastDelim + 1); 
    size_t nextDelim = fen.find(m_spaceDelim, val + 1);
    std::string castlingRightsSection = fen.substr(val + 1, nextDelim - val - 1);
    size_t totalChar{0};
    for(char c : castlingRightsSection){
        if(std::find(m_castleRights.begin(), m_castleRights.end(), c) != m_castleRights.end()){
            ++totalChar;
        } else {
            return std::string::npos;
        }
    }

    if(totalChar >= 1 && totalChar <= 4){
        castlingRights = castlingRightsSection;
        return val;
    } else {
        return std::string::npos;
    }
}

size_t FENString::__isValidEnPassantTarget(std::string& fen, std::string& enPassantTarget) {
    size_t val = fen.find(m_spaceDelim, m_lastDelim + 1);
    char firstChar{fen.at(val + 1)};
    
    // Handle '-' case (no en passant target)
    if(firstChar == '-') {
        enPassantTarget = "-";
        return val;
    }
    
    // Handle valid square notation (e.g., "e3")
    if(val + 2 >= fen.length()) return std::string::npos;
    
    char file{firstChar};
    char rank{fen.at(val + 2)};

    bool validFile{false};
    bool validRank{false};
    if(file >= 'a' && file <= 'h') {
        validFile = true;
    }

    if(std::isdigit(rank)){
        if(rank >= '1' && rank <= '8'){
            validRank = true;
        }
    }
    enPassantTarget = {file, rank};
    return (validRank && validFile)? val : std::string::npos;
}

size_t FENString::__isValidHalfMove(std::string& fen, unsigned int& halfMoves) {
    size_t val = fen.find(m_spaceDelim, m_lastDelim + 1);
    std::string halfMoveData = fen.substr(val + 1, 2);
    char numberOfHalfMoves{halfMoveData.at(0)};
    char secondValCheck{halfMoveData.at(1)};
    size_t num{0};

    if(numberOfHalfMoves == '-'){
        return std::string::npos;
    }

    if(std::isdigit(numberOfHalfMoves)){
        num = numberOfHalfMoves - '0';
    }

    if(std::isdigit(secondValCheck)){ 
        num *= 10;
        num += secondValCheck - '0';
    } else if (secondValCheck != m_spaceDelim){
        return std::string::npos;
    }   
    halfMoves = num;
    return (num <= MAX_HALF_MOVES)? val : std::string::npos;
}

size_t FENString::__isValidTotalMove(std::string& fen, unsigned int& totalMoves) {
    size_t val = fen.find(m_spaceDelim, m_lastDelim + 1);
    std::string totalMoveData = fen.substr(val + 1);
    size_t num{0};
    size_t digitPlaceShift{1};
    for(char digit : totalMoveData){
        if(!isdigit(digit)){
            return std::string::npos;
        }
        num *= digitPlaceShift;
        num += digit - '0';
        digitPlaceShift *= 10;
    } 
    totalMoves = num;
    return val;
}

// -------------------1----------------------- 2 --3- 4 5 6
// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
bool FENString::_isValidFen(std::string& fen, std::string& boardString, char& activeTurn,
                            std::string& castlingRights, std::string& enPassantTarget, 
                            unsigned int& halfMoves, unsigned int& totalMoves) {

    if(fen.length() > MAX_FEN_LENGTH || fen.length() < MIN_FEN_LENGTH) { return false; } // [20-100]
    if(fen.empty()){ return false; }

    size_t spaceDelimOccuranceCount{0};
    size_t index{0};

    while(spaceDelimOccuranceCount != maxNumOfSpaceDelim){
        size_t val = fen.find(m_spaceDelim, index);
        if(val == std::string::npos){
            return false;
        } else {
            ++spaceDelimOccuranceCount;
            index = val;
        }
    }

    size_t returnedVal{0};
    returnedVal = __isValidFenBoard(fen, boardString);
    if(returnedVal == std::string::npos) { return false; } else { m_lastDelim = returnedVal; }

    returnedVal = __isValidActiveTurn(fen, activeTurn);
    if(returnedVal == std::string::npos) { return false; } else { m_lastDelim = returnedVal; }

    returnedVal = __isValidCastleRights(fen, castlingRights);
    if(returnedVal == std::string::npos) { return false; } else { m_lastDelim = returnedVal; }

    returnedVal = __isValidEnPassantTarget(fen, enPassantTarget);
    if(returnedVal == std::string::npos) { return false; } else { m_lastDelim = returnedVal; }

    returnedVal = __isValidHalfMove(fen, halfMoves);
    if(returnedVal == std::string::npos) { return false; } else { m_lastDelim = returnedVal; }

    returnedVal = __isValidTotalMove(fen, totalMoves);
    if(returnedVal == std::string::npos) { return false; } else { m_lastDelim = returnedVal; }

    return true;
} 
        
const std::string& FENString::getFen() const { return m_fen; }
void FENString::setFen(std::string newFen) { 
    std::string boardString;
    char activeTurn;
    std::string castlingRights;
    std::string enPassantTarget;
    unsigned int halfMoveClock;
    unsigned int totalMoves;

    if(_isValidFen(newFen, boardString, activeTurn, castlingRights, enPassantTarget, halfMoveClock, totalMoves)){
        m_fen = newFen;
        m_boardString = boardString;
        m_activeTurn = activeTurn;
        m_castlingRights = castlingRights;
        m_enPassantTarget = enPassantTarget;
        m_halfMoveClock = halfMoveClock;
        m_totalMoves = totalMoves;
    } else {
        throw std::invalid_argument("Error: Invalid FEN in setFen!");
    }
}

const std::string& FENString::getBoardStr() const { return m_boardString; }
void FENString::setBoardStr(std::string newBoardStr) { m_boardString = newBoardStr; }

char FENString::getActiveTurn() const { return m_activeTurn; }
void FENString::setActiveTurn(char newActiveTurn) { m_activeTurn = newActiveTurn; }

const std::string& FENString::getCastlingRightsStr() const { return m_castlingRights; }
void FENString::setCastlingRights(std::string newCastlingRights) { m_castlingRights = newCastlingRights; }

const std::string& FENString::getEnPassantTarget() const { return m_enPassantTarget; }
void FENString::setEnPassantTarget(std::string newEnPassantTarget) { m_enPassantTarget = newEnPassantTarget; }

unsigned int FENString::getHalfMoveClock() const { return m_halfMoveClock; }
void FENString::setHalfMoveClock(unsigned int newHalfMoveClock) { m_halfMoveClock = newHalfMoveClock;}

unsigned int FENString::getTotalMoves() const { return m_totalMoves; }
void FENString::setTotalMoves(unsigned int newTotalMoves) { m_totalMoves = newTotalMoves; }