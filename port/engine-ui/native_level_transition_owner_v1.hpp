#pragma once

#include "application_level_load_handoff_v1.hpp"
#include "gslevel_queued_state_handoff_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::ui {

// Stable references needed between Application::LoadLevel and the later
// queued StateMachine::Update. These are identities of existing owners, not
// copied Level/Save/ObjectManager state.
struct NativeLevelTransitionReferencesV1 {
    std::uintptr_t gslevel{};
    std::uintptr_t object_manager{};
    std::uintptr_t player_manager{};
};

struct NativeLevelTransitionServicesV1 {
    ApplicationLevelLoadServicesV1 application{};
    GsLevelQueuedSwitchServicesV1 queued_switch{};
};

enum class NativeLevelTransitionStatusV1 : std::uint8_t {
    complete, skipped_not_loaded, invalid_state, application_failed,
    queued_switch_failed
};

// Owns one semantic transition across the two source call boundaries. The UI
// launch request remains the only request source; this class retains one
// in-flight request, current Level/Save references, and a native state-machine
// identity token. It does not reproduce the original StateMachine object.
class NativeLevelTransitionOwnerV1 {
    NativeLevelTransitionServicesV1 services_{};
    ApplicationLevelLoadHandoffV1 application_;
    GsLevelQueuedStateHandoffV1 queued_switch_;
    ApplicationLevelLoadStateV1 application_state_{};
    NativeLevelTransitionReferencesV1 references_{};
    LevelTransitionLoadV1 requested_{};
    std::uint8_t native_state_machine_marker_{1};
    std::uintptr_t native_state_machine_token_{};
    bool application_attempted_{};
    bool application_ready_{};
    bool queued_switch_attempted_{};
    static ApplicationLevelLoadServicesV1 bind_application(
        NativeLevelTransitionOwnerV1*) noexcept;
    static bool show_loading(void*,std::uintptr_t,std::string&);
    static bool set_level_flag_145(void*,std::uintptr_t,std::uint8_t,std::string&);
    static bool send_high_score(void*,std::uintptr_t,std::string&);
    static bool set_level_flag_f0(void*,std::uintptr_t,std::uint8_t,std::string&);
    static bool save_local_player(void*,std::uintptr_t,std::uintptr_t,bool,std::string&);
    static bool quick_save(void*,std::uintptr_t,std::uintptr_t,bool,std::string&);
    static bool reset_is_loaded(void*,std::uintptr_t,std::string&);
    static bool continue_application_load(void*,const LevelTransitionLoadV1&,std::string&);
public:
    explicit NativeLevelTransitionOwnerV1(NativeLevelTransitionServicesV1);
    NativeLevelTransitionOwnerV1(const NativeLevelTransitionOwnerV1&)=delete;
    NativeLevelTransitionOwnerV1& operator=(const NativeLevelTransitionOwnerV1&)=delete;

    // Called after the existing UI launch request and DisplayFastTravel
    // callback unwind. Copies the one request and borrowed owner identities,
    // then runs the source Application::LoadLevel branch synchronously.
    NativeLevelTransitionStatusV1 begin_application_load(
        const ApplicationLevelLoadStateV1&,
        const NativeLevelTransitionReferencesV1&,
        const LevelTransitionLoadV1&,std::string& error);

    // Called at the next frame's queued GSLevel/StateMachine boundary. The
    // caller supplies the post-ResetIsLoaded source snapshot; this owner
    // validates it against the retained Level/Save/manager identities and
    // supplies its own native state-machine token.
    NativeLevelTransitionStatusV1 dispatch_queued_switch(
        const GsLevelQueuedSwitchStateV1&,std::uint32_t source_real_time,
        std::string& error);

    bool application_ready() const noexcept { return application_ready_; }
    bool queued_switch_consumed() const noexcept { return queued_switch_attempted_; }
    std::uintptr_t native_state_machine_identity() const noexcept {
        return native_state_machine_token_;
    }
    ApplicationLevelLoadPhaseV1 application_phase() const noexcept {
        return application_.reached_phase();
    }
    GsLevelQueuedSwitchPhaseV1 queued_switch_phase() const noexcept {
        return queued_switch_.reached_phase();
    }
};

} // namespace dh2::ui
