#include "animation_blender.hpp"
#include <cmath>
#include <cstring>

namespace {
bool valid(const dh2::animation::BlenderState* state){
 return state&&state->current<2&&state->previous<2&&
  std::isfinite(state->reciprocal)&&std::isfinite(state->weights[0])&&std::isfinite(state->weights[1]);
}
std::int32_t signed_word(std::uint32_t value){
 std::int32_t result;std::memcpy(&result,&value,4);return result;
}
}
extern "C" int dh2_blender_begin(dh2::animation::BlenderState* state,std::int32_t next_duration){
 if(!valid(state))return 1;
 state->previous=state->current;
 state->current=(state->current+1)%2;
 state->remaining=state->duration;
 if(state->duration>0)state->reciprocal=1.0f/static_cast<float>(state->duration);
 state->duration=next_duration<0?0:next_duration;
 return 0;
}
extern "C" int dh2_blender_update_weights(dh2::animation::BlenderState* state,std::uint32_t timestamp){
 if(!valid(state))return 1;
 if(state->remaining<0)return 0;
 state->remaining=signed_word(static_cast<std::uint32_t>(state->remaining)-(timestamp-state->last_time));
 if(state->remaining>0){
  const float outgoing=static_cast<float>(state->remaining)*state->reciprocal;
  state->weights[state->previous]=outgoing;
  state->weights[state->current]=1.0f-outgoing;
 }else{
  state->weights[state->previous]=0.0f;
  state->weights[state->current]=1.0f;
 }
 return 0;
}
extern "C" int dh2_blender_normalize(dh2::animation::BlenderState* state){
 if(!valid(state))return 1;
 // Preserve source accumulation starting at positive zero and its exceptional
 // zero-sum fallback, which changes the first slot only.
 const float sum=(0.0f+state->weights[0])+state->weights[1];
 if(sum==0.0f)state->weights[0]=1.0f;
 else{state->weights[0]=state->weights[0]/sum;state->weights[1]=state->weights[1]/sum;}
 return 0;
}
