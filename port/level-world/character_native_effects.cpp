#include "character_native_effects.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
bool valid(const NativeEffects32* e,const NativeEffectServices16* c){return e&&e->fsm&&e->fsm->state&&e->fsm->character&&e->fsm->current_present<=1&&!e->fsm->reserved&&!e->reserved&&e->count<=0x7fffffffu&&(!e->count||(e->stunned&&e->scared))&&c&&c->invoke;}
int call(NativeEffects32* e,const NativeEffectServices16* c,NativeEffectService kind,std::uint32_t a=0,std::uint32_t b=0,std::uint32_t d=0,std::uintptr_t payload=0,std::uint32_t* output=nullptr){std::uint32_t ignored=0;const NativeEffectRequest32 r{kind,a,b,d,e->fsm->character,payload};return c->invoke(c->context,e,&r,output?output:&ignored)?-2:1;}
std::int32_t signed_word(std::uint32_t v){std::int32_t result;std::memcpy(&result,&v,4);return result;}
float floating(std::uint32_t v){float result;std::memcpy(&result,&v,4);return result;}
std::uint32_t word(float v){std::uint32_t result;std::memcpy(&result,&v,4);return result;}
int random_heading(NativeEffects32* e,const NativeEffectServices16* c){
 std::uint32_t x=0,y=0,sx=0,sy=0;auto result=call(e,c,effect_random,9998,0,0,0,&x);if(result<0)return result;
 float xx=static_cast<float>(signed_word(x));xx=xx*floating(0x38d1b717u);xx=xx+floating(0x3951b717u);
 result=call(e,c,effect_random,9998,0,0,0,&y);if(result<0)return result;
 float yy=static_cast<float>(signed_word(y));yy=yy*floating(0x38d1b717u);yy=yy+floating(0x3951b717u);
 result=call(e,c,effect_random,100,0,0,0,&sx);if(result<0)return result;
 const auto wx=word(xx)+(signed_word(sx)<=49?0x80000000u:0u);
 result=call(e,c,effect_random,100,0,0,0,&sy);if(result<0)return result;
 return call(e,c,effect_heading_point,wx,word(yy)+(signed_word(sy)<=49?0x80000000u:0u),0);
}
}
extern "C" int dh2_character_native_effect_set(NativeEffects32* e,std::uint32_t kind,std::uint32_t duration,std::uint32_t mode,std::uintptr_t payload,std::uint32_t force,const NativeEffectServices16* c){
 if(!valid(e,c)||kind>1||mode>1||force>1)return -1;
 std::uint32_t flags=0,index=0;auto result=call(e,c,effect_ai_flags,0,0,0,0,&flags);if(result<0)return result;if(flags&4)return 0;
 result=call(e,c,effect_animation_index,0,0,0,0,&index);if(result<0)return result;
 if(signed_word(index)<0||signed_word(index)>=std::int32_t(e->count))index=17;
 if(index>=e->count)return 0;
 const auto state=e->fsm->state;const std::uint32_t pending=kind?4:2;
 if(!(state->attack_gate&pending)){result=call(e,c,effect_start_timer,duration,0,kind?0x2c:0x2b);if(result<0)return result;state->attack_gate|=pending;}
 // Original re-reads the table pointer after the timer callback, then captures
 // the selected row value before constant/stance callbacks. Providers must
 // retain valid arrays; native malformed replacement remains an explicit error.
 if(index>=e->count||!(kind?e->scared:e->stunned))return -2;
 const auto base=static_cast<std::uint32_t>((kind?e->scared:e->stunned)[index]);std::uint32_t modifier=0;
 result=call(e,c,effect_stance_bits,0,0,0,0,&modifier);if(result<0)return result;
 if(modifier&(kind?0x100:0x200)){result=call(e,c,effect_anim_stance,0,0,0,0,&modifier);if(result<0)return result;}else modifier=0;
 state->animation_override=signed_word(base+modifier);
 result=force?call(e,c,effect_force_state,kind?8:9,kind?0xc35d:0xc35c,0,payload):call(e,c,effect_state_event,kind?0xc35d:0xc35c,0,0,payload);if(result<0)return result;
 if(mode)state->flags|=kind?0x400:0x800;
 return 1;
}
extern "C" int dh2_character_native_effect_focus(NativeEffects32* e,std::uint32_t kind,const NativeEffectServices16* c){
 if(!valid(e,c)||kind>1)return -1;
 const auto state=e->fsm->state;state->flags=kind?0x2240:0x2202;
 // Source captures the global row array before the Character index query.
 const auto rows=kind?e->scared:e->stunned;std::uint32_t index=0,modifier=0;
 auto result=call(e,c,effect_animation_index,0,0,0,0,&index);if(result<0)return result;
 if(signed_word(index)<0||signed_word(index)>=std::int32_t(e->count))index=17;
 if(index>=e->count||!rows)return -2;
 const auto base=static_cast<std::uint32_t>(rows[index]);
 result=call(e,c,effect_stance_bits,0,0,0,0,&modifier);if(result<0)return result;
 if(modifier&(kind?0x100:0x200)){result=call(e,c,effect_anim_stance,0,0,0,0,&modifier);if(result<0)return result;}else modifier=0;
 result=call(e,c,effect_set_animation,base+modifier);if(result<0)return result;
 if(kind){result=random_heading(e,c);if(result<0)return result;}
 else{std::uint32_t player=0;result=call(e,c,effect_is_player,0,0,0,0,&player);if(result<0)return result;if(player)state->controller_locked=1;}
 result=call(e,c,effect_cancel_sneaking);if(result<0)return result;
 if(state->body_present)return call(e,c,effect_unpin);
 return 1;
}
extern "C" int dh2_character_native_effect_event(NativeEffects32* e,std::uint32_t kind,std::uint32_t event,const NativeEffectServices16* c){
 if(!valid(e,c)||kind>1)return -1;
 return kind&&event==0x23?random_heading(e,c):1;
}
extern "C" int dh2_character_native_effect_body(NativeEffects32* e,std::uint32_t kind,std::uint32_t operation,const NativeEffectServices16* c){
 if(!valid(e,c)||kind>1||operation>1)return -1;
 const auto state=e->fsm->state;
 if(!operation){if(state->attack_gate&(kind?4:2))return 1;if(kind)return call(e,c,effect_stop_loop);state->idle_suppressed=0;return call(e,c,effect_force_state,3,0xffffffffu);}
 if(kind){const auto result=call(e,c,effect_heading_object);if(result<0)return result;}else state->controller_locked=0;
 if(state->body_present)return call(e,c,effect_pin);
 return 1;
}
