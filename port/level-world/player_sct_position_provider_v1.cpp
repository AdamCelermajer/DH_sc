#include "player_sct_position_provider_v1.hpp"

#include <cmath>

namespace dh2::player_sct_position_provider_v1 {

bool lookup(void* raw, std::uintptr_t requested_identity,
            ui::ScrollingCombatTextPositionFactsV1& facts,
            std::string& error) noexcept {
    error.clear();
    const auto* owner = static_cast<const Owner*>(raw);
    if (!owner || !requested_identity ||
        owner->character_identity != requested_identity ||
        owner->save_character_identity != requested_identity ||
        !owner->runtime || !owner->source_visible_80 ||
        *owner->source_visible_80 > 1) {
        error = "Player SCT lookup lacks a live identity-matched Character/Save/runtime/visibility owner";
        return false;
    }
    for (float value : owner->runtime->position) {
        if (!std::isfinite(value)) {
            error = "Player SCT GameObject position is non-finite";
            return false;
        }
    }
    for (float value : owner->runtime->local_bounds) {
        if (!std::isfinite(value)) {
            error = "Player SCT Character bounds are non-finite";
            return false;
        }
    }

    ui::ScrollingCombatTextPositionFactsV1 result{};
    result.identity = requested_identity;
    result.game_object_position = owner->runtime->position;
    result.visual_scene = owner->visual_scene;
    result.relative_box = owner->runtime->local_bounds;
    result.source_visible_80 = *owner->source_visible_80;
    facts = result;
    return true;
}

} // namespace dh2::player_sct_position_provider_v1
