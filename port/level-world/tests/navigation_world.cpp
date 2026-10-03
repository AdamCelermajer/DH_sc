#include "world.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
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
  Reader r(argv[1]);require(r.word()==0x32545257,"Invalid world-route reference");const unsigned cases=r.word(),n=r.word(),e=r.word(),count=r.word();require(cases&&cases<=4096&&n==335&&e==838&&count==8,"Invalid authored route budgets");
  const auto nodes=r.block(n*56),edges=r.block(e*20);std::vector<FloorGraph> floors(count);r.read(floors.data(),count*48);require(r.word()==count,"Missing world geometry");
  Reader a(argv[2]),b(argv[3]);dh2::resources::BresView view{};require(dh2_bres_open(&view,a.bytes.data(),a.bytes.size())==dh2::resources::BresError::ok,"Invalid authored BRES");dh2::world::Level level;std::string error;require(dh2::world::load(view,b.bytes.data(),b.bytes.size(),level,error),error.c_str());auto& world=*level.native_floor;
  require(world.graph.node_count==n&&world.graph.edge_count==e&&!std::memcmp(world.graph.nodes,nodes.data(),nodes.size())&&!std::memcmp(world.graph.edges,edges.data(),edges.size())&&!std::memcmp(world.floor_graphs.data(),floors.data(),count*48),"Native asset-loader graph differs from original gold");
  for(unsigned i=0;i<count;++i){const unsigned triangles=r.word();const auto expected=r.block(triangles*36);dh2::octree::Box box;r.read(&box,24);require(world.records[i]->triangles.size()==triangles&&!std::memcmp(world.records[i]->triangles.data(),expected.data(),expected.size())&&!std::memcmp(&world.records[i]->bounds,&box,24),"Native authored geometry differs from route gold");require(world.collision_rooms[i].count==1&&world.collision_rooms[i].floors[0]==i&&!std::memcmp(&world.collision_rooms[i].bounds,&box,24),"Native room geometry differs from caller gold");}
  RouteObject object;unsigned direct=0,graph=0,failed=0,rejections=0,sessions=0,query_records=0;
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned reset=r.word(),disabled=r.word();require(reset<=1&&disabled<256,"Invalid route session state");std::vector<FloorTraits> traits(count);r.read(traits.data(),count*8);float source[3],target[3];r.read(source,12);r.read(target,12);const unsigned limit=r.word(),flags=r.word();float radius;r.read(&radius,4);const unsigned actor=r.word(),output=r.word(),enabled=r.word(),prefix=r.word();require(actor<=1&&output<=1&&enabled<=1&&prefix<=4096,"Invalid route request");
   std::vector<unsigned> path(n+prefix+1);r.read(path.data(),prefix*4);const auto expected=r.block(r.word());require(expected.size()>=40,"Missing route result");const auto direct_points=r.block(24),cache=r.block(r.word());const unsigned queries=r.word();require(queries<=16,"Unexpected floor-query count");r.block(queries*16);query_records+=queries;
   if(reset){++sessions;std::memset(&object,0xcc,sizeof(object));world.floor_graphs=floors;dh2::floors::clear_route_cache(world);for(unsigned i=0;i<count;++i){if(disabled&(1u<<i))world.floor_graphs[i].root=0;world.route_traits[i]=traits[i];world.collision_floors[i].traits=traits[i];}world.route_world.floors=world.floor_graphs.data();}
   object.flags=flags;object.radius=radius;RouteResult result{0,0,0,0,{0,0,0,0,0,prefix,path.data(),unsigned(path.size()),0}};RouteRequest request{&world.route_world,&world.collision_world,actor?&object:nullptr,{},{},limit,enabled,output,0};std::copy(source,source+3,request.source);std::copy(target,target+3,request.target);
   require(dh2_nav_route(&result,&request,&world.route_workspace)==0,"Native world route rejected reference");
   const unsigned expected_count=[&](){unsigned value;std::memcpy(&value,expected.data()+36,4);return value;}();require(expected.size()==40+expected_count*4&&expected_count<=path.size(),"Invalid route output length");
   const unsigned cache_count=world.route_world.failed_count;require(cache.size()==4+cache_count*12,"Native route cache count mismatch");unsigned stored_count;std::memcpy(&stored_count,cache.data(),4);
   if(std::memcmp(&result,expected.data(),40)||std::memcmp(path.data(),expected.data()+40,expected_count*4)||std::memcmp(object.direct_source,direct_points.data(),24)||stored_count!=cache_count||std::memcmp(world.failed_routes.data(),cache.data()+4,cache_count*12)){std::cerr<<"World route mismatch case "<<ci<<'\n';return 4;}
   direct+=result.kind==1;graph+=result.kind==2;failed+=!result.found;
   if(!rejections){
    const auto before=result;const auto saved_object=object;const auto saved_path=path;const unsigned saved_cache=world.route_world.failed_count;const auto unchanged=[&](){return !std::memcmp(&result,&before,sizeof(result))&&!std::memcmp(&object,&saved_object,sizeof(object))&&saved_path==path&&saved_cache==world.route_world.failed_count;};
    request.reserved=1;require(dh2_nav_route(&result,&request,&world.route_workspace)==1&&unchanged(),"Reserved-field rejection mutated route");request.reserved=0;++rejections;
    const unsigned capacity=world.route_workspace.internal_capacity;world.route_workspace.internal_capacity=n-1;require(dh2_nav_route(&result,&request,&world.route_workspace)==2&&unchanged(),"Scratch rejection mutated route");world.route_workspace.internal_capacity=capacity;++rejections;
    const unsigned reserved=world.collision_rooms[0].reserved;world.collision_rooms[0].reserved=1;require(dh2_nav_route(&result,&request,&world.route_workspace)==1&&unchanged(),"Room rejection mutated route");world.collision_rooms[0].reserved=reserved;++rejections;
    const unsigned root=world.floor_graphs[0].root;world.floor_graphs[0].root=n+1;require(dh2_nav_route(&result,&request,&world.route_workspace)==1&&unchanged(),"Root rejection mutated route");world.floor_graphs[0].root=root;++rejections;
    if(result.search.path_count){path[0]=e+1;const auto invalid_path=path;require(dh2_nav_route(&result,&request,&world.route_workspace)==1&&!std::memcmp(&result,&before,sizeof(result))&&!std::memcmp(&object,&saved_object,sizeof(object))&&path==invalid_path&&saved_cache==world.route_world.failed_count,"Invalid caller edge mutated route");path[0]=saved_path[0];++rejections;}
   }
  }
  require(r.at==r.bytes.size(),"Trailing world-route gold");std::cout<<"{\"cases\":"<<cases<<",\"sessions\":"<<sessions<<",\"direct_routes\":"<<direct<<",\"graph_routes\":"<<graph<<",\"failed_routes\":"<<failed<<",\"referenced_floor_queries\":"<<query_records<<",\"atomic_rejection_checks\":"<<rejections<<",\"authored_graph_nodes\":"<<n<<",\"authored_graph_edges\":"<<e<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
