#include "../source_random_lifecycle_v1.hpp"

#include <cstdint>
#include <cstdio>
#include <string>

namespace {

bool check(bool condition, const char* message) {
    if (condition) return true;
    std::fprintf(stderr, "FAIL: %s\n", message);
    return false;
}

}  // namespace

int main() {
    using dh2::random_lifecycle::process_state;
    using dh2::random_lifecycle::seed_from_gsinit_update;
    using dh2::random_lifecycle::seed_from_level_unload;

    auto& streams = process_state();
    auto* const owner = &streams;
    bool ok = true;

    streams = {{0x11111111U, 0x22222222U}, {0xabcdef01U, 0x80000002U}};
    seed_from_gsinit_update(0xfedcba98U);
    ok &= check(&process_state() == owner, "state accessor must return one stable owner");
    ok &= check(streams.seeds[0] == 0xfedcba98U && streams.seeds[1] == 0,
                 "GSInit update must write time to ordinary seed and clear synchronized seed");
    ok &= check(streams.counters[0] == 0xabcdef01U && streams.counters[1] == 0x80000002U,
                 "GSInit reseed must preserve both debug counters");

    // Deliberately change only the counters between source lifecycle events;
    // unload must retain those live values while replacing both seeds.
    streams.counters[0] = UINT32_MAX;
    streams.counters[1] = 0x76543210U;
    seed_from_level_unload(0x01234567U);
    ok &= check(&process_state() == owner, "unload must use the same process state object");
    ok &= check(streams.seeds[0] == 0x01234567U && streams.seeds[1] == 0,
                 "Level unload must write time to ordinary seed and clear synchronized seed");
    ok &= check(streams.counters[0] == UINT32_MAX && streams.counters[1] == 0x76543210U,
                 "Level unload reseed must preserve both debug counters");

    // uint32_t time values, including zero and the maximum, are source words;
    // reseeding is not an RNG draw and therefore must not advance counters.
    seed_from_gsinit_update(0);
    ok &= check(streams.seeds[0] == 0 && streams.seeds[1] == 0 &&
                    streams.counters[0] == UINT32_MAX && streams.counters[1] == 0x76543210U,
                 "zero-valued timer seed must be accepted without counter changes");
    seed_from_level_unload(UINT32_MAX);
    ok &= check(streams.seeds[0] == UINT32_MAX && streams.seeds[1] == 0 &&
                    streams.counters[0] == UINT32_MAX && streams.counters[1] == 0x76543210U,
                 "maximum timer seed must be accepted without counter changes");

    // V4 inventory and V7 powered-loot APIs share this borrowed descriptor.
    // Each service draw advances the corresponding original stream once.
    const auto random = dh2::random_lifecycle::inventory_random_service();
    ok &= check(random.context == &streams && random.next != nullptr,
                "inventory/loot adapter must borrow the singleton process RNG");
    std::string error;
    std::int32_t value = -1;
    auto expected = streams;
    const auto expected_ordinary = dh2_random_next(&expected, 97, 0);
    ok &= check(random.next(random.context, 97, 0, value, error) &&
                    value == expected_ordinary && streams.seeds[0] == expected.seeds[0] &&
                    streams.counters[0] == expected.counters[0] &&
                    streams.seeds[1] == expected.seeds[1] &&
                    streams.counters[1] == expected.counters[1],
                "ordinary inventory draw must match and mutate only original stream zero");
    expected = streams;
    const auto expected_sync = dh2_random_next(&expected, 13, 1);
    ok &= check(random.next(random.context, 13, 2, value, error) &&
                    value == expected_sync && streams.seeds[0] == expected.seeds[0] &&
                    streams.counters[0] == expected.counters[0] &&
                    streams.seeds[1] == expected.seeds[1] &&
                    streams.counters[1] == expected.counters[1],
                "any nonzero source sync word must mutate only original stream one");
    expected = streams;
    const auto expected_zero = dh2_random_next(&expected, 0, 0);
    ok &= check(random.next(random.context, 0, 0, value, error) &&
                    value == expected_zero && streams.seeds[0] == expected.seeds[0] &&
                    streams.counters[0] == expected.counters[0] &&
                    streams.seeds[1] == expected.seeds[1] &&
                    streams.counters[1] == expected.counters[1],
                "zero-bound source draw must preserve value semantics and advance counter");
    expected = streams;
    const auto expected_negative = dh2_random_next(&expected, UINT32_MAX, 0);
    ok &= check(random.next(random.context, -1, 0, value, error) &&
                    value == expected_negative && streams.seeds[0] == expected.seeds[0] &&
                    streams.counters[0] == expected.counters[0] &&
                    streams.seeds[1] == expected.seeds[1] &&
                    streams.counters[1] == expected.counters[1],
                "signed bound words must retain the source uint32 conversion");
    const auto before_invalid = streams;
    dh2_random_state unrelated{};
    ok &= check(!random.next(&unrelated, 17, 0, value, error) && streams.seeds[0] == before_invalid.seeds[0] &&
                    streams.counters[0] == before_invalid.counters[0] &&
                    streams.seeds[1] == before_invalid.seeds[1] &&
                    streams.counters[1] == before_invalid.counters[1],
                "a foreign state must reject without touching the process RNG");

    if (!ok) return 1;
    std::puts("source RNG lifecycle selected-library checks passed: singleton identity, GSInit/unload seeds, counter preservation, shared inventory/loot descriptor draws on both streams, invalid-call preservation");
    return 0;
}
