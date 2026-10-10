#include "source_campaign_scene_pf_release_v106.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_release_v88.hpp"
#include "source_campaign_startup_abort_v114.hpp"
#include "source_campaign_admission_v104.hpp"
#include "model_renderer.hpp"
#include <level_pf_release_binding_v1.hpp>
#include <retained_map_mesh_v93.hpp>
#include <campaign_navigation_registry_v64.hpp>
#include <physical_world.hpp>
#include <character_design_services.hpp>
#include <gameplay_skybox_material_v25.hpp>
namespace model_renderer {namespace {
template<class A,class B>bool same(const std::shared_ptr<A>& a,const std::shared_ptr<B>& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
struct ScenePfReleaseV106 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::world::ModulePFRoomsV3> rooms;
 std::weak_ptr<dh2::world::SceneManagerMapOwnerV2> map;
 bool lazy_pf_v115{};
 bool scope(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  auto expected=world.lock();
  if(!expected||!borrow_source_campaign_release_candidate_v115(c,w,e)||!same(w,expected)||
     !same(c.actual_world,w->owner)||!c.roots||!require_source_campaign_quiescence_v104(w->owner,e)){
   if(e.empty())e="Required SAME quiescent Scene/PF release graph";return false;
  }
  // Failed candidate construction may precede its public projection. These
  // are the actual retained C1 prefixes, never inferred from empty lists.
  auto r=c.module_rooms_v69.lock();auto m=c.map_owner_v69.lock();
  auto retained_r=rooms.lock();auto retained_m=map.lock();
  if(!retained_r)retained_r=w->source_pf_rooms_v115;
  if(!retained_m)retained_m=w->source_pf_map_v115;
  if((r&&retained_r&&!same(r,retained_r))||(m&&retained_m&&!same(m,retained_m))||
     (c.floors&&w->source_pf_floors_v115&&!same(c.floors,w->source_pf_floors_v115))||
     (c.navigation_registry&&w->source_pf_navigation_v115&&!same(c.navigation_registry,w->source_pf_navigation_v115))){
   e="Scene/PF release prefix was replaced";return false;
  }
  if(!r)r=retained_r;if(!m)m=retained_m;
  if(!c.floors)c.floors=w->source_pf_floors_v115;
  if(!c.navigation_registry)c.navigation_registry=w->source_pf_navigation_v115;
  if(r&&(!m||!same(r->map_owner_v69(),m)||!same(r->world(),c.floors)||!r->native_storage_v106())){
   e="Actual PF C1 differs from retained floor/map prefix";return false;
  }
  if((!r||!m||!c.floors||!c.navigation_registry)&&!lazy_pf_v115){
   e="Missing actual Scene/PF release producer";return false;
  }
  c.module_rooms_v69=r;c.map_owner_v69=m;
  return true;
 }
};
template<class T>void free_storage(T& value){T{}.swap(value);}
}
bool bind_source_campaign_scene_pf_release_v106(const SourceCampaignCandidateBorrowV55& c,
 dh2::loader::LevelDestroyServicesV1& d,std::string& e){
 using namespace dh2;using namespace loader;
 std::shared_ptr<SourceWorldBorrowV61> w;auto rooms=c.module_rooms_v69.lock();auto map=c.map_owner_v69.lock();
 SourceCampaignCandidateBorrowV55 observed;
 if(!borrow_source_campaign_release_candidate_v115(observed,w,e)||!same(c.actual_world,observed.actual_world)||!same(c.level,observed.level)||!c.roots)return false;
 if(!rooms)rooms=w->source_pf_rooms_v115;if(!map)map=w->source_pf_map_v115;
 const auto floors=c.floors?c.floors:w->source_pf_floors_v115;
 const auto navigation=c.navigation_registry?c.navigation_registry:w->source_pf_navigation_v115;
 if((c.floors&&w->source_pf_floors_v115&&!same(c.floors,w->source_pf_floors_v115))||
    (c.navigation_registry&&w->source_pf_navigation_v115&&!same(c.navigation_registry,w->source_pf_navigation_v115))||
    (c.module_rooms_v69.lock()&&w->source_pf_rooms_v115&&!same(c.module_rooms_v69.lock(),w->source_pf_rooms_v115))||
    (c.map_owner_v69.lock()&&w->source_pf_map_v115&&!same(c.map_owner_v69.lock(),w->source_pf_map_v115))){
  e="Existing candidate PF prefix differs from native retention";return false;
 }
 const bool lazy=!rooms||!map||!floors||!navigation;
 if(lazy){SourceCampaignStartupPrefixV114 prefix;
  if(!borrow_source_campaign_startup_prefix_v114(prefix,e)||!prefix.level_c1_complete||
     !same(prefix.world,w)||!same(prefix.published_level34,c.level)){
   if(e.empty())e="Lazy PF C1 requires the actual failed completed Level constructor";return false;
  }
 }
 if(rooms&&(!rooms->native_storage_v106()||!same(rooms->world(),floors)||!same(rooms->map_owner_v69(),map)))return false;
 if(rooms&&!navigation){e="Produced PF owner is missing its actual native floor-object registry";return false;}
 if(d.scene||d.scene_virtual68||d.scene_active_camera_null||d.pf_world||d.pf_flush||d.physical||d.physical_clear){
  e="Scene/PF release leaves already bound to another transport";return false;
 }
 if(!w->source_pf_floors_v115)w->source_pf_floors_v115=floors;
 if(!w->source_pf_map_v115)w->source_pf_map_v115=map;
 if(!w->source_pf_navigation_v115)w->source_pf_navigation_v115=navigation;
 if(!w->source_pf_rooms_v115)w->source_pf_rooms_v115=rooms;
 auto t=std::make_shared<ScenePfReleaseV106>();t->world=w;t->rooms=rooms;t->map=map;t->lazy_pf_v115=lazy;
 world::SceneMapDestructionV1 map_services;map_services.owner=t;
 map_services.quiesce=[t](auto& actual,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return t->scope(c,w,e)&&c.map_owner_v69.lock().get()==&actual;};
 map_services.native_map_d1_prefix=[](auto& map,auto& e){return map.native_d1_prefix_v106(e);};
 map_services.native_map_d1_tail=[](auto& map,auto& e){return map.native_d1_tail_v106(e);};
 map_services.child_parent_drop=[t](const world::SceneMapNodeBorrowV2& child,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!t->scope(c,w,e)||!child.owner||!child.parentec||*child.parentec)return false;
  if(child.source_mesh_v93){
   if(child.identity!=reinterpret_cast<std::uintptr_t>(child.source_mesh_v93.get())||!same(child.owner,child.source_mesh_v93)){e="Map child mesh lease differs from actual receiver";return false;}
   return world::retire_retained_map_mesh_v106(child.source_mesh_v93,e);
  }
  // PF clone's actual mesh40 lease survives this parent-reference drop.
  // The map transport/closures have really been erased by the source journal.
  auto rooms=c.module_rooms_v69.lock();if(!rooms){e="Missing actual PF rooms for map child drop";return false;}
  for(const auto& room:rooms->rooms())if(room)for(const auto& clone:room->clones)
   if(clone&&clone->identity()==child.identity&&same(child.owner,clone)&&!clone->parent()){e.clear();return true;}
  e="Unrecognized actual map child: no qualified native reference-drop owner";return false;
 };
 world::PFWorldFlushServicesV1 pf;pf.owner=t;
 pf.require_quiescent=[t](auto& actual,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  auto map=actual.map_owner_v69();
  return t->scope(c,w,e)&&c.module_rooms_v69.lock().get()==&actual&&map&&same(c.map_owner_v69.lock(),map)&&map->children().empty();};
 pf.actual_fields=[t](auto& floors,auto id,auto& out,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->scope(c,w,e)||!c.navigation_registry)return false;
  const auto weak=std::weak_ptr<navigation::CampaignNavigationRegistryV64>(c.navigation_registry);
  auto rooms=c.module_rooms_v69.lock();if(!rooms){e="Missing actual PF C1 storage";return false;}
  return rooms->native_storage_v106()->lend(floors,id,[weak](auto& e){auto registry=weak.lock();if(!registry){e="Retired actual PF floor-object storage";return false;}return registry->erase_floor_objects_v106(e);},out,e);
 };
 pf.floor_aux68_d1=[](floors::Record& floor,std::string& e){
  // Native octree/selector scratch is this PFFloor's actual auxiliary domain.
  // The clone still pins original collision triangles until its mesh40 D1.
  floor.tree={};floor.workspace={};free_storage(floor.octants);free_storage(floor.indices);free_storage(floor.scratch);
  e.clear();return true;
 };
 pf.floor_tail_d1=[](floors::Record& floor,std::string& e){
  free_storage(floor.selected);free_storage(floor.selected_ids);free_storage(floor.retained);free_storage(floor.triangles);free_storage(floor.name);e.clear();return true;
 };
 pf.clone.owner=t;
 pf.clone.require_unpublished=[t](auto& clone,auto& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->scope(c,w,e)||clone.parent())return false;
  auto map=c.map_owner_v69.lock();if(!map||!map->children().empty())return false;e.clear();return true;};
 pf.clone.native_mesh_d1=[](auto& clone,auto& e){return clone.native_mesh_d1_v106(e);};
 // The ordinary journal is immediate. A truly unproduced singleton gets its
 // genuine C1 and SAME journal only at the later original PF GetInstance.
 auto journal=std::make_shared<std::shared_ptr<LevelPFReleaseBindingV1>>();
 if(rooms&&map&&navigation)*journal=std::make_shared<LevelPFReleaseBindingV1>(c.level,rooms,map,map_services,pf);
 d.scene=[t](const LevelReleaseReceiverV1& app,LevelReleaseReceiverV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->scope(c,w,e)||!same(app.owner,w->application)||app.identity!=w->application->identity()||!c.roots)return false;
  out={c.roots,c.roots->scene_manager_identity_v16()};e.clear();return true;};
 d.scene_virtual68=[t,journal](const LevelReleaseReceiverV1& scene,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->scope(c,w,e)||!same(scene.owner,c.roots)||scene.identity!=c.roots->scene_manager_identity_v16())return false;
  // SceneManager's own skybox438 reference is separate from the root's
  // parent reference. Retire both with its qualified native loader receipt.
  if(w->skybox_v124){if(!w->skybox_v124->release(e))return false;w->skybox_v124.reset();}
  // Original ObjectManager.Flush/class D0 already removed ordinary visuals.
  //Scene virtual68 drops actual remaining parent references: map group or
  //intrusive batch root. Unknown hookless class roots remain a source error.
  for(auto id:c.roots->roots()){bool handled{};if(*journal&&!(*journal)->scene_child(scene,id,handled,e))return false;
   if(!handled){world::GameObjectSceneRootBorrowV1 actual;
    if(!c.roots->borrow_registered_root_v110(id,actual,e)||!actual.acquire_parent_reference_v110||!actual.release_parent_reference_v110){
     if(e.empty())e="Actual Scene still contains a class root without its qualified D1 receipt";return false;
    }
    //Compiler D1 dropped its own34 reference. The separate Scene parent
    //drop now invokes the actual node D1 if this is its final native reference.
    if(!c.roots->remove_root_parent_reference_v1(id,e))return false;
   }
  }
  return c.roots->retire_source_cached_aliases_v106(e);
 };
 d.scene_active_camera_null=[t](const LevelReleaseReceiverV1& scene,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return t->scope(c,w,e)&&same(scene.owner,c.roots)&&scene.identity==c.roots->scene_manager_identity_v16()?c.roots->set_active_camera_v13({},e):false;};
 d.pf_world=[t,journal,map_services,pf](LevelReleaseReceiverV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->scope(c,w,e))return false;
  auto rooms=c.module_rooms_v69.lock();auto map=c.map_owner_v69.lock();
  if(!rooms){
   if(!t->lazy_pf_v115){e="PF GetInstance lost its produced native owner";return false;}
   // Source GetInstance is unconditional in Level D1. Construct only a truly
   // unproduced owner, on the SAME Scene; retain each allocation prefix.
   if(!w->source_pf_floors_v115)w->source_pf_floors_v115=std::make_shared<dh2::floors::World>();
   if(!map){map=std::make_shared<world::SceneManagerMapOwnerV2>(c.roots);w->source_pf_map_v115=map;}
   if(!w->source_pf_navigation_v115)w->source_pf_navigation_v115=std::make_shared<navigation::CampaignNavigationRegistryV64>();
   world::ModulePFDebugV3 debug;debug.owner=t;
   debug.query=[weak=std::weak_ptr<SourceWorldBorrowV61>(w)](const char* key,bool& value,std::string& e){
    auto world=weak.lock();std::uint32_t observed{};
    if(!world||!world->debug||!key||dh2_character_debug_get(&observed,world->debug.get(),key,world->debug_files)!=1){
     e="Required actual PF Debug query";return false;
    }
    value=observed!=0;e.clear();return true;
   };
   debug.clock_ms=[](std::uint32_t& value,std::string& e){return borrow_application_time_v68(value,e);};
   rooms=std::make_shared<world::ModulePFRoomsV3>(w->source_pf_floors_v115,map,std::move(debug));
   w->source_pf_rooms_v115=rooms;t->rooms=rooms;t->map=map;
  }
  if(!*journal)*journal=std::make_shared<LevelPFReleaseBindingV1>(c.level,rooms,map,map_services,pf);
  auto owner=rooms->native_storage_v106();if(!owner){e="PF C1 did not publish actual native storage";return false;}
  out={owner,owner->identity()};e.clear();return true;
 };
 d.pf_flush=[journal](const LevelReleaseReceiverV1& receiver,std::string& e){
  if(!*journal){e="PF Flush reached before actual GetInstance/C1";return false;}return (*journal)->flush(receiver,e);
 };
 d.physical=[t](const LevelReleaseReceiverV1& app,LevelReleaseReceiverV1& out,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->scope(c,w,e)||!same(app.owner,w->application)||app.identity!=w->application->identity()||!c.physical_world)return false;
  out={c.physical_world,reinterpret_cast<std::uintptr_t>(c.physical_world.get())};e.clear();return true;};
 d.physical_clear=[t](const LevelReleaseReceiverV1& receiver,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!t->scope(c,w,e)||!same(receiver.owner,c.physical_world)||receiver.identity!=reinterpret_cast<std::uintptr_t>(c.physical_world.get()))return false;
  auto* backend=c.physical_world->backend();
  if(backend)for(auto* body=backend->GetBodyList();body;body=body->GetNext())if(body!=backend->GetGroundBody()){
   e="PhysicalWorld still owns a body lacking actual class/physical D1";return false;
  }
  c.physical_world->clear();e.clear();return true;
 };
 e.clear();return true;
}
}
