#pragma once

#include "../../character_state.hpp"
#include "../../actor_state.hpp"
#include "../../../game-data/items.hpp"
#include "../../../level-world/character_skill_combat_v6.hpp"
#include "../platform_input/semantic_input.hpp"

#include <cstdint>
#include <string>

namespace dh::foundation { class CombatSession; }

namespace dh::foundation::inventory {

enum class RuntimePotionUseResultV1 {
    not_pressed,
    used,
    no_potion,
    vitals_full,
    player_inactive,
    consumed_prefix_failure
};

struct RuntimePotionUseServicesV1 {
    const dh2::data::ItemTable* item_table{};
    const dh2::data::PropertyRules* property_rules{};
    // This is the caller's real source DebugSwitches adapter. RegenHP/MP
    // reaches it only for a positive source delta; no default is substituted.
    const dh2::character::skills::SkillAttackNativeServicesV6* source_debug{};
};

struct RuntimePotionUseReceiptV1 {
    RuntimePotionUseResultV1 result{RuntimePotionUseResultV1::not_pressed};
    ActorId player{invalid_actor_id};
    std::string potion_instance;
    std::int32_t quantity_before{}, quantity_after{};
    std::int32_t hp_before{}, hp_after{}, mp_before{}, mp_after{};
    std::int32_t potion_count_property_before{}, potion_count_property_after{};
    bool consumed{};
    bool health_regenerated{}, mana_regenerated{};
};

// Dispatch one already captured semantic input frame against the existing
// CombatSession and canonical CharacterState. `pressed` is the source command
// edge; held frames do not repeat and no timer/cooldown is invented here.
// The source controller's blocked/locked admission remains the caller's gate.
class RuntimeSessionPotionUseV1 final {
public:
    bool dispatch(dh::foundation::CombatSession&,
                  ActorId expected_player,
                  CharacterState& same_character_state,
                  const dh::foundation::platform_input::ButtonEdges&,
                  const RuntimePotionUseServicesV1&,
                  RuntimePotionUseReceiptV1&,
                  std::string& error) const;
};

} // namespace dh::foundation::inventory
