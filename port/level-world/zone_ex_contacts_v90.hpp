#pragma once
#include <cstdint>
#include <cstring>
#include <functional>
#include <set>
#include <string>
namespace dh2::world {
// Original ZoneEx.OnCollisionBegins39858c / OnCollisionEnds398200.
// Caller pins the SAME derived receiver; the ctor-empty contact set/counters
// are borrowed directly. No contact registry, copied counters or Eval exists.
using ZoneExContactQueryV90=std::function<bool(std::uintptr_t,bool&,std::string&)>;
inline void zone_ex_counter_v90(std::int32_t& value,bool add){
 std::uint32_t word{};std::memcpy(&word,&value,sizeof word);word=add?word+1u:word-1u;std::memcpy(&value,&word,sizeof value);
}
inline bool zone_ex_contact_v90(std::set<std::uintptr_t>& contacts,std::int32_t& characters,
 std::int32_t& players,std::uintptr_t peer,bool begin,const ZoneExContactQueryV90& inside,
 const ZoneExContactQueryV90& is_character,const ZoneExContactQueryV90& is_player,std::string& e){
 if(!peer||!inside){e="Required actual ZoneEx peer/IsInside delivery";return false;}
 bool accepted{};if(!inside(peer,accepted,e))return false;
 if(begin){if(!accepted||!contacts.insert(peer).second)return true;}
 else{if(accepted)return true;auto found=contacts.find(peer);if(found==contacts.end())return true;contacts.erase(found);}
 bool character{};if(!is_character||!is_character(peer,character,e)){if(e.empty())e="Required actual ZoneEx virtual24";return false;}
 if(!character)return true;zone_ex_counter_v90(characters,begin); // BEFORE virtual28
 bool player{};if(!is_player||!is_player(peer,player,e)){if(e.empty())e="Required actual ZoneEx virtual28";return false;}
 if(player)zone_ex_counter_v90(players,begin);return true;
}
}
