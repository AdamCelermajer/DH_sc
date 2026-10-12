#pragma once
#include "navigation_avoidance.hpp"
#include "character_body_config.hpp"
#include <Box2D.h>
#include <memory>
namespace dh2::physical {
using Filter=dh2::navigation::ContactFilter;
enum class ContactEvent : unsigned { add=0,persist=1,remove=2,result=3 };
// Native service view of PhysicalBaseObject's virtual interface. Position and
// mass are snapshots of its shape body; velocity is the original virtual query.
struct WorldObject {
 void* context;
 unsigned (*test)(void*,void*,const Filter*,const Filter*);
 void (*contact)(void*,ContactEvent,void*,const float*,unsigned);
 void (*velocity)(void*,float*);
 float position[2],mass;unsigned reserved;
};
struct WorldShape {WorldObject* owner;Filter filter;};
struct WorldContact {WorldShape shapes[2];float position[2];};
static_assert(sizeof(WorldObject)==48&&sizeof(WorldShape)==16&&sizeof(WorldContact)==40);
class NativeWorld final:public b2BoundaryListener,public b2ContactFilter,
 public b2ContactListener,public b2DestructionListener {
 std::unique_ptr<b2World> world_;
 void dispatch(ContactEvent,const b2ContactPoint*);
public:
 // PhysicalWorld::load passes these physics-unit bounds unchanged.
 void load(const float bounds[4]);
 void clear();
 void update(std::uint32_t milliseconds);
 b2World* backend() const {return world_.get();}
 b2Body* create(const b2BodyDef* definition);
 b2Body* create_character(const CharacterBodyConfig&,WorldObject*);
 void destroy(b2Body*&);
 bool ShouldCollide(b2Shape*,b2Shape*) override;
 void Add(const b2ContactPoint*) override;
 void Persist(const b2ContactPoint*) override;
 void Remove(const b2ContactPoint*) override;
 void Result(const b2ContactResult*) override;
 // Original Violation logs only; SayGoodbye overloads are empty.
 void Violation(b2Body*) override {}
 void SayGoodbye(b2Joint*) override {}
 void SayGoodbye(b2Shape*) override {}
};
}
extern "C" {
// Services must be provided when both owners exist; malformed views return -1.
int dh2_physical_world_should_collide(const dh2::physical::WorldShape*,const dh2::physical::WorldShape*);
int dh2_physical_world_contact(const dh2::physical::WorldContact*,unsigned event);
// Emits PhysicalWorld::update's Step arguments: milliseconds * float(0.001),10.
int dh2_physical_world_step_arguments(float*,unsigned*,std::uint32_t milliseconds);
}
