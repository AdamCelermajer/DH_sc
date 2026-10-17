#include "../item_drop_motion_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>

using dh2::item_drop_motion_v1::State;

namespace {
void check(bool value, const char* message, unsigned& checks) {
    ++checks;
    if (!value) {
        std::fprintf(stderr, "FAIL item_drop_motion_v1: %s\n", message);
        std::exit(1);
    }
}
}

int main() {
    unsigned checks = 0;
    const auto flat = dh2::item_drop_motion_v1::update(
        {0.0f, 0.0f, 0.0f}, {100.0f, 0.0f, 0.0f});
    check(flat.state == State::moving && std::fabs(flat.velocity[0] - 2.5f) < 1e-6f &&
              flat.velocity[1] == 0.0f,
          "flat destination uses source speed 2.5", checks);

    const auto elevated = dh2::item_drop_motion_v1::update(
        {0.0f, 0.0f, 0.0f}, {100.0f, 0.0f, 100.0f});
    check(elevated.state == State::moving &&
              std::fabs(elevated.velocity[0] - (2.5f / std::sqrt(2.0f))) < 1e-6f,
          "3D normalization scales horizontal physical velocity", checks);

    const auto item_stop = dh2::item_drop_motion_v1::update(
        {0.0f, 0.0f, 0.0f}, {79.0f, 0.0f, 900.0f});
    check(item_stop.state == State::item_stopped && item_stop.velocity[0] == 0.0f,
          "ItemObject XY stop runs before GameObject physical motion", checks);

    const auto boundary = dh2::item_drop_motion_v1::update(
        {0.0f, 0.0f, 0.0f}, {80.0f, 0.0f, 0.0f});
    check(boundary.state == State::moving && std::fabs(boundary.velocity[0] - 2.5f) < 1e-6f,
          "source XY stop threshold is strict below 6400", checks);

    const auto physical_snap = dh2::item_drop_motion_v1::gameobject_physical_update(
        {0.0f, 0.0f, 0.0f}, {10.0f, 0.0f, 0.0f});
    check(physical_snap.state == State::physical_snap && physical_snap.velocity[0] == 0.0f,
          "base GameObject stops and snaps at XYZ distance squared <= 400", checks);

    const auto invalid = dh2::item_drop_motion_v1::update(
        {0.0f, 0.0f, 0.0f}, {NAN, 1.0f, 0.0f});
    check(invalid.state == State::invalid,
          "non-finite endpoint is rejected", checks);

    std::array<float, 3> shell_position{12.0f, 34.0f, 5.0f};
    std::array<float, 3> shell_destination{212.0f, 134.0f, 25.0f};
    bool shell_motion_active = true;
    dh2::item_drop_motion_v1::de_spawn_projection(
        shell_position, shell_destination, shell_motion_active);
    check(!shell_motion_active && shell_destination == shell_position,
          "DeSpawn detaches the old destination but retains shell position", checks);
    shell_destination = {312.0f, 134.0f, 25.0f};
    shell_motion_active = true;
    const auto rebound = dh2::item_drop_motion_v1::update(
        shell_position, shell_destination);
    check(shell_motion_active && rebound.state == State::moving &&
              std::fabs(rebound.velocity[0]) > 0.0f,
          "InitAgain rebind starts motion from the retained shell position", checks);

    std::printf("PASS item_drop_motion_v1 checks=%u\n", checks);
}
