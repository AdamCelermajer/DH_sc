#pragma once

#include <cstdint>

namespace dh2::native_monster_attack_request_v1 {

struct Input {
    std::uintptr_t actor;
    std::uintptr_t character_owner;
    std::uintptr_t ai_character;
    std::uintptr_t requested_target;
    std::uintptr_t current_target;
    std::uintptr_t player;
    std::int32_t combat_target;
    std::uint32_t player_dead;
};

struct AttackLifecycle {
    std::uint32_t gated_spawn;
    std::uint32_t source_updates_enabled;
    std::uint32_t source_visible;
    std::int32_t character_state;
    std::uint32_t vm_ready;
    std::uintptr_t actor;
    std::uintptr_t character_owner;
    std::uintptr_t char_ai_character;
    std::uintptr_t lifecycle_owner;
    std::uintptr_t active_ais;
    std::uintptr_t lifecycle_active_ais;
    std::uintptr_t ais_projection_identity;
};

// Static placed actors retain the legacy path. A gated source Character can
// apply animation-event combat only after its existing Character and AIS VM
// have passed publication and lifecycle identity checks.
constexpr bool allows_combat_event(const AttackLifecycle& state) noexcept {
    if (!state.gated_spawn) return true;
    return state.source_updates_enabled != 0 && state.source_visible != 0 &&
        state.character_state != 0 && state.vm_ready != 0 && state.actor != 0 &&
        state.actor == state.character_owner && state.actor == state.lifecycle_owner &&
        state.actor == state.char_ai_character &&
        state.active_ais != 0 && state.active_ais == state.lifecycle_active_ais &&
        state.active_ais == state.ais_projection_identity;
}

// Character::Attack records an attack request only for the current live
// player target on the same retained Character/CharAI owner. Damage remains
// authored-animation-event driven and is applied by the canonical combat path.
constexpr bool accepts(const Input& input) noexcept {
    return input.actor != 0 && input.actor == input.character_owner &&
        input.actor == input.ai_character && input.requested_target != 0 &&
        input.requested_target == input.current_target &&
        input.requested_target == input.player && input.combat_target == -2 &&
        input.player_dead == 0;
}

} // namespace dh2::native_monster_attack_request_v1
