#include "../app/src/main/cpp/native_player_input_controller_v1.hpp"

#include <cmath>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace dh2::native::player_input_controller_v1;
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
struct Trace {
    std::vector<std::uint32_t> events;
    std::uint32_t stops = 0;
    dh2::navigation::HeadingState* heading = nullptr;
    static int event(void* raw, std::uint32_t id) {
        static_cast<Trace*>(raw)->events.push_back(id);
        return 0;
    }
    static int stop(void* raw) {
        auto& trace = *static_cast<Trace*>(raw);
        ++trace.stops;
        if (trace.heading) {
            trace.heading->active = 0;
            trace.heading->direction[0] = trace.heading->direction[1] =
                trace.heading->direction[2] = 0.0f;
        }
        return 0;
    }
};
DispatchResult tick(const Input& input, dh2::navigation::HeadingState& heading,
                    float& angle, bool controller_enabled, bool using_skill,
                    bool casting, const dh2::native::character_controller_v1::Services& services) {
    Projection projected{};
    check(project(input, &projected) == Status::complete, "input projection failed");
    DispatchResult result{};
    check(dispatch(projected, &heading, &angle, controller_enabled, using_skill,
                   casting, &services, &result) == Status::complete,
          "Character controller dispatch failed");
    return result;
}
}

int main() {
    try {
        using namespace dh2;
        navigation::HeadingState heading{};
        float angle = 0.0f;
        Trace trace;
        trace.heading = &heading;
        const native::character_controller_v1::Services services{
            &trace, Trace::event, Trace::stop};
        DispatchResult result{};

        check(source_controller_allowed(false, false, false) &&
              !source_controller_allowed(false, true, false) &&
              !source_controller_allowed(false, false, true) &&
              source_controller_allowed(true, true, true),
              "v2Controller global-block/forced gate differs from source");

        // The Android MovementControl transmits screen axes. Its owner applies
        // HUDControls' +45-degree basis once before the canonical controller.
        float android_axes[3]{0.5f, 0.0f, 0.0f};
        result = tick(Input{android_axes, Source::android_movement_control},
                      heading, angle, true, false, false, services);
        constexpr float diagonal = 0.3535533905932738f;
        check(result.input.active && std::fabs(result.input.direction[0] - diagonal) < 1e-6f &&
              std::fabs(result.input.direction[1] - diagonal) < 1e-6f &&
              result.outcome == native::character_controller_v1::Outcome::heading &&
              trace.events == std::vector<std::uint32_t>{0},
              "Android touch did not enter source HeadTowards once");

        // The original SWF joystick already emits a rotated vector: preserve
        // its exact magnitude and do not apply the Android basis again.
        float authored[3]{0.6f, 0.8f, 0.0f};
        result = tick(Input{authored, Source::authored_hud_projected, true},
                      heading, angle, true, false, false, services);
        check(result.input.active && result.input.direction[0] == authored[0] &&
              result.input.direction[1] == authored[1] && trace.events.size() == 2,
              "authored joystick vector was remapped or not dispatched");

        // Authored SWF pointer-up queues a stop and clears its active bit. The
        // next renderer tick must project a zero vector, stop the live heading,
        // then forward Character event 63 without skill/cast queries.
        float release_vector[3]{0.6f, 0.8f, 0.0f};
        Projection released{};
        check(project(Input{release_vector, Source::authored_hud_projected, false},
                      &released) == Status::complete && !released.active &&
              released.direction[0] == 0.0f && !requires_skill_cast_gates(released, true),
              "pointer release did not become an inactive zero vector");
        DispatchResult release_result{};
        check(dispatch(released, &heading, &angle, true, false, false,
                       &services, &release_result) == Status::complete &&
              release_result.outcome == native::character_controller_v1::Outcome::stopped &&
              trace.stops == 1 && trace.events.back() == 63 && heading.active == 0,
              "pointer release did not Stop before event 63");

        // The SWF pointer-up calls Cmd_Stop directly, even if HeadTowards(0)
        // has already cleared heading.active. Do not collapse this edge into
        // the conditional zero-vector path above.
        Input explicit_stop_input{};
        explicit_stop_input.direction = release_vector;
        explicit_stop_input.source = Source::authored_hud_projected;
        explicit_stop_input.stop_requested = true;
        Projection explicit_release{};
        check(project(explicit_stop_input, &explicit_release) == Status::complete &&
              explicit_release.stop_requested && !explicit_release.active &&
              explicit_release.direction[0] == 0.0f &&
              explicit_release.direction[1] == 0.0f,
              "authored pointer-up was not preserved as a direct stop command");
        const auto events_before_explicit_release = trace.events.size();
        DispatchResult explicit_release_result{};
        check(dispatch(explicit_release, &heading, &angle, true, false, false,
                       &services, &explicit_release_result) == Status::complete &&
              explicit_release_result.outcome == native::character_controller_v1::Outcome::stopped &&
              trace.stops == 2 && trace.events.size() == events_before_explicit_release + 1 &&
              trace.events.back() == 63,
              "explicit Cmd_Stop was incorrectly suppressed by an inactive heading");
        const auto events_before_locked_release = trace.events.size();
        DispatchResult locked_release_result{};
        check(dispatch(explicit_release, &heading, &angle, false, false, false,
                       &services, &locked_release_result) == Status::complete &&
              locked_release_result.outcome == native::character_controller_v1::Outcome::unchanged &&
              trace.stops == 2 && trace.events.size() == events_before_locked_release,
              "controller lock did not gate the direct stop command");

        // Android pointer-up is HUD event 6/7 and therefore also calls the
        // direct Cmd_Stop path, even when HeadTowards already cleared heading.
        Input android_stop_input{};
        android_stop_input.direction = release_vector;
        android_stop_input.source = Source::android_movement_control;
        android_stop_input.stop_requested = true;
        Projection android_release{};
        check(project(android_stop_input,&android_release)==Status::complete&&
              android_release.stop_requested&&!android_release.active&&
              android_release.direction[0]==0.0f&&android_release.direction[1]==0.0f,
              "Android HUD pointer-up was not preserved as a direct stop");
        heading.active=0;
        const auto events_before_android_stop=trace.events.size();
        DispatchResult android_release_result{};
        check(dispatch(android_release,&heading,&angle,true,false,false,
                       &services,&android_release_result)==Status::complete&&
              android_release_result.outcome==native::character_controller_v1::Outcome::stopped&&
              trace.stops==3&&trace.events.size()==events_before_android_stop+1&&
              trace.events.back()==63,
              "Android pointer-up incorrectly fell through conditional HeadTowards(0)");
        Input invalid_stop{};
        invalid_stop.direction=release_vector;
        invalid_stop.source=Source::v2_gamepad;
        invalid_stop.stop_requested=true;
        Projection untouched_stop{};
        untouched_stop.direction[0]=8.0f;
        check(project(invalid_stop,&untouched_stop)==Status::invalid_argument&&
              untouched_stop.direction[0]==8.0f,
              "gamepad input accepted an Android/HUD pointer-up stop token");

        // A post-deadzone but sub-1e-4 controller vector uses the same source
        // threshold as dispatch and therefore bypasses skill/cast queries.
        float tiny[3]{0.005f, 0.0f, 0.0f};
        Projection tiny_projection{};
        check(project(Input{tiny, Source::authored_hud_projected, true},
                      &tiny_projection) == Status::complete && tiny_projection.active &&
              !requires_skill_cast_gates(tiny_projection, true),
              "tiny active vector incorrectly entered the skill/cast query path");

        // Character::Ctrl_HeadTowards drops its CharAI target only when the
        // accepted heading forms an angle greater than 1.5708 radians.
        float forward[3]{1.0f, 0.0f, 0.0f};
        Projection target_heading{};
        check(project(Input{forward, Source::authored_hud_projected, true},
                      &target_heading) == Status::complete,
              "target-heading projection failed");
        const float target_ahead[3]{2.0f, 0.0f, 0.0f};
        const float target_behind[3]{-2.0f, 0.0f, 0.0f};
        const float target_side[3]{0.0f, 2.0f, 0.0f};
        const float target_zero[3]{};
        check(!should_clear_ai_target(target_heading, target_ahead, true, false, false) &&
              should_clear_ai_target(target_heading, target_behind, true, false, false) &&
              !should_clear_ai_target(target_heading, target_side, true, false, false) &&
              !should_clear_ai_target(target_heading, target_zero, true, false, false),
              "player target-clear angle differs from source threshold");
        check(!should_clear_ai_target(target_heading, target_behind, false, false, false) &&
              !should_clear_ai_target(target_heading, target_behind, true, true, false) &&
              !should_clear_ai_target(target_heading, target_behind, true, false, true),
              "player target clear bypassed controller or skill/cast gates");

        // A centered gamepad vector follows its separate deadzone route. The
        // active Character heading stops through GameObject::Stop then event63.
        float resume[3]{0.5f, 0.0f, 0.0f};
        tick(Input{resume, Source::android_movement_control},
             heading, angle, true, false, false, services);
        float gamepad[3]{0.1f, 0.0f, 0.0f};
        result = tick(Input{gamepad, Source::v2_gamepad, false, 0.0f, 0.0f, true},
                      heading, angle, true, false, false, services);
        check(!result.input.active && result.outcome == native::character_controller_v1::Outcome::stopped &&
              trace.stops == 4 && trace.events.back() == 63,
              "gamepad deadzone did not preserve source Stop/event order");

        // Skill/cast and controller locks remain gates in the canonical
        // Character controller; this projection owns neither gate nor path.
        const auto prior_heading = heading;
        float moving[3]{1.0f, 0.0f, 0.0f};
        Projection skill_input{};
        check(project(Input{moving, Source::android_movement_control}, &skill_input) ==
                  Status::complete && requires_skill_cast_gates(skill_input, true),
              "active nonzero input skipped the source skill/cast gates");
        result = tick(Input{moving, Source::android_movement_control},
                      heading, angle, true, true, false, services);
        check(result.outcome == native::character_controller_v1::Outcome::skill_blocked &&
              trace.events.back() == 63 && heading.active == prior_heading.active,
              "skill input bypassed the canonical controller gate");
        const auto prior_event_count = trace.events.size();
        result = tick(Input{moving, Source::android_movement_control},
                      heading, angle, true, false, true, services);
        check(result.outcome == native::character_controller_v1::Outcome::skill_blocked &&
              trace.events.size() == prior_event_count,
              "cast input bypassed the canonical controller gate");
        check(!requires_skill_cast_gates(skill_input, false),
              "locked controller queried skill/cast gates before its source early return");
        result = tick(Input{moving, Source::android_movement_control},
                      heading, angle, false, false, false, services);
        check(result.outcome == native::character_controller_v1::Outcome::unchanged &&
              trace.events.size() == prior_event_count,
              "locked controller accepted movement input");

        Projection untouched{};
        untouched.direction[0] = 9.0f;
        float invalid[3]{std::numeric_limits<float>::quiet_NaN(), 0.0f, 0.0f};
        check(project(Input{invalid, Source::android_movement_control}, &untouched) ==
                  Status::invalid_argument && untouched.direction[0] == 9.0f,
              "invalid input changed the published projection");
        std::puts("PASS: Android/SWF/gamepad input -> one Character controller; no PathTo dispatch");
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "player input/controller audit: %s\n", e.what());
        return 1;
    }
}
