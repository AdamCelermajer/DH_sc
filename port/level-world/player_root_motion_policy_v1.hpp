#pragma once

#include <cstdint>

namespace dh2::actor::player_root_motion_policy_v1 {

struct Projection {
    bool displacement = false;
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    source_policy_failed,
};

// Project the recovered Character movement policy into the existing visual
// root-motion gate. Character::Move's position-from-visual bit owns this
// decision; this helper does not synthesize a movement speed or direction.
Status project(Projection* out, std::uint32_t character_flags) noexcept;

} // namespace dh2::actor::player_root_motion_policy_v1
