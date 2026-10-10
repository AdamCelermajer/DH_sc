#include "death_restart_v1.hpp"

namespace dh::foundation::persistence::death_restart {
namespace {
bool run(bool (*callback)(void*, std::string&), void* context,
         const char* failure, std::string& error) {
    if (!callback) {
        error = failure;
        return false;
    }
    if (!callback(context, error)) {
        if (error.empty()) error = failure;
        return false;
    }
    return true;
}
}

LocalDeathRouteV1 local_death_route_v1(bool gameplay_level_type_38,
                                       bool local_character_assigned,
                                       bool character_dead,
                                       bool online) {
    if (!gameplay_level_type_38 || !local_character_assigned || !character_dead)
        return LocalDeathRouteV1::none;
    return online ? LocalDeathRouteV1::start_online_death_timer
                  : LocalDeathRouteV1::show_offline_death_fade;
}

bool prepare_revive_v1(bool gameplay_level_type_38, bool online,
                       ReviveTicketV1& ticket, std::string& error) {
    ticket = {};
    if (!gameplay_level_type_38) {
        error = "Authored player revive requires active Level type 38";
        return false;
    }
    ticket.route = online ? ReviveRouteV1::online_handoff
                          : ReviveRouteV1::offline_checkpoint_revive;
    ticket.valid = true;
    error.clear();
    return true;
}

bool execute_revive_v1(ReviveTicketV1& ticket, const ReviveServicesV1& services,
                       std::string& error) {
    if (!ticket.valid || ticket.consumed) {
        error = "Authored player revive ticket is absent or already consumed";
        return false;
    }
    ticket.consumed = true;

    if (ticket.route == ReviveRouteV1::online_handoff) {
        const bool ok = run(services.notify_online_handoff, services.context,
                            "Online revive requires its host handoff provider", error);
        ticket.valid = false;
        return ok;
    }

    const bool ok =
        run(services.pop_fade_to_black, services.context,
            "Offline revive requires the source fade-to-black pop provider", error) &&
        run(services.push_fade_from_black, services.context,
            "Offline revive requires the source fade-from-black push provider", error) &&
        run(services.load_level_checkpoint, services.context,
            "Offline revive requires the current Level checkpoint provider", error) &&
        run(services.clear_character_buffs, services.context,
            "Offline revive requires the current Character buff owner", error) &&
        run(services.restore_revive_position, services.context,
            "Offline revive requires the source revive-position provider", error) &&
        run(services.revive_to_source_vitals, services.context,
            "Offline revive requires the source Character::Revive provider", error) &&
        run(services.set_character_idle, services.context,
            "Offline revive requires the source idle-state provider", error) &&
        run(services.ensure_minimum_potions, services.context,
            "Offline revive requires CharacterDesign.MinimumPotionsOnDeath", error) &&
        run(services.restart_level_music, services.context,
            "Offline revive requires the current Level music provider", error);
    ticket.valid = false;
    if (ok) error.clear();
    return ok;
}

} // namespace dh::foundation::persistence::death_restart
