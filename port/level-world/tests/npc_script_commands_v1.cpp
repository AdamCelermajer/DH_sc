#include "../npc_script_commands_v1.hpp"
#include "../world.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <cassert>
using namespace dh2;
static std::vector<std::uint8_t> read(const char* path){std::ifstream stream(path,std::ios::binary);return {std::istreambuf_iterator<char>(stream),{}};}
struct Fixture {
 std::array<float,3> target{},look{{0,-1,0}};unsigned calls{},events{};bool fail{},remote{};
 static int position(void* p,std::uintptr_t id,const float*& out){auto& t=*static_cast<Fixture*>(p);if(id!=2)return -1;out=t.target.data();return 0;}
 static int remotely(void* p,bool& out){out=static_cast<Fixture*>(p)->remote;return 0;}
 static int look_at(void* p,const float*& out){out=static_cast<Fixture*>(p)->look.data();return 0;}
 static int policy(void* p,std::uint32_t* out){auto& t=*static_cast<Fixture*>(p);++t.calls;if(t.fail)return -1;*out=1;return 0;}
 static bool heading_remote(void* p,bool& out,std::string&){return remotely(p,out)==0;}
 static bool physics(void*,bool& out,std::string&){out=false;return true;}
 static bool raise(void* p,std::uint32_t event,std::string&){assert(event==0x3f);++static_cast<Fixture*>(p)->events;return true;}
};
int main(int argc,char** argv){
 if(argc!=3)return 2;
 const auto bytes=read(argv[1]),descriptor=read(argv[2]);resources::BresView view{};std::string error;world::Level level;
 if(bytes.empty()||descriptor.empty()){std::cerr<<"Required nonempty actual BRES and DWLD fixtures: "<<argv[1]<<", "<<argv[2]<<"\n";return 3;}
 if(dh2_bres_open(&view,bytes.data(),bytes.size())!=resources::BresError::ok){std::cerr<<"Invalid actual BRES fixture "<<argv[1]<<"\n";return 4;}
 if(!world::load(view,descriptor.data(),descriptor.size(),level,error)){std::cerr<<"Actual DWLD world load failed: "<<error<<"\n";return 5;}auto& floors=*level.native_floor;
 assert(!floors.records.empty()&&!floors.records[0]->triangles.empty());const auto& triangle=floors.records[0]->triangles[0];
 actor::RuntimeState runtime{};Fixture fixture;
 for(unsigned i=0;i<3;++i){runtime.subobjects.position[i]=(triangle.points[0][i]+triangle.points[1][i]+triangle.points[2][i])/3.f;fixture.target[i]=(triangle.points[0][i]+triangle.points[1][i]*2+triangle.points[2][i])/4.f;}
 character::ControllerCommandState32 controller{3,1,0,0,0,0};character::State machine;
 character::TargetOwner16 target_owner{1,0,0,0};character::TargetState48 targets{};targets.owner=&target_owner;
 physical::NativeBody* body=nullptr;
 character::CharacterHeadingOwnerV1 heading(controller,machine,targets,{},runtime.controller,runtime.path,runtime.subobjects.position,runtime.subobjects.destination,body,{&fixture,nullptr,Fixture::heading_remote,Fixture::physics,Fixture::raise});
 std::uint8_t source_static=0;std::uint32_t limit=0;
 character::NpcScriptCommandsV1 commands({1,&controller,&runtime,&targets,runtime.subobjects.position,world::canonical_vec3_k_v1().data(),&source_static,&limit,&heading,&floors},{&fixture,Fixture::remotely,Fixture::position,Fixture::look_at,nullptr,nullptr,Fixture::policy});
 // Explicit bounded scene/physical-policy fixture; graph/triangles are actual
 // cache. The physical-null branch uses these labelled fixture bounds.
 navigation::ObstacleEntry entries[4]{};unsigned buckets[4]{};navigation::ObstacleRegistry registry{entries,0,4,buckets,0,4};
 const navigation::ProducerFields fields{navigation::ProducerClass::character,0,0,0,{-1,-1},{1,1}};
 assert(commands.initialize_pf(registry,fields));
 assert(runtime.object.motion.flags==2&&!(runtime.object.motion.object_flags&1));
 assert(runtime.object.radius==1); // UpdatePFObject source half extent.
 auto& binding=commands.bindings();dh2_script_value object{};object.type=DH2_SCRIPT_IDENTITY;object.identity=2;std::uint32_t returned{};
 auto command=[&](unsigned op){assert(commands.refresh());return dh2_character_script_command(binding.state,op,&object,1,&binding.services,nullptr,0,&returned);};
 assert(command(character::script_head_to)==1);assert(commands.last_route().found&&runtime.path.count&&fixture.calls==1);
 const unsigned count=runtime.path.count;controller.locked=1;fixture.target[0]+=500;
 assert(command(character::script_head_to)==1&&runtime.path.count==count&&fixture.calls==1);controller.locked=0;
 source_static=1;assert(command(character::script_move_to)==1&&fixture.calls==1);source_static=0;
 assert(command(character::script_stop)==1&&runtime.path.count==0&&fixture.events==1);
 fixture.fail=true;assert(command(character::script_head_to)==-2&&runtime.path.count==0&&fixture.calls==2);
 for(unsigned i=0;i<3;++i)assert(runtime.path.target[i]==fixture.target[i]);
 fixture.fail=false;runtime.object.user=0;assert(command(character::script_head_to)==-2);
 // Lifetime boundary: retire routes before destroying the floor, reset the
 // caller-owned registry, then adopt a freshly decoded actual-cache graph.
 assert(dh2_nav_object_set_flying(&runtime.object,1)==0);
 assert(commands.suspend_navigation());level.native_floor.reset();
 assert(command(character::script_head_to)==-2&&runtime.path.count==0);
 world::Level replacement;if(!world::load(view,descriptor.data(),descriptor.size(),replacement,error)){std::cerr<<"Actual DWLD replacement load failed: "<<error<<"\n";return 6;}
 registry.count=0;registry.floor_count=0;
 assert(commands.rebind_floor(replacement.native_floor.get()));
 assert(command(character::script_head_to)==-2); // Genuine PF init still required.
 assert(commands.initialize_pf(registry,fields));
 assert(dh2_nav_object_is_flying(&runtime.object)==1);
 const auto& next=replacement.native_floor->records[0]->triangles[0];
 for(unsigned i=0;i<3;++i)fixture.target[i]=(next.points[0][i]+next.points[1][i]*2+next.points[2][i])/4.f;
 assert(command(character::script_head_to)==1&&commands.last_route().found&&runtime.path.count);
 assert(!commands.rebind_floor(replacement.native_floor.get())); // No active-route replacement.
 std::cout<<"PASS actual-cache NPC HeadTo→controller→PFWorld route; blocked/static/Stop/failure-prefix/missing-PF checks\n";
}
