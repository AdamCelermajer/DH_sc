#include "renderer_menu_preview_domain_v121.hpp"
#include "native_menu_preview_character_services_v122.hpp"
#include "native_menu_preview_fx_initialization_v122.hpp"
#include "native_menu_preview_profile_v122.hpp"
#include "native_menu_preview_player_v122.hpp"
#include "native_menu_preview_lifecycle_v123.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "source_process_objects_v121.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "character_candidate_cache_v62.hpp"
#include "character_properties_temp_global_v62.hpp"
#include "character_script_init_vitals.hpp"
#include "player_save_difficulty_global_v29.hpp"
#include "source_campaign_equipment_services_v118.hpp"
#include "source_item_resources_v88.hpp"
#include "visual_fx_manager_libraries_v63.hpp"
#include "owned_hud_settings_v1.hpp"
#include <algorithm>
#include <cstring>
#include <map>
namespace model_renderer {namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
struct PreviewServicesV122:std::enable_shared_from_this<PreviewServicesV122> {
 MenuPreviewProcessDomainV121 domain;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 dh2::character::CharacterScriptAssetsV1 scripts;
 std::shared_ptr<dh2::character::CharacterScriptObjects> objects;
 std::shared_ptr<dh2::character::SourceItemResourcesV88> items;
 std::unique_ptr<dh2::fx::DebugModules,void(*)(dh2::fx::DebugModules*)> modules{nullptr,dh2_fx_debug_modules_destroy};
 dh2::fx::PreloadServices16 preload{};
 std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> fx_scene;
 std::shared_ptr<dh2::fx::CharacterMeshFxOwnerV4> effects;
 dh2::character::HostPlayer8 player{};dh2::character::HostLevel8 level{};
 std::shared_ptr<void> host_player,host_level;
 std::vector<dh2::character::LevelRangeRow24> ranges;
 dh2::character::HostContextBindings16 host{{this,host_call}};
 dh2::character::LevelServices16 level_services{this,level_call};
 std::string error;
 static int host_call(void* raw,const dh2::character::HostContextRequest16* q,dh2::character::HostContextResponse16* out){
  auto& p=*static_cast<PreviewServicesV122*>(raw);auto app=p.application.lock();if(!app||!q||!out)return 1;
  if(q->service==dh2::character::host_get_player){auto pm=app->source_player_manager_v59();auto online=app->get_online_loading_v55();if(!pm||!online||online->byte5())return 1;
   dh2::player::PlayerInfoFieldsV1* info{};if(!pm->get_by_internal(0,false,info,p.error)||!info)return 1;auto fields=pm->network()->borrow(*info);if(!fields||!fields->managed_fields_produced_v70)return 1;
   p.host_player=fields;p.player.cached_level=fields->character_level330_v70;out->data=&p.player;return 0;
  }
  if(q->service==dh2::character::host_get_current_level){dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,p.error))return 1;
   out->data=nullptr;p.host_level.reset();if(current){p.host_level=current.level();const auto& f=current.level()->constructor_fields_v3();p.level={f.row3c,f.mode118};out->data=&p.level;}return 0;
  }
  if(q->service==dh2::character::host_get_range_rows){out->data=p.ranges.data();out->count=static_cast<std::uint32_t>(p.ranges.size());return 0;}return 1;
 }
 static int level_call(void* raw,dh2::character::LevelModel32*,const dh2::character::LevelRequest24* q){auto& p=*static_cast<PreviewServicesV122*>(raw);if(!q)return -1;
  if(q->service==dh2::character::level_debug_load)return dh2_character_debug_load(p.domain.debug.get(),p.domain.debug_files)==1?0:-1;
  if(q->service==dh2::character::level_debug_query){std::uint32_t word{};return q->name&&dh2_character_debug_get(&word,p.domain.debug.get(),q->name,p.domain.debug_files)==1?0:-1;}return -1;
 }
 bool fx(Record& r,std::string& e){
  if(effects)return true;
  auto visual=r.visual?r.visual->visual():nullptr;if(!visual||!domain.fx_libraries){e="Required actual process Character visual/FX library C1";return false;}
  fx_scene=visual;
  dh2::fx::MeshFxAssetsV1 assets{this,[](void* raw,const char* name,auto& bytes,auto& e){auto& p=*static_cast<PreviewServicesV122*>(raw);bool found{};if(!name||!p.domain.read(name,found,bytes,e))return false;if(!found){e=std::string("Actual preview FX asset missing: ")+name;return false;}return true;}};
  dh2::fx::MeshFxServicesV1 services{this,[](void* raw,auto& q,auto& e){auto& p=*static_cast<PreviewServicesV122*>(raw);
   using O=dh2::fx::MeshFxOperationV1;
   if(q.operation==O::debug_load){if(dh2_character_debug_load(p.domain.debug.get(),p.domain.debug_files)!=1){e="Original preview FX Debug load failed";return false;}return true;}
   if(q.operation==O::module_enabled)return dh2_fx_debug_module_get(&q.result,p.modules.get(),q.text)==1;
   if(q.operation==O::set_switch||q.operation==O::instance_switch)return dh2_character_debug_get(&q.result,p.domain.debug.get(),q.text,p.domain.debug_files)==1;
   std::shared_ptr<Record> actor;bool matched{};
   if(q.identity&&!borrow_native_menu_preview_character_v122(p.domain.owner,q.identity,actor,matched,e))return false;
   if(q.operation==O::anchor_dead){if(!actor||!actor->life){e="Required actual preview FX anchor life";return false;}q.result=actor->life->dead!=0;return true;}
   if(q.operation==O::anchor_disabled){const auto* value=actor?actor->actor->source_bool_field(0x81):nullptr;if(!value){e="Required actual preview anchor81";return false;}q.result=*value;return true;}
   if(q.operation==O::anchor_position||q.operation==O::anchor_rotation||q.operation==O::anchor_scale){if(!actor){e="Required same process FX anchor";return false;}
    const float* values=q.operation==O::anchor_position?actor->actor->source_position160_v7():q.operation==O::anchor_rotation?actor->actor->runtime.rotation.rotation:nullptr;
    dh2::world::GameObjectInitializationFieldsV62 fields;if(!values){if(!actor->actor->inherited_initialization_fields_v62(actor,fields,e)||!fields.vector3)return false;values=fields.vector3(0x120);}if(!values)return false;std::copy_n(values,3,q.point);return true;
   }
   e="Required actual process FX anchor/floor operation "+std::to_string(static_cast<unsigned>(q.operation));return false;
  }};
  effects=std::make_shared<dh2::fx::CharacterMeshFxOwnerV4>(domain.fx_libraries,visual->scene(),assets,services,dh2::fx::CharacterParticleFxFactoryV2{});
  return true;
 }
};
struct PreviewEquipmentV122 {
 std::weak_ptr<Record> actor;std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<PreviewServicesV122> resources;std::shared_ptr<void> required;
 static dh2::skinning::VisualAssetResultV6 asset(void* raw,const char* name,std::vector<std::uint8_t>& bytes,std::string& e){auto& p=*static_cast<PreviewEquipmentV122*>(raw);bool found{};if(!name||!p.resources->domain.read(name,found,bytes,e))return dh2::skinning::VisualAssetResultV6::failed;return found?dh2::skinning::VisualAssetResultV6::found:dh2::skinning::VisualAssetResultV6::missing;}
 static bool world(void* raw,dh2::player::EquipmentWorldQueryV1 q,std::uintptr_t id,std::uintptr_t& identity,std::int32_t& value,std::string& e){auto& p=*static_cast<PreviewEquipmentV122*>(raw);auto r=p.actor.lock();auto app=p.application.lock();identity=0;value=0;if(!r||!app){e="Retired same preview Gear queries";return false;}using Q=dh2::player::EquipmentWorldQueryV1;
  if(q==Q::online){auto online=app->get_online_loading_v55();if(!online){e="Required actual preview GetOnline";return false;}value=online->byte5()!=0;return true;}
  if(q==Q::player_count){auto pm=app->source_player_manager_v59();if(!pm||!pm->count_field()){e="Required actual PM6c4 for preview Gear";return false;}value=*pm->count_field();return true;}
  if(id!=r->actor->object->identity){e="Preview Gear addressed another Character";return false;}
  if(q==Q::current_player){dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;identity=current?current.identity():0;return true;}
  if(q==Q::current_difficulty)return dh2::player::character_game_difficulty_v29({r->actor,id,r->save_fields->save_slot14e8(),r->services.difficulty_global},value,e);
  if(q==Q::remotely_updated){const auto* flag=r->actor->source_bool_field(0x118);if(!flag){e="Required actual preview remote118";return false;}value=*flag;return true;}
  e="Required positive preview Gear network record";return false;
 }
};
std::map<std::uintptr_t,std::weak_ptr<PreviewServicesV122>> preview_service_owners_v123;
bool preview_services_v123(Record& r,std::uintptr_t id,std::shared_ptr<PreviewServicesV122>& out,std::string& e){
 const auto at=preview_service_owners_v123.find(id);out=at==preview_service_owners_v123.end()?nullptr:at->second.lock();
 auto app=out?out->application.lock():nullptr;
 if(!out||!app||!app->source_objects_v121()||r.services.world!=out->domain.owner||
    r.services.canonical_objects!=out->domain.objects||app->source_objects_v121()->manager()!=out->domain.objects||
    !r.services.selected_profile_lease||r.services.selected_profile_lease.owner_before(out)||out.owner_before(r.services.selected_profile_lease)){
  e="Required SAME published preview Character service owner";return false;
 }
 e.clear();return true;
}
}
bool borrow_native_menu_preview_fx_owner_v123(Record& r,std::string& e){
 if(!r.actor||!r.actor->object){e="Required live preview Character at positive FX owner borrow";return false;}
 std::shared_ptr<PreviewServicesV122> p;if(!preview_services_v123(r,r.actor->object->identity,p,e))return false;
 if(!p->effects){e="Positive preview FX field without its real instance backend";return false;}
 if(p->effects->source_libraries_v63()!=p->domain.fx_libraries){e="Changed preview FX library receiver";return false;}
 r.target_fx_pin_v70=p;r.target_fx_manager_v70=p->effects.get();e.clear();return true;
}
bool detach_native_menu_preview_fx_anchors_v123(Record& r,std::uintptr_t provider,std::uintptr_t retiring,std::string& e){
 std::shared_ptr<PreviewServicesV122> p;if(!preview_services_v123(r,provider,p,e))return false;
 if(p->effects){if(p->effects->source_libraries_v63()!=p->domain.fx_libraries){e="Changed preview FX anchor library receiver";return false;}p->effects->detach_anchor_v117(retiring);}
 // A NULL member observes this service owner's unexecuted instance C1.
 e.clear();return true;
}
bool retire_native_menu_preview_script_receiver_v123(Record& r,std::uintptr_t id,const std::shared_ptr<dh2::character::ScriptCharacterObject>& object,std::string& e){
 std::shared_ptr<PreviewServicesV122> p;if(!preview_services_v123(r,id,p,e))return false;
 if(!r.actor||r.actor->object||r.actor->session||r.player_script_owner_v62){e="Preview script receiver retirement precedes actual Character/VM D0";return false;}
 if(!p->objects->retire_native_receiver_v123(id,object,e))return false;
 preview_service_owners_v123.erase(id);e.clear();return true;
}
bool build_native_menu_preview_character_services_v122(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const MenuPreviewProcessDomainV121& d,std::int32_t slot,bool fresh,dh2::world::CanonicalCharacterCandidateServicesV60& s,std::string& e){
 if(!app||slot<0||slot>=4||!d.owner||!d.design||!d.character_cache||!d.controller_blocked||!d.localization||!d.text_owner||!d.fx_libraries){e="Required complete actual process preview inputs";return false;}
 auto p=std::make_shared<PreviewServicesV122>();p->domain=d;p->domain.character_services={};p->application=app;
 p->modules.reset(dh2_fx_debug_modules_create(d.debug.get(),d.debug_files));if(!p->modules){e="Actual process FX Debug constructor failed";return false;}p->preload={p->modules.get(),dh2_fx_debug_preload_service};
 if(!p->scripts.load({d.directory,d.read},d.skills->borrow(),e))return false;
 p->objects=std::make_shared<dh2::character::CharacterScriptObjects>(d.design->borrow(),d.debug.get(),d.debug_files);
 const auto random=app->source_random_v62();if(!random){e="Required SAME process Random globals";return false;}
 s.world=d.owner;s.canonical_objects=d.objects;s.physical_world=d.physical;s.random=&random->channel(0);s.alternate_random=&random->channel(1);s.random_lease=random;
 s.selected_profile_lease=p; //independent providers pin resources, App and actors stay weak
 s.effect_services=&p->preload;s.effect_table=&d.fx_libraries->registration_table_v122();s.effect_queue=&d.fx_libraries->pending_queue_v63();s.character_effects=&d.fx_libraries->source_tables().characters();
 s.faeries_v70=p->scripts.borrow().faeries();s.difficulty_global=dh2::player::process_player_save_difficulty_global_v29();
 s.publish_script_object=[p](const auto& object,auto& e){
  if(!p->objects->publish_existing_v62(object,e))return false;
  auto& slot=preview_service_owners_v123[object->identity];auto previous=slot.lock();
  if(previous&&previous!=p){e="Preview Character service identity reused before source retirement";return false;}slot=p;return true;
 };
 s.is_local_player=[weak=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(app)](auto id,bool& out,auto& e){auto app=weak.lock();if(!app||!app->source_player_manager_v59()){e="Required actual preview PM IsLocalPlayer";return false;}return app->source_player_manager_v59()->source_is_local_player_v61(id,out,e);};
 const auto online=app->get_online_loading_v55();s.online_byte5=[online](bool& out,auto& e){if(!online){e="Required actual preview GetOnline";return false;}out=online->byte5()!=0;return true;};
 s.quest_sync.owner=online;s.quest_sync.online=s.online_byte5;s.quest_sync.is_local_player=s.is_local_player;
 s.current_level=[](auto& out,auto& e){dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;out={};if(current){out.receiver=current.level();out.identity=current.identity();out.mode118=&current.level()->constructor_fields_v3().mode118;}return true;};
 s.controller=[p](auto& out,auto& r,auto& e){if(!r.actor||!r.actor->controller||!p->domain.controller_blocked){e="Required actual preview controller/global";return false;}auto* fields=r.actor->controller->command_state(*p->domain.controller_blocked);if(!fields){e="Invalid actual controller block byte";return false;}out=*fields;return true;};
 s.collision_globals_v68=std::make_shared<dh2::character::WorldNpcCollisionGlobalsV1>();
 s.collision_clock_v68=[weak=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(app)](auto& frame,auto& dt,auto& e){auto app=weak.lock();if(!app){e="Retired process collision clock";return false;}frame=app->source_loading_v55().frame74;dt=app->source_loading_v55().dt8c;return true;};
 s.physical=[p](auto& r,auto& out,auto&,auto& e){
  auto weak=std::weak_ptr<Record>(r.shared_from_this());dh2::character::CharacterCollisionScriptBorrowV68 script;script.receiver=r.actor;script.properties=r.properties;script.life=r.life;script.active=[weak](auto& view){auto r=weak.lock();if(!r)return false;if(r->player_script_owner_v62)return r->player_script_owner_v62->session().owner().active(view);return r->actor->session&&r->actor->session->owner().active(view);};
  dh2::character::WorldNpcCollisionServicesV1 services;services.context=&r;
  services.clock=[](void* raw,auto& frame,auto& dt,auto& e){auto& r=*static_cast<Record*>(raw);return r.services.collision_clock_v68&&r.services.collision_clock_v68(frame,dt,e);};
  services.cancel_sneaking=[](void* raw,auto id,auto& e){auto& r=*static_cast<Record*>(raw);if(id!=r.actor->object->identity||!r.player_script_owner_v62){e="Required actual preview selected player sneaking owner";return -1;}const auto status=r.player_script_owner_v62->native_cancel_sneaking(&r.target_character_v62.interactive415);if(status)e=r.player_script_owner_v62->error();return status? -1:0;};
  services.object_type=[](void* raw,auto id,auto& type,auto& e){auto& r=*static_cast<Record*>(raw);std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* actor{};bool more=r.services.canonical_objects->source_ordered_begin_v38(key,actor);while(more){if(actor&&actor->identity==id&&actor->type_f4){type=*actor->type_f4;return true;}more=r.services.canonical_objects->source_ordered_next_v38(key,key,actor);}e="Required actual process physical peer type";return false;};
  services.ais_method=[](void*,auto,auto method,auto,auto,auto& e){e="Required selected preview AIS collision override "+std::to_string(method);return -1;};
  auto* controller=r.actor->controller->command_state(*p->domain.controller_blocked);if(!controller){e="Required actual preview controller";return false;}
  r.collision_owner_v68=std::make_unique<dh2::character::CharacterWorldCollisionV68>(*r.services.world_targets,*r.actor->machine,std::move(script),*r.actor->object,r.actor->ai_events,*controller,r.collision_fields_v68,*r.services.collision_globals_v68,r.services.debug,r.services.debug_files,services);
  out.receiver=r.actor;out.filter=[weak](auto category,bool& value,auto& e){auto r=weak.lock();if(!r||!r->collision_owner_v68)return false;if(!r->collision_owner_v68->permits_filter(category,value)){e=r->collision_owner_v68->error();return false;}return true;};out.event=[weak](auto event,auto peer,auto persist,auto& e){auto r=weak.lock();if(!r||!r->collision_owner_v68)return false;if(!r->collision_owner_v68->physical_event(event,peer,persist)){e=r->collision_owner_v68->error();return false;}return true;};return true;
 };
 if(!compose_native_menu_preview_character_resources_v122(app,d,s,e))return false;
 s.world_targets=std::make_shared<dh2::character::skills::CharacterWorldRuntimeV1>(*d.design->borrow().ai());
 if(!bind_campaign_character_sound_names_v101(d.character_cache,s,e))return false;
 const auto resources_visual=s.visual;s.visual=[resources_visual](auto& r,auto& out,auto& e){if(!resources_visual(r,out,e))return false;out.events={&r,source_campaign_character_animation_event_v101};out.step_fx={&r,source_campaign_animation_step_v100};return true;};
 s.register_character_fx=[p](auto& r,auto& e){const auto& rows=p->domain.fx_libraries->source_tables().characters();if(rows.empty()){e="Actual CharacterEffects rows empty";return false;}auto index=r.properties->resolved[7];if(index<0||std::size_t(index)>=rows.size())index=0;std::uint32_t word{};if(dh2_character_debug_load(p->domain.debug.get(),p->domain.debug_files)!=1||dh2_character_debug_get(&word,p->domain.debug.get(),"isTracingChar_Init",p->domain.debug_files)!=1)return false;const auto& row=rows[std::size_t(index)];for(auto set:{row.footprint,row.blood,row.blood_death})if(set>=0&&!p->domain.fx_libraries->register_set_to_load(set,e))return false;return true;};
 s.grab_fx=[p](auto& r,auto set,auto& id,auto& e){if(!p->fx(r,e))return false;return p->effects->grab_marker_v28(set,r.actor->object->identity,id,e);};
 s.initialize_target_marker_v70=[p](auto& r,bool& present,auto& e){if(!r.services.models){e="Required actual preview Models";return false;}present=!r.services.models->values.empty();if(!present)return true;if(!p->fx(r,e))return false;
  r.target_fx_pin_v70=p;r.target_fx_manager_v70=p->effects.get();r.target_marker_v70=std::make_unique<dh2::character::CharacterTargetMarkerV28>(*p->effects,p->domain.fx_libraries->source_tables(),r.actor->object->identity,dh2::character::TargetMarkerServicesV28{});
  if(!r.target_marker_v70->initialize(e))return false;bool local{};if(!r.services.is_local_player(r.actor->object->identity,local,e))return false;if(!local)return true;
  const auto settings=p->domain.settings->borrow();const auto row=settings.row_index("Default"),field=settings.field_index("TargetMove");const auto* word=row>=0&&field>=0?settings.word(std::uint32_t(row),std::uint32_t(field)):nullptr;if(!word){e="Actual process Default.TargetMove absent";return false;}std::int32_t set{};std::memcpy(&set,word,4);auto* cross=r.actor->source_target_cross149c_v70();if(!cross){e="Unproduced actual preview149c";return false;}if(!p->effects->grab_marker_v28(set,0,*cross,e))return false;if(!*cross)return true;const float zero[3]{};return p->effects->marker_rotation_v70(*cross,zero,e)&&p->effects->marker_sync_v83(*cross,false,e)&&p->effects->marker_visible_v28(*cross,false,e)&&p->effects->marker_loop_v76(*cross,true,e);
 };
 s.initialize_highlight_v70=[p](auto& r,auto& e){auto app=p->application.lock();auto pm=app?app->source_player_manager_v59():nullptr;dh2::player::PlayerInfoFieldsV1* info{};if(!pm||!pm->manager()->get_by_character(r.actor->object->identity,false,info,e)||!info)return false;const auto friendly=std::uint32_t(info->friendly678);if(friendly>3)return true;const auto& names=p->domain.fx_libraries->source_tables().set_names();const auto it=std::find(names.begin(),names.end(),"multiplayer_player01_glow");if(it==names.end())return true;if(!p->fx(r,e))return false;auto& id=r.actor->source_highlight14a0();if(id)return true;if(!p->effects->grab_marker_v28(std::int32_t(it-names.begin())+std::int32_t(friendly),0,id,e))return false;if(!id)return true;const float zero[3]{};return p->effects->marker_rotation_v70(id,zero,e)&&p->effects->marker_sync_v83(id,false,e)&&p->effects->marker_visible_v28(id,false,e)&&p->effects->marker_store_anchor_v83(id,r.actor->object->identity,e)&&p->effects->marker_sync_v83(id,true,e)&&p->effects->marker_loop_v76(id,true,e);};
 s.initialize_sounds_v70=[cache=d.character_cache](auto& r,auto& e){const auto* sounds=cache->sounds_v70().get(r.properties->resolved[9]);if(!sounds){e="Required original CharSounds fallback2";return false;}dh2::audio::AudioApplicationBorrowV42 audio;if(!borrow_actual_application_audio_v42(audio,e))return false;if(!audio.manager)return true;for(const auto* list:{&sounds->hit,&sounds->death,&sounds->impact1,&sounds->impact2})for(std::uint32_t i=0;i<list->count;++i){if(!list->ids||!borrow_actual_application_audio_v42(audio,e)||!audio.manager||!audio.precache_raw_uid(list->ids[i],e))return false;}return true;};
 s.player_script=[p](auto& r,auto& out,auto& e){auto& input=out.session;const auto design=p->domain.design->borrow();const auto* ai=r.properties&&design.ai()?dh2::data::ai_props(*design.ai(),r.properties->resolved[1]):nullptr;if(!ai){e="Required actual preview Player AI row before script resource selection";return false;}std::string external_path;input.common=p->scripts.borrow().common();input.external={};if(!ai->script.empty()&&ai->script[0]!='_'){external_path="data/scripts/ai/"+ai->script;const char* suffix=std::strstr(ai->script.c_str(),".lua");if(!suffix)external_path+=".luac";else if(std::strncmp(suffix,".luac",5))external_path+='c';const auto* external=p->scripts.borrow().find(external_path);if(!external){e="Actual cached selected preview Player script unavailable: "+external_path;return false;}input.external={external->data(),external->size()};}if(!p->scripts.borrow().session_files(external_path,input.include_files,e))return false;input.host=&p->host;input.level=&p->level_services;input.target=&r.actor->object->binding;input.objects=&p->objects->services();input.temporary=dh2::character::character_properties_temp_global_v62();input.cached_file_context=p.get();input.cached_file=[](void* raw,const char* name,auto* bytes,bool* found){auto& p=*static_cast<PreviewServicesV122*>(raw);if(!name||!bytes||!found)return -1;const auto* file=p.scripts.borrow().find(name);*found=file!=nullptr;*bytes=file?dh2::data::Bytes{file->data(),file->size()}:dh2::data::Bytes{};return 0;};out.faeries=p->scripts.borrow().faeries();out.initialization.context=&r;
  out.initialization.vitals=[](void* raw,auto& session,const auto& q){auto& r=*static_cast<Record*>(raw);if(q.service!=dh2::character::script_refresh_vitals||q.subject!=r.actor->object->identity||session.properties()!=r.properties||!r.services.effect_services)return -1;const dh2::character::ScriptInitVitals32 request{q.subject,&session.property_view(),*r.services.effect_services};dh2::character::ScriptInitVitals24 result{};return dh2_character_script_init_vitals(&result,&request)==1?0:-1;};
  out.initialization.gameplay.ai=&r.actor->ai_events;if(!bind_campaign_character_player_events_v101(r,out.initialization.gameplay,e))return false;r.skill_owner_view_v68.character=r.actor->object->identity;r.skill_owner_view_v68.flags=r.actor->machine->state().flags;out.initialization.gameplay.skill_owner=&r.skill_owner_view_v68;
  const auto weak=std::weak_ptr<Record>(r.shared_from_this());out.initialization.gameplay.live_character_flags_v68=[weak](auto& flags){auto r=weak.lock();if(!r||!r->actor->machine)return false;flags=r->actor->machine->state().flags;return true;};out.initialization.gameplay.difficulty_context=&r;out.initialization.gameplay.difficulty=[](void* raw,std::int32_t* out){auto& r=*static_cast<Record*>(raw);return out&&dh2::player::character_game_difficulty_v29({r.actor,r.actor->object->identity,r.save_fields->save_slot14e8(),r.services.difficulty_global},*out,r.error)?0:-1;};return true;
 };
 s.equipment_inputs=[p](auto& r,auto& out,auto& e){auto app=p->application.lock();auto visual=r.visual?r.visual->visual():nullptr;if(!app||!visual||!r.save||!r.preview_profile_v122){e="Required SAME actual preview visual/Save/profile before Gear";return false;}
  if(!p->items){dh2::character::SourceItemResourceInputsV88 input;input.owner=p->domain.owner;input.localization_owner=p->domain.text_owner;input.localization=p->domain.localization;input.text_environment=p->domain.text_environment;input.loot=p->domain.character_cache->loot().borrow();input.design=p->domain.design->borrow();input.asset=[p](const char* path,bool& found,auto& bytes,auto& e){return path&&p->domain.read(path,found,bytes,e);};p->items=std::make_shared<dh2::character::SourceItemResourcesV88>(std::move(input));if(!p->items->load(e))return false;}
  auto platform=std::make_shared<PreviewEquipmentV122>();platform->actor=r.shared_from_this();platform->application=app;platform->resources=p;
  const auto& bres=visual->bres();if(!bres.bytes||!bres.size){e="Actual preview visual BRES absent";return false;}std::vector<std::uint8_t> bytes(bres.bytes,bres.bytes+bres.size);dh2::skinning::VisualSkinResourcesV6 resources;if(!resources.load(bytes,e))return false;
  out.resources=resources.borrow();out.live_scene=&visual->scene();out.assets={platform.get(),PreviewEquipmentV122::asset};out.world={platform.get(),PreviewEquipmentV122::world};out.services_lease_v62=platform;out.debug=p->domain.debug.get();out.debug_files=*p->domain.debug_files;out.text_environment=p->domain.text_environment;
  const auto settings=app->source_settings4c_v67();if(!settings){e="Required actual process language before Gear";return false;}out.language_pack=settings->language();out.immutable_loot_v88=p->items->loot();out.immutable_powers_v88=p->items->definitions();return make_source_campaign_equipment_services_v118(app,r.shared_from_this(),p->items,out.required,platform->required,e);
 };
 s.prepare_player_equipment=[](auto& r,auto& e){if(r.equipment||r.prepared_equipment_v60||r.inventory_transferred||!r.services.equipment_inputs){e="Cannot replay actual preview Gear creation";return false;}dh2::player::PlayerEquipmentRenderInputsV1 input;if(!r.services.equipment_inputs(r,input,e))return false;input.design=r.services.design->borrow();input.character=r.actor->object->identity;input.properties=r.properties;input.random=r.services.random;input.potion_capacity=r.constructor_inventory->potion_capacity_v4();input.source_visual2d8_v62=&r.actor->source_visual();auto weak=std::weak_ptr<Record>(r.shared_from_this());input.source_profile_load_v59=[weak](auto& gear,auto& e){auto r=weak.lock();if(!r||r->prepared_equipment_v60!=&gear||!r->load){e="Required same preview Gear/SaveLoad at restore";return false;}return r->load->load(4,e);};if(!r.transfer_inventory(input,e))return false;r.equipment=std::make_unique<dh2::player::PlayerEquipmentRenderOwnerV1>(std::move(input));r.prepared_equipment_v60=r.equipment.get();return r.equipment->prepare_restore_v60(e);};
 s.finish_player_profile_v67=[weak=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(app)](auto& r,auto& e){auto app=weak.lock();return app&&finish_native_menu_preview_profile_v122(app,r,e);};s.player_save_final_v122=save_native_menu_preview_profile_v122;s.destroy_character_save_v107=destroy_native_menu_preview_profile_v122;
 s.release_player_equipment=[](auto& r,auto& e){if(r.equipment){r.equipment.reset();r.prepared_equipment_v60=nullptr;r.inventory37c=nullptr;}e.clear();return true;};
 s.remaining=[p](auto& r,const auto& q,auto&,auto& e){if(q.source_entry==0x3b4834)return initialize_native_menu_preview_character_light_material_v122(r,p->domain,e);e="Required original process preview Character leaf "+std::to_string(q.source_entry);return false;};
 if(!bind_native_menu_preview_fx_initialization_v122(app,d,s,e))return false;
 (void)fresh;e.clear();return true;
}
}
