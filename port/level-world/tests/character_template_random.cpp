#include "../character_template_random.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>

namespace {
using namespace dh2::character::template_random;

bool check(bool condition, const char* message) {
    if (condition) return true;
    std::fprintf(stderr, "FAIL: %s\n", message);
    return false;
}

std::uint32_t source_next(std::uint32_t seed) {
    return (seed * UINT32_C(0xe6ab) + UINT32_C(0x2b3fd)) % UINT32_C(0xdaf26b);
}

bool verify_draw(std::uint32_t initial_seed, std::uint32_t synchronized_seed,
                 std::uint32_t ordinary_calls, std::uint32_t synchronized_calls,
                 std::int32_t count) {
    dh2_random_state streams{{initial_seed, synchronized_seed},
                             {ordinary_calls, synchronized_calls}};
    Selection output{Status::invalid_argument, 1234};
    const auto result = select_uncached_slot(&streams, count, &output);
    const auto next_seed = source_next(initial_seed);
    const auto expected_index = next_seed % static_cast<std::uint32_t>(count);
    return check(result.status == Status::selected && output.status == Status::selected,
                 "positive count must select a slot") &&
           check(result.slot_index == static_cast<std::int32_t>(expected_index) &&
                     output.slot_index == result.slot_index,
                 "slot result differs from the source LCG modulo count") &&
           check(streams.seeds[0] == next_seed,
                 "selection must advance the caller-owned ordinary stream exactly once") &&
           check(streams.counters[0] == ordinary_calls + 1U,
                 "ordinary debug call counter must advance exactly once") &&
           check(streams.seeds[1] == synchronized_seed &&
                     streams.counters[1] == synchronized_calls,
                 "template selection must not touch the synchronized stream");
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::fprintf(stderr, "usage: character_template_random_host <cache-slot-count> <caller-seed>\n");
        return 2;
    }
    char* end = nullptr;
    const auto source_count = std::strtol(argv[1], &end, 10);
    if (!end || *end || source_count <= 0 || source_count > INT32_MAX) return 2;
    end = nullptr;
    const auto source_seed = std::strtoull(argv[2], &end, 0);
    if (!end || *end || source_seed > UINT32_MAX) return 2;

    bool ok = true;
    constexpr std::array<std::uint32_t, 8> seeds = {
        0U, 1U, 0xffffffffU, 0x80000000U,
        0x00daf26aU, 0x00daf26bU, 0x12345678U, 0xfedcba98U
    };
    constexpr std::array<std::int32_t, 7> counts = {
        1, 2, 5, 100, 32767, 1000000, INT32_MAX
    };
    std::uint32_t cases = 0;
    for (const auto seed : seeds) {
        for (const auto count : counts) {
            ok &= verify_draw(seed, seed ^ 0xa5a5a5a5U,
                              UINT32_MAX, 0x87654321U, count);
            ++cases;
        }
    }

    dh2_random_state streams{{0x12345678U, 0x87654321U}, {17U, 29U}};
    const auto before_empty = streams;
    Selection empty{Status::selected, 42};
    const auto empty_result = select_uncached_slot(&streams, 0, &empty);
    ok &= check(empty_result.status == Status::no_alternatives &&
                    empty.slot_index == -1 && empty.status == Status::no_alternatives,
                "zero source alternatives must report no selection");
    ok &= check(streams.seeds[0] == before_empty.seeds[0] &&
                    streams.seeds[1] == before_empty.seeds[1] &&
                    streams.counters[0] == before_empty.counters[0] &&
                    streams.counters[1] == before_empty.counters[1],
                "empty template branch must not mutate either stream or counter");

    streams = {{0x11223344U, 0xaabbccddU}, {5U, 6U}};
    Selection untouched{Status::selected, 73};
    auto saved = streams;
    const auto negative = select_uncached_slot(&streams, -1, &untouched);
    ok &= check(negative.status == Status::invalid_argument && untouched.slot_index == 73,
                "negative source count must fail without overwriting output");
    ok &= check(streams.seeds[0] == saved.seeds[0] && streams.seeds[1] == saved.seeds[1] &&
                    streams.counters[0] == saved.counters[0] &&
                    streams.counters[1] == saved.counters[1],
                "negative source count must fail before state mutation");
    ok &= check(select_uncached_slot(nullptr, 5, &untouched).status == Status::invalid_argument,
                "missing caller-owned stream must be rejected");
    ok &= check(select_uncached_slot(&streams, 5, nullptr).status == Status::invalid_argument,
                "missing output must be rejected");
    auto* alias = reinterpret_cast<Selection*>(&streams);
    saved = streams;
    ok &= check(select_uncached_slot(&streams, 5, alias).status == Status::invalid_argument,
                "overlapping stream and output must be rejected");
    ok &= check(streams.seeds[0] == saved.seeds[0] && streams.seeds[1] == saved.seeds[1] &&
                    streams.counters[0] == saved.counters[0] &&
                    streams.counters[1] == saved.counters[1],
                "alias rejection must leave stream untouched");

    streams = {{static_cast<std::uint32_t>(source_seed), 0x76543210U}, {0U, 0U}};
    Selection source_selection{};
    const auto source_result = select_uncached_slot(
        &streams, static_cast<std::int32_t>(source_count), &source_selection);
    ok &= check(source_result.status == Status::selected &&
                    source_result.slot_index >= 0 && source_result.slot_index < source_count,
                "cache template slot count must return an in-range source index");
    if (!ok) return 1;
    std::printf("character template random host checks passed: %u source cases; fixture slot=%d (caller seed=%u)\n",
                cases, source_result.slot_index, static_cast<std::uint32_t>(source_seed));
    return 0;
}
