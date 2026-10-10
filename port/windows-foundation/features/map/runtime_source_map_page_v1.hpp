#pragma once

#include "../map_ui/map_ui.hpp"
#include "../map_ui/source_map_camera_v1.hpp"
#include "../frontend/art/original_art.hpp"
#include "../frontend/input/frontend_input.hpp"
#include "../character_menu/character_menu.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::map_page {

struct AuthoredMapArtV1 {
    const frontend::art::ScreenArt* screen{};
    std::string swf_sha256;
    std::uint32_t map_sheet_sprite{};
};

// A marker is an already-produced source record. This layer never chooses a
// coordinate or assigns a family from an object name; the caller must gather
// it from the same live owners named by SourceMapBorrowV1.
struct SourceMarkerV1 {
    std::shared_ptr<void> owner;
    std::uint64_t source_id{};
    unsigned family{};
    map_ui::Point3V1 world_position;
};

struct ProjectedMarkerV1 {
    std::shared_ptr<void> owner;
    std::uint64_t source_id{};
    unsigned family{};
    map_ui::Point3V1 world_position;
    std::array<float, 2> authored_screen_position{};
};

struct ServicesV1 {
    map_ui::ServicesV1 source_page;
    // Fresh source producers only. Expected source families are 0,1,2,3,4,7,
    // 10,11,12,14-17; reserved families remain absent.
    std::function<bool(const map_ui::SourceMapBorrowV1&,
                       std::vector<SourceMarkerV1>&, std::string&)> collect_markers;
};

struct SourceFrameV1 {
    character_menu::Frame character_menu;
    std::vector<ProjectedMarkerV1> markers;
    std::array<float, 4> render_map_rect{}; // [left,right,top,bottom], source screen pixels.
    std::uint64_t level_generation{};
};

enum class PageInputV1 { none, legend, reset_zoom };

// Runtime facade for the original MenuCharMenu_Map page. Static page art and
// hit contours are borrowed from the retained original SWF ScreenArt. The
// native map owner remains responsible for drawing its MapIconsDynamic clips.
class RuntimeSourceMapPageV1 {
    struct Impl;
    std::unique_ptr<Impl> impl_;
public:
    explicit RuntimeSourceMapPageV1(ServicesV1);
    ~RuntimeSourceMapPageV1();
    RuntimeSourceMapPageV1(const RuntimeSourceMapPageV1&) = delete;
    RuntimeSourceMapPageV1& operator=(const RuntimeSourceMapPageV1&) = delete;

    bool open(std::string& error);
    bool close(std::string& error);
    bool visible() const noexcept;

    // Produces only markers supplied by actual source owners and projects
    // them with the current Map CameraBase matrices through the actual
    // RenderMap rectangle. The output carries no new marker glyph geometry.
    bool frame(const AuthoredMapArtV1&, const map_ui::SourceMapCameraFrameV1&,
               const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>&,
               int width, int height, SourceFrameV1&, std::string& error);

    // Release coordinates use the existing frontend input point type. Only
    // actual source ScreenArt hit contours for Legend/ResetZoom are dispatched;
    // map markers have no recovered native click action and are not selectable.
    bool release(const AuthoredMapArtV1&, frontend::input::Point screen_point,
                 int width, int height, PageInputV1&, std::string& error);
};

} // namespace dh::foundation::map_page
