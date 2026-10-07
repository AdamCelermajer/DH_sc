#include "character_attack_animation_v1.hpp"
#include <cstring>

namespace {
using namespace dh2::character;
struct Run {
 const AttackAnimationBorrowV1& b;
 const AttackAnimationServicesV1& s;
 bool ok=true;
 std::uint32_t call(AttackAnimationServiceV1 service,std::uint32_t value=0,
                    std::uintptr_t subject=0) {
  std::uint32_t out=0;
  if(ok && s.invoke(s.context,*b.ai,{service,value,subject},out))ok=false;
  return out;
 }
 bool valid()const {return b.ai&&b.phase&&b.look_at&&s.invoke;}
};
}
extern "C" int dh2_character_attack_animation_begin_v1(
 const dh2::character::AttackAnimationBorrowV1* b,
 const dh2::character::AttackAnimationServicesV1* s) {
 if(!b||!s)return -1;
 Run r{*b,*s};if(!r.valid())return -1;
 auto& a=*b->ai;
 const auto phase=*b->phase;
 const auto step=r.call(attack_anim_step_index_v1);
 const auto count=r.call(attack_anim_step_count_v1);
 if(!r.ok)return -1;
 if(phase==0){
  std::memcpy(&a.index,&step,4);
  r.call(attack_anim_raise_v1,0x1a,step);
 }else if(phase==1){
  const auto last=step==0?1:std::uint32_t(step==count-1u);
  a.last=last;
  r.call(attack_anim_controller_look_v1,0,*b->look_at);
  if(!r.ok)return -1;
  a.finisher=0;
  if(step==0)r.call(attack_anim_pre_attack_v1,std::uint32_t(a.index));
  else if(last)a.finisher=1;
 }
 return r.ok?1:-1;
}
extern "C" int dh2_character_attack_animation_end_v1(
 const dh2::character::AttackAnimationBorrowV1* b,
 const dh2::character::AttackAnimationServicesV1* s) {
 if(!b||!s)return -1;
 Run r{*b,*s};if(!r.valid())return -1;
 auto& a=*b->ai;
 const auto attacking=r.call(attack_anim_has_combo_v1);
 if(!r.ok)return -1;
 if(!attacking)return 1;
 const auto phase=*b->phase;
 const auto step=r.call(attack_anim_step_index_v1);
 const auto count=r.call(attack_anim_step_count_v1);
 if(!r.ok)return -1;
 if(phase==0){
  const auto no_continue=(a.continued&255u)^1u;
  a.continued=0;
  if(step==count-1u){
   if(no_continue)r.call(attack_anim_clear_nonsticky_v1);
   else r.call(attack_anim_set_step_v1,0);
   if(!r.ok)return -1;
   r.call(attack_anim_raise_v1,0x1b);
   r.call(attack_anim_raise_v1,0x1c);
  }else{
   if(no_continue){
    const auto can=r.call(attack_anim_can_range_v1);
    if(!r.ok)return -1;
    if(!can){
     r.call(attack_anim_clear_nonsticky_v1);
     r.call(attack_anim_set_step_v1,count);
    }
   }
   r.call(attack_anim_raise_v1,0x1b);
  }
 }else if(phase==1){
  std::uint32_t dead=1;
  if(a.target)dead=r.call(attack_anim_target_dead_v1,0,a.target);
  if(!r.ok)return -1;
  // Original reloads +40 after the target virtual; preserve callback reentry.
  const bool ooi=!a.target&&a.object_of_interest_type==8;
  if(step==count-2u&&!ooi&&!(dead&&a.target))
   r.call(attack_anim_skip_next_v1);
  else a.continued=0;
 }
 return r.ok?1:-1;
}
