#include "death_screen_runtime_v1.hpp"

namespace dh::foundation::persistence::death_flow {
namespace {
bool call(bool (*provider)(void*, std::string&), void* context,
          const char* missing, std::string& error) {
    if (!provider) {
        error = missing;
        return false;
    }
    if (!provider(context, error)) {
        if (error.empty()) error = missing;
        return false;
    }
    error.clear();
    return true;
}
}

bool DeathScreenRuntimeV1::start(DeathFlowTicketV1& flow,
                                 std::uint8_t quote_number,
                                 std::string& error) {
    if (started_ || !flow.valid ||
        flow.route != LocalDeathRouteV1::show_offline_death_fade ||
        flow.continue_consumed || flow.revive_callback_consumed) {
        error = "Original offline death screen requires a fresh admitted flow";
        return false;
    }
    if (quote_number < 1 || quote_number > 5) {
        error = "GLOBAL_DEATH quote selector must be in [1,5]";
        return false;
    }
    flow_ = &flow;
    quote_number_ = quote_number;
    frame_ = 0;
    hide_timeline_ = false;
    started_ = true;
    error.clear();
    return true;
}

bool DeathScreenRuntimeV1::advance(
    bool current_level_type_38,
    const DeathScreenRuntimeServicesV1& services,
    std::string& error) {
    if (!started_ || !flow_ || failed_) {
        error = "Death screen timeline is absent or already failed";
        return false;
    }
    if (complete_) {
        error = "Death screen authored timeline has already completed";
        return false;
    }
    if (!hide_timeline_) {
        if (frame_ >= 75) {
            error.clear(); // Authored opening clip remains stopped on frame75.
            return true;
        }
        ++frame_;
        if (frame_ == 64) {
            if (!call(services.native_screen_is_black,
                      services.screen_black_context,
                      "Frame64 NativeScreenIsBlack source callback is required", error)) {
                failed_ = true;
                return false;
            }
        }
        error.clear();
        return true;
    }

    if (frame_ < 76 || frame_ >= 88) {
        error = "Hide timeline frame is outside its source-authored interval";
        failed_ = true;
        return false;
    }
    ++frame_;
    if (frame_ == 88) {
        if (!run_authored_revive_v1(*flow_, current_level_type_38,
                                    services.revive,
                                    services.native_play_previous_music,
                                    services.previous_music_context, error)) {
            failed_ = true;
            return false;
        }
        complete_ = true;
    }
    error.clear();
    return true;
}

bool DeathScreenRuntimeV1::on_release(float stage_x, float stage_y,
                                     const ContinueServicesV1& services,
                                     std::string& error) {
    if (!started_ || !flow_ || failed_ || complete_ || hide_timeline_) {
        error = "Continue release is outside the active death-screen timeline";
        return false;
    }
    if (!original_death_continue_hit_test_v1(stage_x, stage_y, frame_)) {
        error = "Pointer release did not hit the authored Continue control";
        return false;
    }
    if (!activate_continue_v1(*flow_, services, error)) {
        failed_ = true;
        return false;
    }
    hide_timeline_ = true;
    frame_ = 76;
    error.clear();
    return true;
}

bool DeathScreenRuntimeV1::frame(DeathScreenFrameV1& output,
                                std::string& error) const {
    if (!started_ || !flow_) {
        error = "Death screen has not started";
        return false;
    }
    return project_original_death_screen_frame_v1(
        frame_, hide_timeline_, quote_number_, output, error);
}

} // namespace dh::foundation::persistence::death_flow
