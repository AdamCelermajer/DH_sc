#include "gslevel_queued_state_handoff_v1.hpp"

namespace dh2::ui {
namespace {
GsLevelQueuedSwitchStatusV1 missing(const char* name, std::string& error) {
    error = std::string("queued GSLevel handoff provider unavailable: ") + name;
    return GsLevelQueuedSwitchStatusV1::provider_unavailable;
}
GsLevelQueuedSwitchStatusV1 failed(const char* name, std::string& error) {
    if (error.empty()) error = std::string("queued GSLevel handoff provider failed: ") + name;
    return GsLevelQueuedSwitchStatusV1::provider_failed;
}
}

GsLevelQueuedStateHandoffV1::GsLevelQueuedStateHandoffV1(
    GsLevelQueuedSwitchServicesV1 services) : services_(services) {}

GsLevelQueuedSwitchStatusV1 GsLevelQueuedStateHandoffV1::dispatch(
    const GsLevelQueuedSwitchStateV1& state,
    const LevelTransitionLoadV1& request, std::uint32_t source_real_time,
    std::string& error) {
    error.clear();
    if (active_ || consumed_) {
        error = "queued GSLevel handoff already dispatched or in progress";
        return GsLevelQueuedSwitchStatusV1::invalid_state;
    }
    if (!state.state_machine || !state.old_gslevel || !state.old_level ||
        !state.old_level_savegame || !state.object_manager ||
        !state.player_manager || state.level_state != 0 ||
        state.level_unload_force > 1 || state.has_level_cleanup_74 > 1 ||
        state.has_level_cleanup_75 > 1) {
        error = "queued GSLevel handoff requires one reset source Level and its canonical owners";
        return GsLevelQueuedSwitchStatusV1::invalid_state;
    }

    // A queued _switchState is destructive and source code has no failure
    // return. Once started, this adapter cannot safely replay a partial handoff.
    active_ = true;
    consumed_ = true;
    struct ActiveReset { bool& active; ~ActiveReset() { active = false; } } reset{active_};
    const auto invoke = [&](GsLevelQueuedSwitchPhaseV1 phase, const char* name,
                            auto callback, auto&&... args) {
        phase_ = phase;
        if (!callback) return missing(name, error);
        if (!callback(services_.context, args..., error))
            return failed(name, error);
        return GsLevelQueuedSwitchStatusV1::complete;
    };
    auto status = GsLevelQueuedSwitchStatusV1::complete;
#define DH2_GS_HANDOFF_CALL(phase, name, callback, ...) \
    do { status = invoke(GsLevelQueuedSwitchPhaseV1::phase, name, \
                         services_.callback, __VA_ARGS__); \
         if (status != GsLevelQueuedSwitchStatusV1::complete) return status; } while (false)

    DH2_GS_HANDOFF_CALL(quick_save, "Level::QuickSave", quick_save,
                        state.old_level, state.old_level_savegame,
                        state.level_state, state.level_unload_force != 0);
    DH2_GS_HANDOFF_CALL(loading_menu_push, "loading-menu push", push_loading_menu,
                        state.old_level);
    if (state.has_level_cleanup_74) {
        DH2_GS_HANDOFF_CALL(cleanup_74, "Level cleanup field +0x128", destroy_level_cleanup_74,
                            state.old_level);
    }
    if (state.has_level_cleanup_75) {
        DH2_GS_HANDOFF_CALL(cleanup_75, "Level cleanup field +0x12c", destroy_level_cleanup_75,
                            state.old_level);
    }
    DH2_GS_HANDOFF_CALL(save_all_players, "PlayerManager::SG_SaveAllPlayer",
                        save_all_players, state.old_level, state.player_manager,
                        state.level_unload_force != 0);
    DH2_GS_HANDOFF_CALL(stop_sounds, "SoundManager::StopAll", stop_all_sounds,
                        std::uint32_t{500});
    bool loading_menu_visible = false;
    status = invoke(GsLevelQueuedSwitchPhaseV1::loading_menu_visibility,
                    "MenuBase::IsVisible", services_.query_loading_menu_visible,
                    loading_menu_visible);
    if (status != GsLevelQueuedSwitchStatusV1::complete) return status;
    if (loading_menu_visible) {
        status = invoke(GsLevelQueuedSwitchPhaseV1::loading_menu_pop, "loading-menu pop", services_.pop_loading_menu);
        if (status != GsLevelQueuedSwitchStatusV1::complete) return status;
    }
    DH2_GS_HANDOFF_CALL(clear_level_state, "Level loaded-state clear", clear_level_state,
                        state.old_level, std::int32_t{0});
    DH2_GS_HANDOFF_CALL(network_uninit, "ObjectManager::NetworkUnInitLevel",
                        network_uninit_level, state.object_manager);
    DH2_GS_HANDOFF_CALL(player_manager_update, "PlayerManager::Update",
                        update_player_manager, state.player_manager);
    DH2_GS_HANDOFF_CALL(random_seed, "Random seed reset", set_random_seeds,
                        source_real_time, std::uint32_t{0});
    status = invoke(GsLevelQueuedSwitchPhaseV1::close_hud, "GSLevel HUD close", services_.close_hud);
    if (status != GsLevelQueuedSwitchStatusV1::complete) return status;
    DH2_GS_HANDOFF_CALL(delete_old_level, "GSLevel old Level destruction",
                        delete_old_level, state.old_level);
    DH2_GS_HANDOFF_CALL(clear_current_gslevel, "GSLevel::s_level clear",
                        clear_current_gslevel, state.old_gslevel);
    DH2_GS_HANDOFF_CALL(construct_destination, "destination GSLevel construction",
                        construct_destination, request, state.state_machine);
    DH2_GS_HANDOFF_CALL(set_update_guard, "StateMachine updating guard on",
                        set_state_machine_updating, state.state_machine, true);
    status = invoke(GsLevelQueuedSwitchPhaseV1::destination_update,
                    "destination State::Update", services_.update_destination_state,
                    state.state_machine);
    if (status != GsLevelQueuedSwitchStatusV1::complete) {
        const std::string update_error = error;
        std::string clear_error;
        phase_ = GsLevelQueuedSwitchPhaseV1::clear_update_guard;
        if (services_.set_state_machine_updating) {
            try {
                services_.set_state_machine_updating(services_.context,
                    state.state_machine, false, clear_error);
            } catch (...) {}
        }
        if (!clear_error.empty()) error += "; update-guard cleanup: " + clear_error;
        if (error.empty()) error = update_error;
        return status;
    }
    DH2_GS_HANDOFF_CALL(clear_update_guard, "StateMachine updating guard off",
                        set_state_machine_updating, state.state_machine, false);
#undef DH2_GS_HANDOFF_CALL

    phase_ = GsLevelQueuedSwitchPhaseV1::complete;
    return GsLevelQueuedSwitchStatusV1::complete;
}

} // namespace dh2::ui
