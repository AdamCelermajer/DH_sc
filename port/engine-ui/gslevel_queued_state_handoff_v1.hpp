#pragma once

#include "level_transition_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::ui {

struct GsLevelQueuedSwitchStateV1 {
    std::uintptr_t state_machine{};
    std::uintptr_t old_gslevel{};
    std::uintptr_t old_level{};
    std::uintptr_t old_level_savegame{};
    std::uintptr_t object_manager{};
    std::uintptr_t player_manager{};
    std::int32_t level_state{};
    std::uint8_t level_unload_force{}; // Old Level+0xf0, preserved by ResetIsLoaded.
    // Level::_LoadCamera (0x3f1008) publishes these owners; Unload
    // (0x3f0690) deletes CameraLevel first, then CameraOverview.
    std::uint8_t has_level_cleanup_74{}; // DWORD index 74: CameraLevel at +0x128.
    std::uint8_t has_level_cleanup_75{}; // DWORD index 75: CameraOverview at +0x12c.
};

struct GsLevelQueuedSwitchServicesV1 {
    void* context{};
    // Receives the old Level/LevelSavegame and Level::Unload's force byte.
    // Provider must route through the one existing QuickSave/LevelSavegame owner.
    bool (*quick_save)(void*, std::uintptr_t level,
                       std::uintptr_t level_savegame,
                       std::int32_t level_state, bool force,
                       std::string&){};
    bool (*push_loading_menu)(void*, std::uintptr_t level, std::string&){};
    bool (*destroy_level_cleanup_74)(void*, std::uintptr_t level, std::string&){};
    bool (*destroy_level_cleanup_75)(void*, std::uintptr_t level, std::string&){};
    // SaveAllPlayer uses the existing PlayerManager and the same force byte.
    bool (*save_all_players)(void*, std::uintptr_t level,
                             std::uintptr_t player_manager, bool force,
                             std::string&){};
    bool (*stop_all_sounds)(void*, std::uint32_t fade_ms, std::string&){};
    bool (*pop_loading_menu)(void*, std::string&){};
    bool (*clear_level_state)(void*, std::uintptr_t level,
                              std::int32_t state, std::string&){};
    bool (*network_uninit_level)(void*, std::uintptr_t object_manager,
                                 std::string&){};
    bool (*update_player_manager)(void*, std::uintptr_t player_manager,
                                  std::string&){};
    bool (*set_random_seeds)(void*, std::uint32_t seed,
                             std::uint32_t synchronized_seed,
                             std::string&){};
    bool (*close_hud)(void*, std::string&){};
    bool (*delete_old_level)(void*, std::uintptr_t level, std::string&){};
    bool (*clear_current_gslevel)(void*, std::uintptr_t gslevel,
                                  std::string&){};
    bool (*construct_destination)(void*, const LevelTransitionLoadV1&,
                                  std::uintptr_t state_machine,
                                  std::string&){};
    bool (*set_state_machine_updating)(void*, std::uintptr_t state_machine,
                                       bool updating, std::string&){};
    bool (*update_destination_state)(void*, std::uintptr_t state_machine,
                                     std::string&){};
    // Level::Unload queries MenuBase::IsVisible after StopAllSounds, because
    // PushMenu and intervening callbacks can change the retained menu state.
    bool (*query_loading_menu_visible)(void*, bool& visible, std::string&){};
};

enum class GsLevelQueuedSwitchPhaseV1 : std::uint8_t {
    idle, quick_save, loading_menu_push, cleanup_74, cleanup_75,
    save_all_players, stop_sounds, loading_menu_visibility, loading_menu_pop, clear_level_state,
    network_uninit, player_manager_update, random_seed, close_hud,
    delete_old_level, clear_current_gslevel, construct_destination,
    set_update_guard, destination_update, clear_update_guard, complete
};

enum class GsLevelQueuedSwitchStatusV1 : std::uint8_t {
    complete, invalid_state, provider_unavailable, provider_failed
};

// Executes one already-queued StateMachine::SwitchState during the later
// StateMachine::Update. The adapter owns no queue, scene, Level or Save; it
// only sequences the existing old-level and destination-state providers.
class GsLevelQueuedStateHandoffV1 {
    GsLevelQueuedSwitchServicesV1 services_{};
    GsLevelQueuedSwitchPhaseV1 phase_{GsLevelQueuedSwitchPhaseV1::idle};
    bool active_{};
    bool consumed_{};
public:
    explicit GsLevelQueuedStateHandoffV1(GsLevelQueuedSwitchServicesV1);
    GsLevelQueuedStateHandoffV1(const GsLevelQueuedStateHandoffV1&) = delete;
    GsLevelQueuedStateHandoffV1& operator=(const GsLevelQueuedStateHandoffV1&) = delete;

    GsLevelQueuedSwitchStatusV1 dispatch(const GsLevelQueuedSwitchStateV1&,
        const LevelTransitionLoadV1&, std::uint32_t source_real_time,
        std::string& error);
    GsLevelQueuedSwitchPhaseV1 reached_phase() const noexcept { return phase_; }
};

} // namespace dh2::ui
