#include "camera_design_zoom_v1.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <cstring>

namespace dh2::camera_design_zoom_v1 {
namespace {
constexpr std::size_t kDesignSettingsStride = 43u * sizeof(std::uint32_t);

std::uint32_t read_u32(const std::uint8_t* data) noexcept {
    return std::uint32_t(data[0]) | (std::uint32_t(data[1]) << 8) |
           (std::uint32_t(data[2]) << 16) | (std::uint32_t(data[3]) << 24);
}

float read_float(const std::uint8_t* row, std::size_t word) noexcept {
    const std::uint32_t bits = read_u32(row + word * sizeof(std::uint32_t));
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}
} // namespace

Status decode_global_bounds(const std::uint8_t* data, std::size_t size,
                            Bounds* output) noexcept {
    if (!data || !output || size < sizeof(std::uint32_t))
        return Status::invalid_input;
    const std::uint32_t row_count = read_u32(data);
    if (row_count != 1u) return Status::unexpected_row_count;
    if (size < sizeof(std::uint32_t) + kDesignSettingsStride)
        return Status::invalid_input;

    const std::uint8_t* row = data + sizeof(std::uint32_t);
    const Bounds candidate{
        0u,
        read_float(row, 42u), // runtime member +172, normal minimum
        read_float(row, 41u), // runtime member +168, normal maximum
        read_float(row, 18u), // runtime member +76, alternate minimum
        read_float(row, 17u), // runtime member +72, alternate maximum
        read_float(row, 3u),  // AutoZoomStep / runtime +12
        read_float(row, 4u),  // source UpdateCameraZoom reads runtime +16 as float
        read_float(row, 5u),  // CoopCamLimits_Bottom / runtime +24
        read_float(row, 6u),  // CoopCamLimits_Sides / runtime +28
        read_float(row, 7u)   // CoopCamLimits_Top / runtime +32
    };
    if (!std::isfinite(candidate.normal_min) ||
        !std::isfinite(candidate.normal_max) ||
        !std::isfinite(candidate.alternate_min) ||
        !std::isfinite(candidate.alternate_max) ||
        !std::isfinite(candidate.auto_zoom_step) ||
        !std::isfinite(candidate.auto_zoom_rate) ||
        !std::isfinite(candidate.coop_bottom) ||
        !std::isfinite(candidate.coop_sides) ||
        !std::isfinite(candidate.coop_top) ||
        candidate.normal_min > candidate.normal_max ||
        candidate.alternate_min > candidate.alternate_max)
        return Status::invalid_bounds;
    *output = candidate;
    return Status::complete;
}

bool project_screen_point(const std::uint32_t* matrix,float x,float y,float z,
                          ScreenPoint* out) noexcept {
    if(!matrix||!out||!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(z))return false;
    float values[16]{};
    for(unsigned i=0;i<16;++i){
        std::memcpy(&values[i],&matrix[i],sizeof(values[i]));
        if(!std::isfinite(values[i]))return false;
    }
    const float clip_x=values[0]*x+values[4]*y+values[8]*z+values[12];
    const float clip_y=values[1]*x+values[5]*y+values[9]*z+values[13];
    const float clip_w=values[3]*x+values[7]*y+values[11]*z+values[15];
    if(!std::isfinite(clip_x)||!std::isfinite(clip_y)||!std::isfinite(clip_w)||
       std::abs(clip_w)<1e-8f)return false;
    const ScreenPoint candidate{clip_x/clip_w,clip_y/clip_w};
    if(!std::isfinite(candidate.x)||!std::isfinite(candidate.y))return false;
    *out=candidate;return true;
}

bool update_target_zoom(const Bounds& settings,const ScreenPoint* players,
                        std::size_t count,float* target_zoom) noexcept {
    if(!target_zoom||!std::isfinite(*target_zoom)||
       !std::isfinite(settings.auto_zoom_step)||!std::isfinite(settings.auto_zoom_rate)||
       !std::isfinite(settings.coop_bottom)||!std::isfinite(settings.coop_sides)||
       !std::isfinite(settings.coop_top)||(count&&!players))return false;
    if(!count)return true;
    float margin=std::numeric_limits<float>::max();
    for(std::size_t i=0;i<count;++i){
        if(!std::isfinite(players[i].x)||!std::isfinite(players[i].y))return false;
        margin=std::min(margin,settings.coop_sides-std::abs(players[i].x));
        margin=std::min(margin,settings.coop_top-players[i].y);
        margin=std::min(margin,players[i].y+settings.coop_bottom);
    }
    const float distance=std::abs(settings.auto_zoom_step-margin);
    float candidate=*target_zoom;
    if(settings.auto_zoom_step>margin)candidate-=distance*settings.auto_zoom_rate;
    else if(settings.auto_zoom_step<margin)candidate+=distance*settings.auto_zoom_rate;
    if(!std::isfinite(candidate))return false;
    *target_zoom=candidate;return true;
}

} // namespace dh2::camera_design_zoom_v1
