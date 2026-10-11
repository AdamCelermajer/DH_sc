#pragma once

#include <cstdint>
#include <string>

namespace dh::foundation { class OverlayRenderer; }

namespace dh::foundation::persistence::death_flow {

struct FadeFromBlackFrameV1 {
    // Sprite743, zero-based source timeline frame. Draw the current frame
    // after advancing; pop_menu_callback is raised once on frame32.
    std::uint8_t frame = 0;
    std::uint8_t black_alpha = 255;
    bool pop_menu_callback = false;
};

// Starts after NativeReviveAllPlayers pushes menu_FadeFromBlackScreen.
// The host advances once per source 30fps UI tick and services the frame32
// pop through the existing native menu stack owner.
class OriginalFadeFromBlackRuntimeV1 {
    std::uint8_t frame_ = 0;
    bool started_ = false;
    bool pop_pending_ = false;
    bool popped_ = false;
    bool failed_ = false;
public:
    bool start(std::string& error);
    bool advance(FadeFromBlackFrameV1& output, std::string& error);
    bool confirm_native_pop(bool succeeded, std::string& error);
    bool frame(FadeFromBlackFrameV1& output, std::string& error) const;
    std::uint8_t current_frame() const noexcept { return frame_; }
    bool popped() const noexcept { return popped_; }
};

// Draws retained source symbol496 black-fill geometry using sprite743's root
// and nested sprite738 transforms, scaled to the same 480x320 letterbox.
bool draw_original_fade_from_black_v1(const FadeFromBlackFrameV1& frame,
                                      ::dh::foundation::OverlayRenderer& overlay,
                                      int viewport_width, int viewport_height,
                                      std::string& error);

} // namespace dh::foundation::persistence::death_flow
