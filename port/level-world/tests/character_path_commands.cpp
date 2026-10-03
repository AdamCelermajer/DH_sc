#include "character_path_commands.hpp"
#include "world.hpp"
#include "navigation_path.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::character;
namespace {
constexpr std::uintptr_t owner=0x100000001ull;
void require(bool v){if(!v)throw std::runtime_error("path/point command audit mismatch");}
template<class T>T read(std::ifstream& f){T v{};require(bool(f.read(reinterpret_cast<char*>(&v),sizeof(v))));return v;}
struct PathCase40 {std::uint32_t disabled,length,limit,answer;float cached[3],target[3];};
struct LookCase32 {LookAtState16 state;float target[3];std::uint32_t alias;};
static_assert(sizeof(PathCase40)==40&&sizeof(LookCase32)==32);
struct Context {
 std::uint32_t answer=0;int failure=0;std::vector<PathToRequest32> calls;
 static int find(void* p,const PathToRequest32* request,std::uint32_t* result){auto& c=*static_cast<Context*>(p);require(request&&result&&request->owner==owner);auto normalized=*request;normalized.owner=1;c.calls.push_back(normalized);*result=c.answer;return c.failure;}
};
bool close(float a,float b){
 if(std::isnan(a)&&std::isnan(b))return true;
 std::uint32_t x,y;std::memcpy(&x,&a,4);std::memcpy(&y,&b,4);if(x==y)return true;
 if(!std::isfinite(a)||!std::isfinite(b)||(x>>31)!=(y>>31))return false;
 return (x>y?x-y:y-x)<=2;
}
std::vector<unsigned char> file_bytes(const char* p){std::ifstream f(p,std::ios::binary);require(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
struct NativeRoute {
 dh2::floors::World* world=nullptr;dh2::navigation::PathObject object{};
 std::vector<dh2::navigation::PathSegment> segments;std::vector<std::uint32_t> edges;
 std::uint32_t calls=0;
 static int find(void* p,const PathToRequest32* request,std::uint32_t* result){
  auto& c=*static_cast<NativeRoute*>(p);using namespace dh2::navigation;++c.calls;
  RouteResult route{0,0,0,0,{0,0,0,0,0,0,c.edges.data(),std::uint32_t(c.edges.size()),0}};
  FindRequest r{&c.world->route_world,&c.world->collision_world,&c.object,&route,&c.world->route_workspace,{},request->limit,1,0};std::copy(request->target,request->target+3,r.target);
  const int status=dh2_nav_find_path(&r);*result=route.found;return status;
 }
};
}
int main(int argc,char** argv){try{
 require(argc==2||argc==4);std::ifstream gold(argv[1],std::ios::binary);require(bool(gold));require(read<std::array<char,4>>(gold)==std::array<char,4>{'P','C','D','1'});
 const auto path_cases=read<std::uint32_t>(gold);std::uint64_t requests=0;std::uint32_t angle_variants=0;
 for(std::uint32_t i=0;i<path_cases;++i){const auto in=read<PathCase40>(gold);const auto expected=read<PathToResult16>(gold);const auto count=read<std::uint32_t>(gold);require(count<=1);Context context;context.answer=in.answer;std::vector<PathToRequest32> calls(count);for(auto& r:calls)r=read<PathToRequest32>(gold);
  const PathToState40 state{owner,in.disabled,std::uint32_t(in.length!=0),in.limit,0,{in.cached[0],in.cached[1],in.cached[2]},0};const PathToServices16 services{&context,Context::find};PathToResult16 actual{};
  require(dh2_character_path_to(&actual,&state,in.target,&services)==0&&std::memcmp(&actual,&expected,16)==0);require(context.calls.size()==count);if(count)require(std::memcmp(context.calls.data(),calls.data(),32)==0);requests+=count;
 }
 const auto look_cases=read<std::uint32_t>(gold);
 for(std::uint32_t i=0;i<look_cases;++i){const auto in=read<LookCase32>(gold);const auto expected=read<LookAtState16>(gold);auto state=in.state;require(dh2_character_look_at_point(&state,in.alias?state.position:in.target)==0);require(std::memcmp(state.position,expected.position,12)==0&&close(state.heading_angle,expected.heading_angle));angle_variants+=std::memcmp(&state.heading_angle,&expected.heading_angle,4)!=0;}
 require(gold.peek()==std::char_traits<char>::eof());
 Context context;const PathToServices16 services{&context,Context::find};PathToState40 state{owner,0,0,0,0,{0,0,0},0};PathToResult16 result{17,19,23,29};const auto saved=result;float target[3]{1,2,3};unsigned guards=0;
 auto reject=[&](PathToResult16* out,const PathToState40* s,const float* t,const PathToServices16* svc){context.calls.clear();require(dh2_character_path_to(out,s,t,svc)==1&&context.calls.empty());require(std::memcmp(&result,&saved,16)==0);++guards;};
 reject(nullptr,&state,target,&services);reject(&result,nullptr,target,&services);reject(&result,&state,nullptr,&services);reject(&result,&state,target,nullptr);
 for(unsigned i=0;i<5;++i){auto malformed=state;if(i==0)malformed.owner=0;else if(i==1)malformed.disabled=256;else if(i==2)malformed.path_nonempty=2;else if(i==3)malformed.reserved=1;else malformed.reserved1=1;reject(&result,&malformed,target,&services);}
 PathToServices16 absent{nullptr,nullptr};require(dh2_character_path_to(&result,&state,target,&absent)==2&&std::memcmp(&result,&saved,16)==0);state.disabled=1;require(dh2_character_path_to(&result,&state,target,&absent)==0&&!result.requested);state.disabled=0;
 for(int failure:{1,-1}){context.failure=failure;result=saved;require(dh2_character_path_to(&result,&state,target,&services)==2&&std::memcmp(&result,&saved,16)==0);}context.failure=0;
 LookAtState16 look{};require(dh2_character_look_at_point(nullptr,target)==1&&dh2_character_look_at_point(&look,nullptr)==1);guards+=2;
 unsigned routes=0,found_routes=0,failed_routes=0;
 if(argc==4){
  auto bres=file_bytes(argv[2]),metadata=file_bytes(argv[3]);dh2::resources::BresView view{};require(dh2_bres_open(&view,bres.data(),bres.size())==dh2::resources::BresError::ok);dh2::world::Level level;std::string error;require(dh2::world::load(view,metadata.data(),metadata.size(),level,error));require(bool(level.native_floor));
  auto& world=*level.native_floor;NativeRoute backend;backend.world=&world;backend.segments.resize(world.graph.node_count+1);backend.edges.resize(world.graph.node_count+1);backend.object.segments=backend.segments.data();backend.object.capacity=backend.segments.size();backend.object.route.flags=2;backend.object.route.radius=36;
  const PathToServices16 native_services{&backend,NativeRoute::find};
  for(unsigned i=0;i<world.collision_world.floor_count;++i){const auto& triangle=world.records[i]->triangles.front();float p[3];for(unsigned k=0;k<3;++k)p[k]=(triangle.points[0][k]+triangle.points[1][k]+triangle.points[2][k])/3.f;
   for(unsigned j=0;j<world.collision_world.floor_count;++j){const auto& target_triangle=world.records[j]->triangles.front();float target_point[3];for(unsigned k=0;k<3;++k)target_point[k]=(target_triangle.points[0][k]+target_triangle.points[1][k]+target_triangle.points[2][k])/3.f;
    std::copy(p,p+3,backend.object.position);dh2_nav_drop_path(&backend.object);PathToState40 request_state{owner,0,0,0,0,{0,0,0},0};PathToResult16 output{};
    require(dh2_character_path_to(&output,&request_state,target_point,&native_services)==0&&output.requested&&output.limit==30);
    if(output.find_result){++found_routes;require(backend.object.count);request_state.path_nonempty=1;std::copy(backend.object.target,backend.object.target+3,request_state.path_target);const auto before=backend.calls;require(dh2_character_path_to(&output,&request_state,target_point,&native_services)==0&&!output.requested&&backend.calls==before);}else ++failed_routes;
    ++routes;
   }
  }
 }
 std::cout<<"{\"validation\":\"PASS\",\"path_comparisons\":"<<path_cases<<",\"look_comparisons\":"<<look_cases<<",\"find_path_requests\":"<<requests<<",\"host_angle_variants_within_two_ulp\":"<<angle_variants<<",\"native_guards\":"<<guards<<",\"genuine_authored_floor_route_sessions\":"<<routes<<",\"genuine_routes_found\":"<<found_routes<<",\"genuine_routes_failed\":"<<failed_routes<<",\"mismatches\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
