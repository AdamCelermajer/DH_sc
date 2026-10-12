#pragma once

// B144: the original HUD status line (MenuMessageManager<StatusMsg>, HUDelements.itemname_text sprite 166 of
// dqhud_droid.swf). ItemObject::Interact enqueues the picked item's name (rarity colour) on success and the
// GAMEPLAYMENUS_INVENTORY_FULL text when the bag refuses it. The messages play one after another: the authored
// 48-frame clip starts at its "scroll" label, fades/slides in, holds, fades out, and its last-frame
// onAnimationEnd (NativeStopMessage) pops the message and starts the next queued one. The line is a fixed HUD
// element (it does not follow the world); its stage placement, timeline and font height come from the SWF
// (generated status_message_timeline_v1.inc, see export_status_message_v1.py).
//
// Pure logic: no rendering. The caller draws `view()` with the original Fontin glyph owner.

#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <string>

namespace dh::foundation::hud_panels {

struct StatusMessageViewV1 {
    const std::string* text{};
    std::uint32_t rgb{0xFFFFFF};   // 0xRRGGBB (item quality colour from the FontPalette)
    float alpha{};                 // authored container colour-transform multiplier, 0..1
    float centre_x{};              // 480x320 stage px: centre of the centre-aligned text field
    float baseline_y{};            // 480x320 stage px
    float y_scale{1.f};            // authored container Y scale (slide-in squash), applied to glyph height/offsets
    float font_px{14.f};           // source font height in stage px
    int frame{};                   // current authored frame of the 48-frame clip
};

class StatusMessageQueueV1 {
public:
    // MenuMessageManager::EnqueueMessage: appended; the first message starts playing at the "scroll" label.
    void enqueue(std::string text, std::uint32_t rgb);
    // One actual application tick. Like the original (catch_up=false) the clip advances at most one authored
    // frame per tick when the 1/30 s movie cadence has elapsed. Not called while the game is paused.
    void advance(std::uint32_t dt_ms) noexcept;
    // The message currently playing, if any (its alpha is 0 on the first and last frames).
    bool view(StatusMessageViewV1& out) const;
    std::size_t size() const noexcept { return queue_.size(); }
    void clear() noexcept;

    // Authored constants (also used by the tests).
    static int scroll_frame() noexcept;
    static int frame_count() noexcept;
    static float seconds_per_frame() noexcept;

private:
    struct Message { std::string text; std::uint32_t rgb; };
    std::deque<Message> queue_;
    int frame_{};
    double remainder_ms_{};
};

} // namespace dh::foundation::hud_panels
