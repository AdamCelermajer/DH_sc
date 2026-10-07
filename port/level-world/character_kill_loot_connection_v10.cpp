#include "character_kill_loot_connection_v10.hpp"
namespace dh2::character {
bool CharacterKillLootConnectionV10::route(KillActor56& actor,const KillRequest56& q,KillResponse16& out,bool& handled,std::string& e){
 handled=q.service==kill_drop_loot;if(!handled)return true;
 if(!actor.identity||q.subject!=actor.identity){e="Required SAME Character DropLoot source identity";return false;}
 const std::int32_t* table{};
 if(!services_.table101c||!services_.table101c(services_.context,actor.identity,table,e)||!table){if(e.empty())e="Required actual Character GetLootTable101c producer";return false;}
 out={};return loot_.drop_table(*table,actor.identity,q.target,-1,e);
}
}
