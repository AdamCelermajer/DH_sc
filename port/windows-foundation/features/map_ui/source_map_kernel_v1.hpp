#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::map_ui {

struct Point3V1 { float x{}, y{}, z{}; };
struct Bounds3V1 { float min_x{}, min_y{}, min_z{}, max_x{}, max_y{}, max_z{}; };
struct MapBoundsV1 { float min_x{}, min_y{}, max_x{}, max_y{}; };

struct RoomZoneV1 {
    std::shared_ptr<void> owner;
    bool visited{}; // Fresh RoomZone::HasBeenVisited value from the live Level.
};
struct MapObjectV1 {
    std::shared_ptr<void> owner;
    Bounds3V1 bounds; // Fresh source virtual bounds; xyz order is preserved.
    std::function<bool(bool visible, std::string&)> set_visible;
};
struct RoomServicesV1 {
    // Source RoomZone::HasInside(room, point), never an inferred rectangle.
    std::function<bool(const std::shared_ptr<void>& room, const Point3V1&,
                       bool& inside, std::string&)> has_inside;
};

// Exact source MiniMap camera asset/animation and CameraBase::SetData values
// from MenuCharMenu_Map::CreateMapCamera (0x45351c).
struct MapCameraSourceConfigV1 {
    const char* asset_path = "data/3D/camera/minimapcameras.bdae";
    const char* animation_set = "Default";
    const char* clip = "PlayerCamera_Default_minimap";
    float set_data_0 = 0.42963f;
    float set_data_1 = 1.5f;
    float set_data_2 = 0.0f;
    float set_data_3 = 100000.0f;
    float set_data_4 = 0.0f;
    std::uint8_t camera_byte_133 = 1;
    std::uint32_t camera_word_136 = 0;
    float camera_float_140 = 1.0f;
    bool damping_enabled = false;
    // CameraBase::SetData itself sends this vector through vtable +276; the
    // subsequent MenuCharMenu_Map::CreateMapCamera call overrides it.
    std::array<float, 3> set_data_camera_vector{{0.0f, 0.0f, 1.0f}};
    std::array<float, 3> post_set_data_camera_vector{{-1.0f, 1.0f, 0.0f}};
    std::uint32_t initial_camera_virtual_byte_offset = 276;
};

// Implements the source Show sequence's RoomZone partition and actual object
// bounds aggregation. Each object is toggled to its source visibility result;
// only objects whose center is in a visited room contribute to the fit bounds.
bool source_map_show_bounds_v1(const std::vector<RoomZoneV1>&,
                              const std::vector<MapObjectV1>&,
                              const RoomServicesV1&,
                              std::vector<std::shared_ptr<void>>& visited_rooms,
                              std::vector<std::shared_ptr<void>>& unvisited_rooms,
                              MapBoundsV1&, std::string&);

// MenuCharMenu_Map::UpdateMapCamera (0x453178): clamp player+camera XY to
// source Show's expanded object bounds and retain the source Z offset.
bool source_map_camera_offset_v1(const MapBoundsV1&, const Point3V1& player,
                                 Point3V1& camera_offset, std::string&);

// MenuCharMenu_Map::GetMapScreenCoord (0x45389c): CameraBase screen/NDC
// coordinates are projected through the source gameswf::rect word order
// [left,right,top,bottom] obtained from the real RenderMap character.
bool source_map_screen_coord_v1(const std::array<float, 4>& render_rect,
                               const Point3V1& camera_screen,
                               std::array<float, 2>& output, std::string&);
// Composes the exact MenuMap owner calls: actual CameraBase::GetScreenCoord
// first, then the source RenderMap rectangle transform. The position must
// come from a live source marker producer (player/NPC/exit/objective).
bool source_map_project_marker_v1(
    const std::function<bool(const Point3V1&, Point3V1&, std::string&)>&
        camera_get_screen_coord,
    const Point3V1& source_marker_position,
    const std::array<float, 4>& render_rect,
    std::array<float, 2>& output, std::string&);

// CameraBase::GetScreenCoord (0x40f714). The provider returns the matrices
// from the same active SceneManager for source mode 0 (view) and mode 2
// (projection). Source multiplication is projection * view, then the real
// marker [x,y,z,1] is transformed and divided by homogeneous W.
using SourceCameraMatrixV1 = std::array<float, 16>;
using SourceCameraMatricesV1 = std::function<bool(
    std::uint32_t source_mode, SourceCameraMatrixV1&, std::string&)>;
bool source_camera_base_screen_coord_v1(
    const SourceCameraMatricesV1& matrices, const Point3V1& world_position,
    Point3V1& screen_position, std::string& error);

// Guards any map projection against a camera/frame switch between publication
// and use. `read_active` must inspect the actual active SceneManager and
// CameraBase while the supplied frame lease is held.
struct SourceMapCameraFrameV1 {
    std::shared_ptr<void> frame_lease;
    std::uint64_t generation{};
    std::uintptr_t expected_scene_manager{};
    std::uintptr_t expected_camera{};
    std::function<bool(const std::shared_ptr<void>&, std::uint64_t,
                       std::uintptr_t&, std::uintptr_t&, std::string&)> read_active;
};
bool source_map_camera_frame_current_v1(const SourceMapCameraFrameV1&,
                                        std::string& error);

} // namespace dh::foundation::map_ui
