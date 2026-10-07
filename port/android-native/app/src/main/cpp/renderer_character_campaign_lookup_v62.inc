// Include in namespace model_renderer after the retained V60 transport. Root
// factory closure owns the transport; this current-world index is weak only.
namespace {
std::weak_ptr<RendererCharacterCandidateV60> campaign_character_transport_v62;
bool publish_campaign_character_transport_v62(const std::shared_ptr<RendererCharacterCandidateV60>& transport,const std::shared_ptr<void>& world,std::string& e){
 if(!transport||!transport->belongs_to_v62(world)){e="Required SAME actual campaign Character transport";return false;}
 auto prior=campaign_character_transport_v62.lock();
 if(prior&&prior.get()!=transport.get()&&prior->belongs_to_v62(world)){e="Campaign Character transport already published for this World";return false;}
 campaign_character_transport_v62=transport;return true;
}
}
bool retire_source_campaign_closed_character_v111(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 auto transport=campaign_character_transport_v62.lock();
 if(!transport||!transport->belongs_to_v62(world)){e="Required SAME closed Character transport";return false;}
 return transport->retire_closed_v111(id,e);
}
bool source_campaign_player_manager_integer714_v70(const std::shared_ptr<void>& world,std::int32_t& value,std::string& e){
 SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=world||!actual.application||!actual.application->source_player_manager_v59()){if(e.empty())e="Required SAME canonical App PM integer714";return false;}
 return dh2::player::player_manager_integer714_v70(*actual.application->source_player_manager_v59()->manager(),value,e);
}
bool source_campaign_all_players_dead_v70(const std::shared_ptr<void>& world,bool& value,std::string& e){
 SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=world||!actual.application||!actual.application->source_player_manager_v59()){if(e.empty())e="Required SAME canonical App PM AllPlayersDead";return false;}
 auto pm=actual.application->source_player_manager_v59();dh2::player::PlayerManagerDeathServicesV70 services;services.provider=pm;
 services.character_is_dead34=[world](std::uintptr_t id,bool& dead,std::string& error){SourceCampaignCharacterBorrowV62 actor;if(!borrow_source_campaign_character_v62(world,id,actor,error)||!actor.character->life){if(error.empty())error="Required SAME Character1449 IsDead owner";return false;}dead=actor.character->life->dead!=0;return true;};
 services.death_timer3a8=[pm](auto& info,const std::int32_t*& timer,std::shared_ptr<void>& pin,std::string& error){auto fields=pm->network()->borrow(info);if(!fields||!fields->managed_fields_produced_v70){error="Required actual PlayerInfo Reset death timer3a8";return false;}timer=&fields->character_death_timer3a8_v70;pin=fields;return true;};
 return dh2::player::player_manager_all_players_dead_v70(*pm->manager(),services,value,e);
}
bool source_campaign_character_init_all_v70(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 auto transport=campaign_character_transport_v62.lock();if(!world||!transport||!transport->belongs_to_v62(world)){e="Required SAME source World canonical Character InitAll";return false;}
 return transport->initialize_all_v70(id,e);
}
bool source_campaign_manage_characters_v70(const std::shared_ptr<void>& world,std::string& e){
 auto transport=campaign_character_transport_v62.lock();
 if(!world||!transport||!transport->belongs_to_v62(world)){e="Required SAME source World preplayer PM transport";return false;}
 return transport->manage_characters_v70(e);
}
bool borrow_source_campaign_character_targets_v88(const std::shared_ptr<void>& world,std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1>& out,std::string& e){
 SourceCampaignCandidateBorrowV55 actual;std::shared_ptr<SourceWorldBorrowV61> source;
 if(!world||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=world||
    !borrow_source_campaign_condition_world_v70(actual,source,e)||!source||!source->character_targets_v116){
  if(e.empty())e="Required SAME canonical World target-directory constructor prefix";return false;
 }
 auto transport=campaign_character_transport_v62.lock();
 if(transport&&transport->belongs_to_v62(world)&&transport->source_targets_v88()!=source->character_targets_v116){
  e="Published Character transport uses another target-directory owner";return false;
 }
 out=source->character_targets_v116;e.clear();return true;
}
bool borrow_source_campaign_character_v62(const std::shared_ptr<void>& world,std::uintptr_t character,SourceCampaignCharacterBorrowV62& out,std::string& e){
 std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> preview;bool process_domain{};
 if(!borrow_native_menu_preview_character_v122(world,character,preview,process_domain,e))return false;
 if(process_domain){out={world,std::move(preview)};e.clear();return true;}
 auto transport=campaign_character_transport_v62.lock();
 if(!world||!character||!transport||!transport->belongs_to_v62(world)){e="Required SAME current campaign Character factory transport";return false;}
 auto record=transport->actor(character);
 if(!record||!record->actor||!record->actor->object||record->actor->object->identity!=character||record->services.world.get()!=world.get()){
  e="Required actual canonical Character receiver for published PM Character660";return false;
 }
 out={world,std::move(record)};e.clear();return true;
}
bool borrow_source_campaign_animation_tables_v67(const std::shared_ptr<void>& world,std::shared_ptr<const void>& lease,const dh2::data::AnimationTables*& tables,std::string& e){
 auto transport=campaign_character_transport_v62.lock();if(!transport||!transport->belongs_to_v62(world)||!transport->cache_v67()||!transport->cache_v67()->ready()){e="Required SAME decoded campaign animation cache";return false;}
 lease=transport->cache_v67();tables=&transport->cache_v67()->animation_tables();e.clear();return true;
}
bool borrow_source_campaign_character_cache_v81(const std::shared_ptr<void>& world,std::shared_ptr<const dh2::character::CharacterCandidateCacheV62>& cache,std::string& e){
 cache.reset();SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> source;
 if(!world||!borrow_source_campaign_candidate_runtime_v61(candidate,e)||candidate.actual_world!=world||
    candidate.actual_world.owner_before(world)||world.owner_before(candidate.actual_world)||
    !borrow_source_campaign_condition_world_v70(candidate,source,e)||!source->character_cache_v116||
    !source->character_cache_v116->ready()){
  if(e.empty())e="Required SAME decoded campaign Character cache owner";return false;
 }
 auto transport=campaign_character_transport_v62.lock();
 if(transport&&transport->belongs_to_v62(world)&&transport->cache_v67()!=source->character_cache_v116){
  e="Published Character transport uses another decoded cache owner";return false;
 }
 cache=source->character_cache_v116;e.clear();return true;
}
bool borrow_source_campaign_camera_actor_v67(const std::shared_ptr<void>& world,std::uintptr_t character,SourceCampaignCameraActorBorrowV67& out,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 auto& record=*actual.character;auto& actor=*record.actor;
 const auto* disabled=actor.source_bool_field(0x81);
 if(!record.life||!disabled||!actor.position_fields_v7().constructed){e="Required produced SAME canonical Character camera position/anchor/disabled/life fields";return false;}
 out={actual.character,character,actor.source_position160_v7(),&actor.position_fields_v7().attached2e0,disabled,&record.life->dead};e.clear();return true;
}
bool borrow_menu_character_ooi_type_v62(const std::shared_ptr<void>& world,std::uintptr_t character,std::shared_ptr<void>& receiver,const std::int32_t*& cell,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 receiver=actual.character;cell=&actual.character->actor->animation_ai.owner_byte14a8;e.clear();return true;
}
bool source_campaign_character_look_at_v68(const std::shared_ptr<void>& world,std::uintptr_t character,dh2::camera::PointV2& out,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 // Whole393ae4: capture current EulerZ174 once, sinf then cosf, flip the
 // cos float's sign bit through source integer ADD80000000, then XYZ stores.
 const auto angle=actual.character->actor->runtime.rotation.rotation[2];const auto x=::sinf(angle);float y=::cosf(angle);std::uint32_t bits{};std::memcpy(&bits,&y,4);bits+=0x80000000u;std::memcpy(&y,&bits,4);out={x,y,0.f};e.clear();return true;
}
bool source_campaign_character_visual_up_v68(const std::shared_ptr<void>& world,std::uintptr_t character,dh2::camera::PointV2& out,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 const auto& record=*actual.character;
 if(!record.actor->source_visual()){
  auto retained=record.visual?record.visual->animation_visual_v112():nullptr;const std::array<float,16>* matrix{};
  SourceCampaignCandidateBorrowV55 candidate;
  if(!retained||!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!candidate.roots||!retained->borrow_batch_root_matrix_v113(matrix,e)||!matrix){if(e.empty())e="Audio visual Up requires SAME current compiled pose/root basis";return false;}
  //Lend the actual cached compiled pose basis. Do not resurrect visual2d8,
  //reparent an old root, or advance its animation/physics from audio delivery.
  out={(*matrix)[8],(*matrix)[9],(*matrix)[10]};e.clear();return true;
 }
 auto visual=record.visual?record.visual->visual():nullptr;
 if(!visual||!visual->ready()){e="Required SAME retained Character Root absolute matrix for camera Up";return false;}
 SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!candidate.roots||visual->root_parent_identity()!=candidate.roots->identity()){if(e.empty())e="Required actual SceneManager-parent Character Root for Up";return false;}
 // This is the SAME owner TRS producer used by SceneBinding::update_world.
 // Helper is translation-only; it does not change the +20 basis column.
 const auto& root=visual->binding().root;std::array<float,16> matrix{};dh2_node_matrix(matrix.data(),root.position,root.quaternion,root.scale);out={matrix[8],matrix[9],matrix[10]};e.clear();return true;
}
bool source_campaign_initialize_player_save_v67(const std::shared_ptr<void>& world,std::uintptr_t character,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 return campaign_character_transport_v62.lock()->initialize_player_save(character,e);
}
bool construct_source_campaign_character_spawn_v68(const std::shared_ptr<void>& world,const dh2::world::CanonicalFactoryEntryV1& entry,const char* name,dh2::world::CanonicalClassReceiverV1& out,std::string& e){
 auto transport=campaign_character_transport_v62.lock();if(!transport||!transport->belongs_to_v62(world)){e="Required SAME actual campaign source Spawn factory";return false;}return transport->construct_source_spawn_v68(entry,name,out,e);
}
bool source_campaign_character_set_initial_position160_v68(const std::shared_ptr<void>& world,std::uintptr_t character,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 auto& r=*actual.character;dh2::character::NpcInitPostResponseV1 response;
 return dh2::world::CanonicalCharacterCandidateRecordV60::init_service(&r,{0x3a58f4,0,0,character,reinterpret_cast<std::uintptr_t>(r.actor->source_position160_v7()),nullptr},response,e);
}
bool source_campaign_character_save_entrypoint_v68(const std::shared_ptr<void>& world,std::uintptr_t character,std::int32_t value,std::int32_t difficulty,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 auto& r=*actual.character;if(!r.save_fields){e="Required SAME source Character14e8 for SG_SetLevelEntryPoint";return false;}
 if(!*r.save_fields->save_slot14e8()){e.clear();return true;} // Source NULL-save BX before any difficulty read.
 if(!r.save||*r.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r.save.get())){e="Required SAME actual Save14e8 entrypoint receiver";return false;}
 if(difficulty==-1){if(!r.services.difficulty_global){e="Required PlayerSavegame global difficulty for SG_SetLevelEntryPoint";return false;}difficulty=r.services.difficulty_global->value();}
 return r.save->source_store_level_entrypoint_v68(value,difficulty,e);
}
bool source_campaign_initialize_selected_profile_v67(const std::shared_ptr<void>& world,std::uintptr_t character,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 return campaign_character_transport_v62.lock()->initialize_selected_profile_v67(character,e);
}
bool borrow_source_campaign_world_map_tables_v68(const std::shared_ptr<void>& world,std::uintptr_t character,std::shared_ptr<const dh2::data::WorldMapProfileTableV59>& out,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 auto transport=campaign_character_transport_v62.lock();out=transport->world_map_tables_v68(character);
 if(!out){e="Required SAME selected Character profile WorldMap tables";return false;}e.clear();return true;
}
bool source_character_saved_name_v67(const std::shared_ptr<void>& world,std::uintptr_t character,std::string& text,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 const auto& record=*actual.character;
 if(!record.save_fields||!record.save||*record.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(record.save.get())||record.save->character()!=character){e="Required SAME canonical Save14e8 for localized player name";return false;}
 text=record.save->name();e.clear();return true;
}
bool borrow_source_campaign_menu_continuations_v68(const std::shared_ptr<void>& world,std::uintptr_t character,std::shared_ptr<void>& lease,std::shared_ptr<dh2::data::PlayerSaveLoadOwnerV1>& load,dh2::character::CharacterMenuMutationServicesV4& mutations,dh2::character::CharacterMenuFaeryServicesV8& faery,std::string& e){
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
 auto record=actual.character;
 if(!record->init_complete||record->failed||!record->profile_bootstrap||!record->profile_bootstrap->finished()||!record->profile_bootstrap->campaign_writer()||!record->profile_bootstrap->campaign_writer()->ready()||!record->load||!record->player_script_owner_v62||record->player_script_owner_v62->native_savegame()!=record->save.get()){e="Required SAME initialized canonical player/menu/profile writer graph";return false;}
 SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!candidate.application){if(e.empty())e="Menu continuations require SAME actual Application/World";return false;}
 std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record;
 auto app=candidate.application;const auto online=app->get_online_loading_v55();
 mutations=record->services.menu_mutations_v68;mutations.owner=record;
 mutations.constant=[weak](const char* group,const char* key,std::int32_t& value,auto& error){auto r=weak.lock();const auto* design=r?r->design.design():nullptr;if(!design||!design->lookup||design->lookup(design->context,0,group,key,&value)){error="Required actual menu PyDataConstants value";return false;}error.clear();return true;};
 mutations.is_player=[weak](std::uintptr_t identity,bool& value,auto& error){auto r=weak.lock();if(!r||identity!=r->actor->object->identity){error="Menu IsPlayer requires SAME canonical Character";return false;}return r->is_player(value,error);};
 mutations.is_local_player=[pm=app->source_player_manager_v59()](std::uintptr_t identity,bool& value,auto& error){if(!pm){error="Required SAME Application PM local predicate";return false;}return pm->source_is_local_player_v61(identity,value,error);};
 mutations.online=[online](bool& value,auto& error){if(!online){error="Required actual GetOnline receiver";return false;}value=online->byte5()!=0;error.clear();return true;};
 mutations.save=record->profile_bootstrap->campaign_writer()->write_services();
 faery=record->services.menu_faery_v68;faery.owner=record;
 faery.difficulty=[weak](std::int32_t& value,auto& error){auto r=weak.lock();if(!r){error="Expired actual Character faery difficulty receiver";return false;}return dh2::player::character_game_difficulty_v29({r->actor,r->actor->object->identity,r->save_fields->save_slot14e8(),r->services.difficulty_global},value,error);};
 faery.faery_character=[weak](std::uintptr_t identity,std::uintptr_t& value,auto& error){auto r=weak.lock();if(!r||!r->faery_association_v68||r->faery_association_v68->character!=identity){error="Required SAME source Character420 faery field";return false;}value=r->faery_association_v68->faery420;error.clear();return true;};
 faery.current_level=[weak](std::uintptr_t& identity,auto& error){auto r=weak.lock();dh2::world::CharacterCurrentLevelBorrowV61 level;if(!r||!r->services.current_level||!r->services.current_level(level,error)){if(error.empty())error="Required actual current Level for NativeHUDSetActiveFaery";return false;}identity=level.identity;return true;};
 lease=record;load=record->load;e.clear();return true;
}
bool borrow_source_campaign_player_pre_gameplay_v68(const std::shared_ptr<void>& world,PlayerGameplayBinding& out,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;
 if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world||!candidate.application){if(e.empty())e="Canonical player binding requires SAME current campaign World";return false;}
 auto manager=candidate.application->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* local{};
 if(!manager||!manager->get_local_player(0,true,local,e)||!local||!local->character660){if(e.empty())e="Required actual local PlayerInfo Character660 publication";return false;}
 SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,local->character660,actual,e))return false;
 auto& r=*actual.character;
 if(!r.init_complete||r.failed||!r.equipment||!r.equipment->ready()||!r.player_script_owner_v62||!r.player_script_owner_v62->ready()||!r.actor->machine||!r.services.world_targets||!r.target_registered_v62||!r.save||!r.save_fields||*r.save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r.save.get())||r.player_script_owner_v62->native_savegame()!=r.save.get()||!r.profile_bootstrap||!r.profile_bootstrap->finished()){
  e="Required completed SAME source Character InitPost/VM/Gear/Save/profile writer/target publication";return false;
 }
 bool player{};if(!r.is_player(player,e)||!player){if(e.empty())e="Published local660 is not the actual source player";return false;}
 dh2::character::ControllerCommandState32 control{};if(!r.services.controller||!r.services.controller(control,r,e))return false;
 dh2::player::CharacterSaveDifficultyBorrowV29 difficulty{r.actor,r.actor->object->identity,r.save_fields->save_slot14e8(),r.services.difficulty_global};std::int32_t selected{};
 if(!dh2::player::character_game_difficulty_v29(difficulty,selected,e))return false;
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings;dh2::ui::HudTextEnvironmentV1 text;std::shared_ptr<void> presentation;
 if(!borrow_actual_gameplay_presentation_v67(world,settings,text,presentation,e))return false;
 PlayerGameplayBinding binding;binding.design=r.services.design->borrow();binding.gear=r.equipment.get();binding.skills=r.player_script_owner_v62.get();binding.save=r.save.get();binding.properties=&r.player_script_owner_v62->session().property_view();binding.life=r.life.get();binding.state=&r.actor->machine->state();binding.state_owner=&r.actor->machine->owner();binding.world_targets=r.services.world_targets.get();binding.controller=control;binding.character=r.actor->object->identity;binding.difficulty=selected;binding.settings=settings.get();binding.world_owner=world;binding.skill_tables=r.services.skills;binding.skill_list_index=r.properties->resolved.data()+28;binding.temporary=dh2::character::character_properties_temp_global_v62();binding.debug=r.services.debug;binding.debug_files=r.services.debug_files;binding.text_environment=text;binding.presentation_owner=presentation;
 if(!r.init_fields.properties_id13c8){e="Required SAME Character13c8 for menu base-property recalculation";return false;}
 binding.actor_index=*r.init_fields.properties_id13c8;
 binding.potion_capacity_context=actual.character.get();
 binding.potion_capacity_store=[](void* raw,std::int32_t value,std::string& error){
  auto* record=static_cast<dh2::world::CanonicalCharacterCandidateRecordV60*>(raw);
  if(!record||!record->init_complete||record->failed||!record->equipment||!record->equipment->ready()||record->prepared_equipment_v60!=record->equipment.get()||record->equipment->inventory()!=record->inventory37c||record->equipment->properties()!=record->properties){error="Required SAME initialized Character inventory3a8/Gear owner";return false;}
  record->equipment->project_potion_capacity(static_cast<std::int8_t>(static_cast<std::uint8_t>(value)));error.clear();return true;
 };
 // Source LoadMenu3 at stage26 must borrow real player fields before38.
 // This descriptor cannot admit gameplay/actions by constructing that view.
 binding.active=false;out=std::move(binding);e.clear();return true;
}
bool borrow_source_campaign_player_gameplay_v67(const std::shared_ptr<void>& world,PlayerGameplayBinding& out,std::string& e){
 PlayerGameplayBinding binding;if(!borrow_source_campaign_player_pre_gameplay_v68(world,binding,e))return false;
 SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_v55(candidate,e)||candidate.actual_world!=world){if(e.empty())e="Gameplay admission requires SAME current campaign";return false;}
 const auto manager=candidate.application->source_player_manager_v59();
 if(!source_campaign_loading_complete_v64()||!manager||!manager->count_field()||*manager->count_field()<=0){e="Required completed actual campaign load and source AddCharacter count before gameplay activation";return false;}
 binding.active=true;out=std::move(binding);e.clear();return true;
}




bool borrow_source_campaign_character_lifetime_v107(const std::shared_ptr<void>& world,std::uintptr_t id,
 std::weak_ptr<void>& out,std::string& e){
 SourceCampaignCharacterBorrowV62 b;if(!borrow_source_campaign_character_v62(world,id,b,e)||!b.character||!b.character->actor)return false;
 out=b.character->actor->native_lifetime_v107();if(out.expired()){e="Actual Character native allocation is retired";return false;}e.clear();return true;
}
