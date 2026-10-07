#include "character_skill_aggro_v6.hpp"
#include <cstring>
namespace dh2::character::skills {namespace {
float floating(std::uint32_t v){float out;std::memcpy(&out,&v,4);return out;}
std::uint32_t bits(float v){std::uint32_t out;std::memcpy(&out,&v,4);return out;}
bool contains(const data::AggroTable& t,std::uintptr_t id){for(unsigned i=0;i<t.count;++i)if(t.entries[i].character==id)return true;return false;}
}
extern "C" int dh2_character_skill_aggro_v6(SkillAggroOutputV6* out,SkillAggroOwnerV6* owner,
 SkillAggroTargetV6* target,std::uint32_t amount,std::uint32_t add,const SkillAggroServicesV6* services){
 if(!out||!owner||!target||!owner->identity||!target->identity||!owner->outgoing||!target->incoming||add>1||!services||!services->invoke)return -1;
 data::AggroQuery queried{};if(dh2_aggro_query(&queried,owner->outgoing,target->identity,0))return -1;
 const bool previous_exists=contains(*owner->outgoing,target->identity);const auto previous=queried.threat_bits;
 if(add&&previous_exists){volatile float sum=floating(amount)+floating(previous);amount=bits(sum);}
 SkillAggroOutputV6 result{};
 auto end=[&](int status,std::uint32_t returned){
  if(add&&previous_exists){volatile float difference=floating(returned)-floating(previous);returned=bits(difference);}
  result.returned_bits=returned;result.status=status;*out=result;return status;
 };
 auto call=[&](unsigned op,std::uintptr_t subject,std::uintptr_t other,std::uint32_t& value){
  result.phase=op;++result.calls;value=0;SkillAggroRequestV6 request{op,0,subject,other};return services->invoke(services->context,&request,&value)==0;
 };
 std::uint32_t value;
 if(!call(skill_aggro_owner_player_v6,owner->identity,0,value))return end(-2,0);
 if(value)return end(0,0);
 if(!call(skill_aggro_owner_dead_v6,owner->identity,0,value))return end(-2,0);
 if(value)return end(0,0);
 if(!call(skill_aggro_target_dead_v6,target->identity,0,value))return end(-2,0);
 if(value)return end(0,0);
 if(dh2_aggro_query(&queried,owner->outgoing,target->identity,0))return end(-2,0);
 if(!contains(*owner->outgoing,target->identity)){
  if(!call(skill_aggro_target_on_aggro_v6,target->identity,owner->identity,value))return end(-2,0);
  result.notify_delivered=1;
 }
 data::AggroRequest request{owner->outgoing,target->incoming,owner->identity,target->identity,amount,0};data::AggroChange change{};
 if(dh2_aggro_apply(&change,&request,data::aggro_set))return end(-2,0);
 return end(0,change.returned_bits);
}
}
