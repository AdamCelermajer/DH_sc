#pragma once

#include <cstdint>

namespace dh2::character::animation_frame_v1 {

// Character::Update tail after its timer/CharAI work: state machine, then
// CharAnimator::Update, then GameObject::Update. Callers keep those owners
// authoritative and pass their live callbacks; this adapter owns only order.
struct Providers {
    void* context = nullptr;
    int (*update_state)(void*, std::uint32_t dt_ms) = nullptr;
    int (*update_animator)(void*, std::uint32_t dt_ms) = nullptr;
    int (*update_game_object)(void*, std::uint32_t dt_ms) = nullptr;
};

enum class Phase : std::uint8_t { none, state, animator, game_object, complete };
enum class Status : std::uint8_t { complete, invalid_argument, provider_failed };
struct Result {
    Phase phase = Phase::none;
    Phase failed_phase = Phase::none;
};

inline Status advance(std::uint32_t dt_ms, const Providers* providers,
                      Result* out) noexcept {
    if (!providers || !out || !providers->update_state ||
        !providers->update_animator || !providers->update_game_object)
        return Status::invalid_argument;

    Result result{};
    result.phase = Phase::state;
    if (providers->update_state(providers->context, dt_ms) != 0) {
        result.failed_phase = Phase::state;
        *out = result;
        return Status::provider_failed;
    }
    result.phase = Phase::animator;
    if (providers->update_animator(providers->context, dt_ms) != 0) {
        result.failed_phase = Phase::animator;
        *out = result;
        return Status::provider_failed;
    }
    result.phase = Phase::game_object;
    if (providers->update_game_object(providers->context, dt_ms) != 0) {
        result.failed_phase = Phase::game_object;
        *out = result;
        return Status::provider_failed;
    }
    result.phase = Phase::complete;
    *out = result;
    return Status::complete;
}

} // namespace dh2::character::animation_frame_v1
