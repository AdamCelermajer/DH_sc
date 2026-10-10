#pragma once
// Character-menu Map page model (Preview 16, stream map). Pure math and state: the
// host draws the visited level geometry and markers through the existing renderer.
//
// Original (IDA): MenuCharMenu_Map::CreateMapCamera 0x45351c creates the CameraLevel
// from minimapcameras.bdae (anim set "Default", clip PlayerCamera_Default_minimap),
// with CameraBase::SetData(FOV 0.42963 rad, aspect 1.5, near 0, far 100000) and up
// vector (-1,1,0); MenuCharMenu_Map::Show targets the local player. The map is a
// top-down render of the level through that camera (pose: map_camera_pose_v1.hpp).
// MenuCharMenu_Map::UpdateMapCamera 0x45310c keeps the camera inside the visited extent:
// the visited zones' bounds padded by their own size on each side (Show 0x455138).
//
// Original input is touch: ZoomHandler::onEvent 0x382040 (pinch changes CameraLevel +136,
// drag changes the pan offsets +152/+156), Reset zoom = ZoomHandler::ResetZoom 0x38201c.
// P16 MAPFIX PC adaptation (labelled): mouse wheel and left-button drag, plus keys
// (+/- zoom, arrows pan, Home reset). The zoom range below is the PC adaptation.
#include "map_camera_pose_v1.hpp"
#include "room_zone_visit_v1.hpp"

#include <array>
#include <optional>
#include <vector>

namespace dh::foundation::map_visit {

// Authored CreateMapCamera constants.
inline constexpr float map_camera_fov_radians = 0.42963f;
inline constexpr float map_camera_aspect = 1.5f;
// PC adaptation: zoom factor range (1 = authored pose).
inline constexpr float map_zoom_min = 1.0f;
inline constexpr float map_zoom_max = 4.0f;

struct MapExtentV1 {
    bool valid = false;
    float minX = 0, minY = 0, minZ = 0, maxX = 0, maxY = 0, maxZ = 0;
};
// Union of every zone's room box (empty when the level has no zones).
MapExtentV1 map_extent_v1(const std::vector<LevelModuleZone>& zones);
// UpdateMapCamera pan limits: bounds of the visited zones, padded by their own width/height on
// each side (IDA Show 0x455138 accumulates the visited zones and pads). Falls back to every zone
// when nothing is visited yet. visited[i] belongs to zones[i].
MapExtentV1 map_visited_extent_v1(const std::vector<LevelModuleZone>& zones, const std::vector<bool>& visited);

struct MapViewV1 {
    float zoom = 1.0f;
    // Pan of the camera centre in world units along the screen axes (x: (1,1,0)/sqrt2, y: (-1,1,0)/sqrt2).
    float panX = 0.0f;
    float panY = 0.0f;
    bool legend = false;
};
// Clamped zoom step; delta > 0 zooms in. Returns the new view.
MapViewV1 map_zoom_step_v1(MapViewV1 view, float delta);
// Pans the view by screen-space world distances (right, up) along the map axes.
MapViewV1 map_pan_screen_v1(MapViewV1 view, float right, float up);
// Reset zoom: zoom back to the authored pose and centre the pan (the legend state is kept).
MapViewV1 map_reset_zoom_v1(MapViewV1 view);

// Keeps the authored camera's eye inside the visited extent (UpdateMapCamera), with the zoom applied.
MapViewV1 map_clamp_view_v1(const MapCameraPoseV1& pose, const std::array<float, 3>& anchor,
                            const MapExtentV1& limits, const MapViewV1& view);

// Map camera: the authored eye/target offsets (scaled by the zoom distance and panned) placed on the
// anchor (the local player). up (-1,1,0), FOV and aspect are the authored constants.
bool map_camera_v1(const MapCameraPoseV1& pose, const std::array<float, 3>& anchor,
                   const MapViewV1& view, Camera& out, std::string& error);

// True when the point lies inside a visited zone's box (XY inclusive), as RoomZone::HasInside and
// MenuCharMenu_Map::IsInsideRooms(point, visited) (IDA 0x453be8). visited[i] belongs to zones[i].
bool map_point_visited_v1(const std::vector<LevelModuleZone>& zones, const std::vector<bool>& visited,
                          const std::array<float, 3>& point);

// World units per screen pixel at the target distance of the zoomed authored pose, for a rectangle of the given
// pixel height (drag and key pan: a pixel moves the map by this much).
float map_world_per_pixel_v1(const MapCameraPoseV1& pose, const MapViewV1& view, float rectHeightPx);

// Projects a world point to a pixel in the RenderMap rectangle (top-left origin,
// y down). Returns false when the point is behind the camera or outside the rectangle.
struct MapRectV1 { float x = 0, y = 0, width = 0, height = 0; };
bool map_project_v1(const Camera& camera, const MapRectV1& rect, const std::array<float, 3>& world, float& px, float& py);

} // namespace dh::foundation::map_visit
