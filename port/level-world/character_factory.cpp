#include "character_factory.hpp"
#include "object_manager_runtime_owner_v1.hpp"

#include <cstring>

namespace dh2::character::factory {
namespace {

bool bounded_length(const char* text,std::uint32_t capacity,std::uint32_t* length){
 if(!text||!capacity||!length)return false;
 std::uint32_t i=0;while(i<capacity&&text[i]!='\0')++i;
 if(i==capacity)return false;
 *length=i;
 return true;
}

}  // namespace

SpawnResult request_spawn_character(ActorRef* actors,std::uint32_t actor_count,
 const char* exact_name,const Services* services){
 std::uint32_t requested_length=0;
 if((actor_count&&!actors)||!services||!services->invoke||
    !bounded_length(exact_name,max_character_name+1u,&requested_length)||!requested_length)
  return SpawnResult::invalid_request;

 ActorRef* match=nullptr;std::uint32_t match_count=0;
 for(std::uint32_t i=0;i<actor_count;++i){
  std::uint32_t actor_length=0;
  if(!bounded_length(actors[i].exact_name,max_character_name+1u,&actor_length)||!actor_length)
   return SpawnResult::invalid_request;
  if(std::strcmp(actors[i].exact_name,exact_name)==0){match=&actors[i];++match_count;}
 }
 if(!match_count)return SpawnResult::lookup_miss;
 if(match_count!=1)return SpawnResult::ambiguous_name;
 if(!match->state||!match->facts||!match->spawn_facts)return SpawnResult::invalid_request;
 if(!(match->registered_state_mask&state_bit(1)))return SpawnResult::spawn_state_unregistered;
 const int result=dh2_character_spawn_transition(match->state,match->facts,
                                                 match->spawn_facts,1,services);
 return result==1?SpawnResult::requested:SpawnResult::source_state_rejected;
}

SpawnResult request_registered_spawn_character(
 dh2::object_manager_runtime_owner_v1::Owner& object_manager,
 ActorRef* actors,std::uint32_t actor_count,const char* exact_name,
 const Services* services){
 if(actor_count&&!actors)return SpawnResult::invalid_request;
 // Validate the entire borrowed candidate list before the exact-name lookup
 // can issue state services. The map remains the sole source registration;
 // ActorRef is only a temporary view over those same live Character owners.
 for(std::uint32_t i=0;i<actor_count;++i){
  if(actors[i].source_handle<0||!actors[i].object_identity)
   return SpawnResult::object_not_registered;
  const auto* registered=object_manager.find_by_source_handle(actors[i].source_handle);
  if(!registered||registered->identity!=actors[i].object_identity)
   return SpawnResult::object_not_registered;
 }
 return request_spawn_character(actors,actor_count,exact_name,services);
}

const char* spawn_result_name(SpawnResult result){
 switch(result){
 case SpawnResult::requested:return "requested";
 case SpawnResult::lookup_miss:return "lookup_miss";
 case SpawnResult::invalid_request:return "invalid_request";
 case SpawnResult::ambiguous_name:return "ambiguous_name";
 case SpawnResult::spawn_state_unregistered:return "spawn_state_unregistered";
 case SpawnResult::source_state_rejected:return "source_state_rejected";
 case SpawnResult::object_not_registered:return "object_not_registered";
 }
 return "unknown";
}

}  // namespace dh2::character::factory
