#include "world.hpp"
#include "navigation_path.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
using namespace dh2::navigation;
namespace {
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated world-route reference");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
 std::vector<unsigned char> block(unsigned count){if(count>10000000)throw std::runtime_error("World-route reference budget exceeded");std::vector<unsigned char> result(count);read(result.data(),count);return result;}
};
void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
}
int main(int argc,char** argv){
 if(argc!=4)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31564e46,"Invalid world-route reference");const unsigned cases=r.word(),n=r.word(),e=r.word(),count=r.word();require(cases&&cases<=4096&&n==335&&e==838&&count==8,"Invalid authored route budgets");
  const auto nodes=r.block(n*56),edges=r.block(e*20);std::vector<FloorGraph> floors(count);r.read(floors.data(),count*48);require(r.word()==count,"Missing world geometry");
  Reader a(argv[2]),b(argv[3]);dh2::resources::BresView view{};require(dh2_bres_open(&view,a.bytes.data(),a.bytes.size())==dh2::resources::BresError::ok,"Invalid authored BRES");dh2::world::Level level;std::string error;require(dh2::world::load(view,b.bytes.data(),b.bytes.size(),level,error),error.c_str());auto& world=*level.native_floor;
  require(world.graph.node_count==n&&world.graph.edge_count==e&&!std::memcmp(world.graph.nodes,nodes.data(),nodes.size())&&!std::memcmp(world.graph.edges,edges.data(),edges.size())&&!std::memcmp(world.floor_graphs.data(),floors.data(),count*48),"Native asset-loader graph differs from original gold");
  for(unsigned i=0;i<count;++i){const unsigned triangles=r.word();const auto expected=r.block(triangles*36);dh2::octree::Box box;r.read(&box,24);require(world.records[i]->triangles.size()==triangles&&!std::memcmp(world.records[i]->triangles.data(),expected.data(),expected.size())&&!std::memcmp(&world.records[i]->bounds,&box,24),"Native authored geometry differs from route gold");require(world.collision_rooms[i].count==1&&world.collision_rooms[i].floors[0]==i&&!std::memcmp(&world.collision_rooms[i].bounds,&box,24),"Native room geometry differs from caller gold");}

  std::vector<PathSegment> segments(n+1);std::vector<unsigned> path(n+1);PathObject object{};object.segments=segments.data();object.capacity=segments.size();unsigned direct=0,graph=0,failed=0,sessions=0,query_records=0,rejections=0;
  const auto snapshot=[&](){std::vector<unsigned char> raw(76+object.count*48);std::memcpy(raw.data(),&object,68);std::memcpy(raw.data()+68,&object.count,4);std::memcpy(raw.data()+72,&object.owned,4);if(object.count)std::memcpy(raw.data()+76,segments.data(),object.count*48);return raw;};
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned reset=r.word(),disabled=r.word();require(reset<=1&&disabled<256,"Invalid FindPath session");std::vector<FloorTraits> traits(count);r.read(traits.data(),count*8);float source[3],target[3];r.read(source,12);r.read(target,12);const unsigned limit=r.word(),flags=r.word();float radius;r.read(&radius,4);const unsigned enabled=r.word();require(enabled<=1,"Invalid FindPath gate");const auto expected=r.block(r.word()),expected_object=r.block(r.word()),cache=r.block(r.word());const unsigned queries=r.word();require(queries<=16,"Invalid query count");r.block(queries*16);query_records+=queries;
   if(reset){++sessions;object={};object.segments=segments.data();object.capacity=segments.size();world.floor_graphs=floors;dh2::floors::clear_route_cache(world);for(unsigned i=0;i<count;++i){if(disabled&(1u<<i))world.floor_graphs[i].root=0;world.route_traits[i]=traits[i];world.collision_floors[i].traits=traits[i];}world.route_world.floors=world.floor_graphs.data();}
   object.route.flags=flags;object.route.radius=radius;std::copy(source,source+3,object.position);RouteResult result{0,0,0,0,{0,0,0,0,0,0,path.data(),unsigned(path.size()),0}};FindRequest request{&world.route_world,&world.collision_world,&object,&result,&world.route_workspace,{},limit,enabled,0};std::copy(target,target+3,request.target);
   require(dh2_nav_find_path(&request)==0,"Native FindPath rejected gold");unsigned expected_count;require(expected.size()>=40,"Missing FindPath result");std::memcpy(&expected_count,expected.data()+36,4);require(expected.size()==40+expected_count*4,"Invalid FindPath output length");const unsigned cache_count=world.route_world.failed_count;require(cache.size()==4+cache_count*12,"FindPath cache length mismatch");unsigned stored_count;std::memcpy(&stored_count,cache.data(),4);
   if(std::memcmp(&result,expected.data(),40)||std::memcmp(path.data(),expected.data()+40,expected_count*4)||snapshot()!=expected_object||stored_count!=cache_count||std::memcmp(world.failed_routes.data(),cache.data()+4,cache_count*12)){std::cerr<<"FindPath mismatch case "<<ci<<'\n';return 4;}
   direct+=result.kind==1;graph+=result.kind==2;failed+=!result.found;
   if(!rejections){const auto saved=snapshot();const auto saved_segments=segments;const auto saved_path=path;const auto saved_result=result;const auto saved_object=object;const auto saved_cache=world.failed_routes;const auto unchanged=[&](){return !std::memcmp(&object,&saved_object,sizeof(object))&&snapshot()==saved&&!std::memcmp(segments.data(),saved_segments.data(),segments.size()*48)&&path==saved_path&&!std::memcmp(&result,&saved_result,sizeof(result))&&world.route_world.failed_count==cache_count&&!std::memcmp(saved_cache.data(),world.failed_routes.data(),saved_cache.size()*12);};
    request.reserved=1;require(dh2_nav_find_path(&request)==1&&unchanged(),"FindPath reserved rejection mutated state");request.reserved=0;++rejections;
    object.capacity=n;require(dh2_nav_find_path(&request)==2&&snapshot()==saved&&path==saved_path&&world.route_world.failed_count==cache_count,"FindPath capacity rejection mutated state");object.capacity=n+1;require(unchanged(),"Capacity recovery differs");++rejections;
    const unsigned capacity=world.route_workspace.internal_capacity;world.route_workspace.internal_capacity=n-1;require(dh2_nav_find_path(&request)==2&&unchanged(),"FindPath scratch rejection mutated state");world.route_workspace.internal_capacity=capacity;++rejections;
    world.collision_rooms[0].reserved=1;require(dh2_nav_find_path(&request)==1&&unchanged(),"FindPath room rejection mutated state");world.collision_rooms[0].reserved=0;++rejections;
    object.reserved=1;require(dh2_nav_find_path(&request)==1&&snapshot()==saved&&path==saved_path&&world.route_world.failed_count==cache_count,"FindPath object rejection mutated state");object.reserved=0;require(unchanged(),"Object recovery differs");++rejections;
    request.target[0]=std::numeric_limits<float>::quiet_NaN();require(dh2_nav_find_path(&request)==1&&unchanged(),"FindPath nonfinite rejection mutated state");request.target[0]=target[0];++rejections;
   }
  }
  require(r.at==r.bytes.size(),"Trailing FindPath gold");std::cout<<"{\"cases\":"<<cases<<",\"sessions\":"<<sessions<<",\"direct_paths\":"<<direct<<",\"graph_paths\":"<<graph<<",\"failed_paths\":"<<failed<<",\"referenced_floor_queries\":"<<query_records<<",\"atomic_rejection_checks\":"<<rejections<<",\"authored_graph_nodes\":"<<n<<",\"authored_graph_edges\":"<<e<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
