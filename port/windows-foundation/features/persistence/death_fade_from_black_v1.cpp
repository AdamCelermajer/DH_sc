#include "death_fade_from_black_v1.hpp"

namespace dh::foundation::persistence::death_flow {
namespace {
std::uint8_t alpha_for_frame(std::uint8_t frame) {
    if (frame == 0) return 255;
    if (frame >= 32) return 0;
    return static_cast<std::uint8_t>(256 - frame * 8);
}

}

bool OriginalFadeFromBlackRuntimeV1::start(std::string& error) {
    if (started_) {
        error = "Source fade-from-black runtime has already started";
        return false;
    }
    frame_ = 0;
    started_ = true;
    error.clear();
    return true;
}

bool OriginalFadeFromBlackRuntimeV1::advance(FadeFromBlackFrameV1& output,
                                             std::string& error) {
    if (!started_ || failed_ || popped_) {
        error = "Source fade-from-black is absent, failed, or already popped";
        return false;
    }
    if (pop_pending_) {
        error = "NativePopMenu must be confirmed before advancing past frame32";
        return false;
    }
    if (frame_ < 32) ++frame_;
    pop_pending_ = frame_ == 32;
    return frame(output, error);
}

bool OriginalFadeFromBlackRuntimeV1::confirm_native_pop(bool succeeded,
                                                        std::string& error) {
    if (!started_ || failed_ || popped_ || !pop_pending_) {
        error = "NativePopMenu confirmation is outside sprite743 frame32";
        return false;
    }
    if (!succeeded) {
        failed_ = true;
        error = "Source fade-from-black NativePopMenu provider failed";
        return false;
    }
    pop_pending_ = false;
    popped_ = true;
    error.clear();
    return true;
}

bool OriginalFadeFromBlackRuntimeV1::frame(FadeFromBlackFrameV1& output,
                                           std::string& error) const {
    if (!started_ || failed_) {
        error = "Source fade-from-black has not started or has failed";
        return false;
    }
    output.frame = frame_;
    output.black_alpha = alpha_for_frame(frame_);
    output.pop_menu_callback = pop_pending_;
    error.clear();
    return true;
}

} // namespace dh::foundation::persistence::death_flow
