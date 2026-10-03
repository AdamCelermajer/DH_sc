#include "navigation.hpp"
#include "collision.hpp"
#include "selector.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::navigation;
namespace {
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated navigation reference");std::memcpy(out,bytes.data()+at,n);at+=n;}
 std::uint32_t word(){std::uint32_t v;read(&v,4);return v;}
};
bool equal_words(const void* a,const void* b,unsigned words){
 for(unsigned i=0;i<words;++i){std::uint32_t x,y;std::memcpy(&x,static_cast<const unsigned char*>(a)+4*i,4);std::memcpy(&y,static_cast<const unsigned char*>(b)+4*i,4);
  const auto nan=[](std::uint32_t v){return (v&0x7f800000)==0x7f800000&&(v&0x7fffff);};if(x!=y&&!(nan(x)&&nan(y)))return false;}
 return true;
}
struct Probe {float point[3];std::uint32_t result;};
struct Queries {std::vector<Probe> probes;unsigned at=0,floor=0;bool valid=true;dh2::collision::FloorSet* floor_set=nullptr;dh2::selector::FloorSet* selector_set=nullptr;};
std::uint32_t query(void* data,std::uint32_t floor,const float* p){
 auto& q=*static_cast<Queries*>(data);if(q.at>=q.probes.size()){q.valid=false;return 0;}
 const auto& probe=q.probes[q.at++];q.valid&=floor==q.floor&&equal_words(p,probe.point,3);
 if(q.selector_set){const unsigned result=dh2_selector_floor_query(q.selector_set,floor,p);q.valid&=result==probe.result;return result;}
 if(q.floor_set){const unsigned result=dh2_collision_floor_query(q.floor_set,floor,p);q.valid&=result==probe.result;return result;}
 return probe.result;
}
std::uint64_t digest(const Graph& g){
 std::uint64_t value=0xcbf29ce484222325ull;
 const auto append=[&](const void* p,std::size_t size){
  for(std::size_t i=0;i<size;i+=4){std::uint32_t word;std::memcpy(&word,static_cast<const unsigned char*>(p)+i,4);if((word&0x7f800000)==0x7f800000&&(word&0x7fffff))word=0x7fc00000;
   for(unsigned j=0;j<4;++j){value^=(word>>(8*j))&255;value*=0x100000001b3ull;}}
 };
 append(&g.node_count,16);append(g.nodes,g.node_count*sizeof(Node));append(g.edges,g.edge_count*sizeof(Edge));append(g.invalid,g.invalid_count*sizeof(InvalidNode));append(g.validation,g.validation_count*4);append(&g.root,16);return value;
}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;try{
  Reader r(argv[1]);const unsigned magic=r.word();if(magic!=0x3141564e&&magic!=0x3241564e&&magic!=0x3341564e)throw std::runtime_error("Navigation reference version");const unsigned batches=r.word();if(!batches||batches>1024)return 3;
  constexpr unsigned capacity=4096;std::vector<Node> nodes(capacity);std::vector<Edge> edges(capacity);std::vector<InvalidNode> invalid(capacity);std::vector<std::uint32_t> validation(capacity);Queries q;
  std::vector<std::vector<dh2::collision::Triangle>> floor_triangles;std::vector<dh2::collision::Floor> floors;dh2::collision::FloorSet floor_set{};
  struct SelectorStorage {
   std::vector<dh2::octree::Node> nodes=std::vector<dh2::octree::Node>(2048);
   std::vector<unsigned> indices=std::vector<unsigned>(8192),scratch=std::vector<unsigned>(2048),selected_ids;
   std::vector<dh2::collision::Triangle> selected_triangles;dh2::octree::Tree tree{};dh2::selector::Workspace workspace{};
  };
  std::vector<SelectorStorage> selector_storage;std::vector<dh2::selector::Floor> selector_floors;dh2::selector::FloorSet selector_set{};
  if(magic==0x3241564e||magic==0x3341564e){
   const unsigned count=r.word();if(!count||count>512)return 14;floors.resize(count);floor_triangles.resize(count);
   for(unsigned i=0;i<count;++i){const unsigned n=r.word();if(n>100000)return 15;floor_triangles[i].resize(n);r.read(floor_triangles[i].data(),n*36);floors[i]={floor_triangles[i].data(),n,0,{},{}};r.read(floors[i].minimum,24);}
   floor_set={floors.data(),count,0};q.floor_set=&floor_set;
   if(magic==0x3341564e){
    selector_storage.resize(count);selector_floors.resize(count);
    for(unsigned i=0;i<count;++i){
     auto& s=selector_storage[i];auto& geometry=floor_triangles[i];const unsigned n=geometry.size();s.selected_ids.resize(n);s.selected_triangles.resize(n);
     s.tree={s.nodes.data(),s.indices.data(),s.scratch.data(),geometry.data(),n,0,0,2048,8192,2048,15,0,0};if(dh2_octree_build(&s.tree,geometry.data(),n,15))return 16;
     s.workspace={s.selected_ids.data(),s.selected_triangles.data(),n,0};selector_floors[i]={{&s.tree,nullptr,1,0},{},&s.workspace};std::memcpy(&selector_floors[i].bounds,floors[i].minimum,24);
    }
    selector_set={selector_floors.data(),count,0};q.selector_set=&selector_set;
   }
  }
  unsigned cases=0,probes=0;Graph g{};
  for(unsigned b=0;b<batches;++b){
   g={nodes.data(),edges.data(),invalid.data(),validation.data(),0,0,0,0,capacity,capacity,capacity,capacity,0,0,0,0,query,&q};
   const unsigned count=r.word();if(count>10000)return 4;unsigned previous=~0u;
   for(unsigned i=0;i<count;++i){
    const unsigned floor=r.word(),flags=r.word();Triangle t;r.read(&t,sizeof(t));const unsigned n=r.word();if(n>6)return 5;
    q.probes.resize(n);r.read(q.probes.data(),n*sizeof(Probe));q.at=0;q.floor=floor;q.valid=true;
    std::array<std::uint32_t,4> expected;r.read(expected.data(),16);std::uint64_t expected_digest;r.read(&expected_digest,8);
    if(floor!=previous){if(dh2_nav_begin_floor(&g,floor))return 6;previous=floor;}
    if(dh2_nav_triangle(&g,&t,flags)||!q.valid||q.at!=n||!equal_words(&g.node_count,expected.data(),4)||digest(g)!=expected_digest){std::cerr<<"Navigation mismatch batch="<<b<<" triangle="<<i<<'\n';return 7;}
    ++cases;probes+=n;
   }
  }
  if(r.at!=r.bytes.size())return 8;
  const Graph saved=g;Triangle t{};t.points[0][0]=__builtin_inff();if(dh2_nav_triangle(&g,&t,0)!=1||std::memcmp(&g,&saved,sizeof(g)))return 9;
  t={};g.node_capacity=g.node_count;const Graph short_storage=g;if(dh2_nav_triangle(&g,&t,0)!=2||std::memcmp(&g,&short_storage,sizeof(g)))return 10;
  std::cout<<"{\"batches\":"<<batches<<",\"triangle_comparisons\":"<<cases<<",\"floor_query_comparisons\":"<<probes<<",\"native_floor_collision_used\":"<<(q.floor_set?"true":"false")<<",\"native_octree_selector_used\":"<<(q.selector_set?"true":"false")<<",\"atomic_rejection_checks\":2,\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 11;}
}
