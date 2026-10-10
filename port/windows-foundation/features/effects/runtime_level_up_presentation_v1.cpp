#include "runtime_level_up_presentation_v1.hpp"

#include <array>

namespace dh::foundation::effects {

bool play_level_up_presentation_v1(const dh2::data::EffectsTables::Borrow& tables,
    const RuntimeLevelUpPlaySetV1& play, ActorId player,
    RuntimeLevelUpPresentationResultV1& result, std::string& error) {
    result = {};
    error.clear();
    if (!tables) { error = "Level-up FX requires the original EffectsTables borrow"; return false; }
    if (!play) { error = "Level-up FX requires the same-session CharacterMeshFxOwnerV4 play"; return false; }
    if (player == invalid_actor_id) { error = "Level-up FX requires the leveling player ActorId"; return false; }
    const auto& names = tables.set_names();
    if (names.size() <= std::size_t(kLevelUpFxSetV1) ||
        names[std::size_t(kLevelUpFxSetV1)] != kLevelUpFxSetNameV1) {
        error = "Original EffectsTables set 135 is not the level_up set";
        return false;
    }
    result.text = std::string("text=") + kLevelUpTextV1 + " drawn=0 (no HUD status-message owner)";
    const std::array<float, 3> zero{0.f, 0.f, 0.f};
    std::string play_error;
    const auto anchor = static_cast<std::uintptr_t>(player);
    if (play(kLevelUpFxSetV1, zero.data(), nullptr, anchor, nullptr, play_error)) {
        result.fx_played = true;
        result.fx = "fx=135:level_up:played";
    } else {
        result.fx = "fx=135:level_up:failed(" + play_error + ")";
    }
    return true;
}

} // namespace dh::foundation::effects
