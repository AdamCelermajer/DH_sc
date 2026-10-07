#pragma once
#include "../engine-math/math.hpp"
#include <cmath>
#include <cstring>
namespace dh2::scene {
// Source631c14 initializes65 bytes. Native hash storage additionally defines
// padding65..67 as zero; original constructors/copy65 leave it unspecified.
inline void material_matrix_identity_v4(math::Matrix4f& out) {
 std::memset(&out,0,sizeof(out));for(unsigned i=0;i<16;i+=5)out.m[i]=1.f;out.identity_hint=1;
}
// Whole cached CMatrix4<float>::isIdentity5ba19c. Preserve soft-float
// single-precision add/subtract and unordered comparisons.
inline bool material_matrix_is_identity_v4(math::Matrix4f& value) {
 if(value.identity_hint)return true;
 const std::uint32_t bits=0x358637bd;float epsilon;std::memcpy(&epsilon,&bits,4);
 for(unsigned i=0;i<16;++i){const float v=value.m[i];if(i%5==0){
  const float plus=v+epsilon,minus=v-epsilon;if(!(plus>=1.f)||!(minus<=1.f))return false;
 }else if(!(std::fabs(v)<=epsilon))return false;}
 value.identity_hint=1;return true;
}
// Whole BRES matrix default branch6326e8..6327b0 (material branch632080
// agrees): identity candidates do not install a parameter and retain exact
// default identity, while nonidentity parameters copy65 defined bytes.
inline void material_matrix_from_bres_v4(math::Matrix4f& out,const float* source16) {
 math::Matrix4f candidate;std::memset(&candidate,0,sizeof(candidate));
 std::memcpy(candidate.m,source16,64);candidate.identity_hint=0;
 if(material_matrix_is_identity_v4(candidate))material_matrix_identity_v4(out);
 else {std::memset(&out,0,sizeof(out));std::memcpy(&out,&candidate,65);}
}
}
