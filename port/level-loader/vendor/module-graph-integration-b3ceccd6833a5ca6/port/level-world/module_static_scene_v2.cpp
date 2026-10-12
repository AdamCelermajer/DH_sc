#include "module_static_scene_v2.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace dh2::world {
bool ModuleStaticSceneV2::initialize(ModuleSelectedSceneV2&& graph,
 const physical::ObjectVisualTransformV1& root,std::string& error){
 error.clear();if(ready_){error="Module static scene already constructed";return false;}
 for(unsigned i=0;i<graph.scene.graph.size();++i){
  const auto& n=graph.scene.graph[i];
  if(n.parent< -1||n.parent>=static_cast<std::int32_t>(i)){error="Module node factory requires parent-first graph";return false;}
 }
 for(const auto& mesh:graph.scene.instances)if(mesh.node_index>=graph.scene.graph.size()){
  error="Module mesh instance has no source parent node";return false;
 }
 for(float v:root.root_matrix)if(!std::isfinite(v)){error="Module root requires finite actual TRS";return false;}
 graph_=std::move(graph);std::copy_n(root.root_matrix,16,root_relative_.begin());
 root_cached_=root_relative_;std::copy_n(root.root_matrix+12,3,root_position_.begin());
 std::copy_n(root.effective_scale,3,root_scale_.begin());std::copy_n(root.root_quaternion,4,root_quaternion_.begin());
 flags_.assign(graph_.scene.graph.size(),0x60f);
 authored_node_visibility_=graph_.node_visibility;authored_mesh_visibility_=graph_.instance_visibility;
 for(unsigned i=0;i<flags_.size();++i)if(!authored_node_visibility_[i])flags_[i]&=~1u;
 node_boxes_.assign(graph_.scene.graph.size(),std::array<float,6>{-1,-1,-1,1,1,1});
 mesh_fields_.resize(graph_.scene.instances.size());
 for(unsigned i=0;i<mesh_fields_.size();++i)if(!authored_mesh_visibility_[i])mesh_fields_[i].flags&=~1u;
 detached_meshes_.assign(graph_.scene.instances.size(),0);
 detached_nodes_.assign(graph_.scene.graph.size(),0);
 // Source root ctor/update caches relative transform, sets120 and clears50.
 root_flags_=(root_flags_&~0xeu)|0x10u;root_flags_=(root_flags_|0x120u)&~0x50u;
 for(unsigned i=0;i<graph_.scene.graph.size();++i)update_absolute(i);
 for(auto& mesh:graph_.scene.instances)mesh.world=graph_.scene.graph[mesh.node_index].world;
 ready_=true;return true;
}
void ModuleStaticSceneV2::update_absolute(unsigned index){
 auto& n=graph_.scene.graph[index];auto& flags=flags_[index];
 const auto parent_flags=n.parent<0?root_flags_:flags_[n.parent];
 if(!(parent_flags&0x20u)&&!(flags&0x5eu))return;
 std::array<float,16> relative{};
 dh2_node_matrix(relative.data(),n.translation,n.quaternion,n.scale);
 flags=(flags&~0xeu)|0x10u;
 const auto& parent=n.parent<0?root_cached_:graph_.scene.graph[n.parent].world;
 n.world=scene::multiply(parent,relative);
 flags=(flags|0x120u)&~0x50u;
}
void ModuleStaticSceneV2::optimize(unsigned index){
 update_absolute(index);auto& n=graph_.scene.graph[index];
 // Source getAbsoluteTransformation is cache-only after setPosition.
 std::copy_n(n.world.data()+12,3,n.translation);flags_[index]|=8u;
 math::Matrix4f world{};std::copy(n.world.begin(),n.world.end(),world.m);
 // Original updateAbsolutePosition copies/multiplies relative matrices whose
 // getRelativeTransformation tail explicitly stores identity byte zero.
 world.identity_hint=0;math::Quaternion rotation{};dh2_quat_from_matrix(&rotation,&world);
 n.quaternion[0]=rotation.x;n.quaternion[1]=rotation.y;n.quaternion[2]=rotation.z;n.quaternion[3]=rotation.w;
 flags_[index]=(flags_[index]|4u)&~0x200u;
 // Factory constructNode attaches geometry children before SNode children.
 // OptimizeStatic visits these real identity-local mesh children too. Their
 // resulting local rotation is the cached WORLD quaternion, which CopyMesh
 // subsequently copies for PF floors; it is not necessarily identity.
 for(unsigned i=0;i<graph_.scene.instances.size();++i){
  const auto& mesh=graph_.scene.instances[i];if(mesh.node_index!=index)continue;
  auto& fields=mesh_fields_[i];
  std::copy_n(mesh.world.data()+12,3,fields.position.begin());
  math::Matrix4f matrix{};std::copy(mesh.world.begin(),mesh.world.end(),matrix.m);
  matrix.identity_hint=0;math::Quaternion q{};dh2_quat_from_matrix(&q,&matrix);
  fields.quaternion={q.x,q.y,q.z,q.w};fields.flags=(fields.flags|0xcu)&~0x200u;
 }
 for(unsigned child=index+1;child<graph_.scene.graph.size();++child)
  if(graph_.scene.graph[child].parent==static_cast<std::int32_t>(index))optimize(child);
}
bool ModuleStaticSceneV2::optimize_static(std::string& error){
 error.clear();if(!ready_){error="Module static scene requires actual constructed graph";return false;}
 if(optimized_){error="Module static scene requires explicit subsequent reparent/update producer";return false;}
 // VisualObject::SetParent static branch repeats root.setPosition(root.getPosition)
 // then OptimizeStatic. Cache stays the already synchronized root matrix.
 root_flags_=(root_flags_|0xcu)&~0x200u;
 math::Matrix4f root_matrix{};std::copy_n(root_cached_.data(),16,root_matrix.m);root_matrix.identity_hint=0;
 math::Quaternion root_q{};dh2_quat_from_matrix(&root_q,&root_matrix);root_quaternion_={root_q.x,root_q.y,root_q.z,root_q.w};
 for(unsigned i=0;i<graph_.scene.graph.size();++i)if(graph_.scene.graph[i].parent<0)optimize(i);
 for(auto& mesh:graph_.scene.instances)mesh.world=graph_.scene.graph[mesh.node_index].world;
 optimized_=true;
 return true;
}
bool ModuleStaticSceneV2::remove_floor_mesh(unsigned i,std::string& error){
 error.clear();if(!optimized_||i>=mesh_fields_.size()){error="Required actual static floor mesh receiver";return false;}
 // Source virtual48(false) changes effective visibility before virtual68
 // unlinks the actual child. Keep cached pose and immutable geometry alive
 // for the caller's search snapshot; draw/search membership is detached.
 mesh_fields_[i].flags&=~1u;graph_.instance_visibility[i]=0;
 detached_meshes_[i]=1;return true;
}
bool ModuleStaticSceneV2::remove_node(unsigned i,std::string& error){
 error.clear();if(!optimized_||i>=graph_.scene.graph.size()){error="Required actual static exit node receiver";return false;}
 detached_nodes_[i]=1;
 for(unsigned m=0;m<graph_.scene.instances.size();++m){
  auto p=static_cast<int>(graph_.scene.instances[m].node_index);
  while(p>=0){if(static_cast<unsigned>(p)==i){detached_meshes_[m]=1;break;}p=graph_.scene.graph[p].parent;}
 }
 return true;
}
bool ModuleStaticSceneV2::source_node_relative(unsigned i,std::array<float,16>& out,std::string& error){
 if(!ready_||i>=flags_.size()){error="Required actual Module node relative receiver";return false;}
 const auto& n=graph_.scene.graph[i];dh2_node_matrix(out.data(),n.translation,n.quaternion,n.scale);flags_[i]=(flags_[i]&~0xeu)|0x10u;return true;
}
bool ModuleStaticSceneV2::notify_root_visibility(bool parent,std::string& error){
 error.clear();if(!ready_){error="Required constructed Module root visibility graph";return false;}
 const auto previous=root_flags_&1u;if(parent)root_flags_|=1u;else root_flags_&=~1u;
 if(previous==(root_flags_&1u))return true;
 for(unsigned i=0;i<flags_.size();++i){const bool visible=parent&&authored_node_visibility_[i];graph_.node_visibility[i]=visible?1:0;if(visible)flags_[i]|=1u;else flags_[i]&=~1u;}
 for(unsigned i=0;i<mesh_fields_.size();++i){const bool visible=parent&&authored_mesh_visibility_[i]&&!detached_meshes_[i];graph_.instance_visibility[i]=visible?1:0;if(visible)mesh_fields_[i].flags|=1u;else mesh_fields_[i].flags&=~1u;}
 return true;
}
bool ModuleStaticSceneV2::sync_root_position(const float* position,std::string& error){
 error.clear();if(!ready_||!position){error="Required same Module root position receiver";return false;}
 for(unsigned i=0;i<3;++i)if(!std::isfinite(position[i])){error="Required finite source root position";return false;}
 std::copy_n(position,3,root_position_.data());root_flags_|=8u;
 dh2_node_matrix(root_relative_.data(),root_position_.data(),root_quaternion_.data(),root_scale_.data());
 root_flags_=(root_flags_&~0xeu)|0x10u;root_cached_=root_relative_;
 // SAME SceneManager root is identity; source virtual38 cached parent multiply
 // therefore equals this relative matrix. bool false does not walk children.
 root_flags_=(root_flags_|0x120u)&~0x50u;return true;
}
bool ModuleStaticSceneV2::source_mesh_relative(unsigned i,std::array<float,16>& out,std::string& error){
 if(!ready_||i>=mesh_fields_.size()){error="Required actual Module mesh relative receiver";return false;}
 auto& m=mesh_fields_[i];dh2_node_matrix(out.data(),m.position.data(),m.quaternion.data(),m.scale.data());m.flags=(m.flags&~0xeu)|0x10u;return true;
}
bool ModuleStaticSceneV2::source_root_relative(std::array<float,16>& out,std::string& error){
 if(!ready_){error="Required actual Module root relative receiver";return false;}
 dh2_node_matrix(root_relative_.data(),root_position_.data(),root_quaternion_.data(),root_scale_.data());root_flags_=(root_flags_&~0xeu)|0x10u;out=root_relative_;return true;
}
}
