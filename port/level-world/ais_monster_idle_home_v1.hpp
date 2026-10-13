#pragma once

#include <cmath>

namespace dh2::ais_monster_idle_home_v1 {

// Source AISMonster::OnUpdate (0x3dd588) gate after AISDefault::OnUpdate:
// idle/standing state, no Character+0x408 target, outside the nearby AABB,
// and GetTargetPosition farther than the AITable LeashDistance (row+0x1c; the
// AI melee radius is row+0x20) from the initial position at +0x1450.
// CharAI::SetScriptByName (0x3ceeb0) selects AISMonster only for the reserved
// script "__monster__"; every plain-named script (including "monster") runs
// AISExternal, whose OnUpdate (0x3dce64) never reaches this gate.
struct Request {
    int character_state = -1;
    unsigned target_40 = 0;
    float home[3]{};
    float absolute_bounds[6]{}; // min XYZ, max XYZ, GameObject +0x12c
    float current_position[3]{}; // GameObject +0x160 fallback
    float target_position[3]{}; // GameObject +0x184 when its source gates hold
    float leash_distance = 0.0f; // AITable LeashDistance, row+0x1c
    bool target_position_valid = false;
};

inline bool request_home_move(const Request& request) {
    if (request.character_state != 3 && request.character_state != 13 &&
        request.character_state != 18) return false;
    if (request.target_40 != 0 || !std::isfinite(request.leash_distance) ||
        request.leash_distance < 0.0f) return false;

    for (unsigned axis = 0; axis < 3; ++axis) {
        const float current = request.current_position[axis];
        const float target = request.target_position_valid
            ? request.target_position[axis] : current;
        if (!std::isfinite(request.home[axis]) ||
            !std::isfinite(request.absolute_bounds[axis]) ||
            !std::isfinite(request.absolute_bounds[axis + 3]) ||
            request.absolute_bounds[axis] > request.absolute_bounds[axis + 3] ||
            !std::isfinite(current) || !std::isfinite(target)) return false;
    }
    bool home_near_bounds = true;
    for (unsigned axis = 0; axis < 3; ++axis) {
        const float home = request.home[axis];
        if (home < request.absolute_bounds[axis] - 100.0f ||
            home > request.absolute_bounds[axis + 3] + 100.0f) {
            home_near_bounds = false;
            break;
        }
    }
    if (home_near_bounds) return false;

    const float* target = request.target_position_valid
        ? request.target_position : request.current_position;
    float distance_squared = 0.0f;
    for (unsigned axis = 0; axis < 3; ++axis) {
        const float delta = request.home[axis] - target[axis];
        distance_squared += delta * delta;
    }
    const float leash_squared = request.leash_distance * request.leash_distance;
    return distance_squared > leash_squared;
}

} // namespace dh2::ais_monster_idle_home_v1
