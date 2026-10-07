#pragma once
#include "data.hpp"
namespace dh2::data {
struct WorldMapLocationProfileV59 {
 std::int32_t name4{},state8{};
 std::vector<std::int32_t> location_levels;
};
struct WorldMapLockerProfileV59 {std::int32_t on_state4{},quest_id8{};};
// Actual Arrays.WorldMap/WldMapLocation (source stride20), not the separate
// FastTravel table (stride28). Provides _InitLevelStates' real MapLoc+8 cells.
class WorldMapProfileTableV59 {
 bool ready_{};
 std::vector<std::string> names_,locker_names_;
 std::vector<WorldMapLocationProfileV59> rows_;
 std::vector<WorldMapLockerProfileV59> lockers_;
 std::vector<std::int32_t> defaults8_;
public:
 bool decode(Bytes records,Bytes names,Bytes schema,std::string&);
 bool ready()const noexcept{return ready_;}
 const auto& names()const noexcept{return names_;}
 const auto& rows()const noexcept{return rows_;}
 const auto& lockers()const noexcept{return lockers_;}
 const auto& defaults8()const noexcept{return defaults8_;}
};
}
