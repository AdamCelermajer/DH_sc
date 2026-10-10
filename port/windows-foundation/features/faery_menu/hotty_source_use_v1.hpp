#pragma once

#include "hotty_character_cast_v1.hpp"

namespace dh::foundation::faery_menu {

enum class HottyEffectStatusV1 : std::uint8_t {
    not_requested,
    source_branch_skipped,
    source_instance_created,
    dispatch_failed
};

struct HottyTargetEffectReceiptV1 {
    ActorId target{invalid_actor_id};
    HottyEffectStatusV1 status{HottyEffectStatusV1::not_requested};
    std::string diagnostic;
};

struct HottySourceUseV1 {
    HottyCharacterApplyV1 combat;
    std::vector<HottyTargetEffectReceiptV1> target_effects;
    bool source_hits_applied{};
    // True if every requested effect branch was either source-skipped or
    // successfully dispatched. FX problems never invalidate source hits.
    bool target_effects_dispatched{};
};

// Narrow, typed bridge for the two authored Hotty FX call sites. Implementors
// own concrete same-session FX validation; the source-use kernel can therefore
// stay independent of RuntimeEffectsFactory and can retain combat progress if
// presentation resources are missing or stale.
class HottyTargetFxDispatchV1 {
public:
    virtual ~HottyTargetFxDispatchV1() = default;
    virtual bool dispatch_player_pre(
        CombatSession&, const HottyPreparedCastV1&, const CharacterState&,
        const HottyCooldownClockV1&, HottyEffectStatusV1&, std::string&) = 0;
    virtual bool dispatch_positive_target(
        CombatSession&, const HottyPreparedCastV1&, const CharacterState&,
        const HottyCooldownClockV1&, ActorId target,
        HottyEffectStatusV1&, std::string&) = 0;
};

// Applies the exact retained target loop using the current same-world source
// result path. One hit is applied per source-order entry, then its authored
// positive-return FX branch is attempted before the next target. Missing or
// failed FX dispatch is recorded in target_effects and never gates combat.
bool apply_hotty_source_use_v1(
    CombatSession&, const HottyPreparedCastV1&, const HottySourceTargetListV1&,
    const CharacterState&, const HottyCooldownClockV1&,
    const dh2::data::FaeryTables::Borrow&, const dh2::data::ClassTables&,
    HottyTargetFxDispatchV1* optional_fx, HottySourceUseV1&,
    std::string& error);

} // namespace dh::foundation::faery_menu
