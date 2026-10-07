#pragma once
#include "../game-data/data.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include "../game-data/loot_tables_v2.hpp"
namespace dh2::character {
struct CharacterPropsIdServicesV1 {
 void* context{};
 bool(*is_player)(void*,bool&,std::string&){};
 // Source SG_Load(1) only when SAME Save pointer14e8 is non-NULL.
 bool(*load_save)(void*,data::PlayerSavegameV1&,std::int32_t,std::string&){};
 // Whole GetCharPropsArray3b36ec and original typed array row borrows.
 bool(*preset)(void*,const std::string&,const std::int16_t*&,std::uint32_t&,std::string&){};
};
// Whole SafeGetCharPropsId3b3d38. cache is the actor's ONE constructor -1
// signed16 field13c8. CharPropsArray13a8/CharProps13c0 strings are actual
// property-map producers. Does not recalc or copy Character PropertyState.
bool character_safe_props_id_v1(std::int16_t& cache,
 const std::string& array13a8,const std::string& name13c0,
 const data::CharacterTable&,data::PlayerSavegameV1* same_save,
 data::LootRandom8V2& same_random,const CharacterPropsIdServicesV1&,
 std::int32_t& out,std::string&);
}
