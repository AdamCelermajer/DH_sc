//Included within source runtime model_renderer, after actual candidate borrower.
//No whole source-runtime overwrite: preserve current visitor/Stage9/Stage13 hunks.
struct SourceTerminalProviderV97 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::weak_ptr<dh2::world::CanonicalObjectManagerV1> objects;
 SourceTerminalNativeV97 native;
 bool current(std::shared_ptr<SourceWorldBorrowV61>& source,std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& actual,std::string& e)const{
  source=world.lock();actual=level.lock();auto manager=objects.lock();dh2::loader::CanonicalCurrentLevelBorrowV1 current;
  if(!source||!actual||!manager||!source->application||!source->player_manager||!source->canonical_world||source->canonical_world->manager_lease!=manager||source->canonical_world->manager_lease.owner_before(manager)||manager.owner_before(source->canonical_world->manager_lease)||!borrow_current_native_level_v27(current,e)||current.level()!=actual||current.level().owner_before(actual)||actual.owner_before(current.level())){if(e.empty())e="Required SAME terminal World/App/PM/manager/GS current Level";return false;}return true;
 }
 bool local(std::int32_t index,bool flag,dh2::loader::TerminalPlayerBorrowV97& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> actual;if(!current(source,actual,e))return false;
  dh2::player::PlayerInfoFieldsV1* info{};if(!source->player_manager->get_local_player(index,flag,info,e)||!info){if(e.empty())e="Original local PlayerInfo NULL dereference rejected";return false;}
  dh2::loader::TerminalPlayerBorrowV97 value;value.receiver=std::shared_ptr<void>(source->player_manager,info);value.identity=reinterpret_cast<std::uintptr_t>(info);value.character660=&info->character660;
  const std::weak_ptr<SourceWorldBorrowV61> weak=world;auto pin=value.receiver;
  value.ready545=[weak,pin,info](std::uint8_t& value,std::string& e){auto source=weak.lock();auto net=source&&source->player_manager?source->player_manager->network():nullptr;auto actual=net?net->borrow(*info):nullptr;
   if(!actual||actual->receiver!=info||!actual->loading_clear_produced_v70){e="Required SAME source-produced PlayerInfo545 loading member";return false;}value=actual->ready_to_roll545_v70;return true;};
  value.is_host=[method=native.is_host,pin,info](bool& value,std::string& e){if(!method){e="Required actual CNetPlayerInfo.IsHost/online session170";return false;}return method(*info,value,e);};
  out=std::move(value);return true;
 }
};
static bool bind_source_campaign_terminal_v97(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,dh2::loader::EarlyLoadingDebugV46 debug,
 const dh2::loader::LoadPrefixServicesV50& prefix,
 decltype(dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1>::get_menu) lookup,
 dh2::loader::SourceLoadingInputsV43& inputs,std::string& e){
 using namespace dh2;using namespace dh2::loader;
 if(inputs.external.stage_body[36]&&inputs.external.stage_body[37]){e.clear();return true;}
 if(!world||!level||!world->canonical_world||inputs.manager!=world->canonical_world->manager_lease){e="Required SAME terminal source owner/manager";return false;}
 auto provider=std::make_shared<SourceTerminalProviderV97>();provider->world=world;provider->level=level;provider->objects=inputs.manager;
 if(world->terminal_loading_v97)provider->native=*world->terminal_loading_v97;
 auto same=[&](const auto& other){return provider->native.owner&&other&&!provider->native.owner.owner_before(other)&&!other.owner_before(provider->native.owner);};
 if(same(world)||same(world->owner)||same(level)||same(inputs.manager)){e="Terminal Main behavior must retain independent source authority/weak World";return false;}
 if((provider->native.is_host||provider->native.local_hosting||provider->native.all_loading_done||provider->native.all_clients_ready||provider->native.network_init||provider->native.hosting_player)&&!provider->native.owner){e="Actual terminal online behavior requires independent native transport owner";return false;}
 TerminalLoadingServicesV97 services;services.owner=provider;
 services.current=[provider](std::uint32_t phase,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;std::shared_ptr<CanonicalLevelContextV1> level;return provider->current(world,level,e)&&level->constructor_fields_v3().field130==phase;};
 services.trace=[debug](std::string& e){return early_loading_trace_v46(debug,e);};
 services.online=[provider](bool& value,std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;std::shared_ptr<CanonicalLevelContextV1> level;if(!provider->current(world,level,e))return false;auto online=world->application->get_online_loading_v55();if(!online){e="Required actual terminal COnline";return false;}value=online->byte5()!=0;return true;};
 services.local_player=[provider](auto index,auto flag,auto& value,auto& e){return provider->local(index,flag,value,e);};
 services.local_hosting=provider->native.local_hosting;services.all_loading_done=provider->native.all_loading_done;services.all_clients_ready=provider->native.all_clients_ready;
 services.network_initialized1ac=[provider](auto& value,auto& e){auto objects=provider->objects.lock();if(!objects){e="Released SAME terminal ObjectManager";return false;}value=objects->source_network_initialized1ac_v88();return true;};
 services.network_init=[provider](auto& e){auto objects=provider->objects.lock();if(!objects||!provider->native.network_init){e="Required actual NetworkInitLevel340be0 behavior";return false;}return provider->native.network_init(*objects,e);};
 services.update_objects=[provider](float dt,auto& e){auto world=provider->world.lock();if(!world||!world->gameplay_services_v66||!world->gameplay_services_v66->update_objects){e="Required SAME original ObjectManager.Update body";return false;}return world->gameplay_services_v66->update_objects(dt,e);};
 services.try_quest_sync=[provider](const TerminalPlayerBorrowV97& info,auto& e){auto world=provider->world.lock();if(!world||!info.character660){e="Required actual host PlayerInfo660";return false;}if(!*info.character660)return true;
  SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world->owner,*info.character660,actual,e)||!actual.character||!actual.character->save_fields||!actual.character->save_fields->save_slot14e8())return false;
  const auto saved=*actual.character->save_fields->save_slot14e8();if(!saved)return true;
  if(!actual.character->save||saved!=reinterpret_cast<std::uintptr_t>(actual.character->save.get())||!actual.character->quest_sync_owner){e="Required SAME host Save14e8/quest sync owner";return false;}
  return actual.character->quest_sync_owner->try_sync(actual.character->services.quest_sync,e);
 };
 services.capture_menu=[provider](const char* name,auto& out,auto& e){auto world=provider->world.lock();if(!world){e="Released terminal menu campaign";return false;}return capture_native_process_menu_v104(world->application,name,out,e);};
 services.lookup_menu=std::move(lookup);services.debug_switch=prefix.debug_instance_get_switch;services.menu_debug_set_text=prefix.menu_debug_set_text;
 services.num_local_players=[provider](bool flag,auto& count,auto& e){auto world=provider->world.lock();auto manager=world&&world->player_manager?world->player_manager->manager():nullptr;if(!manager){e="Required actual terminal PlayerManager";return false;}return manager->num_local_players(flag,count,e);};
 services.unblock_controller=[provider](std::uintptr_t id,auto& e){auto world=provider->world.lock();if(!world){e="Released terminal Character campaign";return false;}SourceCampaignCharacterBorrowV62 actual;
  if(!borrow_source_campaign_character_v62(world->owner,id,actual,e)||!actual.character)return false;auto record=actual.character;
  const auto controller=record->controllable374_v70.controller378;if(!controller)return true;
  if(!record->player_controllers_v70){e="Required typed SAME positive controller378 owner";return false;}return record->player_controllers_v70->source_end_loading_unblock_v97(controller,e);
 };
 services.place_faery_followers_null=[provider](auto& e){std::shared_ptr<SourceWorldBorrowV61> world;std::shared_ptr<CanonicalLevelContextV1> level;if(!provider->current(world,level,e))return false;
  auto placement=provider->native.faery;
  //Never install a containing-World owner cycle through a native leaf bag.
  if(placement.owner&&!placement.owner.owner_before(world->owner)&&!world->owner.owner_before(placement.owner)){e="Faery resource leaves require independent native authority";return false;}
  placement.owner=world->files_owner;placement.objects=&world->canonical_world->manager;
  const auto weak=provider->world;
  if(!placement.char_type)placement.char_type=[weak](auto id,auto& value,auto& e){auto world=weak.lock();SourceCampaignCharacterBorrowV62 actual;if(!world||!borrow_source_campaign_character_v62(world->owner,id,actual,e)||!actual.character||!actual.character->properties||!actual.character->design.ai()){if(e.empty())e="Required SAME retained Character.GetCharAI";return false;}const auto* ai=data::ai_props(*actual.character->design.ai(),actual.character->properties->resolved[1]);if(!ai){e="Required actual resolved Character AI row";return false;}value=ai->type;return true;};
  if(!placement.default_target_v97)placement.default_target_v97=[provider](auto actor,auto& selected,auto& e){std::shared_ptr<SourceWorldBorrowV61> world;std::shared_ptr<CanonicalLevelContextV1> level;if(!provider->current(world,level,e))return false;
   auto online=world->application->get_online_loading_v55();if(!online){e="Required actual default faery target Online";return false;}
   player::PlayerInfoFieldsV1* target{};std::shared_ptr<void> pin;
   if(!online->byte5()){if(!world->player_manager->get_local_player(0,true,target,e)||!target)return false;selected=target->character660;return true;}
   if(!provider->native.hosting_player||!provider->native.hosting_player(target,pin,e)||!target||!pin){if(e.empty())e="Required actual online GetHostingPlayer36e09c receiver";return false;}
   selected=target->character660;bool hosting{};if(!provider->native.local_hosting||!provider->native.local_hosting(hosting,e))return false;
   if(!hosting){SourceCampaignCharacterBorrowV62 follower;if(!borrow_source_campaign_character_v62(world->owner,actor,follower,e)||!follower.character||!follower.character->actor)return false;
    dh2::world::GameObjectInitializationFieldsV62 fields;if(!follower.character->actor->inherited_initialization_fields_v62(follower.character,fields,e))return false;
    auto* elapsed=follower.character->actor->source_network114_v70();auto* owner=fields.integer(0x110);if(!elapsed||!owner){e="Required SAME online follower114/110";return false;}*elapsed=0;*owner=target->internal670;
   }return true;
  };
  //Common original count/GetPlayer/local getters borrow SAME PM, not a roster copy.
  if(!placement.source_player_count6c4)placement.source_player_count6c4=[weak](auto& count,auto& e){auto world=weak.lock();auto at=world&&world->player_manager?world->player_manager->count_field():nullptr;if(!at){e="Required actual PM count6c4";return false;}count=*at;return true;};
  if(!placement.player)placement.player=[weak](auto index,bool flag,auto& id,auto& e){auto world=weak.lock();auto manager=world&&world->player_manager?world->player_manager->manager():nullptr;player::PlayerInfoFieldsV1* info{};if(!manager||!manager->get_player(index,flag,info,e))return false;id=info?info->character660:0;return true;};
  if(!placement.local_player)placement.local_player=[weak](auto index,bool flag,auto& id,auto& e){auto world=weak.lock();player::PlayerInfoFieldsV1* info{};if(!world||!world->player_manager||!world->player_manager->get_local_player(index,flag,info,e))return false;id=info?info->character660:0;return true;};
  return dh2::world::level_place_faery_followers_v8(0,placement,e);
 };
 return TerminalLoadingV97::bind(std::move(services),inputs,e);
}
