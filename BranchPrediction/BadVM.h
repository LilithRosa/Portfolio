// CR Header.
#pragma once
#include <cstdint>
#include <iostream>
#include "VMData.h"

// Bad Execution Method.
inline void ExecuteBad(const uint8_t* data, const int32_t size) {
    Context ctx;

    for (int i = 0; i < size; ++i) {
        uint8_t opcode = data[i] & 7;   // Ensure numbers 0-7 used.
        
        // Call handlers through switch, to cause branch predictions.
        switch (opcode) {
            case 0: handle_idle(ctx); break;
            case 1: handle_move(ctx); break;
            case 2: handle_attack(ctx); break;
            case 3: handle_heal(ctx); break;
            case 4: handle_cast(ctx); break;
            case 5: handle_jump(ctx); break;
            case 6: handle_block(ctx); break;
            case 7: handle_roll(ctx); break;
        }
    }

    std::cout << "Final ctx.x = " << ctx.x << "\n";
}

// EOF