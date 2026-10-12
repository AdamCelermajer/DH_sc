#pragma once
#include "level_destroy_source_v1.hpp"
#include <module_pf_room_v3.hpp>
namespace dh2::loader {
// SAME first-SWAMP owners; no new PFWorld/global or whole-Scene callback.
// Retain this journal in the existing independent release composition. Its
// actual Level/rooms/map borrows remain weak; Main leaves capture scopes weakly.
class LevelPFReleaseBindingV1 final {
 std::weak_ptr<CanonicalLevelContextV1> level_;
 std::weak_ptr<world::ModulePFRoomsV3> rooms_;
 std::weak_ptr<world::SceneManagerMapOwnerV2> map_;
 world::SceneMapDestructionV1 map_services_;
 world::PFWorldFlushServicesV1 pf_services_;
 bool scope(std::shared_ptr<CanonicalLevelContextV1>& level,
  std::shared_ptr<world::ModulePFRoomsV3>& rooms,
  std::shared_ptr<world::SceneManagerMapOwnerV2>& map,std::string& e)const{
  level=level_.lock();rooms=rooms_.lock();map=map_.lock();
  if(!level||!rooms||!map||!rooms->world()||rooms->map_owner_v69().get()!=map.get()||rooms->map_owner_v69().owner_before(map)||map.owner_before(rooms->map_owner_v69())){e="Required SAME live Level/PF rooms/floor/map release journal";return false;}return true;
 }
public:
 LevelPFReleaseBindingV1(std::weak_ptr<CanonicalLevelContextV1> level,
  std::weak_ptr<world::ModulePFRoomsV3> rooms,std::weak_ptr<world::SceneManagerMapOwnerV2> map,
  world::SceneMapDestructionV1 map_services,world::PFWorldFlushServicesV1 pf_services):
  level_(std::move(level)),rooms_(std::move(rooms)),map_(std::move(map)),map_services_(std::move(map_services)),pf_services_(std::move(pf_services)){}
 // Main's genuine Scene virtual68 child walk calls this at the SAME map root
 // ordinal. A nonmap child is unhandled and must reach its real Main body; this
 // never accepts remaining root children or substitutes a whole Scene clear.
 bool scene_child(const LevelReleaseReceiverV1& scene,std::uintptr_t child,bool& handled,std::string& e){
  handled=false;std::shared_ptr<CanonicalLevelContextV1> level;std::shared_ptr<world::ModulePFRoomsV3> rooms;std::shared_ptr<world::SceneManagerMapOwnerV2> map;
  if(!scope(level,rooms,map,e))return false;
  if(!scene||scene.identity!=map->scene_manager_identity_v1()){e="Required SAME actual SceneManager for map child D1";return false;}
  if(child!=map->identity()){e.clear();return true;}handled=true;return map->release_scene_source_v1(map_services_,e);
 }
 bool flush(const LevelReleaseReceiverV1& actual_pf,std::string& e){
  std::shared_ptr<CanonicalLevelContextV1> level;std::shared_ptr<world::ModulePFRoomsV3> rooms;std::shared_ptr<world::SceneManagerMapOwnerV2> map;
  if(!scope(level,rooms,map,e))return false;
  if(!actual_pf){e="Required SAME actual PFWorld identity, not a replacement global";return false;}
  // Flush checks real PF identity/owner AND its SAME floor-storage projection.
  return rooms->flush_source_v1(pf_services_,actual_pf.owner,actual_pf.identity,e);
 }
};
inline bool bind_level_pf_flush_source_v1(LevelDestroyServicesV1& services,
 std::shared_ptr<LevelPFReleaseBindingV1> journal,std::string& e){
 if(!journal||services.pf_flush){e="Require unbound actual LevelD1 PF flush slot and SAME release journal";return false;}
 services.pf_flush=[journal=std::move(journal)](const LevelReleaseReceiverV1& actual,std::string& error){return journal->flush(actual,error);};e.clear();return true;
}
}
