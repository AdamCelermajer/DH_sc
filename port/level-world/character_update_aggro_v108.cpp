#include "character_update_aggro_v108.hpp"
#include <cstring>
namespace dh2::character {namespace {
std::int32_t add(std::int32_t a,std::uint32_t b){std::uint32_t x;std::memcpy(&x,&a,4);x+=b;std::memcpy(&a,&x,4);return a;}
std::int32_t subtract(std::int32_t a,std::uint32_t b){return add(a,0u-b);}
}
//Whole3cf3f0. The first two Player virtual calls intentionally remain
//separate; source property initialization can change the second result.
bool character_update_aggro_v108(CharacterAiPointerFieldsV105& ai,TargetState48& target,const AggroFrameServicesV108& s,std::string& e){
 auto required=[&](bool value,const char* text){if(!value&&e.empty())e=text;return value;};
 if(!s.query){e="Required actual _UpdateAggro classification";return false;}
 bool value{};if(!s.query(AggroFrameQueryV108::Player,value,e))return false;if(value)return true;
 if(!s.query(AggroFrameQueryV108::Player,value,e))return false;
 if(!value){
  if(!s.query(AggroFrameQueryV108::Faerie,value,e))return false;
  if(!value){
   bool turn{};if(!s.query(AggroFrameQueryV108::MyTurn,turn,e))return false;
   if(!turn&&ai.aggro_defer_c<500){
    std::uint32_t dt{};if(!required(bool(s.dt),"Required App.GetDt for aggro")||!s.dt(dt,e))return false;ai.aggro_delay8=subtract(ai.aggro_delay8,dt);
    if(!s.dt(dt,e))return false;ai.aggro_defer_c=add(ai.aggro_defer_c,dt);return true;
   }
   ai.aggro_defer_c=0;
   if(!required(bool(s.debug),"Required aggro Debug owner")||!s.debug("DisableAIDelayTimerOptim",value,e))return false;
   if(!value){
    if(ai.aggro_delay8>0){std::uint32_t dt{};if(!required(bool(s.dt),"Required App.GetDt for aggro")||!s.dt(dt,e))return false;ai.aggro_delay8=subtract(ai.aggro_delay8,dt);if(ai.aggro_delay8>0)return true;}
    std::uint32_t sample{};if(!required(bool(s.random200),"Required actual App Random0 aggro")||!s.random200(sample,e)||sample>=200)return false;ai.aggro_delay8=static_cast<std::int32_t>(sample+100u);
   }
  }
 }
 if(!s.query(AggroFrameQueryV108::Npc,value,e))return false;if(value)return true;
 if(!s.query(AggroFrameQueryV108::Monster,value,e))return false;
 if(value){
  if(!s.query(AggroFrameQueryV108::Remote,value,e))return false;
  if(!value&&target.target){
   std::uintptr_t highest{},current{};
   if(!required(bool(s.highest)&&bool(s.current_character),"Required actual highest threat/GetHandle Character")||!s.highest(highest,e)||!s.current_character(current,e))return false;
   if(current&&highest&&current!=highest){
    float old{},next{},factor{};
    if(!required(bool(s.threat)&&bool(s.switch_factor),"Required source threat/switch factor")||!s.threat(current,old,e)||!s.threat(highest,next,e)||!s.switch_factor(factor,e))return false;
    volatile float limit=old*factor;if(!(next>limit))return true;
    if(!required(bool(s.debug)&&bool(s.set_target),"Required actual threat-change Debug/SetTarget")||!s.debug("isTracingThreatChange",value,e))return false;
    return s.set_target(highest,e);
   }
   bool enemy{};if(!required(bool(s.relationship),"Required actual aggro relationship")||!s.relationship(AggroRelationV108::Enemy,current,enemy,e))return false;
   auto clear=[&]{if(!required(bool(s.clear)&&bool(s.set_target),"Required source ClearAggro/SetTarget")||!s.clear(current,e)||!s.set_target(0,e))return false;return dh2_character_ai_sync_last_target(&target)==0;};
   if(!enemy)return clear();
   float radius{};std::vector<AggroFrameTargetV108> candidates;
   if(!required(bool(s.no_aggro_radius)&&bool(s.search),"Required source tracked70 Search")||!s.no_aggro_radius(radius,e)||!s.search(true,radius,1,candidates,e))return false;
   if(candidates.empty())return clear();
   std::uintptr_t local{};if(!required(bool(s.local_player)&&bool(s.has_relation),"Required real PM local0/outgoing relation")||!s.local_player(local,e)||!s.has_relation(local,value,e))return false;
   if(value)return true;
   bool found=false;for(const auto& candidate:candidates)if(candidate.identity==local){found=true;break;}
   if(!found)return true;
   if(!s.relationship(AggroRelationV108::Enemy,local,value,e))return false;if(!value)return true;
   float amount{};if(!required(bool(s.spotted_amount)&&bool(s.add),"Required actual EnemySpottedAggro/AddAggro")||!s.spotted_amount(amount,e))return false;
   return s.add(local,amount,e);
  }
 }
 if(!s.query(AggroFrameQueryV108::Faerie,value,e))return false;if(value&&ai.master50)return true;
 float radius{};if(!required(bool(s.view_radius),"Required actual view radius")||!s.view_radius(radius,e))return false;
 if(!s.query(AggroFrameQueryV108::AwaitingSpawn,value,e))return false;
 bool spawn=false;if(value){float custom{};if(!required(bool(s.spawn_radius),"Required actual spawn radius143c")||!s.spawn_radius(custom,e))return false;if(custom>0.f){radius=custom;spawn=true;}}
 if(!spawn){if(!s.query(AggroFrameQueryV108::HasAggro,value,e))return false;if(!value&&(!required(bool(s.no_aggro_radius),"Required actual no-aggro radius")||!s.no_aggro_radius(radius,e)))return false;}
 std::vector<AggroFrameTargetV108> candidates;
 if(!required(bool(s.search),"Required source list60 generic Search")||!s.search(false,radius,0x80000001u,candidates,e))return false;
 bool no_enemy=true;
 for(const auto& candidate:candidates){
  if(!(candidate.flags&1u)&&(!required(bool(s.noncharacter_assert),"Required original list60 assertion policy")||!s.noncharacter_assert(e)))return false;
  if(!required(bool(s.relationship)&&bool(s.raise),"Required source spotted relation/events")||!s.relationship(AggroRelationV108::Enemy,candidate.identity,value,e))return false;
  if(value){if(!s.raise(9,candidate.identity,e))return false;no_enemy=false;continue;}
  if(!s.relationship(AggroRelationV108::Friend,candidate.identity,value,e))return false;
  if(value){if(!s.raise(7,candidate.identity,e))return false;continue;}
  if(!s.relationship(AggroRelationV108::Neutral,candidate.identity,value,e))return false;
  if(value&&!s.raise(8,candidate.identity,e))return false;
 }
 if(no_enemy&&target.target){if(!required(bool(s.raise),"Required source no-enemy event12"))return false;return s.raise(12,target.target,e);}
 return true;
}
}
