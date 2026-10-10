// Include after the V60 transport, V70 Spawn and V70 AddCharacter fragments.
// This receiver is retained by SAME App PM; all World/App callbacks are weak.
class RendererPlayerManagerStartupV70 final:public std::enable_shared_from_this<RendererPlayerManagerStartupV70> {
 std::weak_ptr<RendererCharacterCandidateV60> transport_;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
 std::weak_ptr<void> world_;
 std::unique_ptr<RendererPlayerAddCharacterV70> add_;
 std::map<dh2::player::PlayerInfoFieldsV1*,std::unique_ptr<model_renderer::SourcePlayerCharacterRemovalV114>> removals_v114_;
 std::unique_ptr<dh2::player::PlayerLocalControllersOwnerV70> local_;
 std::unique_ptr<dh2::player::PlayerManagerUpdateOwnerV70> update_;
 dh2::player::PlayerOnlineQueryV70 online_;
 bool stage(dh2::player::PlayerManagerUpdateStageV70 stage,dh2::player::PlayerManagerOwnerV1& manager,std::string& e){
  auto app=application_.lock();auto pm=app?app->source_player_manager_v59():nullptr;auto transport=transport_.lock();
  if(!pm||pm->manager()!=&manager||!transport){e="Released SAME PlayerManager.Update stage receiver";return false;}
  using S=dh2::player::PlayerManagerUpdateStageV70;
  switch(stage){
   case S::check_online_transition:return dh2::player::player_manager_offline_transition_v70(manager,*pm->network(),online_,e);
   case S::check_local_controllers:return local_->update(e);
   case S::manage_characters:return transport->manage_characters_v70(e);
   case S::check_remote_controllers:{bool online{};if(!online_(online,e))return false;if(online){e="Required online _CheckRemoteControllers378b94";return false;}return true;} //378bb0 literal offline return.
   case S::check_local_deaths:
   case S::check_global_deaths:{
    dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!model_renderer::borrow_current_native_level_v27(level,e))return false;
    if(!level||level.level()->constructor_fields_v3().field130!=38)return true;
    if(stage==S::check_global_deaths){bool online{};if(!online_(online,e))return false;if(online){e="Required online _CheckGlobalDeaths375eb4";return false;}return true;}
    dh2::player::PlayerInfoFieldsV1* info{};if(!manager.get_local_player(0,false,info,e)||!info)return false;
    if(!info->character660)return true;
    auto record=transport->actor(info->character660);if(!record||!record->life){e="Required SAME Character.IsDead1449 in _CheckLocalDeaths";return false;}
    if(!record->life->dead||*manager.character_count_field()<=0)return true;
    e="Required source _HandleLocalDeaths375fe8 on actual dead local Character";return false;
   }
   case S::statistics_update:return true; // Whole PlayerStatManager.Update3790d8 is bx lr.
  }
  e="Unknown source PlayerManager.Update stage";return false;
 }
 static bool remaining(void* raw,const dh2::player::PlayerManagerRequestV1& q,dh2::player::PlayerManagerResponseV1& out,std::string& e){
  auto& self=*static_cast<RendererPlayerManagerStartupV70*>(raw);
  if(q.operation==dh2::player::PlayerManagerOperationV1::character_initialization)return self.add_->deliver(q,out,e);
  if(q.operation==dh2::player::PlayerManagerOperationV1::remove_character){
   auto app=self.application_.lock();auto world=self.world_.lock();if(!app||!world||!q.player){e="Required same process local removal scope";return false;}
   auto& body=self.removals_v114_[q.player];if(!body)body=std::make_unique<model_renderer::SourcePlayerCharacterRemovalV114>();
   if(!body->execute(world,app,q,e))return false;self.removals_v114_.erase(q.player);return true;
  }
  e="Required PlayerManager source continuation "+std::to_string(std::uint32_t(q.operation));return false;
 }
public:
 static bool bind(const std::shared_ptr<RendererCharacterCandidateV60>& transport,const model_renderer::SourceCampaignCandidateBorrowV55& candidate,std::string& e){
  auto app=candidate.application;auto pm=app?app->source_player_manager_v59():nullptr;auto input=app?app->source_input_manager_v60():nullptr;
  if(!transport||!transport->belongs_to_v62(candidate.actual_world)||!pm||!pm->manager()||!pm->network()||!input){e="Required actual preplayer App PM/Input/source World";return false;}
  auto self=std::make_shared<RendererPlayerManagerStartupV70>();self->transport_=transport;self->application_=app;self->world_=candidate.actual_world;
  auto online=app->get_online_loading_v55();self->online_=[online](bool& value,std::string& error){if(!online){error="Required actual PM GetOnline receiver";return false;}value=online->byte5()!=0;return true;};
  self->add_=std::make_unique<RendererPlayerAddCharacterV70>();if(!self->add_->initialize(transport,candidate,{},e))return false;
  dh2::player::PlayerLocalControllersServicesV70 controls;controls.provider=input;
  controls.gamepad_count=[input](std::int32_t& value,std::string&){value=input->gamepad_count();return true;};
  controls.gamepad=[input](std::int32_t index,auto& out,std::string& error){std::shared_ptr<const dh2::input::GamepadC1V60> pad;
   if(!input->get_gamepad(index,pad,error)||!pad)return false;out.identity=reinterpret_cast<std::uintptr_t>(pad.get());out.connected758=&pad->connected758;
   out.receiver=std::const_pointer_cast<dh2::input::GamepadC1V60>(pad);return true;};
  controls.online=self->online_;self->local_=std::make_unique<dh2::player::PlayerLocalControllersOwnerV70>(*pm->manager(),std::move(controls));
  dh2::player::PlayerManagerUpdateServicesV70 updates;
  updates.provider=std::make_shared<std::weak_ptr<RendererPlayerManagerStartupV70>>(self);
  const auto weak_transport=self->transport_;std::weak_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> weak_pm=pm;
  updates.disabled81=[weak_transport](std::uintptr_t id,const std::uint8_t*& value,std::shared_ptr<void>& pin,std::string& error){auto t=weak_transport.lock();auto r=t?t->actor(id):nullptr;
   if(!r||!r->actor||(value=r->actor->source_bool_field(0x81))==nullptr){error="Required actual Character.disabled81 PM update borrow";return false;}pin=r;return true;};
  updates.visible4e5=[weak_pm](auto& info,const std::uint8_t*& value,std::shared_ptr<void>& pin,std::string& error){auto p=weak_pm.lock();auto fields=p?p->network()->borrow(info):nullptr;
   if(!fields||!fields->managed_fields_produced_v70){error="Required SAME PlayerInfo4e5 PM update borrow";return false;}value=&fields->character_visible4e5_v70;pin=fields;return true;};
  std::weak_ptr<RendererPlayerManagerStartupV70> weak_self=self;
  updates.stage=[weak_self](auto stage,auto& manager,std::string& error){auto actual=weak_self.lock();if(!actual){error="Released SAME PM stage callback";return false;}return actual->stage(stage,manager,error);};
  self->update_=std::make_unique<dh2::player::PlayerManagerUpdateOwnerV70>(*pm->manager(),std::move(updates));
  dh2::player::PlayerManagerServicesV1 remaining_services{self.get(),remaining};
  // Completed native retirement clears these SAME World-owned wrappers only
  // after genuine GS/Level release and native unpublication. PM keeps its real
  // manager/PlayerInfo owners across Worlds; replace only the old callback bag.
  std::weak_ptr<WorldScriptContext> weak_world=std::static_pointer_cast<WorldScriptContext>(candidate.actual_world);
  auto retired=[weak_world](std::string& error){auto prior=weak_world.lock();
   if(prior&&(prior->native_loading_v50||prior->native_gslevel_v27||prior->native_level_c1_v25)){
    error="Prior source PlayerManager campaign retains actual GS/loading/Level wrappers";return false;
   }
   error.clear();return true;
  };
  if(!pm->bind_campaign_remaining(candidate.actual_world,self,remaining_services,std::move(retired),e))return false;
  return model_renderer::bind_source_player_manager_update_v70(candidate,pm,self,[weak_self](std::string& error){auto actual=weak_self.lock();if(!actual){error="Released SAME source PM Update delivery";return false;}return actual->update_->update(error);},e);
 }
};
