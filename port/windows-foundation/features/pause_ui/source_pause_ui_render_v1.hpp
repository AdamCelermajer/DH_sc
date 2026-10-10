#pragma once

#include "source_pause_ui_v1.hpp"
#include "../frontend/art/original_art.hpp"

#include <array>
#include <string_view>

namespace dh::foundation::pause_ui {

enum class SourcePauseSurfaceV1 : std::uint8_t {
    hud_pause_button,
    pause_page,
    confirmation,
};

struct SourcePauseUiFrameV1 {
    SourcePauseSurfaceV1 surface{};
    bool multiplayer{};
    frontend::art::ScreenArt art;
};

struct SourcePauseUiHitV1 {
    bool hit{};
    SourcePauseActionV1 action{};
    std::string_view authored_path; // View into frame.art.hit_regions; frame must outlive this result.
};

enum class SourcePauseRouteKindV1 : std::uint8_t {
    none,
    open_pause_page,
    resume_game,
    open_main_menu_confirmation,
    return_to_main_menu,
    cancel_confirmation,
    unsupported_action,
};

struct SourcePauseRouteV1 {
    SourcePauseRouteKindV1 kind{SourcePauseRouteKindV1::none};
    bool supported{};
    std::string_view diagnostic;
};

struct SourcePauseTextBindingV1 {
    std::string_view path_suffix;
    std::string_view localization_symbol;
    std::string_view english_fallback;
};

// Returns the authored, settled source frame in the source 480x320 coordinate
// space. The art is a copy of immutable generated source data so the caller may
// supply localized text without changing the feature's canonical art.
SourcePauseUiFrameV1 source_pause_ui_frame_v1(SourcePauseSurfaceV1 surface,
                                               bool multiplayer = false);

// Input coordinates are in the authored 480x320 space, after the caller has
// applied its viewport-to-source transform. Geometry comes only from source
// shape fill triangles; no rectangle/bounds fallback is used.
SourcePauseUiHitV1 source_pause_ui_hit_test_v1(const SourcePauseUiFrameV1& frame,
                                                float source_x,
                                                float source_y) noexcept;

// Pure root-callable command projection. It performs no menu, audio, pause, or
// application transition itself; unsupported authored pages return diagnostics.
SourcePauseRouteV1 source_pause_ui_route_v1(const SourcePauseUiFrameV1& frame,
                                             const SourcePauseUiHitV1& hit) noexcept;

const std::array<SourcePauseTextBindingV1, 8>& source_pause_ui_text_bindings_v1() noexcept;

// Only bitmap1 is used by the exported pause/HUD frames, and in dqhud_droid.swf
// it is the original MenusGraphics_droid atlas. Other movie IDs are rejected.
const char* source_pause_ui_texture_file_v1(std::uint32_t source_bitmap_id) noexcept;
const char* source_pause_ui_font_file_v1(std::uint32_t source_font_id) noexcept;

} // namespace dh::foundation::pause_ui
