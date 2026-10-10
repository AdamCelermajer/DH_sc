#pragma once

#include "hotty_cast_v1.hpp"

namespace dh::foundation::faery_menu {

enum class CelestPrepareStatusV1 : std::uint8_t {
    rejected,
    prepared_pending_spell_combat,
    prepared_no_targets_pending_spell_combat
};

struct CelestPreparedCastV1 {
    CelestPrepareStatusV1 status{CelestPrepareStatusV1::rejected};
    ActorId caster{invalid_actor_id};
    std::int32_t difficulty{-1};
    std::int32_t faery_slot{-1};
    std::int32_t faery_record_id{-1};
    std::int32_t source_spell_type{-1};
    std::int32_t skill_level{};
    std::int32_t fixed_mana_cost{};
    std::int32_t range{};
    std::vector<ActorId> character_targets;
    std::uint64_t session_update_serial{};
    std::uint64_t cooldown_ready_at_ms{};
    std::int32_t spell_class_id{-1};
    dh2::data::PropertySheet spell_properties{};
    bool mana_debited{};
    bool original_target_order_known{};
};

// Runs the authored Celest OnSkillUpdate/Check/Pre prefix on the same live
// player and CombatSession. The script does not consult saved Faery unlock
// state; callers must still resolve a real current-list row with this script.
bool prepare_celest_spell_v1(
    CombatSession&, CharacterState&, std::int32_t difficulty,
    const dh2::data::FaeryTables::Borrow&, const dh2::data::ClassTables&,
    const dh2::data::PropertyRules&, const HottySourcePolicyV1&,
    HottyCooldownClockV1&, CelestPreparedCastV1&, std::string& error,
    const HottySourceTargetListV1* exact_source_targets = nullptr);

} // namespace dh::foundation::faery_menu
