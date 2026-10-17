#include "../app/src/main/cpp/native_character_controller_v1.hpp"
#include "../../level-world/character_state.hpp"

#include <cmath>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <vector>

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
struct Actor {
    dh2::character::State state;
    dh2::character::Facts facts;
    dh2::navigation::HeadingState heading{};
    float angle = 0;
    std::vector<std::int32_t> selected_sequences;
    std::vector<float> selected_speeds;
    std::uint32_t source_events = 0;
};
void state_service(void* raw, dh2::character::State* state,
                   const dh2::character::Request* request) {
    auto& actor = *static_cast<Actor*>(raw);
    using namespace dh2::character;
    switch (request->service) {
    case set_animation:
        state->current_animation = request->argument[0];
        actor.selected_sequences.push_back(request->argument[0]);
        break;
    case swap_animation:
        state->current_animation = request->argument[0];
        actor.selected_sequences.push_back(request->argument[0]);
        break;
    case set_speed:
        actor.selected_speeds.push_back(request->scalar);
        break;
    case raise_event:
        if (request->argument[0] == 0x1d) ++actor.source_events;
        break;
    default:
        break;
    }
}
int controller_event(void* raw, std::uint32_t event) {
    auto& actor = *static_cast<Actor*>(raw);
    actor.facts.heading[0] = actor.heading.direction[0];
    actor.facts.heading[1] = actor.heading.direction[1];
    actor.facts.heading[2] = actor.heading.direction[2];
    actor.facts.is_at_destination = 0;
    actor.facts.following_path = 0;
    const auto source_event = event == 0 ? 0xc351u : event == 63 ? 0x3fu : event;
    const dh2::character::Services services{&actor, state_service};
    return dh2_character_state_event(&actor.state, &actor.facts, source_event, 0,
                                     &services) < 0 ? -1 : 0;
}
int stop_game_object(void* raw) {
    auto& actor = *static_cast<Actor*>(raw);
    actor.heading.active = 0;
    actor.heading.direction[0] = actor.heading.direction[1] = actor.heading.direction[2] = 0;
    actor.facts.heading[0] = actor.facts.heading[1] = actor.facts.heading[2] = 0;
    actor.facts.is_at_destination = 1;
    return 0;
}
}

int main() {
    try {
        using namespace dh2;
        Actor actor;
        actor.state.current = 3;
        actor.state.flags = 0x2380;
        actor.facts.is_player = 1;
        actor.facts.stance_mask = 0xf0;
        actor.facts.stance = 5;
        actor.facts.walk_threshold = .45f;
        actor.facts.run_threshold = .85f;
        actor.facts.walk_speed = 1.25f;
        actor.facts.attack_speed = 1.0f;
        actor.facts.idle = 10;
        actor.facts.walk = 100;
        actor.facts.run = 200;
        actor.facts.attack_moving = 300;
        actor.facts.attack_static = 400;
        const native::character_controller_v1::Services controller_services{
            &actor, controller_event, stop_game_object};
        native::character_controller_v1::Outcome outcome{};

        // MainActivity supplies clamped screen axes. HUDControls applies its
        // fixed 45-degree screen basis before Character::Ctrl_HeadTowards.
        float stick[3]{.5f, 0, 0};
        bool active = false;
        check(native::camera_input_v1::map_movement_control_input(stick, &active) == 0 && active,
              "touch input mapping failed");
        check(native::character_controller_v1::dispatch_head_towards(
                  &actor.heading, &actor.angle, stick, active, true, false, false,
                  &controller_services, &outcome) == native::character_controller_v1::Status::complete &&
              outcome == native::character_controller_v1::Outcome::heading,
              "touch movement did not reach source HeadTowards");
        actor.state.heading_active = actor.heading.active;
        check(actor.state.current == 4 && actor.state.current_animation == 105 &&
              actor.state.move_type == 1 && actor.source_events == 1,
              "Idle -> Move did not select the canonical stance-adjusted Walk sequence");

        // A stronger same-direction input updates canonical heading. The
        // next Character::Move update must switch to the stance-adjusted Run.
        float faster[3]{.9f, 0, 0};
        check(native::camera_input_v1::map_movement_control_input(faster, &active) == 0 && active,
              "strong touch input mapping failed");
        check(native::character_controller_v1::dispatch_head_towards(
                  &actor.heading, &actor.angle, faster, active, true, false, false,
                  &controller_services, &outcome) == native::character_controller_v1::Status::complete,
              "run input did not update canonical heading");
        actor.state.heading_active = actor.heading.active;
        const character::Services services{&actor, state_service};
        const auto update_result = dh2_character_state_update(&actor.state, &actor.facts, 16, &services);
        check(update_result == 1 && actor.state.current == 4 && actor.state.move_type == 2 &&
              actor.state.current_animation == 205,
              (std::string("Move update Walk -> Run mismatch: result=") + std::to_string(update_result) +
               " type=" + std::to_string(actor.state.move_type) + " animation=" +
               std::to_string(actor.state.current_animation) + " heading=" +
               std::to_string(std::hypot(actor.facts.heading[0], actor.facts.heading[1])) +
               " player=" + std::to_string(actor.facts.is_player) + " run=" +
               std::to_string(actor.facts.run_threshold) + " xy=" +
               std::to_string(actor.facts.heading[0]) + "," +
               std::to_string(actor.facts.heading[1])).c_str());

        // Combat selects the moving attack because the previous canonical
        // Character state is Move; the selected sequence becomes the same
        // current_animation field consumed by the native playback owner.
        check(dh2_character_state_event(&actor.state, &actor.facts, 0xc354, 0x1234,
                                        &services) == 1 &&
              actor.state.current == 5 && actor.state.current_animation == 305,
              "Move -> Attack did not select the moving attack sequence");
        const bool was_heading = actor.state.heading_active != 0;
        float released[3]{};
        check(native::camera_input_v1::map_movement_control_input(released, &active) == 0 && !active,
              "released touch vector mapping failed");
        check(native::character_controller_v1::dispatch_head_towards(
                  &actor.heading, &actor.angle, released, active, true, false, false,
                  &controller_services, &outcome) == native::character_controller_v1::Status::complete &&
              outcome == native::character_controller_v1::Outcome::stopped,
              "released movement did not stop the active Character heading");
        actor.state.heading_active = actor.heading.active;
        if (actor.state.current == 5 && was_heading != (actor.state.heading_active != 0))
            check(dh2_character_state_event(&actor.state, &actor.facts, 0x1c, 0,
                                            &services) == 0,
                  "attack heading-change event was rejected");
        check(actor.state.current == 5 && actor.state.current_animation == 405,
              "Attack moving -> stationary variant did not follow joystick release");
        check(actor.selected_sequences.size() == 4 &&
              actor.selected_sequences[0] == 105 && actor.selected_sequences[1] == 205 &&
              actor.selected_sequences[2] == 305 && actor.selected_sequences.back() == 405 &&
              actor.selected_speeds.size() >= 3,
              "animation service sequence/speed handoff changed");
        std::printf("PASS: touch -> canonical Move/Walk -> Run -> moving/stationary Attack (%zu animation selections)\n",
                    actor.selected_sequences.size());
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "locomotion transition audit: %s\n", error.what());
        return 1;
    }
}
