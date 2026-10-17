#include "../character_update_script_scheduler.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh2::character_update_script_scheduler;

namespace {
struct Fixture {
    const char* name;
    std::uint32_t state = 1;
    std::uint8_t field_1480 = 0;
    std::uintptr_t active_ais_3e4 = 0;
    std::int32_t level_oid = 7;
    std::uint32_t map_count = 0;
    std::uint32_t load_result = 0;
    std::uint32_t monster = 1;
    std::uint32_t mini = 0;
    std::uint32_t boss = 0;
    std::uint32_t online = 0;
    std::uint8_t field_3ec = 1;
    std::uint32_t timestamp = 100;
};

struct Context {
    std::int32_t level_oid;
    std::uint32_t load_result, monster, mini, boss, online, timestamp;
    std::vector<std::uint32_t> calls;
    std::uint32_t fail_operation = UINT32_MAX;
    bool fail_after_effect = false;
    bool kill_mutates_map = false;
    std::uint32_t kill_effects = 0;
    std::uint32_t unload_effects = 0;
    std::uint32_t load_effects = 0;
    Character* expected_current = nullptr;
    std::uint32_t expected_eviction = 0;
    Character* nested_character = nullptr;
    ConcurrentMap* nested_map = nullptr;
    const Services* nested_services = nullptr;
    Result* nested_result = nullptr;
    Status nested_status = Status::complete;
    bool try_reentry = false;
};

std::int32_t invoke(void* opaque, Character*, ConcurrentMap* map,
                   const Request* request, Response* response) {
    auto& c = *static_cast<Context*>(opaque);
    const auto op = static_cast<std::uint32_t>(request->operation);
    c.calls.push_back(op);
    if (c.try_reentry && op == static_cast<std::uint32_t>(Operation::current_level)) {
        c.try_reentry = false;
        c.nested_result->decision = Decision::timestamp_replaced;
        c.nested_status = run(c.nested_character, c.nested_map,
                              c.nested_services, c.nested_result);
    }
    if (op == c.fail_operation && !c.fail_after_effect) return 1;
    switch (request->operation) {
        case Operation::current_level:
            response->identity = 0x991100u;
            response->raw = static_cast<std::uint32_t>(c.level_oid);
            break;
        case Operation::controller_kill:
            if (!request->subject_character || request->argument0 != 0 ||
                request->argument1 != 1 || !request->subject_identity)
                throw std::runtime_error("Cmd_Kill source arguments changed");
            ++c.kill_effects;
            if (c.kill_mutates_map && map && map->size != 0) {
                for (std::uint32_t i = 1; i < map->size; ++i)
                    map->entries[i - 1] = map->entries[i];
                --map->size;
            }
            break;
        case Operation::unload_script_process:
            if (!request->subject_character || request->argument1 != 1 ||
                !request->subject_identity)
                throw std::runtime_error("UnLoadScriptProcess args changed");
            if (!map || map->size == 0 ||
                map->entries[0].character != request->subject_character ||
                static_cast<std::uint32_t>(map->entries[0].key) != request->argument0)
                throw std::runtime_error("unload iterator no longer names map begin");
            for (std::uint32_t i = 1; i < map->size; ++i)
                map->entries[i - 1] = map->entries[i];
            --map->size;
            ++c.unload_effects;
            break;
        case Operation::load_n_init_script_process:
            if (request->subject_character != c.expected_current ||
                request->argument0 != 1)
                throw std::runtime_error("LoadNInitScriptProcess args changed");
            ++c.load_effects;
            response->raw = c.load_result;
            break;
        case Operation::is_monster: response->raw = c.monster; break;
        case Operation::is_mini_boss: response->raw = c.mini; break;
        case Operation::is_boss: response->raw = c.boss; break;
        case Operation::get_online_byte:
            response->identity = 0x992200u;
            response->raw = c.online;
            break;
        case Operation::real_time_ms: response->raw = c.timestamp; break;
    }
    if (op == c.fail_operation) return 1;
    return 0;
}

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

void print(const Fixture& f, Status status, const Result& result,
           const ConcurrentMap& map, const std::vector<std::uint32_t>& calls) {
    std::cout << "{\"name\":\"" << f.name << "\",\"status\":"
              << static_cast<int>(status) << ",\"decision\":"
              << static_cast<std::uint32_t>(result.decision)
              << ",\"service_calls\":" << result.service_calls
              << ",\"map_size\":" << map.size
              << ",\"eviction_threshold\":" << result.eviction_threshold
              << ",\"eviction_attempted\":" << result.eviction_attempted
              << ",\"evicted_key\":" << result.evicted_key
              << ",\"evicted_character\":" << result.evicted_character
              << ",\"timestamp_word\":" << result.timestamp_word
              << ",\"timestamp_key\":" << result.timestamp_key
              << ",\"calls\":[";
    for (std::size_t i = 0; i < calls.size(); ++i) {
        if (i) std::cout << ',';
        std::cout << calls[i];
    }
    std::cout << "],\"entries\":[";
    for (std::uint32_t i = 0; i < map.size; ++i) {
        if (i) std::cout << ',';
        std::cout << '[' << map.entries[i].key << ','
                  << map.entries[i].character->identity << ']';
    }
    std::cout << "]}\n";
}

void run_case(const Fixture& f) {
    std::array<Character, 32> characters{};
    for (std::uint32_t i = 0; i < characters.size(); ++i) {
        characters[i] = {0x10000u + i, 0x20000u + i, 0x30000u + i,
                         1, 0, 0, 1, {0, 0}};
    }
    Character current{0x1000, 0x2000, 0x3000, f.state, f.active_ais_3e4,
                      f.field_1480, f.field_3ec, {0, 0}};
    std::array<Entry, 32> entries{};
    for (std::uint32_t i = 0; i < f.map_count; ++i)
        entries[i] = {-100 + static_cast<std::int32_t>(i), &characters[i]};
    if (std::string(f.name) == "timestamp_duplicate")
        entries[0] = {static_cast<std::int32_t>(f.timestamp), &characters[0]};
    ConcurrentMap map{entries.data(), f.map_count,
                      static_cast<std::uint32_t>(entries.size()), sizeof(entries)};
    Context context{};
    context.level_oid = f.level_oid;
    context.load_result = f.load_result;
    context.monster = f.monster;
    context.mini = f.mini;
    context.boss = f.boss;
    context.online = f.online;
    context.timestamp = f.timestamp;
    context.expected_current = &current;
    const Services services{&context, sizeof(context), &invoke};
    Result result{};
    const Status status = run(&current, &map, &services, &result);

    if (std::string(f.name) == "state_12")
        require(status == Status::complete && result.service_calls == 0,
                "state12 must bypass the bounded block");
    if (std::string(f.name) == "state_2")
        require(status == Status::complete && result.service_calls == 0,
                "state2 must skip lazy loading");
    if (std::string(f.name) == "oid29_threshold")
        require(result.eviction_attempted == 0 && map.size == 24,
                "OID29 size 24 must not evict");
    if (std::string(f.name) == "oid29_over_threshold")
        require(result.eviction_attempted == 1 && map.size == 24,
                "OID29 size 25 must evict exactly one");
    if (std::string(f.name) == "ordinary_threshold")
        require(result.eviction_attempted == 0 && map.size == 8,
                "ordinary size 8 must not evict");
    if (std::string(f.name) == "ordinary_over_threshold")
        require(result.eviction_attempted == 1 && map.size == 8,
                "ordinary size 9 must evict exactly one");
    if (std::string(f.name) == "timestamp_duplicate")
        require(status == Status::complete && map.size == 1 &&
                map.entries[0].character == &current,
                "duplicate timestamp must replace mapped Character");
    if (std::string(f.name) == "timestamp_high_bit")
        require(status == Status::complete && map.size == 1 &&
                map.entries[0].key == -268435456,
                "real-time key must retain signed int32 interpretation");
    print(f, status, result, map, context.calls);
}

void run_guards() {
    Character current{0x1000, 0x2000, 0x3000, 1, 0, 0, 1, {0, 0}};
    std::array<Entry, 2> entries{};
    ConcurrentMap map{entries.data(), 0, static_cast<std::uint32_t>(entries.size()),
                      sizeof(entries)};
    Context context{};
    context.level_oid = 7;
    context.load_result = 1;
    context.monster = 1;
    context.expected_current = &current;
    Services services{&context, sizeof(context), &invoke};
    Result result{};

    require(run(reinterpret_cast<Character*>(&map), &map, &services, &result) ==
                Status::invalid_argument,
            "overlapping Character/map descriptors must be rejected");
    require(run(&current, &map, &services, reinterpret_cast<Result*>(&current)) ==
                Status::invalid_argument,
            "output must not overlap live Character facts");
    require(run(&current, &map, &services, reinterpret_cast<Result*>(&map)) ==
                Status::invalid_argument,
            "output must not overlap map descriptor");
    require(run(&current, &map, &services,
                reinterpret_cast<Result*>(&services)) == Status::invalid_argument,
            "output must not overlap service descriptor");
    require(run(&current, &map, &services,
                reinterpret_cast<Result*>(entries.data())) == Status::invalid_argument,
            "output must not overlap entry backing");

    map.entries = reinterpret_cast<Entry*>(&current);
    map.capacity = 1;
    map.entries_extent = sizeof(Entry);
    require(run(&current, &map, &services, &result) == Status::invalid_argument,
            "entry backing must not overlap Character facts");
    map.entries = entries.data();
    map.capacity = static_cast<std::uint32_t>(entries.size());

    alignas(Character) unsigned char misaligned_storage[sizeof(Character) + 1]{};
    auto* misaligned_character = reinterpret_cast<Character*>(misaligned_storage + 1);
    require(run(misaligned_character, &map, &services, &result) == Status::invalid_argument,
            "misaligned Character must be rejected before field access");

    alignas(Entry) unsigned char backing[sizeof(Entry) + 1]{};
    map.entries = reinterpret_cast<Entry*>(backing + 1);
    map.capacity = 1;
    map.entries_extent = sizeof(Entry);
    require(run(&current, &map, &services, &result) == Status::invalid_argument,
            "misaligned entry backing must be rejected before element access");
    map.entries = entries.data();
    map.capacity = static_cast<std::uint32_t>(entries.size());
    map.entries_extent = sizeof(entries);
    map.entries_extent -= sizeof(Entry);
    require(run(&current, &map, &services, &result) == Status::invalid_argument,
            "capacity extent mismatch must be rejected before element reads");
    map.entries_extent = sizeof(entries);

    auto* partial_alias = reinterpret_cast<Character*>(
        reinterpret_cast<unsigned char*>(&current) + sizeof(std::uintptr_t));
    map.entries = reinterpret_cast<Entry*>(partial_alias);
    map.capacity = 1;
    map.entries_extent = sizeof(Entry);
    require(run(&current, &map, &services, &result) == Status::invalid_argument,
            "partial Character/backing overlap must be rejected");
    map.entries = entries.data();
    map.capacity = static_cast<std::uint32_t>(entries.size());
    map.entries_extent = sizeof(entries);

    services.context = &current;
    services.context_extent = sizeof(current);
    require(run(&current, &map, &services, &result) == Status::invalid_argument,
            "provider context must not overlap live Character facts");
    services.context = &result;
    services.context_extent = sizeof(result);
    require(run(&current, &map, &services, &result) == Status::invalid_argument,
            "provider context must not overlap output");
    services.context = reinterpret_cast<void*>(UINTPTR_MAX - 3u);
    services.context_extent = 8;
    require(run(&current, &map, &services, &result) == Status::invalid_argument,
            "overflowing provider extent must be rejected before dereference");
    services.context = &context;
    services.context_extent = sizeof(context);

    Character active_high{0x4000, 0x5000, 0x6000, 1,
                          static_cast<std::uintptr_t>(0x100000000ull),
                          0, 1, {0, 0}};
    context.calls.clear();
    require(run(&active_high, &map, &services, &result) == Status::complete &&
                result.decision == Decision::field_3e4_skip &&
                result.service_calls == 0 && context.calls.empty(),
            "full-width nonzero active AIS identity must take the +0x3e4 source gate");

    context.nested_character = &current;
    context.nested_map = &map;
    context.nested_services = &services;
    Result nested_result{};
    nested_result.decision = Decision::timestamp_replaced;
    context.nested_result = &nested_result;
    context.try_reentry = true;
    require(run(&current, &map, &services, &result) == Status::complete &&
                context.nested_status == Status::reentrant_call &&
                nested_result.decision == Decision::timestamp_replaced,
            "same-character/map callback reentry must be rejected without output mutation");
    std::cerr << "scheduler_guard_cases=15\n";
}

void run_effect_cases() {
    std::array<Character, 12> characters{};
    for (std::uint32_t i = 0; i < characters.size(); ++i)
        characters[i] = {0x11000u + i, 0x21000u + i, 0x31000u + i,
                          1, 0, 0, 1, {0, 0}};
    Character current{0x1000, 0x2000, 0x3000, 1, 0, 0, 1, {0, 0}};
    std::array<Entry, 16> entries{};
    for (std::uint32_t i = 0; i < 9; ++i)
        entries[i] = {-100 + static_cast<std::int32_t>(i), &characters[i]};
    ConcurrentMap map{entries.data(), 9,
                      static_cast<std::uint32_t>(entries.size()), sizeof(entries)};
    Context context{};
    context.level_oid = 7;
    context.load_result = 1;
    context.monster = 1;
    context.expected_current = &current;
    context.fail_operation = static_cast<std::uint32_t>(Operation::unload_script_process);
    context.fail_after_effect = true;
    Services services{&context, sizeof(context), &invoke};
    Result result{};
    auto status = run(&current, &map, &services, &result);
    require(status == Status::service_failed && context.kill_effects == 1 &&
                context.unload_effects == 1 && map.size == 8 &&
                context.calls == std::vector<std::uint32_t>({0, 1, 2}),
            "completed kill/unload effects must remain after unload adapter failure");

    entries = {};
    map = {entries.data(), 0, static_cast<std::uint32_t>(entries.size()), sizeof(entries)};
    context = {};
    context.level_oid = 7;
    context.load_result = 1;
    context.monster = 1;
    context.expected_current = &current;
    context.fail_operation = static_cast<std::uint32_t>(Operation::load_n_init_script_process);
    context.fail_after_effect = true;
    services = {&context, sizeof(context), &invoke};
    status = run(&current, &map, &services, &result);
    require(status == Status::service_failed && context.load_effects == 1 &&
                map.size == 0 &&
                context.calls == std::vector<std::uint32_t>({0, 3}),
            "completed LoadNInit effects must remain after adapter failure");

    for (std::uint32_t i = 0; i < 9; ++i)
        entries[i] = {-100 + static_cast<std::int32_t>(i), &characters[i]};
    map = {entries.data(), 9, static_cast<std::uint32_t>(entries.size()), sizeof(entries)};
    context = {};
    context.level_oid = 7;
    context.load_result = 1;
    context.monster = 1;
    context.expected_current = &current;
    context.kill_mutates_map = true;
    services = {&context, sizeof(context), &invoke};
    status = run(&current, &map, &services, &result);
    require(status == Status::invalid_source_fact && context.kill_effects == 1 &&
                context.unload_effects == 0 && map.size == 8 &&
                context.calls == std::vector<std::uint32_t>({0, 1}),
            "map mutation during Cmd_Kill must retain the kill and refuse stale unload iterator");

    std::cerr << "scheduler_effect_cases=3\n";
}

}  // namespace

int main() {
    const std::array<Fixture, 18> fixtures{{
        {"state_12", 12}, {"state_2", 2},
        {"field_1480", 1, 1}, {"field_3e4", 1, 0, 1},
        {"oid29_threshold", 1, 0, 0, 29, 24},
        {"oid29_over_threshold", 1, 0, 0, 29, 25},
        {"ordinary_threshold", 1, 0, 0, 7, 8},
        {"ordinary_over_threshold", 1, 0, 0, 7, 9},
        {"load_failed", 1, 0, 0, 7, 0, 0},
        {"not_monster", 1, 0, 0, 7, 0, 1, 0},
        {"mini_boss", 1, 0, 0, 7, 0, 1, 1, 0x80},
        {"boss", 1, 0, 0, 7, 0, 1, 1, 0, 1},
        {"online_byte", 1, 0, 0, 7, 0, 1, 1, 0, 0, 0x80},
        {"field_3ec_zero", 1, 0, 0, 7, 0, 1, 1, 0, 0, 0, 0},
        {"timestamp_high_bit", 1, 0, 0, 7, 0, 1, 1, 0, 0, 0, 1, 0xf0000000},
        {"timestamp_duplicate", 1, 0, 0, 7, 1, 1, 1, 0, 0, 0, 1, 100},
        {"timestamp_insert_after_existing", 1, 0, 0, 7, 1, 1, 1, 0, 0, 0, 1, 200},
        {"oid29_non_special_level", 1, 0, 0, 28, 9, 0},
    }};
    try {
        for (const auto& fixture : fixtures) run_case(fixture);
        run_guards();
        run_effect_cases();
        std::cerr << "host_cases=" << fixtures.size() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}
