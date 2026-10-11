#include "source_fx_node_matrix_v4.hpp"
#include "source_fx_billboard_basis_v1.hpp"
#include "visual_motion.hpp"
#include <algorithm>
#include <cstring>
#include <cmath>
namespace dh2::fx {namespace {
float mul(float a,float b){volatile float value=a*b;return value;}
float add(float a,float b){volatile float value=a+b;return value;}
}
void source_fx_matrix_multiply_v4(math::Matrix4f& out,const math::Matrix4f& left,const math::Matrix4f& right){
 const auto a=left,b=right;math::Matrix4f value;std::memset(&value,0,sizeof(value));
 if(a.identity_hint)std::memcpy(&value,&b,65);
 else if(b.identity_hint)std::memcpy(&value,&a,65);
 else {for(unsigned col=0;col<4;++col)for(unsigned row=0;row<4;++row){
  float sum=add(mul(a.m[row],b.m[col*4]),mul(a.m[4+row],b.m[col*4+1]));
  sum=add(sum,mul(a.m[8+row],b.m[col*4+2]));value.m[col*4+row]=add(sum,mul(a.m[12+row],b.m[col*4+3]));
 }value.identity_hint=0;}
 std::memcpy(&out,&value,sizeof(value));
}
void source_fx_trs_matrix_v4(math::Matrix4f& out,const float* position,const float* rotation,const float* scale){
 // AnimatedFX::SyncIrrData calls VisualObject::SetRotation472874, which
 // constructs Euler(Y,-X,-Z). Authored node quaternion tracks are already
 // in engine coordinates and must NOT receive this owner-boundary mapping.
 math::Quaternion q{};float quaternion[4]{};dh2_visual_rotation(quaternion,rotation);std::memcpy(&q,quaternion,16);math::Matrix4f value;std::memset(&value,0,sizeof(value));dh2_quat_matrix_transposed(&q,&value);
 for(unsigned col=0;col<3;++col){for(unsigned row=0;row<3;++row)value.m[col*4+row]=mul(value.m[col*4+row],scale[col]);value.m[12+col]=position[col];}std::memcpy(&out,&value,sizeof(value));
}
bool source_fx_node_world_matrix_v4(const scene::Scene& scene,std::uint32_t node,math::Matrix4f& out,std::string& error){
 if(node>=scene.graph.size()){error="Required same FX graph node";return false;}
 const auto& n=scene.graph[node];if(n.parent< -1||n.parent>=std::int32_t(node)){error="Required source FX graph parent order";return false;}
 for(float x:n.world)if(!std::isfinite(x)){error="Required finite source FX world matrix";return false;}
 math::Quaternion q{n.quaternion[0],n.quaternion[1],n.quaternion[2],n.quaternion[3]};math::Matrix4f local{};dh2_quat_matrix_transposed(&q,&local);
 math::Matrix4f value;std::memset(&value,0,sizeof(value));std::copy(n.world.begin(),n.world.end(),value.m);value.identity_hint=local.identity_hint;std::memcpy(&out,&value,sizeof(value));return true;
}
bool source_fx_rebuild_graph_world_v4(scene::Scene& scene,std::string& error){
 std::vector<math::Matrix4f> matrices;matrices.reserve(scene.graph.size());
 for(unsigned i=0;i<scene.graph.size();++i){const auto& n=scene.graph[i];
  if(n.parent< -1||n.parent>=std::int32_t(i)){error="Required source FX graph parent order";return false;}
  math::Quaternion q{n.quaternion[0],n.quaternion[1],n.quaternion[2],n.quaternion[3]};math::Matrix4f local;std::memset(&local,0,sizeof(local));dh2_quat_matrix_transposed(&q,&local);
  for(unsigned col=0;col<3;++col){for(unsigned row=0;row<3;++row)local.m[4*col+row]=mul(local.m[4*col+row],n.scale[col]);local.m[12+col]=n.translation[col];}
  math::Matrix4f world;std::memset(&world,0,sizeof(world));if(n.parent<0)std::memcpy(&world,&local,65);else source_fx_matrix_multiply_v4(world,matrices[unsigned(n.parent)],local);
  for(float x:world.m)if(!std::isfinite(x)){error="Required finite source FX graph matrix";return false;}matrices.push_back(world);
 }
 for(auto& instance:scene.instances)if(instance.node_index>=matrices.size()){error="Required same source mesh node";return false;}
 for(unsigned i=0;i<matrices.size();++i)std::copy_n(matrices[i].m,16,scene.graph[i].world.data());
 for(auto& instance:scene.instances)instance.world=scene.graph[instance.node_index].world;return true;
}
// ---- P16 LEVELUP2/3: glitch CBillboardSceneNode camera basis lives in source_fx_billboard_basis_v1.cpp
namespace {
using M16 = FxMatrix16;
M16 mul16(const M16& a,const M16& b){return source_fx_mat16_multiply_v1(a,b);}
bool inverse_affine16(const M16& m,M16& out){return source_fx_mat16_inverse_affine_v1(m,out);}
M16 trs16(const scene::Node& n){
 math::Quaternion q{n.quaternion[0],n.quaternion[1],n.quaternion[2],n.quaternion[3]};
 math::Matrix4f local{};std::memset(&local,0,sizeof(local));dh2_quat_matrix_transposed(&q,&local);
 M16 m{};std::copy_n(local.m,16,m.data());
 for(unsigned col=0;col<3;++col){for(unsigned row=0;row<3;++row)m[4*col+row]*=n.scale[col];m[12+col]=n.translation[col];}
 m[3]=m[7]=m[11]=0;m[15]=1;
 return m;
}
}

bool source_fx_scene_has_billboards_v1(const scene::Scene& scene) noexcept {
 for(const auto& n:scene.graph)if(n.source_storage_v91().billboard.present)return true;
 return false;
}
bool source_fx_rebuild_graph_world_billboards_v1(scene::Scene& scene,const math::Matrix4f& outer,
    const float view16[16],const float position3[3],std::string& error){
 error.clear();
 M16 view{};std::copy_n(view16,16,view.data());
 M16 outer16{};std::memcpy(outer16.data(),outer.m,sizeof(outer16));
 M16 outer_inverse{};
 if(!inverse_affine16(outer16,outer_inverse)){error="Required invertible FX outer transform for billboards";return false;}
 // The glitch camera absolute matrix is the world-to-eye view matrix: its row 1 (m1,m5,m9) is the
 // camera up vector, while the camera position is the separate absolute position (the eye). Both are
 // expressed in FX scene space (view * outer; eye through inverse outer).
 M16 camera_scene=mul16(view,outer16);
 const float eye_world[3]={position3[0],position3[1],position3[2]};
 for(unsigned r=0;r<3;++r)camera_scene[12+r]=outer_inverse[0+r]*eye_world[0]+outer_inverse[4+r]*eye_world[1]+outer_inverse[8+r]*eye_world[2]+outer_inverse[12+r];
 camera_scene[15]=1;
 std::vector<M16> matrices;matrices.reserve(scene.graph.size());
 for(unsigned i=0;i<scene.graph.size();++i){const auto& n=scene.graph[i];
  if(n.parent< -1||n.parent>=std::int32_t(i)){error="Required source FX graph parent order";return false;}
  const M16 local=trs16(n);
  const bool has_parent=n.parent>=0;
  const M16 parent_world=has_parent?matrices[unsigned(n.parent)]:M16{};
  M16 world{};
  const auto& bb=n.source_storage_v91().billboard;
  FxBillboardRecordV1 record{};
  record.mode=std::int32_t(bb.mode);record.sub=std::int32_t(bb.sub);
  std::copy_n(bb.axis_a.data(),3,record.axis_a);std::copy_n(bb.axis_b.data(),3,record.axis_b);
  const bool facing=bb.present&&source_fx_billboard_supported_v1(record)&&source_fx_billboard_absolute_v1(record,parent_world,has_parent,local,camera_scene,world);
  if(!facing)world=has_parent?mul16(parent_world,local):local;
  for(float x:world)if(!std::isfinite(x)){error="Required finite source FX billboard matrix";return false;}
  matrices.push_back(world);
 }
 for(auto& instance:scene.instances)if(instance.node_index>=matrices.size()){error="Required same source mesh node";return false;}
 for(unsigned i=0;i<matrices.size();++i){
  std::memcpy(scene.graph[i].world.data(),matrices[i].data(),sizeof(M16));
 }
 for(auto& instance:scene.instances)instance.world=scene.graph[instance.node_index].world;
 return true;
}
}
