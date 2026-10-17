#pragma once

#include "../navigation/navigation.hpp"

#include <cstdint>

// Bounded, host-side movement experiment over the checked SWAMP height mesh.
// This is a height-constrained point step, not the original game controller.
namespace dh2::movement {
enum class Error : std::uint32_t {
    ok,
    argument,
    navigation_query,
    no_module_zero_floor,
};

struct Input {
    float position[3];
    float heading_radians;
    float stick_x;
    float stick_y;
    float dt_seconds;
};

struct Output {
    float position[3];
    float heading_radians;
    float dt_used_seconds;
    float floor_height;
    std::uint32_t floor_surface_index;
    bool moved;
    bool floor_sample_valid;
};

constexpr float max_dt_seconds = 0.1f;
constexpr float max_speed_units_per_second = 100.0f;
// Constructor-derived PFObject baseline: can pass floors requiring water bit 2.
constexpr std::uint32_t baseline_object_path_mask = 2U;
// PFWorld::ValidatePosition's default vertical tolerance; acceptance is strict.
constexpr float native_floor_vertical_tolerance = 100.0f;
constexpr float edge_tolerance = 1.0e-6f;
}

extern "C" {
// One deterministic world-axis stick step. A normalized stick is clamped to
// unit length if its diagonal magnitude exceeds one. Heading uses +Y forward,
// so a +X input faces -pi/2, matching the dev viewer's yaw convention.
// speed_units_per_second, the module-zero restriction, and endpoint-only step
// are explicit host-slice choices. Actor floor eligibility uses the verified
// constructor baseline path mask; this is not the original player controller.
dh2::movement::Error dh2_swamp_movement_step(
    const dh2::navigation::Navigation*, const dh2::movement::Input*,
    float speed_units_per_second, dh2::movement::Output*);
}
