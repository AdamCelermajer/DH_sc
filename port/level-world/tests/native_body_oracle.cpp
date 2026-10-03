#include "native_body.hpp"
#include "Box2D.h"
namespace {
struct Holder {b2World* world;dh2::physical::NativeBody native;};
}
// Test-only bounded allocation and world ownership; production uses NativeWorld.
extern "C" dh2::physical::NativeBody* dh2_native_body_audit_create(const float* input){
 b2AABB bounds;bounds.lowerBound.Set(-100,-100);bounds.upperBound.Set(100,100);auto* h=new Holder{};h->world=new b2World(bounds,b2Vec2(0,0),true);
 const auto flags=static_cast<unsigned>(input[7]);b2BodyDef def;def.position.Set(input[4],input[5]);def.angle=input[6];def.fixedRotation=flags&0x40;def.isSleeping=flags&8;def.isBullet=flags&0x20;
 auto* body=h->world->CreateBody(&def);b2CircleDef shape;shape.radius=input[0];shape.localPosition.Set(input[1],input[2]);shape.density=input[3];body->CreateShape(&shape);body->SetMassFromShapes();h->native={body,input[0],0};if(flags&0x100)dh2_native_body_pin(&h->native);return &h->native;
}
extern "C" void dh2_native_body_audit_step(dh2::physical::NativeBody* n,const float* dt){n->body->GetWorld()->Step(*dt,10);}
extern "C" void dh2_native_body_audit_destroy(dh2::physical::NativeBody* n){auto* h=reinterpret_cast<Holder*>(reinterpret_cast<char*>(n)-offsetof(Holder,native));delete h->world;delete h;}
