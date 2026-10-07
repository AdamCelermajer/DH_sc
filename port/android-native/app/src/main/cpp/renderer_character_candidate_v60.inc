// New source candidate class transport. Include after actual World/C1/PM and
// renderer_profile_bootstrap_v59.inc; link canonical_character_candidate_v60.
// Include renderer_application_save_files_v61.inc before this fragment.
// No old Crypt actor/Scene/descriptor is imported by this bridge.
class RendererCharacterCandidateV60:public std::enable_shared_from_this<RendererCharacterCandidateV60> {
 std::shared_ptr<RendererPlayerManageCharactersV70> manage_v70_;
 std::weak_ptr<WorldScriptContext> world_;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
 std::shared_ptr<dh2::world::CanonicalCharacterCandidateFactoryV60> characters_;
 model_renderer::SourceCampaignClassFactoryV60 other_;
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> selected_;
 std::shared_ptr<dh2::character::CharacterCandidateCacheV62> cache_v67_;
 std::map<std::uintptr_t,std::shared_ptr<RendererProfileBootstrapV59>> profiles_v67_;
public:
 static bool connect(const model_renderer::SourceCampaignCandidateBorrowV55& actual,
  dh2::world::CanonicalCharacterCandidateServicesV60 services,
  model_renderer::SourceCampaignClassFactoryV60 actual_other_classes,
  std::shared_ptr<RendererCharacterCandidateV60>& out,std::string& e){
  if(out||!actual.actual_world||!actual.application||!actual.level||!actual.objects||!actual.properties||!actual.selected_profile){
   e="Required once-only SAME produced source GS/C1/class graph/selected profile";return false;
  }
  auto world=std::static_pointer_cast<WorldScriptContext>(actual.actual_world);
  if(!world||services.world.get()!=world.get()||services.design!=world->design.get()){
   e="Character factory attempted a different World/design receiver";return false;
  }
  auto self=std::make_shared<RendererCharacterCandidateV60>();out=self;
  self->world_=world;self->application_=actual.application;self->selected_=actual.selected_profile;self->other_=std::move(actual_other_classes);
  self->manage_v70_=std::make_shared<RendererPlayerManageCharactersV70>();
  if(!self->manage_v70_->initialize(actual,e))return false;
  services.selected_profile_lease=self->selected_;services.selected_profile_class=&self->selected_->metadata.character_row;
  services.skills=world->skills_owner->borrow();services.debug=world->debug.get();services.debug_files=&world->debug_files;
  services.faeries_v70=world->script_files.faeries();
  services.character_effects=&world->effects->source().characters();services.effect_table=&world->effects->table();
  services.effect_queue=actual.fx_libraries?&actual.fx_libraries->pending_queue_v63():nullptr;services.effect_services=&world->effect_services;
  std::weak_ptr<WorldScriptContext> weak_world=world;
  services.publish_script_object=[weak_world](const auto& object,auto& error){auto same=weak_world.lock();
   if(!same||!same->objects){error="Required SAME source World Lua facade registry";return false;}
   return same->objects->publish_existing_v62(object,error);
  };
  std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> weak_app=actual.application;
  services.is_local_player=[weak_app](std::uintptr_t character,bool& local,auto& error){auto app=weak_app.lock();
   if(!app||!app->source_player_manager_v59()){error="Required SAME App PM for canonical IsLocalPlayer";return false;}
   return app->source_player_manager_v59()->source_is_local_player_v61(character,local,error);
  };
  services.model_name.high_performance=[](bool& value,std::string& error){return model_renderer::borrow_actual_device_high_performance_v54(value,error);};
  // Genuine lazy GetOnline producer; offline byte5 is source C1 storage,
  // not a campaign readiness flag or assumed false callback.
  const auto online=actual.application->get_online_loading_v55();
  services.online_byte5=[online](bool& value,std::string& error){if(!online){error="Required actual GetOnline receiver for spawn";return false;}value=online->byte5()!=0;return true;};
  const auto random=actual.application->source_random_v62();
  if(!random){e="Required SAME Application Random globals for canonical Character";return false;}
  services.random=&random->channel(0);services.alternate_random=&random->channel(1);
  services.random_lease=random;
  if(!actual.fx_libraries||!actual.fx_libraries->belongs_to(actual.application)){e="Required SAME Application Character FX library";return false;}
  const auto libraries=actual.fx_libraries;
  services.character_effects=&libraries->source_tables().characters();
  services.register_character_fx=[libraries,weak_world](auto& record,std::string& error){auto world=weak_world.lock();
   const auto& rows=libraries->source_tables().characters();if(!world||rows.empty()){error="Required actual Character Effects rows and Debug owner";return false;}
   auto index=record.properties->resolved[7];if(index<0||std::size_t(index)>=rows.size())index=0;
   const auto& row=rows[std::size_t(index)];const std::int32_t captured[]={row.footprint,row.blood,row.blood_death};
   std::uint32_t ignored{};if(dh2_character_debug_load(world->debug.get(),&world->debug_files)!=1||dh2_character_debug_get(&ignored,world->debug.get(),"isTracingChar_Init",&world->debug_files)!=1){error="Required original Character FX registration Debug prefix";return false;}
   for(auto id:captured)if(id>=0&&!libraries->register_set_to_load(id,error))return false;return true;
  };
  services.quest_sync.owner=online;
  services.quest_sync.online=[online](bool& value,auto& error){if(!online){error="Required actual GetOnline receiver";return false;}value=online->byte5()!=0;error.clear();return true;};
  services.quest_sync.is_local_player=services.is_local_player;
  services.current_level=[](dh2::world::CharacterCurrentLevelBorrowV61& out,auto& error){
   dh2::loader::CanonicalCurrentLevelBorrowV1 current;
   if(!model_renderer::borrow_current_native_level_v27(current,error))return false;
   out={};if(current){out.receiver=current.level();out.identity=current.identity();out.mode118=&current.level()->constructor_fields_v3().mode118;}
   return true;
  };
  if(!services.difficulty_global)services.difficulty_global=dh2::player::process_player_save_difficulty_global_v29();
  self->characters_=std::make_shared<dh2::world::CanonicalCharacterCandidateFactoryV60>(std::move(services));
  return model_renderer::bind_source_campaign_class_factory_v60(
   [self](const auto& entry,const auto& request,auto& receiver,auto& error){
    if(self->world_.expired()){error="Released actual Character source candidate";return false;}
    if(entry.name&&(!std::strcmp(entry.name,"Character")||!std::strcmp(entry.name,"Player")))
     return self->characters_->construct(entry,request,receiver,error);
    if(!self->other_){error=std::string("Required original canonical class constructor: ")+(entry.name?entry.name:"NULL");return false;}
  return self->other_(entry,request,receiver,error);
   },e);
 }
 bool manage_characters_v70(std::string& e){if(!manage_v70_){e="Required actual preplayer PM manage receiver";return false;}return manage_v70_->update(e);}
 bool belongs_to_v62(const std::shared_ptr<void>& actual)const{
  auto world=world_.lock();return world&&actual&&world.get()==actual.get()&&!world.owner_before(actual)&&!actual.owner_before(world);
 }
 bool bind_cache_v67(std::shared_ptr<dh2::character::CharacterCandidateCacheV62> cache,std::string& e){if(!cache||!cache->ready()||cache_v67_){e="Required once-only actual decoded canonical Character cache";return false;}cache_v67_=std::move(cache);return true;}
 const auto& cache_v67()const noexcept{return cache_v67_;}
 std::shared_ptr<const dh2::data::WorldMapProfileTableV59> world_map_tables_v68(std::uintptr_t id)const{const auto it=profiles_v67_.find(id);return it==profiles_v67_.end()||!it->second?nullptr:it->second->world_map_tables_v68();}
 std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> actor(std::uintptr_t id)const{
  return characters_?characters_->find(id):nullptr;
 }
 bool physical_peer_v68(void* context,std::uintptr_t& identity,std::string& e)const{return characters_&&characters_->physical_peer_v68(context,identity,e);}
 bool retire_closed_v111(std::uintptr_t id,std::string& e){return characters_&&characters_->retire_closed_v111(id,e);}
 std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> source_targets_v88()const{return characters_?characters_->source_targets_v88():nullptr;}
 bool construct_source_spawn_v68(const dh2::world::CanonicalFactoryEntryV1& entry,const char* name,dh2::world::CanonicalClassReceiverV1& out,std::string& e){
  if(!name||!characters_||world_.expired()){e="Required actual source Spawn Character caller name/factory";return false;}
  auto source_name=std::make_shared<const std::string>(name);dh2::world::CanonicalSourceObjectRequestV1 request;request.source_lease=source_name;request.element=UINT32_MAX;request.native_spawn_name_v68=source_name->c_str();
  // Only C1 here. CanonicalSpawnAttempt owns the original Add/property/default/
  // name/archetype/pending sequence; it must not replay XML overrides.
  return characters_->construct(entry,request,out,e);
 }
 bool source_ooi_type_v62(std::uintptr_t id,std::shared_ptr<void>& receiver,
  const std::int32_t*& type,std::string& e)const{
  auto record=actor(id);if(!record||!record->actor||record->actor->shared_handle().cached!=id){e="Required SAME canonical Character OOI type receiver";return false;}
  receiver=record;type=&record->actor->animation_ai.owner_byte14a8;e.clear();return true;
 }
 // Source PlayerManager initialize_save3b36b0 endpoint: operates on the real
 // spawned canonical receiver whose660 publication happened in AddCharacter.
 bool initialize_all_v70(std::uintptr_t id,std::string& e){
  auto record=actor(id);if(!record){e="Required SAME canonical Character for InitAll";return false;}
  if(!profiles_v67_.count(id)&&!initialize_selected_profile_v67(id,e))return false;
  // Character.InitAll3b35f0: actual virtual1c InitPost, then virtual58 InitFinal.
  return record->initialize(e)&&record->initialize_final_v70(e);
 }
 bool initialize_player_save(std::uintptr_t id,std::string& e){
  auto record=actor(id);if(!record){e="Required source-spawned Character before InitializePlayerSavegame";return false;}
  return record->initialize_player_save(e);
 }
 bool initialize_selected_profile_v67(std::uintptr_t id,std::string& e){
  auto world=world_.lock();auto app=application_.lock();auto record=actor(id);
  auto pm=app?app->source_player_manager_v59():nullptr;dh2::player::PlayerInfoFieldsV1* local{};
  if(!world||!app||!pm||!cache_v67_||!pm->get_local_player(0,false,local,e)||!local||local->character660!=id||!record||!record->save||!record->load||!selected_||local->save_slot664!=selected_->metadata.slot){if(e.empty())e="Required SAME source PM660/selected664/Save C1 before selected profile";return false;}
  if(profiles_v67_.count(id)){e="Canonical selected profile creation cannot replay its retained prefix";return false;}
  const auto settings=app->source_settings4c_v67();if(!settings){e="Required SAME App4c CurrentDifficulty source owner";return false;}
  RendererProfileBootstrapInputsV59 input;input.selected=selected_;input.source_cells=pm;
  input.player_info_slot664=&local->save_slot664;input.cache_owner=cache_v67_;input.read_cache=cache_v67_->files().read;
  input.actual_levels=world->tables.levels();input.reads.tables=world;input.reads.characters=world->tables.characters();input.reads.skills=world->skills_owner->borrow();
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record;
  input.set_slot=[weak](std::int32_t slot,auto& error){auto r=weak.lock();if(!r||!r->save||!r->save_fields||*r->save_fields->save_slot14e8()!=reinterpret_cast<std::uintptr_t>(r->save.get())){error="Required SAME Character14e8 for source SetSlot";return false;}r->save->set_slot(slot);error.clear();return true;};
  input.reads.store_current_difficulty=[settings](std::int32_t value,auto& error){settings->source_set_current_difficulty_v67(value);error.clear();return true;};
  input.reads.character_skill_list=[weak](std::uintptr_t identity,const std::vector<std::int32_t>*& rows,auto& error){auto r=weak.lock();if(!r||!r->actor||identity!=r->actor->object->identity||!r->services.skills){error="Required SAME source Character.GetSkillsList tables";return false;}auto index=r->properties->resolved[28];const auto& lists=r->services.skills.lists();if(index<0||std::size_t(index)>=lists.size())index=3;if(index<0||std::size_t(index)>=lists.size()){error="Actual GetSkillsList fallback3 unavailable";return false;}rows=&lists[std::size_t(index)];error.clear();return true;};
  const auto online=app->get_online_loading_v55();input.remaining={online,[online](const auto& q,auto& out,auto& error){if(q.operation==dh2::data::PlayerSaveLoadOpV1::online&&online){out.flag=online->byte5()!=0;error.clear();return true;}error="Required positive canonical SaveLoad network/volatile operation "+std::to_string(unsigned(q.operation));return false;}};
  auto& retained=profiles_v67_[id];return profile_before_gear(id,std::move(input),retained,e);
 }
 bool finish_selected_profile_v67(std::uintptr_t id,std::string& e){
  auto app=application_.lock();auto record=actor(id);const auto found=profiles_v67_.find(id);
  if(!app||!record||!record->equipment||!record->equipment->ready()||found==profiles_v67_.end()||!found->second){e="Required SAME completed Gear/selected profile before full campaign writer";return false;}
  struct Tables {dh2::data::LootTablesV2::Borrow loot;dh2::data::ItemPowerTablesV5::Borrow powers;};
  auto tables=std::make_shared<Tables>();dh2::data::ItemTextServicesV5 text;dh2::data::LootRandom8V2* random{};
  if(!record->equipment->loot_sources_v8(tables->loot,tables->powers,text,random,e)||!tables->powers||random!=record->services.random){if(e.empty())e="Required SAME Gear power names/Application Random for source campaign writer";return false;}
  const auto settings=app->source_settings4c_v67();if(!settings){e="Required SAME App4c CurrentDifficulty for PDFL writer";return false;}
  dh2::character::CharacterMenuCampaignSaveServicesV50 services;services.tables=tables;services.characters=record->design.characters();services.skills=record->services.skills;services.power_names=&tables->powers.names();
  services.current_difficulty=[settings](auto& value,auto& error){value=settings->current_difficulty_v67();error.clear();return true;};
  std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> weak=record;
  services.actor=[weak](auto& out,auto& error){auto r=weak.lock();if(!r||!r->equipment||!r->equipment->ready()||r->equipment->inventory()!=r->inventory37c||r->equipment->properties()!=r->properties){error="Required fresh SAME canonical Gear/Character source save sections";return false;}out.receiver=r;out.properties=r->equipment->property_view();out.inventory=r->inventory37c;error.clear();return true;};
  const auto online=app->get_online_loading_v55();services.remaining={online,[online](const auto& q,auto& out,auto& error){if(q.operation==dh2::data::PlayerSaveWriteOpV1::online&&online){out.flag=online->byte5()!=0;error.clear();return true;}error="Required positive canonical SG_Save network/checkpoint operation "+std::to_string(unsigned(q.operation));return false;}};
  return found->second->finish(*record->equipment,std::move(services),e);
 }
 // Configure V59 from actual PM664/SetSlot/Application files/jobs services.
 // This is callable only after actual initialize_save produced source14e8.
 bool profile_before_gear(std::uintptr_t id,RendererProfileBootstrapInputsV59 input,
  std::shared_ptr<RendererProfileBootstrapV59>& out,std::string& e){
  auto record=actor(id);
  if(!record||!record->save||!record->load||!record->save_fields||!record->actor||
     !world_.lock()||input.selected!=selected_||!input.source_cells){e="Required SAME source Character Save/PM cell lifetime before campaign profile";return false;}
  struct CellPins {std::shared_ptr<void> player_info,character_save;};
  auto cells=std::make_shared<CellPins>();cells->player_info=std::move(input.source_cells);cells->character_save=record->save_fields;
  input.save=record->save;input.load=record->load;input.character=id;
  input.defer_mask2_to_initpost_v62=true; // Actual generic InitPost delivers mask2 later.
  input.source_cells=cells;input.save14e8=record->save_fields->save_slot14e8();
  auto application=application_.lock();
  if(!application||!bind_profile_file_inputs_v61(application,input,e))return false;
  if(!RendererProfileBootstrapV59::create(std::move(input),out,e))return false;
  record->profile_bootstrap=out->source();return true;
 }
 bool move_c1_inventory_to_gear(std::uintptr_t id,
  dh2::player::PlayerEquipmentRenderInputsV1& input,std::string& e){
  auto record=actor(id);if(!record){e="Required same source Character inventory37c before Gear";return false;}
  if(!record->profile_bootstrap){e="Required selected-profile Save bootstrap before Gear transfer";return false;}
  if(!record->profile_bootstrap->configure_equipment(input,e))return false;
  return record->transfer_inventory(input,e);
 }
 // Actual SG_Load4 point after common InitVisual: construction inputs were
 // already configured/moved above. This performs restore, not InitialGrant.
 bool prepare_equipment_at_load4(std::uintptr_t id,
  dh2::player::PlayerEquipmentRenderOwnerV1& same,std::string& e){
  auto record=actor(id);
  if(!record||!record->profile_bootstrap||!record->inventory_transferred||record->prepared_equipment_v60){
   e="Required once-only moved Character inventory before SG_Load4 equipment preparation";return false;
  }
  record->prepared_equipment_v60=&same; // retain failed attempted receiver too.
  if(!same.prepare_restore_v60(e))return false;
  if(same.inventory()!=record->inventory37c||same.properties()!=record->properties){
   e="SG_Load4 prepared a different Character inventory/property owner";return false;
  }
  return true;
 }
 // Called ONLY at original _InitEquipment3b395c after positive PM lookup.
 bool finish_equipment_at_init_equipment(std::uintptr_t id,
  dh2::player::PlayerEquipmentRenderOwnerV1& same,std::string& e){
  auto record=actor(id);
  if(!record||record->prepared_equipment_v60!=&same||same.inventory()!=record->inventory37c||
     same.properties()!=record->properties){
   e="Required SAME SG_Load4 Gear receiver at source _InitEquipment";return false;
  }
  return same.finish_initial_grants_v60(e);
 }
 // Call only after original body/PF release and ObjectManager/World removal.
 // Canonical manager owns the same receiver; do not erase while discoverable.
 bool removed_after_unpublication(std::uintptr_t id,std::string& e){
  return characters_&&characters_->erase_after_unpublication(id,e);
 }
};
// Root seam:
// - after start_source_campaign_v55, borrow_source_campaign_candidate_v55 and
//   connect this retained bridge before the first loading tick (phase<7).
// - source PM AddCharacter.spawn gets this SAME manager/factory; initialize_save
//   routes here, then source slot664/SetSlot and V59 profile_before_gear.
// - Generic InitPost V61 owns both source branches; helpers remain required.
// - transfer the embedded inventory, initialize SAME Gear with actual visual,
//   finish V59, then SkillV6 adopts its same Save/SaveLoad. Do not allocate a
//   second player metadata facet/VM/target/FSM or development descriptor.


