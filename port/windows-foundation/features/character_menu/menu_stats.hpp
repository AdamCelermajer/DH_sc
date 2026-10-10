#pragma once
#include "character_menu.hpp"
namespace dh::foundation::character_menu {
struct NativeMenuStats {
    std::int32_t attack=0,critical=0,defense=0,armor=0;
    std::array<std::int32_t,4> damage{}; // mainmin,max,offmin,max
    std::array<std::int32_t,5> resistance{}; // fire,water,lightning,earth,air
    std::array<std::int32_t,2> element_type{};
};
// Exact NativeGetPlayerStats property/class/gear arithmetic and genuine combat
// bonus kernel. Borrow same resolved sheet/facts; no copied gameplay authority.
bool project_original_menu_stats(const OriginalCombatProperties&,NativeMenuStats&,std::string& error);
bool original_stats_field(const std::string& source_path,const NativeMenuStats&,std::string& value);
bool original_stats_path_visible(const OriginalCombatProperties&,const NativeMenuStats&,const std::string& source_path);
void original_stats_visibility(const OriginalCombatProperties&,const NativeMenuStats&,Frame&);
}
