#include "../lua_script_level_queries.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace dh2::lua_script_level_queries;

#if defined(_WIN32)
#define DH2_TEST_EXPORT __declspec(dllexport)
#else
#define DH2_TEST_EXPORT __attribute__((visibility("default")))
#endif

namespace {
constexpr std::uintptr_t kApplication = 0x1100000011ull;
constexpr std::uintptr_t kManager = 0x2200000022ull;
constexpr std::uintptr_t kPlayer = 0x3300000033ull;
constexpr std::uintptr_t kLevel = 0x4400000044ull;
constexpr std::uintptr_t kTableA = 0x5500000055ull;
constexpr std::uintptr_t kTableB = 0x6600000066ull;
constexpr std::uintptr_t kValue = 0x7700000077ull;

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    std::vector<std::string> trace;
    std::vector<std::int32_t> pushed;
    std::uintptr_t application = kApplication;
    std::uintptr_t manager = kManager;
    std::uintptr_t player = kPlayer;
    std::uintptr_t level = kLevel;
    std::uintptr_t table_object = kTableA;
    std::uintptr_t table_storage = kTableA;
    std::int32_t player_level = 42;
    std::int32_t oid = 7;
    std::int32_t level_difficulty = 2;
    float argument_number = 1.0f;
    std::int32_t min_a = 11, max_a = 21;
    std::int32_t min_b = 12, max_b = 22;
    bool distinct_range_values = false;
    bool mutate_table_on_first_push = false;
    bool mutate_oid_on_get_number = false;
    bool mutate_argument_on_current_level = false;
    bool fail_max_word_read = false;
    bool throw_on_table_word = false;
    bool fail_hosting_player = false;
    Arguments* live_arguments = nullptr;
    std::uint32_t tables_read = 0;
    std::uint32_t row_offset = 0;
    std::uint32_t min_field = 0, max_field = 0;
};

std::int32_t get_application(void* raw, std::uintptr_t* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("application");
    *out = f.application; return 0;
}
std::int32_t get_manager(void* raw, std::uintptr_t app, std::uintptr_t* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("application+40");
    require(app == f.application, "manager lookup did not use captured Application");
    *out = f.manager; return 0;
}
std::int32_t get_hosting_player(void* raw, std::uintptr_t manager, std::uintptr_t* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("GetHostingPlayer");
    require(manager == f.manager, "GetHostingPlayer receiver changed");
    *out = f.fail_hosting_player ? 0 : f.player; return 0;
}
std::int32_t read_player_word(void* raw, std::uintptr_t player, std::uint32_t offset,
                              std::int32_t* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("player.word");
    require(player == f.player && offset == 0x330, "HostPlayerLevel field or owner changed");
    *out = f.player_level; return 0;
}
std::int32_t get_current_level(void* raw, std::uintptr_t* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("GetCurrentLevel");
    if (f.mutate_argument_on_current_level && f.live_arguments && f.live_arguments->values)
        f.live_arguments->values[0] = {kValue, 3, 0};
    *out = f.level; return 0;
}
std::int32_t read_level_word(void* raw, std::uintptr_t level, std::uint32_t offset,
                             std::int32_t* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("level.word");
    require(level == f.level, "level field used a different Level identity");
    if (offset == 0x3c) *out = f.oid;
    else if (offset == 0x118) *out = f.level_difficulty;
    else throw std::runtime_error("unexpected Level field offset");
    return 0;
}
std::int32_t value_get_number(void* raw, std::uintptr_t value, float* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("Value.getNumber");
    require(value == kValue, "range did not use the captured first Value");
    if (f.mutate_oid_on_get_number) f.oid = 99;
    *out = f.argument_number; return 0;
}
std::int32_t float_to_int(void* raw, float number, std::int32_t* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("__aeabi_f2iz");
    require(std::isfinite(number) && number >= -100000.0f && number <= 100000.0f,
            "host fixture conversion domain changed");
    *out = static_cast<std::int32_t>(number); return 0;
}
std::int32_t get_level_table(void* raw, std::uintptr_t* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("LevelTable");
    ++f.tables_read;
    *out = f.table_object; return 0;
}
std::int32_t read_level_table_word(void* raw, std::uintptr_t table,
                                   std::uint32_t row_offset, std::uint32_t field_offset,
                                   std::int32_t* out) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("LevelTable.word");
    if (f.throw_on_table_word) throw std::runtime_error("table read exception");
    require(table == f.table_object, "table field read did not use captured table object");
    f.row_offset = row_offset;
    if (field_offset == 0x3c || field_offset == 0x40 || field_offset == 0x44) {
        f.min_field = field_offset;
        const auto index = field_offset == 0x3c ? 0 : field_offset == 0x40 ? 1 : 2;
        *out = f.table_storage == kTableA ? f.min_a : f.min_b;
        if (f.distinct_range_values) *out += index * 100;
    } else if (field_offset == 0x30 || field_offset == 0x34 || field_offset == 0x38) {
        if (f.fail_max_word_read) return 1;
        f.max_field = field_offset;
        const auto index = field_offset == 0x30 ? 0 : field_offset == 0x34 ? 1 : 2;
        *out = f.table_storage == kTableA ? f.max_a : f.max_b;
        if (f.distinct_range_values) *out += index * 100;
    } else throw std::runtime_error("unexpected level table word offset");
    return 0;
}
std::int32_t push_integer(void* raw, std::int32_t value) {
    auto& f = *static_cast<Fixture*>(raw); f.trace.emplace_back("pushInteger");
    f.pushed.push_back(value);
    if (f.mutate_table_on_first_push && f.pushed.size() == 1) f.table_storage = kTableB;
    return 0;
}

Services services(Fixture& f) {
    return {&f, get_application, get_manager, get_hosting_player, read_player_word,
            get_current_level, read_level_word, value_get_number, float_to_int,
            get_level_table, read_level_table_word, push_integer};
}

extern "C" DH2_TEST_EXPORT std::uint32_t dh2_lua_script_range_probe(
    std::int32_t oid, std::uint32_t argument_type, float argument_number,
    std::int32_t min_a, std::int32_t max_a, std::int32_t min_b,
    std::int32_t max_b, std::uint32_t mutate_storage_after_min,
    std::int32_t* values, std::uint32_t* count) {
    if (!values || !count) return static_cast<std::uint32_t>(Status::invalid_argument);
    Fixture f;
    f.oid = oid;
    f.argument_number = argument_number;
    f.min_a = min_a; f.max_a = max_a; f.min_b = min_b; f.max_b = max_b;
    f.distinct_range_values = true;
    f.mutate_table_on_first_push = mutate_storage_after_min != 0;
    Argument front{kValue, argument_type, 0};
    Arguments args{argument_type ? &front : nullptr, argument_type ? 1U : 0U, 0};
    f.live_arguments = &args;
    auto s = services(f); Result result{};
    const auto status = get_current_level_range(&args, &s, &result);
    *count = result.values_pushed;
    for (std::uint32_t i = 0; i < result.values_pushed; ++i) values[i] = result.values[i];
    return static_cast<std::uint32_t>(status);
}

extern "C" DH2_TEST_EXPORT std::uint32_t dh2_lua_script_host_level_probe(
    std::int32_t player_level, std::int32_t* value) {
    if (!value) return static_cast<std::uint32_t>(Status::invalid_argument);
    Fixture f; f.player_level = player_level;
    auto s = services(f); Result result{};
    const auto status = get_host_player_level(&s, &result);
    if (result.values_pushed) *value = result.values[0];
    return static_cast<std::uint32_t>(status);
}

void same_trace(const Fixture& f, std::initializer_list<const char*> expected) {
    std::vector<std::string> strings;
    for (const char* item : expected) strings.emplace_back(item);
    require(f.trace == strings, "source service order changed");
}

void host_cases(std::uint32_t& cases) {
    {
        Fixture f; auto s = services(f); Result result{};
        require(get_host_player_level(&s, &result) == Status::complete &&
                    result.output_path == OutputPath::host_player_level &&
                    result.values_pushed == 1 && result.values[0] == 42 &&
                    f.pushed == std::vector<std::int32_t>{42},
                "HostPlayerLevel result differs");
        same_trace(f, {"application", "application+40", "GetHostingPlayer", "player.word", "pushInteger"});
        ++cases;
    }
    {
        Fixture f; f.fail_hosting_player = true; auto s = services(f); Result result{};
        require(get_host_player_level(&s, &result) == Status::service_failed &&
                    result.values_pushed == 0 && f.pushed.empty(),
                "missing host must fail without inventing a level");
        same_trace(f, {"application", "application+40", "GetHostingPlayer"});
        ++cases;
    }
    {
        Fixture f; f.level_difficulty = 2; auto s = services(f); Result result{};
        require(get_host_player_difficulty(&s, &result) == Status::complete &&
                    result.values_pushed == 1 && result.values[0] == 2,
                "current-level difficulty result differs");
        same_trace(f, {"GetCurrentLevel", "level.word", "pushInteger"});
        ++cases;
    }
    {
        Fixture f; f.level = 0; auto s = services(f); Result result{};
        require(get_host_player_difficulty(&s, &result) == Status::complete &&
                    result.values_pushed == 1 && result.values[0] == 0,
                "null current level must return the source zero fallback");
        same_trace(f, {"GetCurrentLevel", "pushInteger"});
        ++cases;
    }
}

void range_cases(std::uint32_t& cases) {
    {
        Fixture f; f.oid = 7; f.mutate_table_on_first_push = true;
        Arguments args{nullptr, 0, 0}; f.live_arguments = &args;
        auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::complete &&
                    result.output_path == OutputPath::selected_level_range &&
                    result.values_pushed == 2 && result.values[0] == 11 && result.values[1] == 22 &&
                    f.tables_read == 1 &&
                    f.min_field == 0x3c && f.max_field == 0x30 && f.row_offset == 7U * 72U,
                "default difficulty range or fresh backing-store read differs");
        same_trace(f, {"GetCurrentLevel", "level.word", "LevelTable", "LevelTable.word",
                       "pushInteger", "LevelTable.word", "pushInteger"});
        ++cases;
    }
    {
        Fixture f; f.oid = -1; Arguments args{nullptr, 1, 0}; auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::complete &&
                    result.output_path == OutputPath::sentinel_level &&
                    result.values_pushed == 2 && result.values[0] == -1 && result.values[1] == -1 &&
                    f.tables_read == 0,
                "OID -1 sentinel must bypass malformed arguments and table reads");
        same_trace(f, {"GetCurrentLevel", "level.word", "pushInteger", "pushInteger"});
        ++cases;
    }
    {
        Fixture f; f.oid = 4; f.argument_number = 1.9f;
        Argument first{kValue, 3, 0}; Arguments args{&first, 1, 0}; f.live_arguments = &args;
        auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::complete &&
                    result.selected_difficulty == 1 && result.values[0] == f.min_a &&
                    result.values[1] == f.max_a && f.min_field == 0x40 && f.max_field == 0x34,
                "numeric difficulty must use one Value.getNumber/f2iz truncation and hard fields");
        same_trace(f, {"GetCurrentLevel", "level.word", "Value.getNumber", "__aeabi_f2iz",
                       "LevelTable", "LevelTable.word", "pushInteger", "LevelTable.word",
                       "pushInteger"});
        ++cases;
    }
    {
        Fixture f; f.argument_number = 2.0f;
        Argument first{kValue, 3, 0}; Arguments args{&first, 1, 0}; auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::complete &&
                    result.selected_difficulty == 2 && f.min_field == 0x44 && f.max_field == 0x38,
                "very-hard table offsets differ");
        ++cases;
    }
    {
        Fixture f; Argument first{kValue, 4, 0}; Arguments args{&first, 1, 0};
        auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::complete &&
                    result.selected_difficulty == 0 && result.values_pushed == 2 &&
                    f.trace.end() == std::find(f.trace.begin(), f.trace.end(), "Value.getNumber"),
                "non-number difficulty must use normal fields without numeric conversion");
        ++cases;
    }
    {
        Fixture f; f.argument_number = 3.0f;
        Argument first{kValue, 3, 0}; Arguments args{&first, 1, 0}; auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::complete &&
                    result.selected_difficulty == 3 && result.values_pushed == 0 && f.tables_read == 0,
                "unsupported converted difficulty must return no values before table access");
        ++cases;
    }
    {
        Fixture f; f.mutate_argument_on_current_level = true;
        Argument first{kValue, 4, 0}; Arguments args{&first, 1, 0}; f.live_arguments = &args;
        f.argument_number = 2.0f; auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::complete &&
                    result.selected_difficulty == 2 && f.min_field == 0x44,
                "arguments must be read live after the current-level getter");
        ++cases;
    }
    {
        Fixture f; f.mutate_oid_on_get_number = true; f.oid = 7; f.argument_number = 1.0f;
        Argument first{kValue, 3, 0}; Arguments args{&first, 1, 0}; auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::complete &&
                    result.level_oid == 7 && result.row_byte_offset == 7U * 72U && f.row_offset == 7U * 72U,
                "OID must remain captured before Value.getNumber mutates its producer");
        ++cases;
    }
    {
        Fixture f; f.oid = 0x10000000; Arguments args{nullptr, 0, 0}; auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::complete &&
                    result.row_byte_offset == 0x80000000U && f.row_offset == 0x80000000U,
                "row offset must wrap as an ARM32 uint32 multiplication");
        ++cases;
    }
    {
        Fixture f; f.level = 0; Arguments args{nullptr, 0, 0}; auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::service_failed &&
                    result.values_pushed == 0 && f.trace == std::vector<std::string>{"GetCurrentLevel"},
                "missing current Level must fail closed without a fabricated range");
        ++cases;
    }
    {
        Fixture f; f.fail_max_word_read = true; Arguments args{nullptr, 0, 0};
        auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::service_failed &&
                    result.values_pushed == 1 && result.values[0] == f.min_a && f.pushed.size() == 1,
                "max-field failure must retain the already-pushed minimum");
        ++cases;
    }
    {
        Fixture f; f.throw_on_table_word = true; Arguments args{nullptr, 0, 0};
        auto s = services(f); Result result{};
        require(get_current_level_range(&args, &s, &result) == Status::service_exception &&
                    result.values_pushed == 0 && f.pushed.empty(),
                "provider exception must stop at its source boundary");
        ++cases;
    }
    {
        Fixture f; Arguments args{nullptr, 0, 0}; auto s = services(f);
        auto* result = reinterpret_cast<Result*>(&args);
        require(get_current_level_range(&args, &s, result) == Status::invalid_argument &&
                    f.trace.empty(), "overlapping controls must reject before source calls");
        ++cases;
    }
}
}  // namespace

int main() {
    try {
        std::uint32_t cases = 0;
        host_cases(cases);
        range_cases(cases);
        std::printf("{\"lua_script_level_query_cases\":%u,\"host_player_level_order\":true,"
                    "\"current_level_difficulty_null_zero\":true,\"range_order_and_table_reread\":true,"
                    "\"captured_oid_and_argument_order\":true,\"arm32_row_offset_wrap\":true,"
                    "\"provider_fail_closed\":true,\"mismatches\":0}\n", cases);
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "LuaScript level queries: %s\n", error.what());
        return 1;
    }
}
