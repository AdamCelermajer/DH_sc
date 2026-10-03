#include "world.hpp"
#include "navigation_producers.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
using namespace dh2::navigation;
namespace {
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){require(n<=bytes.size()-at,"Truncated producer gold");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
 std::vector<unsigned char> block(unsigned n){require(n<=10000000,"Producer gold budget");std::vector<unsigned char> v(n);read(v.data(),n);return v;}
};
unsigned get(const std::vector<unsigned char>& bytes,unsigned at){require(at+4<=bytes.size(),"Missing producer word");unsigned v;std::memcpy(&v,bytes.data()+at,4);return v;}
void append(std::vector<unsigned char>& bytes,const void* p,unsigned n){if(!n)return;const auto* v=static_cast<const unsigned char*>(p);bytes.insert(bytes.end(),v,v+n);}
std::vector<unsigned char> snapshot(const ObstacleRegistry& r){
 std::vector<unsigned> keys;if(r.floor_count)keys.assign(r.floors,r.floors+r.floor_count);
 std::sort(keys.begin(),keys.end(),[](unsigned a,unsigned b){return a==~0u?b!=~0u:b!=~0u&&a<b;});
 std::vector<unsigned char> bytes;append(bytes,&r.floor_count,4);append(bytes,&r.count,4);append(bytes,keys.data(),keys.size()*4);
 for(auto floor:keys)for(unsigned i=0;i<r.count;++i)if(r.entries[i].floor==floor)append(bytes,&r.entries[i],16);return bytes;
}
void restore(ObstacleRegistry& r,const std::vector<unsigned char>& bytes){
 r.floor_count=get(bytes,0);r.count=get(bytes,4);
 require(r.floor_count<=r.floor_capacity&&r.count<=r.capacity&&bytes.size()==8+r.floor_count*4+r.count*16,"Invalid producer registry gold");
 if(r.floor_count)std::memcpy(r.floors,bytes.data()+8,r.floor_count*4);
 if(r.count)std::memcpy(r.entries,bytes.data()+8+r.floor_count*4,r.count*16);
}
}
int main(int argc,char** argv){
 if(argc!=4)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31445250,"Invalid producer gold");const unsigned cases=r.word(),actors=r.word(),n=r.word(),e=r.word(),count=r.word();
  require(cases&&cases<=4096&&actors==8&&n==335&&e==838&&count==8,"Invalid producer gold budgets");
  std::uint64_t keys[8];r.read(keys,sizeof(keys));const auto nodes=r.block(n*56),edges=r.block(e*20);std::vector<FloorGraph> floors(count);r.read(floors.data(),count*48);require(r.word()==count,"Missing producer geometry");
  Reader a(argv[2]),b(argv[3]);dh2::resources::BresView view{};
  require(dh2_bres_open(&view,a.bytes.data(),a.bytes.size())==dh2::resources::BresError::ok,"Invalid authored BRES");
  dh2::world::Level level;std::string error;require(dh2::world::load(view,b.bytes.data(),b.bytes.size(),level,error),error.c_str());auto& world=*level.native_floor;
  require(world.graph.node_count==n&&world.graph.edge_count==e&&!std::memcmp(world.graph.nodes,nodes.data(),nodes.size())&&!std::memcmp(world.graph.edges,edges.data(),edges.size())&&!std::memcmp(world.floor_graphs.data(),floors.data(),count*48),"Native graph differs from producer gold");
  for(unsigned i=0;i<count;++i){const unsigned triangles=r.word();const auto expected=r.block(triangles*36);dh2::octree::Box box;r.read(&box,24);require(world.records[i]->triangles.size()==triangles&&!std::memcmp(world.records[i]->triangles.data(),expected.data(),expected.size())&&!std::memcmp(&world.records[i]->bounds,&box,24),"Native geometry differs from producer gold");}
  for(unsigned mode=0;mode<5;++mode){ObstacleTraits expected,actual;r.read(&expected,16);require(dh2_nav_producer_traits(&actual,mode)==0&&!std::memcmp(&actual,&expected,16),"Concrete producer traits mismatch");}
  NavigationObject objects[8];ObstacleEntry entries[512];unsigned buckets[16];ObstacleRegistry registry{entries,0,512,buckets,0,16};unsigned classes[5]{},null_user=0,physical=0,bounds=0,obstacles=0,queries=0,members=0,map_keys=0;
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned i=r.word(),before_size=r.word(),after_size=r.word(),q=r.word(),calls=r.word();require(i<8&&before_size<=10000&&after_size<=10000&&q<=32&&calls<=1,"Invalid producer record");
   ProducerFields fields;r.read(&fields,32);const auto before=r.block(512),before_registry=r.block(before_size),after=r.block(512),after_registry=r.block(after_size);r.block(q*16);r.block(calls*4);
   require(std::uint32_t(fields.type)<5,"Invalid producer class");std::memcpy(objects,before.data(),512);restore(registry,before_registry);const bool linked=objects[i].user!=0;
   ProducerRequest request{&world.collision_world,&registry,&objects[i],keys[i],&fields};
   require(dh2_nav_update_game_object(&request)==0,"Producer rejected original gold");
   if(std::memcmp(objects,after.data(),512)||snapshot(registry)!=after_registry){std::cerr<<"Producer mismatch case "<<ci<<'\n';return 4;}
   ++classes[std::uint32_t(fields.type)];null_user+=!linked;physical+=linked&&fields.physical_present;bounds+=linked&&!fields.physical_present;obstacles+=calls;queries+=q;members+=registry.count;map_keys+=registry.floor_count;
  }
  require(r.at==r.bytes.size(),"Trailing producer gold");unsigned rejections=0;
  NavigationObject object;require(dh2_nav_object_defaults(&object)==0,"Producer defaults failed");object.user=17;object.motion.floor=object.motion.room=0;
  registry={entries,0,512,buckets,0,16};ProducerFields fields{ProducerClass::character,1,.36f,0,{-36,-24},{36,24}};
  ProducerRequest request{&world.collision_world,&registry,&object,keys[0],&fields};
  auto reject=[&](int status){const auto saved=object;const auto saved_registry=snapshot(registry);require(dh2_nav_update_game_object(&request)==status&&!std::memcmp(&object,&saved,64)&&snapshot(registry)==saved_registry,"Producer rejection mutated state");++rejections;};
  fields.type=ProducerClass(5);reject(1);fields.type=ProducerClass::character;
  fields.physical_present=2;reject(1);fields.physical_present=1;
  fields.reserved=1;reject(1);fields.reserved=0;
  fields.physical_radius=std::numeric_limits<float>::infinity();reject(1);
  fields.physical_radius=std::numeric_limits<float>::max();reject(1);fields.physical_radius=.36f;
  fields.physical_present=0;fields.minimum[0]=std::numeric_limits<float>::quiet_NaN();reject(1);fields.minimum[0]=-36;
  fields.maximum[0]=-37;fields.maximum[1]=-25;reject(1);fields.maximum[0]=36;fields.maximum[1]=24;fields.physical_present=1;
  registry.capacity=0;reject(2);registry.capacity=512;registry.floor_capacity=0;reject(2);registry.floor_capacity=16;
  ObstacleTraits invalid{17,19,23,29};const auto saved_traits=invalid;require(dh2_nav_producer_traits(&invalid,5)==1&&!std::memcmp(&invalid,&saved_traits,16),"Invalid traits mutated output");++rejections;
  object.user=0;request.fields=nullptr;request.geometry=nullptr;request.registry=nullptr;const auto unlinked=object;
  require(dh2_nav_update_game_object(&request)==0&&!std::memcmp(&object,&unlinked,64),"Null user did not gate producer fields");
  std::cout<<"{\"cases\":"<<cases<<",\"comparisons\":"<<cases+5<<",\"trait_comparisons\":5,\"totals\":{\"updates\":"<<cases<<",\"class_counts\":[";
  for(unsigned i=0;i<5;++i)std::cout<<(i?",":"")<<classes[i];
  std::cout<<"],\"null_user\":"<<null_user<<",\"physical_radius\":"<<physical<<",\"bounds_radius\":"<<bounds<<",\"obstacle_calls\":"<<obstacles<<",\"floor_queries\":"<<queries<<",\"registry_entries_compared\":"<<members<<",\"registry_keys_compared\":"<<map_keys<<"},\"atomic_rejection_checks\":"<<rejections<<",\"null_user_early_gate_checks\":1,\"authored_graph_nodes\":"<<n<<",\"authored_graph_edges\":"<<e<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
