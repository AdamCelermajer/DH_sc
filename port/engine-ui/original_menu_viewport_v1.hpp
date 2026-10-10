#pragma once

#include <array>
#include <cstdint>
#include <string>

#include "gameplay_menu_stack_v1.hpp"

namespace dh2::ui::original_menu_viewport_v1 {

// The original front SWFs' authored stage rectangle is [0,9600] x [0,6400]
// twips. SWF coordinates use 20 twips per logical pixel: 480 x 320 (3:2).
// Keep this logical stage separate from the source MenuFlash2DCamera's live
// full-driver display viewport.
inline constexpr std::int32_t stage_width_twips = 9600;
inline constexpr std::int32_t stage_height_twips = 6400;

// IDA: MenuFlash2DCamera::Update (0x42cd84) submits the full driver viewport
// and full driver bounds with scale_mode 0. root::set_display_bounds
// (0x7755f4) does not aspect-adjust mode 0; begin_display maps the authored
// axes independently. Thus 480x320 on 2400x1080 is horizontally stretched
// (5.0 vs 3.375 display pixels per authored pixel). Keep that source behavior
// and use the same full surface for rendering and input conversion.
inline float surface_aspect(std::int32_t surface_width,
                            std::int32_t surface_height) noexcept {
    if (surface_width <= 0 || surface_height <= 0)
        return static_cast<float>(stage_width_twips) / stage_height_twips;
    return static_cast<float>(surface_width) / surface_height;
}

// Android/OpenGL pixel coordinates [x,y,width,height].
inline std::array<std::int32_t, 4> full_surface(std::int32_t surface_width,
                                               std::int32_t surface_height) noexcept {
    if (surface_width <= 0 || surface_height <= 0) return {};
    return {0, 0, surface_width, surface_height};
}

// A contained-stage helper remains useful for comparisons and static artwork.
// It is not the source MenuFlash2DCamera policy for front/menu SWFs.
inline std::array<std::int32_t, 4> authored_stage_viewport(
        std::int32_t surface_width, std::int32_t surface_height) noexcept {
    if (surface_width <= 0 || surface_height <= 0) return {};
    if (std::int64_t(surface_width) * stage_height_twips <=
            std::int64_t(surface_height) * stage_width_twips) {
        const auto height = static_cast<std::int32_t>(
                std::int64_t(surface_width) * stage_height_twips / stage_width_twips);
        return {0, (surface_height - height) / 2, surface_width, height};
    }
    const auto width = static_cast<std::int32_t>(
            std::int64_t(surface_height) * stage_width_twips / stage_height_twips);
    return {(surface_width - width) / 2, 0, width, surface_height};
}

// This classifier identifies front-menu states for callers. It does not pick
// a smaller aspect-fit rectangle: the source camera uses the full driver bounds
// and the SWF maps its authored stage across those bounds.
inline bool uses_authored_menu_stage_v1(const std::string& state) {
    return state == "main" || gameplay_menu_state_v1(state);
}

inline std::array<std::int32_t, 4> viewport_for_screen_state_v1(
        const std::string& state, std::int32_t surface_width,
        std::int32_t surface_height) {
    (void) state;
    return full_surface(surface_width, surface_height);
}

// The original MenuFlash2DCamera uses the full driver bounds for front SWFs;
// the 3D menu backdrop receives the same full-surface rectangle.
inline std::array<std::int32_t, 4> background_viewport(
        std::int32_t surface_width, std::int32_t surface_height) noexcept {
    return full_surface(surface_width, surface_height);
}

}  // namespace dh2::ui::original_menu_viewport_v1
