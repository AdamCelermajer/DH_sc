#include "../character_ai_turn.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace dh2::character_ai_turn;

struct Call {
    std::uint32_t query;
    std::uintptr_t owner;
};

struct Fixture {
    std::uint32_t values[3] = {0, 0, 0};
    std::uintptr_t after_follower = 0;
    std::uintptr_t after_faerie = 0;
    int fail_query = -1;
    std::vector<Call> calls;
};

struct Snapshot {
    Services* live = nullptr;
    unsigned original_calls = 0;
    unsigned replacement_calls = 0;
};

void require(bool condition, const char* reason) {
    if (!condition) throw std::runtime_error(reason);
}

std::uint64_t parse_unsigned(const char* text) {
    char* end = nullptr;
    const auto value = std::strtoull(text, &end, 0);
    if (!end || *end) throw std::runtime_error("invalid unsigned command-line word");
    return value;
}

std::int32_t parse_signed(const char* text) {
    char* end = nullptr;
    const auto value = std::strtoll(text, &end, 0);
    if (!end || *end || value < INT32_MIN || value > INT32_MAX)
        throw std::runtime_error("invalid signed command-line timer");
    return static_cast<std::int32_t>(value);
}

std::int32_t invoke(void* raw, State* state, Query query,
                    std::uintptr_t owner, std::uint32_t* value) {
    auto& fixture = *static_cast<Fixture*>(raw);
    const auto index = static_cast<std::uint32_t>(query);
    fixture.calls.push_back({index, owner});
    *value = fixture.values[index];
    if (query == Query::is_follower && fixture.after_follower)
        state->owner = fixture.after_follower;
    if (query == Query::is_faerie && fixture.after_faerie)
        state->owner = fixture.after_faerie;
    return fixture.fail_query == static_cast<int>(index) ? -1 : 0;
}

std::int32_t snapshot_replacement(void* raw, State*, Query,
                                  std::uintptr_t, std::uint32_t* value) {
    auto& data = *static_cast<Snapshot*>(raw);
    ++data.replacement_calls;
    *value = 1;
    return 0;
}

std::int32_t snapshot_original(void* raw, State*, Query query,
                               std::uintptr_t, std::uint32_t* value) {
    auto& data = *static_cast<Snapshot*>(raw);
    ++data.original_calls;
    *value = query == Query::virtual_is_player ? 13U : 0U;
    *data.live = {&data, snapshot_replacement};
    return 0;
}

int no_args_checks() {
    constexpr std::uintptr_t ai = 0x10010000;
    constexpr std::uintptr_t owner = 0x10014000;
    constexpr std::uintptr_t front = 0x10010100;
    unsigned checks = 0;
    Fixture fixture;
    State state{ai, owner};
    Globals globals{front, 1, 1};
    Services services{&fixture, invoke};
    Result result{77, 77, 77, 77, 77};

    require(evaluate(nullptr, &globals, &services, &result) == Status::invalid_argument,
            "null state must reject");
    require(fixture.calls.empty() && result.value == 77, "null rejection has no effects");
    ++checks;

    auto* overlapping_globals = reinterpret_cast<Globals*>(&state);
    require(evaluate(&state, overlapping_globals, &services, &result) ==
                Status::invalid_argument,
            "state/global alias must reject before reading either view");
    require(fixture.calls.empty() && result.value == 77,
            "alias rejection leaves result and callbacks untouched");
    ++checks;

    auto* overlapping_result = reinterpret_cast<Result*>(&state);
    require(evaluate(&state, &globals, &services, overlapping_result) ==
                Status::invalid_argument,
            "state/result alias must reject");
    require(fixture.calls.empty(), "state/result alias invokes no callback");
    ++checks;

    alignas(8) std::uint8_t misaligned_storage[64]{};
    auto* misaligned_state = reinterpret_cast<State*>(misaligned_storage + 1);
    require(evaluate(misaligned_state, &globals, &services, &result) ==
                Status::invalid_argument,
            "misaligned State must reject before typed reads");
    ++checks;
    auto* misaligned_globals = reinterpret_cast<Globals*>(misaligned_storage + 1);
    require(evaluate(&state, misaligned_globals, &services, &result) ==
                Status::invalid_argument,
            "misaligned Globals must reject before typed reads");
    ++checks;
    auto* misaligned_services = reinterpret_cast<Services*>(misaligned_storage + 1);
    require(evaluate(&state, &globals, misaligned_services, &result) ==
                Status::invalid_argument,
            "misaligned Services must reject before typed reads");
    ++checks;
    auto* misaligned_result = reinterpret_cast<Result*>(misaligned_storage + 1);
    require(evaluate(&state, &globals, &services, misaligned_result) ==
                Status::invalid_argument,
            "misaligned Result must reject before typed writes");
    require(fixture.calls.empty(), "misaligned views invoke no callback");
    ++checks;

    state.ai = 0;
    require(evaluate(&state, &globals, &services, &result) == Status::invalid_argument,
            "zero AI identity must reject");
    require(fixture.calls.empty(), "zero AI rejection invokes no callback");
    ++checks;

    state = {ai, owner};
    globals.queue_length = 0;
    const auto empty_status = evaluate(&state, &globals, &services, &result);
    require(empty_status == Status::empty_queue_unsupported,
            "empty queue must be explicit and unsupported");
    require(fixture.calls.empty() && result.value == 0 && result.queue_front_read == 0,
            "empty queue must not query characters or claim a front read");
    ++checks;

    globals = {front, 1, 1};
    Services missing{nullptr, nullptr};
    const auto missing_status = evaluate(&state, &globals, &missing, &result);
    require(missing_status == Status::service_unavailable &&
                result.follower_queries == 0,
            "fallback without a service must fail closed before counting a call");
    ++checks;

    state.owner = 0;
    require(evaluate(&state, &globals, &services, &result) == Status::invalid_argument,
            "fallback with a null owner must reject");
    require(fixture.calls.empty(), "null-owner rejection invokes no callback");
    ++checks;

    // The source queue-front path does not need the owner or character services.
    state.owner = 0;
    globals = {ai, 1, 0};
    require(evaluate(&state, &globals, &missing, &result) == Status::complete &&
                result.value == 1 && result.queue_front_read == 1,
            "matching due queue head bypasses owner and missing services");
    ++checks;

    // Service errors retain synchronous callback side effects and stop later
    // requests; this matches the documented non-transactional contract.
    state = {ai, owner};
    globals = {front, 1, 1};
    fixture = {};
    fixture.fail_query = 0;
    fixture.after_follower = owner + 0x100;
    services = {&fixture, invoke};
    const auto failed = evaluate(&state, &globals, &services, &result);
    require(failed == Status::service_failed && state.owner == owner + 0x100 &&
                result.follower_queries == 1 && result.faerie_queries == 0 &&
                fixture.calls.size() == 1,
            "failed callback retains its mutation and stops dispatch");
    ++checks;

    // `evaluate` captures the service table once, but reloads State::owner for
    // each request. Mutating the caller's table must not redirect an in-flight
    // query sequence to a replacement context.
    Snapshot snapshot;
    snapshot.live = &services;
    services = {&snapshot, snapshot_original};
    fixture = {};
    state = {ai, owner};
    globals = {front, 1, 1};
    require(evaluate(&state, &globals, &services, &result) == Status::complete &&
                result.value == 13 && snapshot.original_calls == 3 &&
                snapshot.replacement_calls == 0,
            "in-flight requests retain captured service table");
    ++checks;

    std::printf("{\"validation\":\"PASS\",\"guard_checks\":%u,\"mismatches\":0}\n",
                checks);
    return 0;
}
}  // namespace

int main(int argc, char** argv) {
    try {
        if (argc == 1) return no_args_checks();
        if (argc != 13)
            throw std::runtime_error(
                "usage: character_ai_turn <ai> <owner> <front> <queue-length> "
                "<signed-timer> <follower> <faerie> <player> <owner-after-follower> "
                "<owner-after-faerie> <fail-query:-1..2> <missing-service:0|1>");

        Fixture fixture;
        fixture.values[0] = static_cast<std::uint32_t>(parse_unsigned(argv[6]));
        fixture.values[1] = static_cast<std::uint32_t>(parse_unsigned(argv[7]));
        fixture.values[2] = static_cast<std::uint32_t>(parse_unsigned(argv[8]));
        fixture.after_follower = static_cast<std::uintptr_t>(parse_unsigned(argv[9]));
        fixture.after_faerie = static_cast<std::uintptr_t>(parse_unsigned(argv[10]));
        fixture.fail_query = static_cast<int>(parse_signed(argv[11]));
        const bool missing_service = parse_unsigned(argv[12]) != 0;

        State state{static_cast<std::uintptr_t>(parse_unsigned(argv[1])),
                    static_cast<std::uintptr_t>(parse_unsigned(argv[2]))};
        Globals globals{static_cast<std::uintptr_t>(parse_unsigned(argv[3])),
                        static_cast<std::uint32_t>(parse_unsigned(argv[4])),
                        parse_signed(argv[5])};
        Services services{&fixture, missing_service ? nullptr : invoke};
        Result result{};
        const auto status = evaluate(&state, &globals, &services, &result);
        std::printf(
            "{\"status\":%d,\"value\":%u,\"follower_queries\":%u,"
            "\"faerie_queries\":%u,\"player_queries\":%u,"
            "\"queue_front_read\":%u,\"owner_after\":%llu,\"calls\":[",
            static_cast<int>(status), result.value, result.follower_queries,
            result.faerie_queries, result.player_queries, result.queue_front_read,
            static_cast<unsigned long long>(state.owner));
        for (std::size_t i = 0; i < fixture.calls.size(); ++i) {
            if (i) std::putchar(',');
            std::printf("[%u,%llu]", fixture.calls[i].query,
                        static_cast<unsigned long long>(fixture.calls[i].owner));
        }
        std::puts("]}");
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "character_ai_turn host check failed: %s\n", error.what());
        return 1;
    }
}
