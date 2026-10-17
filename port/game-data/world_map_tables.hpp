#pragma once
#include "data.hpp"
namespace dh2::data {
// Source WldMapLocation stride20: Name+4, State+8, level count+12/pointer+16.
struct WorldMapLocation {
    std::string name;
    std::int32_t name_id=0,state=0;
    std::vector<std::int32_t> location_levels;
};
// Source WorldMapLocker stride12: OnState+4, QuestID+8.
struct WorldMapLocker {
    std::string name;
    std::int32_t on_state=0,quest_id=0;
};
struct WorldMapTables {
    std::vector<WorldMapLocation> locations;
    std::vector<WorldMapLocker> lockers;
};
// Complete two-section cache decoder; failure preserves the prior owner.
// It owns WorldMap data only. LevelTables remains the sole LevelList owner.
bool load_world_map(Bytes records,Bytes names,Bytes schema,WorldMapTables&,std::string& error);
std::int32_t find_world_map_location(const WorldMapTables&,const std::string& exact_name) noexcept;
bool read_world_map_default_word(const WorldMapTables&,std::uint32_t row,std::int32_t& output) noexcept;
} // namespace dh2::data
