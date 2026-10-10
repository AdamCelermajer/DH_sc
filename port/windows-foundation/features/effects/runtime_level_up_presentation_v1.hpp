#pragma once

#include "../../actor_state.hpp"
#include "../../../game-data/effects_tables.hpp"

#include <cstdint>
#include <functional>
#include <string>

namespace dh::foundation::effects {

// Original Character::LevelUp (IDA 0x3beb88, pseudocode-all.c ~line 138700):
//   VisualFXManager::PlayAnimFXSet(&Singleton<VisualFXManager>, 135, this, 0)
// The EffectsTables set 135 is named "level_up" (one step, file dictionary
// index 63 -> data/3D/interface/level_up.bdae, play_time -1, loop 0, speed 1.0).
// Position is the source Point3D::ZERO, rotation is NULL, anchor is the
// Character itself. The PlayAnimFXSet result is not checked by the source.
inline constexpr std::int32_t kLevelUpFxSetV1 = 135;
inline constexpr const char* kLevelUpFxSetNameV1 = "level_up";
// StrID MENU_LEVEL_UP as read from original-cache data/text/global.english.
// The text is queued by Character::LevelUp into the StatusMsg HUD queue; the
// Windows runtime has no status-message owner yet, so it is logged, not drawn.
inline constexpr const char* kLevelUpTextV1 = "LEVEL UP!";

using RuntimeLevelUpPlaySetV1 = std::function<bool(
    std::int32_t set, const float position[3], const float* rotation,
    std::uintptr_t anchor, std::uintptr_t* created_identity, std::string& error)>;

struct RuntimeLevelUpPresentationResultV1 {
    // "fx=<set>:<name>:played" or "fx=<set>:<name>:failed(<reason>)".
    std::string fx;
    // "text=<MENU_LEVEL_UP> drawn=0 (no HUD status-message owner)".
    std::string text;
    bool fx_played = false;
};

// Plays the level-up FX on the leveling Character. Returns false only for an
// invalid request (set 135 absent or not named level_up, no player). A failed
// FX play is reported in result.fx and does not fail the XP award, matching the
// source, which ignores PlayAnimFXSet's return value.
bool play_level_up_presentation_v1(const dh2::data::EffectsTables::Borrow& tables,
    const RuntimeLevelUpPlaySetV1& play, ActorId player,
    RuntimeLevelUpPresentationResultV1& result, std::string& error);

} // namespace dh::foundation::effects
