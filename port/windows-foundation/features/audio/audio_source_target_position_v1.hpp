#pragma once

#include "../../actor_state.hpp"
#include <array>

namespace dh::foundation::audio {

// Audio spatial consumers use the current actor's GameObject target point.
// A cached +0x184 point belongs to that actor's authored target_node only;
// constructor-initialized {0,0,0} must not override transform when the node is
// known null (or its ownership has not been bound).
inline std::array<float,3> audio_source_target_position_v1(
    const ActorState& actor) noexcept {
    if(actor.source_target_node180&&*actor.source_target_node180!=0&&
       actor.source_target_position184)
        return *actor.source_target_position184;
    return actor.transform.position;
}

} // namespace dh::foundation::audio
