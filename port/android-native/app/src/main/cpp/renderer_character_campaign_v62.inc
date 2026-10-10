// Include in namespace model_renderer after V59/V60 and the lookup fragment.
// Required top-level headers: renderer_character_campaign_v62.hpp,
// character_candidate_cache_v62.hpp, canonical_character_pf_v62.hpp,
// campaign_navigation_registry_v64.hpp, visual_fx_manager_libraries_v63.hpp.
static bool borrow_campaign_character_physical_v90(void* raw,dh2::world::CanonicalZonePhysicalServicesV82& out,std::string& e){
 if(!raw){e="Required actual Character physical callback receiver";return false;}
 auto& record=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);
 SourceCampaignCandidateBorrowV55 actual;std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=record.services.world||
    !borrow_source_campaign_condition_world_v70(actual,world,e)){
  if(e.empty())e="Character physical callback addressed another campaign";return false;
 }
 return borrow_source_campaign_physical_services_v90(world,out,e);
}
bool bind_campaign_character_providers_v62(AAssetManager* assets,const SourceCampaignCandidateBorrowV55& actual,std::string& e){
 if(!assets||!actual.actual_world||!actual.application||!actual.floors||!actual.roots||!actual.physical_world||!actual.navigation_registry){e="Required actual campaign cache/World/roots/PF/physical producers";return false;}
 auto world=std::static_pointer_cast<WorldScriptContext>(actual.actual_world);
 if(!world||!world->design||!world->skills_owner||!world->objects){e="Required actual source World tables and Lua facade";return false;}
 std::shared_ptr<SourceWorldBorrowV61> source_world;
 if(!borrow_source_campaign_condition_world_v70(actual,source_world,e)||!source_world)return false;
 if(source_world->character_cache_v116){e="Actual Character cache constructor prefix already produced; cannot replace/replay";return false;}
 MenuPreviewProcessDomainV121 process;
 if(!borrow_native_menu_preview_domain_v121(actual.application,assets,world->files_directory,process,e))return false;
 auto cache=process.character_cache;
 if(!cache||!cache->ready()||!process.animation_manager){e="Required SAME process Character tables/animation directory";return false;}
 const auto files=cache->files();
 source_world->character_cache_v116=cache;
 if(!world->script_files){if(!world->script_assets.load(files,world->skills_owner->borrow(),e))return false;world->script_files=world->script_assets.borrow();}
 auto animation_sets=process.animation_manager;
 const auto floors=actual.floors;const auto roots=actual.roots;
 const auto navigation=actual.navigation_registry;
 dh2::world::CanonicalCharacterCandidateServicesV60 services;
 services.melee_animation_v101=[](auto& record,const auto& request,auto& response,auto& error){return source_campaign_melee_event_v115(record.services.world,record.actor->object->identity,request,response,error);};
 services.remaining=[](auto& record,const auto& request,auto&,std::string& error){
  if(request.source_entry==0x3b4834)return source_campaign_character_init_light_material_v113(record.services.world,record.actor->object->identity,error);
  if(request.source_entry==0x3b49f8){
   // InitFinal3b49f8..3b4a84 selects ONE local player's Character660.
   // This rejoins the common light/CharAI/local-player tail at3b4a88;
   // Level.PlaceFaeryAndFollowers3f0898 is a different list operation.
   SourceCampaignCandidateBorrowV55 actual;std::shared_ptr<SourceWorldBorrowV61> world;
   if(!record.actor||!record.actor->object||request.subject!=record.actor->object->identity||
      !borrow_source_campaign_candidate_v55(actual,error)||actual.actual_world!=record.services.world||!actual.application||
      !borrow_source_campaign_condition_world_v70(actual,world,error)||!world||!world->player_manager||
      world->player_manager!=actual.application->source_player_manager_v59()){
    if(error.empty())error="Required SAME current Character InitFinal PlayerManager";return false;
   }
   dh2::player::PlayerInfoFieldsV1* info{};
   if(!world->player_manager->get_local_player(0,true,info,error)||!info){if(error.empty())error="Required source GetLocalPlayer(0,true) PlayerInfo";return false;}
   const auto character=info->character660;if(!character){error.clear();return true;}
   std::array<float,3> direction{},position{};
   if(!source_campaign_character_look_at_v68(record.services.world,character,direction,error)||
      !source_campaign_character_target_position_v114(record.services.world,character,position,error))return false;
   for(unsigned axis=0;axis<3;++axis)direction[axis]=direction[axis]+position[axis];
   return record.set_position(direction,true,error);
  }
  error="Required original canonical Character InitPost continuation "+std::to_string(request.source_entry);return false;
 };
 services.destroy_character_save_v107=[weak=std::weak_ptr<WorldScriptContext>(world)](auto& record,std::uintptr_t captured,std::string& error){
  auto actual=weak.lock();if(!actual||record.services.world!=actual||!require_source_campaign_class_delivery_v106(actual,error))return false;
  return dh2::character::destroy_canonical_character_save_v108(record,captured,error);
 };
 if(!bind_campaign_character_sound_names_v101(cache,services,e))return false;
 services.state=[](auto& record,auto& output,auto& error){SourceCampaignCandidateBorrowV55 actual;
  if(!borrow_source_campaign_candidate_v55(actual,error)||actual.actual_world!=record.services.world){if(error.empty())error="Required SAME current source candidate at FSM C1";return false;}
  return bind_campaign_character_fsm_v101(record,actual,output,error);
 };
 services.state_thresholds_v101=[weak=std::weak_ptr<WorldScriptContext>(world)](float& walk,float& run,std::string& error){auto actual=weak.lock();if(!actual){error="Released actual DesignSettings thresholds";return false;}
  const auto index=actual->settings.row_index("Default");const auto* walk_word=index>=0?actual->settings.word(static_cast<unsigned>(index),23):nullptr;const auto* run_word=index>=0?actual->settings.word(static_cast<unsigned>(index),24):nullptr;
  if(!walk_word||!run_word){error="Required actual DesignSettings Default source5c/60";return false;}std::memcpy(&walk,walk_word,4);std::memcpy(&run,run_word,4);return true;
 };
 services.path_policy_v101=[weak=std::weak_ptr<WorldScriptContext>(world)](unsigned& enabled,std::string& error){auto actual=weak.lock();unsigned profile{};
  if(!actual||!actual->debug||!actual->effect_modules||dh2_character_debug_load(actual->debug.get(),&actual->debug_files)!=1||dh2_character_debug_get(&profile,actual->debug.get(),"PF_ProfileSearches",&actual->debug_files)!=1){error="Required actual PFWorld source Debug query";return false;}
  if(profile){error="Required original positive PFWorld search profiling statistics";return false;}return dh2_fx_debug_module_get(&enabled,actual->effect_modules.get(),"PathFinding_Search")==1;
 };
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
 services.initialize_target_marker_v70=[world=std::weak_ptr<WorldScriptContext>(world),libraries=actual.fx_libraries](auto& record,bool& present,std::string& error){
  if(!record.services.models){error="Required actual Arrays Models collection before target marker block";return false;}
  present=!record.services.models->names.empty();if(!present)return true;
  if(record.target_marker_v70){error="Character target marker initialization cannot replay";return false;}
  auto actual=world.lock();dh2::fx::CharacterMeshFxOwnerV4* manager{};
  if(!actual||!libraries||!record.actor||!model_renderer::borrow_source_campaign_fx_v77(actual,record.target_fx_pin_v70,manager,error))return false;
  record.target_fx_manager_v70=manager;
  dh2::character::TargetMarkerServicesV28 callbacks;callbacks.context=&record;
  callbacks.interaction_type=[](void* context,std::uintptr_t target,std::uintptr_t player,std::int32_t& type,std::string& error){
   auto& record=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(context);
   auto world=record.services.world_targets;if(!world||!record.actor||record.actor->object->identity!=player){error="Required SAME canonical target marker player/World";return false;}
   dh2::character::skills::WorldTargetActorBorrowV1 actor{},self{};
   if(world->actor(target,&actor)||world->actor(player,&self)||!actor.character||!self.character){error="Required actual target Character interaction receiver";return false;}
   auto& owner=world->targets();auto services=owner.query_services();const auto& types=owner.source_ai_types_borrow_v4();
   if(dh2::character::skills::dh2_character_skill_target_query_live_flags_v6(&type,dh2::target_providers::interaction_type,actor.character,self.character,&types,&services)){error="Canonical target interaction failed: "+owner.error();return false;}return true;
  };
  // Item tooltip is a distinct positive Item receiver endpoint. No fabricated
  // text is supplied before that source object family is actually published.
  record.target_marker_v70=std::make_unique<dh2::character::CharacterTargetMarkerV28>(*manager,libraries->source_tables(),record.actor->object->identity,callbacks);
  if(!record.target_marker_v70->initialize(error))return false;
  bool local{};if(!record.services.is_local_player||!record.services.is_local_player(record.actor->object->identity,local,error))return false;
  if(local){
   auto* cross=record.actor->source_target_cross149c_v70();if(!cross){error="Required actual Character C1 target-cross149c cell";return false;}
   const auto default_row=actual->settings.row_index("Default");const auto field=actual->settings.field_index("TargetMove");
   const auto* selected=default_row>=0&&field>=0?actual->settings.word(static_cast<std::uint32_t>(default_row),static_cast<std::uint32_t>(field)):nullptr;
   if(!selected){error="Required SAME actual DesignSettings.Default.TargetMove88";return false;}
   std::int32_t set{};std::memcpy(&set,selected,4);
   if(!manager->grab_marker_v28(set,0,*cross,error))return false;
   if(*cross){const auto& origin=dh2::world::canonical_vec3_origin_v1();
    if(!manager->marker_rotation_v70(*cross,origin.data(),error)||!manager->marker_sync_v83(*cross,false,error)||
       !manager->marker_visible_v28(*cross,false,error)||!manager->marker_loop_v76(*cross,true,error))return false;
   }
  }
  return true;
 };
 services.initialize_highlight_v70=[world=std::weak_ptr<WorldScriptContext>(world),application=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(actual.application),libraries=actual.fx_libraries](auto& record,std::string& error){
  if(!record.actor){error="Required SAME Character highlight14a0 receiver";return false;}
  auto& highlight=record.actor->source_highlight14a0();if(highlight)return true;
  auto app=application.lock();auto pm=app?app->source_player_manager_v59():nullptr;dh2::player::PlayerInfoFieldsV1* info{};
  if(!pm||!pm->manager()->get_by_character(record.actor->object->identity,false,info,error)||!info){if(error.empty())error="Required actual PM GetPlayerByCharacter for highlight";return false;}
  const auto friendly=static_cast<std::uint32_t>(info->friendly678);if(friendly>3)return true;
  if(!libraries){error="Required SAME Effects names for highlight";return false;}
  const auto& names=libraries->source_tables().set_names();if(names.empty())return true;
  const auto found=std::find(names.begin(),names.end(),"multiplayer_player01_glow");if(found==names.end())return true;
  const auto index=static_cast<std::int32_t>(found-names.begin())+static_cast<std::int32_t>(friendly);
  auto actual=world.lock();dh2::fx::CharacterMeshFxOwnerV4* manager{};
  if(!actual||!model_renderer::borrow_source_campaign_fx_v77(actual,record.target_fx_pin_v70,manager,error))return false;
  record.target_fx_manager_v70=manager;
  if(!manager->grab_marker_v28(index,0,highlight,error))return false;if(!highlight)return true;
  const auto& origin=dh2::world::canonical_vec3_origin_v1();
  return manager->marker_rotation_v70(highlight,origin.data(),error)&&manager->marker_sync_v83(highlight,false,error)&&
   manager->marker_visible_v28(highlight,false,error)&&manager->marker_store_anchor_v83(highlight,record.actor->object->identity,error)&&
   manager->marker_sync_v83(highlight,true,error)&&manager->marker_loop_v76(highlight,true,error);
 };
 services.initialize_sounds_v70=[cache](auto& record,std::string& error){
  // Whole _InitSounds3b3b00 selects actual CharSounds first, even when the
  // nullable global sound manager causes the subsequent literal return.
  const auto* sounds=cache->sounds_v70().get(record.properties->resolved[9]);
  if(!sounds){error="Required actual Character GetCharSounds fallback2";return false;}
  dh2::audio::AudioApplicationBorrowV42 audio;
  if(!model_renderer::borrow_actual_application_audio_v42(audio,error))return false;if(!audio.manager)return true;
  // Source source-row order is hit(+c), death(+4), impact1(+14), impact2(+1c).
  for(const auto* list:{&sounds->hit,&sounds->death,&sounds->impact1,&sounds->impact2}){
   for(std::uint32_t i=0;i<list->count;++i){
    if(!list->ids){error="Invalid actual CharSounds list";return false;}
    // Each source call reloads the process manager, rather than keeping an
    // earlier manager identity across resource-provider callbacks.
    if(!model_renderer::borrow_actual_application_audio_v42(audio,error))return false;
    if(!audio.manager){error="Original InitSounds sound manager disappeared during delivery";return false;}
    if(!audio.precache_raw_uid(list->ids[i],error))return false;
   }
  }
  return true;
 };
 std::shared_ptr<dh2::world::NativeConditionRuntimeV69> condition_runtime;
 if(!lend_campaign_condition_bag_v75(actual,services.conditions,condition_runtime,e)||
    !bind_source_campaign_module_conditions_runtime_v75(actual,services.conditions,e))return false;
 auto transport_slot=std::make_shared<std::weak_ptr<RendererCharacterCandidateV60>>();
 services.finish_player_profile_v67=[transport_slot](auto& record,auto& error){auto transport=transport_slot->lock();if(!transport||!record.actor||!record.actor->object){error="Required retained canonical player transport at completed InitPost";return false;}return transport->finish_selected_profile_v67(record.actor->object->identity,error);};
 services.world=world;services.design=world->design.get();services.models=&cache->models();services.animation_tables=&cache->animation_tables();services.loot_tables=&cache->loot();services.physical_world=actual.physical_world;
 if(source_world->character_targets_v116){e="Actual campaign target-directory constructor prefix already produced";return false;}
 services.world_targets=std::make_shared<dh2::character::skills::CharacterWorldRuntimeV1>(*world->tables.ai());
 source_world->character_targets_v116=services.world_targets;
 services.world_targets->bind_combat_sound_v112([weak=std::weak_ptr<void>(world)](std::uintptr_t attacker,std::uintptr_t target,const dh2::data::CombatResult& result,std::string& error){
  auto actual=weak.lock();if(!actual){error="Released canonical combat sound World";return false;}
  return source_campaign_combat_sound_v112(actual,attacker,target,true,result,error);
 });
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
  collision_services.cancel_sneaking=[](void* raw,std::uintptr_t identity,auto& error){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!r.actor||identity!=r.actor->object->identity){error="Collision CancelSneaking requires SAME canonical Character";return -1;}if(r.player_script_owner_v62){const auto status=r.player_script_owner_v62->native_cancel_sneaking(&r.target_character_v62.interactive415);if(status!=0){error=r.player_script_owner_v62->error();return -1;}return 0;}
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
  physical.peer_owner=[](void* raw,void* context,std::uintptr_t& identity,auto& error){
   dh2::world::CanonicalZonePhysicalServicesV82 common;
   if(!borrow_campaign_character_physical_v90(raw,common,error))return false;
   if(!common.peer_owner){error="Required actual typed physical peer owner";return false;}
   return common.peer_owner(context,identity,error);
  };
  physical.enabled80=[](void* raw,std::uintptr_t identity,std::uint8_t& value,auto& error){
   dh2::world::CanonicalZonePhysicalServicesV82 common;
   if(!borrow_campaign_character_physical_v90(raw,common,error))return false;
   if(!common.peer_visible80){error="Required actual typed physical peer visible80";return false;}
   return common.peer_visible80(identity,value,error);
  };
  physical.update_pf=[](void* raw,std::uintptr_t identity,const dh2::physical::NativeBody* body,const dh2::physical::NpcBodyProjection*,auto& error){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!r.actor||r.actor->object->identity!=identity||!r.physical_owner_v62||body!=&r.physical_owner_v62->native()||!r.services.inherited){error="Required SAME actual physical/PF assignment";return -1;}dh2::world::GameObjectInitializationServicesV1 services;if(!r.services.inherited(r,services,error)||!services.update_pf_object||!services.update_pf_object(error))return -1;return 0;};
  error.clear();return true;
 };
 services.controller=[](auto& out,auto& record,auto& error){if(!record.actor||!record.actor->controller){error="Required actual canonical controller receiver";return false;}auto* fields=record.actor->controller->command_state(controller_global_blocked);if(!fields){error="Invalid source global controller-block byte";return false;}out=*fields;return true;};
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> weak_app=actual.application;
 std::weak_ptr<WorldScriptContext> weak_world=world;
 services.npc_script=[weak_world,weak_app](auto& record,auto& input,auto& error){
  auto same=weak_world.lock();if(!same||!same->script_files||!same->objects||!record.actor||!record.actor->object){error="Required SAME canonical NPC World script cache and object facade";return false;}
  // Match the SAME resolved AI row used by CharacterScriptSession; never
  // publish monster bytecode under an unrelated companion/boss script name.
  const auto design=same->design->borrow();
  const auto* ai=record.properties&&design.ai()?dh2::data::ai_props(*design.ai(),record.properties->resolved[1]):nullptr;
  if(!ai){error="Required actual NPC resolved AI row before script resource selection";return false;}
  std::string external_path;input.common=same->script_files.common();input.external={};
  if(!ai->script.empty()&&ai->script.rfind("__",0)!=0){
   external_path="data/scripts/ai/"+ai->script;
   const char* suffix=std::strstr(ai->script.c_str(),".lua");
   if(!suffix)external_path+=".luac";else if(std::strncmp(suffix,".luac",5))external_path+='c';
   const auto* external=same->script_files.find(external_path);
   if(!external){error="Actual cached selected NPC script unavailable: "+external_path;return false;}
   input.external={external->data(),external->size()};
  }
  if(!same->script_files.session_files(external_path,input.include_files,error))return false;
  input.temporary=dh2::character::character_properties_temp_global_v62();
    auto app=weak_app.lock();if(!app){error="Released SAME App before canonical script host binding";return false;}
  if(!record.host_context_v70)record.host_context_v70=std::make_shared<RendererCanonicalHostContextV70>(same,app);
  input.host=&std::static_pointer_cast<RendererCanonicalHostContextV70>(record.host_context_v70)->bindings;input.level=&same->level_services;
  input.target=&record.actor->object->binding;input.objects=&same->objects->services();
  if(!bind_source_campaign_register_summon_v81(same,input,error))return false;
  if(!bind_campaign_character_npc_events_v101(record,input,error))return false;
  if(!bind_source_character_projectile_natives_v112(same,record.actor->object->identity,input,record.projectile_bindings_v112,error))return false;
  if(!bind_source_character_combat_natives_v115(same,record.actor->object->identity,input,record.combat_bindings_v115,error))return false;
  error.clear();return true;
 };
 services.player_script=[weak_world,weak_app](auto& record,auto& output,auto& error){
  auto same=weak_world.lock();if(!same||!same->script_files||!same->objects||!record.actor||!record.actor->object||!record.save||!record.save_fields||*record.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(record.save.get())){error="Required SAME canonical Player Save14e8/cache/object facade before source VM";return false;}
  auto& input=output.session;input.common=same->script_files.common();  auto app=weak_app.lock();if(!app){error="Released SAME App before canonical script host binding";return false;}
  if(!record.host_context_v70)record.host_context_v70=std::make_shared<RendererCanonicalHostContextV70>(same,app);
  input.host=&std::static_pointer_cast<RendererCanonicalHostContextV70>(record.host_context_v70)->bindings;input.level=&same->level_services;input.target=&record.actor->object->binding;input.objects=&same->objects->services();
  if(!bind_source_campaign_register_summon_v81(same,input,error))return false;
  input.cached_file_context=&record;input.cached_file=[](void* raw,const char* name,dh2::data::Bytes* bytes,bool* found){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!name||!bytes||!found||!r.services.world)return -1;auto world=std::static_pointer_cast<WorldScriptContext>(r.services.world);if(!world||!world->script_files)return -1;const auto* file=world->script_files.find(name);*found=file!=nullptr;*bytes=file?dh2::data::Bytes{file->data(),file->size()}:dh2::data::Bytes{};return 0;};
  output.faeries=same->script_files.faeries();output.initialization.context=&record;
  output.initialization.vitals=[](void* raw,dh2::character::CharacterScriptSessionV3& session,const dh2::character::ScriptLifecycleRequest32& q){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(q.service!=dh2::character::script_refresh_vitals||!r.actor||q.subject!=r.actor->object->identity||session.timers().owner!=q.subject||session.properties()!=r.properties||!r.services.effect_services)return -1;const dh2::character::ScriptInitVitals32 request{q.subject,&session.property_view(),*r.services.effect_services};dh2::character::ScriptInitVitals24 result{};return dh2_character_script_init_vitals(&result,&request)==1?0:-1;};
  output.initialization.gameplay.ai=&record.actor->ai_events;
  if(!bind_campaign_character_player_events_v101(record,output.initialization.gameplay,error))return false;
  record.skill_owner_view_v68.character=record.actor->object->identity;record.skill_owner_view_v68.flags=record.actor->machine->state().flags;
  output.initialization.gameplay.skill_owner=&record.skill_owner_view_v68;
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record.shared_from_this();output.initialization.gameplay.live_character_flags_v68=[weak](std::uint32_t& flags){auto r=weak.lock();if(!r||!r->actor||!r->actor->machine)return false;flags=r->actor->machine->state().flags;return true;};
  output.initialization.gameplay.difficulty_context=&record;output.initialization.gameplay.difficulty=[](void* raw,std::int32_t* out){auto& r=*static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);if(!out||!r.actor||!r.save_fields)return -1;return dh2::player::character_game_difficulty_v29({r.actor,r.actor->object->identity,r.save_fields->save_slot14e8(),r.services.difficulty_global},*out,r.error)?0:-1;};
  // Native combat/state/event providers are attached below by their actual
  // same-record owners. Absent callbacks remain source required failures;
  // resource/VM construction never substitutes a development player backend.
  if(!bind_source_character_projectile_natives_v112(same,record.actor->object->identity,input,record.projectile_bindings_v112,error))return false;
  if(!bind_source_character_combat_natives_v115(same,record.actor->object->identity,input,record.combat_bindings_v115,error))return false;
  error.clear();return true;
 };
 services.equipment_inputs=[weak_app,cache](auto& record,auto& out,auto& error){auto app=weak_app.lock();if(!app){error="Expired actual Application before canonical Gear preparation";return false;}return make_canonical_equipment_inputs_v62(app,cache,record,out,error);};
 services.release_player_equipment=[](auto& record,auto& error){
  if(!record.equipment||record.equipment.get()!=record.prepared_equipment_v60){error="Required teardown of externally transferred SAME Gear before canonical visual release";return false;}
  record.equipment->detach_visual();record.equipment.reset();record.inventory37c=nullptr;error.clear();return true;
 };
 services.initial_height=[floors](const float* p,bool& found,float& height,std::string& error){dh2::navigation::HeightHit actual{};const auto status=dh2_nav_world_height(&actual,&floors->collision_world,p,0);if(status<0){error="Actual Character initial PF floor query failed";return false;}found=status!=0;if(found)height=actual.height;return true;};
 services.inherited=[floors,navigation,roots](auto& record,auto& output,auto& error){
  output.set_visible=[world=std::weak_ptr<void>(record.services.world),id=record.actor->object->identity](bool visible,std::string& error){auto actual=world.lock();if(!actual){error="Released actual Character before source SetVisible";return false;}return source_campaign_character_set_visible_v96(actual,id,visible,error);};
  output.device_high_performance=[](bool& value,std::string& error){return model_renderer::borrow_actual_device_high_performance_v54(value,error);};
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record.shared_from_this();
  output.update_pf_object=[weak,floors,navigation](auto& error){auto r=weak.lock();if(!r){error="Expired actual Character PF receiver";return false;}return dh2::world::canonical_character_update_pf_v62(*r,floors->collision_world,navigation->registry(),error);};
  output.init_pf_object=[weak,floors](bool,const float* p,float radius,std::uintptr_t identity,auto& error){auto r=weak.lock();if(!r||!r->actor||r->actor->object->identity!=identity){error="Required same Character PF InitObject identity";return false;}return dh2::world::canonical_character_init_pf_v62(*r,floors->collision_world,p,radius,error);};
  const auto visual=[weak](std::uintptr_t identity,std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>& out,std::string& e){
   auto r=weak.lock();out=r&&r->visual?r->visual->visual():nullptr;
   if(!r||!r->actor||!out||r->actor->source_visual()!=identity||reinterpret_cast<std::uintptr_t>(out.get())!=identity){e="Required SAME Character source VisualObject2d8 for inherited light/root delivery";return false;}
   e.clear();return true;
  };
  output.light_set_id=[scene=std::weak_ptr<dh2::world::GameObjectSceneRootRegistryV1>(roots)](const std::string& name,std::int32_t& id,std::string& e){
   auto manager=scene.lock();auto names=manager?manager->source_light_names_v89():nullptr;
   if(!names){e="Required SAME Scene LightSetManager name owner";return false;}
   id=names->get_id(name);e.clear();return true;
  };
  output.visual_set_light_set=[visual](std::uintptr_t id,std::int32_t set,std::string& e){
   std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> owner;if(!visual(id,owner,e))return false;owner->store_light_set(set);e.clear();return true;
  };
  output.visual_root=[visual](std::uintptr_t id,std::uintptr_t& root,std::string& e){
   std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> owner;if(!visual(id,owner,e))return false;root=owner->root_identity();e.clear();return true;
  };
  output.node_from_name=[weak,visual](std::uintptr_t root,const char* name,std::uintptr_t& node,std::string& e){
   auto r=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> owner;
   if(!r||!r->actor||!visual(r->actor->source_visual(),owner,e))return false;
   if(owner->root_identity()!=root){e="Required SAME Character root for inherited named-node lookup";return false;}
   return owner->node_from_name(name,node,e);
  };
  error.clear();return true; // Concrete services bound; not InitPost completion.
 };
 const auto fx_libraries=actual.fx_libraries;
 services.visual=[cache,roots,floors,navigation,animation_sets,fx_libraries](auto& record,auto& output,auto& error){
  output.step_fx={&record,source_campaign_animation_step_v100};
  output.events={&record,source_campaign_character_animation_event_v101};
  output.world=record.services.world;output.scene_manager=roots;output.animation_manager=animation_sets;
  output.add_animation_trace_v116=[weak=std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(record.shared_from_this())](std::string& e){
   auto r=weak.lock();std::uint32_t value{};
   if(!r||!r->services.debug||!r->services.debug_files||dh2_character_debug_load(r->services.debug,r->services.debug_files)!=1||
      dh2_character_debug_get(&value,r->services.debug,"isTracingAnimSetManager",r->services.debug_files)!=1){e="Required original AnimSetManager AddAnim Debug load/query";return false;}
   e.clear();return true;
  };
  output.animation_dictionary=&cache->animations();
  auto sound_at_step=std::make_shared<dh2::audio::AudioApplicationBorrowV42>();
  output.registration.current_sound_manager=[sound_at_step](bool&present,std::string&error){
   if(!model_renderer::borrow_actual_application_audio_v42(*sound_at_step,error))return false;
   present=bool(sound_at_step->manager);error.clear();return true;
  };
  output.animation_read=[cache](const auto& path,auto& bytes,auto& error){bool found{};if(!cache->files().read(path,found,bytes,error))return false;if(!found){error="Actual authored Character animation asset unavailable: "+path;return false;}return true;};
  output.visual.owner=record.services.world;
  output.visual.read_asset=[cache](const std::string& path,std::vector<std::uint8_t>& bytes,bool& found,std::string& error){return cache->files().read(path,found,bytes,error);};
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record.shared_from_this();
  output.update_pf=[weak,floors,navigation](auto& error){auto r=weak.lock();if(!r){error="Expired actual Character visual/PF receiver";return false;}return dh2::world::canonical_character_update_pf_v62(*r,floors->collision_world,navigation->registry(),error);};
  // Character::IsAnimated is the original virtual at libDungeonHunter2.so
  // 0x3a2ee0; IDA shows `MOV R0, #1; BX LR` (always true).
  output.visual.parent_is_animated=[](bool& value,std::string& error){value=true;error.clear();return true;};
  output.registration.invoke=[weak,fx_libraries,sound_at_step](const auto& q,auto& error){auto r=weak.lock();if(!r||!r->services.debug||!r->services.debug_files){error="Required actual Character animation Debug owner";return false;}
   using O=dh2::character::NpcAnimationRegistrationOperationV6;
   if(q.operation==O::debug_load)return dh2_character_debug_load(r->services.debug,r->services.debug_files)==1;
   if(q.operation==O::debug_trace){std::uint32_t value{};if(dh2_character_debug_get(&value,r->services.debug,"isTracingChar_Init",r->services.debug_files)!=1){error="Required source animation registration Debug query";return false;}return true;}
   if(q.operation==O::preload_fx){if(!fx_libraries){error="Required SAME Application animation FX registration owner";return false;}return fx_libraries->register_set_to_load(q.resource,error);}
   if(q.operation==O::preload_sound){
    // Consume the SAME receiver captured by the reached3c9ea0 global read.
    // Clearing the retained slot before IO prevents World->audio lease cycles;
    // the local borrow still pins this receiver across LoadSound callbacks.
    auto audio=std::move(*sound_at_step);*sound_at_step={};
    if(!audio.manager){error="Required reached animation LoadSound receiver";return false;}
    return audio.precache_raw_uid(q.resource,error);
   }
   error="Required source Character animation sound/FX registration continuation";return false;
  };
  error.clear();return true; // Actual resource loaders bound; no playback claim.
 };
 services.position=[campaign=std::weak_ptr<void>(actual.actual_world)](auto& record,auto& output,auto& error){
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record.shared_from_this();
  output.physical_body=[weak](std::uintptr_t identity,dh2::physical::NativeBody*& body,auto& error){auto r=weak.lock();if(!r||!r->physical_owner_v62||identity!=reinterpret_cast<std::uintptr_t>(r->physical_owner_v62.get())){error="Required exact Character physical2dc receiver";return false;}body=&r->physical_owner_v62->native();return true;};
  // CharacterC1 InitCam publishes an actual Anchor at Character+0x2e0.
  // SpawnPoint::PlaceObject later reaches Character::SetPosition, whose
  // attached branch mutates that same Anchor's +0x0c XYZ by the position
  // delta. Resolve by the published anchor identity and pin its source owner.
  output.attached_position=[campaign](std::uintptr_t identity,float*& xyz,std::string& error){
   auto world=campaign.lock();if(!world){xyz=nullptr;error="Expired SAME campaign World for Character attached Anchor";return false;}
   return source_campaign_anchor_attached_position_v75(world,identity,xyz,error);
  };
  error.clear();return true;
 };
 std::shared_ptr<RendererCharacterCandidateV60> transport;
 if(!source_world->noncharacter_inputs_v81){e="Required actual Swamp non-Character source producer";return false;}
 RendererCampaignNonCharacterInputsV69 inputs;
 if(!source_world->noncharacter_inputs_v81(actual,inputs,e))return false;
 RendererCampaignNonCharacterCompositionV69 noncharacters;
 if(!compose_campaign_noncharacter_v69(assets,actual,std::move(inputs),noncharacters,e))return false;
 if(source_world->noncharacter_release_provider_v92&&!source_world->noncharacter_release_provider_v92->attach(actual,noncharacters,e))return false;
 if(source_world->noncharacter_owners_v105){e="Non-Character owners already published for this World";return false;}
 source_world->noncharacter_owners_v105=std::make_shared<dh2::loader::ProductionNonCharacterOwnersV67>(noncharacters.owners);
 // Stage34 was assembled during bootstrap, before these catalog owners were
 // published. Enroll the same existing typed loan on its live service bag.
 auto checkpoint_bag=source_world->stage34_native_v80;
 if(!checkpoint_bag||!noncharacters.borrow_spawn_point_v80){e="Required actual Stage34 bag and typed SpawnPoint loan";return false;}
 if(!checkpoint_bag->checkpoint.spawn_point)checkpoint_bag->checkpoint.spawn_point=
  [borrow=noncharacters.borrow_spawn_point_v80](const dh2::world::CanonicalObjectBorrowV1& actor,
   std::shared_ptr<dh2::world::CanonicalSpawnPointV15>& out,std::string& e){
   out.reset();if(!borrow(actor.identity,out,e))return false;
   if(!out||out->base().identity()!=actor.identity||out.owner_before(actor.lease)||actor.lease.owner_before(out)){
    out.reset();e="Foreign actual checkpoint SpawnPoint loan";return false;
   }
   e.clear();return true;
  };
 //Actual optional upstream behavior provider is composed before any Module
 //XML/C1. Absence is not completed room initialization; positive saved Module
 //tails and generated room creation still require this same retained sibling.
 if(source_world->generated_room_native_v92){CampaignGeneratedRoomNativeV92 native;
  if(!source_world->generated_room_native_v92(actual,native,e)||!bind_campaign_generated_rooms_complete_v92(actual,noncharacters,source_world,std::move(native),source_world->noncharacter_release_journal_v89,source_world->module_room_zones_v91,e))return false;
 }
 if(!install_campaign_object_loading_inputs_v95(source_world,noncharacters,source_world->module_room_zones_v91,e))return false;
 SourceNonCharacterRestoreLenderV89 native_restore;
 if(!make_campaign_noncharacter_restore_lender_v91(noncharacters,native_restore,e))return false;
 if(!bind_campaign_noncharacter_saved_objects_v89(actual,noncharacters,
    source_world->noncharacter_release_journal_v89,std::move(native_restore),e,source_world->module_room_zones_v91))return false;
 services.conditions=noncharacters.services->native.conditions;
 if(!RendererCharacterCandidateV60::connect(actual,std::move(services),std::move(noncharacters.factory),transport,e))return false;
 *transport_slot=transport;
 if(!transport->bind_cache_v67(cache,e))return false;
 // The original Stage6 PM.Update must already have its real preplayer
 // callbacks before the first source tick. It owns AddCharacter count/660
 // publication; Stage14 only places the subsequently existing characters.
 if(!RendererPlayerManagerStartupV70::bind(transport,actual,e))return false;
 if(source_world->object_update_inputs_v107){e="Canonical Character frame provider already published";return false;}
 source_world->object_update_inputs_v107=[](const SourceCampaignCandidateBorrowV55& scope,SourceObjectUpdateLeavesV105& out,std::string& e){
  return compose_source_campaign_character_frame_leaves_v111(scope,out,e);
 };
 if(!publish_campaign_character_transport_v62(transport,actual.actual_world,e))return false;
 if(!bind_source_campaign_script_initialization_v116(actual,e))return false;
 return bind_source_campaign_combat_v115(actual,e);
}
bool borrow_source_campaign_noncharacter_owners_v105(const SourceCampaignCandidateBorrowV55& actual,
 std::shared_ptr<dh2::loader::ProductionNonCharacterOwnersV67>& out,std::string& e){
 out.reset();std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(actual,world,e)||!world->noncharacter_owners_v105){
  if(e.empty())e="Required actual published class factory owners";return false;
 }
 out=world->noncharacter_owners_v105;e.clear();return true;
}
bool borrow_source_campaign_physical_filter_v105(const SourceCampaignCandidateBorrowV55& actual,
 std::uintptr_t id,std::shared_ptr<void>& pin,dh2::physical::NativePhysicalFilterBorrowV1& out,std::string& e){
 pin.reset();out={};std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(actual,world,e)||!world->physical_filter_v105){
  if(e.empty())e="Required same-world typed physical filter provider";return false;
 }
 return world->physical_filter_v105(id,pin,out,e);
}
bool borrow_source_campaign_frame_services_v106(const SourceCampaignCandidateBorrowV55& actual,
 std::uintptr_t id,std::shared_ptr<void>& pin,dh2::loader::GameObjectSourceFrameServicesV74& out,std::string& e){
 pin.reset();out={};std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(actual,world,e)||!world->noncharacter_frame_services_v106){
  if(e.empty())e="Required same-world source frame services";return false;
 }
 return world->noncharacter_frame_services_v106(id,pin,out,e);
}

