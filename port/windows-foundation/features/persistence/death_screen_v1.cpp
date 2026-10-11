#include "death_screen_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation::persistence::death_flow {
namespace {
constexpr std::array<DeathScreenArtPlacementV1, 5> kArt{{
    {DeathScreenArtRoleV1::continue_hit_area, 33,
     {1.f, 0.f, 0.f, 1.f, -10281.f, -46.f},
     {0, 9887, 0, 6540}},
    {DeathScreenArtRoleV1::inactive_block_hit_area, 33,
     {1.f, 0.f, 0.f, 1.f, -281.f, -46.f},
     {0, 9887, 0, 6540}},
    {DeathScreenArtRoleV1::black_fade_overlay, 685,
     {1.0415802001953125f, 0.f, 0.f, 1.0617523193359375f, -242.f, -204.f},
     {0, 9619, 0, 6478}},
    {DeathScreenArtRoleV1::original_panel_art, 739,
     {1.f, 0.f, 0.f, 1.f, 4813.f, 3219.f},
     {-4890, 4890, -3280, 3280}},
    {DeathScreenArtRoleV1::localized_death_quote, 741,
     {1.f, 0.f, 0.f, 1.f, 640.f, 4422.f},
     {0, 8199, 0, 491}},
}};
constexpr DeathScreenTimelineV1 kTimeline{};
} // namespace

const std::array<DeathScreenArtPlacementV1, 5>&
original_death_screen_art_v1() {
    return kArt;
}

const DeathScreenTimelineV1& original_death_screen_timeline_v1() {
    return kTimeline;
}

namespace {
std::uint8_t fade_alpha(std::uint16_t frame, bool hide) {
    (void)hide;
    if (frame < 46) return 0;
    if (frame >= 64) return 255;
    return static_cast<std::uint8_t>(std::min(255L, std::lround(256.0 * (frame - 45) / 19.0)));
}

std::uint8_t panel_alpha(std::uint16_t frame) {
    if (frame == 0) return 0;
    return static_cast<std::uint8_t>(std::min<std::uint16_t>(255, frame * 8));
}

std::uint8_t quote_alpha(std::uint16_t frame, bool hide) {
    if (!hide) {
        if (frame < 65) return 0;
        if (frame >= 74) return 255;
        return static_cast<std::uint8_t>(std::min(255L, std::lround(256.0 * (frame - 64) / 10.0)));
    }
    if (frame < 82) return 255;
    constexpr std::array<std::uint8_t, 7> alpha{{219, 183, 146, 110, 73, 37, 0}};
    return alpha[frame - 82];
}

DeathScreenDrawItemV1 draw_item(DeathScreenArtRoleV1 role,
                                std::uint8_t alpha = 255) {
    for (const auto& art : kArt) {
        if (art.role == role) {
            return {art.role, art.swf_character, art.matrix_twips,
                    art.local_bounds_twips, alpha};
        }
    }
    return {};
}

} // namespace

bool project_original_death_screen_frame_v1(std::uint16_t frame,
                                            bool continue_routed_to_hide,
                                            std::uint8_t quote_number,
                                            DeathScreenFrameV1& output,
                                            std::string& error) {
    output = {};
    if (frame > 88) {
        error = "sprite742 has no authored frame beyond 88";
        return false;
    }
    if (quote_number < 1 || quote_number > 5) {
        error = "GLOBAL_DEATH quote selector must be in [1,5]";
        return false;
    }
    if (continue_routed_to_hide && frame < 76) {
        error = "Hide timeline frames cannot precede the Continue gotoAndPlay label";
        return false;
    }
    if (!continue_routed_to_hide && frame > 75) {
        error = "sprite742 is stopped at frame75 until Continue jumps to Hide";
        return false;
    }

    const auto& timeline = kTimeline;
    output.frame = frame;
    output.quote_number = quote_number;
    output.black_alpha = fade_alpha(frame, continue_routed_to_hide);
    output.stopped = (!continue_routed_to_hide && frame >= timeline.settled_stop_frame) ||
                     (continue_routed_to_hide && frame >= timeline.revive_callback_frame);
    output.continue_visible = !continue_routed_to_hide &&
                              frame >= timeline.continue_button_enabled_frame &&
                              frame < timeline.continue_goto_frame;
    output.continue_hit_enabled = output.continue_visible;
    output.screen_black_callback = !continue_routed_to_hide &&
                                   frame == timeline.screen_black_callback_frame;
    output.revive_callback = continue_routed_to_hide &&
                             frame == timeline.revive_callback_frame;
    output.previous_music_callback = output.revive_callback;

    // Source display-list depths are 7 (fade), 9 (panel), then 10 (quote).
    output.draw_items[output.draw_count++] =
        draw_item(DeathScreenArtRoleV1::black_fade_overlay, output.black_alpha);
    output.draw_items[output.draw_count++] =
        draw_item(DeathScreenArtRoleV1::original_panel_art, panel_alpha(frame));
    output.draw_items[output.draw_count++] =
        draw_item(DeathScreenArtRoleV1::localized_death_quote, quote_alpha(frame, continue_routed_to_hide));
    error.clear();
    return true;
}

bool original_death_continue_hit_test_v1(float stage_x, float stage_y,
                                         std::uint16_t frame) {
    if (frame == 0 || frame >= 76 || !std::isfinite(stage_x) ||
        !std::isfinite(stage_y)) return false;
    const auto& hit = kArt[0];
    const float x = stage_x * 20.f - kTimeline.continue_enabled_x_twips - kTimeline.root_x_twips;
    const float y = stage_y * 20.f - hit.matrix_twips[5] - kTimeline.root_y_twips;
    return x >= hit.local_bounds_twips[0] && x <= hit.local_bounds_twips[1] &&
           y >= hit.local_bounds_twips[2] && y <= hit.local_bounds_twips[3];
}

} // namespace dh::foundation::persistence::death_flow
