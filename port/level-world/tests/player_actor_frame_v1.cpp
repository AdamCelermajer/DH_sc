#include "../player_actor_frame_v1.hpp"
#include "../level_frame_v1.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>

namespace frame = dh2::actor::player_actor_frame_v1;
namespace level_frame = dh2::level_frame_v1;

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
std::uint32_t bits(float value) {
    std::uint32_t word{};
    std::memcpy(&word, &value, sizeof(word));
    return word;
}
struct Trace {
    std::vector<unsigned> calls;
    std::uint32_t timestamp = 0;
    std::uint32_t actor_dt = 0;
    std::uint32_t physics_ms = 0;
    float physics_dt = 0.0f;
    unsigned iterations = 0;
    unsigned fail = 0;
    static int scene(void* context, std::uint32_t timestamp) {
        auto& self = *static_cast<Trace*>(context);
        self.calls.push_back(1);
        self.timestamp = timestamp;
        return self.fail == 1;
    }
    static int world(void* context, std::uint32_t dt_ms, float dt,
                     std::uint32_t iterations) {
        auto& self = *static_cast<Trace*>(context);
        self.calls.push_back(3);
        self.physics_ms = dt_ms;
        self.physics_dt = dt;
        self.iterations = iterations;
        return self.fail == 3;
    }
    static int before_physics(void* context, std::uint32_t dt) {
        auto& self = *static_cast<Trace*>(context);
        self.calls.push_back(2);
        self.actor_dt = dt;
        return self.fail == 2;
    }
    static int actor(void* context, std::uint32_t dt) {
        auto& self = *static_cast<Trace*>(context);
        self.calls.push_back(4);
        self.actor_dt = dt;
        return self.fail == 4;
    }
    frame::Providers providers() { return {this, scene, before_physics, world, actor}; }
};
}

int main() {
    try {
        // Source-derived cases: CSceneManager::update (0x58b9f0) accumulates
        // float ms and calls onAnimate at 0x58baa4; PhysicalWorld::update
        // (0x34bd08) does one dt*0.001f Step(...,10); Character::Update then
        // GameObject::Update run after Step in the Level frame trace.
        frame::Clock clock{};
        Trace trace;
        const auto providers = trace.providers();
        frame::Result result{};
        const std::vector<unsigned> source_order{1, 2, 3, 4};

        require(frame::advance(&clock, 16, &providers, &result) == frame::Status::complete,
                "16ms source frame rejected");
        require(result.scene_timestamp_ms == 16 && trace.timestamp == 16 &&
                bits(result.physics_dt_seconds) == bits(0.016f) &&
                bits(trace.physics_dt) == bits(0.016f) && trace.physics_ms == 16 &&
                result.physics_iterations == 10 &&
                trace.actor_dt == 16 && trace.calls == source_order,
                "scene -> one 10-iteration Step -> actor contract differs");

        trace.calls.clear();
        require(frame::advance(&clock, 17, &providers, &result) == frame::Status::complete &&
                result.scene_timestamp_ms == 33 && trace.timestamp == 33 &&
                trace.actor_dt == 17 && trace.calls == source_order,
                "scene clock was treated as a per-frame delta");

        // Same-millisecond scene timestamps remain legal. The physics wrapper
        // still makes exactly one Step, while actor and Step receive the raw dt.
        trace.calls.clear();
        require(frame::advance(&clock, 0, &providers, &result) == frame::Status::complete &&
                result.scene_timestamp_ms == 33 && bits(trace.physics_dt) == bits(0.0f) &&
                trace.iterations == 10 && trace.actor_dt == 0 && trace.calls == source_order,
                "zero-dt source frame was clamped or subdivided");

        // CSceneManager's clock is float: at 2^24 ms, adding one millisecond
        // rounds back to the same absolute timestamp. Do not replace it with
        // an integer accumulator.
        frame::Clock precision{16777216.0f};
        trace.calls.clear();
        require(frame::advance(&precision, 1, &providers, &result) == frame::Status::complete &&
                precision.scene_milliseconds == 16777216.0f &&
                result.scene_timestamp_ms == 16777216u,
                "scene clock lost source float precision behavior");

        // The source coordinator commits its accumulated clock before invoking
        // downstream providers; a failed physics provider stops before actor.
        frame::Clock failed_clock{};
        trace.calls.clear(); trace.fail = 3;
        require(frame::advance(&failed_clock, 20, &providers, &result) == frame::Status::provider_failed &&
                result.failed_provider == frame::FailedProvider::physics &&
                failed_clock.scene_milliseconds == 20.0f &&
                trace.calls == std::vector<unsigned>{1, 2, 3},
                "provider failure did not preserve completed phase order");

        // Application::ComputeDt is the sole dt producer; the same computed
        // millisecond delta feeds scene accumulation, one physics step, and
        // actor update. Secondary scale remains a separate result field.
        level_frame::State integrated_state{};
        level_frame::initialize(&integrated_state, 1000, 0.5f, 0.5f);
        level_frame::Providers level_providers{
            &trace, Trace::scene, Trace::before_physics, Trace::world, Trace::actor};
        level_frame::Result integrated{};
        trace.calls.clear(); trace.fail = 0;
        require(level_frame::advance(&integrated_state, 1033, &level_providers,
                    &integrated) == level_frame::Status::advanced &&
                integrated.application.dt_ms == 16 && integrated.application.scaled_dt_ms == 8 &&
                integrated.application.fractional_remainder == 0.0f &&
                integrated.frame.scene_timestamp_ms == 16 && trace.actor_dt == 16 &&
                trace.calls == source_order && integrated_state.application.last_real_time_ms == 1033,
                "Application clock did not feed the canonical scene/physics/actor frame");

        // Application::Update computes and commits dt, then returns before
        // scene/Level/physics on device gaps greater than 2000ms.
        trace.calls.clear();
        require(level_frame::advance(&integrated_state, 5034, &level_providers,
                    &integrated) == level_frame::Status::skipped_large_gap &&
                integrated.application.raw_elapsed_ms == 4001 &&
                integrated.application.fractional_remainder == 0.0f &&
                integrated_state.application.last_real_time_ms == 5034 &&
                integrated_state.scene.scene_milliseconds == 16.0f && trace.calls.empty(),
                "large application gap failed to commit clock and skip frame providers");
        trace.calls.clear();
        require(level_frame::advance(&integrated_state, 5050, &level_providers,
                    &integrated) == level_frame::Status::advanced &&
                integrated.application.dt_ms == 8 && integrated.frame.scene_timestamp_ms == 24 &&
                trace.calls == source_order,
                "post-gap frame did not resume from the committed application baseline");

        // EGL/Activity restoration retains the live scene/timeline and the
        // Application scales/carry, but rebases only its raw real-time origin.
        // Model the production owner mutation in load_world: it must not call
        // initialize(), which would reset the absolute scene clock to zero.
        integrated_state.application.last_real_time_ms = 9000;
        integrated_state.application.fractional_remainder = 0.375f;
        trace.calls.clear();
        require(level_frame::advance(&integrated_state, 9016, &level_providers,
                    &integrated) == level_frame::Status::advanced &&
                integrated.application.raw_elapsed_ms == 16 &&
                integrated.application.dt_ms == 8 &&
                integrated.application.scaled_dt_ms == 4 &&
                integrated.application.fractional_remainder == 0.375f &&
                integrated.frame.scene_timestamp_ms == 32 &&
                trace.timestamp == 32 && trace.calls == source_order &&
                integrated_state.application.dt_scale == 0.5f &&
                integrated_state.application.scaled_dt_scale == 0.5f,
                "restore rebased app time but lost retained scene/timeline clock or app carry");

        frame::Clock invalid{std::numeric_limits<float>::infinity()};
        trace.calls.clear(); trace.fail = 0;
        require(frame::advance(&invalid, 20, &providers, &result) == frame::Status::invalid_argument &&
                trace.calls.empty(), "malformed clock invoked a provider");
        std::puts("PASS: source actor frame clock/order; scene -> Level pre-step -> Step(10) -> actor");
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "player actor frame: %s\n", e.what());
        return 1;
    }
}
