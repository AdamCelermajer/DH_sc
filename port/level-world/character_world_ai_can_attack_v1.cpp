#include "character_world_ai_can_attack_v1.hpp"
namespace dh2::character {
bool world_ai_can_attack_v1(std::uintptr_t owner,std::uintptr_t target,
 std::uintptr_t current,const WorldAIAttackServicesV1& s,bool& result,std::string& error){
 error.clear();result=false;
 if(!owner){error="AI_CanAttack requires the actual Character owner";return false;}
 if(!target)target=current;if(!target)return true;
 auto read=[&](WorldAIAttackQueryV1 q,std::int32_t& v){
  if(!s.query){error="AI_CanAttack reached a required native query";return false;}
  if(!s.query(s.context,owner,target,q,v,error)){if(error.empty())error="AI_CanAttack query producer unavailable";return false;}
  return true;
 };
 std::int32_t v;
 if(!read(WorldAIAttackQueryV1::IsEnemy,v))return false;if(!v)return true;
 if(!read(WorldAIAttackQueryV1::InventoryCanMeleeAttack,v))return false;
 if(v){if(!read(WorldAIAttackQueryV1::IsInMeleeRange,v))return false;if(v){result=true;return true;}}
 if(!read(WorldAIAttackQueryV1::CharacterCanRangeAttack,v))return false;if(!v)return true;
 if(!read(WorldAIAttackQueryV1::IsInRange,v))return false;result=v!=0;return true;
}
}
