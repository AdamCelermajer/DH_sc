#include "../character_aggro_delay.hpp"

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::character_aggro_delay;
namespace {
void require(bool value, const char* reason) { if (!value) throw std::runtime_error(reason); }
struct Fixture {
    std::uint32_t turn = 0, disabled = 0;
    std::uint32_t dt[2] = {16, 16};
    unsigned reads = 0;
    bool mutate_dt = false;
    int fail_dt = -1;
    int throw_dt = -1;
    std::vector<std::string> trace;
};
int turn(void* raw, State*, std::uint32_t* value) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("turn"); *value = f.turn; return 0;
}
int debug(void* raw, State* state, std::uint32_t* value) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("debug");
    require(state->elapsed_not_turn_ms == 0, "elapsed reset must precede debug query");
    *value = f.disabled; return 0;
}
int delta(void* raw, State* state, std::uint32_t* value) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("dt");
    const auto read = f.reads++; *value = f.dt[read < 2 ? read : 1];
    if (f.mutate_dt) {
        if (!read) { state->countdown_ms = 999; state->elapsed_not_turn_ms = 41; }
        else state->elapsed_not_turn_ms = 999;
    }
    if (int(read) == f.throw_dt) throw std::runtime_error("delta fixture exception");
    return int(read) == f.fail_dt ? 1 : 0;
}
std::uint32_t word(const char* value) { return std::uint32_t(std::strtoull(value, nullptr, 0)); }
}
int main(int argc, char** argv) {
    try {
        // Oracle mode accepts raw ARM words and distinct fresh GetDt values.
        if (argc == 10) {
            Fixture f; f.turn = word(argv[3]); f.disabled = word(argv[4]);
            f.dt[0] = word(argv[5]); f.dt[1] = word(argv[6]); f.mutate_dt = word(argv[9]) != 0;
            State state{word(argv[1]), word(argv[2])};
            dh2_random_state rng{{word(argv[7]), 0x76543210}, {word(argv[8]), 0x01234567}};
            Services svc{&f, turn, debug, delta}; Result result{};
            require(update(&state, &rng, &svc, &result) == Status::complete, "oracle host call failed");
            std::printf("{\"countdown\":%u,\"elapsed\":%u,\"seed\":%u,\"counter\":%u,\"sync_seed\":%u,\"sync_counter\":%u,\"decision\":%u,\"turn_queries\":%u,\"debug_queries\":%u,\"delta_reads\":%u,\"random_draws\":%u}\n",
                state.countdown_ms, state.elapsed_not_turn_ms, rng.seeds[0], rng.counters[0], rng.seeds[1], rng.counters[1], unsigned(result.decision), result.turn_queries, result.debug_queries, result.delta_reads, result.ordinary_random_draws);
            return 0;
        }
        unsigned guards = 0;
        Fixture f; State state{10, 20}; dh2_random_state rng{{123, 456}, {2, 3}};
        Services svc{&f, turn, debug, delta}; Result result{};
        require(update(nullptr, &rng, &svc, &result) == Status::invalid_argument && f.trace.empty(), "null effects"); ++guards;
        require(update(&state, &rng, &svc, reinterpret_cast<Result*>(&rng)) == Status::invalid_argument && f.trace.empty(), "RNG/result alias"); ++guards;
        require(update(&state, reinterpret_cast<dh2_random_state*>(&state), &svc, &result) == Status::invalid_argument && f.trace.empty(), "state/RNG alias"); ++guards;
        alignas(Services) unsigned char unaligned[sizeof(Services) + 8]{};
        require(update(reinterpret_cast<State*>(unaligned + 1), &rng, &svc, &result) == Status::invalid_argument && f.trace.empty(), "misaligned state rejected before read"); ++guards;
        require(update(&state, &rng, reinterpret_cast<Services*>(unaligned + 1), &result) == Status::invalid_argument && f.trace.empty(), "misaligned service table rejected before read"); ++guards;
        Services missing{&f, turn, nullptr, delta};
        require(update(&state, &rng, &missing, &result) == Status::complete && state.countdown_ms == 0xfffffffau && state.elapsed_not_turn_ms == 36, "untaken debug must be optional"); ++guards;
        f.trace.clear(); f.turn = 1; state = {10, 20};
        require(update(&state, &rng, &missing, &result) == Status::service_unavailable && state.elapsed_not_turn_ms == 0 && rng.counters[0] == 2, "missing taken debug effects"); ++guards;
        f.trace.clear(); f.turn = 0; f.reads = 0; f.fail_dt = 1; state = {10, 20};
        require(update(&state, &rng, &svc, &result) == Status::service_failed && state.countdown_ms == 0xfffffffau && state.elapsed_not_turn_ms == 20 && result.delta_reads == 2 && result.decision == Decision::incomplete, "second delta failure keeps first effects"); ++guards;
        f.trace.clear(); f.reads = 0; f.fail_dt = 0; f.mutate_dt = true; state = {10, 20};
        require(update(&state, &rng, &svc, &result) == Status::service_failed && state.countdown_ms == 999 && state.elapsed_not_turn_ms == 41 && rng.counters[0] == 2, "failed provider mutations retained"); ++guards;
        f.trace.clear(); f.reads = 0; f.fail_dt = -1; f.throw_dt = 1; f.mutate_dt = false; state = {10, 20};
        require(update(&state, &rng, &svc, &result) == Status::service_failed && state.countdown_ms == 0xfffffffau && state.elapsed_not_turn_ms == 20 && result.delta_reads == 2 && rng.counters[0] == 2, "throwing second provider retains first effects"); ++guards;
        std::printf("{\"validation\":\"PASS\",\"guard_cases\":%u,\"mismatches\":0,\"native_ai_wired\":false}\n", guards);
        return 0;
    } catch (const std::exception& error) { std::fprintf(stderr, "%s\n", error.what()); return 1; }
}
