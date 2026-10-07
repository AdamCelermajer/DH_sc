#include "swf_input_geometry.hpp"
#include <cstddef>
#include <cstring>
#include <limits>
namespace {
float mul(float a,float b){volatile float x=a*b;return x;}
float add(float a,float b){volatile float x=a+b;return x;}
float sub(float a,float b){volatile float x=a-b;return x;}
float div(float a,float b){volatile float x=a/b;return x;}
float finite(float a){return a>=-std::numeric_limits<float>::max()&&a<=std::numeric_limits<float>::max()?a:0.f;}
float sign(float a){std::uint32_t bits;std::memcpy(&bits,&a,4);bits^=0x80000000;std::memcpy(&a,&bits,4);return a;}
bool overlap(const void*a,std::size_t n,const void*b,std::size_t m){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return x<=y?y-x<n:x-y<m;}
void inverse(float*out,const float*m){const float determinant=sub(mul(m[4],m[0]),mul(m[1],m[3]));
 if(determinant==0.f){out[0]=out[4]=1.f;out[1]=out[3]=0.f;out[2]=finite(sign(m[2]));out[5]=finite(sign(m[5]));return;}
 const float reciprocal=div(1.f,determinant);out[0]=finite(mul(reciprocal,m[4]));out[4]=finite(mul(reciprocal,m[0]));out[1]=finite(mul(sign(m[1]),reciprocal));out[3]=finite(mul(sign(m[3]),reciprocal));
 out[2]=finite(sign(add(mul(out[0],m[2]),mul(out[1],m[5]))));out[5]=finite(sign(add(mul(out[3],m[2]),mul(out[4],m[5]))));
}
}
extern "C" int dh2_ui_swf_inverse(float*out,const float*m){if(!out||!m||overlap(out,24,m,24))return -1;inverse(out,m);return 0;}
extern "C" int dh2_ui_swf_inverse_point(float*out,const float*m,const float*p){if(!out||!m||!p||overlap(out,8,m,24)||overlap(out,8,p,8))return -1;float inv[6];inverse(inv,m);out[0]=add(add(mul(inv[0],p[0]),mul(inv[1],p[1])),inv[2]);out[1]=add(add(mul(inv[3],p[0]),mul(inv[4],p[1])),inv[5]);return 0;}
