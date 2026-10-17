#pragma once

#include "data.hpp"

namespace dh2::data {

struct FastTravelDestination {
    std::string name;
    std::int32_t description_id, entrypoint_id;
    std::string level_name;
    std::int32_t location_type, string_id;
};

// Owned native data, not a copy of an ARM32 object. The original declaration
// has a vptr and two length/pointer pairs; its runtime stride is 72 bytes.
// Serialized rows have variable-length strings and two one-byte booleans.
struct LevelDeclaration {
    std::string name;
    bool dbg_is_stable;
    std::string dynamic_bus_routing;
    std::int32_t hub;
    bool is_random;
    std::int32_t level_description;
    std::string level_file;
    std::int32_t level_name_id, level_state, map_name;
    std::int32_t monster_lvl_max, monster_lvl_max_hard, monster_lvl_max_nightmare;
    std::int32_t monster_lvl_min, monster_lvl_min_hard, monster_lvl_min_nightmare;
};

struct LevelTables {
    std::vector<FastTravelDestination> fast_travel;
    std::vector<LevelDeclaration> levels;
};

// Decode the complete FastTravelList + LevelList wire data/names/schema.
// Native validation bounds counts/strings and rejects malformed input. Failed
// loads preserve the previous owned table. Modded row counts are supported;
// schema changes require a corresponding reader update.
bool load_levels(Bytes records, Bytes names, Bytes schema, LevelTables&, std::string& error);
std::int32_t find_level(const LevelTables&, const std::string& exact_name) noexcept;

// Map the original Lua caller's wrapped row offset and six range-word offsets
// onto current native owned rows. Each call resolves table.levels afresh, so
// a source ReturnValues push may replace its backing storage between min/max.
// Invalid offsets preserve output and return false. This is a storage adapter,
// not an ARM32 pointer overlay or a current-Level identity producer.
bool read_level_range_word(const LevelTables&, std::uint32_t row_byte_offset,
                           std::uint32_t word_offset, std::int32_t& output) noexcept;

}  // namespace dh2::data
