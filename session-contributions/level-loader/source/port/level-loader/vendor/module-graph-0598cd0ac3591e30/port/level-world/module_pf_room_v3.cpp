#include "module_pf_room_v3.hpp"
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
 // Decor::LoadFloorMap388730 updates the room flag before ExtendBoundingBox.
 room->flags24=(room->flags24&~1u)|(solid?1u:0u);
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
 error.clear();if(!world_||!map_||!resource){error="Required SAME PF world/map/resource producer";return false;}
 if(world_->sewn||rooms_.size()>=512){error="PF LoadRoom requires active unsewn room graph";return false;}
 // PFWorld publishes its freshly constructed room before SearchByType.
 auto room=std::make_shared<ModulePFRoomV3>();room->name=name;room->source_room1c=id;
 room->id=static_cast<unsigned>(rooms_.size());rooms_.push_back(room);
 const auto& s=visual.selected().scene;const auto& fields=visual.mesh_fields();
 if(fields.size()!=s.instances.size()){error="Required actual static mesh field owner";return false;}
 // Search snapshot includes hidden mesh children; parent fallback is source
 // constructor mesh.name empty, not visibility-filtered scene::load output.
 unsigned minimap=UINT32_MAX;
 for(unsigned i=0;i<s.instances.size();++i){if(!visual.mesh_attached(i))continue;
  const auto& n=s.graph[s.instances[i].node_index];if(n.name.find("minimap")!=std::string::npos)minimap=i;}
 if(minimap!=UINT32_MAX){error="Required actual minimap mesh AddNodeToMap continuation";return false;}
 for(unsigned i=0;i<s.instances.size();++i){
  const auto& instance=s.instances[i];const auto node_index=instance.node_index;
  const auto& n=s.graph[node_index];const auto& pose=fields[i];
  if(n.name.find("floor")!=std::string::npos){
   if(world_->records.size()>=512){error="PF floor record limit exceeded";return false;}
   const unsigned fid=world_->records.size();auto record=std::make_unique<floors::Record>();record->name=n.name;record->room=room->id;record->flags={0,1};
   auto* actual=record.get();world_->records.push_back(std::move(record));room->floors.push_back(fid);
   if(!debug_.owner||!debug_.query){error="Required source Debug navmesh-load producer";return false;}
   bool tracing=false;if(!debug_.query("isTracingNavMeshLoadTime",tracing,error))return false;
   std::uint32_t start=0,end=0;
   if(tracing&&!debug_.clock_ms){error="Required source navmesh tracing clock continuation";return false;}
   if(tracing&&!debug_.clock_ms(start,error))return false;
   if(!module_floor_load_record_pose_v2(view,s,instance,room->id,pose.quaternion.data(),pose.scale.data(),*actual,error))return false;
   // Source copies the node then hides/removes original before building its
   // navigation graph. Retain geometry/node/PF lifetimes across callbacks.
   std::shared_ptr<ModuleFloorCloneV3> clone;
   if(!ModuleFloorCloneV3::create(resource,world_,*actual,pose.quaternion.data(),pose.scale.data(),map_,clone,error))return false;
   room->clones.push_back(clone);
   if(!visual.remove_floor_mesh(i,error)||!module_floor_graph_append_v3(*world_,fid,error))return false;
   if(tracing&&!debug_.clock_ms(end,error))return false;
   if(room->floors.size()==1)room->bounds=actual->bounds;else extend(room->bounds,actual->bounds);
   if(!map_->add(clone->map_node(),error))return false; // PFRoom::_LoadFloor
   if(!map_->add(clone->map_node(),error))return false; // PFWorld::LoadRoom
  }
  if(n.name.find("_exit_")!=std::string::npos){
   ModulePFExitV3 exit;const char* names[]{"north","south","east","west"};
   for(unsigned d=0;d<4;++d)if(n.name.find(names[d])!=std::string::npos){exit.direction=d;break;}
   std::copy_n(n.world.data()+12,3,exit.position.begin());exits_.push_back(exit);
   if(!visual.remove_node(node_index,error))return false;
  }
 }
 if(room->floors.empty()){rooms_.pop_back();out.reset();return true;}
 if(rooms_.size()==1)bounds_=room->bounds;else extend(bounds_,room->bounds);
 out=std::move(room);return true;
}
}
