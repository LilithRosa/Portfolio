// CR Header.
#pragma once

struct Context {
    int x = 0;;
};

// Handler forward declarations.
constexpr void handle_idle(Context&);
constexpr void handle_move(Context&);
constexpr void handle_attack(Context&);
constexpr void handle_heal(Context&);
constexpr void handle_cast(Context&);
constexpr void handle_jump(Context&);
constexpr void handle_block(Context&);
constexpr void handle_roll(Context&);

// Handler definitions.
constexpr void handle_idle(Context& c)    { c.x += 1; }
constexpr void handle_move(Context& c)    { c.x += 2; }
constexpr void handle_attack(Context& c)  { c.x += 3; }
constexpr void handle_heal(Context& c)    { c.x += 4; }
constexpr void handle_cast(Context& c)    { c.x += 5; }
constexpr void handle_jump(Context& c)    { c.x += 6; }
constexpr void handle_block(Context& c)   { c.x += 7; }
constexpr void handle_roll(Context& c)    { c.x += 8; }

// EOF