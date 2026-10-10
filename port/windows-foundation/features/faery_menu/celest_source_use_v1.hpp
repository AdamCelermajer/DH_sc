#pragma once

#include "celest_cast_v1.hpp"
#include "../../combat_session.hpp"

namespace dh::foundation::faery_menu {

enum class CelestEffectStatusV1 : std::uint8_t {
    not_requested,
    source_branch_skipped,
    source_instance_created,
    dispatch_failed
};

struct CelestEffectReceiptV1 {
    ActorId target{invalid_actor_id};
    CelestEffectStatusV1 status{CelestEffectStatusV1::not_requested};
    std::string diagnostic;
};

// A Lua-reached second SpellCombatRoll may target an actor killed by its first
// roll. Keep that full same-Session formula result separate from applied HP
// receipts: it consumes the source RNG occurrence but performs no second hit.
struct CelestDeadTargetCalculationV1 {
    ActorId target{invalid_actor_id};
    std::uint32_t event_index{};
    OriginalMeleeResolution result{};
};

struct CelestSourceUseV1 {
    std::vector<DamageEvent> rolls;
    std::vector<CelestDeadTargetCalculationV1> dead_target_calculations;
    std::vector<CelestEffectReceiptV1> target_effects;
    bool source_hits_applied{};
    bool source_use_complete{};
    bool target_effects_dispatched{};
    std::string partial_prefix_diagnostic;
};

// Optional presentation sink for the two actual Celest PlayFX call sites.
// Its failures are diagnostics only and never veto mana, cooldown, RNG or HP.
class CelestTargetFxDispatchV1 {
public:
    virtual ~CelestTargetFxDispatchV1() = default;
    virtual bool dispatch_player_pre(
        CombatSession&, const CelestPreparedCastV1&, const CharacterState&,
        const HottyCooldownClockV1&, CelestEffectStatusV1&, std::string&) = 0;
    virtual bool dispatch_target_main(
        CombatSession&, const CelestPreparedCastV1&, const CharacterState&,
        const HottyCooldownClockV1&, ActorId target,
        CelestEffectStatusV1&, std::string&) = 0;
};

// Reached after successful source preparation regardless of target count: Lua
// always calls Player_Pre in Celest OnPreSkill_. Missing/failed FX is retained
// by the caller as a non-veto diagnostic.
bool dispatch_celest_player_pre_best_effort_v1(
    CombatSession&, const CelestPreparedCastV1&, const CharacterState&,
    const HottyCooldownClockV1&, CelestTargetFxDispatchV1*,
    CelestEffectReceiptV1&, std::string& error);

// Called only for the retained state7 `do_spell` event. Character targets get
// two ordered SpellCombatRoll occurrences each, then unconditional Celest Main
// FX. Empty targets complete the Use loop without any roll.
bool apply_celest_source_use_v1(
    CombatSession&, const CelestPreparedCastV1&,
    const HottySourceTargetListV1&, const CharacterState&,
    const HottyCooldownClockV1&, const dh2::data::FaeryTables::Borrow&,
    const dh2::data::ClassTables&, CelestTargetFxDispatchV1*,
    CelestSourceUseV1&, std::string& error);

} // namespace dh::foundation::faery_menu
