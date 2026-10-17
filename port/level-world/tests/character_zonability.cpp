#include "../character_zonability.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <vector>

using namespace dh2::character_zonability;

struct Fixture {
    std::vector<Operation> calls;
    std::uint32_t player = 0;
    std::uint32_t faerie = 0;
    Operation fail = Operation::is_player;
    bool fail_enabled = false;
    bool throw_enabled = false;
    bool mutate_identity = false;
    std::uintptr_t replacement = 0;
    std::uintptr_t captured = 0;
};

static std::int32_t invoke(void* raw, State* state, const Request* request,
                           Response* response) {
    auto& fixture = *static_cast<Fixture*>(raw);
    assert(state && request && response && request->reserved == 0);
    fixture.calls.push_back(request->operation);
    if (!fixture.captured) fixture.captured = request->character;
    assert(request->character == fixture.captured);
    if (fixture.mutate_identity && request->operation == Operation::is_player)
        state->character = fixture.replacement;
    if (fixture.throw_enabled && request->operation == fixture.fail)
        throw std::runtime_error("provider exception");
    if (fixture.fail_enabled && request->operation == fixture.fail) return 1;
    response->word = request->operation == Operation::is_player
        ? fixture.player : fixture.faerie;
    return 0;
}

static void run_case(std::uint32_t player, std::uint32_t faerie,
                     Decision decision, std::uint32_t zonable,
                     std::uint32_t expected_calls) {
    constexpr std::uintptr_t identity = 0x100000123ull;
    State state{identity};
    Fixture fixture{};
    fixture.player = player;
    fixture.faerie = faerie;
    const Services services{&fixture, invoke};
    Result result{};
    assert(evaluate(&state, &services, &result) == Status::complete);
    assert(result.decision == decision && result.zonable == zonable);
    assert(result.captured_character == identity && fixture.captured == identity);
    assert(result.service_calls == expected_calls && fixture.calls.size() == expected_calls);
    assert(result.is_player_word == player);
    assert(result.is_faerie_word == (expected_calls == 2 ? faerie : 0));
}

int main() {
    unsigned cases = 0;
    for (const auto player : {0u, 1u, 0x80000000u, 0xffffffffu}) {
        for (const auto faerie : {0u, 1u, 0x80000000u}) {
            const bool is_player = player != 0;
            const bool is_faerie = !is_player && faerie != 0;
            run_case(player, faerie,
                is_player ? Decision::player : is_faerie ? Decision::faerie : Decision::base_condition,
                (!is_player && !is_faerie) ? 1u : 0u,
                is_player ? 1u : 2u);
            ++cases;
        }
    }

    // The first callback may refresh/rebind a live projection. The source
    // method retains its original Character `this` for the second query.
    {
        State state{0x100000123ull};
        Fixture fixture{};
        fixture.mutate_identity = true;
        fixture.replacement = 0x200000456ull;
        fixture.faerie = 1;
        const Services services{&fixture, invoke};
        Result result{};
        assert(evaluate(&state, &services, &result) == Status::complete);
        assert(state.character == fixture.replacement);
        assert(fixture.calls.size() == 2 && result.captured_character == 0x100000123ull);
        assert(result.decision == Decision::faerie && result.zonable == 0);
        ++cases;
    }

    // Failed providers preserve the completed source prefix and never turn a
    // missing fact into a false/true classification.
    for (bool throws : {false, true}) {
        State state{0x100000123ull};
        Fixture fixture{};
        fixture.player = 0;
        fixture.fail = Operation::is_faerie;
        fixture.fail_enabled = !throws;
        fixture.throw_enabled = throws;
        const Services services{&fixture, invoke};
        Result result{};
        assert(evaluate(&state, &services, &result) == Status::service_failed);
        assert(result.decision == Decision::incomplete && result.zonable == 0);
        assert(result.service_calls == 2 && result.is_player_word == 0);
        assert(fixture.calls.size() == 2 && fixture.calls[0] == Operation::is_player &&
               fixture.calls[1] == Operation::is_faerie);
        ++cases;
    }

    // Null provider fails explicitly. Invalid/aliased/misaligned arguments do
    // not alter caller output or state.
    {
        State state{0x100000123ull};
        const Services services{nullptr, nullptr};
        Result result{};
        assert(evaluate(&state, &services, &result) == Status::service_unavailable);
        assert(result.decision == Decision::incomplete && result.zonable == 0);
        ++cases;
    }
    {
        State state{0x100000123ull};
        Fixture fixture{};
        const Services services{&fixture, invoke};
        const auto before = state.character;
        assert(evaluate(&state, &services, reinterpret_cast<Result*>(&state)) == Status::invalid_argument);
        assert(state.character == before);
        ++cases;
    }
    {
        alignas(State) std::uint8_t raw[sizeof(State) + alignof(State)]{};
        auto* misaligned = reinterpret_cast<State*>(raw + 1);
        State* state = misaligned;
        Result result{Decision::player, 9, 8, 7, 6, 5, 4};
        Fixture fixture{};
        const Services services{&fixture, invoke};
        assert(evaluate(state, &services, &result) == Status::invalid_argument);
        assert(result.decision == Decision::player && result.service_calls == 9 && fixture.calls.empty());
        ++cases;
    }
    {
        State state{0};
        Fixture fixture{};
        const Services services{&fixture, invoke};
        Result result{Decision::faerie, 4, 3, 2, 1, 0, 9};
        assert(evaluate(&state, &services, &result) == Status::invalid_argument);
        assert(result.decision == Decision::faerie && result.service_calls == 4 && fixture.calls.empty());
        ++cases;
    }

    std::printf("{\"validation\":\"PASS\",\"cases\":%u,\"source_identity_width\":64,\"provider_failures\":2,\"stale_owner_rebind_checked\":true}\n", cases);
    return 0;
}
