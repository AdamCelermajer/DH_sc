#pragma once

#include "level_transition_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::ui {

struct ApplicationLevelLoadStateV1 {
    std::uintptr_t level{};
    std::uintptr_t player_savegame{};
    std::uintptr_t level_savegame{};
    std::int32_t level_state{};
    // Application::LoadLevel invokes the loading-menu callback when Level+0x198
    // is clear. The callback is supplied only for that source condition.
    std::uint8_t loading_callback_required{};
};

struct ApplicationLevelLoadServicesV1 {
    void* context{};
    bool (*show_loading)(void*, std::uintptr_t level, std::string&){};
    bool (*set_level_flag_145)(void*, std::uintptr_t level,
                               std::uint8_t value, std::string&){};
    bool (*send_high_score)(void*, std::uintptr_t level, std::string&){};
    bool (*set_level_flag_f0)(void*, std::uintptr_t level,
                              std::uint8_t value, std::string&){};
    bool (*save_local_player)(void*, std::uintptr_t level,
                              std::uintptr_t player_savegame,
                              bool force, std::string&){};
    bool (*quick_save)(void*, std::uintptr_t level,
                       std::uintptr_t level_savegame,
                       bool force, std::string&){};
    bool (*reset_is_loaded)(void*, std::uintptr_t level, std::string&){};
    // Continues Application::LoadLevel's seed/online branch and then GSLevel::LoadLevel.
    bool (*continue_application_load)(void*, const LevelTransitionLoadV1&,
                                      std::string&){};
};

enum class ApplicationLevelLoadPhaseV1 : std::uint8_t {
    idle, loading_callback, flag_145, high_score, flag_f0, save_player,
    quick_save, reset_is_loaded, state_request, complete
};

enum class ApplicationLevelLoadStatusV1 : std::uint8_t {
    complete, skipped_not_loaded, invalid_state, provider_unavailable,
    provider_failed
};

// Application::LoadLevel's already-loaded Level branch at ELF 0x32bdc8.
// This adapter stops at the source ResetIsLoaded boundary. It borrows one
// Level, PlayerSavegame and per-level LevelSavegame identity and creates none.
class ApplicationLevelLoadHandoffV1 {
    ApplicationLevelLoadServicesV1 services_{};
    ApplicationLevelLoadPhaseV1 phase_{ApplicationLevelLoadPhaseV1::idle};
    bool active_{};
public:
    explicit ApplicationLevelLoadHandoffV1(ApplicationLevelLoadServicesV1);
    ApplicationLevelLoadHandoffV1(const ApplicationLevelLoadHandoffV1&) = delete;
    ApplicationLevelLoadHandoffV1& operator=(const ApplicationLevelLoadHandoffV1&) = delete;

    ApplicationLevelLoadStatusV1 run(const ApplicationLevelLoadStateV1&,
        const LevelTransitionLoadV1&, std::string& error);
    ApplicationLevelLoadPhaseV1 reached_phase() const noexcept { return phase_; }
};

} // namespace dh2::ui
