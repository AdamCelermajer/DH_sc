#include "../character_targetability_owner_v1.hpp"
#include <array>
#include <cassert>
using namespace dh2::character;
int main(){
 std::array<std::int32_t,224> resolved{};resolved[1]=0;
 skills::SkillTargetCharacterV6 actor{};actor.identity=42;actor.resolved=resolved.data();
 actor.flags520=0x2380;actor.visible8a=1;
 CharacterTargetabilityOwnerV1 owner(actor);assert(owner.construct()==1);
 assert(owner.source_byte415()==&actor.interactive415&&actor.interactive415==1);
 std::array<std::int32_t,9> types{};types[0]=4;
 dh2::target_providers::Types16 table{types.data(),9,0};
 dh2::target_providers::Services16 service{nullptr,[](void*,const dh2::target_providers::Request24* q,std::uintptr_t* out){
  if(q->service!=dh2::target_providers::virtual_dead)return -1;*out=0;return 0;}};
 int answer=-1;assert(!skills::dh2_character_skill_target_query_v6(&answer,dh2::target_providers::is_interactive,&actor,nullptr,&table,&service)&&answer==1);
 assert(owner.set_is_targetable(1,1,false)==1&&actor.interactive415==0);
 assert(!skills::dh2_character_skill_target_query_v6(&answer,dh2::target_providers::is_interactive,&actor,nullptr,&table,&service)&&answer==0);
 assert(owner.set_is_targetable(0,1,true)==1&&actor.interactive415==0);
 assert(owner.set_is_targetable(1,3,true)==1&&actor.interactive415==0);
 assert(owner.set_is_targetable(2,1,true)==1&&actor.interactive415==1);
 actor.disabled81=1;assert(!skills::dh2_character_skill_target_query_v6(&answer,dh2::target_providers::is_interactive,&actor,nullptr,&table,&service)&&answer==0);
 actor.disabled81=0;actor.flags520=0;assert(!skills::dh2_character_skill_target_query_v6(&answer,dh2::target_providers::is_interactive,&actor,nullptr,&table,&service)&&answer==0);
}
