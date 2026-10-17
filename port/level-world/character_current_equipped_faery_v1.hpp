#pragma once

#include "character_current_spell_v1.hpp"

#include <cstdint>

namespace dh2::character_current_equipped_faery_v1 {

enum class Status : std::int32_t {
    complete = 1,
    invalid_argument = -1,
    provider_failed = -2,
};

struct Result {
    std::uintptr_t character{};
    std::int32_t faery_id{};
    std::int32_t level{};
    std::uint32_t calls{};
    std::uint32_t complete{};
};

// Source wrapper for Character::_GetCurrentEquippedFaeryId: one saved
// selection read with selector -1. Arguments are not consulted.
Status current_equipped_faery_id(std::uintptr_t character,
                                 const character_current_spell_v1::Services*,
                                 Result*);

// Source wrapper for Character::_GetCurrentEquippedFaeryLevel: one selection
// read followed by one level read for that captured ID, both with selector -1.
Status current_equipped_faery_level(std::uintptr_t character,
                                    const character_current_spell_v1::Services*,
                                    Result*);

struct Bindings {
    std::uintptr_t character{};
    character_current_spell_v1::Services services{};
};

int current_equipped_faery_id_v1(void*, const dh2_script_value*, std::uint32_t,
                                 dh2_script_value*, std::uint32_t,
                                 std::uint32_t*, char*, std::size_t) noexcept;
int current_equipped_faery_level_v1(void*, const dh2_script_value*, std::uint32_t,
                                    dh2_script_value*, std::uint32_t,
                                    std::uint32_t*, char*, std::size_t) noexcept;

}  // namespace dh2::character_current_equipped_faery_v1
