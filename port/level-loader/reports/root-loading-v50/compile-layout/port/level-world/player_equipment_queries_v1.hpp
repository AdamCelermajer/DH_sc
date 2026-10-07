#pragma once
#include "../game-data/items.hpp"
namespace dh2::player {
struct EquipmentRequirements32V1 {std::uint32_t online,remote,present;std::int32_t cached[5];};
struct EquipmentQueries12V1 {std::int32_t main_category,off_category;std::uint32_t flags;};
enum EquipmentQueryFlagV1:std::uint32_t {query_main=1,query_bow=2,query_staff=4,query_dual=8,query_shield=16,query_two_raw=32,query_two_effective=64};
static_assert(sizeof(EquipmentRequirements32V1)==32&&sizeof(EquipmentQueries12V1)==12);
}
extern "C" int dh2_equipment_requirements_v1(std::int32_t*,const dh2::player::EquipmentRequirements32V1*,const dh2::data::ItemRecord164*) noexcept;
extern "C" int dh2_equipment_queries_v1(dh2::player::EquipmentQueries12V1*,const dh2::data::ItemRecord164* main,const dh2::data::ItemRecord164* off,std::int32_t flag1324) noexcept;
