#include "navigation_heading.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>
using dh2::navigation::HeadingState;
namespace {
void require(bool value,const char* message){if(!value)throw std::runtime_error(message);}
std::uint32_t word(float f){std::uint32_t u;std::memcpy(&u,&f,4);return u;}
std::uint32_t ordered(float f){const auto u=word(f);return u&0x80000000u?~u:u|0x80000000u;}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){require(n<=bytes.size()-at,"Truncated heading gold");std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned u;read(&u,4);return u;}
};
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31474448,"Invalid heading gold");const auto cases=r.word();require(cases&&cases<10000,"Heading gold budget");unsigned look=0,set=0,aliases=0,active=0,variants=0,max_ulp=0;
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned op=r.word(),rotate=r.word(),alias=r.word();r.word();require(op<=1&&rotate<=1&&alias<=1,"Invalid heading record");HeadingState state,expected;r.read(&state,24);float direction[3];r.read(direction,12);r.read(&expected,24);const auto* input=alias?state.direction:direction;
   require((op?dh2_nav_set_heading(&state,input,rotate):dh2_nav_look_towards(&state.angle,input))==0,"Heading rejected original input");
   require(!std::memcmp(state.direction,expected.direction,12)&&state.active==expected.active&&state.reserved==expected.reserved,"Heading arithmetic/state differs from original gold");
   if(word(state.angle)!=word(expected.angle)){
    require(state.angle!=0&&expected.angle!=0,"Heading zero sign mismatch");const auto a=ordered(state.angle),b=ordered(expected.angle);const unsigned delta=a>b?a-b:b-a;
    require(delta<=2,"Host libm heading differs by more than two ULP");max_ulp=std::max(max_ulp,delta);++variants;
   }
   look+=op==0;set+=op==1;aliases+=alias;active+=state.active;
  }
  require(r.at==r.bytes.size(),"Trailing heading gold");unsigned rejections=0;
  HeadingState state{{1,2,3},.37f,1,0};float direction[3]{4,5,6};
  auto reject=[&]{const auto before=state;require(dh2_nav_set_heading(&state,direction,1)==1&&!std::memcmp(&state,&before,24),"Heading failure mutated state");++rejections;};
  state.reserved=1;reject();state.reserved=0;direction[0]=std::numeric_limits<float>::quiet_NaN();reject();direction[0]=std::numeric_limits<float>::max();reject();direction[0]=4;
  const auto before=state;require(dh2_nav_set_heading(&state,direction,2)==1&&!std::memcmp(&state,&before,24),"Heading bool failure mutated state");++rejections;
  state.angle=std::numeric_limits<float>::infinity();reject();state.angle=.37f;
  float angle=.37f;direction[1]=std::numeric_limits<float>::infinity();require(dh2_nav_look_towards(&angle,direction)==1&&angle==.37f,"Look failure mutated angle");++rejections;
  std::cout<<"{\"cases\":"<<cases<<",\"look\":"<<look<<",\"set_heading\":"<<set<<",\"self_alias\":"<<aliases<<",\"active\":"<<active<<",\"host_libm_angle_variants\":"<<variants<<",\"maximum_host_angle_ulp\":"<<max_ulp<<",\"angle_tolerance_ulp\":2,\"atomic_rejection_checks\":"<<rejections<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
