#include "movement.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace {
bool finite_input(const dh2::movement::Input& in, float speed) {
    return std::isfinite(in.position[0]) && std::isfinite(in.position[1]) &&
        std::isfinite(in.position[2]) && std::isfinite(in.heading_radians) &&
        std::isfinite(in.stick_x) && std::isfinite(in.stick_y) &&
        std::isfinite(in.dt_seconds) && std::isfinite(speed) &&
        in.stick_x >= -1.0f && in.stick_x <= 1.0f &&
        in.stick_y >= -1.0f && in.stick_y <= 1.0f && in.dt_seconds >= 0.0f &&
        speed >= 0.0f && speed <= dh2::movement::max_speed_units_per_second;
}

void preserve_input(const dh2::movement::Input* input, dh2::movement::Output* output) {
    *output = {};
    output->floor_surface_index = std::numeric_limits<std::uint32_t>::max();
    if (input) {
        output->position[0] = input->position[0];
        output->position[1] = input->position[1];
        output->position[2] = input->position[2];
        output->heading_radians = input->heading_radians;
    }
}

dh2::movement::Error query_module_zero(
    const dh2::navigation::Navigation* nav, float x, float y, float reference_z,
    float max_vertical_distance, dh2::navigation::FloorHit* hit) {
    bool found = false;
    const auto error = dh2_nav_query_actor_floor(nav, x, y, reference_z,
        max_vertical_distance, dh2::movement::edge_tolerance,
        dh2::movement::baseline_object_path_mask, hit, &found);
    if (error != dh2::navigation::Error::ok)
        return dh2::movement::Error::navigation_query;
    if (!found)
        return dh2::movement::Error::no_module_zero_floor;
    // PFWorld::ValidatePosition accepts only strict abs(candidateZ-floorZ)<100.
    // The shared query includes its upper band edge, so enforce strictness here.
    if (hit->vertical_distance >= dh2::movement::native_floor_vertical_tolerance)
        return dh2::movement::Error::no_module_zero_floor;

    dh2::navigation::Surface surface{};
    if (dh2_nav_surface(nav, hit->surface_index, &surface) != dh2::navigation::Error::ok)
        return dh2::movement::Error::navigation_query;
    if (surface.module_index != 0)
        return dh2::movement::Error::no_module_zero_floor;
    return dh2::movement::Error::ok;
}
}

extern "C" dh2::movement::Error dh2_swamp_movement_step(
    const dh2::navigation::Navigation* nav, const dh2::movement::Input* input,
    float speed_units_per_second, dh2::movement::Output* output) {
    if (!output) return dh2::movement::Error::argument;
    preserve_input(input, output);
    if (!nav || !input || !finite_input(*input, speed_units_per_second))
        return dh2::movement::Error::argument;

    const float dt = std::min(input->dt_seconds, dh2::movement::max_dt_seconds);
    output->dt_used_seconds = dt;
    const float stick_square = input->stick_x * input->stick_x +
                               input->stick_y * input->stick_y;
    if (dt == 0.0f || speed_units_per_second == 0.0f || stick_square <= 1.0e-12f)
        return dh2::movement::Error::ok;

    float stick_x = input->stick_x;
    float stick_y = input->stick_y;
    if (stick_square > 1.0f) {
        const float inverse_length = 1.0f / std::sqrt(stick_square);
        stick_x *= inverse_length;
        stick_y *= inverse_length;
    }

    dh2::navigation::FloorHit current{};
    auto status = query_module_zero(nav, input->position[0], input->position[1],
        input->position[2], dh2::movement::native_floor_vertical_tolerance, &current);
    if (status != dh2::movement::Error::ok) return status;

    const float dx = stick_x * speed_units_per_second * dt;
    const float dy = stick_y * speed_units_per_second * dt;
    const float next_x = input->position[0] + dx;
    const float next_y = input->position[1] + dy;
    if (!std::isfinite(next_x) || !std::isfinite(next_y))
        return dh2::movement::Error::argument;

    dh2::navigation::FloorHit next{};
    status = query_module_zero(nav, next_x, next_y, input->position[2],
        dh2::movement::native_floor_vertical_tolerance, &next);
    if (status != dh2::movement::Error::ok) return status;

    output->position[0] = next_x;
    output->position[1] = next_y;
    // PFWorld::ValidatePosition writes the sampled floor height back to the
    // accepted candidate after its vertical-distance and path-mask checks.
    output->position[2] = next.height;
    output->heading_radians = std::atan2(-stick_x, stick_y);
    output->floor_height = next.height;
    output->floor_surface_index = next.surface_index;
    output->floor_sample_valid = true;
    output->moved = next_x != input->position[0] || next_y != input->position[1];
    return dh2::movement::Error::ok;
}
