#include "subobjects_update.hpp"
#include <cmath>
#include <cstdint>
namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return a&&b&&(x<=y?y-x<an:x-y<bn);}
float squared(const float* p){float a=p[0]*p[0],b=p[1]*p[1];return (a+b)+p[2]*p[2];}
}
extern "C" int dh2_subobjects_update_source_v69(dh2::subobjects::Result* out,const dh2::subobjects::Request* r,const dh2::subobjects::AuxiliaryBranchServicesV69* source){
 using namespace dh2::subobjects;
 if(!out||!r||!r->state||!r->policy||!r->services||!r->services->invoke||(r->body&&!r->transform)||r->policy->reserved||(r->body&&r->body->flags>65535))return 1;
 const void* ptr[]={out,r,r->state,r->body,r->transform,r->policy,r->services};const std::size_t len[]={8,40,128,48,16,52,16};
 for(unsigned i=0;i<7;++i)for(unsigned j=i+1;j<7;++j)if(overlap(ptr[i],len[i],ptr[j],len[j]))return 1;
 auto& s=*r->state;const auto& p=*r->policy;
 auto call=[&](Event e,float* v=nullptr){return r->services->invoke(r->services->context,e,v);};
 auto set_position=[&](){dh2_physical_request_position(r->transform,r->body,s.position);float values[]={r->transform->position[0],r->transform->position[1],r->transform->angle};call(apply_body_transform,values);};
 auto set_velocity=[&](float x,float y){float v[]={x,y};dh2_physical_set_linear(r->body,v);call(physical_set_velocity,v);};
 if(p.has_visual)call(visual_update);
 if(r->body)call(physical_update);
 const float* target=s.path_count?s.path_target:s.destination;float dx=target[0]-s.position[0],dy=target[1]-s.position[1];
 const bool arrived=dx*dx+dy*dy<6400.f;bool accepted=false;
 if(r->body){
  if(!(r->body->flags&8)){
   float query[4];dh2_physical_query(query,r->body);
   if(std::fabs(query[0]-s.position[0])>1.f||std::fabs(query[1]-s.position[1])>1.f){
    s.position[0]=query[0];s.position[1]=query[1];
    accepted=p.validating_floor==1?call(validate_position,s.position)!=0:true;
    if(!accepted)set_position();
   }
  }
  if(p.position_from_physics){
   float v[]={s.destination[0]-s.position[0],s.destination[1]-s.position[1],s.destination[2]-s.position[2]};float length2=squared(v);
   if(length2>0.f){
    const float speed=p.speed;float observed_speed=speed;call(get_speed,&observed_speed);
    if(length2>400.f){float length=std::sqrt(length2);v[0]=v[0]/length;v[1]=v[1]/length;v[2]=v[2]/length;set_velocity(speed*v[0],speed*v[1]);}
    else {set_velocity(0.f,0.f);set_position();}
   }
  }else set_velocity(0.f,0.f);
 }
 if(p.position_from_visual&&p.has_visual&&!accepted){call(visual_apply_position);if(r->body){dh2_physical_wake(r->body);call(physical_wake);}}
 if(p.has_visual)call(visual_sync_position);
 if(r->body)set_position();
 if(p.rotation_from_visual&&p.has_visual)call(visual_apply_rotation);
 else if(p.rotation_from_physics&&r->body&&!(r->body->flags&8))s.rotation=r->body->angle;
 if(p.has_visual){if(p.visual_with_rotation)call(visual_sync_rotation);call(visual_sync_scaling);}
 if(!p.validating_floor){
  float camera[3]={};bool camera_reject=false;
  if(p.validating_camera&&call(camera_get,camera))camera_reject=!call(camera_can_move,s.position);
  bool valid;
  if(camera_reject){for(int i=0;i<3;++i)s.position[i]=s.previous_position[i];valid=false;}
  else valid=call(validate_position,s.position)!=0;
  if(!valid&&r->body)set_position();
  if(p.has_visual)call(visual_sync_position);
 }
 if(arrived)for(int i=0;i<3;++i)s.destination[i]=s.position[i];
 for(int i=0;i<6;++i)s.absolute_bounds[i]=s.local_bounds[i]+s.position[i%3];
 if(p.has_auxiliary){
  const auto delivery=call(auxiliary_update);
  AuxiliaryBranchV69 branch{p.auxiliary_type,p.auxiliary_mode,s.auxiliary_position};
  if(source&&(delivery==UINT32_MAX||!source->borrow||source->borrow(source->context,&branch)||(branch.type==2&&!branch.position_c)))return 2;
  if(branch.type==2){
   float camera[3]={};
   if(branch.mode==3){call(camera_get,camera);float free=1.f;call(camera_set_free,&free);}
   else if(branch.mode==1||branch.mode==4){
    call(camera_get,camera);
    if(camera[0]!=0.f){call(camera_position,camera);float delta[]={branch.position_c[0]-camera[0],branch.position_c[1]-camera[1],branch.position_c[2]-camera[2]};if(squared(delta)<=3.f){float free=0.f;call(camera_set_free,&free);}}
   }
  }
 }
 *out={std::uint32_t(arrived),std::uint32_t(accepted)};return 0;
}
extern "C" int dh2_subobjects_update(dh2::subobjects::Result* out,const dh2::subobjects::Request* request){return dh2_subobjects_update_source_v69(out,request,nullptr);}
