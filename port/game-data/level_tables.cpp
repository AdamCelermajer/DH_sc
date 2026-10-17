#include "level_tables.hpp"

#include <algorithm>
#include <cstring>
#include <set>
#include <stdexcept>
#include <utility>

namespace dh2::data {
namespace {
constexpr std::size_t kMaximumInput = 8 * 1024 * 1024;
constexpr std::uint32_t kMaximumCount = 4096;

bool valid_utf8(const std::string& text) {
    for (std::size_t i = 0; i < text.size();) {
        const auto first = static_cast<unsigned char>(text[i++]);
        if (!first) return false;
        if (first < 0x80) continue;
        std::uint32_t code = 0, minimum = 0;
        unsigned trailing = 0;
        if (first >= 0xc2 && first <= 0xdf) { code = first & 31; trailing = 1; minimum = 0x80; }
        else if (first >= 0xe0 && first <= 0xef) { code = first & 15; trailing = 2; minimum = 0x800; }
        else if (first >= 0xf0 && first <= 0xf4) { code = first & 7; trailing = 3; minimum = 0x10000; }
        else return false;
        if (trailing > text.size() - i) return false;
        for (unsigned n = 0; n < trailing; ++n) {
            const auto byte = static_cast<unsigned char>(text[i++]);
            if ((byte & 0xc0) != 0x80) return false;
            code = (code << 6) | (byte & 63);
        }
        if (code < minimum || code > 0x10ffff || (code >= 0xd800 && code <= 0xdfff)) return false;
    }
    return true;
}

struct Reader {
    Bytes bytes;
    std::size_t offset = 0;
    explicit Reader(Bytes input) : bytes(input) {
        if (!input.data || input.size < 4 || input.size > kMaximumInput)
            throw std::runtime_error("Level table input outside limit");
    }
    void require(std::size_t size) const {
        if (offset > bytes.size || size > bytes.size - offset)
            throw std::runtime_error("Truncated level table");
    }
    std::uint32_t word() {
        require(4);
        const auto* p = bytes.data + offset;
        offset += 4;
        return p[0] | (std::uint32_t(p[1]) << 8) | (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
    }
    std::int32_t integer() {
        const auto bits = word();
        std::int32_t value;
        std::memcpy(&value, &bits, 4);
        return value;
    }
    bool boolean() {
        require(1);
        const auto value = bytes.data[offset++];
        if (value > 1) throw std::runtime_error("Invalid level boolean");
        return value != 0;
    }
    std::uint32_t count() {
        const auto value = word();
        if (value > kMaximumCount) throw std::runtime_error("Level table count outside limit");
        return value;
    }
    std::string string() {
        const auto size = word();
        if (size > 4096) throw std::runtime_error("Level string outside limit");
        require(size);
        std::string value(reinterpret_cast<const char*>(bytes.data + offset), size);
        offset += size;
        if (!valid_utf8(value)) throw std::runtime_error("Invalid level UTF-8 string");
        return value;
    }
    std::vector<std::string> names() {
        std::vector<std::string> result;
        const auto size = count();
        std::set<std::string> seen;
        for (std::uint32_t n = 0; n < size; ++n) {
            auto value = string();
            if (value.empty() || !seen.insert(value).second)
                throw std::runtime_error("Empty or duplicate level name");
            result.push_back(std::move(value));
        }
        return result;
    }
    void finish() const {
        if (offset != bytes.size) throw std::runtime_error("Unexpected level table suffix");
    }
};

bool valid_level_file(const std::string& file) {
    if (file.empty() || file == "." || file == ".." || file.find_first_of("/\\:") != std::string::npos)
        return false;
    const auto ends = [&](const char* suffix) {
        const auto length = std::strlen(suffix);
        return file.size() >= length && file.compare(file.size() - length, length, suffix) == 0;
    };
    return ends(".mlx") || ends(".rule.xml");
}
}  // namespace

bool load_levels(Bytes records, Bytes names, Bytes schema, LevelTables& output, std::string& error) {
    error.clear();
    try {
        Reader data(records), keys(names), layout(schema);
        auto fast_names = keys.names(), level_names = keys.names();
        keys.finish();
        const std::vector<std::string> fast_schema = {
            "DescriptionId", "EntryPointId", "LevelName", "LocationType", "StringId"};
        const std::vector<std::string> level_schema = {
            "Dbg_IsStable", "DynamicBusRouting", "Hub", "IsRandom", "LevelDescription", "LevelFile",
            "LevelName", "LevelState", "MapName", "MonsterLvlMax", "MonsterLvlMaxHard",
            "MonsterLvlMaxNightmare", "MonsterLvlMin", "MonsterLvlMinHard", "MonsterLvlMinNightmare"};
        if (layout.names() != fast_schema || layout.names() != level_schema)
            throw std::runtime_error("Level schema differs");
        layout.finish();
        if (data.count() != fast_names.size()) throw std::runtime_error("Fast travel count differs");
        LevelTables next;
        for (auto& name : fast_names) {
            FastTravelDestination row{};
            row.name = std::move(name);
            row.description_id = data.integer();
            row.entrypoint_id = data.integer();
            row.level_name = data.string();
            row.location_type = data.integer();
            row.string_id = data.integer();
            next.fast_travel.push_back(std::move(row));
        }
        if (data.count() != level_names.size()) throw std::runtime_error("Level count differs");
        for (auto& name : level_names) {
            LevelDeclaration row{};
            row.name = std::move(name);
            row.dbg_is_stable = data.boolean();
            row.dynamic_bus_routing = data.string();
            row.hub = data.integer();
            row.is_random = data.boolean();
            row.level_description = data.integer();
            row.level_file = data.string();
            row.level_name_id = data.integer();
            row.level_state = data.integer();
            row.map_name = data.integer();
            row.monster_lvl_max = data.integer();
            row.monster_lvl_max_hard = data.integer();
            row.monster_lvl_max_nightmare = data.integer();
            row.monster_lvl_min = data.integer();
            row.monster_lvl_min_hard = data.integer();
            row.monster_lvl_min_nightmare = data.integer();
            if (row.dynamic_bus_routing.empty() || !valid_level_file(row.level_file))
                throw std::runtime_error("Invalid level routing or file");
            next.levels.push_back(std::move(row));
        }
        data.finish();
        for (const auto& row : next.fast_travel)
            if (find_level(next, row.level_name) < 0) throw std::runtime_error("Unknown fast travel level");
        output = std::move(next);
        return true;
    } catch (const std::exception& failure) {
        error = failure.what();
        return false;
    }
}

std::int32_t find_level(const LevelTables& table, const std::string& name) noexcept {
    for (std::size_t n = 0; n < table.levels.size(); ++n)
        if (table.levels[n].name == name) return static_cast<std::int32_t>(n);
    return -1;
}

bool read_level_range_word(const LevelTables& table, std::uint32_t row_offset,
                           std::uint32_t word_offset, std::int32_t& output) noexcept {
    if (row_offset % 72 || row_offset / 72 >= table.levels.size()) return false;
    const auto& row = table.levels[row_offset / 72];
    switch (word_offset) {
        case 0x30: output = row.monster_lvl_max; break;
        case 0x34: output = row.monster_lvl_max_hard; break;
        case 0x38: output = row.monster_lvl_max_nightmare; break;
        case 0x3c: output = row.monster_lvl_min; break;
        case 0x40: output = row.monster_lvl_min_hard; break;
        case 0x44: output = row.monster_lvl_min_nightmare; break;
        default: return false;
    }
    return true;
}
}  // namespace dh2::data
