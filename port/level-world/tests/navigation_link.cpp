#include "../tools/navigation_state.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2::navigation;
namespace {
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(std::vector<unsigned char> source):bytes(std::move(source)){}
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated Link reference");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned value;read(&value,4);return value;}
 std::vector<unsigned char> block(){const unsigned n=word();if(n>10000000||n>bytes.size()-at)throw std::runtime_error("Oversized Link reference");std::vector<unsigned char> result(n);read(result.data(),n);return result;}
};
bool equal(const std::vector<unsigned char>& a,const std::vector<unsigned char>& b){
 if(a.size()!=b.size())return false;
 for(std::size_t i=0;i<a.size();i+=4){unsigned x,y;std::memcpy(&x,a.data()+i,4);std::memcpy(&y,b.data()+i,4);const auto nan=[](unsigned v){return (v&0x7f800000)==0x7f800000&&(v&0x7fffff);};if(x!=y&&!(nan(x)&&nan(y)))return false;}
 return true;
}
unsigned query(void*,unsigned,const float*){throw std::runtime_error("Forced boundary creation queried floor support");}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try{
  std::ifstream input(argv[1],std::ios::binary);Reader r({std::istreambuf_iterator<char>(input),{}});if(r.word()!=0x314b4e4c)return 3;const unsigned cases=r.word();if(!cases||cases>2048)return 4;
  std::vector<Node> nodes(4096);std::vector<Edge> edges(32768);std::vector<InvalidNode> invalid(4096);std::vector<unsigned> validation(32768),owners(32768);
  std::vector<BoundaryNode> first(4096),second(4096);std::vector<FloorLink> links(4096);std::vector<FloorGraph> floors;
  unsigned operations=0,links_tested=0,postloads=0,atomic=0;std::uint64_t crypt_digest=0;unsigned crypt_nodes=0,crypt_edges=0;
  for(unsigned ci=0;ci<cases;++ci){
   Reader initial(r.block());const unsigned n=initial.word(),e=initial.word(),inv=initial.word(),v=initial.word(),count=initial.word(),linked=initial.word();
   if(n>nodes.size()||e>edges.size()||inv>invalid.size()||v>validation.size()||count>512||linked>links.size())return 5;floors.resize(count);
   Graph g{nodes.data(),edges.data(),invalid.data(),validation.data(),n,e,inv,v,unsigned(nodes.size()),unsigned(edges.size()),unsigned(invalid.size()),unsigned(validation.size()),0,0,0,0,query,nullptr};
   LinkWorkspace w{first.data(),second.data(),unsigned(first.size()),0,owners.data(),links.data(),linked,unsigned(links.size())};
   initial.read(&g.root,16);initial.read(g.nodes,n*sizeof(Node));initial.read(g.edges,e*sizeof(Edge));initial.read(g.invalid,inv*sizeof(InvalidNode));initial.read(g.validation,v*4);initial.read(owners.data(),v*4);initial.read(floors.data(),count*sizeof(FloorGraph));initial.read(links.data(),linked*sizeof(FloorLink));if(initial.at!=initial.bytes.size())return 6;
   const unsigned steps=r.word();if(steps>20000)return 7;
   for(unsigned step=0;step<steps;++step){const unsigned a=r.word(),b=r.word();const auto expected=r.block();if(a>=count)return 8;
    if(b==0xffffffff){floors[a].invalid_count=0;++postloads;}
    else{if(b>=count)return 9;
     if(ci==0&&atomic==0){
      const auto before=link_state(g,floors.data(),count,w);const unsigned capacity=g.node_capacity;g.node_capacity=g.node_count;
      if(dh2_nav_link(&g,&floors[a],&floors[b],&w)!=2||!equal(before,link_state(g,floors.data(),count,w)))return 10;g.node_capacity=capacity;++atomic;
      const auto original=floors[a];floors[a].invalid_count=g.invalid_count+1;
      if(dh2_nav_link(&g,&floors[a],&floors[b],&w)!=1)return 11;floors[a]=original;if(!equal(before,link_state(g,floors.data(),count,w)))return 12;++atomic;
     }
     if(dh2_nav_link(&g,&floors[a],&floors[b],&w))return 13;++links_tested;
    }
    if(!equal(expected,link_state(g,floors.data(),count,w))){std::cerr<<"Link mismatch case="<<ci<<" operation="<<step<<'\n';return 14;}++operations;
   }
   if(ci==0){crypt_nodes=g.node_count;crypt_edges=g.edge_count;crypt_digest=link_state_digest(link_state(g,floors.data(),count,w));}
  }
  if(r.at!=r.bytes.size())return 15;
  std::cout<<"{\"cases\":"<<cases<<",\"logical_snapshot_comparisons\":"<<operations<<",\"link_calls\":"<<links_tested<<",\"floor_postloads\":"<<postloads<<",\"atomic_rejection_checks\":"<<atomic<<",\"crypt_nodes\":"<<crypt_nodes<<",\"crypt_edges\":"<<crypt_edges<<",\"crypt_state_fnv1a64\":\""<<std::hex<<crypt_digest<<std::dec<<"\",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 16;}
}
