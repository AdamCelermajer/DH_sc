#include "character_script_collision.hpp"
#include <cstddef>
namespace {
bool aligned(const void* p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
}
extern "C" int dh2_character_script_collision(dh2::character::ScriptCollisionState72* s,
 dh2::character::ScriptCollisionObject16* object,std::uint32_t persist,
 const dh2::character::ScriptCollisionServices16* services){
 using namespace dh2::character;
 if(!aligned(s,8)||!aligned(object,8)||!aligned(services,8)||!services->invoke||
    overlap(s,sizeof(*s),object,sizeof(*object))||overlap(s,sizeof(*s),services,sizeof(*services))||overlap(object,sizeof(*object),services,sizeof(*services))||
    !s->script_owner||!s->owner||!s->ai_owner||!s->controller||!object->identity||object->reserved||s->paused>255)return -1;
 if(s->movement_state!=4&&s->movement_state!=19)return 1;
 const auto collided=object->identity,current=s->current_target;
 if(collided==current)return 1;
 auto call=[&](std::uint32_t service,std::uintptr_t subject,std::uintptr_t target=0,std::uint32_t arg=0){
  const ScriptCollisionRequest32 request{service,arg,subject,target,0,0};
  return services->invoke(services->context,s,object,&request);
 };
 if(call(script_collision_is_character,collided)){
  const auto preferred=s->preferred_target;
  if(!call(script_collision_owner_is_player,s->owner)&&(!preferred||preferred!=current)){
   if(call(script_collision_is_enemy,s->ai_owner,collided)){
    call(script_collision_set_target,s->ai_owner,collided,0);return 1;
   }
  }
  if(call(script_collision_owner_is_player,s->owner)&&call(script_collision_is_enemy,s->ai_owner,collided))
   call(script_collision_cancel_sneaking,s->owner);
 }
 if(!persist)return 1;
 if(!call(script_collision_is_character,collided)&&object->type!=2&&object->type!=21)return 1;
 if(s->last_collision_frame==s->application_frame||s->paused)return 1;
 s->last_collision_frame=s->application_frame;
 const auto old=s->collision_ms;
 s->collision_ms=old+s->dt_ms;
 return 1;
}
