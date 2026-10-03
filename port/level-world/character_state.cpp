#include "character_state.hpp"
#include <cmath>
#include <cstring>
namespace {
using namespace dh2::character;
bool id(int value){return value==-1||value==3||value==4||value==5||value==12;}
bool valid(const State* s,const Facts* f,const Services* c){
 return s&&f&&c&&c->invoke&&id(s->current)&&f->is_player<=1&&!f->reserved&&
 f->is_at_destination<=1&&f->following_path<=1&&f->has_ranged_weapon<=1&&
 s->idle_suppressed<=255&&s->stop_attack_allowed<=255&&s->heading_active<=255&&
 s->controller_locked<=255&&s->dead_alternate<=255&&s->body_present<=1&&s->move_type<=2;
}
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
void blur(State& s,const Facts& f,const Services& c){
 switch(s.current){
 case 3:s.idle_suppressed=0;break;
 case 4:call(s,c,stop);if(s.body_present)call(s,c,pin);break;
 case 5:if(f.attack_delay)call(s,c,start_timer,std::int32_t(f.attack_delay),0,0x2a);if(s.body_present)call(s,c,pin);break;
 case 12:s.controller_locked=0;if(s.body_present)call(s,c,reset_filter);break;
 }
}
void focus(State& s,const Facts& f,int prior,std::uint64_t payload,const Services& c){
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
 }
}
int transition(State& s,const Facts& f,int next,int event,std::uint64_t payload,const Services& c){
 const int prior=s.current;blur(s,f,c);s.current=next;if(prior!=next)s.elapsed_ms=0;
 focus(s,f,prior,payload,c);call(s,c,raise_event,0x1d,prior,0,0,std::uint64_t(std::int64_t(prior)));
 (void)event;return 1;
}
}
extern "C" int dh2_character_state_transition(dh2::character::State* s,const dh2::character::Facts* f,
 std::int32_t next,std::int32_t event,std::uint64_t payload,const dh2::character::Services* c){
 if(!valid(s,f,c)||next==-1||!id(next))return -1;
 return transition(*s,*f,next,event,payload,*c);
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
 int next=-1;
 if(event==0xc358&&(s->current==3||s->current==4||s->current==5))next=12;
 else if(event==0xc354&&(s->current==3||s->current==4)&&!(s->attack_gate&1))next=5;
 else if(event==0xc351&&(s->current==3||(s->current==5&&s->stop_attack_allowed)))next=4;
 else if((event==0x22&&s->current==5)||(event==0x3f&&s->current==4)||
         ((event==0xc352||event==0x22||event==0x23)&&s->current==3))next=3;
 return next<0?0:transition(*s,*f,next,std::int32_t(event),payload,*c);
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
extern "C" int dh2_character_attack_speed(float* out,const std::int32_t* properties){
 if(!out||!properties)return -1;
 float value=static_cast<float>(properties[48]);value=value*0.00390625f;
 value=value*0.01f;value=value+1.0f;*out=value>0.0f?value:0.0f;return 1;
}
extern "C" int dh2_character_state_is_idle(std::int32_t current,std::uint32_t include_dialog){
 if(include_dialog>1)return -1;
 return current==3||current==13||(current==18&&!include_dialog);
}
