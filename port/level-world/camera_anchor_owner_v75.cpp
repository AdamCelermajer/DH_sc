#include "camera_anchor_owner_v75.hpp"
#include <cmath>
#include <cstring>
#include <limits>
namespace dh2::camera {namespace {
//Original soft-float operation boundaries: keep each intermediate rounded to
//float, with no FMA or graphics-dependent approximation of source arithmetic.
float add(float a,float b){volatile float r=a+b;return r;}
float sub(float a,float b){volatile float r=a-b;return r;}
float mul(float a,float b){volatile float r=a*b;return r;}
float div(float a,float b){volatile float r=a/b;return r;}
float square(const std::array<float,3>& p){return add(add(mul(p[0],p[0]),mul(p[1],p[1])),mul(p[2],p[2]));}
float angle(const std::array<float,3>& a,const std::array<float,3>& b){
 const auto dot=add(add(mul(a[0],b[0]),mul(a[1],b[1])),mul(a[2],b[2]));
 return std::acos(div(dot,mul(std::sqrt(square(a)),std::sqrt(square(b)))));
}
std::array<float,3> normalized(std::array<float,3> v){const auto length=std::sqrt(square(v));for(auto& x:v)x=div(x,length);return v;}
std::int32_t signed_bits(std::uint32_t v){std::int32_t r;std::memcpy(&r,&v,4);return r;}
struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}};
}
CameraAnchorOwnerV75::CameraAnchorOwnerV75(CameraAnchorServicesV75 s):services_(std::move(s)){}
bool CameraAnchorOwnerV75::fail(const char* why,std::string& e){if(!failed_){failed_=true;failure_=e.empty()?why:e;}e=failure_;return false;}
bool CameraAnchorOwnerV75::actor(CameraAnchorActorV75& out,std::string& e){
 if(!services_.provider||!services_.actor||!services_.actor(fields_.actor8,out,e)||!out.receiver||out.identity!=fields_.actor8||!out.position160)
  return fail("Required SAME Anchor.GameObject8/Position160",e);
 return true;
}
bool CameraAnchorOwnerV75::reset(std::string& e){
 CameraAnchorActorV75 actual;if(!actor(actual,e))return false;
 for(unsigned i=0;i<3;++i)fields_.position_c[i]=actual.position160[i];
 if(forward_)fields_.previous2c={};e.clear();return true;
}
bool CameraAnchorOwnerV75::construct(std::uintptr_t id,bool forward,float maximum,float speed,float threshold,std::string& e){
 if(attempted_||failed_)return fail("Anchor C1 cannot replay reached constructor prefix",e);
 attempted_=true;fields_.type4=forward?2:0;fields_.actor8=id;
 if(!id)return fail("Original AnchorBase NULL GameObject assertion/dereference",e);
 if(!reset(e))return false; //BaseC1 zero/enable/source pointer stores then Base.Reset.
 forward_=forward;
 if(forward){
  fields_.max_distance1c=maximum;fields_.distance_per_sec20=speed;fields_.threshold24=threshold;
  const auto require=[&](bool valid,int line,const char* expression){
   return valid||(services_.assertion&&services_.assertion(line,expression,e));
  };
  if(!require(maximum>=0.f,48,"maxDistance >= 0")||!require(speed>=0.f,49,"distancePerSec >= 0")||
     !require(threshold>=0.f&&threshold<=1.f,50,"threshold >= 0 && threshold <= 1"))return fail("AnchorForward original parameter assertion",e);
  if(!services_.handle_character||!services_.handle_character(id,fields_.character28,e))return fail("Required actual GetHandle -> Character conversion",e);
  if(!reset(e))return false; //DerivedReset repeats Base.Reset, clears previous2c.
 }
 constructed_=true;e.clear();return true;
}
bool CameraAnchorOwnerV75::forward_update(std::string& e){
 auto& f=fields_;if(!f.enabled18)return true;
 CameraAnchorActorV75 a;if(!actor(a,e))return false;
 if(!a.heading_active1b5||!a.heading1b8)return fail("Required SAME GameObject heading1b5/1b8",e);
 auto moving=[&](bool& out){return services_.moving_false&&services_.moving_false(f.character28,out,e);};
 auto attacking=[&](bool& out){return services_.attacking&&services_.attacking(f.character28,out,e);};
 auto at=[&](float distance,const std::array<float,3>& direction){for(unsigned i=0;i<3;++i)f.position_c[i]=add(mul(distance,direction[i]),a.position160[i]);};
 if(!f.alternate38){
  f.state3c=5;bool engaged=false;
  if(static_cast<std::uint8_t>(*a.heading_active1b5)&&f.character28){
   if(!moving(engaged))return fail("Required source SM_IsMoving(false)",e);
   if(!engaged&&!attacking(engaged))return fail("Required source SM_IsAttacking",e);
  }
  if(engaged){
   std::array<float,3> direction{a.heading1b8[0],a.heading1b8[1],a.heading1b8[2]};
   const auto turn=angle(f.last_direction58,direction); //NaNs preserve original ordered comparisons.
   const std::array<float,3> previous=f.previous2c;std::array<float,3> now{a.position160[0],a.position160[1],a.position160[2]};
   bool moving_again{};if(!moving(moving_again))return fail("Required repeated source SM_IsMoving(false)",e);
   float distance=mul(f.max_distance1c,.5f);
   if(moving_again){std::array<float,3> delta{};for(unsigned i=0;i<3;++i)delta[i]=sub(previous[i],now[i]);if(square(delta)>.05f)distance=f.max_distance1c;}
   direction=normalized(direction);std::array<float,3> desired{};for(unsigned i=0;i<3;++i)desired[i]=add(mul(distance,direction[i]),a.position160[i]);
   if(turn>1.f){f.forward_distance54=sub(f.forward_distance54,mul(f.distance_per_sec20,.25f));if(!(f.forward_distance54>0.f))f.forward_distance54=0.f;}
   else{
    const auto half=mul(f.max_distance1c,.5f);
    //Source __aeabi_f2iz -> __aeabi_i2f; avoid undefined native conversion.
    const auto integral=std::isnan(half)?0:(half>=2147483648.f?INT32_MAX:(half<=-2147483648.f?INT32_MIN:static_cast<std::int32_t>(half)));
    const auto minimum=static_cast<float>(integral);
    if(f.forward_distance54<minimum)f.forward_distance54=minimum;
    else{auto next=add(f.distance_per_sec20,f.forward_distance54);if(distance<next)next=distance;f.forward_distance54=next;}
   }
   std::array<float,3> delta{};for(unsigned i=0;i<3;++i)delta[i]=sub(desired[i],f.position_c[i]);
   if(square(delta)>0.f&&sub(distance,f.forward_distance54)>.05f)at(f.forward_distance54,direction);
   else f.position_c=desired;
  }else{
   const auto decay=mul(f.max_distance1c,.4f);if(f.forward_distance54>decay)f.forward_distance54=decay;
   at(f.forward_distance54,f.last_direction58);
  }
  for(unsigned i=0;i<3;++i)f.previous2c[i]=a.position160[i];
  if(!services_.look_at||!services_.look_at(f.actor8,f.last_direction58,e))return fail("Required actual GetLookAtVec393ae4",e);
  return true;
 }
 bool moving_initial{};if(!moving(moving_initial))return fail("Required alternate anchor SM_IsMoving(false)",e);
 const auto distance=moving_initial?f.max_distance1c:mul(f.max_distance1c,.5f);
 const std::array<float,3> direction{a.heading1b8[0],a.heading1b8[1],a.heading1b8[2]};
 bool engaged{};if(static_cast<std::uint8_t>(*a.heading_active1b5)&&f.character28){
  if(!moving(engaged))return fail("Required repeated alternate SM_IsMoving(false)",e);
  if(!engaged&&!attacking(engaged))return fail("Required alternate SM_IsAttacking",e);
 }
 if(engaged){
  if(f.holding4c)f.state3c=2;
  else{
   std::array<float,3> delta{};for(unsigned i=0;i<3;++i)delta[i]=sub(a.position160[i],f.hold_position40[i]);
   if(!(square(delta)>=62500.f)){f.state3c=0;for(unsigned i=0;i<3;++i)f.position_c[i]=a.position160[i];return true;}
   f.state3c=3;
  }
  at(distance,direction);f.remaining50=350;f.holding4c=1;return true;
 }
 if(!f.holding4c)f.state3c=1;
 else{
  std::uint32_t dt{};if(!services_.application_dt||!services_.application_dt(dt,e))return fail("Required SAME Application.GetDt8c",e);
  std::uint32_t old{};std::memcpy(&old,&f.remaining50,4);f.remaining50=signed_bits(old-dt);
  if(f.remaining50<=0){f.holding4c=0;f.state3c=4;}
 }
 for(unsigned i=0;i<3;++i){f.hold_position40[i]=a.position160[i];f.position_c[i]=a.position160[i];}
 return true;
}
bool CameraAnchorOwnerV75::update(std::string& e){
 if(failed_){e=failure_;return false;}if(!constructed_)return fail("Required actual completed Anchor C1",e);
 if(busy_)return fail("Anchor.Update reentered",e);Busy busy(busy_);
 const bool result=forward_?forward_update(e):reset(e);
 if(!result||failed_)return fail("Source Anchor.Update provider failed",e);e.clear();return true;
}
}
