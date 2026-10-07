#include "../material_matrix_v4.hpp"
#include "material_matrix_v4_gold.hpp"
#include <cassert>
#include <iostream>
int main(){unsigned checks=0;for(const auto& gold:matrix_gold_v4){dh2::math::Matrix4f matrix;std::memcpy(matrix.m,gold.bits,64);matrix.identity_hint=gold.hint;assert(dh2::scene::material_matrix_is_identity_v4(matrix)==bool(gold.result));assert(matrix.identity_hint==gold.after);++checks;
 if(!gold.hint){dh2::scene::material_matrix_from_bres_v4(matrix,matrix.m);assert(matrix.identity_hint==gold.result);if(gold.result){for(unsigned i=0;i<16;++i)assert(matrix.m[i]==float(i%5==0));}else assert(!std::memcmp(matrix.m,gold.bits,64));const auto* bytes=reinterpret_cast<const unsigned char*>(&matrix);assert(bytes[65]==0&&bytes[66]==0&&bytes[67]==0);++checks;}}
 std::cout<<checks<<" original matrix cases/normalization checks PASS\n";}
