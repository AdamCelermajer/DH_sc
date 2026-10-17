#pragma once

#include "player_actor_frame_v1.hpp"

namespace dh2::level_frame_v1 {

struct State {
    actor::application_clock_v1::State application{};
    actor::player_actor_frame_v1::Clock scene{};
    bool initialized = false;
};
using Result = actor::player_actor_frame_v1::ApplicationFrameResult;
enum class Status : std::uint8_t { advanced, skipped_large_gap, invalid_argument, provider_failed };

// Selected-source frame projection. Level::Update is decomposed around its
// PhysicalWorld call so the native backend can preserve authored ordering:
// scene animation -> Level event/script work -> one physics Step -> Characters.
struct Providers {
    void* context = nullptr;
    int (*animate_scene)(void*, std::uint32_t absolute_timestamp_ms) = nullptr;
    int (*update_level)(void*, std::uint32_t dt_ms) = nullptr;
    int (*step_physics)(void*, std::uint32_t dt_ms, float dt_seconds,
                        std::uint32_t iterations) = nullptr;
    int (*update_actors)(void*, std::uint32_t dt_ms) = nullptr;
};

inline void initialize(State* state, std::uint32_t now_ms,
                       float dt_scale = 1.0f,
                       float scaled_dt_scale = 1.0f) noexcept {
    if (!state) return;
    *state = {};
    state->application.last_real_time_ms = now_ms;
    state->application.dt_scale = dt_scale;
    state->application.scaled_dt_scale = scaled_dt_scale;
    state->initialized = true;
}

inline Status advance(State* state, std::uint32_t now_ms,
                      const Providers* providers, Result* out) noexcept {
    if (!state || !state->initialized || !out || !providers ||
        !providers->animate_scene || !providers->update_level ||
        !providers->step_physics || !providers->update_actors)
        return Status::invalid_argument;
    auto app_state = state->application;
    actor::application_clock_v1::Result application{};
    if (actor::application_clock_v1::compute(&app_state, now_ms, &application) !=
        actor::application_clock_v1::Status::complete)
        return Status::invalid_argument;
    // Application::Update always commits ComputeDt before the device-gap gate.
    state->application = app_state;
    out->application = application;
    out->frame = {};
    if (application.raw_elapsed_ms > 2000)
        return Status::skipped_large_gap;

    const actor::player_actor_frame_v1::Providers ordered{
        providers->context, providers->animate_scene, providers->update_level,
        providers->step_physics, providers->update_actors};
    const auto status = actor::player_actor_frame_v1::advance_with_application_result(
        &state->scene, &application, &ordered, out);
    return status == actor::player_actor_frame_v1::Status::complete
        ? Status::advanced
        : status == actor::player_actor_frame_v1::Status::provider_failed
            ? Status::provider_failed : Status::invalid_argument;
}

} // namespace dh2::level_frame_v1
