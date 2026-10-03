#include "navigation_path.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::navigation;
namespace {
void require(bool v,const char* message){if(!v)throw std::runtime_error(message);}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){require(n<=bytes.size()-at,"Truncated path gold");if(n)std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
 std::vector<unsigned char> block(unsigned n){require(n<10000000,"Path gold budget exceeded");std::vector<unsigned char> v(n);read(v.data(),n);return v;}
};
std::vector<unsigned char> snapshot(const PathObject& p){
 std::vector<unsigned char> result(76+p.count*48);std::memcpy(result.data(),&p,68);std::memcpy(result.data()+68,&p.count,4);std::memcpy(result.data()+72,&p.owned,4);if(p.count)std::memcpy(result.data()+76,p.segments,p.count*48);return result;
}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31564e50,"Invalid path gold");const auto line_count=r.word(),steps=r.word(),n=r.word(),e=r.word();require(n==335&&e==838&&line_count<10000&&steps<100000,"Invalid path budgets");std::vector<Node> nodes(n);std::vector<Edge> edges(e);r.read(nodes.data(),n*56);r.read(edges.data(),e*20);Graph graph{};graph.nodes=nodes.data();graph.edges=edges.data();graph.node_count=n;graph.edge_count=e;
  unsigned kinds[6]{},ops[7]{};for(unsigned i=0;i<line_count;++i){float points[8];LineIntersection actual{};r.read(points,32);r.read(&actual,16);const auto expected=r.block(20);require(dh2_nav_line_intersection(&actual,points,points+2,points+4,points+6)==0&&!std::memcmp(&actual,expected.data(),20),"Line intersection mismatch");require(actual.kind<6,"Invalid line kind");++kinds[actual.kind];}
  std::vector<PathSegment> segments(1024);unsigned rejections=0;
  for(unsigned i=0;i<steps;++i){const unsigned op=r.word(),before_size=r.word(),after_size=r.word(),output_size=r.word();require(op&&op<=6&&before_size>=76&&after_size>=76&&output_size<=20,"Invalid path operation");const auto before=r.block(before_size),expected=r.block(after_size),output=r.block(output_size);PathObject object{};std::memcpy(&object,before.data(),68);std::memcpy(&object.count,before.data()+68,4);std::memcpy(&object.owned,before.data()+72,4);object.segments=segments.data();object.capacity=segments.size();require(before_size==76+object.count*48&&object.count<=segments.size(),"Invalid path input");if(object.count)std::memcpy(segments.data(),before.data()+76,object.count*48);
   MoveResult result{};unsigned past=0;float length=0;int status=0;const void* bytes=nullptr;
   switch(op){case 1:status=dh2_nav_smooth_path(&object,&graph);break;case 2:status=dh2_nav_calc_waypoint(&object);break;case 3:status=dh2_nav_past_waypoint(&past,&object);bytes=&past;break;case 4:status=dh2_nav_drop_path(&object);break;case 5:status=dh2_nav_path_length(&length,&object);bytes=&length;break;case 6:status=dh2_nav_move_path(&result,&object,&graph);bytes=&result;break;}
   const auto actual=snapshot(object);require(status==0&&actual==expected&&(!output_size||!std::memcmp(bytes,output.data(),output_size)),"Native path operation mismatch");++ops[op];
   if(!rejections&&object.count){
    const auto saved=snapshot(object);const auto saved_result=result;
    object.reserved=1;require(dh2_nav_move_path(&result,&object,&graph)==1&&snapshot(object)==saved&&!std::memcmp(&result,&saved_result,sizeof(result)),"Reserved rejection mutated path");object.reserved=0;++rejections;
    object.owned=object.count+1;const auto invalid=snapshot(object);require(dh2_nav_drop_path(&object)==1&&snapshot(object)==invalid,"Owned-count rejection mutated path");object.owned=0;std::memcpy(&object.owned,saved.data()+72,4);++rejections;
    result.reserved=1;require(dh2_nav_move_path(&result,&object,&graph)==1&&snapshot(object)==saved,"Output rejection mutated path");result.reserved=0;++rejections;
   }
  }
  require(r.at==r.bytes.size(),"Trailing path gold");std::cout<<"{\"line_comparisons\":"<<line_count<<",\"path_operation_comparisons\":"<<steps<<",\"atomic_rejection_checks\":"<<rejections<<",\"line_classifications\":[";for(unsigned i=0;i<6;++i)std::cout<<(i?",":"")<<kinds[i];std::cout<<"],\"operation_counts\":[";for(unsigned i=0;i<7;++i)std::cout<<(i?",":"")<<ops[i];std::cout<<"],\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
