#include "collision.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using namespace dh2::collision;
namespace {
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){if(n>bytes.size()-at)throw std::runtime_error("Truncated collision reference");std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned v;read(&v,4);return v;}
};
bool equal(const void* a,const void* b,std::size_t size){
 for(std::size_t i=0;i<size;i+=4){unsigned x,y;std::memcpy(&x,static_cast<const unsigned char*>(a)+i,4);std::memcpy(&y,static_cast<const unsigned char*>(b)+i,4);
  const auto nan=[](unsigned v){return (v&0x7f800000)==0x7f800000&&(v&0x7fffff);};if(x!=y&&!(nan(x)&&nan(y)))return false;}return true;
}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;try{
  Reader r(argv[1]);if(r.word()!=0x314c4f43)return 3;const unsigned cases=r.word();if(cases>100000)return 4;unsigned counts[3]{},hits[3]{};
  for(unsigned i=0;i<cases;++i){
   const unsigned mode=r.word(),length=r.word();if(mode>2||length>4000000)return 5;const auto end=r.at+length;
   if(mode==0){
    Triangle t;float origin[3],direction[3],point[3];r.read(&t,36);r.read(origin,12);r.read(direction,12);if(r.at!=end)return 6;
    unsigned expected_hit=r.word();float expected[3];r.read(expected,12);std::memset(point,0xcc,12);const int hit=dh2_collision_line(point,&t,origin,direction);
    if(hit!=int(expected_hit)||!equal(point,expected,12)){std::cerr<<"Line collision mismatch "<<i<<'\n';return 7;}hits[mode]+=hit;
   }else{
    const unsigned count=r.word();if(count>100000)return 8;std::vector<Triangle> triangles(count);r.read(triangles.data(),36*count);Result result,expected;std::memset(&result,0xcc,sizeof(result));int hit;
    if(mode==1){Ray ray;r.read(&ray,24);hit=dh2_collision_raycast(&result,triangles.data(),count,&ray);}
    else{float point[3];r.read(point,12);Floor floor{triangles.data(),count,0,{},{}};r.read(floor.minimum,24);hit=dh2_collision_floor(&result,&floor,point);}
    if(r.at!=end)return 9;r.read(&expected,sizeof(expected));if(hit!=int(expected.hit)||!equal(&result,&expected,sizeof(result))){std::cerr<<"Collision mismatch "<<i<<" mode="<<mode<<'\n';return 10;}hits[mode]+=hit;
   }
   ++counts[mode];
  }
  if(r.at!=r.bytes.size())return 11;Result out;std::memset(&out,0xcc,sizeof(out));const Result saved=out;Ray ray{};
  if(dh2_collision_raycast(&out,nullptr,1,&ray)!=-1||std::memcmp(&out,&saved,sizeof(out)))return 12;
  std::cout<<"{\"comparisons\":"<<cases<<",\"cases\":{\"line\":"<<counts[0]<<",\"ray\":"<<counts[1]<<",\"floor\":"<<counts[2]<<"},\"hits\":{\"line\":"<<hits[0]<<",\"ray\":"<<hits[1]<<",\"floor\":"<<hits[2]<<"},\"atomic_rejection_checks\":1,\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 13;}
}
