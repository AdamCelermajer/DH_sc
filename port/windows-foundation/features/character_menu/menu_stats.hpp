#pragma once
#include "character_menu.hpp"
namespace dh::foundation::character_menu {
struct NativeMenuStats {
    std::int32_t attack=0,critical=0,defense=0,armor=0;
    std::array<std::int32_t,4> damage{}; // mainmin,max,offmin,max
    std::array<std::int32_t,5> resistance{}; // fire,water,lightning,earth,air
    std::array<std::int32_t,2> element_type{};
    // B111: the remaining NativeGetPlayerStats fields the Defense/Offense/Magic/Recovery sub-pages list.
    // Property indices are the ones CharacterMenuQueriesOwnerV1::stats writes (IDA NativeGetPlayerStats).
    std::int32_t dodge=0,block=0,melee_hit=0,melee_reduction=0,spell_crit=0,spell_hit=0;
    std::array<std::int32_t,5> spell_bonus{}; // fire,water,lightning,earth,air
    std::int32_t regen_hp=0,regen_mp=0,leech_hp=0,leech_mp=0;
};
// Exact NativeGetPlayerStats property/class/gear arithmetic and genuine combat
// bonus kernel. Borrow same resolved sheet/facts; no copied gameplay authority.
bool project_original_menu_stats(const OriginalCombatProperties&,NativeMenuStats&,std::string& error);
// True when the field path belongs to one of the five Statistics sub-page sheets (menu_CharacterSheetStats/Defense/
// Offense/Magic/Recovery).
bool is_stat_sheet_path(const std::string& source_path) noexcept;
bool original_stats_field(const std::string& source_path,const NativeMenuStats&,std::string& value);
bool original_stats_path_visible(const OriginalCombatProperties&,const NativeMenuStats&,const std::string& source_path);
void original_stats_visibility(const OriginalCombatProperties&,const NativeMenuStats&,Frame&);
}
