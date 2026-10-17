#include "../character_enemy_spotted.hpp"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::character_enemy_spotted;
namespace {
constexpr std::uintptr_t ai = 0x2001000, owner = 0x2004000, owner_b = 0x2008000;
constexpr std::uintptr_t enemy = 0x200c000, group = 0x2010000;
constexpr ActiveAIS first{0x2011000, 0x501ff00}, second{0x2012000, 0x501ff04};
std::uint32_t bits(float value) {
    std::uint32_t word;
    std::memcpy(&word, &value, sizeof(word));
    return word;
}
struct Fixture {
    State state{ai, owner, group, first, 0};
    Services services{};
    Result result{};
    std::int32_t enemy_state = 3, owner_state = 3, owner_b_state = 3;
    std::uint32_t combat = 0, player = 0, aggro = 0, delta = bits(1.0f);
    std::uint32_t amount = bits(5.0f);
    int mutation = 0;
    std::string failure, throwing;
    bool nested = false, replace_services = false;
    unsigned holds = 0, releases = 0;
    ActiveAIS selected{};
    std::uintptr_t added_owner = 0, dispatched_enemy = 0;
    std::vector<std::string> calls;
    Fixture();
    std::string name(std::uintptr_t value) const {
        if (value == enemy) return "enemy";
        if (value == owner) return "owner";
        if (value == owner_b) return "owner_b";
        throw std::runtime_error("unexpected character identity");
    }
    int record(const std::string& call) {
        calls.push_back(call);
        if (call == throwing) throw std::runtime_error("provider exception");
        return call == failure;
    }
    std::int32_t id(std::uintptr_t character) const {
        return character == enemy ? enemy_state : character == owner ? owner_state : owner_b_state;
    }
    Status run() { return on_enemy_spotted(&state, enemy, &services, &result); }
};
Fixture::Fixture() {
    services.context = this;
    services.debug_switch = [](void* context, State* s, DebugPoint point) {
        auto& f = *static_cast<Fixture*>(context);
        const auto status = f.record(point == DebugPoint::entry ? "debug_entry" : "debug_added");
        if (point == DebugPoint::positive_aggro_added && f.mutation == 6) s->active = second;
        return status;
    };
    services.group_enemy_spotted = [](void* context, State* s, std::uintptr_t g,
                                      std::uintptr_t o, std::uintptr_t e) {
        auto& f = *static_cast<Fixture*>(context);
        assert(g == group && o == owner && e == enemy);
        const auto status = f.record("group");
        if (f.mutation == 1) { s->owner_identity = owner_b; s->active = second; }
        if (f.nested) {
            Result untouched{}; untouched.debug_queries = 123;
            assert(on_enemy_spotted(s, e, &f.services, &untouched) == Status::reentrant_call);
            assert(untouched.debug_queries == 123);
        }
        if (f.replace_services) f.services = {};
        return status;
    };
    services.is_awaiting_to_spawn = [](void* context, State* s, std::uintptr_t c,
                                      std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context);
        const auto status = f.record("await_" + f.name(c));
        *out = f.id(c) == 17;
        if (c == enemy && f.mutation == 2) s->owner_identity = owner_b;
        return status;
    };
    services.is_in_limbus = [](void* context, State* s, std::uintptr_t c, std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context);
        const auto status = f.record("limbus_" + f.name(c));
        *out = f.id(c) == 0;
        if (c == enemy && f.mutation == 10) s->owner_identity = owner_b;
        return status;
    };
    services.is_in_combat = [](void* context, State*, std::uintptr_t a, std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context); assert(a == ai);
        const auto status = f.record("combat"); *out = f.combat; return status;
    };
    services.is_player = [](void* context, State*, std::uintptr_t e, std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context); assert(e == enemy);
        const auto status = f.record("player"); *out = f.player; return status;
    };
    services.get_aggro = [](void* context, State* s, std::uintptr_t a, std::uintptr_t e,
                            std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context); assert(a == ai && e == enemy);
        const auto status = f.record("get_aggro"); *out = f.aggro;
        if (f.mutation == 3) s->owner_identity = owner_b;
        return status;
    };
    services.initial_aggro = [](void* context, State* s, std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context);
        const auto status = f.record("amount"); *out = f.amount;
        if (f.mutation == 4) s->owner_identity = owner_b;
        return status;
    };
    services.add_aggro = [](void* context, State* s, std::uintptr_t o, std::uintptr_t e,
                            std::uint32_t amount, std::uint32_t* out) {
        auto& f = *static_cast<Fixture*>(context); assert(e == enemy && amount == f.amount);
        const auto status = f.record("add_" + f.name(o)); f.added_owner = o; *out = f.delta;
        if (f.mutation == 5) s->active = second;
        return status;
    };
    services.retain_active = [](void* context, State* s, const ActiveAIS* active, void** hold) {
        auto& f = *static_cast<Fixture*>(context);
        const auto status = f.record("retain");
        if (status) return status;
        f.selected = *active; ++f.holds; *hold = &f.selected;
        if (f.mutation == 8) s->active = second;
        return 0;
    };
    services.dispatch_active = [](void* context, State* s, const ActiveAIS* active,
                                 void* hold, std::uintptr_t e) {
        auto& f = *static_cast<Fixture*>(context);
        assert(hold == &f.selected && f.holds == 1 && e == enemy);
        assert(active->identity == f.selected.identity && active->callee == f.selected.callee);
        const auto status = f.record(active->identity == first.identity ? "dispatch_first" : "dispatch_second");
        f.dispatched_enemy = e;
        if (f.mutation == 9) s->active = {};  // Held session must survive removal.
        return status;
    };
    services.release_active = [](void* context, const ActiveAIS* active, void* hold) noexcept {
        auto& f = *static_cast<Fixture*>(context);
        assert(hold == &f.selected && f.holds == 1 && active->identity == f.selected.identity);
        --f.holds; ++f.releases;
    };
}
void emit(const Fixture& f, Status status) {
    std::cout << "{\"status\":" << static_cast<int>(status)
              << ",\"decision\":" << static_cast<unsigned>(f.result.decision)
              << ",\"selected\":" << f.result.dispatched_active.identity
              << ",\"added_owner\":" << f.added_owner << ",\"calls\":[";
    for (std::size_t i = 0; i < f.calls.size(); ++i) {
        if (i) std::cout << ',';
        std::cout << '"' << f.calls[i] << '"';
    }
    std::cout << "]}\n";
}
}  // namespace

int main(int argc, char** argv) {
    if (argc == 10) {
        Fixture f;
        f.enemy_state = std::stoi(argv[1]); f.owner_state = std::stoi(argv[2]);
        f.combat = static_cast<std::uint32_t>(std::stoul(argv[3]));
        f.player = static_cast<std::uint32_t>(std::stoul(argv[4]));
        f.aggro = static_cast<std::uint32_t>(std::stoul(argv[5]));
        f.delta = static_cast<std::uint32_t>(std::stoul(argv[6]));
        if (!std::stoi(argv[7])) f.state.group_identity = 0;
        f.mutation = std::stoi(argv[8]);
        if (f.mutation == 10) f.owner_b_state = 0;
        if (!std::stoi(argv[9])) f.state.active = {};
        const auto status = f.run(); emit(f, status); return 0;
    }
    assert(argc == 1);
    unsigned cases = 0;
    auto complete = [&](Fixture& f, Decision expected = Decision::dispatched) {
        assert(f.run() == Status::complete && f.result.decision == expected);
        assert(f.state.in_progress == 0 && f.holds == 0);
        ++cases;
    };
    for (auto threat : {0u, 0x80000000u, 0x3f800000u, 0xbf800000u,
                        0x7fc12345u, 0x7f800000u}) {
        Fixture f; f.aggro = threat; complete(f);
        assert(f.result.aggro_adds == (threat == 0 || threat == 0x80000000u));
        assert(f.result.player_queries == 0 && f.result.active_dispatches == 1);
    }
    for (auto delta : {0u, 0x80000000u, 0xbf800000u, 0x7fc12345u, 0x7f800000u}) {
        Fixture f; f.delta = delta; complete(f);
        assert(f.result.aggro_adds == 1 && f.result.debug_queries == (delta == 0x7f800000u ? 2u : 1u));
    }
    {
        Fixture f; f.combat = 7; f.player = 0;
        f.services.get_aggro = nullptr; f.services.initial_aggro = nullptr;
        f.services.add_aggro = nullptr; complete(f);
        assert(f.result.player_queries == 1 && f.result.aggro_queries == 0);
    }
    { Fixture f; f.combat = 1; f.player = 9; complete(f); assert(f.result.aggro_adds == 1); }
    for (unsigned gate = 0; gate < 4; ++gate) {
        Fixture f;
        if (gate == 0) f.enemy_state = 17;
        if (gate == 1) f.owner_state = 17;
        if (gate == 2) f.enemy_state = 0;
        if (gate == 3) f.owner_state = 0;
        f.services.is_in_combat = nullptr;
        complete(f, static_cast<Decision>(gate + 1));
        assert(f.calls[0] == "debug_entry" && f.calls[1] == "group");
        assert(f.result.combat_queries == 0 && f.result.active_retains == 0);
    }
    { Fixture f; f.enemy_state = f.owner_state = 12; complete(f); }
    {
        Fixture f; f.mutation = 1; f.owner_state = 17; complete(f);
        assert(f.calls[3] == "await_owner_b" && f.result.dispatched_active.identity == second.identity);
    }
    {
        Fixture f; f.mutation = 2; f.owner_state = 17; complete(f);
        assert(f.calls[3] == "await_owner_b" && f.calls[5] == "limbus_owner_b");
    }
    {
        Fixture f; f.mutation = 10; f.owner_b_state = 0;
        complete(f, Decision::owner_in_limbus);
        assert(f.calls[3] == "await_owner" && f.calls[5] == "limbus_owner_b");
    }
    { Fixture f; f.mutation = 3; complete(f); assert(f.added_owner == owner_b); }
    { Fixture f; f.mutation = 4; complete(f); assert(f.added_owner == owner); }
    for (int mutation : {5, 6}) {
        Fixture f; f.mutation = mutation; complete(f);
        assert(f.result.dispatched_active.identity == second.identity);
    }
    {
        Fixture f; f.state.active = {}; f.services.retain_active = nullptr;
        complete(f, Decision::no_active_ais); assert(f.result.aggro_adds == 1);
    }
    {
        Fixture f; f.mutation = 8; complete(f);
        assert(f.state.active.identity == second.identity && f.result.dispatched_active.identity == first.identity);
    }
    { Fixture f; f.mutation = 9; complete(f); assert(!f.state.active.identity && f.releases == 1); }
    { Fixture f; f.nested = true; complete(f); }
    { Fixture f; f.replace_services = true; complete(f); }
    {
        Fixture f; f.state.group_identity = 0; f.services.group_enemy_spotted = nullptr;
        f.services.debug_switch = nullptr; complete(f); assert(!f.result.group_calls && !f.result.debug_queries);
    }
    {
        Fixture f; f.failure = "add_owner"; f.mutation = 5;
        assert(f.run() == Status::service_failed && f.result.aggro_adds == 1);
        assert(f.state.active.identity == second.identity && f.result.active_retains == 0);
        assert(!f.state.in_progress); ++cases;
    }
    {
        Fixture f; f.failure = "dispatch_first";
        assert(f.run() == Status::service_failed && f.releases == 1 && !f.holds);
        assert(!f.state.in_progress && f.result.aggro_adds == 1); ++cases;
    }
    {
        Fixture f; f.throwing = "dispatch_first";
        try { f.run(); assert(false); } catch (const std::runtime_error&) {}
        assert(f.releases == 1 && !f.holds && !f.state.in_progress && f.result.aggro_adds == 1); ++cases;
    }
    {
        Fixture f; f.failure = "retain";
        assert(f.run() == Status::active_lifetime_failed && !f.holds && !f.releases);
        assert(f.result.aggro_adds == 1 && !f.state.in_progress); ++cases;
    }
    {
        Fixture f; f.services.get_aggro = nullptr;
        assert(f.run() == Status::service_unavailable && f.result.combat_queries == 1);
        assert(f.result.aggro_queries == 0 && !f.state.in_progress); ++cases;
    }
    {
        Fixture f; f.state.owner_identity = 0;
        assert(f.run() == Status::invalid_live_projection && f.calls.size() == 1);
        assert(!f.state.in_progress); ++cases;
    }
    {
        Fixture f; f.result.debug_queries = 123;
        assert(on_enemy_spotted(&f.state, enemy, &f.services, reinterpret_cast<Result*>(&f.state)) == Status::invalid_argument);
        assert(on_enemy_spotted(&f.state, 0, &f.services, &f.result) == Status::invalid_argument);
        assert(on_enemy_spotted(nullptr, enemy, &f.services, &f.result) == Status::invalid_argument);
        assert(f.result.debug_queries == 123 && f.calls.empty()); ++cases;
    }
    std::cout << "{\"enemy_spotted_cases\":" << cases
              << ",\"awaiting_spawn_not_dead_gate\":true,\"group_precedes_state_queries\":true,"
                 "\"fresh_owner_and_active_reads\":true,\"zero_aggro_adds_before_dispatch\":true,"
                 "\"binary32_edges_checked\":true,\"held_selected_ais_survives_replacement\":true,"
                 "\"partial_failure_and_reentry_guard_checked\":true,\"mismatches\":0,"
                 "\"native_wired\":false}\n";
}
