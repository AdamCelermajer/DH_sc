#pragma once

#include <cstdint>

namespace dh2::character_physics_position {

// Borrowed projection for the Character vtable implementation at slot +0x64.
// `flags_520` must point to the live Character policy word corresponding to
// source offset +0x520; it is read when query() runs, not cached here.
struct CharacterView {
    std::uintptr_t identity;
    const std::uint32_t* flags_520;
};

struct Result {
    std::uintptr_t character_identity;
    std::uint32_t raw;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    policy_failure = 2,
};

// Character::IsUpdatingPositionFromPhysics at original ELF 0x3a2e44 returns
// (Character+0x520 bit 1). This adapter delegates bit decoding to the existing
// move-policy producer, and preserves the full-width owner identity for a
// Stop callback. It implements only the Character override; base GameObject
// returns 1 and requires a distinct dynamic-class binding.
Status query(const CharacterView*, Result*);

static_assert(sizeof(void*) == 8);
static_assert(sizeof(CharacterView) == 16);
static_assert(sizeof(Result) == 16);

}  // namespace dh2::character_physics_position

extern "C" int dh2_character_is_updating_position_from_physics(
    const dh2::character_physics_position::CharacterView*,
    dh2::character_physics_position::Result*);
