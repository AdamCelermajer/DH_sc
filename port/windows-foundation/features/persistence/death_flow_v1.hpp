#pragma once

#include "../persistence/death_restart_v1.hpp"

#include <string>

namespace dh::foundation::persistence::death_flow {

using death_restart::LocalDeathRouteV1;
using death_restart::ReviveServicesV1;

struct DeathAdmissionV1 {
    bool gameplay_level_type_38 = false;
    bool local_character_assigned = false;
    bool character_dead = false;
    bool player_manager_phase_active = false;
    bool online = false;
};

struct DeathFlowTicketV1 {
    LocalDeathRouteV1 route = LocalDeathRouteV1::none;
    bool valid = false;
    bool continue_consumed = false;
    bool revive_callback_consumed = false;
};

// Mirrors PlayerManager::_CheckLocalDeaths and records which source route is
// active. Phase +433 is supplied by the live PlayerManager owner.
bool begin_death_flow_v1(const DeathAdmissionV1&, DeathFlowTicketV1&,
                         std::string& error);

struct ContinueServicesV1 {
    bool (*hide_continue_control)(void*, std::string&) = nullptr;
    bool (*play_parent_timeline)(void*, std::string&) = nullptr;
    void* context = nullptr;
};

// Mirrors dqhud_droid root/sprite742's btn_Continue release: Hide(1), then
// parent gotoAndPlay. This event deliberately does not revive the Character.
bool activate_continue_v1(DeathFlowTicketV1&, const ContinueServicesV1&,
                          std::string& error);

// Mirrors the later frame-88 NativeReviveAllPlayers callback, followed by the
// separately authored NativePlayPreviousMusic action. Checkpoint/Character
// effects stay with the existing death_restart providers.
bool run_authored_revive_v1(DeathFlowTicketV1&, bool gameplay_level_type_38,
                            const ReviveServicesV1&,
                            bool (*restart_previous_music)(void*, std::string&),
                            void* music_context, std::string& error);

} // namespace dh::foundation::persistence::death_flow