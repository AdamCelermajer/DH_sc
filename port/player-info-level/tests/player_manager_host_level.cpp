#include "player_manager_host_level.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>

using namespace dh2::player_manager_host_level;

namespace {

struct OnlineContext {
    std::vector<std::uint8_t> online{0};
    std::vector<std::uint8_t> state{0};
    std::vector<std::int32_t> host{0};
    std::vector<std::uintptr_t> managers{0x9000};
    std::vector<std::int32_t> initialized{0};
    std::int32_t host_id = 0;
    PlayerInfoProjection* network_player = nullptr;
    std::string fail;
    std::vector<std::string> calls;
    std::size_t online_i = 0, state_i = 0, host_i = 0, manager_i = 0, init_i = 0;
    std::uintptr_t net_lookup_manager = 0;
    std::int32_t net_lookup_id = 0;
    std::uint32_t net_lookup_flag = 99;
};

template <typename T>
T take(const std::vector<T>& values, std::size_t& index) {
    const auto i = index++;
    return values[i < values.size() ? i : values.size() - 1];
}

std::int32_t online(void* opaque, std::uint8_t* value) {
    auto& c = *static_cast<OnlineContext*>(opaque);
    c.calls.emplace_back("online");
    if (c.fail == "online") return 1;
    *value = take(c.online, c.online_i);
    return 0;
}
std::int32_t game_state(void* opaque, std::uint8_t* value) {
    auto& c = *static_cast<OnlineContext*>(opaque);
    c.calls.emplace_back("state");
    if (c.fail == "state") return 1;
    *value = take(c.state, c.state_i);
    return 0;
}
std::int32_t matching_host(void* opaque, std::int32_t* value) {
    auto& c = *static_cast<OnlineContext*>(opaque);
    c.calls.emplace_back("host");
    if (c.fail == "host") return 1;
    *value = take(c.host, c.host_i);
    return 0;
}
std::int32_t net_manager(void* opaque, std::uintptr_t* value) {
    auto& c = *static_cast<OnlineContext*>(opaque);
    c.calls.emplace_back("net_manager");
    if (c.fail == "net_manager") return 1;
    *value = take(c.managers, c.manager_i);
    return 0;
}
std::int32_t net_initialized(void* opaque, std::uintptr_t manager,
                             std::int32_t* value) {
    auto& c = *static_cast<OnlineContext*>(opaque);
    c.calls.emplace_back("initialized:" + std::to_string(manager));
    if (c.fail == "initialized") return 1;
    *value = take(c.initialized, c.init_i);
    return 0;
}
std::int32_t host_internal_id(void* opaque, std::uintptr_t manager,
                              std::int32_t* value) {
    auto& c = *static_cast<OnlineContext*>(opaque);
    c.calls.emplace_back("host_id:" + std::to_string(manager));
    if (c.fail == "host_id") return 1;
    *value = c.host_id;
    return 0;
}
std::int32_t network_player(void* opaque, std::uintptr_t manager,
                            std::int32_t id, std::uint32_t flag,
                            PlayerInfoProjection** value) {
    auto& c = *static_cast<OnlineContext*>(opaque);
    c.calls.emplace_back("net_player");
    if (c.fail == "net_player") return 1;
    c.net_lookup_manager = manager;
    c.net_lookup_id = id;
    c.net_lookup_flag = flag;
    *value = c.network_player;
    return 0;
}

Services online_services(OnlineContext& c) {
    return {&c, online, game_state, matching_host, net_manager,
            net_initialized, host_internal_id, network_player};
}

struct ReconcileContext {
    PlayerInfoProjection* player = nullptr;
    std::vector<std::int32_t> values;
    std::size_t next = 0;
    std::vector<std::string> calls;
    std::string fail;
    std::int32_t setter_value = 0;
    bool setter_mutates_before_failure = false;
};
std::int32_t get_property(void* opaque, std::uintptr_t props,
                          std::uint32_t property, std::uint32_t bonus,
                          std::int32_t* value) {
    auto& c = *static_cast<ReconcileContext*>(opaque);
    c.calls.emplace_back("get:" + std::to_string(props) + ":" +
                         std::to_string(property) + ":" + std::to_string(bonus));
    if (c.fail == "get" || (c.fail == "second_get" && c.next == 1)) return 1;
    *value = take(c.values, c.next);
    return 0;
}
std::int32_t set_level(void* opaque, PlayerInfoProjection* player,
                       std::int32_t value) {
    auto& c = *static_cast<ReconcileContext*>(opaque);
    c.calls.emplace_back("set:" + std::to_string(value));
    c.setter_value = value;
    if (c.fail == "setter" && !c.setter_mutates_before_failure) return 1;
    player->character_level_member->value = value;
    return c.fail == "setter" ? 1 : 0;
}

bool check(bool condition, const char* label) {
    if (!condition) std::fprintf(stderr, "FAIL: %s\n", label);
    return condition;
}

bool host_selection_cases() {
    bool ok = true;
    dh2::character_level_member::IntMember l0{}, l1{}, lf{}, ln{};
    l0.value = 4; l1.value = 7; lf.value = -3; ln.value = 99;
    PlayerInfoProjection p0{0x1100, 0, &l0};
    PlayerInfoProjection p1{0x1200, 7, &l1};
    PlayerInfoProjection fallback{0x1300, -99, &lf};
    PlayerInfoProjection net{0x1400, 7, &ln};
    PlayerInfoProjection* entries[] = {&p0, &p1};
    PlayerRegistry registry{0x1000, entries, 2, &fallback};

    // Offline source path: GetHostingPlayer uses ID zero; nested lookup repeats
    // the online byte and selects the exact local tree entry.
    {
        OnlineContext c;
        auto services = online_services(c);
        Result result{};
        ok &= check(get_hosting_level(&registry, &services, &result) == Status::complete,
                    "offline selection status");
        ok &= check(result.route == Route::local_id_tree &&
                    result.requested_internal_id == 0 &&
                    result.player_identity == p0.identity &&
                    result.character_level_330 == 4, "offline tree result");
        ok &= check(c.calls == std::vector<std::string>{"online", "online"},
                    "offline fresh double online query");
    }
    // Local tree miss is source manager+8, and lookup of -1 bypasses every
    // nested online/network query after the outer host-ID read.
    {
        OnlineContext c;
        c.online = {1, 1}; c.state = {1}; c.host = {1};
        c.managers = {0x9100, 0x9200}; c.initialized = {1}; c.host_id = -1;
        auto services = online_services(c);
        Result result{};
        ok &= check(get_hosting_level(&registry, &services, &result) == Status::complete,
                    "negative host ID status");
        ok &= check(result.route == Route::manager_plus_8_fallback &&
                    result.player_identity == fallback.identity &&
                    result.character_level_330 == -3, "negative host ID fallback");
        ok &= check(c.calls == std::vector<std::string>{"online", "state", "host",
                    "net_manager", "initialized:37120", "net_manager", "host_id:37376"},
                    "negative host ID no nested branch");
    }
    // Network route re-reads singleton for GetNetPlayerInfo and then performs
    // a second complete online/host predicate inside GetPlayerByInternalID.
    {
        OnlineContext c;
        c.online = {1, 1}; c.state = {1, 1}; c.host = {1, 1};
        c.managers = {0x9100, 0x9200, 0x9300};
        c.initialized = {1, 1}; c.host_id = 7; c.network_player = &net;
        auto services = online_services(c);
        Result result{};
        ok &= check(get_hosting_level(&registry, &services, &result) == Status::complete,
                    "network selection status");
        ok &= check(result.route == Route::net_player_info &&
                    result.player_identity == net.identity &&
                    result.character_level_330 == 99, "network selection result");
        ok &= check(c.net_lookup_manager == 0x9300 && c.net_lookup_id == 7 &&
                    c.net_lookup_flag == 0, "fresh network manager and exact args");
        ok &= check(c.calls == std::vector<std::string>{"online", "state", "host",
                    "net_manager", "initialized:37120", "net_manager", "host_id:37376",
                    "online", "state", "host", "net_manager", "initialized:37632",
                    "net_player"}, "network exact query order");
    }
    // A transition to offline between the outer and inner helper uses the ID
    // chosen by the outer call but resolves it from the current local tree.
    {
        OnlineContext c;
        c.online = {1, 0}; c.state = {1}; c.host = {1};
        c.managers = {0x9100, 0x9200}; c.initialized = {1}; c.host_id = 7;
        auto services = online_services(c);
        Result result{};
        ok &= check(get_hosting_level(&registry, &services, &result) == Status::complete,
                    "fresh transition status");
        ok &= check(result.route == Route::local_id_tree &&
                    result.player_identity == p1.identity &&
                    result.requested_internal_id == 7, "fresh transition resolution");
        ok &= check(c.calls.back() == "online", "transition stops after inner online");
    }
    // A net response which lacks a PlayerInfo projection is a port boundary;
    // it must not silently fall back to a different local identity.
    {
        OnlineContext c;
        c.online = {1, 1}; c.state = {1, 1}; c.host = {1, 1};
        c.managers = {0x9100, 0x9200, 0x9300}; c.initialized = {1, 1};
        c.host_id = 7; c.network_player = nullptr;
        auto services = online_services(c);
        Result result{};
        ok &= check(get_hosting_level(&registry, &services, &result) ==
                    Status::no_player_projection, "null net projection status");
        ok &= check(result.player_identity == 0 && result.route == Route::none,
                    "null net projection does not substitute local identity");
    }
    // The manager tree fallback is preserved for a normal ID miss.
    {
        OnlineContext c;
        c.online = {1, 1}; c.state = {1, 0}; c.host = {1};
        c.managers = {0x9100, 0x9200}; c.initialized = {1}; c.host_id = 90;
        auto services = online_services(c);
        Result result{};
        ok &= check(get_hosting_level(&registry, &services, &result) == Status::complete,
                    "local miss status");
        ok &= check(result.route == Route::manager_plus_8_fallback &&
                    result.player_identity == fallback.identity,
                    "local miss manager+8 result");
    }
    // Known output/source alias is rejected before the source projection can
    // be overwritten by the larger result record.
    {
        OnlineContext c;
        auto services = online_services(c);
        const auto before = registry;
        auto* aliased_output = reinterpret_cast<Result*>(&registry);
        ok &= check(get_hosting_level(&registry, &services, aliased_output) ==
                    Status::invalid_argument, "host result alias status");
        ok &= check(std::memcmp(&registry, &before, sizeof(registry)) == 0,
                    "host result alias preserves registry");
    }
    return ok;
}

bool reconciliation_cases() {
    bool ok = true;
    dh2::character_level_member::IntMember member{};
    member.value = 5;
    PlayerInfoProjection player{0x2100, 0, &member};
    ReconcileState state{&player, 0x2200, 0x2300};
    // Equal value: exactly one GetInt and no setter.
    {
        ReconcileContext c; c.player = &player; c.values = {5};
        ReconcileServices services{&c, get_property, set_level};
        ReconcileResult result{};
        ok &= check(reconcile_character_level(&state, &services, &result) ==
                    ReconcileStatus::complete, "equal reconcile status");
        ok &= check(result.property_reads == 1 && result.setter_calls == 0 &&
                    result.level_before == 5 && result.level_after == 5,
                    "equal reconcile one read");
        ok &= check(c.calls == std::vector<std::string>{"get:8960:19:0"},
                    "property ID and no-bonus args");
    }
    // Mismatch: source repeats GetInt and uses the second value for the setter.
    {
        member.value = 5;
        ReconcileContext c; c.player = &player; c.values = {7, 9};
        ReconcileServices services{&c, get_property, set_level};
        ReconcileResult result{};
        ok &= check(reconcile_character_level(&state, &services, &result) ==
                    ReconcileStatus::complete, "mismatch reconcile status");
        ok &= check(result.property_reads == 2 && result.setter_calls == 1 &&
                    result.first_property_value == 7 && result.setter_argument == 9 &&
                    result.level_after == 9 && member.value == 9,
                    "mismatch fresh second value");
    }
    // A failed first read has no setter effects.
    {
        member.value = 5;
        ReconcileContext c; c.player = &player; c.values = {8}; c.fail = "get";
        ReconcileServices services{&c, get_property, set_level};
        ReconcileResult result{};
        ok &= check(reconcile_character_level(&state, &services, &result) ==
                    ReconcileStatus::service_failed, "first read failure status");
        ok &= check(result.property_reads == 1 && result.setter_calls == 0 &&
                    member.value == 5, "first read failure effects");
    }
    // A failing second read does not fall back to the comparison value.
    {
        member.value = 5;
        ReconcileContext c; c.player = &player; c.values = {8}; c.fail = "second_get";
        ReconcileServices services{&c, get_property, set_level};
        ReconcileResult result{};
        ok &= check(reconcile_character_level(&state, &services, &result) ==
                    ReconcileStatus::service_failed, "second read failure status");
        ok &= check(result.property_reads == 2 && result.setter_calls == 0 &&
                    member.value == 5, "second read failure effects");
    }
    // Setter adapter can report a post-mutation failure; completed member
    // mutation remains observable and is not rolled back.
    {
        member.value = 5;
        ReconcileContext c; c.player = &player; c.values = {8, 10};
        c.fail = "setter"; c.setter_mutates_before_failure = true;
        ReconcileServices services{&c, get_property, set_level};
        ReconcileResult result{};
        ok &= check(reconcile_character_level(&state, &services, &result) ==
                    ReconcileStatus::service_failed, "setter failure status");
        ok &= check(result.setter_calls == 1 && result.setter_argument == 10 &&
                    result.level_after == 10 && member.value == 10,
                    "setter partial effects retained");
    }
    // No associated Character means the caller is outside the source sync
    // branch; do not ask the property service.
    {
        member.value = 11;
        ReconcileState unbound{&player, 0, 0};
        ReconcileContext c; c.player = &player;
        ReconcileServices services{&c, get_property, set_level};
        ReconcileResult result{};
        ok &= check(reconcile_character_level(&unbound, &services, &result) ==
                    ReconcileStatus::skipped_unbound_character,
                    "unbound reconcile status");
        ok &= check(result.property_reads == 0 && c.calls.empty() &&
                    result.level_before == 11, "unbound has no property read");
    }
    // Output may not alias the mutable +0x330 source member.
    {
        member.value = 12;
        ReconcileContext c; c.player = &player;
        ReconcileServices services{&c, get_property, set_level};
        const auto before = member;
        auto* aliased_output = reinterpret_cast<ReconcileResult*>(&member);
        ok &= check(reconcile_character_level(&state, &services, aliased_output) ==
                    ReconcileStatus::invalid_argument, "reconcile result alias status");
        ok &= check(std::memcmp(&member, &before, sizeof(member)) == 0 && c.calls.empty(),
                    "reconcile result alias has no effects");
    }
    return ok;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc == 5 && std::string(argv[1]) == "--reconcile") {
        dh2::character_level_member::IntMember member{};
        member.value = static_cast<std::int32_t>(std::strtol(argv[2], nullptr, 0));
        PlayerInfoProjection player{0x2100, 0, &member};
        ReconcileState state{&player, 0x2200, 0x2300};
        ReconcileContext c;
        c.player = &player;
        c.values = {static_cast<std::int32_t>(std::strtol(argv[3], nullptr, 0)),
                    static_cast<std::int32_t>(std::strtol(argv[4], nullptr, 0))};
        ReconcileServices services{&c, get_property, set_level};
        ReconcileResult result{};
        const auto status = reconcile_character_level(&state, &services, &result);
        std::printf("{\"status\":%u,\"property_reads\":%u,\"setter_calls\":%u,"
                    "\"level_before\":%d,\"first_property\":%d,"
                    "\"setter_argument\":%d,\"level_after\":%d}\n",
                    static_cast<unsigned>(status), result.property_reads,
                    result.setter_calls, result.level_before,
                    result.first_property_value, result.setter_argument,
                    result.level_after);
        return 0;
    }
    const bool ok = host_selection_cases() & reconciliation_cases();
    if (!ok) return 1;
    std::puts("{\"validation\":\"PASS\",\"host_cases\":12}");
    return 0;
}
