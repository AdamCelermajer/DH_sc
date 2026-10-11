#pragma once

#include <array>
#include <cstdint>

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
    std::uint16_t screen_black_callback_frame = 64;
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
};

// Source geometry from dqhud_droid.swf SHA-256
// a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238.
// Coordinates use sprite742-local space. The continue and block controls use
// symbol33, whose child shape32 fills almost the whole stage as a hit target.
const std::array<DeathScreenArtPlacementV1, 5>&
original_death_screen_art_v1();
const DeathScreenTimelineV1& original_death_screen_timeline_v1();

} // namespace dh::foundation::persistence::death_flow
