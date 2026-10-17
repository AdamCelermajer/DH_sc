#pragma once

#include "character_ai_initialization.hpp"
#include "character_ai_set_target.hpp"
#include <cstdint>

namespace dh2::character_clear_target_v1 {

enum class Status : std::uint32_t { complete, invalid_argument, source_failed };

// Character::_ClearTarget is a void Lua native which forwards exactly
// AI_SetTarget(nullptr, false). This adapter projects the existing CharAI
// fields for that call and publishes all reached writes back to the same owner.
Status clear(character_ai_initialization::State*,
             character::set_target::OwnerFacts*,
             std::uintptr_t expected_character,
             const character::set_target::Services*);

} // namespace dh2::character_clear_target_v1
