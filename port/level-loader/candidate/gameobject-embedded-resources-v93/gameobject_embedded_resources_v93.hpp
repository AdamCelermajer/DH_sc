#pragma once
#include "actor_runtime.hpp"
#include "character_target_search.hpp"
#include <deque>
#include <iterator>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <utility>
namespace dh2::world {
struct TargetBackupObjectBorrowV93 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};std::uint8_t* references29{};
};
// Original ObjectSearcher.TargetList at GameObject304: primary deque of
// TargetInfo + map<string,BackupObjectList>. Not a world target registry.
// Source20-byte TargetInfo is SAME existing native-width Target24 semantics.
class GameObjectTargetListResourceV93 final {
 std::deque<target_search::Target24> primary_;
 std::map<std::string,std::vector<TargetBackupObjectBorrowV93>> backups_;
 std::uintptr_t owner2c_{},reference_character30_{};
 std::int32_t character_flags34_{1},object_filter38_{0},sort_{1};
 bool destroyed_{};
 static bool clear_backup(std::vector<TargetBackupObjectBorrowV93>& source,std::string& e){
  // _Clear4a19f8 touches actual byte29 in original vector order, then resets
  // end to begin without freeing capacity. No virtual/gameplay delivery.
  for(const auto& object:source)if(!object.owner||!object.identity||!object.references29){e="Required SAME live backup GameObject reference29";return false;}
  for(auto& object:source)if(*object.references29)--*object.references29;
  source.clear();e.clear();return true;
 }
public:
 // Native GameObjectC2 38c518..57c: ownerNULL, flags1,filter0,sortClosest1.
 GameObjectTargetListResourceV93()=default;
 GameObjectTargetListResourceV93(const GameObjectTargetListResourceV93&)=delete;
 std::deque<target_search::Target24>& source_primary_v93()noexcept{return primary_;}
 std::map<std::string,std::vector<TargetBackupObjectBorrowV93>>& source_backups_v93()noexcept{return backups_;}
 std::uintptr_t owner_v93()const noexcept{return owner2c_;}
 std::uintptr_t reference_character_v93()const noexcept{return reference_character30_;}
 std::int32_t character_flags_v93()const noexcept{return character_flags34_;}
 std::int32_t object_filter_v93()const noexcept{return object_filter38_;}
 std::int32_t sort_v93()const noexcept{return sort_;}
 // SetRefObject4a191c. NULL leaves fields unchanged. Store owner BEFORE
 // genuine virtual24, then reread owner on true; false retains old char30.
 bool set_ref_object(std::uintptr_t object,const std::function<bool(bool&,std::string&)>& is_character,std::string& e){
  if(destroyed_){e="Actual TargetList resource already destroyed";return false;}if(!object){e.clear();return true;}
  owner2c_=object;bool character{};if(!is_character||!is_character(character,e)){if(e.empty())e="Required actual TargetList virtual24 IsCharacter";return false;}if(character)reference_character30_=owner2c_;return true;
 }
 bool flush_backup_results(std::string& e){
  if(destroyed_){e="Actual TargetList resource already destroyed";return false;}
  for(auto& entry:backups_)if(!clear_backup(entry.second,e))return false;
  e.clear();return true; // keys/nodes and vector capacities genuinely retained
 }
 bool destroy(std::string& e){
  if(destroyed_){e="TargetListD1 cannot replay completed source destruction";return false;}
  //38d144 _M_erase: recurse right, node, left = descending key, independent
  // of balancing. Each BackupObjectList _Clear precedes its vector/string
  // and map-node release. The actor owner remains pinned by actual journals.
  while(!backups_.empty()){auto current=std::prev(backups_.end());if(!clear_backup(current->second,e))return false;std::vector<TargetBackupObjectBorrowV93>{}.swap(current->second);backups_.erase(current);}
  //TargetInfo is source-trivial; D1 then releases actual deque blocks/map.
  std::deque<target_search::Target24>{}.swap(primary_);destroyed_=true;e.clear();return true;
 }
 bool destroyed()const noexcept{return destroyed_;}
};
struct PfPathStorageBorrowV93 {
 std::shared_ptr<void> owner;
 // Actual existing Main buffer, not a second path, scratch or synthetic ptr.
 std::vector<navigation::PathSegment>* segments{};
 std::function<bool(std::string&)> release_storage;
};
struct PfObjectDestructionServicesV93 {
 std::shared_ptr<void> owner; // independent actual Main resource authority
 std::function<bool(actor::RuntimeState&,std::shared_ptr<void>&,const navigation::CollisionWorld*&,navigation::ObstacleRegistry*&,std::uint64_t&,std::string&)> borrow_world;
 std::function<bool(actor::RuntimeState&,PfPathStorageBorrowV93&,std::string&)> borrow_path_storage;
 std::function<bool(actor::RuntimeState&,std::string&)> require_delivery_barrier;
};
// Remaining PFObject resource fields over SAME runtime.object/runtime.path.
// The existing navigation/FindPath algorithms and Main buffer owners remain
// the only producers. No actor/PF/target/World registry is constructed here.
class GameObjectPfResourceV93 final {
 actor::RuntimeState& runtime_;
 std::string debug8c_; // exact source C1 CString literal8f29e9 = empty
 bool constructed_{},drop_done_{},obstacle_done_{},debug_done_{},destroyed_{};
 PfPathStorageBorrowV93 buffer_;
public:
 explicit GameObjectPfResourceV93(actor::RuntimeState& actual):runtime_(actual){}
 bool construct(std::string& e){
  if(constructed_){e="PFObjectC1 resource producer cannot replay";return false;}
  // Existing port's semantic views, not an ARM memcpy or opaque extra words.
  // PF C1: path-list38 empty; owned-prefix7c0; target40/44/48 and waypoint
  // scalar slots0; flags14=2/radius8=1. Engineering buffer metadata startsNULL0.
  runtime_.path={};runtime_.path.route.flags=2;runtime_.path.route.radius=1;
  constructed_=true;e.clear();return true;
 }
 std::string& source_debug8c_v93()noexcept{return debug8c_;}
 actor::RuntimeState& runtime()noexcept{return runtime_;}
 bool destroy(const PfObjectDestructionServicesV93& source,std::string& e){
  if(!constructed_||destroyed_){e="Required actual live constructed PFObject resource";return false;}
  if(!source.owner||!source.require_delivery_barrier||!source.require_delivery_barrier(runtime_,e)){if(e.empty())e="Required actual PF frame/path delivery barrier";return false;}
  auto& path=runtime_.path;
  if(path.segments&&!buffer_.owner){
   PfPathStorageBorrowV93 reached;
   if(!source.borrow_path_storage||!source.borrow_path_storage(runtime_,reached,e)||!reached.owner||!reached.segments||reached.segments->data()!=path.segments||reached.segments->size()!=path.capacity||!reached.release_storage){if(e.empty())e="Required SAME existing Main PF path buffer owner";return false;}
   buffer_=std::move(reached); // commit only an admitted SAME actual storage loan
  }
  if(!drop_done_){
   // Actual existing bounded segment producer represents copied owned edges
   // in its SAME PathSegment buffer. It does not expose foreign sparse edges
   // or invent graph virtual20 destruction. Native malformed views fail.
   if(dh2_nav_drop_path(&path)){e="Existing SAME PFWorld.DropPath mapped kernel failed";return false;}drop_done_=true;
  }
  if(!obstacle_done_){
   std::shared_ptr<void> world;const navigation::CollisionWorld* geometry{};navigation::ObstacleRegistry* registry{};std::uint64_t key{};
   if(!source.borrow_world||!source.borrow_world(runtime_,world,geometry,registry,key,e)||!world||!geometry||!registry||!key){if(e.empty())e="Required SAME existing PF/navigation world and receiver key";return false;}
   navigation::ObstacleInitRequest request{geometry,registry,&runtime_.object,key,0.f,0.f,0,0};
   if(dh2_nav_init_obstacle(&request)){e="Existing SAME PFWorld.InitObstacle(false,0,0) failed";return false;}obstacle_done_=true;
  }
  if(!debug_done_){std::string{}.swap(debug8c_);debug_done_=true;}
  // Original PF D1 frees path-list storage38 AFTER debug CString8c.
  if(buffer_.owner){if(!buffer_.release_storage(e))return false;if(path.segments||path.capacity||path.count||path.owned){e="Main PF path buffer storage/aliases not genuinely retired";return false;}buffer_={};}
  else if(path.segments||path.capacity||path.count||path.owned){e="PF path storage without actual owner";return false;}
  destroyed_=true;e.clear();return true;
 }
 bool destroyed()const noexcept{return destroyed_;}
};
struct GameObjectEmbeddedResourcesV93 {
 GameObjectPfResourceV93 pf;
 GameObjectTargetListResourceV93 targets304;
 explicit GameObjectEmbeddedResourcesV93(actor::RuntimeState& actual):pf(actual){}
};
}
