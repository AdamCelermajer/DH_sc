#include "module_pf_room_v3.hpp"
#include "retained_gameobject_visual_v1.hpp"
#include "module_floor_append_v2.hpp"
#include <algorithm>
#include <stdexcept>
#include <type_traits>
namespace dh2::world {
namespace {
void extend(octree::Box& a,const octree::Box& b){for(unsigned k=0;k<3;++k){a.minimum[k]=std::min(a.minimum[k],b.minimum[k]);a.maximum[k]=std::max(a.maximum[k],b.maximum[k]);}}
}
ModulePFRoomsV3::ModulePFRoomsV3(std::shared_ptr<floors::World> world,std::shared_ptr<SceneManagerMapOwnerV2> map,ModulePFDebugV3 debug):
 world_(std::move(world)),native_storage_v106_(PFNativeStorageLifecycleV106::create_fresh(world_)),map_(std::move(map)),debug_(std::move(debug)){}
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
  [&visual](unsigned i,std::string& e){return visual.remove_node(i,e);},
  [&visual,this](unsigned i,SceneMapNodeBorrowV2& out,std::string& e){return visual.lend_map_mesh_v93(i,map_,out,e);},
  [&visual,this](unsigned i,std::string& name,unsigned& parent,std::string& e){return visual.source_mesh_name_v93(i,map_,name,parent,e);},id,name,out,error);
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
  [&visual](unsigned i,std::string& e){return visual.remove_node_v76(i,e);},
  [&visual,this](unsigned i,SceneMapNodeBorrowV2& out,std::string& e){return visual.lend_map_mesh_v93(i,map_,out,e);},
  [&visual,this](unsigned i,std::string& name,unsigned& parent,std::string& e){return visual.source_mesh_name_v93(i,map_,name,parent,e);},id,name,out,error);
}
bool ModulePFRoomsV3::load_graph_v77(const resources::BresView& view,std::shared_ptr<void> resource,
 const scene::Scene& s,const std::function<bool(unsigned)>& attached,
 const std::function<bool(unsigned,std::array<float,4>&,std::array<float,3>&,std::string&)>& mesh_pose,
 const std::function<bool(unsigned,std::string&)>& remove_floor,
 const std::function<bool(unsigned,std::string&)>& remove_node,
 const std::function<bool(unsigned,SceneMapNodeBorrowV2&,std::string&)>& map_mesh,
 const std::function<bool(unsigned,std::string&,unsigned&,std::string&)>& mesh_name,unsigned id,const std::string& name,
 std::shared_ptr<ModulePFRoomV3>& out,std::string& error){
 error.clear();if(flush_attempted_v1_){error="Cannot load into PFWorld after reached Flush";return false;}
 if(!world_||!map_||!resource){error="Required SAME PF world/map/resource producer";return false;}
 if(world_->sewn||rooms_.size()>=512){error="PF LoadRoom requires active unsewn room graph";return false;}
 if(!native_storage_v106_->initialize_for_load(error))return false;
 // PFWorld publishes its freshly constructed room before SearchByType.
 auto room=std::make_shared<ModulePFRoomV3>();room->name=name;room->source_room1c=id;
 room->id=static_cast<unsigned>(rooms_.size());rooms_.push_back(room);
 if(!attached||!mesh_pose||!remove_floor||!remove_node||!map_mesh||!mesh_name){error="Required actual retained mesh/node source operations";return false;}
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
 for(const unsigned i:snapshot){unsigned operand{};std::string actual_name;if(!mesh_name(i,actual_name,operand,error))return false;if(actual_name.find("minimap")!=std::string::npos)minimap=i;}
 for(const unsigned i:snapshot){
  const auto& instance=s.instances[i];unsigned exit_node{};std::string actual_name;if(!mesh_name(i,actual_name,exit_node,error))return false;
  if(actual_name.find("floor")!=std::string::npos){
   if(world_->records.size()>=512){error="PF floor record limit exceeded";return false;}
   const unsigned fid=world_->records.size();auto record=std::make_unique<floors::Record>();record->name=actual_name;record->room=room->id;record->flags={0,1};
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
   // Original523dfc completed _LoadFloor (including first clone map add)
   // BEFORE523e00 selects its captured minimap or current LAST floor clone.
   if(minimap!=UINT32_MAX){
    if(!map_mesh(minimap,room->minimap_prefix_v93,error))return false;
    if(!map_->add(room->minimap_prefix_v93,error))return false;
    room->minimap_prefix_v93={}; // Actual map owns SAME child after success.
   }else{
    if(room->clones.empty()){error="Original LoadRoom last floor clone was not produced";return false;}
    if(!map_->add(room->clones.back()->map_node(),error))return false; //523eb4..ed4, EACH reached floor.
   }
  }
  if(actual_name.find("_exit_")!=std::string::npos){
   ModulePFExitV3 exit;const char* names[]{"north","south","east","west"};
   for(unsigned d=0;d<4;++d)if(actual_name.find(names[d])!=std::string::npos){exit.direction=d;break;}
   const auto& cache=exit_node==UINT32_MAX?instance.world:s.graph[exit_node].world;
   std::copy_n(cache.data()+12,3,exit.position.begin());exits_.push_back(exit);
   if(exit_node!=UINT32_MAX){if(!remove_node(exit_node,error))return false;}
   else{if(!map_mesh(i,room->exit_prefix_v93,error)||!room->exit_prefix_v93.detach||!room->exit_prefix_v93.detach(error))return false;room->exit_prefix_v93={};}
  }
 }
 if(room->floors.empty()){rooms_.pop_back();out.reset();return true;}
 if(rooms_.size()==1)bounds_=room->bounds;else extend(bounds_,room->bounds);
 out=std::move(room);return true;
}
bool ModulePFRoomsV3::flush_source_v1(const PFWorldFlushServicesV1& services,const std::shared_ptr<void>& actual_pf_owner,std::uintptr_t actual_pf_identity,std::string& e){
 if(!actual_pf_owner||!actual_pf_identity){e="Required live actual PFWorld receiver for source Flush";return false;}
 if(flush_attempted_v1_){auto expected=flush_receiver_v1_.lock();if(!expected||flush_receiver_identity_v1_!=actual_pf_identity||expected.owner_before(actual_pf_owner)||actual_pf_owner.owner_before(expected)){e="PF Flush journal belongs to a different actual receiver";return false;}}
 if(flush_complete_v1_){e.clear();return true;}
 if(flush_attempted_v1_){e=flush_failure_v1_.empty()?"PFWorld.Flush cannot replay/reenter a reached native prefix":flush_failure_v1_;return false;}
 if(!world_||!map_||!services.owner||!services.require_quiescent||!services.actual_fields){e="Required SAME PF owner and genuine Flush admission/field borrows";return false;}
 flush_phase_v1_=PFWorldFlushPhaseV1::quiesce;
 if(!services.require_quiescent(*this,e))return false;
 // Native PFFloorD1 does only mesh40.drop; its last reference must be the
 // floor after earlier actual Scene virtual68 removed SAME map children.
 if(!map_->children().empty()){e="Require genuine Scene/map child unpublication before PFWorld.Flush";return false;}
 PFWorldFlushFieldsV1 fields;flush_phase_v1_=PFWorldFlushPhaseV1::actual_fields;
 if(!services.actual_fields(*world_,actual_pf_identity,fields,e)||!fields.owner||fields.identity!=actual_pf_identity||fields.owner.owner_before(actual_pf_owner)||actual_pf_owner.owner_before(fields.owner)||fields.storage_owner.get()!=world_.get()||fields.storage_owner.owner_before(world_)||world_.owner_before(fields.storage_owner)||!fields.initialized4||!fields.outer44||!fields.inner48||!fields.erase_floor_objects2c){if(e.empty())e="Required actual SAME PFWorld initialized4/graph44/48/map2c/exits84 producers";return false;}
 flush_receiver_v1_=actual_pf_owner;flush_receiver_identity_v1_=actual_pf_identity;flush_attempted_v1_=true;
 auto fail=[&]{if(flush_failure_v1_.empty())flush_failure_v1_=e.empty()?"Required actual PFWorld native destruction leaf":e;if(flush_phase_v1_!=PFWorldFlushPhaseV1::failed){flush_failed_at_v1_=flush_phase_v1_;flush_phase_v1_=PFWorldFlushPhaseV1::failed;}e=flush_failure_v1_;return false;};
 try{
  // PFWorld5236f0 room D0; PFRoomD1 521de8 floor D0 in actual array order.
  for(auto& room_slot:rooms_){if(!room_slot)continue;flush_phase_v1_=PFWorldFlushPhaseV1::rooms;auto room=room_slot;
   if(room->clones.size()>room->floors.size()){e="PFRoom clone ownership differs from SAME floor slots";return fail();}
   for(std::size_t index=0;index<room->floors.size();++index){const auto fid=room->floors[index];
    if(fid>=world_->records.size()){e="Require SAME published PFRoom floor allocation";return fail();}
    if(!world_->records[fid])continue;auto* record=world_->records[fid].get();
    flush_phase_v1_=PFWorldFlushPhaseV1::floor_aux68;
    if(!services.floor_aux68_d1||!services.floor_aux68_d1(*record,e))return fail();
    // Original51ccdc..51ccfc drops mesh40 BEFORE c0/b4/a8/tree/name tail.
    // A bounded native failed load can have an actually NULL clone slot.
    if(index<room->clones.size()&&room->clones[index]){
     flush_phase_v1_=PFWorldFlushPhaseV1::clone_drop40;auto clone=room->clones[index];
     if(&clone->floor()!=record){e="Floor mesh40 does not borrow SAME Record";return fail();}
     if(!clone->drop_floor_reference_v1(services.clone,e))return fail();
     if(index>=room->clones.size()||room->clones[index].get()!=clone.get()||room->clones[index].owner_before(clone)||clone.owner_before(room->clones[index])){e="Actual floor mesh40 owner replaced during clone D1";return fail();}room->clones[index].reset();
    }
    flush_phase_v1_=PFWorldFlushPhaseV1::floor_tail;
    if(!services.floor_tail_d1||!services.floor_tail_d1(*record,e))return fail();
    if(fid>=world_->records.size()||world_->records[fid].get()!=record){e="Actual PFFloor allocation replaced during native D1";return fail();}
    // Native PFFloorD0 CustomFree continuation, not passive expiry. Existing
    // Record owns this successor's buffers; real D1 leaves preceded freeing.
    world_->records[fid].reset();
   }
   // Actual PFRoomD1 frees floor pointer array then room name; D0 frees room.
   flush_phase_v1_=PFWorldFlushPhaseV1::room_storage;
   std::vector<unsigned>{}.swap(room->floors);std::vector<std::shared_ptr<ModuleFloorCloneV3>>{}.swap(room->clones);std::string{}.swap(room->name);
   if(room_slot.get()!=room.get()){e="PFRoom allocation replaced during native D1";return fail();}room_slot.reset();
  }
  // Original Flush sets vector.end=begin, retaining its room-vector capacity.
  rooms_.clear();
  for(const auto& record:world_->records)if(record){e="Actual PF Record outside destroyed source Room floor ownership; refusing passive D0";return fail();}
  world_->records.clear();
  flush_phase_v1_=PFWorldFlushPhaseV1::bounds_zero;bounds_={}; // SAME six14..28 cells
  flush_phase_v1_=PFWorldFlushPhaseV1::object_map2c;if(!fields.erase_floor_objects2c(e))return fail();
  flush_phase_v1_=PFWorldFlushPhaseV1::exits84;exits_.clear(); // original vector84.end=begin BEFORE graph D0s
  auto graph_d0=[&](std::uintptr_t& slot,PFWorldFlushPhaseV1 phase){flush_phase_v1_=phase;const auto identity=slot;if(!identity)return true;PFGraphDestructionV1 graph;
   if(!fields.graph||!fields.graph(identity,graph,e)||!graph.receiver||graph.identity!=identity||!graph.native_d0){if(e.empty())e="Required SAME positive PF sparse-graph native D0 owner";return fail();}
   if(!graph.native_d0(e))return fail();slot=0;return true;
  };
  if(!graph_d0(*fields.outer44,PFWorldFlushPhaseV1::outer44)||!graph_d0(*fields.inner48,PFWorldFlushPhaseV1::inner48))return false;
  flush_phase_v1_=PFWorldFlushPhaseV1::initialized_zero;*fields.initialized4=0;
  // Native projections invalidate only after actual source graph D0s. Keep
  // old PF arrays/pins intact if either owner/leaf failed. No alternate graph.
  auto retire=[](auto& values){using Storage=std::decay_t<decltype(values)>;Storage{}.swap(values);};
  retire(world_->selectors);retire(world_->nodes);retire(world_->edges);retire(world_->invalid);retire(world_->validation);retire(world_->floor_graphs);retire(world_->validation_floors);retire(world_->first_boundary);retire(world_->second_boundary);retire(world_->links);
  retire(world_->search_edges);retire(world_->search_offsets);retire(world_->search_nodes);retire(world_->search_heap);retire(world_->collision_room_floors);retire(world_->collision_rooms);retire(world_->collision_floors);retire(world_->route_traits);// Source Flush leaves SearchFailCache78 and path deque4c alive.
  // SAME supported route cache/path buffers persist until their real source
  // owner reset/D1; do not silently add a cache/queue destruction here.
  world_->sewing={};world_->collision_world={};world_->route_graph={};world_->route_world={};world_->route_search={};world_->route_workspace={};world_->graph={};world_->sewn=false;
  flush_complete_v1_=true;flush_phase_v1_=PFWorldFlushPhaseV1::complete;e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return fail();}catch(...){e="PFWorld native teardown provider threw; preserving remaining source prefix";return fail();}
}
}

