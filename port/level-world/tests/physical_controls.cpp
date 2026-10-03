#include "physical_controls.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
using dh2::physical::BodyState;
using dh2::physical::TransformRequest;
namespace {
void require(bool v,const char* s){if(!v)throw std::runtime_error(s);}
struct Reader {
 std::vector<unsigned char> bytes;std::size_t at=0;
 explicit Reader(const char* path){std::ifstream f(path,std::ios::binary);bytes={std::istreambuf_iterator<char>(f),{}};}
 void read(void* out,std::size_t n){require(at<=bytes.size()&&n<=bytes.size()-at,"Truncated physical gold");std::memcpy(out,bytes.data()+at,n);at+=n;}
 unsigned word(){unsigned u;read(&u,4);return u;}
};
bool nan(std::uint32_t u){return (u&0x7f800000u)==0x7f800000u&&(u&0x007fffffu);}
unsigned equal(const void* expected,const void* actual,std::size_t bytes){
 unsigned variants=0;
 for(std::size_t at=0;at<bytes;at+=4){std::uint32_t e,a;std::memcpy(&e,static_cast<const char*>(expected)+at,4);std::memcpy(&a,static_cast<const char*>(actual)+at,4);require(e==a||(nan(e)&&nan(a)),"Physical state differs from original gold");variants+=e!=a;}
 return variants;
}
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 try{
  Reader r(argv[1]);require(r.word()==0x31434850,"Invalid physical gold");const auto cases=r.word();require(cases&&cases<100000,"Physical gold budget");unsigned totals[6]{},nan_variants=0,transforms=0,stop_midpoints=0;
  for(unsigned ci=0;ci<cases;++ci){
   const unsigned op=r.word(),present=r.word(),movable=r.word(),calls=r.word();require(op<6&&present<=1&&movable<=1&&calls<=1,"Invalid physical record");++totals[op];
   float args[4];BodyState state,expected,mid;TransformRequest out{},expected_out;
   r.read(args,16);r.read(&state,48);r.read(&expected,48);r.read(&expected_out,16);r.read(&mid,48);
   int code=0;
   switch(op){
    case 0:code=dh2_physical_set_linear(&state,args);require(!std::memcmp(state.linear_velocity,args,8),"Linear setter lost input bits");break;
    case 1:code=dh2_physical_add_linear(&state,args);break;
    case 2:code=dh2_physical_set_angular(&state,args);require(!std::memcmp(&state.angular_velocity,args,4),"Angular setter lost input bits");break;
    case 3:{float values[4];code=dh2_physical_query(values,&state);std::memcpy(&out,values,16);break;}
    case 4:code=dh2_physical_request_position(&out,&state,args);break;
    case 5:
     if(present&&movable){code=dh2_physical_stop_begin(&out,&state,args);nan_variants+=equal(&mid,&state,48);++stop_midpoints;require(code==0,"Stop begin rejected original");state.position[0]=out.position[0];state.position[1]=out.position[1];code=dh2_physical_stop_finish(&state);}
     break;
   }
   require(code==0,"Physical helper rejected original input");nan_variants+=equal(&expected,&state,48);nan_variants+=equal(&expected_out,&out,16);transforms+=calls;
  }
  require(r.at==r.bytes.size(),"Trailing physical gold");unsigned rejected=0;BodyState s{};TransformRequest out{};float args[4]{1,2,3,4};s.flags=0x10000;
  auto reject=[&](int code,const BodyState& before,const TransformRequest& previous){require(code==1&&!std::memcmp(&s,&before,48)&&!std::memcmp(&out,&previous,16),"Malformed physical caller mutated state");++rejected;};
  const auto before=s;const auto previous=out;
  reject(dh2_physical_set_linear(&s,args),before,previous);reject(dh2_physical_add_linear(&s,args),before,previous);reject(dh2_physical_set_angular(&s,args),before,previous);reject(dh2_physical_wake(&s),before,previous);
  float query[4]{7,8,9,10};reject(dh2_physical_query(query,&s),before,previous);require(query[0]==7&&query[3]==10,"Query rejection mutated output");
  reject(dh2_physical_request_position(&out,&s,args),before,previous);reject(dh2_physical_stop_begin(&out,&s,args),before,previous);reject(dh2_physical_stop_finish(&s),before,previous);
  s.flags=8;const auto valid=s;
  reject(dh2_physical_set_linear(&s,nullptr),valid,previous);reject(dh2_physical_add_linear(&s,nullptr),valid,previous);reject(dh2_physical_set_angular(&s,nullptr),valid,previous);
  reject(dh2_physical_query(reinterpret_cast<float*>(&s),&s),valid,previous);reject(dh2_physical_request_position(reinterpret_cast<TransformRequest*>(&s),&s,args),valid,previous);reject(dh2_physical_stop_begin(reinterpret_cast<TransformRequest*>(&s),&s,args),valid,previous);
  std::cout<<"{\"cases\":"<<cases<<",\"transform_argument_checks\":"<<transforms<<",\"stop_intermediate_state_checks\":"<<stop_midpoints<<",\"generated_nan_field_variants\":"<<nan_variants<<",\"atomic_rejection_checks\":"<<rejected<<",\"mismatches\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}
}
