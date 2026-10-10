#pragma once

#include "../../combat_session.hpp"
#include "../effects/runtime_effects_factory_v1.hpp"
#include "../../../game-data/effects_tables.hpp"
#include "hotty_cast_v1.hpp"
#include "hotty_source_use_v1.hpp"

namespace dh::foundation::faery_menu {

struct HottyEffectIdsV1 {
    std::int32_t player_pre{-1};
    std::int32_t character_target{-1};
};

enum class HottyEffectKindV1 : std::uint8_t {
    player_pre,
    positive_character_target
};

class HottyRuntimeEffectsDispatchV1 final : public HottyTargetFxDispatchV1 {
public:
    HottyRuntimeEffectsDispatchV1(
        effects::RuntimeEffectsFactoryV1& factory,
        const dh2::data::EffectsTables::Borrow& tables,
        const HottyEffectIdsV1& ids)
        : factory_(factory), tables_(tables), ids_(ids) {}

    bool dispatch_player_pre(
        CombatSession&, const HottyPreparedCastV1&, const CharacterState&,
        const HottyCooldownClockV1&, HottyEffectStatusV1&, std::string&) override;
    bool dispatch_positive_target(
        CombatSession&, const HottyPreparedCastV1&, const CharacterState&,
        const HottyCooldownClockV1&, ActorId target,
        HottyEffectStatusV1&, std::string&) override;

private:
    effects::RuntimeEffectsFactoryV1& factory_;
    dh2::data::EffectsTables::Borrow tables_;
    HottyEffectIdsV1 ids_;
};

// Resolve the exact original AnimatedEffectTable names. No ordinal constants
// or synthesized effect rows are accepted.
bool resolve_hotty_effect_ids_v1(dh2::data::EffectsTables::Borrow,
                                 HottyEffectIdsV1&, std::string& error);

// Directly dispatches source PlayFX through the same CombatSession Scene's
// retained RuntimeEffectsFactory. Player-pre FX is reached only after a
// complete source target-list query reports at least one item. Character-target
// FX is reached only after source F_ApplyResult returns positive damage.
// Caller must supply the same original FX table borrow used by the factory.
bool dispatch_hotty_effect_v1(
    CombatSession&, effects::RuntimeEffectsFactoryV1&,
    const dh2::data::EffectsTables::Borrow&, const HottyEffectIdsV1&,
    const HottyPreparedCastV1&, const CharacterState&,
    const HottyCooldownClockV1&,
    ActorId scene_actor, ActorId effect_actor, HottyEffectKindV1,
    bool source_target_count_known, std::uint32_t source_target_count,
    bool source_apply_result_known, bool source_return_positive,
    HottyEffectStatusV1&, std::string& error);

// Authored OnPre Player_Pre branch. Call after prepare_hotty_spell_v1 has
// committed the source UseMana/cooldown prefix; the FX is skipped for an empty
// target list exactly as Hotty's OnPreSkill_ does.
bool dispatch_hotty_player_pre_v1(
    CombatSession&, effects::RuntimeEffectsFactoryV1&,
    const dh2::data::EffectsTables::Borrow&, const HottyEffectIdsV1&,
    const HottyPreparedCastV1&, const CharacterState&,
    const HottyCooldownClockV1&, HottyEffectStatusV1&,
    std::string& error);

} // namespace dh::foundation::faery_menu
