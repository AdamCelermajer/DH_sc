#include "player_root_motion_policy_v1.hpp"

#include "move_state.hpp"

namespace dh2::actor::player_root_motion_policy_v1 {

Status project(Projection* out, std::uint32_t character_flags) noexcept {
    if (!out) return Status::invalid_argument;

    dh2::move::Policy source{};
    if (dh2_move_policy(&source, &character_flags) != 0)
        return Status::source_policy_failed;

    out->displacement = source.position_from_visual != 0;
    return Status::complete;
}

} // namespace dh2::actor::player_root_motion_policy_v1
