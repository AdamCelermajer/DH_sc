#include "character_state.hpp"
#include "character_skill_state_dispatch_v1.hpp"
#include "character_cast_lifecycle_v1.hpp"
#include <cmath>
#include <cstring>
namespace {
using namespace dh2::character;
std::int32_t signed_bits(std::uint32_t value){std::int32_t result;std::memcpy(&result,&value,sizeof result);return result;}
bool combat_id(int value){return value>=8&&value<=11;}
bool id(int value){return value==-1||value==2||value==3||value==4||value==5||value==12||value==13;}
bool valid_base(const State* s,const Facts* f,const Services* c){
 return s&&f&&c&&c->invoke&&f->is_player<=1&&!f->reserved&&
 f->is_at_destination<=1&&f->following_path<=1&&f->has_ranged_weapon<=1&&
 s->idle_suppressed<=255&&s->stop_attack_allowed<=255&&s->heading_active<=255&&
 s->controller_locked<=255&&s->dead_alternate<=255&&s->body_present<=1&&s->move_type<=2;
}
bool valid(const State* s,const Facts* f,const Services* c){return valid_base(s,f,c)&&id(s->current);}
void call(State& s,const Services& c,Service service,int a=0,int b=0,int d=0,float value=0,std::uint64_t identity=0){
 const Request request{service,{a,b,d},value,0,identity};c.invoke(c.context,&s,&request);
}
int selected(const Facts& f,int base,unsigned mask){return std::int32_t(std::uint32_t(base)+((f.stance_mask&mask)?std::uint32_t(f.stance):0u));}
void speed(State& s,const Services& c,float value){s.cached_speed=value;call(s,c,set_speed,0,0,0,value);}
void move_type(State& s,const Facts& f,const Services& c){
 unsigned type=s.move_type;
 if(f.is_player){
  float sq=f.heading[0]*f.heading[0];sq=sq+f.heading[1]*f.heading[1];sq=sq+f.heading[2]*f.heading[2];
  if(type==2){if(!(f.walk_threshold*f.walk_threshold>sq))return;type=1;}
  else if(f.run_threshold*f.run_threshold<sq)type=2;
  else if(type==0)type=1;else if(type==1)return;
 }else{if(type)return;type=1;}
 s.move_type=type;s.cached_speed=f.walk_speed;
 call(s,c,set_animation,selected(f,type==2?f.run:f.walk,type==2?0x20:0x10));
 call(s,c,set_speed,0,0,0,s.cached_speed);
}
void blur(State& s,const Facts& f,const Services& c,const CombatFacts* combat=nullptr){
 switch(s.current){
 case 3:s.idle_suppressed=0;break;
 case 4:call(s,c,stop);if(s.body_present)call(s,c,pin);break;
 case 5:if(f.attack_delay)call(s,c,start_timer,std::int32_t(f.attack_delay),0,0x2a);if(s.body_present)call(s,c,pin);break;
 case 12:s.controller_locked=0;if(s.body_present)call(s,c,reset_filter);break;
 case 13:call(s,c,restore_interaction_rotation);break;
 case 8:if(combat)call(s,c,scare_blur);if(s.body_present)call(s,c,pin);break;
 case 9:if(combat&&combat->controller_present){s.controller_locked=0;call(s,c,controller_lock,0);}if(s.body_present)call(s,c,pin);break;
 case 10:
  if(combat&&combat->controller_present){s.controller_locked=0;call(s,c,controller_lock,0);}
  if(s.body_present){call(s,c,reset_filter);call(s,c,pin);}break;
 case 11:if(combat)call(s,c,look_at,1,0,0,0,combat->last_target);break;
 }
}
void focus(State& s,const Facts& f,int prior,std::uint64_t payload,const Services& c,const CombatFacts* combat=nullptr,
           const AnimFacts* anim=nullptr){
 switch(s.current){
 case 3:
  if(s.idle_suppressed)return;
  s.flags=0x2380;call(s,c,set_animation,selected(f,f.idle,2));break;
 case 4:
  s.flags=0x23c1;s.move_type=0;move_type(s,f,c);if(s.body_present)call(s,c,unpin);break;
 case 5:{
  s.flags=0x2341;if(f.attack_delay)s.attack_gate|=1;
  const bool moving=f.is_player?prior==4:f.attack_static==-1;
  call(s,c,set_animation,selected(f,moving?f.attack_moving:f.attack_static,moving?0x40:0x80));
  if(s.body_present)call(s,c,moving?unpin:pin);
  speed(s,c,f.attack_speed);call(s,c,cancel_sneaking);break;
 }
 case 12:
  call(s,c,dead_focus_prelude);
  s.flags=0x241;if(f.is_player)s.flags|=0x2000;
  call(s,c,look_at,1,0,0,0,payload);s.controller_locked=1;call(s,c,remove_highlight);
  if(!s.dead_alternate){
   const int animation=s.animation_override;
   if(s.animation_override!=-1)s.animation_override=-1;
   call(s,c,set_animation,animation);
  }
  call(s,c,stop_loop);
  if(s.current_animation==-1&&!f.is_player)call(s,c,start_timer,std::int32_t(f.despawn_delay),0,0x2e);
  if(s.body_present)call(s,c,set_death_filter,0,0x51c,3);
  call(s,c,cancel_sneaking);call(s,c,disable_state_fx);call(s,c,disable_self_fx);call(s,c,remove_buffs);
  call(s,c,raise_event,0x2a);call(s,c,raise_event,0x2c);call(s,c,raise_event,0x2b);break;
 case 13:
  // CSInteract::OnFocus for a non-monster Character: source flags, clear the
  // current animation, then the existing Character::CancelSneaking path.
  s.flags=25537;call(s,c,set_animation,-1);call(s,c,cancel_sneaking);break;
 case 2:{
  // CSDespawn::OnFocus saves prior flags, disables animation, then cancels sneaking.
  const auto previous=s.flags;s.flags=512;
  call(s,c,store_previous_flags,std::int32_t(previous));
  call(s,c,set_animation,-1);call(s,c,cancel_sneaking);break;
 }
 case 8:
  if(!combat)break;
  s.flags=8768;call(s,c,set_animation,combat->scared_animation);
  call(s,c,scare_focus,0,0,0,0,payload);
  call(s,c,cancel_sneaking);if(s.body_present)call(s,c,unpin);break;
 case 9:
  if(!combat)break;
  s.flags=8706;call(s,c,set_animation,combat->stunned_animation);
  if(combat->controller_present){s.controller_locked=1;call(s,c,controller_lock,1);}
  call(s,c,cancel_sneaking);if(s.body_present)call(s,c,unpin);break;
 case 10:
  if(!combat)break;
  s.flags=9025;call(s,c,set_animation,-1);
  if((combat->machine_flags_44&0x8)&&s.body_present)call(s,c,set_death_filter,0,0x51c,3);
  // IDA 0x3c4a48: with the great-knock bit (0x10) set the whole machine word
  // (Character+0x528) is replaced by 0x20; otherwise only bit 0x20 is cleared.
  if(combat->machine_flags_44&0x10)call(s,c,combat_machine_flags,-1,0x20);
  else call(s,c,combat_machine_flags,0x20,0);
  if(combat->controller_present){s.controller_locked=1;call(s,c,controller_lock,1);}
  call(s,c,look_at,0,0,0,0,payload);call(s,c,cancel_sneaking);
  if(s.body_present)call(s,c,unpin);
  break;
 case 11:
  if(!combat)break;
  s.flags=11073;call(s,c,set_animation,-1);call(s,c,cancel_sneaking);break;
 case 14:{
  // CSAnim::OnFocus 0x3c324c: Character+0x520 is 0x2240 when the +0x45 byte is
  // set, else 0x2340, then SM_SetAnim(-1) 0x3c0b50 plays the machine's pending
  // AnimTable row (+0x28) and resets it to -1.
  if(!anim)break;
  s.flags=anim->idle_on_update_541?8768:9024;
  const int row=s.animation_override;
  if(row!=-1)s.animation_override=-1;
  call(s,c,set_animation,row);break;
 }
 }
}
int transition(State& s,const Facts& f,int next,int event,std::uint64_t payload,const Services& c,const CombatFacts* combat=nullptr,
               const AnimFacts* anim=nullptr){
 const int prior=s.current;
 if(combat&&prior==11&&!combat->last_target_available)return -1;
 blur(s,f,c,combat);s.current=next;if(prior!=next)s.elapsed_ms=0;
 focus(s,f,prior,payload,c,combat,anim);call(s,c,raise_event,0x1d,prior,0,0,std::uint64_t(std::int64_t(prior)));
 (void)event;return 1;
}
bool interrupted(const State& s,const CombatFacts& f,int target,bool& known){
 known=true;
 if(target==6)return (s.flags&1u)!=0;
 if(target==10)return (f.machine_flags_44&0x20u)==0;
 if(target==5){known=f.interrupted_byte_available!=0;return known&&f.interrupted_byte_441!=0;}
 return true;
}
bool skill_id(int value){return (id(value)&&value!=2)||value==6;}
int skill_transition(State& s,const Facts& f,
 dh2::character_skill_state_dispatch_v1::Projection& projection,
 int next,int event,std::uint64_t payload,const Services& c,const CombatFacts* combat=nullptr,
 const AnimFacts* anim=nullptr){
 namespace skill=dh2::character_skill_state_dispatch_v1;
 const int prior=s.current;
 if(combat&&prior==11&&!combat->last_target_available)return -1;
 if(prior==6){
  skill::Result callback{};
  if(skill::callback(&projection,dh2::character_skill_fsm_callbacks_v1::Callback::blur,
                     &callback)!=skill::Status::complete)return -1;
 }else blur(s,f,c,combat);
 s.current=next;if(prior!=next)s.elapsed_ms=0;
 if(next==6){
  skill::Result callback{};
  if(skill::callback(&projection,dh2::character_skill_fsm_callbacks_v1::Callback::focus,
                     &callback)!=skill::Status::complete)return -1;
 }else focus(s,f,prior,payload,c,combat,anim);
 call(s,c,raise_event,0x1d,prior,0,0,std::uint64_t(std::int64_t(prior)));
 (void)event;return 1;
}
int cast_transition(State& s,const Facts& f,
 dh2::character_cast_lifecycle_v1::Projection& projection,
 int next,int event,std::uint64_t payload,const Services& c,
 const CombatFacts* combat=nullptr,const AnimFacts* anim=nullptr){
 namespace cast=dh2::character_cast_lifecycle_v1;
 const int prior=s.current;
 if(prior==7){
  cast::Result callback{};
  if(cast::execute(&projection,cast::Callback::blur,&callback)!=cast::Status::complete)return -1;
 }else blur(s,f,c);
 s.current=next;if(prior!=next)s.elapsed_ms=0;
 if(next==7){
  cast::Result callback{};
  if(cast::execute(&projection,cast::Callback::focus,&callback)!=cast::Status::complete)return -1;
 }else focus(s,f,prior,payload,c,combat,anim);
 call(s,c,raise_event,0x1d,prior,0,0,std::uint64_t(std::int64_t(prior)));
 (void)event;return 1;
}
bool anim_facts_valid(const AnimFacts* a){
 return a&&a->wait_for_end_540<=255&&a->idle_on_update_541<=255&&a->scripted_1480<=255;
}
// The non-CSAnim side of a transition that has CSAnim on exactly one side (or
// both). Spawn/Limbus states and the skill/cast pair have their own entry points.
bool anim_partner(const State& s,int next,bool combat_present){
 if(s.current!=14&&next!=14)return false;
 if(s.current==14&&next==14)return true;
 if(next==14){
  const int from=s.current;
  return from==-1||from==2||from==3||from==4||from==5||from==12||from==13||
         (combat_id(from)&&combat_present);
 }
 // Leaving CSAnim: Despawn (2) needs a dead source state, never CSAnim; -1 is
 // not a target.
 return next==3||next==4||next==5||next==12||next==13||(combat_id(next)&&combat_present);
}
bool spawn_state_id(int value){return value==-1||value==0||value==1||value==3||value==17;}
bool valid_spawn_facts(const State* s,const Facts* f,const SpawnFacts* sf,const Services* c,
                       std::int32_t min_spawn_animation=0){
 return valid_base(s,f,c)&&sf&&sf->spawn_animation>=min_spawn_animation&&
  sf->reserved_zero==0&&sf->visual_present<=1&&std::isfinite(sf->raw_fade_in_argument)&&
  sf->raw_fade_in_argument>=0&&sf->respawn_timer_eligible<=1&&sf->can_respawn<=1&&
  sf->group_role3<=1&&sf->group_has_other_in_limbus<=1&&
  (!sf->group_has_other_in_limbus||sf->group_role3);
}
// CSSpawn::OnFocus (0x3c35ec) passes CharAnimTable+0x80 to CharAnimator::ANIM_Set;
// a table without a Spawn sequence (MadrukEnergySpawner's) supplies -1, which
// CharAnimator::_SetAnim (0x3cab38) ignores, so -1 is a valid "no animation".
// The CSAnim state-14 check keeps the strict >=0 floor (valid_spawn_facts default).
bool valid_spawn(const State* s,const Facts* f,const SpawnFacts* sf,const Services* c){
 return valid_base(s,f,c)&&spawn_state_id(s->current)&&valid_spawn_facts(s,f,sf,c,-1);
}
void limbus_blur(State& s,const SpawnFacts& sf,const Services& c){
 // CSLimbus::OnBlur first clears its controller lock byte, then invokes
 // Character SetVisible(true), restores authored position/rotation and revives.
 call(s,c,reset_controller_lock);
 call(s,c,set_visible,1);
 call(s,c,restore_limbus_position);
 call(s,c,restore_limbus_rotation);
 call(s,c,revive_character);
 // The original group update is conditional on the source group role. The
 // caller supplies the role3 gate and any-other-member-in-Limbus predicate.
 // This differs from GroupInfo::CanRespawn's all-members predicate.
 if(sf.group_role3)call(s,c,set_limbus_group_status,sf.group_has_other_in_limbus?2:0);
}
void limbus_focus(State& s,const SpawnFacts& sf,const Services& c){
 s.flags=0;
 call(s,c,set_visible,0);
 if(sf.respawn_timer_eligible&&sf.respawn_delay_ms)
  call(s,c,start_timer,signed_bits(sf.respawn_delay_ms),0,0x2f);
 call(s,c,clear_all_aggro);
}
void spawn_blur(State& s,const Services& c){
 if(!(s.flags&0x2000u)){
  call(s,c,init_physical_object);s.body_present=1;
 }
}
void spawn_focus(State& s,const SpawnFacts& sf,int prior,
                 std::uint32_t prior_flags,const Services& c){
 s.flags=0x241;
 if(prior==17&&(prior_flags&0x2000u))s.flags|=0x2000u;
 // CSSpawn reads CharAnimTable+0x80 and requests that exact sequence. It does
 // not apply Character::GetAnimStance or an AnimStancedAnim table offset here.
 call(s,c,set_animation,sf.spawn_animation);
 call(s,c,clear_ai_target);
 call(s,c,sync_last_ai_target);
 call(s,c,cancel_sneaking);
 // The pinned VisualObject::StartFadeIn implementation is a `bx lr` stub.
 // Retain its authored raw argument as an observation; the adapter must not
 // synthesize an alpha ramp for this source build.
 if(sf.visual_present)call(s,c,start_fade_in,0,0,0,sf.raw_fade_in_argument);
}
void source_blur(State& s,const Facts& f,const SpawnFacts& sf,const Services& c){
 switch(s.current){
 case -1:break;
 case 0:limbus_blur(s,sf,c);break;
 case 1:spawn_blur(s,c);break;
 case 3:blur(s,f,c);break;
 case 17:
  // CSSPreSpawn::OnBlur: restore virtual actor hook, revive and collisions.
  call(s,c,set_visible,1);call(s,c,revive_character);call(s,c,enable_collisions);break;
 }
}
void source_focus(State& s,const Facts& f,const SpawnFacts& sf,int prior,
                  std::uint32_t prior_flags,const Services& c){
 switch(s.current){
 case 0:limbus_focus(s,sf,c);break;
  case 1:spawn_focus(s,sf,prior,prior_flags,c);break;
 case 3:focus(s,f,prior,0,c);break;
 }
}
int spawn_transition(State& s,const Facts& f,const SpawnFacts& sf,int next,
                     const Services& c){
 const int prior=s.current;const std::uint32_t prior_flags=s.flags;
 source_blur(s,f,sf,c);s.current=next;if(prior!=next)s.elapsed_ms=0;
 source_focus(s,f,sf,prior,prior_flags,c);
 call(s,c,raise_event,0x1d,prior,0,0,std::uint64_t(std::int64_t(prior)));
 return 1;
}
}
extern "C" int dh2_character_state_transition(dh2::character::State* s,const dh2::character::Facts* f,
 std::int32_t next,std::int32_t event,std::uint64_t payload,const dh2::character::Services* c){
 if(!valid(s,f,c)||next==-1||!id(next)||
    (next==2&&(s->current!=12||event!=0x2e)))return -1;
 return transition(*s,*f,next,event,payload,*c);
}
extern "C" int dh2_character_combat_state_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::CombatFacts* combat,
 std::int32_t next,std::int32_t event,std::uint64_t payload,
 const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!combat||combat->controller_present>1||
    combat->interrupted_byte_441>1||combat->interrupted_byte_available>1||
    combat->last_target_available>1||
    (!id(s->current)&&!combat_id(s->current))||
    (!id(next)&&!combat_id(next))||next==-1||
    (next==2&&(s->current!=12||event!=0x2e)))return -1;
 return transition(*s,*f,next,event,payload,*c,combat);
}
extern "C" int dh2_character_state_event(dh2::character::State* s,const dh2::character::Facts* f,
 std::uint32_t event,std::uint64_t payload,const dh2::character::Services* c){
 using namespace dh2::character;if(!valid(s,f,c))return -1;
 // RaiseStateEvent clears machine bits before current OnEvent and registered
 // transitions. AI routing/expired callbacks precede this entry point.
 if(event>=0x2a&&event<=0x2c)s->attack_gate&=~(1u<<(event-0x2a));
 // Source OnEvent executes before registered transition predicates.
 if(s->current==5&&event==0x1c&&f->is_player){
  const bool moving_heading=s->heading_active!=0;
  const int moving=selected(*f,f->attack_moving,0x40),stationary=selected(*f,f->attack_static,0x80);
  call(*s,*c,swap_animation,moving_heading?moving:stationary,moving_heading?stationary:moving);
  if(s->body_present)call(*s,*c,moving_heading?unpin:pin);
 }else if(s->current==5&&event==0x1a){
  if(f->target)call(*s,*c,look_at,2,0,0,0,f->target);
  else if(f->has_ranged_weapon&&s->heading_active){
   std::int32_t words[3];std::memcpy(words,f->heading,sizeof words);
   call(*s,*c,set_heading,words[0],words[1],words[2],1.0f);
  }
 }
 else if(s->current==12&&event==0x22){
  call(*s,*c,remove_body);s->body_present=0;
  if(!f->is_player){call(*s,*c,start_timer,std::int32_t(f->despawn_delay),0,0x2e);s->flags=0x40;}
 }
 // Monster Character::RaiseEvent(2,killer) reaches the FSM only after
 // CharAI::OnDied has called AI_SetDead and entered state12. CSDead::OnInit
 // registers 46 and 50009, while CSDead::OnEvent handles only 34; event2 is
 // therefore the source no-op here, not another transition to Dead.
 int next=-1;
 if(event==50003&&(s->current==3||s->current==4))next=13;
 else if(event==0xc358&&(s->current==3||s->current==4||s->current==5))next=12;
 else if(event==0x2e&&s->current==12)next=2;
 else if(event==0xc354&&(s->current==3||s->current==4)&&!(s->attack_gate&1))next=5;
 else if(event==0xc351&&(s->current==3||(s->current==5&&s->stop_attack_allowed)))next=4;
 else if((event==0x22&&s->current==5)||(event==0x3f&&s->current==4)||
         ((event==0xc352||event==0x22||event==0x23)&&s->current==3))next=3;
 return next<0?0:transition(*s,*f,next,std::int32_t(event),payload,*c);
}
extern "C" int dh2_character_combat_state_event(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::CombatFacts* combat,
 std::uint32_t event,std::uint64_t payload,const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!combat||combat->controller_present>1||
    combat->interrupted_byte_441>1||combat->interrupted_byte_available>1||
    combat->last_target_available>1||
    (!id(s->current)&&!combat_id(s->current)))return -1;
 if(event>=0x2a&&event<=0x2c)s->attack_gate&=~(1u<<(event-0x2a));
 if(s->current==10&&event==35){
  call(*s,*c,knockback_impact_event,std::int32_t(payload));return 0;
 }
 if(s->current==10&&event==39){if(s->body_present)call(*s,*c,reset_filter);return 1;}
 int next=-1;
 if(s->current==8&&(event==44||event==34))next=3;
 else if(event==63&&s->current==4)next=3;
 else if(event==34&&(s->current==5||s->current==10||s->current==11))next=3;
 else if(event==50008&&(s->current==3||s->current==4||s->current==5||
         s->current==8||s->current==9||s->current==10||s->current==11))next=12;
 else if(event==50010&&(s->current==3||s->current==4||s->current==5||
         s->current==8||s->current==9||s->current==11))next=11;
 else if(event==50011&&(s->current==3||s->current==4||s->current==5||
         s->current==8||s->current==9||s->current==11)){
  bool known=false;if(!interrupted(*s,*combat,10,known))return known?0:-1;next=10;
 }
 else if(event==50012&&(s->current==3||s->current==4||s->current==5||s->current==8)){
  bool known=false;if(!interrupted(*s,*combat,9,known))return known?0:-1;next=9;
 }
 else if(event==50013&&(s->current==3||s->current==4||s->current==5)){
  bool known=false;if(!interrupted(*s,*combat,8,known))return known?0:-1;next=8;
 }
 return next<0?0:transition(*s,*f,next,static_cast<std::int32_t>(event),payload,*c,combat);
}
extern "C" int dh2_character_state_update(dh2::character::State* s,const dh2::character::Facts* f,
 std::uint32_t dt,const dh2::character::Services* c){
 using namespace dh2::character;if(!valid(s,f,c))return -1;s->elapsed_ms+=dt;
 if(s->current==3)call(*s,*c,idle_common_update);
 else if(s->current==4){
  if(!s->heading_active){call(*s,*c,raise_event,0x3f);return 1;}
  if(!f->target&&!f->is_player&&f->is_at_destination&&!f->following_path){call(*s,*c,raise_event,0x3f);return 1;}
  move_type(*s,*f,*c);
  if(!(std::fabs(f->walk_speed-s->cached_speed)<0.0001f))speed(*s,*c,f->walk_speed);
 }else if(s->current==5){call(*s,*c,look_at,0,0,0,0,f->target);if(!(std::fabs(f->attack_speed-s->cached_speed)<0.0001f))speed(*s,*c,f->attack_speed);}
 return 1;
}
extern "C" int dh2_character_combat_state_update(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::CombatFacts* combat,
 std::uint32_t dt,const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!combat||combat->controller_present>1||
    combat->interrupted_byte_441>1||combat->interrupted_byte_available>1||
    combat->last_target_available>1||!combat_id(s->current))return -1;
 s->elapsed_ms+=dt;
 // IDA CSScared::OnUpdate 0x3c4788 tests Character+0x528 bit 4 (SM_SetScareState's
 // bit and timer event 44); CSStunned::OnUpdate 0x3c549c tests bit 2 (event 43).
 if(s->current==8){if(!(combat->machine_flags_44&0x4u))call(*s,*c,stop_loop);}
 else if(s->current==9&&!(combat->machine_flags_44&0x2u)){
  s->idle_suppressed=0;return transition(*s,*f,3,0,0,*c,combat);
 }
 return 1;
}
extern "C" int dh2_character_skill_state_transition(dh2::character::State* s,
 const dh2::character::Facts* f,
 dh2::character_skill_state_dispatch_v1::Projection* projection,
 std::int32_t next,std::int32_t event,std::uint64_t payload,
 const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!projection||projection->machine!=s||
    !skill_id(s->current)||next==-1||!skill_id(next))return -1;
 // State 6's registered graph only connects to the selected states below.
 // Entry is selected from Move/Attack/Idle (3/4/5) by C355.
 if((next==6&&s->current!=3&&s->current!=4&&s->current!=5&&s->current!=6&&
     !(s->current==11&&event==50005))||
    (s->current==6&&next!=3&&next!=4&&next!=5&&next!=6&&next!=12))return -1;
 return skill_transition(*s,*f,*projection,next,event,payload,*c);
}
extern "C" int dh2_character_combat_skill_state_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::CombatFacts* combat,
 dh2::character_skill_state_dispatch_v1::Projection* projection,
 std::int32_t next,std::int32_t event,std::uint64_t payload,
 const dh2::character::Services* c){
 using namespace dh2::character;
 // Two registered edges only: CSInjured event 50005 resumes CSSkill, and a
 // direct _SetState (F_ApplyResult special) leaves a running CSSkill for one of
 // the combat reaction states 8..11 through CSSkill::OnBlur.
 const bool resume=s&&combat_id(s->current)&&event==50005&&s->current==11&&next==6;
 const bool interrupt=s&&s->current==6&&combat_id(next);
 if(!valid_base(s,f,c)||!combat||!projection||projection->machine!=s||
    (!resume&&!interrupt)||
    combat->controller_present>1||combat->interrupted_byte_441>1||
    combat->interrupted_byte_available>1||combat->last_target_available>1)return -1;
 return skill_transition(*s,*f,*projection,next,event,payload,*c,combat);
}
extern "C" int dh2_character_combat_cast_state_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::CombatFacts* combat,
 dh2::character_cast_lifecycle_v1::Projection* projection,
 std::int32_t next,std::int32_t event,std::uint64_t payload,
 const dh2::character::Services* c){
 using namespace dh2::character;
 // A direct _SetState (F_ApplyResult special) leaves CSCast through
 // CSCast::OnBlur for one of the combat reaction states 8..11.
 if(!valid_base(s,f,c)||!combat||!projection||s->current!=7||!combat_id(next)||
    combat->controller_present>1||combat->interrupted_byte_441>1||
    combat->interrupted_byte_available>1||combat->last_target_available>1)return -1;
 return cast_transition(*s,*f,*projection,next,event,payload,*c,combat);
}
extern "C" int dh2_character_cast_state_transition(dh2::character::State* s,
 const dh2::character::Facts* f,
 dh2::character_cast_lifecycle_v1::Projection* projection,
 std::int32_t next,std::int32_t event,std::uint64_t payload,
 const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!projection||next==2||(next!=7&&!id(next))||
    (s->current!=3&&s->current!=4&&s->current!=5&&s->current!=7))return -1;
 if((next==7&&(s->current!=3&&s->current!=4&&s->current!=5))||
    (s->current==7&&next!=3&&next!=12))return -1;
 return cast_transition(*s,*f,*projection,next,event,payload,*c);
}
extern "C" int dh2_character_anim_state_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::AnimFacts* anim,
 const dh2::character::CombatFacts* combat,std::int32_t next,std::int32_t event,
 std::uint64_t payload,const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!anim_facts_valid(anim))return -1;
 if(combat&&(combat->controller_present>1||combat->interrupted_byte_441>1||
    combat->interrupted_byte_available>1||combat->last_target_available>1))return -1;
 if(!anim_partner(*s,next,combat!=nullptr))return -1;
 return transition(*s,*f,next,event,payload,*c,combat,anim);
}
extern "C" int dh2_character_anim_spawn_state_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::SpawnFacts* sf,
 const dh2::character::AnimFacts* anim,std::int32_t event,
 const dh2::character::Services* c){
 using namespace dh2::character;
 // Idle (3) and the uninitialised machine use the base entry point.
 if(!valid_spawn(s,f,sf,c)||!anim_facts_valid(anim)||s->current==3||s->current==-1)return -1;
 const int prior=s->current;
 source_blur(*s,*f,*sf,*c);s->current=14;s->elapsed_ms=0;
 focus(*s,*f,prior,0,*c,nullptr,anim);
 call(*s,*c,raise_event,0x1d,prior,0,0,std::uint64_t(std::int64_t(prior)));
 (void)event;return 1;
}
extern "C" int dh2_character_anim_limbus_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::SpawnFacts* sf,
 const dh2::character::AnimFacts* anim,std::int32_t event,
 const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_spawn_facts(s,f,sf,c)||!anim_facts_valid(anim)||s->current!=14)return -1;
 // CSAnim::OnBlur 0x3c2dd0 only reads the isTracingCharState switch.
 const int prior=s->current;
 s->current=0;s->elapsed_ms=0;
 limbus_focus(*s,*sf,*c);
 call(*s,*c,raise_event,0x1d,prior,0,0,std::uint64_t(std::int64_t(prior)));
 (void)event;return 1;
}
extern "C" int dh2_character_anim_skill_state_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::AnimFacts* anim,
 dh2::character_skill_state_dispatch_v1::Projection* projection,
 std::int32_t next,std::int32_t event,std::uint64_t payload,
 const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!anim_facts_valid(anim)||!projection||projection->machine!=s||
    !((s->current==6&&next==14)||(s->current==14&&next==6)))return -1;
 return skill_transition(*s,*f,*projection,next,event,payload,*c,nullptr,anim);
}
extern "C" int dh2_character_anim_cast_state_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::AnimFacts* anim,
 dh2::character_cast_lifecycle_v1::Projection* projection,
 std::int32_t next,std::int32_t event,std::uint64_t payload,
 const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!anim_facts_valid(anim)||!projection||s->current!=7||next!=14)return -1;
 return cast_transition(*s,*f,*projection,next,event,payload,*c,nullptr,anim);
}
extern "C" int dh2_character_anim_state_event(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::AnimFacts* anim,
 dh2::character_skill_state_dispatch_v1::Projection* projection,
 std::uint32_t event,std::uint64_t payload,const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!anim_facts_valid(anim)||s->current!=14)return -1;
 if(event>=0x2a&&event<=0x2c)s->attack_gate&=~(1u<<(event-0x2a));
 // CSAnim::OnEvent 0x3c1a14: event 34 with Character+0x540 set calls
 // SM_SetIdleState(false), i.e. the idle-suppression byte is cleared first.
 if(event==34){
  if(!anim->wait_for_end_540)return 0;
  s->idle_suppressed=0;
  return transition(*s,*f,3,34,payload,*c,nullptr,anim);
 }
 // CSAnim::OnInit 0x3c8b28 registers 50001 -> CSMove and 50005 -> CSSkill, both
 // without a predicate.
 if(event==50001)return transition(*s,*f,4,std::int32_t(event),payload,*c,nullptr,anim);
 if(event==50005){
  if(!projection||projection->machine!=s)return -1;
  return skill_transition(*s,*f,*projection,6,std::int32_t(event),payload,*c,nullptr,anim);
 }
 return 0;
}
extern "C" int dh2_character_anim_state_update(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::AnimFacts* anim,
 std::uint32_t dt,const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_base(s,f,c)||!anim_facts_valid(anim)||s->current!=14)return -1;
 s->elapsed_ms+=dt;
 // CSAnim::OnUpdate 0x3c1a3c: a Character that is not scripted and carries the
 // +0x541 byte calls SM_SetIdleState(Character+0x1480), which is zero here.
 if(!anim->scripted_1480&&anim->idle_on_update_541){
  s->idle_suppressed=anim->scripted_1480;
  return transition(*s,*f,3,0,0,*c,nullptr,anim);
 }
 return 1;
}
extern "C" int dh2_character_attack_speed(float* out,const std::int32_t* properties){
 if(!out||!properties)return -1;
 float value=static_cast<float>(properties[48]);value=value*0.00390625f;
 value=value*0.01f;value=value+1.0f;*out=value>0.0f?value:0.0f;return 1;
}
extern "C" int dh2_character_state_is_idle(std::int32_t current,std::uint32_t include_dialog){
 if(include_dialog>1)return -1;
 return current==3||current==13||(current==18&&!include_dialog);
}
extern "C" int dh2_character_spawn_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::SpawnFacts* sf,
 std::int32_t next,const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_spawn(s,f,sf,c)||(next!=0&&next!=1&&next!=3))return -1;
 return spawn_transition(*s,*f,*sf,next,*c);
}
extern "C" int dh2_character_spawn_dead_transition(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::SpawnFacts* sf,
 std::uint64_t payload,const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_spawn(s,f,sf,c)||s->current!=1)return -1;
 const int prior=s->current;
 source_blur(*s,*f,*sf,*c);s->current=12;s->elapsed_ms=0;
 focus(*s,*f,prior,payload,*c);
 call(*s,*c,raise_event,0x1d,prior,0,0,std::uint64_t(std::int64_t(prior)));
 return 1;
}
extern "C" int dh2_character_spawn_event(dh2::character::State* s,
 const dh2::character::Facts* f,const dh2::character::SpawnFacts* sf,
 std::uint32_t event,const char* event_name,const dh2::character::Services* c){
 using namespace dh2::character;
 if(!valid_spawn(s,f,sf,c))return -1;
 if(event==0x28){
  if(s->current!=1||!event_name)return -1;
  std::size_t length=0;while(length<64&&event_name[length])++length;
  if(length==64)return -1;
  if(std::strcmp(event_name,"is_interactive")!=0)return 0;
  s->flags|=0x2000u;
  call(*s,*c,init_physical_object);s->body_present=1;
  return 1;
 }
 if(s->current==0&&event==0x2f){
  if(!sf->can_respawn)return 0;
  return spawn_transition(*s,*f,*sf,1,*c);
 }
 if(s->current==1&&event==0x22)
  return spawn_transition(*s,*f,*sf,3,*c);
 if(s->current==1&&event==50008){
  // The legacy public event wrapper has no payload; the Coordinator calls
  // dh2_character_spawn_dead_transition directly for the real opaque value.
  return dh2_character_spawn_dead_transition(s,f,sf,0,c);
 }
 if(s->current==3)
  return dh2_character_state_event(s,f,event,0,c);
 // CSLimbus::OnEvent and CSSpawn::OnEvent have no other source-owned effect.
 return 0;
}
extern "C" int dh2_character_spawn_update(const dh2::character::State* s,
 const dh2::character::SpawnFacts* sf){
 if(!s||!sf||!spawn_state_id(s->current)||sf->reserved_zero!=0||
    sf->visual_present>1||!std::isfinite(sf->raw_fade_in_argument)||sf->raw_fade_in_argument<0)
  return -1;
 return 0;
}
