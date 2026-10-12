#include "module_pf_room_v3.hpp"
#include "retained_gameobject_visual_v1.hpp"
#include "module_floor_append_v2.hpp"
#include <algorithm>
namespace dh2::world {
namespace {
void extend(octree::Box& a,const octree::Box& b){for(unsigned k=0;k<3;++k){a.minimum[k]=std::min(a.minimum[k],b.minimum[k]);a.maximum[k]=std::max(a.maximum[k],b.maximum[k]);}}
}
ModulePFRoomsV3::ModulePFRoomsV3(std::shared_ptr<floors::World> world,std::shared_ptr<SceneManagerMapOwnerV2> map,ModulePFDebugV3 debug):
 world_(std::move(world)),map_(std::move(map)),debug_(std::move(debug)){}
bool ModulePFRoomsV3::extend_owner_bounds(const std::shared_ptr<ModulePFRoomV3>& room,
 const float* box,bool solid,std::string& error){
 error.clear();if(!room||!box||std::find(rooms_.begin(),rooms_.end(),room)==rooms_.end()){
  error="Required SAME published PFRoom and owner AABB";return false;
 }
 // Historical combined adapter preserves its existing source flag write.
 room->flags24=(room->flags24&~1u)|(solid?1u:0u);
 return extend_source_bounds_v77(room,box,error);
}
bool ModulePFRoomsV3::extend_source_bounds_v77(const std::shared_ptr<ModulePFRoomV3>& room,
 const float* box,std::string& error){
 error.clear();if(!room||!box||std::find(rooms_.begin(),rooms_.end(),room)==rooms_.end()){
  error="Required SAME published PFRoom and owner AABB";return false;
 }
 octree::Box owner;std::copy_n(box,3,owner.minimum);std::copy_n(box+3,3,owner.maximum);
 // PFRoom::ExtendBoundingBox388218: room union first, parent PFWorld union next.
 extend(room->bounds,owner);extend(bounds_,owner);return true;
}
bool ModulePFRoomsV3::publish_collision_bounds(std::string& error){
 error.clear();if(!world_||!world_->sewn){error="Required SAME completed PF floor post-load graph";return false;}
 for(const auto& room:rooms_){
  if(room->id>=world_->collision_rooms.size()||room->id>=world_->collision_room_floors.size()||world_->collision_room_floors[room->id]!=room->floors){
   error="Required exact retained PFRoom floor membership";return false;
  }
 }
 for(const auto& room:rooms_)world_->collision_rooms[room->id].bounds=room->bounds;
 world_->collision_world.bounds=bounds_;return true;
}
bool ModulePFRoomsV3::load(const resources::BresView& view,std::shared_ptr<void> resource,
 ModuleStaticSceneV2& visual,unsigned id,const std::string& name,
 std::shared_ptr<ModulePFRoomV3>& out,std::string& error){
 if(visual.mesh_fields().size()!=visual.selected().scene.instances.size()){
  error="Required actual static mesh field owner";return false;
 }
 auto pose=[&visual](unsigned i,std::array<float,4>& q,std::array<float,3>& scale,std::string& e){
  if(i>=visual.mesh_fields().size()){e="Required SAME static mesh local TRS";return false;}
  const auto& actual=visual.mesh_fields()[i];q=actual.quaternion;scale=actual.scale;e.clear();return true;
 };
 return load_graph_v77(view,std::move(resource),visual.selected().scene,
  [&visual](unsigned i){return visual.mesh_attached(i);},pose,
  [&visual](unsigned i,std::string& e){return visual.remove_floor_mesh(i,e);},
  [&visual](unsigned i,std::string& e){return visual.remove_node(i,e);},id,name,out,error);
}
bool ModulePFRoomsV3::load(RetainedGameObjectVisualV1& visual,unsigned id,const std::string& name,
 std::shared_ptr<ModulePFRoomV3>& out,std::string& error){
 if(!visual.ready()||!visual.root_identity()){
  error="Required actual initialized retained scenery visual/root for PFRoom load";return false;
 }
 return load_graph_v77(visual.bres(),visual.resource_owner_v76(),visual.scene(),
  [&visual](unsigned i){return visual.mesh_attached_v76(i);},
  [&visual](unsigned i,std::array<float,4>& q,std::array<float,3>& scale,std::string& e){return visual.source_mesh_pose_v76(i,q,scale,e);},
  [&visual](unsigned i,std::string& e){return visual.remove_floor_mesh_v76(i,e);},
  [&visual](unsigned i,std::string& e){return visual.remove_node_v76(i,e);},id,name,out,error);
}
bool ModulePFRoomsV3::load_graph_v77(const resources::BresView& view,std::shared_ptr<void> resource,
 const scene::Scene& s,const std::function<bool(unsigned)>& attached,
 const std::function<bool(unsigned,std::array<float,4>&,std::array<float,3>&,std::string&)>& mesh_pose,
 const std::function<bool(unsigned,std::string&)>& remove_floor,
 const std::function<bool(unsigned,std::string&)>& remove_node,unsigned id,const std::string& name,
 std::shared_ptr<ModulePFRoomV3>& out,std::string& error){
 error.clear();if(!world_||!map_||!resource){error="Required SAME PF world/map/resource producer";return false;}
 if(world_->sewn||rooms_.size()>=512){error="PF LoadRoom requires active unsewn room graph";return false;}
 // PFWorld publishes its freshly constructed room before SearchByType.
 auto room=std::make_shared<ModulePFRoomV3>();room->name=name;room->source_room1c=id;
 room->id=static_cast<unsigned>(rooms_.size());rooms_.push_back(room);
 if(!attached||!mesh_pose||!remove_floor||!remove_node){error="Required actual retained mesh/node source operations";return false;}
 // Original SearchByType snapshot includes hidden attached meshes. Keep stable
 // indices through subsequent native removal; never requery or reindex them.
 std::vector<unsigned> snapshot;snapshot.reserve(s.instances.size());
 for(unsigned i=0;i<s.instances.size();++i)if(attached(i)){
  if(s.instances[i].node_index>=s.graph.size()){error="Required actual PF mesh parent node";return false;}
  snapshot.push_back(i);
 }
 // Search snapshot includes hidden mesh children; parent fallback is source
 // constructor mesh.name empty, not visibility-filtered scene::load output.
 unsigned minimap=UINT32_MAX;
 for(const unsigned i:snapshot){
  const auto& n=s.graph[s.instances[i].node_index];if(n.name.find("minimap")!=std::string::npos)minimap=i;}
 for(const unsigned i:snapshot){
  const auto& instance=s.instances[i];const auto node_index=instance.node_index;
  const auto& n=s.graph[node_index];
  if(n.name.find("floor")!=std::string::npos){
   if(world_->records.size()>=512){error="PF floor record limit exceeded";return false;}
   const unsigned fid=world_->records.size();auto record=std::make_unique<floors::Record>();record->name=n.name;record->room=room->id;record->flags={0,1};
   auto* actual=record.get();world_->records.push_back(std::move(record));room->floors.push_back(fid);
   if(!debug_.owner||!debug_.query){error="Required source Debug navmesh-load producer";return false;}
   bool tracing=false;if(!debug_.query("isTracingNavMeshLoadTime",tracing,error))return false;
   std::uint32_t start=0,end=0;
   if(tracing&&!debug_.clock_ms){error="Required source navmesh tracing clock continuation";return false;}
   if(tracing&&!debug_.clock_ms(start,error))return false;
   std::array<float,4> quaternion;std::array<float,3> scale;
   if(!mesh_pose(i,quaternion,scale,error))return false;
   if(!module_floor_load_record_pose_v2(view,s,instance,room->id,quaternion.data(),scale.data(),*actual,error))return false;
   // Source copies the node then hides/removes original before building its
   // navigation graph. Retain geometry/node/PF lifetimes across callbacks.
   std::shared_ptr<ModuleFloorCloneV3> clone;
   if(!ModuleFloorCloneV3::create(resource,world_,*actual,quaternion.data(),scale.data(),map_,clone,error))return false;
   room->clones.push_back(clone);
   if(!remove_floor(i,error)||!module_floor_graph_append_v3(*world_,fid,error))return false;
   if(tracing&&!debug_.clock_ms(end,error))return false;
   if(room->floors.size()==1)room->bounds=actual->bounds;else extend(room->bounds,actual->bounds);
   if(!map_->add(clone->map_node(),error))return false; // PFRoom::_LoadFloor
   // Original523dfc completes _LoadFloor BEFORE523e00 tests the captured
   // minimap. With no floor, this branch is never reached and the native
   // room later returns NULL. Keep the real record/clone/first-map-add prefix.
   if(minimap!=UINT32_MAX){
    // The selected SAME authored mesh has no independent native child owner
    // in this retained graph yet. A Visual/root alias or copied mesh/Scene is
    // not an AddNodeToMap receiver; fail exactly at native523e24.
    error="Required independent SAME authored minimap mesh AddNodeToMap receiver";return false;
   }
   // Native523eb4..523ed4 uses the last floor's freshly copied mesh only
   // when the first search found no minimap. This is the actual clone above.
   if(!map_->add(clone->map_node(),error))return false; // PFWorld::LoadRoom
  }
  if(n.name.find("_exit_")!=std::string::npos){
   ModulePFExitV3 exit;const char* names[]{"north","south","east","west"};
   for(unsigned d=0;d<4;++d)if(n.name.find(names[d])!=std::string::npos){exit.direction=d;break;}
   std::copy_n(n.world.data()+12,3,exit.position.begin());exits_.push_back(exit);
   if(!remove_node(node_index,error))return false;
  }
 }
 if(room->floors.empty()){rooms_.pop_back();out.reset();return true;}
 if(rooms_.size()==1)bounds_=room->bounds;else extend(bounds_,room->bounds);
 out=std::move(room);return true;
}
}
