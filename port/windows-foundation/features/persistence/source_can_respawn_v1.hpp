#pragma once
#include <optional>

namespace dh::foundation::persistence::death_flow {

// Source Character::CanRespawn (0x3a5248): InitSpawned's byte+5249 and
// resolved property 11 are decisive false gates. A proven NULL GroupInfo is
// decisive true; unknown source ownership or a present group remains unknown.
inline std::optional<bool> source_can_respawn_v1(
    int property_11, std::optional<bool> init_spawned,
    std::optional<bool> source_group_present) noexcept {
    if((init_spawned&&*init_spawned)||property_11<=0)return false;
    if(init_spawned&&!*init_spawned&&source_group_present&&!*source_group_present)return true;
    return std::nullopt;
}

}
