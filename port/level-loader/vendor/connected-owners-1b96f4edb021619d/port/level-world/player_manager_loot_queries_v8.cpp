#include "player_manager_loot_queries_v8.hpp"
#include <climits>
namespace dh2::player {
bool player_class_count_v8(const std::int32_t* count,std::int32_t base,const LootPlayerManagerServicesV8& s,std::int32_t& out,std::string& e){
 e.clear();out=0;if(!count){e="Required actual PlayerManager count6c4 field";return false;}
 if(*count<=0)return true;
 std::int32_t index=0;do{
  if(!s.get_player){e="Required original PlayerManager GetPlayer(index,true)";return false;}
  LootPlayerBorrowV8 player;if(!s.get_player(s.context,index,true,player,e))return false;
  if(!player.identity||!player.character660){e="Required non-NULL source Player record and Character660 field";return false;}
  if(*player.character660){if(!player.character_base_id13c8){e="Required same Character InitPre cached base-id13c8";return false;}if(std::int32_t(*player.character_base_id13c8)==base)++out;}
  if(index==INT_MAX){e="Unsupported original PlayerManager count overflow";return false;}++index;
 }while(index<*count);
 return true;
}
}
