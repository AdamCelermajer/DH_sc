#pragma once
#include <cstdint>

namespace dh2::ui {
// Original root movie rectangle uses [xmin,xmax,ymin,ymax] in twips.
struct ViewportState64 {
    float movie_rect[4];
    std::int32_t viewport[4]; // x,y,width,height
    std::int32_t bounds[4];   // x,y,width,height
    float pixel_scale;
    std::uint32_t reserved;
    std::uintptr_t player_receiver; // live original weak-player projection, 0 skips
};
struct FlashCamera40 {
    float limit_rect[4]; // captured pixel rectangle, same ordering as movie_rect
    std::uintptr_t limit_identity;
    std::int32_t current[2], desired[2];
};
enum class ViewportOperation : std::uint32_t {
    orientation=1, driver_dimensions=2, publish_viewport=3,
    camera_set_viewport=4, camera_set_bounds=5
};
struct ViewportRequest40 {
    ViewportOperation operation;
    std::uint32_t reserved;
    std::int32_t values[4];
    float rectangle[4];
};
struct ViewportResponse16 { std::int32_t values[4]; };
// Return 1 delivered, 0 unavailable. The owner can synchronously reenter;
// state writes before player-global Viewport publication remain on failure.
struct ViewportServices16 {
    void* context;
    int (*invoke)(void*, ViewportState64*, const ViewportRequest40*, ViewportResponse16*);
};
static_assert(sizeof(ViewportState64)==64);
static_assert(sizeof(FlashCamera40)==40);
static_assert(sizeof(ViewportRequest40)==40);
static_assert(sizeof(ViewportResponse16)==16);
static_assert(sizeof(ViewportServices16)==16);
}
// 0 success, -1 malformed pointer/overlap, -2 required provider unavailable.
// IEEE values and signed dimensions are source inputs, not aspect-fit policy.
extern "C" {
int dh2_ui_set_bounds(dh2::ui::ViewportState64*, const std::int32_t xywh[4], std::int32_t mode, const dh2::ui::ViewportServices16*);
int dh2_ui_set_viewport(dh2::ui::ViewportState64*, const std::int32_t xywh[4], const dh2::ui::ViewportServices16*);
int dh2_ui_screen_to_logical(dh2::ui::ViewportState64*, float point[2], const dh2::ui::ViewportServices16*);
int dh2_ui_logical_to_screen(dh2::ui::ViewportState64*, float point[2], const dh2::ui::ViewportServices16*);
// Original begin_display's movie-corner projection, [xmin,xmax,ymin,ymax]
// in submitted renderer twips. Distinct from publish_viewport [xMin,yMin,xMax,yMax].
int dh2_ui_display_rectangle(dh2::ui::ViewportState64*, float rectangle[4], const dh2::ui::ViewportServices16*);
int dh2_ui_flash_camera_update(dh2::ui::FlashCamera40*, dh2::ui::ViewportState64*, const dh2::ui::ViewportServices16*);
}
