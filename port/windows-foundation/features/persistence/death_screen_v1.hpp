#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace dh::foundation { class OverlayRenderer; }

namespace dh::foundation::persistence::death_flow {

enum class DeathScreenArtRoleV1 : std::uint8_t {
    continue_hit_area,
    inactive_block_hit_area,
    black_fade_overlay,
    original_panel_art,
    localized_death_quote
};

struct DeathScreenArtPlacementV1 {
    DeathScreenArtRoleV1 role{};
    std::uint16_t swf_character = 0;
    // SWF twips, with 20 twips per stage pixel.
    std::array<float, 6> matrix_twips{};
    std::array<std::int32_t, 4> local_bounds_twips{};
};

struct DeathScreenTimelineV1 {
    std::uint16_t swf_sprite = 742;
    std::uint16_t stage_width_twips = 9600;
    std::uint16_t stage_height_twips = 6400;
    std::uint16_t frame_rate = 30;
    std::uint16_t show_label_frame = 0;
    std::uint16_t continue_button_enabled_frame = 1;
    std::uint16_t panel_fully_visible_frame = 32;
    std::uint16_t black_fade_first_frame = 46;
    std::uint16_t screen_black_callback_frame = 64;
    std::uint16_t quote_fade_first_frame = 65;
    std::uint16_t quote_fully_visible_frame = 74;
    std::uint16_t settled_stop_frame = 75;
    std::uint16_t continue_goto_frame = 76;
    std::int32_t continue_enabled_x_twips = -281;
    std::int32_t continue_hidden_x_twips = -10681;
    std::uint16_t fade_out_first_frame = 82;
    std::uint16_t revive_callback_frame = 88;
    const char* continue_label = "Hide";
    const char* quote_symbol_prefix = "GLOBAL_DEATH";
    const char* quote_symbol_suffix = "_01";
    std::uint8_t quote_symbol_count = 5;
    std::int32_t root_x_twips = -23;
    std::int32_t root_y_twips = -39;
};

// Executable projection of sprite742's display list. The root owner supplies
// the retained SWF symbol renderer and localized Flash text resolver; this
// feature supplies the exact symbols, matrices, frame alpha, layer order and
// full-stage Continue hit region. Stage coordinates are SWF pixels.
struct DeathScreenDrawItemV1 {
    DeathScreenArtRoleV1 role{};
    std::uint16_t swf_character = 0;
    std::array<float, 6> matrix_twips{};
    std::array<std::int32_t, 4> local_bounds_twips{};
    std::uint8_t alpha = 255;
};

struct DeathScreenFrameV1 {
    std::uint16_t frame = 0;
    std::uint8_t quote_number = 1; // GLOBAL_DEATH1_01 through GLOBAL_DEATH5_01
    std::uint8_t black_alpha = 0;
    bool stopped = false;
    bool continue_visible = false;
    bool continue_hit_enabled = false;
    bool screen_black_callback = false;
    bool revive_callback = false;
    bool previous_music_callback = false;
    std::array<DeathScreenDrawItemV1, 3> draw_items{};
    std::size_t draw_count = 0;
};

// Projects one authored sprite frame into visible layers and callbacks.
// `continue_routed_to_hide` means the user already invoked the source
// gotoAndPlay("Hide") action. `quote_number` is selected by the SWF's native
// RandomNumber(5)+1 expression and is injected here for deterministic tests.
bool project_original_death_screen_frame_v1(std::uint16_t frame,
                                            bool continue_routed_to_hide,
                                            std::uint8_t quote_number,
                                            DeathScreenFrameV1& output,
                                            std::string& error);

// Tests the source symbol33 hit rectangle in stage-pixel coordinates. The
// block control is present in the SWF but has no release action.
bool original_death_continue_hit_test_v1(float stage_x, float stage_y,
                                         std::uint16_t frame);

struct DeathScreenDrawServicesV1 {
    bool (*draw_localized_quote)(void*, std::uint8_t quote_number,
                                 const std::array<float, 6>& matrix_twips,
                                 const std::array<std::int32_t, 4>& bounds_twips,
                                 std::uint8_t alpha,
                                 std::string& error) = nullptr;
    void* context = nullptr;
};

// Executes the projected display list with retained source shape739/496
// triangles (OverlayRenderer texture0). The text owner resolves sprite741's
// GLOBAL_DEATH{n}_01 through the source font/localizer.
bool draw_original_death_screen_v1(const DeathScreenFrameV1& frame,
                                   ::dh::foundation::OverlayRenderer& overlay,
                                   int viewport_width, int viewport_height,
                                   const DeathScreenDrawServicesV1& services,
                                   std::string& error);

// Source geometry from dqhud_droid.swf SHA-256
// a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238.
// Coordinates use sprite742-local space. The continue and block controls use
// symbol33, whose child shape32 fills almost the whole stage as a hit target.
const std::array<DeathScreenArtPlacementV1, 5>&
original_death_screen_art_v1();
const DeathScreenTimelineV1& original_death_screen_timeline_v1();

} // namespace dh::foundation::persistence::death_flow
