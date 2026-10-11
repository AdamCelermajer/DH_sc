#include "interaction_approach_v1.hpp"

#include <cmath>

namespace dh::foundation {

bool interaction_in_range_v1(const InteractionRangeInputV1& in) noexcept {
    const float dx = in.owner_target_position[0] - in.interaction_spot[0];
    const float dy = in.owner_target_position[1] - in.interaction_spot[1];
    const float dz = in.owner_target_position[2] - in.interaction_spot[2];
    const float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    float target_radius = 0.0f, owner_melee = 0.0f, limit = 80.0f;
    if (!in.has_interaction_node) {
        target_radius = in.target_radius;
        owner_melee = in.owner_melee_radius;
        limit = in.owner_interact_radius;
    }
    const float v = (distance - owner_melee) - target_radius;
    if (!std::isfinite(v)) return false;
    return in.target_type == 8 ? !(v > 0.0f) : !(v > limit);
}

bool camera_relative_move_v1(float rx, float ry, float fx, float fy, float wx, float wy,
                             InteractionMove2DV1& out) noexcept {
    const float det = rx * fy - ry * fx;
    const float len = std::sqrt(wx * wx + wy * wy);
    if (!(std::fabs(det) > 1e-6f) || !(len > 1e-6f)) return false;
    wx /= len;
    wy /= len;
    // Solve [right forward] * [x y]^T = world direction (Cramer's rule).
    const float x = (wx * fy - wy * fx) / det;
    const float y = (rx * wy - ry * wx) / det;
    const float l = std::sqrt(x * x + y * y);
    if (!(l > 1e-6f)) return false;
    out.x = x / l;
    out.y = y / l;
    return true;
}

} // namespace dh::foundation
