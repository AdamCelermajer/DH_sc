#include "player_manager_death_queries_v70.hpp"
#include <cstring>
#include <limits>
namespace dh2::player {
bool player_manager_all_players_dead_v70(PlayerManagerOwnerV1& manager,const PlayerManagerDeathServicesV70& services,bool& dead,std::string& e){
 if(!manager.source_initialized_v59()||!services.provider){e="Required SAME constructed PM AllPlayersDead receiver";return false;}
 std::int32_t count{};if(!manager.num_players(count,e))return false;std::int32_t i=0;
 while(i<count){PlayerInfoFieldsV1* p{};if(!manager.get_player(i,false,p,e))return false;
  if(!p){e="Required actual PlayerInfo AllPlayersDead receiver";return false;}if(!p->character660){dead=false;return true;}
  bool character_dead{};if(!services.character_is_dead34||!services.character_is_dead34(p->character660,character_dead,e)){if(e.empty())e="Required Character virtual34 IsDead";return false;}
  if(!character_dead){
   // Original second lookup passes the live predicate's zero as bool argument;
   // it must not continue using the first record across that callback.
   if(!manager.get_player(i,false,p,e))return false;
   const std::int32_t* timer{};std::shared_ptr<void> pin;
   if(!p||!services.death_timer3a8||!services.death_timer3a8(*p,timer,pin,e)||!timer||!pin){if(e.empty())e="Required SAME PlayerInfo death timer3a8";return false;}
   if(*timer==-1){dead=false;return true;}
  }
  if(!manager.num_players(count,e))return false;++i;
 }
 dead=true;return true; // source empty loop also returns1.
}
bool player_manager_integer714_v70(PlayerManagerOwnerV1& manager,std::int32_t& value,std::string& e){
 const auto* fields=manager.source_frame_fields_v68();if(!fields){e="Required actual PM C1 field714";return false;}
 // Typed V68 storage is uintptr-width. Menu uses the original raw integer word;
 // a later pointer-bearing producer needs its genuine integer projection rather
 // than silently truncating a native address.
 if(fields->field714>std::numeric_limits<std::uint32_t>::max()){e="Required source32 integer projection of non-scalar PM714";return false;}
 const auto word=std::uint32_t(fields->field714);std::memcpy(&value,&word,4);return true;
}
}
