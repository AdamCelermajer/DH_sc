#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::camera_design_zoom_v1 {

// Projection of the single source DesignSettingsTable row used by
// CameraLevel::HandleZoom. The packed cache row excludes the runtime vptr;
// these offsets are cache-word indices from its first serialized field.
struct Bounds {
    std::uint32_t row = 0;
    float normal_min = 0.0f;
    float normal_max = 0.0f;
    float alternate_min = 0.0f;
    float alternate_max = 0.0f;
    // Level::UpdateCameraZoom source fields from the same row. The source
    // reads AutoZoomStep and then uses the raw +16 word as a float rate.
    float auto_zoom_step = 0.0f;
    float auto_zoom_rate = 0.0f;
    float coop_bottom = 0.0f;
    float coop_sides = 0.0f;
    float coop_top = 0.0f;
};

struct ScreenPoint { float x=0.0f,y=0.0f; };

// CameraBase::GetScreenCoord multiplies the character +352 point by the
// active camera view-projection matrix and returns clip x/w,y/w.
// Matrix storage is the source engine's 32-bit Word[16]. Interpret each word
// as IEEE-754 bits (do not numerically convert the integer values).
bool project_screen_point(const std::uint32_t* view_projection,float x,float y,float z,
                          ScreenPoint* out) noexcept;
// Source Level::UpdateCameraZoom: take the smallest configured edge margin
// across live player points, then adjust CameraLevel target zoom +140.
bool update_target_zoom(const Bounds&,const ScreenPoint* players,std::size_t count,
                        float* target_zoom) noexcept;

enum class Status : std::uint8_t {
    complete,
    invalid_input,
    unexpected_row_count,
    invalid_bounds
};

// design_pyarray.bin begins with the DesignSettings row count followed by
// 43-word/172-byte rows. The supplied cache has one global row; require that
// exact shape instead of silently treating a future multi-row table as row 0.
// Fields 17/18 (MiniMapZoomMaxLimit/MiniMapZoomMinLimit) and 41/42
// (ZoomMaxLimit/ZoomMinLimit) are confirmed by pystructnames.bin.
Status decode_global_bounds(const std::uint8_t* data, std::size_t size,
                            Bounds* output) noexcept;

} // namespace dh2::camera_design_zoom_v1
