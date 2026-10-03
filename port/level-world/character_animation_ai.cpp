#include "character_animation_ai.hpp"
#include <cstring>

namespace {
using namespace dh2::character;
float add(float a,float b){volatile float r=a+b;return r;}
float sub(float a,float b){volatile float r=a-b;return r;}
float mul(float a,float b){volatile float r=a*b;return r;}
float floating(std::uint32_t u){float r;std::memcpy(&r,&u,4);return r;}
std::int32_t signed_word(std::uint32_t u){std::int32_t r;std::memcpy(&r,&u,4);return r;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<an:x-y<bn;}
bool valid(const AnimationAIState96* s,const AnimationAIServices16* c,std::uint32_t op){
 return s&&c&&!overlap(s,sizeof(*s),c,sizeof(*c))&&c->invoke&&op<=ai_skill_end&&s->owner&&s->controller&&!s->reserved0&&!s->reserved1&&
  s->seeking<=255&&s->target_sticky<=255&&s->attack_continued<=255&&s->attack_last<=255&&s->attack_finisher<=255&&
  s->skill_started<=255&&s->skill_stop_requested<=255&&s->owner_byte14a8>=-128&&s->owner_byte14a8<=127;
}
struct Kernel {
 AnimationAIState96& s;const AnimationAIServices16& c;
 AnimationAIResponse16 call(std::uint32_t service,std::uint32_t argument=0,std::uintptr_t subject=0,std::uintptr_t payload=0){
  AnimationAIRequest32 request{service,argument,0,0,subject?subject:s.owner,payload};AnimationAIResponse16 out{};
  c.invoke(c.context,&s,&request,&out);return out;
 }
 std::uint32_t index(){return call(ai_animation_step_index).word;}
 std::uint32_t count(){return call(ai_animation_step_count).word;}
 void event(std::uint32_t id,std::uintptr_t payload=0){call(ai_character_event,id,0,payload);}
 void clear_target(){call(ai_clear_nonsticky_target);}
 int move_begin(){
  if(!s.target||!s.seeking||(s.owner_flags&0x1000))return 1;
  const auto point=call(ai_target_position,0,s.target);
  const float x=sub(point.position[0],s.owner_position[0]),y=sub(point.position[1],s.owner_position[1]),z=sub(point.position[2],s.owner_position[2]);
  const float radius=floating(call(ai_melee_radius_squared).word);
  const float distance=add(add(mul(x,x),mul(y,y)),mul(z,z));
  if(radius<=distance)call(ai_controller_move_to,0,s.controller,s.target);
  return 1;
 }
 int attack_begin(){
  const auto depth=s.animation_depth;const auto step=index();const auto n=count();
  if(depth==0){s.attack_index=signed_word(step);event(0x1a,step);}
  else if(depth==1){
   if(step==0){
    s.attack_last=1;call(ai_controller_look_at,0,s.controller,s.look_target);
    s.attack_finisher=0;call(ai_pre_attack_virtual,std::uint32_t(s.attack_index));
   }else{
    const bool last=step==n-1;s.attack_last=last;
    call(ai_controller_look_at,0,s.controller,s.look_target);
    s.attack_finisher=last?1u:0u;
   }
  }
  return 1;
 }
 int attack_end(){
  if(!call(ai_has_combo_attack).word)return 1;
  const auto depth=s.animation_depth;const auto step=index();const auto n=count();
  if(depth==0){
   const auto inverted=s.attack_continued^1u;s.attack_continued=0;
   if(step==n-1){
    if(inverted)clear_target();else call(ai_animation_set_step,0);
    event(0x1b);event(0x1c);
   }else{
    if(inverted&&!call(ai_owner_can_range_attack).word){clear_target();call(ai_animation_set_step,n);}
    event(0x1b);
   }
  }else if(depth==1){
   std::uint32_t result=1;
   if(s.target)result=call(ai_target_is_dead,0,s.target).word;
   const bool target=s.target!=0;
   const bool special=!target&&s.owner_byte14a8==8;
   if(step!=n-2){s.attack_continued=0;return 1;}
   if((result&&target)||special)s.attack_continued=0;
   else call(ai_animation_skip_next_step);
  }
  return 1;
 }
 int skill(bool begin){
  const auto depth=s.animation_depth;const auto step=index();(void)count();
  if(step==0&&depth==1){
   if(begin)s.skill_started=1;
   if(s.skill_stop_requested)call(ai_animation_stop_loop,1);
  }
  return 1;
 }
 int execute(std::uint32_t op){
  if(op==ai_step_begin||op==ai_step_end){
   const auto state=signed_word(call(ai_animation_state).word);
   if(op==ai_step_begin){if(state==4)return move_begin();if(state==5)return attack_begin();if(state==6||state==7)return skill(true);}
   else {if(state==5)return attack_end();if(state==6||state==7)return skill(false);}
   return 1;
  }
  if(op==ai_move_begin)return move_begin();
  if(op==ai_attack_begin)return attack_begin();
  if(op==ai_attack_end)return attack_end();
  return skill(op==ai_skill_begin);
 }
};
}
extern "C" int dh2_character_animation_ai(dh2::character::AnimationAIState96* s,std::uint32_t op,const dh2::character::AnimationAIServices16* c){
 if(!valid(s,c,op))return -1;
 return Kernel{*s,*c}.execute(op);
}
extern "C" int dh2_character_animation_has_combo(std::int32_t attack,std::int32_t count,std::int32_t type){return attack>=0&&attack<count&&type==1;}
extern "C" std::int32_t dh2_character_animation_table_id(std::int32_t property,std::int32_t count){return property>=0&&property<count?property:17;}
