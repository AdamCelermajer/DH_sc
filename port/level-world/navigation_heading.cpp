#include "navigation_heading.hpp"
#include <cmath>
#include <cstring>
namespace {
float from_word(std::uint32_t word){float value;std::memcpy(&value,&word,4);return value;}
float add(float a,float b){volatile float value=a+b;return value;}
float mul(float a,float b){volatile float value=a*b;return value;}
float divide(float a,float b){volatile float value=a/b;return value;}
bool finite3(const float* p){return p&&std::isfinite(p[0])&&std::isfinite(p[1])&&std::isfinite(p[2]);}
void look_unchecked(float& angle,const float* direction){
 const float x=direction[0],y=direction[1];
 if(y==0){if(x>0)angle=from_word(0x3fc90fdb);else if(x<0)angle=from_word(0x4096cbe4);return;}
 float next=std::atan(divide(x,-y));
 if(y>0)next=add(x>0?from_word(0x40490fdb):from_word(0xc0490fdb),next);
 angle=next;
}
}
void dh2::navigation::look_towards_unchecked(float& angle,const float* direction){
 look_unchecked(angle,direction);
}
extern "C" int dh2_nav_look_towards(float* angle,const float* direction){
 if(!angle||!std::isfinite(*angle)||!finite3(direction))return 1;
 look_unchecked(*angle,direction);return 0;
}
void dh2::navigation::set_heading_unchecked(HeadingState& state,const float* direction,std::uint32_t rotate){
 auto next=state;next.direction[0]=direction[0];next.direction[1]=direction[1];next.direction[2]=0;
 const float length=add(add(mul(next.direction[0],next.direction[0]),mul(next.direction[1],next.direction[1])),0);
 next.active=length>from_word(0x38d1b717);
 if(length>1){const float scale=divide(1.f,std::sqrt(length));for(auto& component:next.direction)component=mul(component,scale);}
 state=next;
 if(next.active&&rotate)look_unchecked(state.angle,direction);
}
extern "C" int dh2_nav_set_heading(dh2::navigation::HeadingState* state,const float* direction,std::uint32_t rotate){
 if(!state||state->reserved||rotate>1||!std::isfinite(state->angle)||!finite3(direction))return 1;
 const float length=add(add(mul(direction[0],direction[0]),mul(direction[1],direction[1])),0);
 if(!std::isfinite(length))return 1;
 dh2::navigation::set_heading_unchecked(*state,direction,rotate);
 return 0;
}
