#include "../angle_interpreter.hpp"
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
namespace {
void check(bool b,const char* message){if(!b)throw std::runtime_error(message);}
struct Reader {std::vector<unsigned char> bytes;std::size_t at=0;explicit Reader(const char* name){std::ifstream f(name,std::ios::binary);check(bool(f),"Missing angle gold");bytes={std::istreambuf_iterator<char>(f),{}};}template<class T>T get(){check(at+sizeof(T)<=bytes.size(),"Truncated angle gold");T result;std::memcpy(&result,bytes.data()+at,sizeof(T));at+=sizeof(T);return result;}};
bool same(unsigned a,unsigned b){return a==b||((a&0x7f800000)==0x7f800000&&(a&0x7fffff)&&(b&0x7f800000)==0x7f800000&&(b&0x7fffff));}
float floating(unsigned word){float result;std::memcpy(&result,&word,4);return result;}
unsigned bits(float v){unsigned result;std::memcpy(&result,&v,4);return result;}
std::vector<std::array<unsigned,3>> trace;std::size_t called=0;
float libm(unsigned type,float input){check(called<trace.size(),"Unexpected libm call");const auto& row=trace[called++];check(row[0]==type&&same(row[1],bits(input)),"Ordered libm input differed");return floating(row[2]);}
}
extern "C" float sinf(float x) noexcept{return libm(0,x);}
extern "C" float cosf(float x) noexcept{return libm(3,x);}
extern "C" void sincosf(float x,float* sine,float* cosine) noexcept{*sine=libm(0,x);*cosine=libm(3,x);}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;
 Reader gold(argv[1]);check(gold.get<unsigned>()==0x31474e41,"ANG1 magic differs");const auto cases=gold.get<unsigned>();unsigned calls=0;
 for(unsigned i=0;i<cases;++i){
  const auto operation=gold.get<unsigned>(),key=gold.get<unsigned>(),next=gold.get<unsigned>(),fraction=gold.get<unsigned>(),count=gold.get<unsigned>();std::vector<float> values(count);for(auto& value:values)value=floating(gold.get<unsigned>());std::array<float,4> defaults;for(auto& value:defaults)value=floating(gold.get<unsigned>());const auto expected=gold.get<std::array<unsigned,4>>();const auto n=gold.get<unsigned>();trace.resize(n);for(auto& row:trace)row=gold.get<std::array<unsigned,3>>();called=0;
  dh2::animation::AngleAccessor24 accessor{values.data(),defaults.data(),count,0};dh2::math::Quaternion out{};const auto status=operation?dh2_animation_angle_between(&out,&accessor,key,next,floating(fraction)):dh2_animation_angle_key(&out,&accessor,key);check(status==0,"Native angle rejected");std::array<unsigned,4> actual;std::memcpy(actual.data(),&out,16);for(unsigned j=0;j<4;++j)if(!same(actual[j],expected[j])){std::cerr<<"Angle case "<<i<<" word "<<j<<" differs\n";return 3;}check(called==n,"Missing ordered libm call");calls+=n;
 }
 check(gold.at==gold.bytes.size(),"Trailing angle gold");trace.clear();called=0;float values[2]{},defaults[4]{};dh2::math::Quaternion out{1,2,3,4};dh2::animation::AngleAccessor24 base{values,defaults,2,0};unsigned guards=0;
 auto reject=[&](dh2::math::Quaternion* output,const dh2::animation::AngleAccessor24* accessor,unsigned key,unsigned next,int wanted=-1){const auto before=out;check(dh2_animation_angle_between(output,accessor,key,next,.5f)==wanted,"Angle native guard accepted");check(!std::memcmp(&out,&before,16)&&!called,"Rejected angle had effect");++guards;};
 reject(nullptr,&base,0,1);reject(&out,nullptr,0,1);reject(&out,reinterpret_cast<const dh2::animation::AngleAccessor24*>(reinterpret_cast<const char*>(&base)+1),0,1);reject(reinterpret_cast<dh2::math::Quaternion*>(reinterpret_cast<char*>(&out)+1),&base,0,1);reject(reinterpret_cast<dh2::math::Quaternion*>(&base),&base,0,1);reject(reinterpret_cast<dh2::math::Quaternion*>(values),&base,0,1);reject(reinterpret_cast<dh2::math::Quaternion*>(defaults),&base,0,1);reject(&out,&base,2,1);reject(&out,&base,0,2);
 for(unsigned i=0;i<5;++i){auto bad=base;switch(i){case 0:bad.values=nullptr;break;case 1:bad.values=reinterpret_cast<const float*>(reinterpret_cast<const char*>(values)+1);break;case 2:bad.default_value=reinterpret_cast<const float*>(reinterpret_cast<const char*>(defaults)+1);break;case 3:bad.count=0;break;case 4:bad.reserved=1;break;}reject(&out,&bad,0,1);}auto missing=base;missing.default_value=nullptr;reject(&out,&missing,0,1,-2);
 std::cout<<"{\"validation\":\"PASS\",\"comparisons\":"<<cases<<",\"ordered_libm_calls\":"<<calls<<",\"atomic_rejection_checks\":"<<guards<<",\"mismatches\":0}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
