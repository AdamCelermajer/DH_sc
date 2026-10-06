#include "loot_pickup_quest_tail_v10.hpp"
namespace dh2::character {
bool loot_pickup_quest_tail_v10(std::uintptr_t character,std::int32_t item,const LootPickupQuestServicesV10& s,std::string& e){
 if(!character)return true;
 bool player{};if(!s.is_player||!s.is_player(s.context,character,player,e)){if(e.empty())e="Required actual pickup destination IsPlayer";return false;}if(!player)return true;
 bool found{};if(!s.registered_gathering_id||!s.registered_gathering_id(s.context,character,item,found,e)){if(e.empty())e="Required SAME inventory gathering-ID list30";return false;}if(!found)return true;
 std::uintptr_t gs{};if(!s.current_game_state||!s.current_game_state(s.context,gs,e)){if(e.empty())e="Required actual Application current GS for pickup quest";return false;}if(!gs)return true;
 LootPickupQuestEventV10 event;event.character=character;event.item_id=item;
 if(!s.constant||!s.constant(s.context,"v2QuestObjectiveType","GatherLoot",event.objective_type,e)){if(e.empty())e="Required actual GatherLoot quest constant";return false;}
 if(!s.raise_async||!s.raise_async(s.context,gs,event,e)){if(e.empty())e="Required actual GS pickup quest AsyncCall";return false;}return true;
}
}
