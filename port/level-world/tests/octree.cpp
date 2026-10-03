#include "octree.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>
using namespace dh2::octree;
using dh2::collision::Triangle;
namespace {
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){
  if(n>bytes.size()-at)throw std::runtime_error("Truncated octree reference");
  if(n)std::memcpy(out,bytes.data()+at,n);
  at+=n;
 }
 unsigned word(){unsigned v;read(&v,4);return v;}
};
void require(bool condition,const char* error){if(!condition)throw std::runtime_error(error);}
struct Storage {
 std::vector<Node> nodes=std::vector<Node>(4096);
 std::vector<unsigned> indices=std::vector<unsigned>(32768),scratch=std::vector<unsigned>(4096);
 Tree tree{nodes.data(),indices.data(),scratch.data(),nullptr,0,0,0,4096,32768,4096,0,0,0};
};
void compare_node(Reader& r,const Tree& t,unsigned id,unsigned& nodes,unsigned& triangles){
 require(id>0&&id<=t.node_count,"Invalid native node");const auto& n=t.nodes[id-1];
 Box expected;r.read(&expected,24);const unsigned count=r.word(),mask=r.word();
 require(std::memcmp(&n.box,&expected,24)==0,"Octree bounds mismatch");
 require(n.count==count,"Octree partition size mismatch");
 unsigned actual_mask=0;for(unsigned i=0;i<8;++i)actual_mask|=bool(n.children[i])<<i;
 require(mask==actual_mask,"Octree child order mismatch");
 for(unsigned i=0;i<count;++i){
  Triangle expected_triangle;r.read(&expected_triangle,36);const unsigned index=t.indices[n.first+i];
  require(index<t.triangle_count&&std::memcmp(&t.triangles[index],&expected_triangle,36)==0,"Octree triangle order mismatch");
 }
 ++nodes;triangles+=count;
 for(unsigned child:n.children)if(child)compare_node(r,t,child,nodes,triangles);
}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x3154434f,"Invalid octree reference");const unsigned builds=r.word();require(builds<=10000,"Too many references");
  unsigned nodes=0,triangles=0,queries=0,selected=0,bounded_queries=0,rejections=0;
  for(unsigned batch=0;batch<builds;++batch){
   const unsigned count=r.word(),leaf=r.word(),expected_nodes=r.word();require(count>0&&count<=4096,"Invalid triangle count");
   std::vector<Triangle> geometry(count);r.read(geometry.data(),count*36);Storage s;
   require(dh2_octree_build(&s.tree,geometry.data(),count,leaf)==0,"Octree build failed");
   require(s.tree.node_count==expected_nodes,"Octree node count mismatch");
   const auto tree_length=r.word();const auto tree_end=r.at+tree_length;compare_node(r,s.tree,s.tree.root,nodes,triangles);require(r.at==tree_end,"Octree reference length mismatch");
   const auto query_count=r.word();std::vector<unsigned> out(count+2,0xdeadbeef);
   for(unsigned q=0;q<query_count;++q){
    Box box;r.read(&box,24);const unsigned expected_count=r.word();require(expected_count<=count,"Invalid selection count");
    std::vector<Triangle> expected(expected_count);r.read(expected.data(),36*expected_count);
    require(dh2_octree_box(&s.tree,&box,out.data()+1,count)==expected_count,"Octree selection count mismatch");
    require(out.front()==0xdeadbeef&&out.back()==0xdeadbeef,"Query wrote outside output");
    for(unsigned i=0;i<expected_count;++i)require(std::memcmp(&geometry.at(out[i+1]),&expected[i],36)==0,"Octree query order mismatch");
    ++queries;selected+=expected_count;
    // Modern output contract: bounded prefixes, even when later siblings match.
    for(unsigned capacity: {0u,1u,expected_count/2}){
     std::fill(out.begin(),out.end(),0xdeadbeef);const unsigned wanted=std::min(capacity,expected_count);
     require(dh2_octree_box(&s.tree,&box,capacity?out.data()+1:nullptr,capacity)==wanted,"Bounded selection mismatch");
     require(out[0]==0xdeadbeef&&out[capacity+1]==0xdeadbeef,"Bounded query overrun");
     for(unsigned i=0;i<wanted;++i)require(std::memcmp(&geometry.at(out[i+1]),&expected[i],36)==0,"Bounded query prefix mismatch");
     ++bounded_queries;
    }
   }
   // Exhaustion must discard the partially built tree, without out-of-bounds writes.
   s.tree.index_capacity=count-1;require(dh2_octree_build(&s.tree,geometry.data(),count,leaf)==2,"Missing index-capacity rejection");
   require(!s.tree.root&&!s.tree.node_count&&!s.tree.index_count,"Failed build exposed tree");++rejections;
   s.tree.index_capacity=32768;s.tree.node_capacity=1;
   const int status=dh2_octree_build(&s.tree,geometry.data(),count,leaf);
   if(expected_nodes>1){require(status==2&&!s.tree.root&&!s.tree.node_count&&!s.tree.index_count,"Missing recursive capacity rejection");++rejections;}
   else require(status==0,"Unnecessary capacity rejection");
  }
  require(r.at==r.bytes.size(),"Trailing octree reference bytes");
  Storage empty;require(dh2_octree_build(&empty.tree,nullptr,0,15)==0&&!empty.tree.root,"Empty build failed");Box box{};
  require(dh2_octree_box(&empty.tree,&box,nullptr,0)==0,"Empty query failed");
  require(dh2_octree_box(nullptr,&box,nullptr,0)==std::numeric_limits<unsigned>::max(),"Missing invalid tree rejection");++rejections;
  Triangle invalid{};invalid.points[0][0]=std::numeric_limits<float>::quiet_NaN();const Tree saved=empty.tree;
  require(dh2_octree_build(&empty.tree,&invalid,1,15)==1&&std::memcmp(&empty.tree,&saved,sizeof(Tree))==0,"Invalid input modified tree");++rejections;
  std::cout<<"{\"build_comparisons\":"<<builds<<",\"node_comparisons\":"<<nodes<<",\"partitioned_triangle_comparisons\":"<<triangles<<",\"box_comparisons\":"<<queries<<",\"selected_triangle_comparisons\":"<<selected<<",\"bounded_query_checks\":"<<bounded_queries<<",\"storage_rejection_checks\":"<<rejections<<",\"empty_tree_checks\":2,\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
