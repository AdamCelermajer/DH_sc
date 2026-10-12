#include "move_state.hpp"
#include <cmath>
#include <cstddef>
namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<an:x-y<bn;}
float multiplier(std::int32_t raw){float value=static_cast<float>(raw)*(1.f/256.f);value=value*.01f;value=value+1.f;return value>0.f?value:0.f;}
}
extern "C" int dh2_move_policy(dh2::move::Policy* out,const std::uint32_t* flags){
 if(!out||!flags||overlap(out,sizeof(*out),flags,4))return 1;
 const auto f=*flags;
 *out={f&1u,(f>>1)&1u,(f>>2)&1u,(f>>3)&1u,((f^0x10u)>>4)&1u,(f>>7)&1u,((f^0x40u)>>6)&1u};return 0;
}
extern "C" int dh2_move_speed(dh2::move::Speed* out,const std::int32_t* sheet,const float* speed){
 if(!out||!sheet||!speed||!std::isfinite(*speed)||*speed<=0.f||*speed>100.f||overlap(out,sizeof(*out),sheet,224*4)||overlap(out,sizeof(*out),speed,4))return 1;
 const auto walk=multiplier(sheet[dh2::move::walk_property]),rotation=multiplier(sheet[dh2::move::rotation_property]);*out={walk,rotation,walk*(*speed)};return 0;
}
extern "C" int dh2_move_rotation_speed(float* out,const std::uint32_t* flags,const std::int32_t* sheet){
 if(!out||!flags||!sheet||overlap(out,4,flags,4)||overlap(out,4,sheet,224*4))return 1;
 *out=(*flags&0x20u)?-1.f:multiplier(sheet[dh2::move::rotation_property]);return 0;
}
extern "C" int dh2_move_focus_begin(std::uint32_t* flags,std::uint32_t* type){if(!flags||!type||overlap(flags,4,type,4))return 1;*flags=0x23c1;*type=0;return 0;}
