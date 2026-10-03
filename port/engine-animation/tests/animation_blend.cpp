#include "../animation_blend.hpp"
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void check(bool value,const std::string& reason){if(!value)throw std::runtime_error(reason);}
std::uint32_t bits(float value){std::uint32_t word;std::memcpy(&word,&value,4);return word;}
float number(std::uint32_t word){float value;std::memcpy(&value,&word,4);return value;}
bool equal(std::uint32_t a,std::uint32_t b,bool copy=false){return a==b||(!copy&&std::isnan(number(a))&&std::isnan(number(b)));}
struct Reader {
 std::vector<char> bytes;std::size_t offset=0;
 explicit Reader(const char* path){std::ifstream file(path,std::ios::binary);check(bool(file),"Missing original corpus");bytes.assign(std::istreambuf_iterator<char>(file),{});}
 std::uint32_t word(){check(offset+4<=bytes.size(),"Truncated corpus");std::uint32_t result;std::memcpy(&result,bytes.data()+offset,4);offset+=4;return result;}
 std::vector<float> values(unsigned count){std::vector<float> result(count);for(auto& value:result)value=number(word());return result;}
};
using Blend=int(*)(float*,const float*,const float*,std::int32_t);
const std::array<Blend,4> kernels{dh2_animation_blend_scalar,dh2_animation_blend_vector3,dh2_animation_blend_vector3,dh2_animation_blend_quaternion};
const unsigned widths[]{1,3,3,4};
std::vector<std::array<std::uint32_t,3>> expected_libm;
std::size_t next_libm=0;
unsigned libm_calls=0;
float dependency(unsigned kind,float input){
 check(next_libm<expected_libm.size(),"Unexpected host libm call");
 const auto& expected=expected_libm[next_libm++];
 check(expected[0]==kind&&equal(expected[1],bits(input)),"Host libm argument/order differs from original fixture");
 ++libm_calls;return number(expected[2]);
}
}
// Test-only dependency fixtures. The production blend and math sources are
// unchanged. These verify every original libm input and return its audited
// UCRT result; host glibc rounding is not compared with Windows UCRT output.
extern "C" float __wrap_sinf(float input){return dependency(0,input);}
extern "C" float __wrap_acosf(float input){return dependency(1,input);}
extern "C" float __wrap_sqrtf(float input){return dependency(2,input);}

int main(int argc,char** argv){try{
 check(argc==2,"Usage: animation-blend-audit original-contribution-fixtures.bin");
 Reader reader(argv[1]);check(reader.word()==0x314b4241,"Wrong ABK1 magic");const auto count=reader.word();
 unsigned operations[4]{},copied=0;
 for(unsigned record=0;record<count;++record){
  const auto operation=reader.word(),slots=reader.word(),flags=reader.word(),trig_count=reader.word();
  check(operation<4&&slots<=65536&&flags<=7,"Malformed corpus record");const unsigned width=widths[operation];
  auto values=reader.values(slots*width),weights=reader.values(slots);
  const auto prior_values=values,prior_weights=weights;
  std::vector<std::uint32_t> expected(width);for(auto& value:expected)value=reader.word();
  expected_libm.clear();next_libm=0;
  for(unsigned i=0;i<trig_count;++i)expected_libm.push_back({reader.word(),reader.word(),reader.word()});
  float output[6];for(auto& value:output)value=number(0xaabbccdd);
  const float* weight_data=slots&&!(flags&2)?((flags&4)?values.data():weights.data()):nullptr;
  check(kernels[operation](output+1,slots?values.data():nullptr,weight_data,int(slots))==0,"Native blend returned rejection");
  for(unsigned i=0;i<width;++i)check(equal(expected[i],bits(output[i+1]),flags&1),"Original output word differs at record"+std::to_string(record)+" component"+std::to_string(i));
  check(bits(output[0])==0xaabbccdd&&bits(output[width+1])==0xaabbccdd,"Output guard changed");
  check(!slots||(!std::memcmp(values.data(),prior_values.data(),values.size()*4)&&!std::memcmp(weights.data(),prior_weights.data(),weights.size()*4)),"Read input changed");
  check(next_libm==expected_libm.size(),"Original libm fixture was not consumed");
  ++operations[operation];copied+=bool(flags&1);
 }
 check(reader.offset==reader.bytes.size(),"Unexpected corpus tail");
 unsigned rejects=0;
 for(unsigned operation:{0u,1u,3u}){
  alignas(16) float output[32],values[32],weights[32];
  struct Arguments{float* out;const float* values;const float* weights;int count;};
  const Arguments invalid[]{
   {nullptr,values,weights,2},{output,nullptr,weights,2},{output,values,nullptr,2},
   {output,values,weights,-1},{output,values,weights,65537},{values,values,weights,2},
   {weights,values,weights,2},{values+1,values,weights,2},
   {reinterpret_cast<float*>(reinterpret_cast<char*>(output)+1),values,weights,2},
   {output,reinterpret_cast<const float*>(reinterpret_cast<const char*>(values)+1),weights,2},
   {output,values,reinterpret_cast<const float*>(reinterpret_cast<const char*>(weights)+1),2},
   {output,reinterpret_cast<const float*>(std::uintptr_t(-4)),weights,2}};
  for(const auto& arguments:invalid){
   for(auto* buffer:{output,values,weights})for(unsigned i=0;i<32;++i)buffer[i]=number(0x12345678);
   check(kernels[operation](arguments.out,arguments.values,arguments.weights,arguments.count)==1,"Malformed binding accepted");
   for(auto* buffer:{output,values,weights})for(unsigned i=0;i<32;++i)check(bits(buffer[i])==0x12345678,"Malformed binding mutated memory");
   ++rejects;
  }
 }
 std::cout<<"{\"original_derived_replay\":"<<count<<",\"operation_counts\":["<<operations[0]<<","<<operations[1]<<","<<operations[2]<<","<<operations[3]<<"],\"copy_bit_exact_cases\":"<<copied<<",\"ordered_libm_fixture_calls\":"<<libm_calls<<",\"atomic_rejection_checks\":"<<rejects<<",\"mismatches\":0}\n";
 return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 1;}}
