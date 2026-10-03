#include "world.hpp"
#include "navigation_objects.hpp"
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
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated object reference");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
 std::vector<unsigned char> block(unsigned n){if(n>10000000)throw std::runtime_error("Object reference budget");std::vector<unsigned char> v(n);read(v.data(),n);return v;}
};
void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
unsigned get(const std::vector<unsigned char>& raw,unsigned at){require(at+4<=raw.size(),"Missing object input word");unsigned v;std::memcpy(&v,raw.data()+at,4);return v;}
void append(std::vector<unsigned char>& bytes,const void* p,unsigned n){const auto* v=static_cast<const unsigned char*>(p);bytes.insert(bytes.end(),v,v+n);}
std::vector<unsigned char> snapshot(const ObstacleRegistry& r){
 std::vector<unsigned> keys;if(r.floor_count)keys.assign(r.floors,r.floors+r.floor_count);std::sort(keys.begin(),keys.end(),[](unsigned a,unsigned b){return a==~0u?b!=~0u:b!=~0u&&a<b;});
 std::vector<unsigned char> bytes;append(bytes,&r.floor_count,4);append(bytes,&r.count,4);if(!keys.empty())append(bytes,keys.data(),keys.size()*4);
 for(auto floor:keys)for(unsigned i=0;i<r.count;++i)if(r.entries[i].floor==floor)append(bytes,&r.entries[i],16);return bytes;
}
void restore(ObstacleRegistry& r,const std::vector<unsigned char>& raw){r.floor_count=get(raw,0);r.count=get(raw,4);require(r.floor_count<=r.floor_capacity&&r.count<=r.capacity&&raw.size()==8+r.floor_count*4+r.count*16,"Invalid registry gold");if(r.floor_count)std::memcpy(r.floors,raw.data()+8,r.floor_count*4);if(r.count)std::memcpy(r.entries,raw.data()+8+r.floor_count*4,r.count*16);}
}
int main(int argc,char** argv){
 if(argc!=4)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x314a424f,"Invalid object gold");const unsigned cases=r.word(),actors=r.word(),n=r.word(),e=r.word(),count=r.word();require(cases&&cases<=4096&&actors==8&&n==335&&e==838&&count==8,"Invalid object gold budgets");std::uint64_t keys[8];r.read(keys,sizeof(keys));const auto nodes=r.block(n*56),edges=r.block(e*20);std::vector<FloorGraph> floors(count);r.read(floors.data(),count*48);require(r.word()==count,"Missing object geometry");
  Reader a(argv[2]),b(argv[3]);dh2::resources::BresView view{};require(dh2_bres_open(&view,a.bytes.data(),a.bytes.size())==dh2::resources::BresError::ok,"Invalid authored BRES");dh2::world::Level level;std::string error;require(dh2::world::load(view,b.bytes.data(),b.bytes.size(),level,error),error.c_str());auto& world=*level.native_floor;
  require(world.graph.node_count==n&&world.graph.edge_count==e&&!std::memcmp(world.graph.nodes,nodes.data(),nodes.size())&&!std::memcmp(world.graph.edges,edges.data(),edges.size())&&!std::memcmp(world.floor_graphs.data(),floors.data(),count*48),"Native asset-loader graph differs from object gold");
  for(unsigned i=0;i<count;++i){const unsigned triangles=r.word();const auto expected=r.block(triangles*36);dh2::octree::Box box;r.read(&box,24);require(world.records[i]->triangles.size()==triangles&&!std::memcmp(world.records[i]->triangles.data(),expected.data(),expected.size())&&!std::memcmp(&world.records[i]->bounds,&box,24),"Native geometry differs from object gold");}
  NavigationObject objects[8];ObstacleEntry entries[512];unsigned buckets[16];ObstacleRegistry registry{entries,0,512,buckets,0,16};unsigned totals[8]{},query_records=0,members=0,map_keys=0;
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned op=r.word(),i=r.word(),parameter_size=r.word(),extra_size=r.word(),before_size=r.word(),after_size=r.word(),queries=r.word();require(op<8&&i<actors&&parameter_size<=64&&extra_size<=36&&before_size<=10000&&after_size<=10000&&queries<=32,"Invalid object request");const auto parameter=r.block(parameter_size),before=r.block(actors*64),before_registry=r.block(before_size),extra_expected=r.block(extra_size),expected=r.block(actors*64),expected_registry=r.block(after_size);r.block(queries*16);query_records+=queries;std::memcpy(objects,before.data(),before.size());restore(registry,before_registry);auto& object=objects[i];unsigned char extra[36]{};int status=0;
   if(op==0){require(parameter_size==0&&extra_size==0,"Invalid defaults record");status=dh2_nav_object_defaults(&object);
   }else if(op==1){require(parameter_size==28&&extra_size==0,"Invalid InitObject record");ObjectInitRequest request{&world.collision_world,&object,0,{},0,get(parameter,0),0};std::memcpy(&request.user,parameter.data()+4,8);std::memcpy(request.position,parameter.data()+12,12);std::memcpy(&request.radius,parameter.data()+24,4);status=dh2_nav_init_object(&request);
   }else if(op==2){require(parameter_size==12&&extra_size==0,"Invalid InitObstacle record");ObstacleInitRequest request{&world.collision_world,&registry,&object,keys[i],0,0,get(parameter,0),0};std::memcpy(&request.weight,parameter.data()+4,4);std::memcpy(&request.extent,parameter.data()+8,4);status=dh2_nav_init_obstacle(&request);
   }else if(op==3){require(parameter_size==4&&extra_size==0,"Invalid parent record");status=dh2_nav_change_obstacle_parent(&registry,&object,keys[i],get(parameter,0));
   }else if(op==4||op==5){require(parameter_size==4&&extra_size==8,"Invalid capability record");status=op==4?dh2_nav_object_set_flying(&object,get(parameter,0)):dh2_nav_object_set_swimming(&object,get(parameter,0));const unsigned value[]{unsigned(dh2_nav_object_is_flying(&object)),unsigned(dh2_nav_object_is_swimming(&object))};std::memcpy(extra,value,8);
   }else if(op==6){require(parameter_size==20&&extra_size==36,"Invalid position backend record");float point[3];MotionPolicy policy;std::memcpy(point,parameter.data(),12);std::memcpy(&policy,parameter.data()+12,8);ObjectPositionRequest request{&world.collision_world,&registry,&object,keys[i],point,&policy};PositionResult result{};status=dh2_nav_validate_object_position(&result,&request);std::memcpy(extra,&result,24);std::memcpy(extra+24,point,12);
   }else{require(parameter_size==0&&extra_size==8,"Invalid default policy record");MotionPolicy policy;status=dh2_nav_motion_policy_defaults(&policy);std::memcpy(extra,&policy,8);}
   require(status==0,"Object source rejected original gold");if(std::memcmp(objects,expected.data(),expected.size())||(extra_size&&std::memcmp(extra,extra_expected.data(),extra_size))||snapshot(registry)!=expected_registry){std::cerr<<"Object mismatch case "<<ci<<" operation "<<op<<'\n';return 4;}++totals[op];members+=registry.count;map_keys+=registry.floor_count;
  }
  require(r.at==r.bytes.size(),"Trailing object gold");unsigned rejections=0;
  NavigationObject object;require(dh2_nav_object_defaults(&object)==0,"Defaults failed");float source[3];for(unsigned k=0;k<3;++k)source[k]=float((double(world.records[0]->triangles.front().points[0][k])+world.records[0]->triangles.front().points[1][k]+world.records[0]->triangles.front().points[2][k])/3.);
  ObjectInitRequest init{&world.collision_world,&object,0x100000009ull,{},36,0,0};std::memcpy(init.position,source,12);require(dh2_nav_init_object(&init)==0&&object.user==init.user,"High key initialization failed");
  const auto saved=object;init.radius=std::numeric_limits<float>::infinity();require(dh2_nav_init_object(&init)==1&&!std::memcmp(&object,&saved,64),"Nonfinite radius rejection mutated object");++rejections;init.radius=36;
  init.flying=2;require(dh2_nav_init_object(&init)==1&&!std::memcmp(&object,&saved,64),"Invalid InitObject bool mutated object");++rejections;init.flying=0;
  registry={entries,0,0,buckets,0,16};ObstacleInitRequest obstacle{&world.collision_world,&registry,&object,keys[0],1,36,1,0};const auto registry_saved=registry;require(dh2_nav_init_obstacle(&obstacle)==2&&!std::memcmp(&object,&saved,64)&&!std::memcmp(&registry,&registry_saved,32),"Registration capacity rejection mutated state");++rejections;
  registry.capacity=512;registry.floor_capacity=0;const auto no_buckets=registry;require(dh2_nav_init_obstacle(&obstacle)==2&&!std::memcmp(&object,&saved,64)&&!std::memcmp(&registry,&no_buckets,32),"Bucket capacity rejection mutated state");++rejections;
  registry.floor_capacity=16;obstacle.weight=-1;require(dh2_nav_init_obstacle(&obstacle)==1&&!std::memcmp(&object,&saved,64)&&registry.count==0&&registry.floor_count==0,"Negative weight rejection mutated state");++rejections;obstacle.weight=1;
  require(dh2_nav_init_obstacle(&obstacle)==0&&registry.count==1&&registry.floor_count==1,"Initial registration failed");registry.floor_capacity=1;const auto registered=object;const auto registered_registry=snapshot(registry);require(dh2_nav_change_obstacle_parent(&registry,&object,keys[0],1)==2&&!std::memcmp(&object,&registered,64)&&snapshot(registry)==registered_registry,"Relocation capacity rejection mutated state");++rejections;
  MotionPolicy policy{1000,0};float point[3];for(unsigned k=0;k<3;++k)point[k]=float((double(world.records[1]->triangles.back().points[0][k])+world.records[1]->triangles.back().points[1][k]+world.records[1]->triangles.back().points[2][k])/3.);point[2]+=36;float point_saved[3];std::memcpy(point_saved,point,12);PositionResult result{13,17,19,23,29,31};const auto result_saved=result;ObjectPositionRequest position{&world.collision_world,&registry,&object,keys[0],point,&policy};require(dh2_nav_validate_object_position(&result,&position)==2&&!std::memcmp(&object,&registered,64)&&!std::memcmp(&result,&result_saved,24)&&!std::memcmp(point,point_saved,12)&&snapshot(registry)==registered_registry,"Position backend capacity rejection mutated state");++rejections;
  registry.floor_capacity=16;entries[0].reserved=1;const auto corrupt_entry=entries[0];require(dh2_nav_validate_object_position(&result,&position)==1&&!std::memcmp(&object,&registered,64)&&!std::memcmp(&result,&result_saved,24)&&!std::memcmp(point,point_saved,12)&&!std::memcmp(entries,&corrupt_entry,16),"Malformed registry rejection mutated state");++rejections;entries[0].reserved=0;
  require(dh2_nav_object_set_flying(&object,2)==1&&!std::memcmp(&object,&registered,64),"Capability rejection mutated state");++rejections;
  std::cout<<"{\"cases\":"<<cases<<",\"operation_counts\":[";for(unsigned i=0;i<8;++i)std::cout<<(i?",":"")<<totals[i];std::cout<<"],\"referenced_floor_queries\":"<<query_records<<",\"registry_entries_compared\":"<<members<<",\"registry_keys_compared\":"<<map_keys<<",\"atomic_rejection_checks\":"<<rejections<<",\"authored_graph_nodes\":"<<n<<",\"authored_graph_edges\":"<<e<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
