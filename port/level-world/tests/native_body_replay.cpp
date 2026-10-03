// Host replay of the actual-original instruction corpus. Trig imports use the
// same double-evaluation/float-rounding service as the desktop CPU oracle;
// production Box2D and native_body remain unchanged.
#include "native_body.hpp"
#include "Box2D.h"
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <vector>
extern "C" float sinf(float x) noexcept {return static_cast<float>(std::sin(static_cast<double>(x)));}
extern "C" float cosf(float x) noexcept {return static_cast<float>(std::cos(static_cast<double>(x)));}
extern "C" void sincosf(float x,float* s,float* c) noexcept {*s=sinf(x);*c=cosf(x);}
using namespace dh2::physical;
namespace {
struct Record {float config[8];std::uint32_t operation;float args[4];NativeBodyObservation expected;float query[4];};
static_assert(sizeof(Record)==144);
int fail(std::size_t row,const char* what){std::fprintf(stderr,"Native body original replay mismatch row %zu %s\n",row,what);return 1;}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;std::ifstream stream(argv[1],std::ios::binary);std::uint32_t header[2];if(!stream.read(reinterpret_cast<char*>(header),8)||header[0]!=0x31424e44||header[1]>100000)return 2;
 std::vector<Record> records(header[1]);if(!stream.read(reinterpret_cast<char*>(records.data()),records.size()*sizeof(Record)))return 2;
 std::unique_ptr<b2World> world;NativeBody native{};std::array<float,8> previous{};std::size_t scenes=0,steps=0;
 for(std::size_t i=0;i<records.size();++i){const auto& r=records[i];
  if(!world||std::memcmp(r.config,previous.data(),32)){
   b2AABB bounds;bounds.lowerBound.Set(-100,-100);bounds.upperBound.Set(100,100);world.reset(new b2World(bounds,b2Vec2(0,0),true));b2BodyDef def;const auto flags=static_cast<unsigned>(r.config[7]);def.position.Set(r.config[4],r.config[5]);def.angle=r.config[6];def.fixedRotation=flags&0x40;def.isSleeping=flags&8;def.isBullet=flags&0x20;
   auto* body=world->CreateBody(&def);b2CircleDef shape;shape.radius=r.config[0];shape.localPosition.Set(r.config[1],r.config[2]);shape.density=r.config[3];body->CreateShape(&shape);body->SetMassFromShapes();native={body,r.config[0],0};if(flags&0x100)dh2_native_body_pin(&native);std::memcpy(previous.data(),r.config,32);++scenes;
  }
  int status=0;
  switch(r.operation){
   case 0:status=dh2_native_body_set_linear(&native,r.args);break;
   case 1:status=dh2_native_body_add_linear(&native,r.args);break;
   case 2:status=dh2_native_body_set_angular(&native,r.args);break;
   case 3:status=dh2_native_body_set_position(&native,r.args)<0;break;
   case 4:status=dh2_native_body_stop(&native,r.args);break;
   case 5:status=dh2_native_body_pin(&native);break;
   case 6:status=dh2_native_body_unpin(&native);break;
   case 7:world->Step(r.args[0],10);++steps;break;
   default:return 2;
  }
  if(status)return fail(i,"status");NativeBodyObservation observed{};float query[4];dh2_native_body_observe(&observed,&native);dh2_native_body_query(query,&native);
  if(std::memcmp(&observed,&r.expected,sizeof observed))return fail(i,"public body state");if(std::memcmp(query,r.query,sizeof query))return fail(i,"game query");
 }
 std::printf("{\"original_derived_replay\":%zu,\"scenes\":%zu,\"genuine_world_steps\":%zu,\"host_modeled_trig_imports\":true,\"mismatches\":0}\n",records.size(),scenes,steps);return 0;
}
