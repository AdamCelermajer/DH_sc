#include "death_restart_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace dh::foundation::persistence::death_restart;

namespace {
struct Trace {
    std::vector<std::string> calls;
    std::string fail_at;
};

bool record(Trace& trace, const char* name, std::string& error) {
    trace.calls.emplace_back(name);
    if (trace.fail_at == name) {
        error = std::string("provider failed at ") + name;
        return false;
    }
    return true;
}
#define CALLBACK(name) \
    bool name(void* context, std::string& error) { \
        return record(*static_cast<Trace*>(context), #name, error); \
    }
CALLBACK(pop_fade_to_black)
CALLBACK(push_fade_from_black)
CALLBACK(load_level_checkpoint)
CALLBACK(clear_character_buffs)
CALLBACK(restore_revive_position)
CALLBACK(revive_to_source_vitals)
CALLBACK(set_character_idle)
CALLBACK(ensure_minimum_potions)
CALLBACK(restart_level_music)
CALLBACK(notify_online_handoff)
#undef CALLBACK

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

ReviveServicesV1 services(Trace& trace) {
    return {pop_fade_to_black, push_fade_from_black, load_level_checkpoint,
            clear_character_buffs, restore_revive_position,
            revive_to_source_vitals, set_character_idle,
            ensure_minimum_potions, restart_level_music,
            notify_online_handoff, &trace};
}
}

int main() {
    require(local_death_route_v1(false, true, true, false) == LocalDeathRouteV1::none,
            "non-gameplay Level rejects local death routing");
    require(local_death_route_v1(true, false, true, false) == LocalDeathRouteV1::none,
            "unassigned Character rejects local death routing");
    require(local_death_route_v1(true, true, false, false) == LocalDeathRouteV1::none,
            "living Character does not enter death flow");
    require(local_death_route_v1(true, true, true, false) ==
                LocalDeathRouteV1::show_offline_death_fade,
            "offline death starts authored fade screen");
    require(local_death_route_v1(true, true, true, true) ==
                LocalDeathRouteV1::start_online_death_timer,
            "online death stays on source timer/handoff path");

    ReviveTicketV1 invalid;
    std::string error;
    require(!prepare_revive_v1(false, false, invalid, error) && !invalid.valid,
            "inactive Level rejects revive action");

    ReviveTicketV1 offline;
    require(prepare_revive_v1(true, false, offline, error) &&
                offline.route == ReviveRouteV1::offline_checkpoint_revive,
            "active offline Level selects authored checkpoint revive");
    Trace trace;
    require(execute_revive_v1(offline, services(trace), error),
            "normal offline revive providers complete");
    const std::vector<std::string> expected = {
        "pop_fade_to_black", "push_fade_from_black", "load_level_checkpoint",
        "clear_character_buffs", "restore_revive_position",
        "revive_to_source_vitals", "set_character_idle",
        "ensure_minimum_potions", "restart_level_music"};
    require(trace.calls == expected, "offline restart side effects follow source order");
    require(!execute_revive_v1(offline, services(trace), error) &&
                trace.calls == expected,
            "duplicate source timeline callback cannot replay restart side effects");

    ReviveTicketV1 online;
    require(prepare_revive_v1(true, true, online, error) &&
                online.route == ReviveRouteV1::online_handoff,
            "online native action chooses its multiplayer handoff");
    Trace online_trace;
    require(execute_revive_v1(online, services(online_trace), error) &&
                online_trace.calls == std::vector<std::string>{"notify_online_handoff"},
            "online path does not run offline checkpoint or full-health revive");

    ReviveTicketV1 failed;
    require(prepare_revive_v1(true, false, failed, error), "prepare failure case");
    Trace failure_trace;
    failure_trace.fail_at = "revive_to_source_vitals";
    require(!execute_revive_v1(failed, services(failure_trace), error) &&
                failure_trace.calls.size() == 6 &&
                failure_trace.calls.back() == "revive_to_source_vitals",
            "provider failure stops later idle/potion/music effects");
    require(!execute_revive_v1(failed, services(failure_trace), error) &&
                failure_trace.calls.size() == 6,
            "partially failed ticket cannot replay checkpoint and revive prefix");

    std::cout << "PASS: source local-death gates, offline checkpoint revive order, online handoff, invalid-level rejection, provider failure, and duplicate suppression\n";
    return 0;
}
