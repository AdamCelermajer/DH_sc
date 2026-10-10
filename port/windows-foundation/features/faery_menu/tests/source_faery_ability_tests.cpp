#include "../source_faery_ability_binding_v1.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    using dh::foundation::faery_menu::CastAnimationV1;
    using dh::foundation::faery_menu::source_cast_sequence_facts_v1;
    const std::vector<std::int32_t> spells{31, -1, 0x7fffffff};
    CastAnimationV1 value{};
    value.animation_override = 0xdeadbeefu;
    if (source_cast_sequence_facts_v1(spells, 17, -1, 0x400000, 3, value) ||
        value.animation_override != 0xdeadbeefu) return 1;
    if (source_cast_sequence_facts_v1(spells, 17, 3, 0x400000, 3, value) ||
        value.animation_override != 0xdeadbeefu) return 2;
    if (!source_cast_sequence_facts_v1(spells, 17, 1, 0, 4, value) ||
        value.source_sequence != -1 || value.stance != 0 ||
        value.animation_override != 0xffffffffu) return 3;
    if (!source_cast_sequence_facts_v1(spells, 9, 0, 0x400000, 4, value) ||
        value.source_sequence != 31 || value.stance != 4 ||
        value.animation_override != 35u) return 4;
    if (!source_cast_sequence_facts_v1(spells, 9, 2, 0x400000, 1, value) ||
        value.source_sequence != std::numeric_limits<std::int32_t>::max() ||
        value.animation_override != 0x80000000u) return 5;
    std::cout << "PASS source Faery cast sequence facts: invalid slot no-op, authored Spells indexing, conditional stance, stored -1 sequence, and 32-bit source addition\n";
}
