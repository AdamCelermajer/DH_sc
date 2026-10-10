#include "auto_target_marker_v1.hpp"

namespace dh::foundation {

AutoTargetMarkerCandidateV1 resolve_auto_target_marker_v1(
    std::uintptr_t player,
    std::uintptr_t last_target,
    std::uintptr_t object_of_interest) noexcept {
    if (last_target != 0 && last_target != player)
        return {last_target, AutoTargetMarkerSourceV1::last_target};
    if (object_of_interest != 0 && object_of_interest != player)
        return {object_of_interest, AutoTargetMarkerSourceV1::object_of_interest};
    return {};
}

} // namespace dh::foundation
