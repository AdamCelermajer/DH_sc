#include "death_flow_v1.hpp"

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

bool begin_death_flow_v1(const DeathAdmissionV1& admission,
                         DeathFlowTicketV1& ticket, std::string& error) {
    ticket = {};
    if (!admission.player_manager_phase_active) {
        error = "Player death flow requires active PlayerManager phase";
        return false;
    }
    ticket.route = death_restart::local_death_route_v1(
        admission.gameplay_level_type_38,
        admission.local_character_assigned,
        admission.character_dead, admission.online);
    if (ticket.route == LocalDeathRouteV1::none) {
        error = "Source local-death Level/Character gate rejected the flow";
        return false;
    }
    ticket.valid = true;
    error.clear();
    return true;
}

bool activate_continue_v1(DeathFlowTicketV1& ticket,
                          const ContinueServicesV1& services,
                          std::string& error) {
    if (!ticket.valid || ticket.route != LocalDeathRouteV1::show_offline_death_fade ||
        ticket.continue_consumed) {
        error = "Continue requires a live offline death-flow ticket and is single-use";
        return false;
    }
    ticket.continue_consumed = true;
    return call(services.hide_continue_control, services.context,
                "Source Continue control requires its Hide(1) provider", error) &&
           call(services.play_parent_timeline, services.context,
                "Source Continue control requires its parent timeline provider", error);
}

bool run_authored_revive_v1(
    DeathFlowTicketV1& flow, bool gameplay_level_type_38,
    const ReviveServicesV1& revive_services,
    bool (*restart_previous_music)(void*, std::string&), void* music_context,
    std::string& error) {
    if (!flow.valid || flow.route != LocalDeathRouteV1::show_offline_death_fade ||
        !flow.continue_consumed || flow.revive_callback_consumed) {
        error = "Authored revive requires one Continue event and is single-use";
        return false;
    }
    flow.revive_callback_consumed = true;

    death_restart::ReviveTicketV1 revive;
    if (!death_restart::prepare_revive_v1(gameplay_level_type_38, false,
                                           revive, error))
        return false;
    if (!death_restart::execute_revive_v1(revive, revive_services, error))
        return false;
    if (!call(restart_previous_music, music_context,
              "Frame-88 NativePlayPreviousMusic requires its source provider", error))
        return false;
    error.clear();
    return true;
}

} // namespace dh::foundation::persistence::death_flow