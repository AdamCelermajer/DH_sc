#include "source_module_floors.hpp"
#include "content_paths.hpp"
#include "../level-world/decor_scene.hpp"
#include "../level-world/module_floor_append_v2.hpp"
#include "../level-world/module_floor_graph_v3.hpp"
#include "../engine-math/math.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
namespace dh::foundation {
SourceModuleFloors::SourceModuleFloors(std::shared_ptr<dh2::floors::World>w):world_(std::move(w)){if(!world_||world_->sewn||!world_->records.empty())throw std::invalid_argument("Source PF room registry requires its SAME fresh unsewn floor world");}
bool SourceModuleFloors::load(const AssetCatalog&assets,const SourceModuleFloorRequest&q,std::shared_ptr<SourceModuleFloorRoom>&out,std::string&e){
 if(failed_||world_->sewn||rooms_.size()>=512){e="Source floor registry unavailable after reached failure/postload/capacity";return false;}
 if(!q.admitted||q.pose_phase==SourceModulePosePhase::unavailable||!q.record||!q.record->receiver||!q.trace.receiver.receiver||q.trace.receiver.identity!=q.record->receiver->base().identity()||!q.trace.receiver.read||q.trace.receiver.receiver.owner_before(q.record)||q.record.owner_before(q.trace.receiver.receiver)){e="Required actual admitted Module receipt and source transform phase";return false;}
 auto&module=*q.record->receiver;auto&base=module.base();if(!module.load_floor_pending()){e="Actual Module floor375 disabled/already completed";return false;}std::int32_t module_id=-1;std::array<float,3> position;
 if(!q.trace.receiver.read(module_id,position,e))return false;if(module_id!=module.module_id()||module_id!=q.trace.runtimeId){e="Actual Module ID differs from retained source trace";return false;}
 const auto*actual_position=base.vector3(0x160);const auto*rotation=base.vector3(0x16c);const auto*scale=base.vector3(0x120);const auto*stat=base.byte(0x84);const auto*model=base.string(0x290);const auto*xref=base.string(0x2a8);const auto*name=base.string(0x30);
 if(!actual_position||!rotation||!scale||!stat||!model||!xref||!name||model->empty()||xref->empty()||q.trace.occurrence.hostOccurrence.empty()){e="Required SAME Module model/xref/TRS/static/name source fields";return false;}
 for(unsigned k=0;k<3;++k)if(position[k]!=actual_position[k]||!std::isfinite(position[k])||!std::isfinite(rotation[k])||!std::isfinite(scale[k])){e="Module position receipt or transform fields invalid";return false;}
 for(const auto&old:rooms_)if(old->module.get()==q.record.get()||old->occurrence==q.trace.occurrence.hostOccurrence){e="Source Module floor load cannot replay same receiver/occurrence";return false;}
 auto room=std::make_shared<SourceModuleFloorRoom>();room->id=static_cast<unsigned>(rooms_.size());room->source_room1c=base.room64();room->module_id=module_id;room->name=*name;room->occurrence=q.trace.occurrence.hostOccurrence;room->module=q.record;
 // Actual source visual/resource construction precedes PFRoom publication.
 try{
  room->bytes=read_content(assets,*model,q.trace.occurrence.sourceUri);if(dh2_bres_open(&room->bres,room->bytes.data(),room->bytes.size())!=dh2::resources::BresError::ok)throw std::runtime_error("Original module BRES rejected");bool found=false;
  if(!dh2::world::module_selected_scene_v2(room->bres,xref->c_str(),room->factory,found,e))throw std::runtime_error(e);if(!found)throw std::runtime_error("Actual source Module xref node missing");
  // SceneManager.LoadScene3596f8 calls ResetPositionFromFile35cc0c:
  // reset the selected first-child TRS before owner SetParent/cache update.
  if(room->factory.scene.graph.empty())throw std::runtime_error("Source selected root child absent");auto&selected_root=room->factory.scene.graph.front();std::fill_n(selected_root.translation,3,0.f);std::fill_n(selected_root.quaternion,3,0.f);selected_root.quaternion[3]=1.f;std::fill_n(selected_root.scale,3,1.f);
  dh2::physical::ObjectVisualTransformV1 transform{};
  if(q.pose_phase==SourceModulePosePhase::constructor_degrees){if(dh2_object_visual_transform_v1(&transform,position.data(),rotation,scale))throw std::runtime_error("Original Module root TRS rejected");}
  else {for(unsigned k=0;k<3;++k){transform.effective_scale[k]=scale[k];transform.rotation_radians[k]=rotation[k];}dh2::math::Quaternion quat{};dh2_quat_from_euler(&quat,rotation[1],-rotation[0],-rotation[2]);if(quat.x==0&&quat.y==0&&quat.z==0&&quat.w==1)quat={0,0,0,1};transform.root_quaternion[0]=quat.x;transform.root_quaternion[1]=quat.y;transform.root_quaternion[2]=quat.z;transform.root_quaternion[3]=quat.w;dh2_node_matrix(transform.root_matrix,position.data(),transform.root_quaternion,transform.effective_scale);}
  std::array<float,16> root;std::copy_n(transform.root_matrix,16,root.begin());auto&scene=room->factory.scene;
  // Source selected factory graph, including hidden geometry children. Each
  // child starts identity-local; update cache once through actual parent TRS.
  for(unsigned i=0;i<scene.graph.size();++i){auto&node=scene.graph[i];if(node.parent>=static_cast<int>(i)||node.parent< -1)throw std::runtime_error("Source Module graph not constructor ordered");std::array<float,16> relative;dh2_node_matrix(relative.data(),node.translation,node.quaternion,node.scale);node.world=dh2::scene::multiply(node.parent<0?root:scene.graph[node.parent].world,relative);}
  rooms_.push_back(room);out=room; // Genuine registry constructor publication before SearchByType snapshot.
  for(unsigned i=0;i<scene.instances.size();++i){auto&instance=scene.instances[i];if(instance.node_index>=scene.graph.size())throw std::runtime_error("Actual Module mesh parent missing");instance.world=scene.graph[instance.node_index].world;
   // Actual constructor mesh name is empty: source name fallback to parent.
   const auto&source_name=scene.graph[instance.node_index].name;if(source_name.find("floor")==std::string::npos)continue;if(instance.controller>=0)throw std::runtime_error("Source static floor controller branch unsupported");
   SourceModuleFloorMesh mesh;mesh.instance=i;mesh.floor=static_cast<unsigned>(world_->records.size());mesh.source_name=source_name;mesh.source_cached_world=instance.world;
   if(*stat){dh2::math::Matrix4f matrix{};std::copy(instance.world.begin(),instance.world.end(),matrix.m);matrix.identity_hint=0;dh2::math::Quaternion quat{};dh2_quat_from_matrix(&quat,&matrix);mesh.mesh_local_quaternion={quat.x,quat.y,quat.z,quat.w};}
   // Source OptimizeStatic changes child's position/quaternion/flags; its
   // constructor local scale remains1. Cached world is not recomputed after.
   if(!dh2::world::module_floor_append_pose_v2(room->bres,scene,instance,room->id,mesh.mesh_local_quaternion.data(),mesh.mesh_local_scale.data(),*world_,e))throw std::runtime_error(e);
   room->meshes.push_back(mesh);room->floors.push_back(mesh.floor);if(!dh2::world::module_floor_graph_append_v3(*world_,mesh.floor,e))throw std::runtime_error(e);
  }
  room->completed=true;e.clear();return true;
 }catch(const std::exception&failure){failed_=true;room->failure=e=failure.what();return false;}
}
bool SourceModuleFloors::post_load(std::string&e){if(failed_||world_->sewn||rooms_.empty()){e="Actual PF room graph incomplete/unavailable";return false;}for(const auto&room:rooms_)if(!room->completed){e="Source PF room load prefix incomplete";return false;}if(!dh2::floors::post_load(*world_,e)){failed_=true;return false;}
 // Preserve real empty trailing PFRoom constructor entries that floor grouping
 // alone cannot infer. Refresh borrowed room pointers after vector growth.
 world_->collision_room_floors.resize(rooms_.size());world_->collision_rooms.resize(rooms_.size());for(unsigned i=0;i<rooms_.size();++i){auto&room=world_->collision_rooms[i];const auto&ids=world_->collision_room_floors[i];room.floors=ids.data();room.count=static_cast<unsigned>(ids.size());}world_->collision_world.rooms=world_->collision_rooms.data();world_->collision_world.room_count=static_cast<unsigned>(rooms_.size());e.clear();return true;}
}



