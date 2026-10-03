#include "../animation_blender.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
using dh2::animation::BlenderState;
struct Record{std::uint32_t op,arg;BlenderState before,expected;};
static_assert(sizeof(Record)==72);
int apply(BlenderState* s,unsigned op,std::uint32_t arg){
 if(op==0){std::int32_t value;std::memcpy(&value,&arg,4);return dh2_blender_begin(s,value);}
 if(op==2)return dh2_blender_normalize(s);
 return dh2_blender_update_weights(s,arg);
}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream f(argv[1],std::ios::binary);assert(f);std::uint32_t header[2];f.read(reinterpret_cast<char*>(header),8);assert(header[0]==0x31414c42);
 for(unsigned i=0;i<header[1];++i){Record r;f.read(reinterpret_cast<char*>(&r),sizeof r);assert(f);auto actual=r.before;assert(apply(&actual,r.op,r.arg)==0);std::uint32_t a[8],b[8];std::memcpy(a,&actual,32);std::memcpy(b,&r.expected,32);
  for(unsigned j=0;j<8;++j){if(a[j]==b[j])continue;float x,y;std::memcpy(&x,a+j,4);std::memcpy(&y,b+j,4);assert((j==4||j==6||j==7)&&std::isnan(x)&&std::isnan(y));}
 }
 unsigned rejects=0;
 for(unsigned op=0;op<3;++op){
  assert(apply(nullptr,op,16)==1);++rejects;
  for(auto field:{0u,1u,4u,6u,7u}){BlenderState s{0,1,100,100,.01f,0,{1,0}};std::uint32_t w[8];std::memcpy(w,&s,32);w[field]=field<2?2:0x7f800000;std::memcpy(&s,w,32);auto before=s;assert(apply(&s,op,16)==1);assert(std::memcmp(&s,&before,32)==0);++rejects;}
 }
 std::printf("{\"original_derived_replay\":%u,\"atomic_rejection_checks\":%u,\"mismatches\":0}\n",header[1],rejects);
}
