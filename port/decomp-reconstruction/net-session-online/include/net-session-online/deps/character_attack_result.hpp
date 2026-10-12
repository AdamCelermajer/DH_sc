// Stand-in for Character::AttackResult (owned by the character code, not in this category).
// The layout is not recovered. The original copies it as ten 32-bit words (40 bytes), so the
// stand-in keeps that shape and nothing else.
#pragma once

#include <cstdint>

namespace Character {

struct AttackResult {
    std::uint32_t words[10] = {};
};

}  // namespace Character
