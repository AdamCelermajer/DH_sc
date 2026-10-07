// Include in actual canonical Character transport domain, after V60/V62.
// PM is App-owned and available BEFORE player admission/source Stage6.
class RendererPlayerManageCharactersV70 final {
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> app_;
 std::weak_ptr<WorldScriptContext> world_;
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> selected_;
 std::map<dh2::player::PlayerInfoFieldsV1*,std::shared_ptr<dh2::player::PlayerInfoProfileOwnerV70>> profiles_;
 std::unique_ptr<dh2::player::PlayerManageCharactersOwnerV70> owner_;
public:
 bool initialize(const model_renderer::SourceCampaignCandidateBorrowV55& actual,std::string& e){
  if(owner_||!actual.application||!actual.actual_world||!actual.selected_profile){e="Required once-only canonical PM management binding";return false;}
  auto pm=actual.application->source_player_manager_v59();auto world=std::static_pointer_cast<WorldScriptContext>(actual.actual_world);
  if(!pm||!pm->manager()||!pm->network()||!world||!world->tables.characters()){e="Required SAME preplayer App PM and CharacterTable";return false;}
  app_=actual.application;world_=world;selected_=actual.selected_profile;
  dh2::player::PlayerManageServicesV70 s;
  // Weak captures avoid App->PM-provider->App/World cycles. The original PM
  // and source record have longer lifetime than this App-owned delivery.
  s.provider=std::make_shared<std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>>(actual.application);
  const auto online=actual.application->get_online_loading_v55();
  s.online=[online](bool& value,std::string& error){if(!online){error="Required original GetOnline receiver";return false;}value=online->byte5()!=0;return true;};
  const auto selectors=dh2::player::process_player_manager_offline_selectors_v70();
  s.game_center_enabled11=[selectors](bool& value,std::string&){value=selectors->game_center_enabled11()!=0;return true;};
  s.network_alias_enabled28=[selectors](bool& value,std::string&){value=selectors->online_alias_enabled28()!=0;return true;};
  s.current_level=[](auto& lease,const std::uint32_t*& phase,const std::uint8_t*& active,std::string& error){
   dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!model_renderer::borrow_current_native_level_v27(current,error))return false;
   lease.reset();phase=nullptr;active=nullptr;if(current){lease=current.level();const auto& fields=current.level()->constructor_fields_v3();phase=&fields.field130;active=&fields.byte144;}return true;
  };
  std::weak_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> weak_pm=pm;
  s.selected5c=[weak_pm,online](auto& info,bool& value,std::string& error){auto same=weak_pm.lock();if(!same||!online){error="Released SAME PM selected5c receiver";return false;}return same->network()->is_active_selected_v67(info,online->byte5(),value,error);};
  s.fields=[weak_pm](auto& info,auto& out,std::string& error){auto same=weak_pm.lock();auto fields=same?same->network()->borrow(info):nullptr;
   if(!fields||!fields->managed_fields_produced_v70){error="Required actual derived PlayerInfo Reset managed field producer";return false;}
   out.receiver=fields;out.name2d0=&fields->character_name2d0_v70;out.level330=&fields->character_level330_v70;out.class380=&fields->character_class380_v68;
   out.visible4e5=&fields->character_visible4e5_v70;out.loading525=&fields->character_loading525_v70;out.profile680=&fields->profile680_v70;return true;
  };
  s.assign=[weak_pm](auto& info,auto kind,std::int32_t value,const std::string* name,std::string& error){auto same=weak_pm.lock();auto fields=same?same->network()->borrow(info):nullptr;
   if(!fields||!fields->managed_fields_produced_v70){error="Required SAME PlayerInfo assignment backing";return false;}
   switch(kind){
    case dh2::player::PlayerManageAssignmentV70::name:if(!name){error="Required actual CString for CharacterName assignment";return false;}fields->character_name2d0_v70=*name;break;
    case dh2::player::PlayerManageAssignmentV70::character_class:fields->character_class380_v68=value;break;
    case dh2::player::PlayerManageAssignmentV70::level:fields->character_level330_v70=value;break;
    case dh2::player::PlayerManageAssignmentV70::visible:fields->character_visible4e5_v70=std::uint8_t(value);break;
    case dh2::player::PlayerManageAssignmentV70::loading:fields->character_loading525_v70=std::uint8_t(value);break;
   }
   // These exact scalar/string assignments are the offline projection. Real
   // online NetStruct packet change/serialization has a separate required body.
   return true;
  };
  s.construct_profile680=[this](auto& info,std::int32_t slot,auto& out,std::string& error){auto app=app_.lock();auto world=world_.lock();
   if(!app||!world||!selected_||slot!=selected_->metadata.slot||info.save_slot664!=slot||!app->source_save_files_v61()||!app->source_settings4c_v67()){
    error="Required SAME selected664/App file/jobs/settings before profile680 C1";return false;}
   auto& retained=profiles_[&info];if(retained){error="PlayerInfo profile680 failed prefix cannot be retried";return false;}retained=std::make_shared<dh2::player::PlayerInfoProfileOwnerV70>();
   const auto& bytes=selected_->file.bytes;
   if(!retained->construct(slot,app->source_save_files_v61(),app->source_settings4c_v67(),world,world->tables.characters(),selected_,{bytes.data(),bytes.size()},error))return false;
   out=retained->save();return true;
  };
  s.clear_loading_info=[weak_pm,query=s.online](std::string& error){auto same=weak_pm.lock();
   if(!same||!same->manager()||!same->network()){error="Released SAME PM ClearLoadingInfo receiver";return false;}
   return dh2::player::player_manager_clear_loading_v70(*same->manager(),*same->network(),query,error);
  };
  s.remove_all_characters=[weak_pm](std::string& error){auto same=weak_pm.lock();
   if(!same||!same->manager()){error="Released same process PM RemoveAllCharacters";return false;}
   return same->manager()->remove_all_characters_v114(error);
  };
  owner_=std::make_unique<dh2::player::PlayerManageCharactersOwnerV70>(*pm->manager(),std::move(s));return true;
 }
 bool update(std::string& e){auto app=app_.lock();if(!app||!app->source_player_manager_v59()||!owner_){e="Required canonical preplayer PM management receiver";return false;}return owner_->update(e);}
};

