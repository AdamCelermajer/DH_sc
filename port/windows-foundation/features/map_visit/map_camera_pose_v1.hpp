#pragma once
// P16 MAPFIX: authored Map camera pose read from minimapcameras.bdae.
//
// Original (IDA 0x45351c MenuCharMenu_Map::CreateMapCamera): CameraLevel::Load(data/3D/camera/minimapcameras.bdae,
// animation set "Default", node "PlayerCamera_Default_minimap"), CameraTarget::SetTarget(local player), damping off,
// CameraBase::SetData(FOV 0.42963 rad, aspect 1.5, near 0, far 100000) and up vector (-1, 1, 0).
// The authored eye and target are kept as offsets from the scene root. The root is the player anchor
// (CameraTarget follows the target), so the map camera is the authored offset placed on the local player.
// The up vector, FOV and aspect are the IDA constants in map_page_v1.hpp.

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation::map_visit {

inline constexpr const char* map_camera_file_v1 = "data/3D/camera/minimapcameras.bdae";
inline constexpr const char* map_camera_node_v1 = "PlayerCamera_Default_minimap";

struct MapCameraPoseV1 {
    bool loaded = false;
    // Authored eye and target minus the authored scene root (player anchor) position.
    std::array<float, 3> eye_offset{};
    std::array<float, 3> target_offset{};
};

// Parses the BRES bytes of minimapcameras.bdae. Fails closed on a missing node or a nonfinite pose.
bool load_map_camera_pose_v1(const std::vector<std::uint8_t>& bytes, MapCameraPoseV1& out, std::string& error);

} // namespace dh::foundation::map_visit
