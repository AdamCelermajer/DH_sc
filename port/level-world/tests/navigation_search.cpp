#include "navigation_search.hpp"
#include "world.hpp"
#include <algorithm>
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
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated search reference");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned n;read(&n,4);return n;}
};
void require(bool condition,const char* error){if(!condition)throw std::runtime_error(error);}
struct Predicates {std::vector<unsigned> nodes,edges,events;};
unsigned answer(Predicates& p,unsigned kind,unsigned id){const unsigned value=kind==0?(p.nodes.at(id-1)&1):kind==1?!p.edges.at(id-1):!(p.nodes.at(id-1)&2);p.events.insert(p.events.end(),{kind,id,value});return value;}
unsigned goal(void* user,unsigned id){return answer(*static_cast<Predicates*>(user),0,id);}
unsigned edge(void* user,unsigned id){return answer(*static_cast<Predicates*>(user),1,id);}
unsigned node(void* user,unsigned id){return answer(*static_cast<Predicates*>(user),2,id);}
}
int main(int argc,char** argv){
 if(argc!=2&&argc!=4)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31435253,"Invalid search reference");const unsigned cases=r.word();require(cases&&cases<=10000,"Invalid search case count");unsigned calls=0,records=0,rejections=0,found=0,authored=0;
  dh2::world::Level level;std::vector<unsigned char> bres,descriptor;
  if(argc==4){Reader a(argv[2]),b(argv[3]);bres=std::move(a.bytes);descriptor=std::move(b.bytes);dh2::resources::BresView view{};std::string error;require(dh2_bres_open(&view,bres.data(),bres.size())==dh2::resources::BresError::ok,"Invalid authored BRES");require(dh2::world::load(view,descriptor.data(),descriptor.size(),level,error),error.c_str());}
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned n=r.word(),e=r.word(),start=r.word(),limit=r.word(),external=r.word(),prefix=r.word();require(n<=4096&&e<=32768&&prefix<=4096,"Search fixture budget exceeded");
   std::vector<Node> nodes(n);std::vector<Edge> edges(e);r.read(nodes.data(),n*56);r.read(edges.data(),e*20);Predicates p;p.nodes.resize(n);p.edges.resize(e);r.read(p.nodes.data(),n*4);r.read(p.edges.data(),e*4);
   std::vector<unsigned> path(n+prefix),prefix_values(prefix);r.read(prefix_values.data(),prefix*4);std::copy(prefix_values.begin(),prefix_values.end(),path.begin());
   unsigned expected[6];r.read(expected,24);require(expected[5]<=path.size(),"Search reference path budget exceeded");std::vector<unsigned> expected_path(expected[5]);r.read(expected_path.data(),expected[5]*4);
   std::vector<SearchNode> states(n),expected_states(n);r.read(expected_states.data(),n*16);const unsigned event_count=r.word();require(event_count<=100000,"Search reference event budget exceeded");std::vector<unsigned> expected_events(event_count*3);r.read(expected_events.data(),event_count*12);
   std::vector<unsigned> order,offsets{0};for(unsigned i=1;i<=n;++i){std::vector<std::pair<unsigned,unsigned>> outgoing;for(unsigned j=0;j<e;++j)if(edges[j].from==i)outgoing.push_back({edges[j].to,j+1});std::sort(outgoing.begin(),outgoing.end());for(auto entry:outgoing)order.push_back(entry.second);offsets.push_back(order.size());}
   Graph g{nodes.data(),edges.data(),nullptr,nullptr,n,e,0,0,n,e,0,0,0,0,0,0,nullptr,nullptr};SearchGraph view{&g,order.data(),offsets.data()};SearchTest test{goal,edge,node,&p};SearchRequest request{&view,&test,start,limit,external,0};
   std::vector<SearchEntry> heap(e+1);SearchWorkspace workspace{states.data(),heap.data(),n,e+1};SearchResult result{0,0,0,0,0,prefix,path.data(),unsigned(path.size()),0};
   require(dh2_nav_search(&result,&request,&workspace)==0,"Native search rejected reference");
   if(std::memcmp(&result,expected,24)||(n&&std::memcmp(states.data(),expected_states.data(),n*16))||p.events!=expected_events||!std::equal(expected_path.begin(),expected_path.end(),path.begin())){std::cerr<<"Search mismatch case "<<ci<<'\n';return 4;}
   calls+=event_count;records+=n;found+=result.found!=0;
   if(argc==4&&n==335&&e==838){
    auto& world=*level.native_floor;const auto& graph=world.graph;require(graph.node_count==n&&graph.edge_count==e&&!std::memcmp(graph.nodes,nodes.data(),n*56)&&!std::memcmp(graph.edges,edges.data(),e*20),"Authored native graph differs from original search fixture");
    std::fill(path.begin(),path.end(),0);std::copy(prefix_values.begin(),prefix_values.end(),path.begin());p.events.clear();result={0,0,0,0,0,prefix,path.data(),unsigned(path.size()),0};
    require(dh2::floors::search_nodes(world,start,limit,test,result)==0,"Authored search rejected reference");require(!std::memcmp(&result,expected,24)&&!std::memcmp(world.search_nodes.data(),expected_states.data(),n*16)&&p.events==expected_events&&std::equal(expected_path.begin(),expected_path.end(),path.begin()),"Authored search differs from original instructions");++authored;
   }
   if(n&&e&&rejections==0){
    const auto before=result;const auto saved_states=states;const auto saved_path=path,saved_events=p.events;const auto unchanged=[&](){return !std::memcmp(&result,&before,sizeof(result))&&!std::memcmp(states.data(),saved_states.data(),n*16)&&path==saved_path&&p.events==saved_events;};
    --workspace.heap_capacity;require(dh2_nav_search(&result,&request,&workspace)==2&&unchanged(),"Capacity rejection mutated search output");++workspace.heap_capacity;++rejections;
    request.reserved=1;require(dh2_nav_search(&result,&request,&workspace)==1&&unchanged(),"Invalid request mutated search output");request.reserved=0;++rejections;
    const auto saved=edges[0];edges[0].weight=-1;require(dh2_nav_search(&result,&request,&workspace)==1&&unchanged(),"Invalid weight mutated search output");edges[0]=saved;++rejections;
    const auto saved_order=order[0];order[0]=e+1;require(dh2_nav_search(&result,&request,&workspace)==1&&unchanged(),"Invalid edge order mutated search output");order[0]=saved_order;++rejections;
    const auto saved_offset=offsets[1];offsets[1]=e+1;require(dh2_nav_search(&result,&request,&workspace)==1&&unchanged(),"Invalid outgoing range mutated search output");offsets[1]=saved_offset;++rejections;
   }
  }
  require(r.at==r.bytes.size(),"Trailing search reference bytes");if(argc==4)require(authored==256,"Authored search cases missing");std::cout<<"{\"cases\":"<<cases<<",\"predicate_call_comparisons\":"<<calls<<",\"search_node_record_comparisons\":"<<records<<",\"successful_searches\":"<<found<<",\"authored_crypt_cases\":"<<authored<<",\"atomic_rejection_checks\":"<<rejections<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
