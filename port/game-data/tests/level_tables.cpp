#include "../level_tables.hpp"
#include "../../level-world/lua_script_level_queries.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace data = dh2::data;
namespace query = dh2::lua_script_level_queries;

void require(bool value, const char* message) { if (!value) throw std::runtime_error(message); }
std::vector<std::uint8_t> read(const char* path) {
    std::ifstream stream(path, std::ios::binary);
    require(bool(stream), "cannot read input");
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}
data::Bytes bytes(const std::vector<std::uint8_t>& value) { return {value.data(), value.size()}; }
void quote(const std::string& value) {
    std::cout << '"';
    const char hex[] = "0123456789abcdef";
    for (unsigned char c : value) {
        if (c == '"' || c == '\\') std::cout << '\\' << char(c);
        else if (c < 32) std::cout << "\\u00" << hex[c >> 4] << hex[c & 15];
        else std::cout << char(c);
    }
    std::cout << '"';
}

struct Context {
    data::LevelTables* table;
    std::int32_t oid = 0;
    float difficulty = 0;
    std::vector<std::int32_t> pushed;
    static Context& self(void* raw) { return *static_cast<Context*>(raw); }
    static int current(void* raw, std::uintptr_t* output) {
        *output = reinterpret_cast<std::uintptr_t>(raw); return 0;
    }
    static int level_word(void* raw, std::uintptr_t identity, unsigned offset, std::int32_t* output) {
        require(identity == reinterpret_cast<std::uintptr_t>(raw) && offset == 0x3c, "wrong current level word");
        *output = self(raw).oid; return 0;
    }
    static int number(void* raw, std::uintptr_t identity, float* output) {
        auto& c = self(raw);
        require(identity == reinterpret_cast<std::uintptr_t>(&c.difficulty), "wrong numeric identity");
        *output = c.difficulty; return 0;
    }
    static int convert(void*, float input, std::int32_t* output) {
        if (!std::isfinite(input) || input < -2147483648.0f || input >= 2147483648.0f) return 1;
        *output = static_cast<std::int32_t>(input); return 0;
    }
    static int table_id(void* raw, std::uintptr_t* output) {
        *output = reinterpret_cast<std::uintptr_t>(self(raw).table); return 0;
    }
    static int table_word(void* raw, std::uintptr_t identity, unsigned row, unsigned at, std::int32_t* output) {
        auto& c = self(raw);
        require(identity == reinterpret_cast<std::uintptr_t>(c.table), "table identity changed");
        return data::read_level_range_word(*c.table, row, at, *output) ? 0 : 1;
    }
    static int push(void* raw, std::int32_t value) { self(raw).pushed.push_back(value); return 0; }
    void range(unsigned row, unsigned mode) {
        oid = static_cast<std::int32_t>(row); difficulty = float(mode); pushed.clear();
        query::Argument value{reinterpret_cast<std::uintptr_t>(&difficulty), 3, 0};
        query::Arguments arguments{&value, 1, 0};
        query::Services services{};
        services.context = this; services.get_current_level = current; services.read_level_word = level_word;
        services.value_get_number = number; services.float_to_signed_int = convert;
        services.get_level_table = table_id; services.read_level_table_word = table_word; services.push_integer = push;
        query::Result result{};
        require(query::get_current_level_range(&arguments, &services, &result) == query::Status::complete,
                "range composition failed");
        require(pushed.size() == 2 && result.values_pushed == 2 && result.row_byte_offset == row * 72,
                "wrong range output shape");
    }
};

int main(int argc, char** argv) { try {
    require(argc == 4, "pass records/names/schema");
    const auto records = read(argv[1]), names = read(argv[2]), schema = read(argv[3]);
    data::LevelTables table;
    std::string error;
    require(data::load_levels(bytes(records), bytes(names), bytes(schema), table, error), error.c_str());
    require(table.fast_travel.size() == 33 && table.levels.size() == 51, "actual catalogue dimensions differ");
    require(data::find_level(table, "GOTHICUS_CRYPT_01") == 23 && data::find_level(table, "crypt01") == -1,
            "exact native level name lookup differs");
    const auto retained_name = table.levels.at(23).name;
    unsigned rejection_cases = 0;
    for (auto length : {0u, 3u, 4u, 8u, 32u, unsigned(records.size() - 1)}) {
        require(!data::load_levels({records.data(), length}, bytes(names), bytes(schema), table, error) &&
                table.levels.at(23).name == retained_name, "failed load replaced live catalogue");
        ++rejection_cases;
    }
    auto suffix = records; suffix.push_back(0);
    require(!data::load_levels(bytes(suffix), bytes(names), bytes(schema), table, error), "record suffix accepted");
    ++rejection_cases;
    auto count = records; count[0] = 0xff; count[1] = 0xff;
    require(!data::load_levels(bytes(count), bytes(names), bytes(schema), table, error), "excessive count accepted");
    ++rejection_cases;
    auto wrong_schema = schema; wrong_schema.back() ^= 1;
    require(!data::load_levels(bytes(records), bytes(names), bytes(wrong_schema), table, error), "changed schema accepted");
    ++rejection_cases;
    std::int32_t word = 123;
    for (auto pair : {std::pair<unsigned, unsigned>{1, 0x30}, {51 * 72, 0x30}, {23 * 72, 0x2c}}) {
        require(!data::read_level_range_word(table, pair.first, pair.second, word) && word == 123,
                "invalid ARM range offset changed output");
        ++rejection_cases;
    }
    std::cout << "{\"validation\":\"PASS\",\"rejection_cases\":" << rejection_cases << ",\"fast_travel\":[";
    bool first = true;
    for (const auto& row : table.fast_travel) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"name\":"; quote(row.name);
        std::cout << ",\"description_id\":" << row.description_id << ",\"entrypoint_id\":" << row.entrypoint_id;
        std::cout << ",\"level_name\":"; quote(row.level_name);
        std::cout << ",\"location_type\":" << row.location_type << ",\"string_id\":" << row.string_id << '}';
    }
    std::cout << "],\"levels\":["; first = true;
    for (const auto& row : table.levels) {
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"name\":"; quote(row.name);
        std::cout << ",\"dbg_is_stable\":" << (row.dbg_is_stable ? "true" : "false") << ",\"dynamic_bus_routing\":";
        quote(row.dynamic_bus_routing);
        std::cout << ",\"hub\":" << row.hub << ",\"is_random\":" << (row.is_random ? "true" : "false")
                  << ",\"level_description\":" << row.level_description << ",\"level_file\":";
        quote(row.level_file);
        std::cout << ",\"level_name_id\":" << row.level_name_id << ",\"level_state\":" << row.level_state
                  << ",\"map_name\":" << row.map_name << ",\"monster_lvl_max\":" << row.monster_lvl_max
                  << ",\"monster_lvl_max_hard\":" << row.monster_lvl_max_hard
                  << ",\"monster_lvl_max_nightmare\":" << row.monster_lvl_max_nightmare
                  << ",\"monster_lvl_min\":" << row.monster_lvl_min << ",\"monster_lvl_min_hard\":" << row.monster_lvl_min_hard
                  << ",\"monster_lvl_min_nightmare\":" << row.monster_lvl_min_nightmare << '}';
    }
    std::cout << "],\"ranges\":["; first = true;
    Context context{&table, 0, 0, {}};
    for (unsigned n = 0; n < table.levels.size(); ++n) for (unsigned mode = 0; mode < 3; ++mode) {
        context.range(n, mode);
        if (!first) std::cout << ',';
        first = false;
        std::cout << '[' << n << ',' << mode << ',' << context.pushed[0] << ',' << context.pushed[1] << ']';
    }
    std::cout << "]}\n";
    return 0;
} catch (const std::exception& failure) {
    std::cerr << "level tables: " << failure.what() << '\n'; return 1;
} }
