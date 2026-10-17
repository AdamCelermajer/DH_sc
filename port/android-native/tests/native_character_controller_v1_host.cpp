#include "../app/src/main/cpp/native_character_controller_v1.hpp"

#include <array>
#include <cstdio>
#include <stdexcept>
#include <vector>

namespace {
unsigned checks = 0;
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
    ++checks;
}
struct Capture {
    dh2::navigation::HeadingState* heading;
    std::vector<unsigned> calls;
    bool stop_saw_active = false;
};
int raise_character_event(void* raw, std::uint32_t event) {
    auto& capture = *static_cast<Capture*>(raw);
    capture.calls.push_back(event);
    return 0;
}
int stop_game_object(void* raw) {
    auto& capture = *static_cast<Capture*>(raw);
    capture.stop_saw_active = capture.heading->active != 0;
    capture.calls.push_back(1000);
    // Model GameObject::Stop's state clear after its DropPath service begins.
    capture.heading->active = 0;
    capture.heading->direction[0] = capture.heading->direction[1] =
        capture.heading->direction[2] = 0.0f;
    return 0;
}
}

int main() {
    using namespace dh2::native::character_controller_v1;
    dh2::navigation::HeadingState heading{{0, 0, 0}, 0.25f, 0, 0};
    float angle = heading.angle;
    const float forward[3]{0, 1, 0};
    const float zero[3]{};
    Capture capture{&heading, {}, false};
    const Services services{&capture, raise_character_event, stop_game_object};
    Outcome outcome{};

    check(dispatch_head_towards(&heading, &angle, forward, true, true, false, false,
                                &services, &outcome) == Status::complete &&
          outcome == Outcome::heading && heading.active &&
          capture.calls == std::vector<unsigned>{0},
          "accepted HeadTowards must set heading then request Character::RaiseEvent(0)");

    const auto heading_before = heading;
    const auto angle_before = angle;
    capture.calls.clear();
    check(dispatch_head_towards(&heading, &angle, forward, true, true, true, false,
                                &services, &outcome) == Status::complete &&
          outcome == Outcome::skill_blocked && !capture.calls.size() &&
          heading.active == heading_before.active &&
          heading.direction[0] == heading_before.direction[0] &&
          heading.direction[1] == heading_before.direction[1] && angle == angle_before,
          "using-skill gate must block nonzero heading and RaiseEvent");
    check(dispatch_head_towards(&heading, &angle, forward, true, true, false, true,
                                &services, &outcome) == Status::complete &&
          outcome == Outcome::skill_blocked && capture.calls.empty(),
          "casting gate must block nonzero heading and RaiseEvent");

    // Character::Ctrl_HeadTowards applies its 1e-4 squared-length threshold
    // after the gamepad radial mapper, so near-edge mapper activity alone is
    // not enough to accept a heading command.
    float small_mapped_input[3]{0.255f, 0.0f, 0.0f};
    bool mapped_input_active = false;
    check(dh2::native::camera_input_v1::map_touch_ground_input(
              small_mapped_input, 0.0f, 0.0f, false, &mapped_input_active) == 0 &&
          mapped_input_active && small_mapped_input[0] > 0.0f &&
          small_mapped_input[0] * small_mapped_input[0] <= 0.0001f,
          "radial deadzone maps valid near-edge movement below SetHeadingDirection activation epsilon");
    capture.calls.clear();
    capture.stop_saw_active = true;
    check(dispatch_head_towards(&heading, &angle, small_mapped_input, mapped_input_active, true,
                                true, true, &services, &outcome) == Status::complete &&
          outcome == Outcome::stopped && capture.calls == std::vector<unsigned>{1000, 63} &&
          !capture.stop_saw_active && !heading.active,
          "tiny HeadTowards updates direction then stops without skill/casting gates");

    const float start_heading[3]{0, 1, 0};
    check(dh2::native::camera_input_v1::apply_head_towards(
              &heading, &angle, start_heading) == 0 && heading.active,
          "restore active heading before Stop-order regression");
    capture.calls.clear();

    // The source zero-vector branch has no skill/casting predicates: release
    // invokes Ctrl_Stop while heading is still active; GameObject::Stop clears
    // it after DropPath and RaiseEvent(63) follows Stop.
    capture.stop_saw_active = false;
    check(dispatch_head_towards(&heading, &angle, zero, true, true, true, true,
                                &services, &outcome) == Status::complete &&
          outcome == Outcome::stopped && !heading.active &&
          heading.direction[0] == 0 && heading.direction[1] == 0 &&
          !capture.stop_saw_active &&
          capture.calls == std::vector<unsigned>{1000, 63},
          "centered active joystick zero-vector branch sets heading then raises stop event 63");

    capture.calls.clear();
    check(dispatch_head_towards(&heading, &angle, zero, true, true, false, false,
                                &services, &outcome) == Status::complete &&
          outcome == Outcome::unchanged && capture.calls.empty(),
          "zero vector while already stopped must not stop or raise an event");
    check(dh2::native::camera_input_v1::apply_head_towards(
              &heading, &angle, start_heading) == 0 && heading.active,
          "restore active heading for direct release stop ordering");
    capture.calls.clear();
    capture.stop_saw_active = false;
    check(dispatch_head_towards(&heading, &angle, zero, false, true, false, false,
                                &services, &outcome) == Status::complete &&
          outcome == Outcome::stopped && capture.stop_saw_active &&
          capture.calls == std::vector<unsigned>{1000, 63},
          "direct release stops GameObject before clearing prior heading and raises event 63");
    capture.calls.clear();
    check(dispatch_head_towards(&heading, &angle, forward, true, false, false, false,
                                &services, &outcome) == Status::complete &&
          outcome == Outcome::unchanged && !heading.active && capture.calls.empty(),
          "disabled controller must not accept touch heading");

    std::printf("PASS: Character HeadTowards state routing (%u assertions)\n", checks);
    return 0;
}
