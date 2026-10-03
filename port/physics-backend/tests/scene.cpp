#include "scene.hpp"
#include <memory>
#include <cstring>
// Audit-only introspection. Production exposes the unaltered Box2D public API.
#define private public
#define protected public
#include <Box2D.h>
#undef protected
#undef private
using namespace dh2::backend_audit;
struct Listener: b2ContactListener {
 std::uint32_t added=0,persisted=0,removed=0,resolved=0;
 void Add(const b2ContactPoint*) override {++added;}
 void Persist(const b2ContactPoint*) override {++persisted;}
 void Remove(const b2ContactPoint*) override {++removed;}
 void Result(const b2ContactResult*) override {++resolved;}
};
extern "C" int dh2_backend_audit_scene(Snapshot* out,const Scene* input){
 if(!out||!input||input->count>4)return -1;
 Snapshot result{};Listener listener;b2ContactFilter filter;
 b2AABB bounds;bounds.lowerBound.Set(input->bounds[0],input->bounds[1]);bounds.upperBound.Set(input->bounds[2],input->bounds[3]);
 b2World* world=new b2World(bounds,b2Vec2(input->gravity[0],input->gravity[1]),true);
 world->SetContactListener(&listener);world->SetContactFilter(&filter);b2Body* bodies[4]{};
 for(std::uint32_t i=0;i<input->count;++i){const auto& in=input->bodies[i];b2BodyDef def;def.position.Set(in.position[0],in.position[1]);def.angle=in.angle;def.fixedRotation=in.fixed_rotation;def.isSleeping=in.sleeping;def.isBullet=in.bullet;def.userData=reinterpret_cast<void*>(static_cast<std::uintptr_t>(i+1));
  b2Body* body=world->CreateBody(&def);bodies[i]=body;b2CircleDef circle;b2PolygonDef polygon;b2ShapeDef* shape;
  if(in.shape==0){circle.localPosition.Set(in.local_position[0],in.local_position[1]);circle.radius=in.radius;shape=&circle;}
  else {polygon.SetAsBox(in.half_extents[0],in.half_extents[1]);shape=&polygon;}
  shape->isSensor=in.sensor;shape->density=in.density;shape->friction=in.friction;shape->restitution=in.restitution;shape->filter.groupIndex=static_cast<int16>(in.group);shape->filter.categoryBits=static_cast<uint16>(in.category);shape->filter.maskBits=static_cast<uint16>(in.mask);
  body->CreateShape(shape);body->SetMassFromShapes();if(in.pin){b2MassData mass;mass.mass=0;mass.center=body->GetLocalCenter();mass.I=0;body->SetMass(&mass);}
  body->SetLinearVelocity(b2Vec2(in.velocity[0],in.velocity[1]));body->SetAngularVelocity(in.angular_velocity);
  if(input->actions&1){const b2Vec2 position((input->actions&2)?200.0f:in.position[0]+0.25f,in.position[1]+0.125f);body->SetXForm(position,in.angle);}
 }
 for(std::uint32_t i=0;i<input->steps;++i)world->Step(input->dt,input->iterations);
 result.body_count=world->GetBodyCount();result.contact_count=world->GetContactCount();result.pair_count=world->GetPairCount();result.added=listener.added;result.persisted=listener.persisted;result.removed=listener.removed;result.resolved=listener.resolved;result.inverse_dt=world->m_inv_dt0;
 for(std::uint32_t i=0;i<input->count;++i){const auto* b=bodies[i];auto& row=result.bodies[i];row.flags=b->m_flags;row.type=b->m_type;row.shape_count=b->m_shapeCount;row.shape_type=b->m_shapeList->GetType();float* v=row.values;
  const float values[]={b->m_xf.position.x,b->m_xf.position.y,b->m_xf.R.col1.x,b->m_xf.R.col1.y,b->m_xf.R.col2.x,b->m_xf.R.col2.y,b->m_sweep.localCenter.x,b->m_sweep.localCenter.y,b->m_sweep.c0.x,b->m_sweep.c0.y,b->m_sweep.c.x,b->m_sweep.c.y,b->m_sweep.a0,b->m_sweep.a,b->m_sweep.t0,b->m_linearVelocity.x,b->m_linearVelocity.y,b->m_angularVelocity,b->m_force.x,b->m_force.y,b->m_torque,b->m_mass,b->m_invMass,b->m_I,b->m_invI,b->m_linearDamping,b->m_angularDamping,b->m_sleepTime};std::memcpy(v,values,sizeof values);
  b2MassData mass;b->m_shapeList->ComputeMass(&mass);v[28]=mass.mass;v[29]=mass.center.x;v[30]=mass.center.y;v[31]=mass.I;v[32]=b->m_shapeList->GetSweepRadius();
 }
 delete world;*out=result;return 0;
}
