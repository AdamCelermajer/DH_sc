// Include in namespace model_renderer after V59/V60 and the lookup fragment.
// Required top-level headers: renderer_character_campaign_v62.hpp,
// character_candidate_cache_v62.hpp, canonical_character_pf_v62.hpp,
// campaign_navigation_registry_v64.hpp, visual_fx_manager_libraries_v63.hpp.
bool bind_campaign_character_providers_v62(AAssetManager* assets,const SourceCampaignCandidateBorrowV55& actual,std::string& e){
 if(!assets||!actual.actual_world||!actual.application||!actual.floors||!actual.roots||!actual.physical_world||!actual.navigation_registry){e="Required actual campaign cache/World/roots/PF/physical producers";return false;}
 auto world=std::static_pointer_cast<WorldScriptContext>(actual.actual_world);
 if(!world||!world->design||!world->skills_owner||!world->objects){e="Required actual source World tables and Lua facade";return false;}
 auto assets_owner=std::make_shared<dh2::android_ui::OriginalCacheAssetsV1>(assets);
 dh2::character::ScriptAssetServicesV1 files;
 files.directory=[assets_owner](auto& names,auto& error){return assets_owner->directory(names,error);};
 files.read=[assets_owner](const auto& uri,bool& found,auto& bytes,auto& error){return assets_owner->read(uri,found,bytes,error);};
 auto cache=std::make_shared<dh2::character::CharacterCandidateCacheV62>(files);
 if(!cache->load(e))return false;
 if(!world->script_files){if(!world->script_assets.load(files,world->skills_owner->borrow(),e))return false;world->script_files=world->script_assets.borrow();}
 auto animation_sets=std::make_shared<dh2::character::CharacterAnimationSetCacheV6>();
 const auto floors=actual.floors;const auto roots=actual.roots;
 const auto navigation=actual.navigation_registry;
 dh2::world::CanonicalCharacterCandidateServicesV60 services;
 services.character_templates_v78=cache->templates_v78();
 //The C callback's opaque ABI is mutable void*, but this provider only reads
 //the immutable table. The SAME per-record shared lease outlives callbacks;
 //do not lend a raw CharacterCandidateCache address that can be retired first.
 services.presets.context=const_cast<dh2::data::CharacterTemplateTableV78*>(services.character_templates_v78.get());
 services.presets.preset=[](void* raw,const std::string& name,const std::int16_t*& rows,std::uint32_t& count,std::string& error){
  const auto* templates=static_cast<const dh2::data::CharacterTemplateTableV78*>(raw);
  if(!templates||!templates->ready()){error="Required SAME initialized CharacterTemplates owner";return false;}
  return templates->preset(name,rows,count,error);
 };
 services.visual_ready_v77=[world=std::weak_ptr<WorldScriptContext>(world)](auto& record,auto& error){
  auto actual=world.lock();if(!actual||!record.actor||!record.actor->object){error="Required actual Character/World at FX Scene publication";return false;}
  SourceCampaignFxRenderServicesV77 render;
  render.camera=[world](float* view,float* position,std::string& error){auto actual=world.lock();if(!actual){error="Expired campaign FX camera scope";return false;}return borrow_source_campaign_fx_camera_v77(actual,view,position,error);};
  return publish_source_campaign_fx_v77(actual,record.actor->object->identity,std::move(render),error);
 };
 services.grab_fx=[world=std::weak_ptr<WorldScriptContext>(world)](auto& record,std::int32_t set,std::uintptr_t& identity,std::string& error){
  auto actual=world.lock();std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* manager{};
  if(!actual||!record.actor||!record.actor->object||!borrow_source_campaign_fx_v77(actual,pin,manager,error))return false;
  return manager->grab_marker_v28(set,record.actor->object->identity,identity,error);
 };
 std::shared_ptr<dh2::world::NativeConditionRuntimeV69> condition_runtime;
 if(!lend_campaign_condition_bag_v75(actual,services.conditions,condition_runtime,e)||
    !bind_source_campaign_module_conditions_runtime_v75(actual,services.conditions,e))return false;
 auto transport_slot=std::make_shared<std::weak_ptr<RendererCharacterCandidateV60>>();
 services.finish_player_profile_v67=[transport_slot](auto& record,auto& error){auto transport=transport_slot->lock();if(!transport||!record.actor||!record.actor->object){error="Required retained canonical player transport at completed InitPost";return false;}return transport->finish_selected_profile_v67(record.actor->object->identity,error);};
 services.world=world;services.design=world->design.get();services.models=&cache->models();services.animation_tables=&cache->animation_tables();services.loot_tables=&cache->loot();services.physical_world=actual.physical_world;
 services.world_targets=std::make_shared<dh2::character::skills::CharacterWorldRuntimeV1>(*world->tables.ai());
 services.canonical_objects=actual.objects;
 services.collision_globals_v68=std::make_shared<dh2::character::WorldNpcCollisionGlobalsV1>();
 services.collision_clock_v68=[app=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(actual.application)](std::uint32_t& frame,std::uint32_t& dt,auto& error){auto actual=app.lock();if(!actual){error="Expired actual Application collision clock";return false;}if(!model_renderer::borrow_application_frame_count_v68(frame,error))return false;dt=actual->source_loading_v55().dt8c;error.clear();return true;};
 services.physical=[floors,navigation,transport_slot](auto& record,auto& output,auto& physical,auto& error){
  if(!record.actor||!record.actor->object||!record.actor->machine||!record.actor->controller||!record.services.world_targets||!record.services.collision_globals_v68){error="Required SAME canonical Character FSM/controller/target/collision owners";return false;}
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record.shared_from_this();
  dh2::character::CharacterCollisionScriptBorrowV68 script;script.receiver=record.actor;script.properties=record.properties;script.life=record.life;
  script.active=[weak](auto& view){auto r=weak.lock();if(!r)return false;if(r->player_script_owner_v62)return r->player_script_owner_v62->session().owner().active(view);return r->actor->session&&r->actor->session->owner().active(view);};
  dh2::character::WorldNpcCollisionServicesV1 collision_services;collision_services.context=&record;
  collision_services.clock=[](void* raw,std::uint32_t& frame,std::uint32_t& dt,auto& error){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!r.services.collision_clock_v68){error="Required actual Application frame74/dt8c collision producer";return false;}return r.services.collision_clock_v68(frame,dt,error);};
  collision_services.cancel_sneaking=[](void* raw,std::uintptr_t identity,auto& error){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!r.actor||identity!=r.actor->object->identity){error="Collision CancelSneaking requires SAME canonical Character";return -1;}if(r.player_script_owner_v62){const auto status=r.player_script_owner_v62->native_cancel_sneaking(&r.target_character_v62.interactive415);if(status!=1){error=r.player_script_owner_v62->error();return -1;}return 0;}
   dh2::character::sneaking::Character48 c{};c.identity=identity;c.resolved={r.properties->resolved.data(),224,0};const dh2::character::sneaking::Services16 services{&r,[](void* p,const auto* q,std::uint32_t* out){auto& self=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(p);if(!q||!out||q->operation!=dh2::character::sneaking::is_player)return -1;bool value{};if(!self.is_player(value,self.error))return -1;*out=value;return 0;}};const auto status=dh2_character_cancel_sneaking(&c,&services);if(status){error="Required actual NPC sneaking skill/buff continuation";return -1;}return 0;
  };
  collision_services.object_type=[](void* raw,std::uintptr_t identity,std::uint32_t& type,auto& error){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!r.services.canonical_objects){error="Required SAME canonical object type registry";return false;}std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};bool more=r.services.canonical_objects->source_ordered_begin_v38(key,object);while(more){if(object&&object->identity==identity&&object->type_f4){type=*object->type_f4;return true;}more=r.services.canonical_objects->source_ordered_next_v38(key,key,object);}error="Required actual physical peer ObjectBase typef4";return false;};
  collision_services.ais_method=[](void*,std::uintptr_t,std::uintptr_t method,std::uintptr_t,std::uint32_t,auto& error){error="Required actual selected AIS collision override "+std::to_string(method);return -1;};
  auto* control=record.actor->controller->command_state(controller_global_blocked);if(!control){error="Invalid actual global controller block byte";return false;}
  record.collision_owner_v68=std::make_unique<dh2::character::CharacterWorldCollisionV68>(*record.services.world_targets,*record.actor->machine,std::move(script),*record.actor->object,record.actor->ai_events,*control,record.collision_fields_v68,*record.services.collision_globals_v68,record.services.debug,record.services.debug_files,collision_services);
  output.receiver=record.actor;
  output.filter=[weak](std::uint16_t category,bool& value,auto& error){auto r=weak.lock();if(!r||!r->collision_owner_v68){error="Expired SAME Character collision filter owner";return false;}r->actor->controller->command_state(controller_global_blocked);if(!r->collision_owner_v68->permits_filter(category,value)){error=r->collision_owner_v68->error();return false;}return true;};
  output.event=[weak](auto event,std::uintptr_t peer,std::uint32_t persist,auto& error){auto r=weak.lock();if(!r||!r->collision_owner_v68){error="Expired SAME Character collision event owner";return false;}r->actor->controller->command_state(controller_global_blocked);if(!r->collision_owner_v68->physical_event(event,peer,persist)){error=r->collision_owner_v68->error();return false;}return true;};
  physical.context=&record;
  physical.debug_switch=[](void* raw,const char* name,bool& value,auto& error){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);return r.services.debug&&r.services.debug_files&&dh2::character::character_npc_physical_debug_v1(*r.services.debug,*r.services.debug_files,name,value,error);};
  physical.peer_owner=[](void*,void* context,std::uintptr_t& identity,auto& error){auto transport=campaign_character_transport_v62.lock();if(!transport){error="Required SAME campaign physical peer directory";return false;}return transport->physical_peer_v68(context,identity,error);};
  physical.enabled80=[](void*,std::uintptr_t identity,std::uint8_t& value,auto& error){auto transport=campaign_character_transport_v62.lock();auto r=transport?transport->actor(identity):nullptr;const auto* field=r?r->actor->source_bool_field(0x80):nullptr;if(!field){error="Required actual physical peer ObjectBase80 producer";return false;}value=*field;return true;};
  physical.update_pf=[](void* raw,std::uintptr_t identity,const dh2::physical::NativeBody* body,const dh2::physical::NpcBodyProjection*,auto& error){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!r.actor||r.actor->object->identity!=identity||!r.physical_owner_v62||body!=&r.physical_owner_v62->native()||!r.services.inherited){error="Required SAME actual physical/PF assignment";return -1;}dh2::world::GameObjectInitializationServicesV1 services;if(!r.services.inherited(r,services,error)||!services.update_pf_object||!services.update_pf_object(error))return -1;return 0;};
  error.clear();return true;
 };
 services.controller=[](auto& out,auto& record,auto& error){if(!record.actor||!record.actor->controller){error="Required actual canonical controller receiver";return false;}auto* fields=record.actor->controller->command_state(controller_global_blocked);if(!fields){error="Invalid source global controller-block byte";return false;}out=*fields;return true;};
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> weak_app=actual.application;
 std::weak_ptr<WorldScriptContext> weak_world=world;
 services.npc_script=[weak_world,weak_app](auto& record,auto& input,auto& error){
  auto same=weak_world.lock();if(!same||!same->script_files||!same->objects||!record.actor||!record.actor->object){error="Required SAME canonical NPC World script cache and object facade";return false;}
  // AISExternal's bundled monster program is the existing source resource,
  // not a copied Crypt session. Session construction binds this receiver's
  // own identity, property sheets, FSM and delayed field at its source point.
  const auto* external=same->script_files.find("data/scripts/ai/monster.luac");
  if(!external){error="Actual cached AISExternal monster program unavailable";return false;}
  input.common=same->script_files.common();input.external={external->data(),external->size()};
  if(!same->script_files.session_files("data/scripts/ai/monster.luac",input.include_files,error))return false;
  input.temporary=dh2::character::character_properties_temp_global_v62();
    auto app=weak_app.lock();if(!app){error="Released SAME App before canonical script host binding";return false;}
  if(!record.host_context_v70)record.host_context_v70=std::make_shared<RendererCanonicalHostContextV70>(same,app);
  input.host=&std::static_pointer_cast<RendererCanonicalHostContextV70>(record.host_context_v70)->bindings;input.level=&same->level_services;
  input.target=&record.actor->object->binding;input.objects=&same->objects->services();
  // Commands/timer expiry are not fabricated here: reached native wrappers
  // retain required-service failures until the actor command owner is bound.
  error.clear();return true;
 };
 services.player_script=[weak_world,weak_app](auto& record,auto& output,auto& error){
  auto same=weak_world.lock();if(!same||!same->script_files||!same->objects||!record.actor||!record.actor->object||!record.save||!record.save_fields||*record.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(record.save.get())){error="Required SAME canonical Player Save14e8/cache/object facade before source VM";return false;}
  auto& input=output.session;input.common=same->script_files.common();  auto app=weak_app.lock();if(!app){error="Released SAME App before canonical script host binding";return false;}
  if(!record.host_context_v70)record.host_context_v70=std::make_shared<RendererCanonicalHostContextV70>(same,app);
  input.host=&std::static_pointer_cast<RendererCanonicalHostContextV70>(record.host_context_v70)->bindings;input.level=&same->level_services;input.target=&record.actor->object->binding;input.objects=&same->objects->services();
  input.cached_file_context=&record;input.cached_file=[](void* raw,const char* name,dh2::data::Bytes* bytes,bool* found){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!name||!bytes||!found||!r.services.world)return -1;auto world=std::static_pointer_cast<WorldScriptContext>(r.services.world);if(!world||!world->script_files)return -1;const auto* file=world->script_files.find(name);*found=file!=nullptr;*bytes=file?dh2::data::Bytes{file->data(),file->size()}:dh2::data::Bytes{};return 0;};
  output.faeries=same->script_files.faeries();output.initialization.context=&record;
  output.initialization.vitals=[](void* raw,dh2::character::CharacterScriptSessionV3& session,const dh2::character::ScriptLifecycleRequest32& q){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(q.service!=dh2::character::script_refresh_vitals||!r.actor||q.subject!=r.actor->object->identity||session.timers().owner!=q.subject||session.properties()!=r.properties||!r.services.effect_services)return -1;const dh2::character::ScriptInitVitals32 request{q.subject,&session.property_view(),*r.services.effect_services};dh2::character::ScriptInitVitals24 result{};return dh2_character_script_init_vitals(&result,&request)==1?0:-1;};
  output.initialization.gameplay.ai=&record.actor->ai_events;
  record.skill_owner_view_v68.character=record.actor->object->identity;record.skill_owner_view_v68.flags=record.actor->machine->state().flags;
  output.initialization.gameplay.skill_owner=&record.skill_owner_view_v68;
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record.shared_from_this();output.initialization.gameplay.live_character_flags_v68=[weak](std::uint32_t& flags){auto r=weak.lock();if(!r||!r->actor||!r->actor->machine)return false;flags=r->actor->machine->state().flags;return true;};
  output.initialization.gameplay.difficulty_context=&record;output.initialization.gameplay.difficulty=[](void* raw,std::int32_t* out){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!out||!r.actor||!r.save_fields)return -1;return dh2::player::character_game_difficulty_v29({r.actor,r.actor->object->identity,r.save_fields->save_slot14e8(),r.services.difficulty_global},*out,r.error)?0:-1;};
  // Native combat/state/event providers are attached below by their actual
  // same-record owners. Absent callbacks remain source required failures;
  // resource/VM construction never substitutes a development player backend.
  error.clear();return true;
 };
 services.equipment_inputs=[weak_app,cache](auto& record,auto& out,auto& error){auto app=weak_app.lock();if(!app){error="Expired actual Application before canonical Gear preparation";return false;}return make_canonical_equipment_inputs_v62(app,cache,record,out,error);};
 services.release_player_equipment=[](auto& record,auto& error){
  if(!record.equipment||record.equipment.get()!=record.prepared_equipment_v60){error="Required teardown of externally transferred SAME Gear before canonical visual release";return false;}
  record.equipment->detach_visual();record.equipment.reset();record.inventory37c=nullptr;error.clear();return true;
 };
 services.initial_height=[floors](const float* p,bool& found,float& height,std::string& error){dh2::navigation::HeightHit actual{};const auto status=dh2_nav_world_height(&actual,&floors->collision_world,p,0);if(status<0){error="Actual Character initial PF floor query failed";return false;}found=status!=0;if(found)height=actual.height;return true;};
 services.inherited=[floors,navigation](auto& record,auto& output,auto& error){
  output.device_high_performance=[](bool& value,std::string& error){return model_renderer::borrow_actual_device_high_performance_v54(value,error);};
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record.shared_from_this();
  output.update_pf_object=[weak,floors,navigation](auto& error){auto r=weak.lock();if(!r){error="Expired actual Character PF receiver";return false;}return dh2::world::canonical_character_update_pf_v62(*r,floors->collision_world,navigation->registry(),error);};
  output.init_pf_object=[weak,floors](bool,const float* p,float radius,std::uintptr_t identity,auto& error){auto r=weak.lock();if(!r||!r->actor||r->actor->object->identity!=identity){error="Required same Character PF InitObject identity";return false;}return dh2::world::canonical_character_init_pf_v62(*r,floors->collision_world,p,radius,error);};
  error.clear();return true; // Concrete services bound; not InitPost completion.
 };
 const auto fx_libraries=actual.fx_libraries;
 services.visual=[cache,roots,floors,navigation,animation_sets,fx_libraries](auto& record,auto& output,auto& error){
  output.world=record.services.world;output.scene_manager=roots;output.animation_manager=animation_sets;
  output.animation_dictionary=&cache->animations();
  output.animation_read=[cache](const auto& path,auto& bytes,auto& error){bool found{};if(!cache->files().read(path,found,bytes,error))return false;if(!found){error="Actual authored Character animation asset unavailable: "+path;return false;}return true;};
  output.visual.owner=record.services.world;output.visual.read_asset=cache->files().read;
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record.shared_from_this();
  output.update_pf=[weak,floors,navigation](auto& error){auto r=weak.lock();if(!r){error="Expired actual Character visual/PF receiver";return false;}return dh2::world::canonical_character_update_pf_v62(*r,floors->collision_world,navigation->registry(),error);};
  output.registration.invoke=[weak,fx_libraries](const auto& q,auto& error){auto r=weak.lock();if(!r||!r->services.debug||!r->services.debug_files){error="Required actual Character animation Debug owner";return false;}
   using O=dh2::character::NpcAnimationRegistrationOperationV6;
   if(q.operation==O::debug_load)return dh2_character_debug_load(r->services.debug,r->services.debug_files)==1;
   if(q.operation==O::debug_trace){std::uint32_t value{};if(dh2_character_debug_get(&value,r->services.debug,"isTracingChar_Init",r->services.debug_files)!=1){error="Required source animation registration Debug query";return false;}return true;}
   if(q.operation==O::preload_fx){if(!fx_libraries){error="Required SAME Application animation FX registration owner";return false;}return fx_libraries->register_set_to_load(q.resource,error);}
   error="Required source Character animation sound/FX registration continuation";return false;
  };
  error.clear();return true; // Actual resource loaders bound; no playback claim.
 };
 services.position=[](auto& record,auto& output,auto& error){
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record.shared_from_this();
  output.physical_body=[weak](std::uintptr_t identity,dh2::physical::NativeBody*& body,auto& error){auto r=weak.lock();if(!r||!r->physical_owner_v62||identity!=reinterpret_cast<std::uintptr_t>(r->physical_owner_v62.get())){error="Required exact Character physical2dc receiver";return false;}body=&r->physical_owner_v62->native();return true;};
  error.clear();return true;
 };
 // Compose native auxiliary/container services before the ONE existing
 // combined class-factory connection. Keep this function's current FX,
 // profile, host and Character resource hooks unchanged.
 std::shared_ptr<SourceWorldBorrowV61> source_world;
 if(!borrow_source_campaign_condition_world_v70(actual,source_world,e)||!source_world)return false;
 if(!source_world->noncharacter_inputs_v81){e="Required actual first-SWAMP non-Character source service producer";return false;}
 RendererCampaignNonCharacterInputsV69 native_inputs{};
 if(!source_world->noncharacter_inputs_v81(actual,native_inputs,e))return false;
 RendererCampaignNonCharacterCompositionV69 noncharacters;
 if(!compose_campaign_noncharacter_v69(assets,actual,std::move(native_inputs),noncharacters,e))return false;
 services.conditions=noncharacters.services->native.conditions; // SAME returned Main arena
 std::shared_ptr<RendererCharacterCandidateV60> transport;
 if(!RendererCharacterCandidateV60::connect(actual,std::move(services),std::move(noncharacters.factory),transport,e))return false;
 *transport_slot=transport;
 if(!transport->bind_cache_v67(cache,e))return false;
 return publish_campaign_character_transport_v62(transport,actual.actual_world,e);
}

