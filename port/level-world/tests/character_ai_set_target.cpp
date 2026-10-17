#include "../character_ai_set_target.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <vector>

namespace {
using namespace dh2::character::set_target;

struct Call {
    std::uint32_t operation;
    std::uint32_t key;
    std::uintptr_t ai;
    std::uintptr_t owner;
    std::uintptr_t target;
};

struct Fixture {
    explicit Fixture(State* source) : state(source) {}
    State* state = nullptr;
    OwnerFacts* replacement_owner = nullptr;
    std::vector<Call> calls;
    std::uint32_t trace_changes = 0;
    std::uint32_t dead = 0;
    std::uint32_t sight = 0;
    std::uint32_t dead_replacement_target = 0;
    int fail_operation = 0;
    bool change_target_in_trace_query = false;
    bool mutate_target_and_owner_on_dead = false;
};

bool check(bool condition, const char* message) {
    if (condition) return true;
    std::fprintf(stderr, "FAIL: %s\n", message);
    return false;
}

int invoke(void* raw, const Request* request, Response* response) {
    auto* fixture = static_cast<Fixture*>(raw);
    fixture->calls.push_back({request->operation, request->key,
                              request->ai_identity, request->owner_identity,
                              request->target_identity});
    *response = {};
    if (fixture->fail_operation == static_cast<int>(request->operation)) return -1;

    if (request->operation == debug_switch_lookup) {
        if (request->key == trace_target_changes) {
            response->word = fixture->trace_changes;
            if (fixture->change_target_in_trace_query) {
                fixture->state->target = fixture->state->requested_target;
            }
        }
    } else if (request->operation == target_is_dead) {
        response->word = fixture->dead;
        if (fixture->mutate_target_and_owner_on_dead) {
            fixture->state->target = fixture->dead_replacement_target;
            fixture->state->owner = fixture->replacement_owner;
        }
    } else if (request->operation == ai_is_in_sight) {
        response->word = fixture->sight;
    }
    return 0;
}

void initialize(State& state, OwnerFacts& owner) {
    owner = {0x100, 35, 9, 0};
    state = {0x200, &owner, 0, 0, 0, 0, 0, 1, 0};
}

bool forced_target_is_early_store_only() {
    OwnerFacts owner{};
    State state{};
    initialize(state, owner);
    state.requested_target = 77;
    state.target = 11;
    state.last_target = 12;
    state.alive_snapshot = 7;
    state.sight_snapshot = 8;
    owner.target_change_marker_14d0 = 0x1234;
    Fixture fixture{&state};
    Services services{&fixture, 76, invoke};
    const int result = dh2_character_ai_set_target(&state, 22, 1, &services);
    return check(result == complete, "force target succeeds") &&
           check(state.requested_target == 22 && state.target == 22,
                 "force stores requested and current target") &&
           check(state.last_target == 12 && state.alive_snapshot == 7 &&
                     state.sight_snapshot == 8 && state.sticky == 1,
                 "force skips last/dead/sight/sticky work") &&
           check(owner.target_change_marker_14d0 == 0x1234,
                 "force skips owner marker reset") &&
           check(fixture.calls.empty(), "force makes no external calls");
}

bool normal_target_set_and_duplicate_refresh() {
    bool ok = true;
    OwnerFacts owner{};
    State state{};
    initialize(state, owner);
    state.target = 0;
    state.last_target = 9;
    state.sticky = 1;
    owner.target_change_marker_14d0 = 0xfeed;
    Fixture fixture{&state};
    fixture.dead = 1;
    fixture.sight = 0;
    Services services{&fixture, 76, invoke};
    ok &= check(dh2_character_ai_set_target(&state, 30, 0, &services) == complete,
                "normal nonnull target set completes");
    ok &= check(state.requested_target == 30 && state.target == 30 &&
                    state.last_target == 30 && state.sticky == 0,
                "normal path writes current/last and clears sticky on change");
    ok &= check(state.alive_snapshot == 0 && state.sight_snapshot == 0,
                "dead snapshot is inverted and sight snapshot is stored");
    ok &= check(owner.target_change_marker_14d0 == 0,
                "null to nonnull clears the owner target-change marker");
    const std::array<std::uint32_t, 5> expected{
        debug_switches_load, debug_switch_lookup, target_is_dead,
        ai_is_in_sight, 0};
    ok &= check(fixture.calls.size() == 4,
                "normal set queries debug, target-dead and sight in source order");
    if (fixture.calls.size() == 4) {
        ok &= check(fixture.calls[0].operation == expected[0] &&
                        fixture.calls[1].operation == expected[1] &&
                        fixture.calls[1].key == trace_target_changes &&
                        fixture.calls[2].operation == expected[2] &&
                        fixture.calls[2].target == 30 &&
                        fixture.calls[3].operation == expected[3] &&
                        fixture.calls[3].target == 30,
                    "normal set callback identities/order match source");
    }

    fixture.calls.clear();
    state.last_target = state.target;
    state.sticky = 1;
    fixture.dead = 0;
    fixture.sight = 1;
    ok &= check(dh2_character_ai_set_target(&state, 30, 0, &services) == complete,
                "duplicate target refresh completes");
    ok &= check(state.target == 30 && state.last_target == 30 && state.sticky == 1,
                "duplicate target refresh preserves sticky when target equals last");
    ok &= check(state.alive_snapshot == 1 && state.sight_snapshot == 1,
                "duplicate still refreshes virtual dead and sight snapshots");
    return ok;
}

bool null_target_and_trace_null_early_return() {
    bool ok = true;
    OwnerFacts owner{};
    State state{};
    initialize(state, owner);
    state.target = 55;
    state.last_target = 44;
    state.alive_snapshot = 8;
    state.sight_snapshot = 9;
    state.sticky = 1;
    owner.target_change_marker_14d0 = 0x1234;
    Fixture fixture{&state};
    Services services{&fixture, 76, invoke};

    ok &= check(dh2_character_ai_set_target(&state, 0, 0, &services) == complete,
                "null target clear completes");
    ok &= check(state.requested_target == 0 && state.target == 0 &&
                    state.last_target == 44 && state.alive_snapshot == 8 &&
                    state.sight_snapshot == 9 && state.sticky == 1,
                "null target retains last/dead/sight/sticky source snapshots");
    ok &= check(owner.target_change_marker_14d0 == 0,
                "changing to null clears the owner target-change marker first");
    ok &= check(fixture.calls.size() == 2,
                "null target exits after the two debug-switch calls");

    initialize(state, owner);
    state.target = 55;
    state.last_target = 44;
    state.alive_snapshot = 8;
    state.sight_snapshot = 9;
    state.sticky = 1;
    owner.target_change_marker_14d0 = 0x1234;
    fixture.calls.clear();
    fixture.trace_changes = 1;
    ok &= check(dh2_character_ai_set_target(&state, 0, 0, &services) == complete,
                "trace-mode null target clear completes");
    ok &= check(state.target == 0 && state.last_target == 44 &&
                    state.alive_snapshot == 8 && state.sight_snapshot == 9 &&
                    state.sticky == 1,
                "trace old-target to null branch stores null then exits early");
    ok &= check(fixture.calls.size() == 4 &&
                    fixture.calls[2].operation == debug_switches_load &&
                    fixture.calls[3].key == trace_target_details,
                "trace null transition queries detail, then skips virtual/sight refresh");
    return ok;
}

bool tracing_transition_and_fresh_target_reads() {
    bool ok = true;
    OwnerFacts owner{}, replacement_owner{};
    State state{};
    initialize(state, owner);
    replacement_owner = {0x101, 1, 5, 0};
    state.target = 11;
    state.last_target = 10;
    Fixture fixture{&state};
    fixture.trace_changes = 1;
    fixture.dead = 0;
    fixture.sight = 1;
    Services services{&fixture, 76, invoke};

    ok &= check(dh2_character_ai_set_target(&state, 22, 0, &services) == complete,
                "traced target transition completes");
    const std::array<std::uint32_t, 7> expected{
        debug_switches_load, debug_switch_lookup, debug_switches_load,
        debug_switch_lookup, target_is_dead, ai_is_in_sight, 0};
    ok &= check(fixture.calls.size() == 6,
                "traced nonnull transition queries detail before target predicates");
    if (fixture.calls.size() == 6) {
        ok &= check(fixture.calls[0].operation == expected[0] &&
                        fixture.calls[1].key == trace_target_changes &&
                        fixture.calls[2].operation == expected[2] &&
                        fixture.calls[3].key == trace_target_details &&
                        fixture.calls[4].operation == expected[4] &&
                        fixture.calls[5].operation == expected[5],
                    "traced transition calls use exact key/order");
    }

    fixture.calls.clear();
    initialize(state, owner);
    state.target = 11;
    state.last_target = 0;
    fixture.change_target_in_trace_query = true;
    // Main debug lookup changes the live target to the requested pointer.
    // The original rereads it and takes the ordinary path without detail log.
    ok &= check(dh2_character_ai_set_target(&state, 22, 0, &services) == complete,
                "fresh target after debug query completes");
    ok &= check(fixture.calls.size() == 4 &&
                    fixture.calls[2].operation == target_is_dead &&
                    fixture.calls[2].target == 22,
                "fresh target read suppresses stale detail branch");

    fixture.calls.clear();
    fixture.change_target_in_trace_query = false;
    fixture.mutate_target_and_owner_on_dead = true;
    fixture.trace_changes = 0;
    fixture.dead_replacement_target = 33;
    fixture.replacement_owner = &replacement_owner;
    initialize(state, owner);
    state.target = 11;
    state.last_target = 0;
    ok &= check(dh2_character_ai_set_target(&state, 22, 0, &services) == complete,
                "live target mutation in death virtual completes");
    ok &= check(state.target == 33 && state.last_target == 22 &&
                    state.alive_snapshot == 1 && state.sight_snapshot == 1,
                "death result applies to captured target then source reloads target for sight");
    ok &= check(fixture.calls.size() == 4 &&
                    fixture.calls[2].target == 22 && fixture.calls[3].target == 33 &&
                    fixture.calls[3].owner == replacement_owner.identity,
                "IsDead and AI_IsInSight receive distinct source-time snapshots");
    return ok;
}

bool service_failure_keeps_prior_source_writes() {
    OwnerFacts owner{};
    State state{};
    initialize(state, owner);
    state.target = 11;
    state.last_target = 0;
    owner.target_change_marker_14d0 = 0xaaaa;
    Fixture fixture{&state};
    fixture.fail_operation = target_is_dead;
    Services services{&fixture, 76, invoke};
    const int result = dh2_character_ai_set_target(&state, 22, 0, &services);
    return check(result == source_service_failed, "target virtual failure surfaced") &&
           check(state.requested_target == 22 && state.target == 22 &&
                     state.last_target == 22 && state.sticky == 0,
                 "failure does not roll back writes completed before IsDead") &&
           check(state.alive_snapshot == 0 && state.sight_snapshot == 0,
                 "failed IsDead stops before alive and sight updates") &&
           check(fixture.calls.size() == 3,
                 "failed IsDead prevents later sight call");
}

bool noncharacter_target_uses_virtual_dead_boundary() {
    OwnerFacts owner{};
    State state{};
    initialize(state, owner);
    Fixture fixture{&state};
    fixture.sight = 1;
    Services services{&fixture, 76, invoke};
    // GameObject::IsDead returns false in the original. The setter itself has
    // no Character-only gate; dynamic virtual dispatch belongs to this service.
    const int result = dh2_character_ai_set_target(&state, 99, 0, &services);
    bool saw_virtual = false;
    for (const auto& call : fixture.calls) {
        if (call.operation == target_is_dead && call.target == 99) saw_virtual = true;
    }
    return check(result == complete && state.target == 99,
                 "base GameObject target is accepted") &&
           check(saw_virtual && state.alive_snapshot == 1,
                 "non-Character target still uses its source IsDead virtual");
}

bool alias_rejected_before_mutation() {
    OwnerFacts owner{};
    State state{};
    initialize(state, owner);
    const auto original = state;
    state.owner = reinterpret_cast<OwnerFacts*>(&state);
    Fixture fixture{&state};
    Services services{&fixture, 76, invoke};
    const int result = dh2_character_ai_set_target(&state, 22, 0, &services);
    return check(result == invalid_argument, "overlapping owner projection rejected") &&
           check(state.requested_target == original.requested_target &&
                     state.target == original.target,
                 "alias rejection happens before source writes") &&
           check(fixture.calls.empty(), "alias rejection makes no calls");
}

}  // namespace

int main() {
    bool ok = true;
    ok &= forced_target_is_early_store_only();
    ok &= normal_target_set_and_duplicate_refresh();
    ok &= null_target_and_trace_null_early_return();
    ok &= tracing_transition_and_fresh_target_reads();
    ok &= service_failure_keeps_prior_source_writes();
    ok &= noncharacter_target_uses_virtual_dead_boundary();
    ok &= alias_rejected_before_mutation();
    if (!ok) return 1;
    std::puts("character AI set target host checks passed: 7 suites; force, null, duplicate, tracing, fresh reads, virtual class dispatch, partial failure");
    return 0;
}
