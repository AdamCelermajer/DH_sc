#include "character_native_fsm.hpp"
#include <cstring>
#include <cstdio>
namespace {
using namespace dh2::character;
bool valid(const NativeFsm24* f){return f&&f->state&&f->current_present<=1&&!f->reserved;}
int send(NativeFsm24* f,const NativeFsmServices16* c,NativeFsmService service,std::uint32_t a=0,std::uint32_t b=0,std::uintptr_t subject=0,std::uint32_t* out=nullptr){std::uint32_t ignored=0;const NativeFsmRequest32 request{service,a,b,0,subject,0};return c->invoke(c->context,f,&request,out?out:&ignored)?-2:1;}
int script_get(void* context,std::uint32_t selector,dh2_script_value* values,std::uint32_t capacity,std::uint32_t* count,char* error,std::size_t size){
 if(!values||!capacity||!count){if(error&&size)std::snprintf(error,size,"native FSM getter malformed output");return 1;}
 std::int32_t integer=0;const auto result=dh2_character_native_fsm_get_integer(&integer,static_cast<const NativeFsm24*>(context),selector);
 if(result<0){if(error&&size)std::snprintf(error,size,"native FSM getter malformed Character");return 1;}
 values[0]={};values[0].type=DH2_SCRIPT_NUMBER;values[0].number=static_cast<float>(integer);*count=1;return 0;
}
}
extern "C" int dh2_character_native_fsm_get_integer(std::int32_t* out,const NativeFsm24* f,std::uint32_t selector){
 if(!out||!valid(f)||selector>1)return -1;
 if(selector==0)*out=f->current_present?f->state->current:-1;
 else std::memcpy(out,&f->state->elapsed_ms,sizeof(*out));
 return 1;
}
extern "C" int dh2_character_native_fsm_preset_state(std::int32_t* out,const char* text){if(!out||!text)return -1;*out=!std::strcmp(text,"Limbus")?0:!std::strcmp(text,"PreSpawn")?17:3;return 1;}
extern "C" int dh2_character_native_fsm_update(NativeFsm24* f,const NativeFsmServices16* c){
 if(!valid(f)||!f->character||!c||!c->invoke)return -1;
 auto result=send(f,c,fsm_profile_begin);if(result<0)return result;
 const auto state=f->state;const auto previous=state->elapsed_ms;std::uint32_t dt=0;
 result=send(f,c,fsm_engine_dt,0,0,0,&dt);if(result<0)return result;
 state->elapsed_ms=previous+dt;
 if(state->attack_gate&2){
  if(!f->current_present||state->current!=9){result=send(f,c,fsm_set_stun,0xffffffffu,(state->flags>>11)&1,f->character);if(result<0)return result;}
 }
 if(state->attack_gate&4){
  if(!f->current_present||state->current!=8){result=send(f,c,fsm_set_scare,0xffffffffu,(state->flags>>10)&1,f->character);if(result<0)return result;}
 }
 if(f->current_present){result=send(f,c,fsm_current_update,static_cast<std::uint32_t>(state->current),0,f->character);if(result<0)return result;}
 return send(f,c,fsm_profile_end);
}
extern "C" int dh2_character_native_fsm_bounded_tail(NativeFsm24* f,const Facts* facts,const Services* services){
 if(!valid(f)||!facts||!services||!services->invoke)return -1;
 if(!f->current_present)return 0;
 if(f->state->current!=3&&f->state->current!=4&&f->state->current!=5&&f->state->current!=12)return -2;
 return dh2_character_state_update(f->state,facts,0,services);
}
extern "C" int dh2_character_native_fsm_script_get_state(void* context,const dh2_script_value*,std::uint32_t,dh2_script_value* values,std::uint32_t capacity,std::uint32_t* count,char* error,std::size_t size){return script_get(context,0,values,capacity,count,error,size);}
extern "C" int dh2_character_native_fsm_script_get_time(void* context,const dh2_script_value*,std::uint32_t,dh2_script_value* values,std::uint32_t capacity,std::uint32_t* count,char* error,std::size_t size){return script_get(context,1,values,capacity,count,error,size);}
