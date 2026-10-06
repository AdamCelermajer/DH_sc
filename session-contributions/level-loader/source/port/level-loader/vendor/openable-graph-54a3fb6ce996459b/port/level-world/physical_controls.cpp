#include "physical_controls.hpp"
#include <cstdint>
namespace {
using dh2::physical::BodyState;
using dh2::physical::TransformRequest;
bool overlap(const void* a,std::size_t as,const void* b,std::size_t bs){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return x<=y?y-x<as:x-y<bs;
}
bool valid(const BodyState* s){return s&&s->flags<=0xffffu;}
void wake(BodyState& s){s.flags&=~dh2::physical::sleeping_flag;s.sleep_time=0.f;}
void linear(BodyState& s,float x,float y){if(x!=0.f||y!=0.f)wake(s);s.linear_velocity[0]=x;s.linear_velocity[1]=y;}
void position(TransformRequest& out,const BodyState& s,float x,float y){out.position[0]=x*.01f;out.position[1]=y*.01f;out.angle=s.angle;out.pending=1;}
}
extern "C" int dh2_physical_wake(BodyState* s){if(!valid(s))return 1;wake(*s);return 0;}
extern "C" int dh2_physical_set_linear(BodyState* s,const float* xy){
 if(!valid(s)||!xy)return 1;const float x=xy[0],y=xy[1];linear(*s,x,y);return 0;
}
extern "C" int dh2_physical_add_linear(BodyState* s,const float* values){
 if(!valid(s)||!values)return 1;const float dx=values[0],dy=values[1],cx=values[2],cy=values[3];
 if(dx!=0.f||dy!=0.f)wake(*s);
 float x=dx+s->linear_velocity[0],y=dy+s->linear_velocity[1];if(x>cx)x=cx;if(y>cy)y=cy;
 s->linear_velocity[0]=x;s->linear_velocity[1]=y;return 0;
}
extern "C" int dh2_physical_set_angular(BodyState* s,const float* angular){
 if(!valid(s)||!angular)return 1;const float value=*angular;wake(*s);s->angular_velocity=value;return 0;
}
extern "C" int dh2_physical_query(float* out,const BodyState* s){
 if(!valid(s)||!out||overlap(out,16,s,sizeof(*s)))return 1;
 out[0]=s->position[0]*100.f;out[1]=s->position[1]*100.f;out[2]=s->angle;out[3]=s->radius*100.f;return 0;
}
extern "C" int dh2_physical_request_position(TransformRequest* out,const BodyState* s,const float* xy){
 if(!valid(s)||!out||!xy||overlap(out,sizeof(*out),s,sizeof(*s)))return 1;
 const float x=xy[0],y=xy[1];position(*out,*s,x,y);return 0;
}
extern "C" int dh2_physical_stop_begin(TransformRequest* out,BodyState* s,const float* xy){
 if(!valid(s)||!out||!xy||overlap(out,sizeof(*out),s,sizeof(*s)))return 1;
 const float x=xy[0],y=xy[1];linear(*s,0.f,0.f);wake(*s);s->angular_velocity=0.f;position(*out,*s,x,y);return 0;
}
extern "C" int dh2_physical_stop_finish(BodyState* s){
 if(!valid(s))return 1;s->flags|=dh2::physical::sleeping_flag;
 s->linear_velocity[0]=s->linear_velocity[1]=s->angular_velocity=s->force[0]=s->force[1]=s->torque=s->sleep_time=0.f;
 return 0;
}
