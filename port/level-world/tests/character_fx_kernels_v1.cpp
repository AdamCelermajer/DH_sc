#include "../character_fx_kernels_v1.hpp"
#include "../fx_texture_animation_v1.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
using namespace dh2;
struct Reader{std::ifstream f;explicit Reader(const char* p):f(p,std::ios::binary){if(!f)throw std::runtime_error(p);}template<class T>T get(){T v;f.read(reinterpret_cast<char*>(&v),sizeof(v));if(!f)throw std::runtime_error("gold truncated");return v;}};
struct Trace {std::uint32_t op,in,out;};static std::vector<Trace> trace;static unsigned used;
static float trig(unsigned op,float in){if(used>=trace.size())throw std::runtime_error("unexpected trig call");const auto& t=trace[used++];std::uint32_t bits;std::memcpy(&bits,&in,4);if(t.op!=op||bits!=t.in)throw std::runtime_error("trig input mismatch");float result;std::memcpy(&result,&t.out,4);return result;}
extern "C" float cosf(float in)noexcept{return trig(3,in);}extern "C" float sinf(float in)noexcept{return trig(0,in);}
extern "C" void sincosf(float in,float* sine,float* cosine)noexcept{*cosine=trig(3,in);*sine=trig(0,in);}
static std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error(p);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{if(argc!=3)throw std::runtime_error("gold assetdir required");Reader r(argv[1]);if(r.get<unsigned>()!=0x31475846)throw std::runtime_error("gold magic");auto count=r.get<unsigned>();std::vector<std::vector<std::uint8_t>> raws;std::vector<resources::BresView> views(3);for(unsigned i=0;i<3;++i)raws.push_back(read(std::string(argv[2])+"/swoosh_prince_1hand_combo_0"+std::to_string(i+1)+".bdae"));for(unsigned i=0;i<3;++i)if(dh2_bres_open(&views[i],raws[i].data(),raws[i].size())!=resources::BresError::ok)throw std::runtime_error("actual BRES");unsigned samplers=0,matrices=0;
 for(unsigned i=0;i<count;++i){auto op=r.get<unsigned>();int rc=0;bool same=false;
  if(op==0){auto type=r.get<std::int32_t>(),loop=r.get<std::int32_t>();auto step=r.get<fx::FxStep24V1>();auto expected=r.get<fx::FxData32V1>();fx::FxData32V1 actual;rc=dh2_fx_data_v1(&actual,&step,type,loop,expected.set_identity);same=!std::memcmp(&actual,&expected,sizeof(actual));}
  else if(op==1||op==2){auto n=r.get<unsigned>(),reference=0u;if(op==2)reference=r.get<unsigned>();auto key=r.get<unsigned>(),next=r.get<unsigned>();float fraction=r.get<float>();std::vector<float> values(n);for(auto& v:values)v=r.get<float>();auto expected=r.get<unsigned>();float actual;rc=op==1?dh2_fx_texture_between_v1(&actual,values.data(),n,key,next,fraction):dh2_fx_texture_delta_v1(&actual,values.data(),n,reference,key,next,fraction);same=!std::memcmp(&actual,&expected,4);}
  else if(op==3){auto v=r.get<fx::TextureTransform20V1>();auto expected=r.get<math::Matrix4f>();auto n=r.get<unsigned>();trace.clear();used=0;for(unsigned j=0;j<n;++j)trace.push_back(r.get<Trace>());math::Matrix4f actual{};rc=dh2_fx_texture_matrix_v1(&actual,&v);same=!std::memcmp(actual.m,expected.m,64)&&actual.identity_hint==expected.identity_hint&&used==n;++matrices;}
  else if(op==4){auto asset=r.get<unsigned>(),index=r.get<unsigned>();auto ms=r.get<std::int32_t>();auto interpolate=r.get<unsigned>();auto expected=r.get<fx::TextureTransform20V1>();assets::Animation a{};if(asset<1||asset>3||dh2_animation_open(&a,&views[asset-1],index,0)!=assets::Error::ok)throw std::runtime_error("gold accessor");fx::TextureTransform20V1 actual;std::string e;rc=fx::texture_sample_v1(actual,a,ms,interpolate,e)?0:-1;same=!std::memcmp(&actual,&expected,20);++samplers;}
  else throw std::runtime_error("gold opcode");if(rc||!same)throw std::runtime_error("FX gold mismatch "+std::to_string(i));
 }
 if(r.f.peek()!=EOF)throw std::runtime_error("gold trailing bytes");std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<count<<",\"shipping_sampler_cases\":"<<samplers<<",\"matrix_cases\":"<<matrices<<",\"sanitizer_findings\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
