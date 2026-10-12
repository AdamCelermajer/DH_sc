#pragma once
#include <cstdint>
#include <string>
namespace dh2::player {
struct LootPlayerBorrowV8 {
 std::uintptr_t identity{};
 const std::uintptr_t* character660{};
 const std::int16_t* character_base_id13c8{};
};
struct LootPlayerManagerServicesV8 {
 void* context{};
 // Whole source GetPlayer(index,true) remains the actual required friendly-ID
 // map/online provider. Returned fields borrow the same live Player/Character.
 bool(*get_player)(void*,std::int32_t,bool,LootPlayerBorrowV8&,std::string&){};
};
// Source36ea50 reads manager+6c4 afresh after each player query. The three
// entry points use Character InitPre cached base IDs263/290/325, not Save's
// selectable ClassID, current AI strings or invented zero class counters.
bool player_class_count_v8(const std::int32_t* count6c4,std::int32_t base_id,
                           const LootPlayerManagerServicesV8&,
                           std::int32_t& count,std::string&);
inline bool player_warrior_count_v8(const std::int32_t* n,const LootPlayerManagerServicesV8& s,std::int32_t& out,std::string& e){return player_class_count_v8(n,263,s,out,e);}
inline bool player_mage_count_v8(const std::int32_t* n,const LootPlayerManagerServicesV8& s,std::int32_t& out,std::string& e){return player_class_count_v8(n,290,s,out,e);}
inline bool player_rogue_count_v8(const std::int32_t* n,const LootPlayerManagerServicesV8& s,std::int32_t& out,std::string& e){return player_class_count_v8(n,325,s,out,e);}
}
