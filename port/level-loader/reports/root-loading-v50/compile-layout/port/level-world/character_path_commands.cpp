#include "character_path_commands.hpp"
#include "navigation_heading.hpp"
#include <cstddef>
namespace {
float sub(float a,float b){volatile float r=a-b;return r;}
float mul(float a,float b){volatile float r=a*b;return r;}
float add(float a,float b){volatile float r=a+b;return r;}
bool aligned(const void* p,std::uintptr_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
}
extern "C" int dh2_character_path_to(dh2::character::PathToResult16* out,
 const dh2::character::PathToState40* state,const float* target,
 const dh2::character::PathToServices16* services){
 using namespace dh2::character;
 if(!aligned(out,alignof(PathToResult16))||!aligned(state,alignof(PathToState40))||!aligned(target,alignof(float))||
    !aligned(services,alignof(PathToServices16))||!state->owner||state->disabled>255||state->path_nonempty>1||state->reserved||state->reserved1||
    overlap(out,sizeof(*out),state,sizeof(*state))||overlap(out,sizeof(*out),target,12)||overlap(out,sizeof(*out),services,sizeof(*services)))return 1;
 bool request=!state->disabled;
 if(request&&state->path_nonempty){
  const float x=sub(state->path_target[0],target[0]),y=sub(state->path_target[1],target[1]),z=sub(state->path_target[2],target[2]);
  request=add(add(mul(x,x),mul(y,y)),mul(z,z))>40000.f;
 }
 PathToResult16 result{};
 if(request){
  if(!services->find_path)return 2;
  const PathToRequest32 call{state->owner,state->limit?state->limit:30,0,{target[0],target[1],target[2]},0};
  result.requested=1;result.limit=call.limit;
  if(services->find_path(services->context,&call,&result.find_result)!=0)return 2;
 }
 *out=result;return 0;
}
extern "C" int dh2_character_look_at_point(dh2::character::LookAtState16* state,const float* target){
 if(!aligned(state,alignof(dh2::character::LookAtState16))||!aligned(target,alignof(float)))return 1;
 // Source computes Y, then Z, then X before calling LookTowards.
 const float y=sub(target[1],state->position[1]),z=sub(target[2],state->position[2]),x=sub(target[0],state->position[0]);
 const float direction[3]{x,y,z};
 dh2::navigation::look_towards_unchecked(state->heading_angle,direction);return 0;
}
