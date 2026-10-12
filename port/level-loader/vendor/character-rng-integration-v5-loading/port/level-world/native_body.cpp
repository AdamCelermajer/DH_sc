#include "native_body.hpp"
#include "Box2D.h"
#include <cmath>
namespace {
using namespace dh2::physical;
bool valid(const NativeBody* n){return n&&n->body&&n->pinned<=1;}
bool finite2(const float* p){return p&&std::isfinite(p[0])&&std::isfinite(p[1]);}
void linear(NativeBody& n,float x,float y){if(x!=0.f||y!=0.f)n.body->WakeUp();n.body->SetLinearVelocity(b2Vec2(x,y));}
void angular(NativeBody& n,float value){n.body->WakeUp();n.body->SetAngularVelocity(value);}
}
extern "C" int dh2_native_body_set_linear(NativeBody* n,const float* xy){if(!valid(n)||!xy)return 1;const float x=xy[0],y=xy[1];linear(*n,x,y);return 0;}
extern "C" int dh2_native_body_add_linear(NativeBody* n,const float* p){if(!valid(n)||!p)return 1;const float dx=p[0],dy=p[1],cx=p[2],cy=p[3];if(dx!=0.f||dy!=0.f)n->body->WakeUp();const auto old=n->body->GetLinearVelocity();float x=dx+old.x,y=dy+old.y;if(x>cx)x=cx;if(y>cy)y=cy;n->body->SetLinearVelocity(b2Vec2(x,y));return 0;}
extern "C" int dh2_native_body_set_angular(NativeBody* n,const float* value){if(!valid(n)||!value)return 1;angular(*n,*value);return 0;}
extern "C" int dh2_native_body_wake(NativeBody* n){if(!valid(n))return 1;n->body->WakeUp();return 0;}
extern "C" int dh2_native_body_query(float* out,const NativeBody* n){if(!valid(n)||!out)return 1;const auto p=n->body->GetPosition();out[0]=p.x*100.f;out[1]=p.y*100.f;out[2]=n->body->GetAngle();out[3]=n->radius*100.f;return 0;}
extern "C" int dh2_native_body_observe(NativeBodyObservation* out,const NativeBody* n){
 if(!valid(n)||!out)return 1;const auto p=n->body->GetPosition(),v=n->body->GetLinearVelocity(),local=n->body->GetLocalCenter(),world=n->body->GetWorldCenter();
 *out={{p.x,p.y},n->body->GetAngle(),n->radius,{v.x,v.y},n->body->GetAngularVelocity(),n->body->GetMass(),n->body->GetInertia(),{local.x,local.y},{world.x,world.y},n->body->IsSleeping(),n->body->IsFrozen(),n->body->IsDynamic(),n->body->IsBullet(),n->pinned,0};return 0;
}
extern "C" int dh2_native_body_refresh_view(BodyState* out,const NativeBody* n){
 if(!valid(n)||!out||out->flags>0xffffu)return 1;const auto p=n->body->GetPosition(),v=n->body->GetLinearVelocity();
 out->position[0]=p.x;out->position[1]=p.y;out->angle=n->body->GetAngle();out->radius=n->radius;out->linear_velocity[0]=v.x;out->linear_velocity[1]=v.y;out->angular_velocity=n->body->GetAngularVelocity();
 out->flags=(out->flags&~0x2au)|(n->body->IsFrozen()?2u:0u)|(n->body->IsSleeping()?8u:0u)|(n->body->IsBullet()?0x20u:0u);return 0;
}
extern "C" int dh2_native_body_apply_transform(NativeBody* n,const TransformRequest* r){if(!valid(n)||!r||!finite2(r->position)||!std::isfinite(r->angle))return -1;return n->body->SetXForm(b2Vec2(r->position[0],r->position[1]),r->angle);}
extern "C" int dh2_native_body_set_position(NativeBody* n,const float* xy){if(!valid(n)||!finite2(xy))return -1;return n->body->SetXForm(b2Vec2(xy[0]*.01f,xy[1]*.01f),n->body->GetAngle());}
extern "C" int dh2_native_body_stop(NativeBody* n,const float* xy){if(!valid(n)||!finite2(xy))return 1;const float x=xy[0],y=xy[1];linear(*n,0.f,0.f);angular(*n,0.f);n->body->SetXForm(b2Vec2(x*.01f,y*.01f),n->body->GetAngle());n->body->PutToSleep();return 0;}
extern "C" int dh2_native_body_pin(NativeBody* n){if(!valid(n))return 1;if(!n->pinned){n->pinned=1;b2MassData m;m.mass=0.f;m.center=n->body->GetLocalCenter();m.I=0.f;n->body->SetMass(&m);}return 0;}
extern "C" int dh2_native_body_unpin(NativeBody* n){if(!valid(n))return 1;if(n->pinned){n->pinned=0;n->body->SetMassFromShapes();n->body->WakeUp();}return 0;}
extern "C" std::uint32_t dh2_native_body_subobject_service(void* opaque,std::uint32_t event,float* payload){
 auto* bridge=static_cast<NativeSubobjectsBridge*>(opaque);if(!bridge||!valid(bridge->native)||!bridge->view||bridge->view->flags>0xffffu)return 0;
 using namespace dh2::subobjects;
 switch(event){
  case physical_update:dh2_native_body_refresh_view(bridge->view,bridge->native);return 1;
  case apply_body_transform:{if(!payload)return 0;const TransformRequest request{{payload[0],payload[1]},payload[2],1};const int result=dh2_native_body_apply_transform(bridge->native,&request);dh2_native_body_refresh_view(bridge->view,bridge->native);return result==1;}
  case physical_set_velocity:if(dh2_native_body_set_linear(bridge->native,payload))return 0;dh2_native_body_refresh_view(bridge->view,bridge->native);return 1;
  case physical_wake:if(dh2_native_body_wake(bridge->native))return 0;dh2_native_body_refresh_view(bridge->view,bridge->native);return 1;
  default:return bridge->fallback?bridge->fallback(bridge->context,event,payload):0;
 }
}
