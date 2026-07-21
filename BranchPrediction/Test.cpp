// CR Header.
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>
#include <cstdlib>

#include "BadVM.h"
#include "GoodVM.h"

// Forward declarations of your two executors
void ExecuteBad(const uint8_t* data, int32_t size);
void ExecuteGood(const uint8_t* data, int32_t size);

int main() {
    const int32_t size = 10'000'000;   // 10 million opcodes
    std::vector<uint8_t> data(size);

    // Generate random opcodes 0–7
    for (int32_t i = 0; i < size; ++i)
        data[i] = rand() % 8;

    // --- BAD VERSION TIMING ---
    auto startBad = std::chrono::high_resolution_clock::now();
    ExecuteBad(data.data(), size);
    auto endBad = std::chrono::high_resolution_clock::now();

    auto badNs = std::chrono::duration_cast<std::chrono::nanoseconds>(endBad - startBad).count();
    std::cout << "Bad version time: " << badNs << " ns\n";


    // --- GOOD VERSION TIMING ---
    auto startGood = std::chrono::high_resolution_clock::now();
    ExecuteGood(data.data(), size);
    auto endGood = std::chrono::high_resolution_clock::now();

    auto goodNs = std::chrono::duration_cast<std::chrono::nanoseconds>(endGood - startGood).count();
    std::cout << "Good version time: " << goodNs << " ns\n";

    return 0;
}

// EOF