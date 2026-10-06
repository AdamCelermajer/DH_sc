#include "authored_fx_alignment_v39.hpp"
#include <cmath>
#include <cstring>
namespace dh2::fx {namespace {
bool finite(const math::Matrix4f& a){for(float f:a.m)if(!std::isfinite(f))return false;return true;}
math::Matrix4f identity(){math::Matrix4f m{};m.m[0]=m.m[5]=m.m[10]=m.m[15]=1;m.identity_hint=1;return m;}
}
bool authored_fx_draw_world_v39(math::Matrix4f& out,AuthoredPositionSpaceV39 space,const math::Matrix4f* owner,const math::Matrix4f* node,std::string& error){
 error.clear();math::Matrix4f value{};
 if(space==AuthoredPositionSpaceV39::particle_world){if(owner||node){error="World-space source particle vertices require identity draw transform";return false;}value=identity();}
 else {if(!owner||!finite(*owner)){error="Required actual finite source owner/socket transform";return false;}
  if(space==AuthoredPositionSpaceV39::owner_local_skinned){if(node){error="Source skin palette already contains authored node transform";return false;}std::memcpy(&value,owner,65);}
  else if(space==AuthoredPositionSpaceV39::node_local||space==AuthoredPositionSpaceV39::weapon_socket_local){if(!node||!finite(*node)){error="Required actual finite authored node/weapon local transform";return false;}source_fx_matrix_multiply_v4(value,*owner,*node);}
  else {error="Unknown source position-space contract";return false;}
 }
 if(!finite(value)){error="Source draw transform overflow";return false;}out=value;return true;
}
bool authored_fx_wvp_v39(math::Matrix4f& out,const math::Matrix4f& camera,const math::Matrix4f& world,std::string& error){
 error.clear();if(!finite(camera)||!finite(world)){error="Required actual finite camera/draw transform";return false;}
 math::Matrix4f value{};source_fx_matrix_multiply_v4(value,camera,world);if(!finite(value)){error="Source camera projection overflow";return false;}out=value;return true;
}
}
