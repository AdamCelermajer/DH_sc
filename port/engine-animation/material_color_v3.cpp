#include "material_color_v3.hpp"
#include <cstring>
namespace {
std::uint32_t source_unsigned(float f){std::uint32_t w;std::memcpy(&w,&f,4);const auto e=(w>>23)&255;if((w>>31)||e<127)return 0;if(e>158)return e==255&&(w&0x7fffff)?0:UINT32_MAX;const auto m=(w&0x7fffff)|0x800000;return e>=150?m<<(e-150):m>>(150-e);}
}
extern "C" int dh2_material_color_between_v3(std::uint8_t out[4],const dh2::animation::MaterialColorAccessorV3* a,std::uint32_t key,std::uint32_t next,float fraction){
 if(!out||!a||!a->values||!a->count||key>=a->count||next>=a->count)return -1;
 const auto p=reinterpret_cast<std::uintptr_t>(a->values),q=reinterpret_cast<std::uintptr_t>(out);const auto n=std::size_t(a->count)*4;
 if(n>UINTPTR_MAX-p||(p<=q?q-p<n:p-q<4))return -1;
 std::uint8_t result[4];
 // Original6268b0 reads adjacent keys and accumulates two source-weighted
 // products from zero. It is not first+fraction*(last-first).
 if(next!=key+1)return -1;volatile float inverse=1.f-fraction;
 for(unsigned j=0;j<4;++j){volatile float first=float(a->values[std::size_t(key)*4+j])*inverse;volatile float sum=first+0.f;volatile float last=float(a->values[std::size_t(next)*4+j])*fraction;volatile float value=last+sum;result[j]=std::uint8_t(source_unsigned(value));}
 std::memcpy(out,result,4);return 0;
}
