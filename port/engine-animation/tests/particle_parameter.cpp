#include "../particle_parameter.hpp"
#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace {
std::uint32_t w(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
float f(const std::uint8_t* p){float v;std::memcpy(&v,p,4);return v;}
bool nan(std::uint32_t x){return (x&0x7f800000)==0x7f800000&&(x&0x7fffff);}
}
extern "C" int dh2_particle_parameter_test(std::uint32_t op,const std::uint8_t* input,std::uint32_t size,std::uint8_t* output){
 if(!input||!output)return -99;float result=0;int status;
 if(op<4){
  if(size<20||w(input)>100000||size-20!=w(input)*4)return -99;
  std::vector<float> values(w(input));if(!values.empty())std::memcpy(values.data(),input+20,values.size()*4);
  dh2::animation::ParticleAccessor16 a{values.data(),std::uint32_t(values.size()),0};
  if(op==0)status=dh2_particle_parameter_key(&result,&a,w(input+4));
  else if(op==1)status=dh2_particle_parameter_between(&result,&a,w(input+4),w(input+8),f(input+16));
  else if(op==2)status=dh2_particle_parameter_delta_key(&result,&a,w(input+4),w(input+8));
  else status=dh2_particle_parameter_delta_between(&result,&a,w(input+12),w(input+4),w(input+8),f(input+16));
 }else if(op==4){
  if(size<4)return -99;const auto count=std::int32_t(w(input));const auto n=count>0?std::uint32_t(count):0;
  if(n>100000||size-4!=n*8)return -99;std::vector<float> v(n),weights(n);
  if(n){std::memcpy(v.data(),input+4,n*4);std::memcpy(weights.data(),input+4+n*4,n*4);}
  status=dh2_particle_parameter_blend(&result,v.data(),weights.data(),count);
 }else if(op==5){if(size!=4)return -99;const float value=f(input);status=dh2_particle_parameter_apply(&result,&value);}
 else return -99;
 std::memcpy(output,&result,4);return status;
}
extern "C" int dh2_particle_parameter_test_guards(){
 float out=13,values[]{1,2,3},weights[]{1,2,3};dh2::animation::ParticleAccessor16 a{values,3,0};int count=0;bool good=true;
 auto guard=[&](int result){good=good&&result==-1&&out==13;++count;};
 guard(dh2_particle_parameter_key(&out,&a,3));guard(dh2_particle_parameter_between(&out,&a,0,3,1));
 guard(dh2_particle_parameter_delta_key(&out,&a,3,0));guard(dh2_particle_parameter_delta_between(&out,&a,3,0,1,1));
 a.reserved=1;guard(dh2_particle_parameter_key(&out,&a,0));a.reserved=0;
 a.count=0;guard(dh2_particle_parameter_key(&out,&a,0));a.count=3;a.values=nullptr;guard(dh2_particle_parameter_key(&out,&a,0));a.values=values;
 guard(dh2_particle_parameter_blend(&out,nullptr,weights,1));guard(dh2_particle_parameter_blend(&out,values,nullptr,1));
 a.values=&out;a.count=1;guard(dh2_particle_parameter_key(&out,&a,0));
 guard(dh2_particle_parameter_blend(&out,&out,weights,1));guard(dh2_particle_parameter_blend(&out,values,&out,1));
 guard(dh2_particle_parameter_apply(&out,nullptr));guard(dh2_particle_parameter_apply(&out,&out));
 guard(dh2_particle_parameter_key(&out,nullptr,0));guard(dh2_particle_parameter_between(&out,nullptr,0,1,1));
 return good?count:-1;
}
#ifndef DH2_PARTICLE_PARAMETER_ORACLE
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::ifstream stream(argv[1],std::ios::binary);if(!stream)throw std::runtime_error("gold file");
 auto read=[&](void* p,std::size_t n){if(!stream.read(static_cast<char*>(p),n))throw std::runtime_error("truncated gold");};
 auto u=[&](){std::uint32_t v;read(&v,4);return v;};if(u()!=0x31505046)throw std::runtime_error("gold magic");const auto cases=u();
 for(unsigned i=0;i<cases;++i){const auto op=u(),n=u();if(n>1000000)throw std::runtime_error("length");std::vector<std::uint8_t> input(n);read(input.data(),n);const auto expected=std::int32_t(u());const auto gold=u();std::uint8_t output[4];const int status=dh2_particle_parameter_test(op,input.data(),n,output);const auto actual=w(output);if(status!=expected||(gold!=actual&&!((op!=0&&op!=5)&&nan(gold)&&nan(actual))))throw std::runtime_error("gold mismatch "+std::to_string(i));}
 const int guards=dh2_particle_parameter_test_guards();if(guards!=16)throw std::runtime_error("guards");
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<cases<<",\"atomic_guards\":"<<guards<<",\"mismatches\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
