#include "module_static_scene_v2.hpp"
#include "native_scene_lights_v113.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace dh2::world {
bool ModuleStaticSceneV2::source_light_by_name_v113(const std::string& name,std::shared_ptr<NativeLightV113>& out,std::string& e)const{
 out.reset();if(!ready_){e="Actual Module root light lookup unconstructed";return false;}for(const auto& parent:native_nodes_v93_)if(parent&&!parent->native_destroyed_v106)for(const auto& node:parent->lights_v113)if(node&&node->live()&&node->source_name_v113()==name){out=node->light();e.clear();return true;}e.clear();return true;
}
bool ModuleStaticSceneV2::borrow_mesh_source_v111(unsigned index,std::shared_ptr<RetainedMeshNodeV91>& out,std::string& e)const{
 out.reset();
 if(!ready_||index>=native_meshes_v93_.size()||!native_meshes_v93_[index]||native_meshes_v93_[index]->native_destroyed_v106||!native_meshes_v93_[index]->fields){e="Required SAME live Module mesh source receiver";return false;}
 out=native_meshes_v93_[index];e.clear();return true;
}
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
 flags_.bind(graph_.scene.graph);flags_.assign(graph_.scene.graph.size(),0x60f);
 authored_node_visibility_=graph_.node_visibility;authored_mesh_visibility_=graph_.instance_visibility;
 for(unsigned i=0;i<flags_.size();++i)if(!authored_node_visibility_[i])flags_[i]&=~1u;
 node_boxes_.assign(graph_.scene.graph.size(),std::array<float,6>{-1,-1,-1,1,1,1});
 mesh_fields_.bind(graph_.scene.instances);mesh_fields_.assign(graph_.scene.instances.size(),ModuleStaticMeshFieldsV2{});
 for(unsigned i=0;i<mesh_fields_.size();++i){mesh_fields_[i].visibility.parent121=authored_node_visibility_[graph_.scene.instances[i].node_index]?1:0;if(!authored_mesh_visibility_[i])mesh_fields_[i].flags&=~1u;}
 detached_meshes_.bind(graph_.scene.instances);detached_meshes_.assign(graph_.scene.instances.size(),0);
 detached_nodes_.bind(graph_.scene.graph);detached_nodes_.assign(graph_.scene.graph.size(),0);
 // Source root ctor/update caches relative transform, sets120 and clears50.
 root_flags_=(root_flags_&~0xeu)|0x10u;root_flags_=(root_flags_|0x120u)&~0x50u;
 for(unsigned i=0;i<graph_.scene.graph.size();++i)update_absolute(i);
 for(auto& mesh:graph_.scene.instances)if(!mesh.source_storage_v91().detached)mesh.world=graph_.scene.graph[mesh.node_index].world;
 if(!initialize_native_children_v93(error))return false;ready_=true;return true;
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
  const auto& mesh=graph_.scene.instances[i];if(mesh.node_index!=index||detached_meshes_[i])continue;
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
 for(auto& mesh:graph_.scene.instances)if(!mesh.source_storage_v91().detached)mesh.world=graph_.scene.graph[mesh.node_index].world;
 optimized_=true;
 return true;
}
bool ModuleStaticSceneV2::remove_floor_mesh(unsigned i,std::string& error){
 error.clear();if(!optimized_||i>=mesh_fields_.size()){error="Required actual static floor mesh receiver";return false;}
 // Source virtual48(false) changes effective visibility before virtual68
 // unlinks the actual child. Keep cached pose and immutable geometry alive
 // for the caller's search snapshot; draw/search membership is detached.
 mesh_fields_[i].visibility.local120=0;mesh_fields_[i].flags&=~1u;graph_.instance_visibility[i]=0;
 if(i<native_meshes_v93_.size()){auto mesh=native_meshes_v93_[i];if(auto parent=mesh->parent.lock()){auto found=std::find(parent->meshes.begin(),parent->meshes.end(),mesh);if(found!=parent->meshes.end())parent->meshes.erase(found);}mesh->parent.reset();mesh->fields->parentec=0;}
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
void ModuleStaticSceneV2::update_mesh_visibility_v94(unsigned i,bool parent){
 if(detached_meshes_[i])return;auto& state=mesh_fields_[i];state.visibility.parent121=parent?1:0;
 if(state.visibility.local120&&parent)state.flags|=1u;else state.flags&=~1u;graph_.instance_visibility[i]=(state.flags&1u)?1:0;
}
void ModuleStaticSceneV2::propagate_node_visibility_v94(unsigned i){
 for(unsigned m=0;m<graph_.scene.instances.size();++m)if(graph_.scene.instances[m].node_index==i)update_mesh_visibility_v94(m,(flags_[i]&1u)!=0);
 for(unsigned n=0;n<graph_.scene.graph.size();++n)if(graph_.scene.graph[n].parent==static_cast<std::int32_t>(i))notify_node_visibility_v94(n,(flags_[i]&1u)!=0);
}
void ModuleStaticSceneV2::notify_node_visibility_v94(unsigned i,bool parent){
 if(detached_nodes_[i])return;auto& actual=graph_.scene.graph[i].source_storage_v91().visibility;const auto before=flags_[i]&1u;actual.parent121=parent?1:0;
 if(actual.local120&&parent)flags_[i]|=1u;else flags_[i]&=~1u;graph_.node_visibility[i]=(flags_[i]&1u)?1:0;
 if(before!=(flags_[i]&1u))propagate_node_visibility_v94(i);
}
bool ModuleStaticSceneV2::notify_root_visibility(bool parent,std::string& e){
 if(!ready_){e="Required SAME constructed Module root parent visibility";return false;}const auto before=root_flags_&1u;root_visibility_v94_.parent121=parent?1:0;
 if(root_visibility_v94_.local120&&parent)root_flags_|=1u;else root_flags_&=~1u;
 if(before!=(root_flags_&1u))for(unsigned i=0;i<graph_.scene.graph.size();++i)if(graph_.scene.graph[i].parent<0)notify_node_visibility_v94(i,(root_flags_&1u)!=0);e.clear();return true;
}
bool ModuleStaticSceneV2::set_root_local_visibility_v94(bool local,std::string& e){
 if(!ready_){e="Required SAME constructed Module root local visibility";return false;}
 root_visible_request209_v94_=local?1:0; //35c270 before any qualified setter.
 if(local){e.clear();return true;}
 if(root_visibility_v94_.local120==0){e.clear();return true;}const auto before=root_flags_&1u;root_visibility_v94_.local120=0;root_flags_&=~1u;
 if(before!=(root_flags_&1u))for(unsigned i=0;i<graph_.scene.graph.size();++i)if(graph_.scene.graph[i].parent<0)notify_node_visibility_v94(i,false);e.clear();return true;
}
bool ModuleStaticSceneV2::commit_root_local_visibility_v94(const std::function<bool(std::string&)>& notify,std::string& e){
 if(root_visibility_failed_v94_){e="Retained Module root failed visibility notification prefix";return false;}
 if(!ready_){e="Required SAME live Module onAnimate root";return false;}
 if(!root_visible_request209_v94_||(root_flags_&1u)){e.clear();return true;}
 //35d310 qualified ISceneNode.SetVisible(true), actual manager notification,
 //THEN209=0. This prefix precedes Root's hidden/OnAnimate200 early returns.
 const auto before=root_flags_&1u;root_visibility_v94_.local120=1;
 if(root_visibility_v94_.parent121)root_flags_|=1u;else root_flags_&=~1u;
 if(before!=(root_flags_&1u))for(unsigned i=0;i<graph_.scene.graph.size();++i)if(graph_.scene.graph[i].parent<0)notify_node_visibility_v94(i,true);
 if(!notify||!notify(e)){root_visibility_failed_v94_=true;if(e.empty())e="Required SAME Scene.notifyVisibilityChanged5890a8";return false;}root_visible_request209_v94_=0;e.clear();return true;
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
bool ModuleStaticSceneV2::source_update_absolute_v69(std::string& error){
 if(!ready_){error="Required SAME initialized Module absolute-cache owner";return false;}
 if(root_flags_&0x5eu){
  // SAME SceneManager parent's cached transform is identity. Updating this
  // optimized root must never recompute/bake already-optimized child poses.
  std::array<float,16> relative;if(!source_root_relative(relative,error))return false;
  root_cached_=relative;root_flags_=(root_flags_|0x120u)&~0x50u;
 }
 error.clear();return true;
}
}

namespace dh2::world {
bool ModuleStaticSceneV2::initialize_native_children_v93(std::string& e){
 native_nodes_v93_.reserve(graph_.scene.graph.size());for(std::size_t i=0;i<graph_.scene.graph.size();++i){auto node=std::make_shared<RetainedVisualNodeV91>();node->index=std::uint32_t(i);node->fields=graph_.scene.graph[i].source_owner_v91();node->native_parent_owned_v106=true;native_nodes_v93_.push_back(std::move(node));}
 for(std::size_t i=0;i<native_nodes_v93_.size();++i){const auto parent=graph_.scene.graph[i].parent;if(parent>=0){native_nodes_v93_[i]->parent=native_nodes_v93_[std::size_t(parent)];native_nodes_v93_[std::size_t(parent)]->children.push_back(native_nodes_v93_[i]);}}
 native_meshes_v93_.reserve(graph_.scene.instances.size());for(const auto& instance:graph_.scene.instances){auto mesh=std::make_shared<RetainedMeshNodeV91>();mesh->fields=instance.source_owner_v91();mesh->parent=native_nodes_v93_.at(instance.node_index);mesh->fields->parentec=reinterpret_cast<std::uintptr_t>(native_nodes_v93_[instance.node_index].get());mesh->source_resource_v93=resource_pin_;mesh->source_image_v93=graph_.source_image_v93;mesh->hierarchy_changed_v93=notify_hierarchy_;
  // Existing manual inspection graphs keep their historical construction
  // ABI. They never advertise an unproduced native positive map receiver.
  if(graph_.source_image_v93.bytes&&!RetainedMeshDataV91::create(graph_.source_image_v93,instance.geometry,mesh->mesh,e))return false;
  native_nodes_v93_[instance.node_index]->meshes.push_back(mesh);native_meshes_v93_.push_back(std::move(mesh));}
 if(!construct_retained_scene_lights_v113(graph_.source_image_v93,graph_.scene,native_nodes_v93_,native_meshes_v93_,e))return false;
 e.clear();return true;
}
bool ModuleStaticSceneV2::lend_map_mesh_v93(unsigned i,const std::shared_ptr<SceneManagerMapOwnerV2>& map,SceneMapNodeBorrowV2& out,std::string& e){
 if(!ready_||i>=native_meshes_v93_.size()){e="Required SAME source-created Module mesh receiver";return false;}return lend_retained_map_mesh_v93(native_meshes_v93_[i],map,out,e);
}
}

namespace dh2::world {
bool ModuleStaticSceneV2::source_mesh_name_v93(unsigned i,const std::shared_ptr<SceneManagerMapOwnerV2>& map,std::string& out,unsigned& parent,std::string& e)const{
 if(!(ready_)||i>=native_meshes_v93_.size()){e="Required SAME current mesh/name receiver";return false;}return read_retained_mesh_name_v93(native_meshes_v93_[i],map,out,parent,e);
}
}

namespace dh2::world {
bool ModuleStaticSceneV2::release_native_children_v106(std::string& e){
 if(root_parentec_){e="Actual Module root must unpublish before native child drops";return false;}
 for(const auto& node:native_nodes_v93_)if(node&&node->native_parent_owned_v106&&node->parent.expired()){
  if(!node->drop_parent_native_v106(e))return false;
 }
 for(const auto& mesh:native_meshes_v93_)if(mesh&&mesh->fields&&!mesh->fields->parentec&&!mesh->native_destroyed_v106){if(!retire_retained_map_mesh_v106(mesh,e))return false;}
 native_nodes_v93_.clear();native_meshes_v93_.clear();e.clear();return true;
}
}
#include "module_scene_node_borrow_v109.inc"
