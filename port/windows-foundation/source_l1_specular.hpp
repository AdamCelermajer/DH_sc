#pragma once
#include "renderer.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation {
// GL_Diffuse_L1_iPhone_VS: inverse-transpose world normal, no normalization.
inline bool source_l1_world_normal(const Mat4& m,Vec3 n,Vec3& out) noexcept {
    const Vec3 a{m[0],m[1],m[2]},b{m[4],m[5],m[6]},c{m[8],m[9],m[10]};
    const auto cross=[](Vec3 u,Vec3 v){return Vec3{u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x};};
    const auto bc=cross(b,c),ca=cross(c,a),ab=cross(a,b);
    const float determinant=a.x*bc.x+a.y*bc.y+a.z*bc.z;
    if(!std::isfinite(determinant)||determinant==0)return false;
    out={(bc.x*n.x+ca.x*n.y+ab.x*n.z)/determinant,
         (bc.y*n.x+ca.y*n.y+ab.y*n.z)/determinant,
         (bc.z*n.x+ca.z*n.y+ab.z*n.z)/determinant};
    return std::isfinite(out.x)&&std::isfinite(out.y)&&std::isfinite(out.z);
}
// Finite-domain CPU evaluation of original L1 SP varying. Undefined GLSL
// zero-length normalize/singular inputs reject, rather than inventing a light.
inline bool source_l1_specular(const LevelPointLight& light,Vec3 world,Vec3 normal,
                               Vec3 eye,float gloss,std::array<float,4>& out) noexcept {
    const auto dot=[](Vec3 a,Vec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;};
    const auto unit=[&](Vec3& v){const float s=dot(v,v);if(!std::isfinite(s)||s<=0)return false;const float k=1/std::sqrt(s);v={v.x*k,v.y*k,v.z*k};return true;};
    Vec3 l{light.position.x-world.x,light.position.y-world.y,light.position.z-world.z};
    const float d=std::sqrt(dot(l,l));
    const float denominator=light.attenuation[0]+light.attenuation[1]*d+light.attenuation[2]*d*d;
    if(!std::isfinite(denominator)||denominator<=0||!std::isfinite(gloss)||gloss<0||!unit(l))return false;
    const float ndotl=dot(normal,l);
    if(!std::isfinite(ndotl))return false;
    out={0,0,0,0};
    if(ndotl<=0)return true; // original nDotVP > 0 gate
    Vec3 e{eye.x-world.x,eye.y-world.y,eye.z-world.z};
    if(!unit(e))return false;
    Vec3 half{l.x+e.x,l.y+e.y,l.z+e.z};
    if(!unit(half))return false;
    const float factor=std::pow(std::max(0.f,dot(normal,half)),gloss)/denominator;
    for(unsigned i=0;i<3;++i){out[i]=light.specular[i]*factor;if(!std::isfinite(out[i]))return false;}
    return true;
}
}
