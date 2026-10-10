#pragma once

#include "runtime_effects_factory_v1.hpp"
#include "../faery_menu/celest_source_use_v1.hpp"

namespace dh::foundation::effects {

struct CelestTargetFxIdsV1 {
    std::int32_t player_pre{-1};
    std::int32_t target_main{-1};
};

// Resolves the exact two names called by the original Celest Lua script in
// the supplied original EffectsTables snapshot. Rows without a playable step
// are rejected; no numeric IDs are treated as stable constants.
bool resolve_celest_target_fx_ids_v1(
    const dh2::data::EffectsTables::Borrow&, CelestTargetFxIdsV1&,
    std::string& error);

// Implements Celest's optional presentation sink using the existing
// same-CombatSession RuntimeEffectsFactory. The factory and table borrow must
// be constructed from the same retained source EffectsTables snapshot.
// `play_set` creates the authored source instance only; packet preparation,
// texture upload and GPU submission remain the normal separate render path.
class CelestTargetFxDispatchV1 final
    : public faery_menu::CelestTargetFxDispatchV1 {
public:
    CelestTargetFxDispatchV1(
        RuntimeEffectsFactoryV1& factory,
        dh2::data::EffectsTables::Borrow tables,
        CelestTargetFxIdsV1 ids)
        : factory_(factory), tables_(std::move(tables)), ids_(ids) {}

    bool dispatch_player_pre(
        CombatSession&, const faery_menu::CelestPreparedCastV1&,
        const CharacterState&, const faery_menu::HottyCooldownClockV1&,
        faery_menu::CelestEffectStatusV1&, std::string&) override;
    bool dispatch_target_main(
        CombatSession&, const faery_menu::CelestPreparedCastV1&,
        const CharacterState&, const faery_menu::HottyCooldownClockV1&,
        ActorId target, faery_menu::CelestEffectStatusV1&,
        std::string&) override;

private:
    bool dispatch_source_set(
        CombatSession&, const faery_menu::CelestPreparedCastV1&,
        const CharacterState&, const faery_menu::HottyCooldownClockV1&,
        ActorId anchor, const char* expected_name, std::int32_t set_id,
        faery_menu::CelestEffectStatusV1&, std::string&);

    RuntimeEffectsFactoryV1& factory_;
    dh2::data::EffectsTables::Borrow tables_;
    CelestTargetFxIdsV1 ids_;
};

} // namespace dh::foundation::effects
