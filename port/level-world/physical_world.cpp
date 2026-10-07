#include "physical_world.hpp"
#include <cmath>
#include <stdexcept>
namespace {
using namespace dh2::physical;
bool valid(const WorldObject* o){return !o||!o->reserved;}
bool valid(const Filter& f){return f.present<=1;}
bool default_filter(const Filter& a,const Filter& b){
 if(a.group==b.group&&a.group!=0)return a.group>0;
 return (a.mask&b.category)!=0&&(a.category&b.mask)!=0;
}
WorldShape shape(b2Shape* s,WorldObject* snapshot=nullptr){
 const auto f=s->GetFilterData();
 auto* o=static_cast<WorldObject*>(s->GetUserData());
 if(o&&snapshot){*snapshot=*o;const auto* b=s->GetBody();const auto p=b->GetPosition();snapshot->position[0]=p.x;snapshot->position[1]=p.y;snapshot->mass=b->GetMass();o=snapshot;}
 return {o,{f.groupIndex,f.categoryBits,f.maskBits,1}};
}
}
extern "C" int dh2_physical_world_should_collide(const dh2::physical::WorldShape* a,const dh2::physical::WorldShape* b){
 if(!a||!b||!valid(a->owner)||!valid(b->owner)||!valid(a->filter)||!valid(b->filter))return -1;
 if(!a->owner||!b->owner)return default_filter(a->filter,b->filter);
 if(!a->owner->test||!b->owner->test)return -1;
 // Both calls execute even when the first object rejects the collision.
 const auto first=a->owner->test(a->owner->context,b->owner->context,&a->filter,&b->filter);
 const auto second=b->owner->test(b->owner->context,a->owner->context,&b->filter,&a->filter);
 return first!=0&&second!=0;
}
extern "C" int dh2_physical_world_contact(const dh2::physical::WorldContact* p,unsigned event){
 using namespace dh2::physical;
 if(!p||event>3||!valid(p->shapes[0].owner)||!valid(p->shapes[1].owner))return -1;
 auto* a=p->shapes[0].owner;auto* b=p->shapes[1].owner;
 if(!a||!b)return 0;
 if(!a->contact||!b->contact||(event!=3&&a->mass>0&&!a->velocity))return -1;
 unsigned first=1;
 if(event!=3){
  if(a->mass>0){
   const float x=p->position[0]-a->position[0],y=p->position[1]-a->position[1];
   float velocity[2]{0,0};a->velocity(a->context,velocity);
   first=(x*velocity[0]+y*velocity[1])>0;
  }else first=!(b->mass>0);
 }
 // Fresh local copies match the two original Vec2f stack arguments.
 float point1[2]{p->position[0],p->position[1]},point2[2]{p->position[0],p->position[1]};
 const auto kind=static_cast<ContactEvent>(event);
 a->contact(a->context,kind,b->context,event==3?nullptr:point1,first);
 b->contact(b->context,kind,a->context,event==3?nullptr:point2,first^1);
 return 0;
}
extern "C" int dh2_physical_world_step_arguments(float* dt,unsigned* iterations,std::uint32_t milliseconds){
 if(!dt||!iterations)return -1;
 *dt=static_cast<float>(milliseconds)*0.001f;*iterations=10;return 0;
}
namespace dh2::physical {
void NativeWorld::load(const float bounds[4]){
 if(!bounds||!std::isfinite(bounds[0])||!std::isfinite(bounds[1])||!std::isfinite(bounds[2])||!std::isfinite(bounds[3])||bounds[0]>=bounds[2]||bounds[1]>=bounds[3])throw std::invalid_argument("Invalid physical world bounds");
 clear();b2AABB aabb;aabb.lowerBound.Set(bounds[0],bounds[1]);aabb.upperBound.Set(bounds[2],bounds[3]);
 world_=std::make_unique<b2World>(aabb,b2Vec2(0,0),true);
 world_->SetBoundaryListener(this);world_->SetContactFilter(this);world_->SetContactListener(this);world_->SetDestructionListener(this);
}
void NativeWorld::clear(){DeliveryV106 delivery(mutation_depth_v106_);world_.reset();}
void NativeWorld::update(std::uint32_t milliseconds){
 if(!world_)throw std::logic_error("Physical world not loaded");
 DeliveryV106 delivery(step_depth_v106_);float dt;unsigned iterations;dh2_physical_world_step_arguments(&dt,&iterations,milliseconds);world_->Step(dt,iterations);
}
b2Body* NativeWorld::create(const b2BodyDef* definition){DeliveryV106 delivery(mutation_depth_v106_);return definition&&world_?world_->CreateBody(definition):nullptr;}
void NativeWorld::destroy(b2Body*& body){DeliveryV106 delivery(mutation_depth_v106_);if(body){if(!world_)throw std::logic_error("Physical world not loaded");world_->DestroyBody(body);}body=nullptr;}
b2Body* NativeWorld::create_character(const CharacterBodyConfig& c,WorldObject* owner){
 DeliveryV106 delivery(mutation_depth_v106_);if(!world_||!c.enabled)return nullptr;
 if(c.shape.kind>1||c.shape.vertex_count>4||!owner||owner->reserved)throw std::invalid_argument("Invalid character body definition");
 b2BodyDef definition;definition.userData=owner;definition.massData.mass=c.body.mass;
 definition.massData.center.Set(c.body.local_center[0],c.body.local_center[1]);definition.massData.I=c.body.inertia;
 definition.position.Set(c.body.position[0],c.body.position[1]);definition.angle=c.body.angle;
 definition.linearDamping=c.body.linear_damping;definition.angularDamping=c.body.angular_damping;
 definition.allowSleep=c.body.allow_sleep;definition.isSleeping=c.body.is_sleeping;
 definition.fixedRotation=c.body.fixed_rotation;definition.isBullet=c.body.bullet;
 auto* body=world_->CreateBody(&definition);if(!body)return nullptr;
 b2CircleDef circle;b2PolygonDef polygon;b2ShapeDef* s=c.shape.kind?static_cast<b2ShapeDef*>(&polygon):static_cast<b2ShapeDef*>(&circle);
 s->userData=owner;s->isSensor=c.shape.sensor;s->friction=c.shape.friction;s->restitution=c.shape.restitution;s->density=c.shape.density;
 s->filter.groupIndex=c.shape.group_index;s->filter.categoryBits=c.shape.category_bits;s->filter.maskBits=c.shape.mask_bits;
 if(c.shape.kind){polygon.vertexCount=c.shape.vertex_count;for(unsigned i=0;i<c.shape.vertex_count;++i)polygon.vertices[i].Set(c.shape.vertices[2*i],c.shape.vertices[2*i+1]);}
 else {circle.localPosition.Set(c.shape.local_position[0],c.shape.local_position[1]);circle.radius=c.shape.radius;}
 body->CreateShape(s);body->SetMassFromShapes();
 if(c.pinned){b2MassData mass;mass.mass=0;mass.I=0;mass.center=body->GetLocalCenter();body->SetMass(&mass);}
 return body;
}
bool NativeWorld::ShouldCollide(b2Shape* a,b2Shape* b){DeliveryV106 delivery(filter_depth_v106_);const auto x=shape(a),y=shape(b);return dh2_physical_world_should_collide(&x,&y)==1;}
void NativeWorld::dispatch(ContactEvent event,const b2ContactPoint* p){
 DeliveryV106 delivery(contact_depth_v106_);
 WorldObject owners[2];WorldContact c{{shape(p->shape1,&owners[0]),shape(p->shape2,&owners[1])},{p->position.x,p->position.y}};
 if(dh2_physical_world_contact(&c,static_cast<unsigned>(event)))throw std::logic_error("Incomplete physical contact services");
}
void NativeWorld::Add(const b2ContactPoint* p){dispatch(ContactEvent::add,p);}
void NativeWorld::Persist(const b2ContactPoint* p){dispatch(ContactEvent::persist,p);}
void NativeWorld::Remove(const b2ContactPoint* p){dispatch(ContactEvent::remove,p);}
void NativeWorld::Result(const b2ContactResult* p){
 DeliveryV106 delivery(contact_depth_v106_);
 WorldObject owners[2];WorldContact c{{shape(p->shape1,&owners[0]),shape(p->shape2,&owners[1])},{0,0}};
 if(dh2_physical_world_contact(&c,3))throw std::logic_error("Incomplete physical result services");
}
}
