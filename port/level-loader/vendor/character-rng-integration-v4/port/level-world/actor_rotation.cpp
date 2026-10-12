#include "actor_rotation.hpp"
#include <cmath>
#include <cstring>
#include <cstddef>
namespace {
float constant(std::uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
float mul(float x,float y){volatile float f=x*y;return f;}
float add(float x,float y){volatile float f=x+y;return f;}
float sub(float x,float y){volatile float f=x-y;return f;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<an:x-y<bn;}
}
extern "C" int dh2_actor_update_rotation(dh2::actor::RotationState* state,const dh2::actor::RotationPolicy* policy,std::uint32_t* sync){
 if(!state||!policy||!sync||overlap(state,sizeof(*state),policy,sizeof(*policy))||overlap(sync,4,state,sizeof(*state))||overlap(sync,4,policy,sizeof(*policy)))return 1;
 if(state->reserved||state->turn_positive>1||policy->visual_present>1||policy->visual_with_rotation>1||!std::isfinite(state->rotation[0])||!std::isfinite(state->rotation[1])||!std::isfinite(state->rotation[2])||!std::isfinite(state->heading_angle)||!std::isfinite(policy->speed))return 1;
 auto next=*state;
 if(policy->speed<0.f)next.rotation[2]=next.heading_angle;
 else {
  const float rate=mul(policy->speed,constant(0x41490fdb));const float seconds=mul(static_cast<float>(policy->dt_ms),constant(0x3a83126f));const float max_delta=mul(rate,seconds);float delta=sub(next.heading_angle,next.rotation[2]);
  if(!std::isfinite(max_delta)||!std::isfinite(delta))return 1;
  if(delta>constant(0x40490fdb))delta=sub(delta,constant(0x40c90fdb));else if(delta<constant(0xc0490fdb))delta=add(delta,constant(0x40c90fdb));
  if(std::fabs(delta)<max_delta)next.rotation[2]=next.heading_angle;
  else if(delta<0.f){next.rotation[2]=sub(next.rotation[2],max_delta);next.turn_positive=0;}
  else {next.rotation[2]=add(next.rotation[2],max_delta);next.turn_positive=1;}
  if(!std::isfinite(next.rotation[2]))return 1;
 }
 *state=next;*sync=policy->visual_present&&policy->visual_with_rotation;return 0;
}
