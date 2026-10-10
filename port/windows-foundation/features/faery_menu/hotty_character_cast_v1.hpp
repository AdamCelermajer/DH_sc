#pragma once

#include "hotty_cast_v1.hpp"
#include "../../combat_session.hpp"

namespace dh::foundation::faery_menu {

enum class HottyCharacterApplyStatusV1 : std::uint8_t {
    rejected,
    no_character_targets,
    source_ordered_character_hits_applied,
    source_ordered_character_hits_partially_applied
};

struct HottyCharacterHitV1 {
    ActorId target{invalid_actor_id};
    DamageEvent receipt;
    // SpellCombatRoll returns AttackResult.amount after F_ApplyResult. This
    // exact sign predicate is retained for the authored target-FX branch.
    bool spell_return_positive{};
};

struct HottyCharacterApplyV1 {
    HottyCharacterApplyStatusV1 status{HottyCharacterApplyStatusV1::rejected};
    std::uint64_t generation{};
    std::vector<HottyCharacterHitV1> hits;
};

// Applies only a complete, retained-order Character-only TargetListSearch
// result. Each target runs the original Hotty SpellCombatRoll request through
// CombatSession's shared source-result/application path using the prepared
// Spell_Fire_1+rank sheet and actual selected Faery SpellType. Any non-character
// scope (including future GameObject entries), unknown ordering, stale
// session/cooldown/list, or invalid source fact rejects before the first hit.
// A later source application failure preserves already-reached hits and returns
// their receipts; it never rewinds RNG/health. The host order is current
// ActorPopulation enrollment order, not a claim of native pointer-order parity.
bool apply_hotty_character_targets_v1(
    CombatSession&, const CharacterState&, const HottyCooldownClockV1&,
    const HottyPreparedCastV1&, const HottySourceTargetListV1&,
    const dh2::data::FaeryTables::Borrow&, const dh2::data::ClassTables&,
    HottyCharacterApplyV1&, std::string& error,
    std::uint32_t event_index_base = 0);

} // namespace dh::foundation::faery_menu
