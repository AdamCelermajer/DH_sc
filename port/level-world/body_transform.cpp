#include "body_transform.hpp"
#include <cmath>
using namespace dh2::physical;
extern "C" int dh2_body_set_transform(TransformBody* body,const TransformRequest* request,TransformWorld* world,const TransformCallbacks* callbacks) {
 if(!body||!body->state||!request||!world||!callbacks)return -1;
 if(body->reserved||body->state->flags>0xffff)return -1;
 for(std::uint8_t value:world->reserved)if(value)return -1;
 if(world->locked)return 1;
 BodyState& state=*body->state;
 if(state.flags&2)return 0;
 if(!callbacks->commit||(world->first_shape&&(!callbacks->synchronize||!callbacks->destroy_proxy)))return -1;
 const float c=std::cos(request->angle),s=std::sin(request->angle);
 body->rotation[0]=c;body->rotation[1]=s;body->rotation[2]=-s;body->rotation[3]=c;
 state.position[0]=request->position[0];state.position[1]=request->position[1];
 const float xx=body->local_center[0]*c;
 const float xy=body->local_center[1]*(-s);
 const float rx=xx+xy;
 const float yx=body->local_center[0]*s;
 const float yy=body->local_center[1]*c;
 const float ry=yx+yy;
 body->center[0]=rx+state.position[0];body->center[1]=ry+state.position[1];
 body->previous_center[0]=body->center[0];body->previous_center[1]=body->center[1];
 body->previous_angle=request->angle;state.angle=request->angle;
 BodyTransform transform{{state.position[0],state.position[1]},{c,s,-s,c}};
 for(TransformShape* shape=world->first_shape;shape;shape=shape->next){
  if(callbacks->synchronize(world->context,shape->identity,world->broadphase,&transform,&transform))continue;
  state.flags|=2;state.angular_velocity=0.0f;state.linear_velocity[0]=0.0f;state.linear_velocity[1]=0.0f;
  for(TransformShape* proxy=world->first_shape;proxy;proxy=proxy->next)callbacks->destroy_proxy(world->context,proxy->identity,world->broadphase);
  return 0;
 }
 callbacks->commit(world->context,world->broadphase);return 1;
}
