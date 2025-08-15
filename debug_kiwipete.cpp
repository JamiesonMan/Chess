#include "libs/chess_engine/include/chess_engine/ChessEngine.h"
#include "libs/chess_engine/include/chess_engine/FENString.h"
#include <iostream>
#include <chrono>

int main() {
    // Kiwipete position
    std::string kiwipeteFen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    
    std::cout << "Testing Kiwipete position: " << kiwipeteFen << std::endl;
    
    try {
        FENString fen(kiwipeteFen);
        ChessEngine engine(fen);
        
        std::cout << "Running perft(5)..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        
        unsigned long result = engine.perft(5);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "\nFinal result: " << result << std::endl;
        std::cout << "Expected: 193690690" << std::endl;
        std::cout << "Difference: " << (long long)result - 193690690LL << std::endl;
        std::cout << "Time: " << duration.count() << "ms" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}