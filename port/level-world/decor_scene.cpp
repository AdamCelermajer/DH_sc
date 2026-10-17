#include "decor_scene.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>

namespace {
float literal(std::uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
}
extern "C" int dh2_decor_scene(dh2::physical::DecorSceneOutput* out,
 const dh2::physical::DecorSceneInput* in){
 if(!out||!in)return -1;
 dh2::physical::DecorSceneOutput result{};
 for(unsigned i=0;i<3;++i){
  result.effective_scale[i]=std::fabs(in->scale[i])<literal(0x38d1b717)?1.0f:in->scale[i];
  result.rotation_radians[i]=in->rotation_degrees[i]*literal(0x3c8efa35);
 }
 dh2::math::Quaternion q{};
 // Original ARM argument registers are r1=owner Y, r2=-owner X, r3=-owner Z.
 dh2_quat_from_euler(&q,result.rotation_radians[1],-result.rotation_radians[0],-result.rotation_radians[2]);
 // A new root starts with +0,+0,+0,1. SetRotation compares numerically and
 // preserves those stored zero signs when the calculated rotation is equal.
 if(q.x==0.0f&&q.y==0.0f&&q.z==0.0f&&q.w==1.0f)q={0,0,0,1};
 result.root_quaternion[0]=q.x;result.root_quaternion[1]=q.y;
 result.root_quaternion[2]=q.z;result.root_quaternion[3]=q.w;
 dh2_node_matrix(result.root_matrix,in->position,result.root_quaternion,result.effective_scale);
 dh2::physical::DecorMeshBoxInput box{};
 std::copy(in->marker_bounds,in->marker_bounds+6,box.bounds);
 std::copy(in->marker_parent_scale,in->marker_parent_scale+3,box.parent_scale);
 std::copy(result.root_matrix,result.root_matrix+16,box.node_matrix);
 if(dh2_decor_marker_mesh_box(result.mesh_box,&box)!=0)return -1;
 *out=result;return 0;
}
namespace dh2::physical {
bool decor_scene_marker(const resources::BresView& view,const scene::Scene& scene,
 DecorSceneMarker& out,std::string& error){
 error.clear();DecorSceneMarker result{};
 for(unsigned i=0;i<scene.graph.size();++i){
  const auto& node=scene.graph[i];
  if(node.parent< -1||node.parent>=static_cast<std::int32_t>(i)){error="Invalid decor scene parent order";return false;}
  if(node.name.compare(0,8,"_colbox_")!=0)continue;
  if(node.parent<0){error="Collision marker has no authored parent";return false;}
  for(const auto& child:scene.graph)if(child.parent==static_cast<std::int32_t>(i)){
   error="Collision marker child-group bounds are not reconstructed";return false;
  }
  const scene::Instance* mesh=nullptr;
  for(const auto& instance:scene.instances)if(instance.node_index==i){
   if(mesh||instance.controller>=0){error="Collision marker requires one static mesh";return false;}
   mesh=&instance;
  }
  if(!mesh){error="Collision marker mesh missing from complete scene";return false;}
  assets::Mesh bounds{};
  if(dh2_mesh_open(&bounds,&view,static_cast<std::int32_t>(mesh->geometry))!=assets::Error::ok){
   error="Invalid collision marker mesh";return false;
  }
  result.found=1;result.node_index=i;result.geometry=mesh->geometry;
  std::copy(bounds.minimum,bounds.minimum+3,result.bounds);
  std::copy(bounds.maximum,bounds.maximum+3,result.bounds+3);
  const auto& parent=scene.graph[node.parent];
  for(unsigned k=0;k<3;++k){
   if(!std::isfinite(parent.scale[k])){error="Invalid collision marker parent scale";return false;}
   result.parent_scale[k]=parent.scale[k];
  }
  out=result;return true;
 }
 out=result;return true;
}
bool decor_scene_marker(const resources::BresView& view,DecorSceneMarker& out,std::string& error){
 scene::Scene complete;
 if(!scene::load(view,complete,error))return false;
 return decor_scene_marker(view,complete,out,error);
}
}
