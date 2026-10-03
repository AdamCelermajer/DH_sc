#include "character_ai_state.hpp"
#include <cstddef>
namespace {
bool aligned(const void* p,std::uintptr_t n){return p&&(reinterpret_cast<std::uintptr_t>(p)%n)==0;}
bool overlaps(const void* a,std::size_t n,const void* b,std::size_t m){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return x<=y ? y-x<n : x-y<m;
}
}
extern "C" int dh2_character_ai_can_attack(dh2::character::AttackQueryState16* state,
 std::uintptr_t explicit_target,const dh2::character::AttackQueryServices16* services,std::uint32_t* result){
 using namespace dh2::character;
 if(!aligned(state,alignof(AttackQueryState16))||!aligned(services,alignof(AttackQueryServices16))||
    !aligned(result,alignof(std::uint32_t))||!services->query||
    overlaps(state,sizeof(*state),result,sizeof(*result))||
    overlaps(services,sizeof(*services),result,sizeof(*result)))return 1;
 const std::uintptr_t target=explicit_target?explicit_target:state->target;
 if(!target){*result=0;return 0;}
 auto query=[&](AttackQueryService kind){
  const AttackQueryRequest24 request{kind,0,state->owner,target};
  return services->query(services->context,state,&request);
 };
 if(!query(attack_is_enemy)){*result=0;return 0;}
 if(query(attack_inventory_can_melee)&&query(attack_in_melee_range)){*result=1;return 0;}
 if(!query(attack_owner_can_range)){*result=0;return 0;}
 *result=query(attack_in_range);return 0;
}
