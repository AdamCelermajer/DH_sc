#include "../world.hpp"
#include "../navigation_path.hpp"
#include <fstream>
#include <iterator>
#include <cstdio>
#include <cstring>
#include <stdexcept>
int main(int argc,char** argv){try{
 if(argc!=2)return 2;auto read=[&](const char* name){std::ifstream f(std::string(argv[1])+"/"+name,std::ios::binary);return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(f),{});};
 auto bdae=read("crypt.bdae"),dwld=read("crypt01.dwld");dh2::resources::BresView image{};
 if(dh2_bres_open(&image,bdae.data(),bdae.size())!=dh2::resources::BresError::ok)return 3;
 dh2::world::Level level;std::string error;if(!dh2::world::load(image,dwld.data(),dwld.size(),level,error))throw std::runtime_error(error);
 auto& world=*level.native_floor;const auto required=world.graph.node_count+1;
 std::vector<dh2::navigation::PathSegment> segments(required);dh2::navigation::PathObject object{};object.segments=segments.data();object.capacity=required;object.route.radius=5;
 object.position[0]=-1025.8129f;object.position[1]=-696.6603f;object.position[2]=41.7078f;
 dh2::navigation::RouteResult result{};dh2::navigation::FindRequest request{&world.route_world,&world.collision_world,&object,&result,&world.route_workspace,{-1234.43005f,-369.638f,198.437f},30,0,0};
 unsigned policy_calls=0;dh2::navigation::FindSourceServicesV1 services{&policy_calls,[](void* p,std::uint32_t* out){++*static_cast<unsigned*>(p);*out=1;return 0;}};
 auto before=object;if(dh2_nav_find_path_source_v1(&request,&services)!=2||policy_calls||std::memcmp(&before,&object,sizeof(object)))throw std::runtime_error("Missing caller output did not fail atomically before policy");
 std::vector<std::uint32_t> retained(required);result.search.path=retained.data();result.search.path_capacity=unsigned(retained.size());
 const int status=dh2_nav_find_path_source_v1(&request,&services);if(status)throw std::runtime_error("Bound actual graph scratch rejected "+std::to_string(status));
 if(policy_calls!=1||std::memcmp(object.target,request.target,12))throw std::runtime_error("Actual FindPath policy/store prefix differs");
 // Same retained output survives a subsequent source path call and copying
 // into independent actor-owned PathSegments; no graph/result alias exists.
 result.search.path_count=0;if(dh2_nav_find_path_source_v1(&request,&services)||policy_calls!=2)throw std::runtime_error("Retained caller scratch repeat failed");
 std::printf("PASS 3 actual Crypt graph caller-output storage regressions: nodes %u, source route found %u, actor segments %u; explicit fixture PF capability/radius and search policy, not live movement acceptance\n",world.graph.node_count,result.found,object.count);return 0;
}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
