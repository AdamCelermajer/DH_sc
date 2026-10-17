#pragma once

#include "character_distribute_give_xp_dispatch_v1.hpp"
#include "../game-data/quest_reward_execution_v1.hpp"

namespace dh2::quest_reward_give_xp_bridge_v1 {

// Borrow the caller's current Character projection and the already selected
// Character::_GiveXP backend. This adapter owns no Character, Save, property,
// inventory, receipt, LevelUp, or PlayerManager state.
struct Context {
    character_distribute_give_xp_dispatch_v1::Owner* xp_owner = nullptr;
    character_distribute_xp_v1::CharacterView* character = nullptr;
    std::uintptr_t expected_character = 0;
    std::uint32_t calls = 0;
    std::uint32_t last_source_return = 0;
    std::string error;
};

// Signature-compatible with quest_reward_execution_v1::Services::give_xp.
// Reward_XP::Give passes source_flag=1; its returned bool controls whether the
// canonical Reward XP receipt word is stored, not whether Give itself succeeds.
std::int32_t give_xp(
    void* context,
    data::quest_reward_execution_v1::Record& reward,
    data::quest_reward_execution_v1::CharacterRef& character,
    std::int32_t amount_fixed,
    std::uint8_t source_flag,
    bool* granted);

} // namespace dh2::quest_reward_give_xp_bridge_v1
