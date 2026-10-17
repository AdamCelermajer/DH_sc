#pragma once

#include <cmath>
#include <cstdint>
#include <limits>

#include "application_clock_v1.hpp"

namespace dh2::actor::player_actor_frame_v1 {

// Source frame order recovered from Application::_Update, CSceneManager::update,
// PhysicalWorld::update, Character::Update and GameObject::Update:
// scene/root animation -> one physics Step -> actor update. Scene time is an
// accumulated float millisecond clock; physics receives dt seconds separately.
struct Clock {
    float scene_milliseconds = 0.0f;
};

struct Providers {
    void* context = nullptr;
    // CSceneManager::update calls root onAnimate with the accumulated absolute
    // timestamp, not a per-frame delta.
    int (*animate_scene)(void*, std::uint32_t timestamp_ms) = nullptr;
    // GSLevel/Level work such as event/script updates runs after scene
    // animation and before PhysicalWorld::update.
    int (*before_physics)(void*, std::uint32_t dt_ms) = nullptr;
    // PhysicalWorld::update calls one Box2D Step with seconds and 10 iterations.
    int (*step_world)(void*, std::uint32_t dt_ms, float dt_seconds,
                      std::uint32_t iterations) = nullptr;
    // Character/GameObject frame updates receive the original millisecond dt.
    int (*update_actor)(void*, std::uint32_t dt_ms) = nullptr;
};

enum class Status : std::uint8_t { complete, invalid_argument, provider_failed };
enum class FailedProvider : std::uint8_t { none, scene, before_physics, physics, actor };

struct Result {
    std::uint32_t scene_timestamp_ms = 0;
    float physics_dt_seconds = 0.0f;
    std::uint32_t physics_iterations = 0;
    FailedProvider failed_provider = FailedProvider::none;
};

inline Status advance(Clock* clock, std::uint32_t dt_ms,
                     const Providers* providers, Result* out) noexcept {
    if (!clock || !providers || !out || !providers->animate_scene ||
        !providers->step_world || !providers->update_actor ||
        !std::isfinite(clock->scene_milliseconds) || clock->scene_milliseconds < 0.0f)
        return Status::invalid_argument;

    // CSceneManager stores +0x254 as float milliseconds, then converts it to
    // uint32 for onAnimate. Keep the add and the unit conversion separate.
    volatile float frame_delta = static_cast<float>(dt_ms);
    volatile float next_scene_ms = clock->scene_milliseconds + frame_delta;
    constexpr double kUint32Limit = 4294967296.0;
    if (!std::isfinite(next_scene_ms) || static_cast<double>(next_scene_ms) >= kUint32Limit)
        return Status::invalid_argument;
    const auto timestamp = static_cast<std::uint32_t>(next_scene_ms);

    // PhysicalWorld::update uses unsigned dt, converts it to float, then
    // multiplies by the source literal 0.001f and issues one Step(...,10).
    volatile float source_dt = static_cast<float>(dt_ms);
    volatile float physics_dt = source_dt * 0.001f;

    clock->scene_milliseconds = next_scene_ms;
    Result result{timestamp, physics_dt, 10, FailedProvider::none};
    if (providers->animate_scene(providers->context, timestamp) != 0) {
        result.failed_provider = FailedProvider::scene;
        *out = result;
        return Status::provider_failed;
    }
    if (providers->before_physics &&
        providers->before_physics(providers->context, dt_ms) != 0) {
        result.failed_provider = FailedProvider::before_physics;
        *out = result;
        return Status::provider_failed;
    }
    if (providers->step_world(providers->context, dt_ms, physics_dt, 10) != 0) {
        result.failed_provider = FailedProvider::physics;
        *out = result;
        return Status::provider_failed;
    }
    if (providers->update_actor(providers->context, dt_ms) != 0) {
        result.failed_provider = FailedProvider::actor;
        *out = result;
        return Status::provider_failed;
    }
    *out = result;
    return Status::complete;
}

struct ApplicationFrameResult {
    application_clock_v1::Result application{};
    Result frame{};
};

// Persistent state shared by one production frame owner: the app-owned
// real-time/carry clock and the CSceneManager absolute-float clock.
struct ApplicationFrameState {
    application_clock_v1::State application{};
    Clock scene{};
};

inline Status advance_with_application_result(
        Clock* scene_clock, const application_clock_v1::Result* application,
        const Providers* providers, ApplicationFrameResult* out) noexcept {
    if (!scene_clock || !application || !providers || !out)
        return Status::invalid_argument;
    Result frame{};
    const Status status = advance(scene_clock, application->dt_ms, providers, &frame);
    out->application = *application;
    out->frame = frame;
    return status;
}

// Direct convenience API; callers with Application's >2s gate should use the
// level-frame owner so ComputeDt commits while simulation providers are skipped.
inline Status advance_from_application_clock(
        application_clock_v1::State* application_clock,
        Clock* scene_clock, std::uint32_t now_ms,
        const Providers* providers, ApplicationFrameResult* out) noexcept {
    if (!application_clock || !scene_clock || !providers || !out)
        return Status::invalid_argument;
    application_clock_v1::State next = *application_clock;
    application_clock_v1::Result application{};
    if (application_clock_v1::compute(&next, now_ms, &application) !=
        application_clock_v1::Status::complete)
        return Status::invalid_argument;
    *application_clock = next;
    return advance_with_application_result(scene_clock, &application, providers, out);
}

inline Status advance(ApplicationFrameState* state, std::uint32_t now_ms,
                      const Providers* providers,
                      ApplicationFrameResult* out) noexcept {
    return state ? advance_from_application_clock(
                       &state->application, &state->scene, now_ms, providers, out)
                 : Status::invalid_argument;
}

} // namespace dh2::actor::player_actor_frame_v1
