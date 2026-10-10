#pragma once
// Character-menu Map page model (Preview 16, stream map). Pure math and state: the
// host draws the visited level geometry and markers through the existing renderer.
//
// Original (IDA): MenuCharMenu_Map::CreateMapCamera 0x45351c creates the CameraLevel
// from minimapcameras.bdae (anim set "Default", clip PlayerCamera_Default_minimap),
// with CameraBase::SetData(FOV 0.42963 rad, aspect 1.5, near 0, far 100000) and up
// vector (-1,1,0); MenuCharMenu_Map::Show targets the local player. The map is a
// top-down render of the level through that camera.
// Inference, not decoded: the zoom behaviour. Zoom in centres on the player (the
// camera target); Reset zoom returns to the full-level view centred on the level.
#include "room_zone_visit_v1.hpp"

#include <array>
#include <optional>

namespace dh::foundation::map_visit {

// Authored CreateMapCamera constants.
inline constexpr float map_camera_fov_radians = 0.42963f;
inline constexpr float map_camera_aspect = 1.5f;
inline constexpr float map_zoom_min = 1.0f;
inline constexpr float map_zoom_max = 4.0f;
// Margin so the level's bounding circle is not clipped at the reset view.
inline constexpr float map_fit_margin = 1.08f;

struct MapExtentV1 {
    bool valid = false;
    float minX = 0, minY = 0, minZ = 0, maxX = 0, maxY = 0, maxZ = 0;
};
// Union of every zone's room box (empty when the level has no zones).
MapExtentV1 map_extent_v1(const std::vector<LevelModuleZone>& zones);

struct MapViewV1 {
    float zoom = 1.0f;
    bool legend = false;
};
// Clamped zoom step; delta > 0 zooms in. Returns the new view.
MapViewV1 map_zoom_step_v1(MapViewV1 view, float delta);
MapViewV1 map_reset_zoom_v1(MapViewV1 view);

// Top-down camera for the map viewport. The player position (when present and zoomed)
// is the centre; otherwise the level centre. Returns false when the extent is invalid.
bool map_camera_v1(const MapExtentV1& extent, const std::optional<std::array<float, 3>>& player,
                   const MapViewV1& view, Camera& out, std::string& error);

// Projects a world point to a pixel in the RenderMap rectangle (top-left origin,
// y down). Returns false when the point is behind the camera or outside the rectangle.
struct MapRectV1 { float x = 0, y = 0, width = 0, height = 0; };
bool map_project_v1(const Camera& camera, const MapRectV1& rect, const std::array<float, 3>& world, float& px, float& py);

} // namespace dh::foundation::map_visit
