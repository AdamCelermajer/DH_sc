#include "character_fx_kernels_v1.hpp"
#include <cmath>
#include <cstring>
namespace {
float add(float a,float b){volatile float x=a+b;return x;}
float sub(float a,float b){volatile float x=a-b;return x;}
float mul(float a,float b){volatile float x=a*b;return x;}
float div(float a,float b){volatile float x=a/b;return x;}
float bits(std::uint32_t u){float f;std::memcpy(&f,&u,4);return f;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return a&&b&&(x<=y?y-x<an:x-y<bn);}
std::int32_t product(std::int32_t a,std::int32_t b){auto u=std::uint32_t(a)*std::uint32_t(b);std::int32_t r;std::memcpy(&r,&u,4);return r;}
bool scalar(float* out,const float* v,std::uint32_t n){return out&&v&&n&&n<=1048576&&!overlap(out,4,v,std::size_t(n)*4);}
}
extern "C" int dh2_fx_data_v1(dh2::fx::FxData32V1* out,const dh2::fx::FxStep24V1* s,std::int32_t type,std::int32_t loop,std::uintptr_t id){
 if(!out||!s||s->orient_once>255||s->orient_with_anchor>255||s->scale_with_anchor>255||overlap(out,32,s,24))return -1;
 auto effective=s->loop;
 if(type!=1){if(s->loop==-1||loop==-1)effective=-1;else if(s->loop==0)effective=loop;else if(loop!=0)effective=product(s->loop,loop);}
 *out={s->orient_with_anchor,s->orient_once,s->scale_with_anchor,s->speed,effective,s->play_time,id};return 0;
}
extern "C" int dh2_fx_texture_key_v1(float* out,const float* v,std::uint32_t n,std::uint32_t key){if(!scalar(out,v,n)||key>=n)return -1;std::memcpy(out,v+key,4);return 0;}
extern "C" int dh2_fx_texture_between_v1(float* out,const float* v,std::uint32_t n,std::uint32_t key,std::uint32_t next,float t){if(!scalar(out,v,n)||key>=n||next>=n)return -1;*out=add(v[key],mul(t,sub(v[next],v[key])));return 0;}
extern "C" int dh2_fx_texture_delta_v1(float* out,const float* v,std::uint32_t n,std::uint32_t reference,std::uint32_t key,std::uint32_t next,float t){if(!scalar(out,v,n)||reference>=n||key>=n||next>=n)return -1;*out=sub(add(v[key],mul(t,sub(v[next],v[key]))),v[reference]);return 0;}
extern "C" int dh2_fx_texture_matrix_v1(dh2::math::Matrix4f* out,const dh2::fx::TextureTransform20V1* in){
 if(!out||!in||overlap(out,sizeof(*out),in,sizeof(*in)))return -1;
 auto a=mul(div(in->rotation_degrees,180.f),bits(0x40490fe9));float c=::cosf(a),s=::sinf(a);
 dh2::math::Matrix4f m{};m.m[0]=mul(in->scale_u,c);m.m[1]=mul(in->scale_v,s);
 std::uint32_t u;std::memcpy(&u,&s,4);u^=0x80000000u;m.m[4]=mul(bits(u),in->scale_u);m.m[5]=mul(in->scale_v,c);
 m.m[8]=add(mul(add(sub(.5f,mul(c,.5f)),mul(s,.5f)),in->scale_u),in->offset_u);
 m.m[9]=add(mul(sub(sub(.5f,mul(s,.5f)),mul(c,.5f)),in->scale_v),in->offset_v);
 m.m[10]=m.m[15]=1.f;m.identity_hint=0;*out=m;return 0;
}
