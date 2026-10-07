#include "retained_map_mesh_v93.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
namespace dh2::world {
bool lend_retained_map_mesh_v93(const std::shared_ptr<RetainedMeshNodeV91>& mesh,
 const std::shared_ptr<SceneManagerMapOwnerV2>& map,SceneMapNodeBorrowV2& out,std::string& e){
 if(!mesh||!mesh->fields||!mesh->mesh||!map||!mesh->source_resource_v93||!mesh->source_image_v93.bytes){e="Required independent actual minimap mesh/geometry/resource owner";return false;}
 if(mesh->fields->controller>=0){e="Actual skinned minimap child is outside admitted static IMeshSceneNode domain";return false;}
 SceneMapNodeBorrowV2 source;source.owner=mesh;source.source_mesh_v93=mesh;source.identity=reinterpret_cast<std::uintptr_t>(mesh.get());source.parentec=&mesh->fields->parentec;source.flags11c=&mesh->fields->mesh_fields.flags;
 const std::weak_ptr<SceneManagerMapOwnerV2> weak=map;
 source.cached_position=[mesh](std::array<float,3>& out,std::string& e){std::copy_n(mesh->fields->world.data()+12,3,out.begin());e.clear();return true;};
 source.set_position=[mesh](const std::array<float,3>& value,std::string& e){mesh->fields->mesh_fields.position=value;mesh->fields->mesh_fields.flags|=8u;e.clear();return true;};
 source.detach=[mesh,weak](std::string& e){
  if(!mesh->fields->parentec){e.clear();return true;}
  if(auto old=mesh->map_parent_v93.lock();old&&mesh->fields->parentec==old->identity())return old->remove(reinterpret_cast<std::uintptr_t>(mesh.get()),e);
  auto parent=mesh->parent.lock();if(!parent||mesh->fields->parentec!=reinterpret_cast<std::uintptr_t>(parent.get())){e="Required SAME original minimap mesh parent membership";return false;}
  const auto found=std::find(parent->meshes.begin(),parent->meshes.end(),mesh);if(found==parent->meshes.end()){e="Actual parent has no SAME minimap mesh child";return false;}
  parent->meshes.erase(found);mesh->parent.reset();mesh->fields->parentec=0;mesh->fields->detached=1; // Original visual membership only; map visibility remains genuine.
  auto map=weak.lock();if(!map){e="Retired SAME map authority after native mesh detach";return false;}return map->source_child_detached_v93(e);
 };
 source.optimize_static=[mesh,weak](std::string& e){
  if(mesh->fields->parentec){e="Actual minimap must detach before source OptimizeStatic";return false;}
  auto& state=mesh->fields->mesh_fields;auto& cache=mesh->fields->world;
  if(state.flags&0x5eu){dh2_node_matrix(cache.data(),state.position.data(),state.quaternion.data(),state.scale.data());state.flags=(state.flags|0x120u)&~0x5eu;state.flags|=0x10u;}
  std::copy_n(cache.data()+12,3,state.position.begin());math::Matrix4f matrix{};std::copy(cache.begin(),cache.end(),matrix.m);matrix.identity_hint=0;math::Quaternion q{};dh2_quat_from_matrix(&q,&matrix);
  state.quaternion={q.x,q.y,q.z,q.w};state.flags=(state.flags|0xcu)&~0x200u;mesh->map_parent_v93=weak;e.clear();return true;
 };
 source.notify_parent_visibility=[mesh](bool parent,std::string& e){auto& actual=mesh->fields->mesh_fields;actual.visibility.parent121=parent?1:0;
  if(actual.visibility.local120&&parent)actual.flags|=1u;else actual.flags&=~1u;e.clear();return true;};
 source.scene_phase_v69=[mesh,weak](std::uint32_t,std::string& e){auto map=weak.lock();if(!map||mesh->fields->parentec!=map->identity()){e="Required SAME mapped mesh parent/cache phase";return false;}
  auto& actual=mesh->fields->mesh_fields;const auto flags=actual.flags;if(((flags&0x400u)&&!(flags&1u))||!(flags&0x200u)){e.clear();return true;}
  // Actual OptimizeStatic disabled animation on this SAME child. If native
  // parent flags later demand cache work, update this one real node only.
  if((flags&0x5eu)||(map->flags()&0x20u)){std::array<float,16> relative;dh2_node_matrix(relative.data(),actual.position.data(),actual.quaternion.data(),actual.scale.data());mesh->fields->world=scene::multiply(map->cached_matrix(),relative);actual.flags=(actual.flags|0x120u)&~0x5eu;actual.flags|=0x10u;}
  e.clear();return true;
 };
 out=std::move(source);e.clear();return true;
}
}

namespace dh2::world {
bool read_retained_mesh_name_v93(const std::shared_ptr<RetainedMeshNodeV91>& mesh,
 const std::shared_ptr<SceneManagerMapOwnerV2>& map,std::string& out,unsigned& exit_node,std::string& e){
 if(!mesh||!mesh->fields||!map){e="Required SAME source-created mesh/name fields";return false;}exit_node=UINT32_MAX;
 const auto& own=mesh->fields->native_name24;if(!own.empty()||!mesh->fields->parentec){out=own;e.clear();return true;}
 if(auto parent=mesh->parent.lock();parent&&mesh->fields->parentec==reinterpret_cast<std::uintptr_t>(parent.get())){out=parent->fields->name;exit_node=parent->index;e.clear();return true;}
 if(mesh->fields->parentec==map->identity()){auto actual=mesh->map_parent_v93.lock();if(actual!=map){e="Minimap parent receipt differs from actual map group";return false;}out=map->source_name_v93();e.clear();return true;}
 e="Required actual current mesh parent name receiver";return false;
}
bool retire_retained_map_mesh_v106(const std::shared_ptr<RetainedMeshNodeV91>& mesh,std::string& e){
 if(!mesh||!mesh->fields){e="Required SAME actual map mesh D1 receiver";return false;}
 if(mesh->native_destroyed_v106){e.clear();return true;}
 if(mesh->fields->parentec){e="Map child must lose actual parent link before final native drop";return false;}
 if(auto parent=mesh->parent.lock();parent&&std::find(parent->meshes.begin(),parent->meshes.end(),mesh)!=parent->meshes.end()){
  e="Transferred map mesh still owns original Visual parent membership";return false;
 }
 mesh->destroy_native_storage_v106();e.clear();return true;
}
void RetainedMeshNodeV91::destroy_native_storage_v106()noexcept{
 // Static IMeshSceneNode successor: mesh data/material storage and its real
 // resource reference are independent from the retired containing Visual.
 // Retained diagnostic field aliases stay valid but may not republish a node.
 mesh.reset();source_resource_v93.reset();source_image_v93={};local_light_v113={};material_lights_v113.clear();
 std::vector<std::uint32_t>{}.swap(fields->materials);
 std::string{}.swap(fields->native_name24);std::string{}.swap(fields->node);
 fields->detached=1;parent.reset();map_parent_v93.reset();hierarchy_changed_v93={};
 native_destroyed_v106=true;
}
}
