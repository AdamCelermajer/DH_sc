#include "native_menu_preview_character_services_v122.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "actual_device_android_v54.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "canonical_character_pf_v62.hpp"
#include "character_candidate_cache_v62.hpp"
#include "application_services_owner_v5.hpp"
#include "native_scene_lights_v113.hpp"
#include "visual_fx_manager_libraries_v63.hpp"
#include "../../../../../engine-audio/integration-v42/application/native_audio_application_v42.hpp"
#include <map>

namespace model_renderer {
bool borrow_application_frame_count_v68(std::uint32_t&,std::string&);
namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
using Visual=dh2::world::RetainedGameObjectVisualV1;
// Source PFWorld C1 522e2c writes its room vector and bounds14..28 to zero.
// These are constructor-produced process fields, not a loaded Level/floor.
// Preview SetupScene loads only PhysicalWorld; PF.GetFloorHeightAt525508
// therefore returns a genuine miss from the C1 room-vector guard.
struct PreviewResourcesV122 {
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 MenuPreviewProcessDomainV121 domain;
 dh2::navigation::CollisionWorld pf_geometry{};
 dh2::navigation::ObstacleRegistry pf_obstacles{};
 std::map<std::uintptr_t,std::weak_ptr<Record>> records;
 bool current(std::string& e)const{
  auto app=application.lock();
  if(!app||!domain.owner||!domain.scene||!domain.objects||!domain.character_cache||!domain.character_cache->ready()){
   e="Retired actual process preview resource receivers";return false;
  }
  e.clear();return true;
 }
 bool observe(Record& r,std::string& e){
  if(!current(e)||!r.actor||!r.actor->object||r.services.world!=domain.owner||r.services.canonical_objects!=domain.objects){
   if(e.empty())e="Preview resource callback addressed another canonical Character";return false;
  }
  auto& cell=records[r.actor->object->identity];auto previous=cell.lock();
  if(previous&&previous.get()!=&r){e="Preview Character identity reused before resource retirement";return false;}
  cell=r.shared_from_this();return true;
 }
 bool visual(Record& r,std::uintptr_t id,std::shared_ptr<Visual>& out,std::string& e){
  if(!observe(r,e))return false;out=r.visual?r.visual->visual():nullptr;
  if(!out||r.actor->source_visual()!=id||reinterpret_cast<std::uintptr_t>(out.get())!=id){e="Required SAME preview Character VisualObject2d8";return false;}
  e.clear();return true;
 }
 bool update_pf(Record& r,std::string& e){
  if(!observe(r,e))return false;
  return dh2::world::canonical_character_update_pf_v62(r,pf_geometry,pf_obstacles,e);
 }
 bool peer(void* physical,std::uintptr_t& id,std::string& e){
  id=0;if(!physical){e.clear();return true;} //PhysicalObject.owner8 genuine NULL.
  if(!current(e))return false;
  for(auto at=records.begin();at!=records.end();){auto r=at->second.lock();if(!r){at=records.erase(at);continue;}
   if(r->physical_owner_v62&&r->physical_owner_v62.get()==physical){id=r->actor->object->identity;e.clear();return true;}++at;}
  e="Required typed process PhysicalObject owner8 association";return false;
 }
 bool enabled(std::uintptr_t id,std::uint8_t& value,std::string& e){
  auto at=records.find(id);auto r=at==records.end()?nullptr:at->second.lock();
  const auto* field=r&&r->actor?r->actor->source_bool_field(0x80):nullptr;
  if(!field){e="Required SAME process Character ObjectBase80 source cell";return false;}value=*field;e.clear();return true;
 }
 bool visible(Record& r,bool requested,std::string& e){
  if(!observe(r,e))return false;
  std::uint8_t stored{};if(!r.actor->source_set_visible80_v96(requested,stored,e))return false;
  const auto id=r.actor->source_visual();if(!id){e.clear();return true;}
  std::shared_ptr<Visual> v;if(!visual(r,id,v,e))return false;
  bool local=stored!=0,player{};if(local&&!r.is_player(player,e))return false;
  if(local&&!player){
   auto ai=r.properties->resolved[1];const auto* rows=r.design.ai();
   if(!rows||rows->rows.size()<=8){e="Required source preview IsZonable AI table";return false;}
   if(ai<0||std::size_t(ai)>=rows->rows.size())ai=8;
   if(rows->rows[std::size_t(ai)].type!=3){
    const auto* zoned=r.actor->source_bool_field(0x2ee);
    if(!zoned){e="Required SAME preview zoning2ee field";return false;}
    if(*zoned){const auto* inside=r.actor->source_bool_field(0x2f0);if(!inside){e="Required SAME preview membership2f0 field";return false;}local=*inside!=0;}
   }
  }
  return v->set_root_local_visibility_v3(local,e);
 }
};
std::map<std::uintptr_t,std::weak_ptr<PreviewResourcesV122>> process_resources_v122;
bool debug_query(Record& r,const char* key,std::uint32_t& value,std::string& e){
 if(!r.services.debug||!r.services.debug_files||dh2_character_debug_load(r.services.debug,r.services.debug_files)!=1||
    dh2_character_debug_get(&value,r.services.debug,key,r.services.debug_files)!=1){e="Required actual process Character Debug query";return false;}
 e.clear();return true;
}
}
bool compose_native_menu_preview_character_resources_v122(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const MenuPreviewProcessDomainV121& d,dh2::world::CanonicalCharacterCandidateServicesV60& s,std::string& e){
 if(!app||!d.owner||!d.scene||!d.objects||!d.properties||!d.physical||!d.design||!d.skills||
    !d.character_cache||!d.character_cache->ready()||!d.debug||!d.debug_files||!d.read||!d.animation_manager){
  e="Required complete SAME process preview Character resources";return false;
 }
 auto& slot=process_resources_v122[app->identity()];auto resources=slot.lock();
 if(resources&&(resources->application.lock()!=app||resources->domain.owner!=d.owner||resources->domain.scene!=d.scene||resources->domain.objects!=d.objects)){
  e="Process preview resource domain replaced before source release";return false;
 }
 if(!resources){resources=std::make_shared<PreviewResourcesV122>();resources->application=app;resources->domain=d;resources->domain.character_services={};slot=resources;}
 auto cache=d.character_cache;
 s.world=d.owner;s.design=d.design.get();s.canonical_objects=d.objects;s.physical_world=d.physical;
 s.models=&cache->models();s.animation_tables=&cache->animation_tables();s.loot_tables=&cache->loot();s.skills=d.skills->borrow();
 s.character_templates_v78=cache->templates_v78();s.debug=d.debug.get();s.debug_files=d.debug_files;
 s.state=[resources](Record& r,auto& out,std::string& e){
  // FSM services are borrowed before CandidateActor.construct_fields publishes
  // object/machine. The later inherited-resource boundary observes that object.
  if(!resources->current(e)||!r.actor||r.services.world!=resources->domain.owner||
   r.services.canonical_objects!=resources->domain.objects){
   if(e.empty())e="Required SAME process Character resource loan at FSM C1";return false;
  }
  return bind_process_character_fsm_v121(r,resources->application.lock(),resources,
   resources->pf_geometry,resources->pf_obstacles,out,e);
 };
 s.model_name.high_performance=borrow_actual_device_high_performance_v54;
 if(!s.initial_height)s.initial_height=[resources](const float* p,bool& found,float& height,std::string& e){
  if(!resources->current(e))return false;dh2::navigation::HeightHit hit{};
  const auto result=dh2_nav_world_height(&hit,&resources->pf_geometry,p,0);
  if(result<0){e="Actual process PF floor-height query rejected its inputs";return false;}found=result!=0;if(found)height=hit.height;e.clear();return true;
 };
 const auto previous_inherited=s.inherited;
 s.inherited=[resources,previous_inherited](Record& r,auto& out,std::string& e){
  if(!resources->observe(r,e)||(previous_inherited&&!previous_inherited(r,out,e)))return false;
  out.owner=resources;
  std::weak_ptr<Record> weak=r.shared_from_this();
  if(!out.device_high_performance)out.device_high_performance=borrow_actual_device_high_performance_v54;
  if(!out.set_visible)out.set_visible=[resources,weak](bool value,std::string& e){auto same=weak.lock();if(!same){e="Released preview visibility receiver";return false;}return resources->visible(*same,value,e);};
  if(!out.update_pf_object)out.update_pf_object=[resources,weak](std::string& e){auto same=weak.lock();if(!same){e="Released preview PF receiver";return false;}return resources->update_pf(*same,e);};
  if(!out.init_pf_object)out.init_pf_object=[resources,weak](bool,const float* p,float radius,std::uintptr_t id,std::string& e){
   auto same=weak.lock();if(!same||!resources->observe(*same,e)||id!=same->actor->object->identity){if(e.empty())e="Required SAME preview PF.InitObject receiver";return false;}
   return dh2::world::canonical_character_init_pf_v62(*same,resources->pf_geometry,p,radius,e);
  };
  if(!out.light_set_id)out.light_set_id=[resources](const std::string& name,std::int32_t& id,std::string& e){if(!resources->current(e))return false;id=resources->domain.scene->source_light_names_v89()->get_id(name);e.clear();return true;};
  if(!out.visual_set_light_set)out.visual_set_light_set=[resources,weak](auto id,auto set,std::string& e){auto r=weak.lock();std::shared_ptr<Visual> v;if(!r||!resources->visual(*r,id,v,e))return false;v->store_light_set(set);e.clear();return true;};
  if(!out.visual_root)out.visual_root=[resources,weak](auto id,auto& root,std::string& e){auto r=weak.lock();std::shared_ptr<Visual> v;if(!r||!resources->visual(*r,id,v,e))return false;root=v->root_identity();e.clear();return true;};
  if(!out.node_from_name)out.node_from_name=[resources,weak](auto root,const char* name,auto& node,std::string& e){auto r=weak.lock();std::shared_ptr<Visual> v;if(!r||!resources->visual(*r,r->actor->source_visual(),v,e))return false;if(root!=v->root_identity()){e="Different preview visual root at named-node query";return false;}return v->node_from_name(name,node,e);};
  e.clear();return true;
 };
 const auto previous_visual=s.visual;
 s.visual=[resources,cache,previous_visual](Record& r,auto& out,std::string& e){
  if(!resources->observe(r,e)||(previous_visual&&!previous_visual(r,out,e)))return false;
  out.world=resources->domain.owner;out.scene_manager=resources->domain.scene;out.animation_manager=resources->domain.animation_manager;
  out.visual.owner=resources->domain.owner;
  out.visual.read_asset=[cache](const auto& path,auto& bytes,bool& found,std::string& e){return cache->files().read(path,found,bytes,e);};
  out.animation_dictionary=&cache->animations();
  out.animation_read=[cache](const auto& path,auto& bytes,std::string& e){bool found{};if(!cache->files().read(path,found,bytes,e))return false;if(!found){e="Missing authored preview Character animation: "+path;return false;}e.clear();return true;};
  std::weak_ptr<Record> weak=r.shared_from_this();
  out.update_pf=[resources,weak](std::string& e){auto r=weak.lock();if(!r){e="Released preview visual/PF receiver";return false;}return resources->update_pf(*r,e);};
  out.add_animation_trace_v116=[weak](std::string& e){auto r=weak.lock();std::uint32_t value{};if(!r){e="Released preview AnimSet Debug receiver";return false;}return debug_query(*r,"isTracingAnimSetManager",value,e);};
  auto sound=std::make_shared<dh2::audio::AudioApplicationBorrowV42>();
  out.registration.current_sound_manager=[sound](bool& present,std::string& e){if(!dh2::android_audio::borrow_application_audio_v42(*sound,e))return false;present=bool(sound->manager);e.clear();return true;};
  out.registration.invoke=[resources,weak,sound](const auto& q,std::string& e){
   using O=dh2::character::NpcAnimationRegistrationOperationV6;auto r=weak.lock();if(!r||!resources->current(e)){if(e.empty())e="Released preview animation registration receiver";return false;}
   if(q.operation==O::debug_load){if(dh2_character_debug_load(r->services.debug,r->services.debug_files)!=1){e="Original preview Character Debug load failed";return false;}e.clear();return true;}
   if(q.operation==O::debug_trace){std::uint32_t value{};return debug_query(*r,"isTracingChar_Init",value,e);}
   if(q.operation==O::preload_sound){auto captured=std::move(*sound);*sound={};if(!captured.manager){e="Required reached preview LoadSound receiver";return false;}return captured.precache_raw_uid(q.resource,e);}
   if(q.operation==O::preload_fx){auto app=resources->application.lock();auto libraries=app?app->source_fx_libraries_v63():nullptr;if(!libraries){e="Required SAME process FX animation-library registration";return false;}return libraries->register_set_to_load(q.resource,e);}
   e="Animation AddAnim/AddTemplate must execute on SAME retained CharAnimator";return false;
  };
  e.clear();return true;
 };
 const auto previous_position=s.position;
 s.position=[resources,previous_position](Record& r,auto& out,std::string& e){
  if(!resources->observe(r,e)||(previous_position&&!previous_position(r,out,e)))return false;out.world=resources->domain.owner;
  std::weak_ptr<Record> weak=r.shared_from_this();
  if(!out.physical_body)out.physical_body=[weak](auto id,auto*& body,std::string& e){auto r=weak.lock();if(!r||!r->physical_owner_v62||id!=reinterpret_cast<std::uintptr_t>(r->physical_owner_v62.get())){e="Required SAME preview PhysicalObject2dc";return false;}body=&r->physical_owner_v62->native();e.clear();return true;};
  if(!out.visual_sync_position)out.visual_sync_position=[weak](auto id,std::string& e){auto r=weak.lock();if(!r||!r->visual){e="Released preview position VisualObject";return false;}return r->visual->sync_visual_position_v7(id,e);};
  e.clear();return true;
 };
 //The script/FSM compositor supplies the selected collision dispatcher. This
 //resource wrapper supplies the real body/PF/Debug/peer endpoints, without
 //changing that dispatcher's identity or substituting a gameplay World.
 const auto previous_physical=s.physical;
 s.physical=[resources,previous_physical](Record& r,auto& collision,auto& out,std::string& e){
  if(!resources->observe(r,e))return false;
  if(previous_physical&&!previous_physical(r,collision,out,e))return false;
  if(!collision.receiver||!collision.filter||!collision.event){e="Required SAME preview selected AIS/FSM physical collision dispatcher";return false;}
  out.context=resources.get();
  out.peer_owner=[](void* raw,void* physical,std::uintptr_t& id,std::string& e){return static_cast<PreviewResourcesV122*>(raw)->peer(physical,id,e);};
  out.enabled80=[](void* raw,std::uintptr_t id,std::uint8_t& value,std::string& e){return static_cast<PreviewResourcesV122*>(raw)->enabled(id,value,e);};
  //Debug callbacks need the actual receiver, not a cast of the process owner.
  //The object identity is resolved from this transport's weak canonical loans.
  out.debug_switch=[](void* raw,const char* name,bool& value,std::string& e){
   auto& p=*static_cast<PreviewResourcesV122*>(raw);if(!p.current(e))return false;
   return dh2::character::character_npc_physical_debug_v1(*p.domain.debug,*p.domain.debug_files,name,value,e);
  };
  out.update_pf=[](void* raw,std::uintptr_t id,const dh2::physical::NativeBody* body,const dh2::physical::NpcBodyProjection*,std::string& e){
   auto& p=*static_cast<PreviewResourcesV122*>(raw);auto at=p.records.find(id);auto r=at==p.records.end()?nullptr:at->second.lock();
   if(!r||!r->physical_owner_v62||body!=&r->physical_owner_v62->native()){e="Required SAME preview body at physical assignment/PF update";return -1;}
   return p.update_pf(*r,e)?0:-1;
  };
  e.clear();return true;
 };
 if(!s.failed_spawn_visible)s.failed_spawn_visible=[resources](Record& r,std::string& e){return resources->visible(r,false,e);};
 if(!s.failed_spawn_mark)s.failed_spawn_mark=[resources](Record& r,std::string& e){return resources->observe(r,e)&&resources->domain.objects->source_mark_for_deletion_v89(r.actor->object->identity,e);};
 e.clear();return true;
}
bool initialize_native_menu_preview_character_light_material_v122(Record& r,const MenuPreviewProcessDomainV121& d,std::string& e){
 if(!d.owner||!d.scene||r.services.world!=d.owner||!r.actor){e="Required SAME process Character light/material domain";return false;}
 const auto id=r.actor->source_visual();if(!id){e.clear();return true;} //3b4860 genuine NULL2d8.
 auto v=r.visual?r.visual->visual():nullptr;
 if(!v||reinterpret_cast<std::uintptr_t>(v.get())!=id){e="Required SAME preview visual for InitLightSetAndMaterial";return false;}
 auto names=d.scene->source_light_names_v89();auto lights=d.scene->source_light_runtime_v113();
 if(!lights){lights=std::make_shared<dh2::world::NativeLightSetV113>(names);if(!d.scene->publish_light_runtime_v113(lights,e))return false;}
 if(lights->names()!=names){e="Different process Scene light-name authority";return false;}
 std::vector<bool> filter;dh2::world::NativeLightSetV113::initialize_filter(filter,true);
 v->store_light_set(names->get_id("MonsterLight"));v->source_light_filter44_v113()=filter;
 return v->source_apply_light_set_v113(*lights,e)&&v->source_apply_material_tail_v113(e);
}
}
