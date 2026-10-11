#include "native_level_transition_owner_v1.hpp"

namespace dh2::ui {
namespace {
NativeLevelTransitionStatusV1 invalid(const char* message, std::string& error) {
    error = message;
    return NativeLevelTransitionStatusV1::invalid_state;
}
}

ApplicationLevelLoadServicesV1 NativeLevelTransitionOwnerV1::bind_application(
    NativeLevelTransitionOwnerV1* owner) noexcept {
    return {owner,
        &NativeLevelTransitionOwnerV1::show_loading,
        &NativeLevelTransitionOwnerV1::set_level_flag_145,
        &NativeLevelTransitionOwnerV1::send_high_score,
        &NativeLevelTransitionOwnerV1::set_level_flag_f0,
        &NativeLevelTransitionOwnerV1::save_local_player,
        &NativeLevelTransitionOwnerV1::quick_save,
        &NativeLevelTransitionOwnerV1::reset_is_loaded,
        &NativeLevelTransitionOwnerV1::continue_application_load};
}

NativeLevelTransitionOwnerV1::NativeLevelTransitionOwnerV1(
    NativeLevelTransitionServicesV1 services)
    : services_(services), application_(bind_application(this)),
      queued_switch_(services.queued_switch),
      native_state_machine_token_(reinterpret_cast<std::uintptr_t>(
          &native_state_machine_marker_)) {}

bool NativeLevelTransitionOwnerV1::show_loading(
    void* context, std::uintptr_t level, std::string& error) {
    auto& self = *static_cast<NativeLevelTransitionOwnerV1*>(context);
    const auto callback = self.services_.application.show_loading;
    return callback && callback(self.services_.application.context, level, error);
}

bool NativeLevelTransitionOwnerV1::set_level_flag_145(
    void* context, std::uintptr_t level, std::uint8_t value, std::string& error) {
    auto& self = *static_cast<NativeLevelTransitionOwnerV1*>(context);
    const auto callback = self.services_.application.set_level_flag_145;
    return callback && callback(self.services_.application.context, level, value, error);
}

bool NativeLevelTransitionOwnerV1::send_high_score(
    void* context, std::uintptr_t level, std::string& error) {
    auto& self = *static_cast<NativeLevelTransitionOwnerV1*>(context);
    const auto callback = self.services_.application.send_high_score;
    return callback && callback(self.services_.application.context, level, error);
}

bool NativeLevelTransitionOwnerV1::set_level_flag_f0(
    void* context, std::uintptr_t level, std::uint8_t value, std::string& error) {
    auto& self = *static_cast<NativeLevelTransitionOwnerV1*>(context);
    const auto callback = self.services_.application.set_level_flag_f0;
    return callback && callback(self.services_.application.context, level, value, error);
}

bool NativeLevelTransitionOwnerV1::save_local_player(
    void* context, std::uintptr_t level, std::uintptr_t savegame,
    bool force, std::string& error) {
    auto& self = *static_cast<NativeLevelTransitionOwnerV1*>(context);
    const auto callback = self.services_.application.save_local_player;
    return callback && callback(self.services_.application.context, level,
                                savegame, force, error);
}

bool NativeLevelTransitionOwnerV1::quick_save(
    void* context, std::uintptr_t level, std::uintptr_t savegame,
    bool force, std::string& error) {
    auto& self = *static_cast<NativeLevelTransitionOwnerV1*>(context);
    const auto callback = self.services_.application.quick_save;
    return callback && callback(self.services_.application.context, level,
                                savegame, force, error);
}

bool NativeLevelTransitionOwnerV1::reset_is_loaded(
    void* context, std::uintptr_t level, std::string& error) {
    auto& self = *static_cast<NativeLevelTransitionOwnerV1*>(context);
    const auto callback = self.services_.application.reset_is_loaded;
    return callback && callback(self.services_.application.context, level, error);
}

bool NativeLevelTransitionOwnerV1::continue_application_load(
    void* context, const LevelTransitionLoadV1& request, std::string& error) {
    auto& self = *static_cast<NativeLevelTransitionOwnerV1*>(context);
    const auto callback = self.services_.application.continue_application_load;
    return callback && callback(self.services_.application.context, request, error);
}

NativeLevelTransitionStatusV1 NativeLevelTransitionOwnerV1::begin_application_load(
    const ApplicationLevelLoadStateV1& state,
    const NativeLevelTransitionReferencesV1& references,
    const LevelTransitionLoadV1& request, std::string& error) {
    error.clear();
    if (application_attempted_ || application_ready_ || queued_switch_attempted_)
        return invalid("native level transition already started", error);
    if (!state.level || !state.player_savegame || !state.level_savegame ||
        !references.gslevel || !references.object_manager ||
        !references.player_manager || state.level_state != 38 ||
        state.loading_callback_required > 1 || request.file.empty())
        return invalid("native level transition requires a loaded Level, canonical owners, and destination", error);

    // Commit the one in-flight source request before invoking callbacks: any
    // failed provider may already have performed an irreversible source side effect.
    application_attempted_ = true;
    application_state_ = state;
    references_ = references;
    requested_ = request;
    const auto status = application_.run(application_state_, requested_, error);
    switch (status) {
    case ApplicationLevelLoadStatusV1::complete:
        application_ready_ = true;
        return NativeLevelTransitionStatusV1::complete;
    case ApplicationLevelLoadStatusV1::skipped_not_loaded:
        return NativeLevelTransitionStatusV1::skipped_not_loaded;
    case ApplicationLevelLoadStatusV1::invalid_state:
        return NativeLevelTransitionStatusV1::invalid_state;
    case ApplicationLevelLoadStatusV1::provider_unavailable:
    case ApplicationLevelLoadStatusV1::provider_failed:
        return NativeLevelTransitionStatusV1::application_failed;
    }
    return NativeLevelTransitionStatusV1::application_failed;
}

NativeLevelTransitionStatusV1 NativeLevelTransitionOwnerV1::dispatch_queued_switch(
    const GsLevelQueuedSwitchStateV1& source_snapshot,
    std::uint32_t source_real_time, std::string& error) {
    error.clear();
    if (!application_ready_ || queued_switch_attempted_)
        return invalid("queued GSLevel switch has no ready transition or was already consumed", error);
    if (source_snapshot.state_machine != 0 ||
        source_snapshot.old_gslevel != references_.gslevel ||
        source_snapshot.old_level != application_state_.level ||
        source_snapshot.old_level_savegame != application_state_.level_savegame ||
        source_snapshot.object_manager != references_.object_manager ||
        source_snapshot.player_manager != references_.player_manager ||
        source_snapshot.level_state != 0 || source_snapshot.level_unload_force != 1 ||
        source_snapshot.has_level_cleanup_74 > 1 ||
        source_snapshot.has_level_cleanup_75 > 1) {
        return invalid("queued GSLevel snapshot does not match the retained reset Level and canonical owners", error);
    }

    auto dispatch_state = source_snapshot;
    // This is the identity of this reconstructed native owner. There is no
    // retained source StateMachine pointer in the current renderer lifecycle.
    dispatch_state.state_machine = native_state_machine_token_;
    const auto status = queued_switch_.dispatch(
        dispatch_state, requested_, source_real_time, error);
    if (status == GsLevelQueuedSwitchStatusV1::complete)
        queued_switch_attempted_ = true;
    else if (queued_switch_.reached_phase() != GsLevelQueuedSwitchPhaseV1::idle)
        queued_switch_attempted_ = true;
    switch (status) {
    case GsLevelQueuedSwitchStatusV1::complete:
        return NativeLevelTransitionStatusV1::complete;
    case GsLevelQueuedSwitchStatusV1::invalid_state:
        return NativeLevelTransitionStatusV1::invalid_state;
    case GsLevelQueuedSwitchStatusV1::provider_unavailable:
    case GsLevelQueuedSwitchStatusV1::provider_failed:
        return NativeLevelTransitionStatusV1::queued_switch_failed;
    }
    return NativeLevelTransitionStatusV1::queued_switch_failed;
}

} // namespace dh2::ui
