// CR Header.
#pragma once
#include <cstdint>
#include <iostream>
#include "VMData.h"

// Branchless dispatch table.
constexpr void (*handlers[8])(Context&) = {
    handle_idle,
    handle_move,
    handle_attack,
    handle_heal,
    handle_cast,
    handle_jump,
    handle_block,
    handle_roll
};

// Good Execution Method.
inline void ExecuteGood(const uint8_t* data, const int32_t size) {
    Context ctx;

    for (int i = 0; i < size; ++i) {
        uint8_t opcode = data[i] & 7;   // Ensured numbers 0-7 used.
    
        // Call handlers through dispatch table, to avoid branch predictions.
        handlers[opcode](ctx);
    }

    std::cout << "Final ctx.x = " << ctx.x << "\n";
}

// EOF