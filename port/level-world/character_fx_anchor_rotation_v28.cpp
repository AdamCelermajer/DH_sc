#include "character_fx_anchor_rotation_v28.hpp"
#include "../engine-math/math.hpp"
#include "../scene-materials/scene.hpp"
#include <cstring>
#include <cmath>
namespace dh2::fx {
bool character_fx_anchor_rotation_v28(float* out,const visual::Root& root,std::string& error){
 if(!out){error="Required source FX anchor rotation output";return false;}
 math::Matrix4f matrix{};dh2_node_matrix(matrix.m,root.position,root.quaternion,root.scale);
 for(float f:matrix.m)if(!std::isfinite(f)){error="Required finite SAME anchor CScene root matrix";return false;}
 math::Vector3f degrees{};dh2_matrix_rotation_degrees(&degrees,&matrix);
 std::uint32_t raw=0x3c8efa35;float radians_per_degree;std::memcpy(&radians_per_degree,&raw,4);
 const float values[]{degrees.x,degrees.y,degrees.z};float result[3];
 for(unsigned i=0;i<3;++i){volatile float product=values[i]*radians_per_degree;result[i]=product;}
 if(!std::isfinite(result[0])||!std::isfinite(result[1])||!std::isfinite(result[2])){error="Required valid original anchor rotation-degrees domain";return false;}
 std::memcpy(out,result,12);return true;
}
}
