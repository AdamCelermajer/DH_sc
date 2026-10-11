#pragma once

#include "death_flow_v1.hpp"
#include "death_screen_v1.hpp"

namespace dh::foundation::persistence::death_flow {

struct DeathScreenRuntimeServicesV1 {
    bool (*native_screen_is_black)(void*, std::string&) = nullptr;
    void* screen_black_context = nullptr;
    ReviveServicesV1 revive{};
    bool (*native_play_previous_music)(void*, std::string&) = nullptr;
    void* previous_music_context = nullptr;
};

// Deterministic one-authored-frame stepper. The host UI loop calls advance()
// at the SWF's 30fps cadence and calls on_release() only for pointer-up input.
// It owns neither the host loop nor rendering; frame() feeds the source renderer.
class DeathScreenRuntimeV1 {
    DeathFlowTicketV1* flow_ = nullptr;
    std::uint16_t frame_ = 0;
    std::uint8_t quote_number_ = 1;
    bool hide_timeline_ = false;
    bool started_ = false;
    bool complete_ = false;
    bool failed_ = false;

public:
    bool start(DeathFlowTicketV1&, std::uint8_t quote_number, std::string& error);
    bool advance(bool current_level_type_38,
                 const DeathScreenRuntimeServicesV1&, std::string& error);
    bool on_release(float stage_x, float stage_y,
                    const ContinueServicesV1&, std::string& error);
    bool frame(DeathScreenFrameV1&, std::string& error) const;
    std::uint16_t current_frame() const noexcept { return frame_; }
    bool hide_timeline() const noexcept { return hide_timeline_; }
    bool complete() const noexcept { return complete_; }
    bool failed() const noexcept { return failed_; }
};

} // namespace dh::foundation::persistence::death_flow
