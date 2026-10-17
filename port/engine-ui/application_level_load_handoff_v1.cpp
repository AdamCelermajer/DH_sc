#include "application_level_load_handoff_v1.hpp"

namespace dh2::ui {
namespace {
constexpr std::int32_t kLoadedState = 38;

ApplicationLevelLoadStatusV1 missing(const char* provider,
                                     std::string& error) {
    error = std::string("Application::LoadLevel provider unavailable: ") + provider;
    return ApplicationLevelLoadStatusV1::provider_unavailable;
}
}

ApplicationLevelLoadHandoffV1::ApplicationLevelLoadHandoffV1(
    ApplicationLevelLoadServicesV1 services) : services_(services) {}

ApplicationLevelLoadStatusV1 ApplicationLevelLoadHandoffV1::run(
    const ApplicationLevelLoadStateV1& state,
    const LevelTransitionLoadV1& request, std::string& error) {
    error.clear();
    if (active_) {
        error = "Application::LoadLevel handoff is already active";
        return ApplicationLevelLoadStatusV1::invalid_state;
    }
    if (!state.level || !state.player_savegame || !state.level_savegame ||
        state.loading_callback_required > 1) {
        error = "Application::LoadLevel handoff requires one complete existing-level save chain";
        return ApplicationLevelLoadStatusV1::invalid_state;
    }
    if (state.level_state != kLoadedState) {
        phase_ = ApplicationLevelLoadPhaseV1::idle;
        return ApplicationLevelLoadStatusV1::skipped_not_loaded;
    }

    active_ = true;
    struct Reset { bool& active; ~Reset() { active = false; } } reset{active_};
    const auto invoke = [&](ApplicationLevelLoadPhaseV1 phase,
                            const char* provider, auto callback,
                            auto&&... args) {
        phase_ = phase;
        if (!callback) return missing(provider, error);
        if (!callback(services_.context, args..., error)) {
            if (error.empty()) error = std::string("Application::LoadLevel provider failed: ") + provider;
            return ApplicationLevelLoadStatusV1::provider_failed;
        }
        return ApplicationLevelLoadStatusV1::complete;
    };
    auto status = ApplicationLevelLoadStatusV1::complete;

    if (state.loading_callback_required) {
        status = invoke(ApplicationLevelLoadPhaseV1::loading_callback,
                        "loading-menu callback", services_.show_loading,
                        state.level);
        if (status != ApplicationLevelLoadStatusV1::complete) return status;
    }
    status = invoke(ApplicationLevelLoadPhaseV1::flag_145,
                    "Level+0x145 transition flag", services_.set_level_flag_145,
                    state.level, std::uint8_t{1});
    if (status != ApplicationLevelLoadStatusV1::complete) return status;
    status = invoke(ApplicationLevelLoadPhaseV1::high_score,
                    "Application::SendGLHiScore", services_.send_high_score,
                    state.level);
    if (status != ApplicationLevelLoadStatusV1::complete) return status;
    status = invoke(ApplicationLevelLoadPhaseV1::flag_f0,
                    "Level+0xf0 transition flag", services_.set_level_flag_f0,
                    state.level, std::uint8_t{1});
    if (status != ApplicationLevelLoadStatusV1::complete) return status;
    status = invoke(ApplicationLevelLoadPhaseV1::save_player,
                    "Level::SG_SaveLocalPlayer(true)", services_.save_local_player,
                    state.level, state.player_savegame, true);
    if (status != ApplicationLevelLoadStatusV1::complete) return status;
    status = invoke(ApplicationLevelLoadPhaseV1::quick_save,
                    "Level::QuickSave(true)", services_.quick_save,
                    state.level, state.level_savegame, true);
    if (status != ApplicationLevelLoadStatusV1::complete) return status;
    status = invoke(ApplicationLevelLoadPhaseV1::reset_is_loaded,
                    "Level::ResetIsLoaded", services_.reset_is_loaded,
                    state.level);
    if (status != ApplicationLevelLoadStatusV1::complete) return status;
    status = invoke(ApplicationLevelLoadPhaseV1::state_request,
                    "Application::LoadLevel continuation", services_.continue_application_load,
                    request);
    if (status != ApplicationLevelLoadStatusV1::complete) return status;
    phase_ = ApplicationLevelLoadPhaseV1::complete;
    return ApplicationLevelLoadStatusV1::complete;
}

} // namespace dh2::ui
