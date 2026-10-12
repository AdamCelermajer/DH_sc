#include "../material_color.hpp"
#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>
namespace {
std::uint32_t w(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
float f(const std::uint8_t* p){float v;std::memcpy(&v,p,4);return v;}
}
// Fixed byte-format oracle adapter; no animation/material factory is supplied.
extern "C" int dh2_material_color_test(std::uint32_t op,const std::uint8_t* input,std::uint32_t size,std::uint8_t* output){
 if(!input||!output)return -99;
 if(op<3){
  if(size<36||size-36!=w(input))return -99;
  dh2::animation::ColorAccessor24 a{input+36,w(input),w(input+4),w(input+8)?input+32:nullptr};
  std::memcpy(output,input+28,4);
  if(op==0)return dh2_material_alpha_key(output,&a,w(input+12));
  if(op==1)return dh2_material_alpha_between(output,&a,w(input+12),w(input+16),f(input+24));
  return dh2_material_alpha_delta(output,&a,w(input+20),w(input+12),w(input+16),f(input+24));
 }
 if(op==3){
  if(size<8||w(input)>100000||size-8!=w(input)*8)return -99;
  const auto n=w(input);std::memcpy(output,input+4,4);
  std::vector<float> weights(n);if(n)std::memcpy(weights.data(),input+8+n*4,n*4);
  return dh2_material_color_blend(output,input+8,weights.data(),std::int32_t(n));
 }
 if(op==4){
  if(size!=40)return -99;dh2::animation::ColorParameter32 p;std::memcpy(&p,input,32);
  const int status=dh2_material_color_set(&p,w(input+32),input+36);std::memcpy(output,&p,32);return status;
 }
 if(op==5){
  if(size<36||w(input+32)>100000||size-36!=w(input+32)*8)return -99;
  dh2::animation::ColorParameter32 p;std::memcpy(&p,input,32);const auto n=w(input+32);std::vector<float> weights(n);if(n)std::memcpy(weights.data(),input+36+n*4,n*4);
  std::uint8_t color[4];int status=dh2_material_color_blend(color,input+36,weights.data(),std::int32_t(n));if(!status)status=dh2_material_color_set(&p,0,color);std::memcpy(output,&p,32);return status;
 }
 if(op==6||op==7){
  if(size<68||size-68!=w(input+32)||w(input+36)!=1||w(input+40)!=1)return -99;
  dh2::animation::ColorParameter32 p;std::memcpy(&p,input,32);const auto* ainput=input+32;
  dh2::animation::ColorAccessor24 a{ainput+36,w(ainput),1,ainput+32};std::uint8_t color[4];std::memcpy(color,ainput+28,4);
  int status=op==6?dh2_material_alpha_key(color,&a,w(ainput+12)):dh2_material_alpha_between(color,&a,w(ainput+12),w(ainput+16),f(ainput+24));
  if(!status)status=dh2_material_color_set(&p,0,color);std::memcpy(output,&p,32);return status;
 }
 return -99;
}
extern "C" int dh2_material_color_test_guards(){
 // Meaningful caller-contract checks: every rejected call preserves output.
 std::uint8_t output[4]{11,12,13,14},values[4]{0,1,2,3},def[4]{3,2,1,0};float weight=1;dh2::animation::ColorAccessor24 a{values,4,1,def};unsigned guards=0;
 bool pass=true;auto guard=[&](int status,int expected){pass=pass&&status==expected&&!std::memcmp(output,"\013\014\015\016",4);++guards;};
 guard(dh2_material_alpha_key(output,&a,4),-1);guard(dh2_material_alpha_between(output,&a,0,4,0.5f),-1);guard(dh2_material_alpha_delta(output,&a,4,0,1,0.5f),-1);
 a.has_default=2;guard(dh2_material_alpha_key(output,&a,0),-1);a.has_default=1;a.default_value=nullptr;guard(dh2_material_alpha_between(output,&a,0,1,0.5f),-2);guard(dh2_material_alpha_delta(output,&a,0,1,2,0.5f),-2);
 a.values=output;guard(dh2_material_alpha_key(output,&a,0),-1);a.values=values;a.count=0;guard(dh2_material_alpha_key(output,&a,0),-1);
 guard(dh2_material_color_blend(output,values,&weight,-1),-1);guard(dh2_material_color_blend(output,nullptr,&weight,1),-1);guard(dh2_material_color_blend(output,values,nullptr,1),-1);guard(dh2_material_color_blend(output,output,&weight,1),-1);
 dh2::animation::ColorParameter32 p{17,1,{1,2,3,4},5,6};const auto before=p;pass=pass&&dh2_material_color_set(&p,0,values)==-2&&!std::memcmp(&p,&before,32);++guards;return pass?int(guards):-1;
}
#ifndef DH2_MATERIAL_COLOR_ORACLE
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::ifstream file(argv[1],std::ios::binary);if(!file)throw std::runtime_error("gold file");
 auto read=[&](void* p,std::size_t n){if(!file.read(static_cast<char*>(p),n))throw std::runtime_error("truncated gold");};
 auto u=[&](){std::uint32_t v;read(&v,4);return v;};if(u()!=0x31434d46)throw std::runtime_error("gold magic");const auto count=u();
 for(unsigned i=0;i<count;++i){const auto op=u(),n=u();if(n>1000000)throw std::runtime_error("gold length");std::vector<std::uint8_t> input(n);read(input.data(),n);const auto expected=std::int32_t(u());const auto length=u();std::uint8_t out[32]{};std::vector<std::uint8_t> golden(length);read(golden.data(),length);const int status=dh2_material_color_test(op,input.data(),n,out);if(status!=expected||length>32||std::memcmp(out,golden.data(),length))throw std::runtime_error("gold mismatch "+std::to_string(i));}
 const int guards=dh2_material_color_test_guards();if(guards!=13)throw std::runtime_error("caller guards");
 std::cout<<"{\"validation\":\"PASS\",\"original_gold_cases\":"<<count<<",\"atomic_guards\":"<<guards<<",\"mismatches\":0}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
#endif
