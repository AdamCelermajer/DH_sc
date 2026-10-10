#include "../ais_monster_idle_home_v1.hpp"

#include <cstdlib>
#include <iostream>

namespace {
void require(bool value, const char* message) {
    if (!value) {
        std::cerr << message << '\n';
        std::exit(1);
    }
}

dh2::ais_monster_idle_home_v1::Request base() {
    dh2::ais_monster_idle_home_v1::Request request{};
    request.character_state = 13;
    request.home[0] = 0.0f;
    request.home[1] = 0.0f;
    request.home[2] = 0.0f;
    request.absolute_bounds[0] = 500.0f;
    request.absolute_bounds[1] = 500.0f;
    request.absolute_bounds[2] = -20.0f;
    request.absolute_bounds[3] = 520.0f;
    request.absolute_bounds[4] = 520.0f;
    request.absolute_bounds[5] = 20.0f;
    request.current_position[0] = 500.0f;
    request.current_position[1] = 500.0f;
    request.current_position[2] = 0.0f;
    request.target_position[0] = 500.0f;
    request.target_position[1] = 500.0f;
    request.target_position[2] = 0.0f;
    request.melee_radius = 10.0f;
    return request;
}
}

int main() {
    using dh2::ais_monster_idle_home_v1::request_home_move;
    auto request = base();
    require(request_home_move(request), "idle source state should request return home");
    require(request_home_move(request), "repeated source idle callback should keep requesting home");

    request.character_state = 4;
    require(!request_home_move(request), "non-idle state must not request return home");
    request.character_state = 18;
    request.target_40 = 1;
    require(!request_home_move(request), "retained source target must suppress return home");
    request.target_40 = 0;

    request.absolute_bounds[0] = -50.0f;
    request.absolute_bounds[1] = -50.0f;
    request.absolute_bounds[2] = -20.0f;
    request.absolute_bounds[3] = 50.0f;
    request.absolute_bounds[4] = 50.0f;
    request.absolute_bounds[5] = 20.0f;
    require(!request_home_move(request), "expanded current AABB should suppress return");

    request = base();
    request.target_position_valid = true;
    request.target_position[0] = 5.0f;
    request.target_position[1] = 5.0f;
    require(!request_home_move(request), "cached source target within melee radius should suppress return");
    request.target_position_valid = false;
    require(request_home_move(request), "absent source target node must use GameObject position fallback");

    request.melee_radius = -1.0f;
    require(!request_home_move(request), "invalid melee radius must fail closed");
    std::cout << "AISMonster idle home gate checks passed\n";
}
