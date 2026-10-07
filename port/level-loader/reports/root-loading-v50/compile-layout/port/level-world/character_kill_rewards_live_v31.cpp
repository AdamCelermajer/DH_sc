#include "character_kill_rewards_live_v31.hpp"
namespace dh2::character {
bool CharacterKillRewardsLiveV31::route(KillActor56& actor,const KillRequest56& q,KillResponse16& out,std::string& e){
 if(!providers_){e="Required retained Kill reward service graph";return false;}
 bool handled{};
 if(q.service==kill_drop_loot){
  if(q.subject!=actor.identity){e="Kill DropLoot must address SAME dying Character";return false;}
  if(!loot_.route(actor,q,out,handled,e))return false;
  if(!handled){e="Original Kill drop route was not delivered";return false;}return true;
 }
 if(q.service==kill_distribute_xp){
  if(!progression_.route(actor,q,out,handled,e))return false;
  if(!handled){e="Original Kill XP route was not delivered";return false;}return true;
 }
 if(!remaining_){e="Required Kill continuation "+std::to_string(q.service);return false;}
 return remaining_(actor,q,out,e);
}
bool CharacterKillRewardsLiveV31::attach(CharacterKillProductionServicesV23& target,std::string& e){
 if(!providers_||!loot_.ready()||!remaining_){e="Required actual initialized Item145/reward provider lease before Kill binding";return false;}
 target.remaining=[this](auto& a,const auto& q,auto& out,auto& error){return route(a,q,out,error);};return true;
}
}
