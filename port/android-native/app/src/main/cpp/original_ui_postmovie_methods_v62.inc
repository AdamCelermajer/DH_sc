bool OriginalUiSession::select_process_hud_v132(const ui::SourceMenuVariantV132& variant,std::string& e){
 auto& self=*impl_;
 if(self.movie||self.source_movies_v132||!variant.uri[0]||!variant.uri[3]||!variant.hud_digest){e="Source HUD selection requires fresh actual primary3";return false;}
 self.process_variant_v132=variant;self.source_movies_v132=true;e.clear();return true;
}
bool OriginalUiSession::source_load_hud_primary_v63(const std::string& directory,std::string& e){
 auto& self=*impl_;
 if(directory.empty()||directory.front()!='/'||!self.manager||self.movie||self.menu_camera_v93){e="LoadMenu3 requires private directory and retired actual HUD primary/camera slots";return false;}
 self.directory=directory;self.selected=false;self.live_player=false;
 return self.load(e,true);
}
bool OriginalUiSession::source_scan_loaded_hud_v63(std::uintptr_t id,std::string& e){
 if(!impl_->movie||id!=reinterpret_cast<std::uintptr_t>(impl_->movie.get())){e="Flash.ScanForAnims requires SAME new HUD3 primary";return false;}
 return flash_manager_->scan_hud(e);
}
bool OriginalUiSession::source_action_icon_cell_v62(const std::int32_t*& out,std::shared_ptr<void>& lease,std::string& e){
 out=impl_->source_action_icon108_v62.get();lease=impl_->source_action_icon108_v62;e.clear();return true;
}
bool OriginalUiSession::bind_hud_world_v62(std::shared_ptr<void> transport,HudWorldOperationV62 world,
 const ui::HudControlsServicesV62& controls,std::string& e,std::shared_ptr<void> actual_world){
 if(!transport||!world){e="Required retained actual InfoHUD/controls transport";return false;}
 auto& self=*impl_;
 if(self.source_hud_transport_v62&&
    (self.source_hud_transport_v62.get()!=transport.get()||self.source_hud_transport_v62.owner_before(transport)||transport.owner_before(self.source_hud_transport_v62)||
     self.source_hud_graph_v62||(!self.source_campaign_detached_v104&&
      (self.attack_controls_v46.held9||self.joystick.active||!self.hud_pointers.empty()||!self.world_pointers.empty())))){
  e="HUD transport adoption requires SAME owner and no active callbacks/input";return false;
 }
 self.source_hud_transport_v62=std::move(transport);self.source_hud_world_v62=std::move(world);self.source_controls_v62=controls;
 self.source_campaign_world_v104=std::move(actual_world);
 self.source_campaign_detached_v104=false;
 //SAME held/joystick fields are borrowed during update. Preserve original
 //transport contexts; the synchronous forwarding record supplies cache cells.
 e.clear();return true;
}
bool OriginalUiSession::require_unbound_campaign_aliases_v115(std::string& e)const{
 const auto& self=*impl_;
 if(self.source_campaign_world_v104.lock()||self.source_script_world_v104.lock()||self.script_ui_world_v97||
    self.source_hud_graph_v62||self.source_hud_error_v62||self.live_player||self.gameplay_activated){
  e="Startup native detach found a positive actual HUD/Script/Character campaign alias";return false;
 }
 e.clear();return true;
}
bool OriginalUiSession::can_retire_hud_campaign_v104(const std::shared_ptr<void>& world,
 const std::shared_ptr<void>& transport,std::string& e)const{
 const auto& self=*impl_;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!world||!transport||!same(self.source_hud_transport_v62,transport)||self.source_hud_graph_v62||self.source_hud_error_v62){
  e="HUD campaign detach requires SAME process transport and quiescent callbacks";return false;
 }
 const auto bound=self.source_campaign_world_v104.lock();const auto script=self.source_script_world_v104.lock();
 if((bound&&!same(bound,world))||(script&&!same(script,world))){e="HUD/script callbacks belong to another actual World";return false;}
 e.clear();return true;
}
bool OriginalUiSession::retire_hud_campaign_v104(const std::shared_ptr<void>& world,
 const std::shared_ptr<void>& transport,std::string& e){
 if(!can_retire_hud_campaign_v104(world,transport,e))return false;
 auto& self=*impl_;
 //Clear only native old-world aliases. Real HUD/control cache cells, held
 //fields, movie/camera, Flash ownership and message queues follow the source
 //Unload/D0 receipts already reached by the caller, never this adapter.
 self.source_hud_world_v62={};self.source_controls_v62={};self.source_campaign_world_v104.reset();
 self.script_ui_world_v97.reset();self.source_script_world_v104.reset();
 self.live_player=false;self.gameplay_activated=false;
 self.source_campaign_detached_v104=true;
 e.clear();return true;
}
bool OriginalUiSession::info_hud_update_v62(std::string& e){return impl_->source_info_run_v62(ui::HudManagerEntry::update,e);}
bool OriginalUiSession::bind_level_ui_release_v107(loader::LevelDestroyServicesV1& out,std::string& e){
 if(out.flush_message_queue||out.info_hud||out.info_hud_flush||out.hud_controls||out.hud_controls_flush){e="Level UI release slots already own a provider";return false;}
 const std::weak_ptr<Impl> weak=impl_;
 out.flush_message_queue=[weak](loader::LevelMessageQueueV1 family,std::string& e){
  auto self=weak.lock();if(!self||self->source_hud_graph_v62){e="Message Flush requires the actual idle process UI owner";return false;}
  switch(family){
   case loader::LevelMessageQueueV1::status:return self->message_owner()->flush_source_context_v97(0,e);
   case loader::LevelMessageQueueV1::tutorial:self->auxiliary_messages_v97.flush_tutorial();break;
   case loader::LevelMessageQueueV1::dialog:self->dialog_owner_v97()->flush();break;
   case loader::LevelMessageQueueV1::online_status:self->auxiliary_messages_v97.flush_online();break;
   default:e="Unknown source message singleton";return false;
  }
  e.clear();return true;
 };
 out.info_hud=[weak](loader::LevelReleaseReceiverV1& out,std::string& e){
  auto self=weak.lock();if(!self){e="Actual InfoHUD process owner expired";return false;}
  out={self,reinterpret_cast<std::uintptr_t>(&self->source_info_hud_v62)};e.clear();return true;
 };
 out.info_hud_flush=[weak](const loader::LevelReleaseReceiverV1& captured,std::string& e){
  auto self=weak.lock();if(!self||captured.owner.get()!=self.get()||captured.owner.owner_before(self)||self.owner_before(captured.owner)||
   captured.identity!=reinterpret_cast<std::uintptr_t>(&self->source_info_hud_v62)){e="InfoHUD Flush addressed a different singleton";return false;}
  return self->source_info_run_v62(ui::HudManagerEntry::flush,e);
 };
 out.hud_controls=[weak](loader::LevelReleaseReceiverV1& out,std::string& e){
  auto self=weak.lock();if(!self){e="Required actual HUDControls process owner";return false;}
  if(!self->source_controls_constructed_v62){model_renderer::SourceCampaignCandidateBorrowV55 c;
   if(!model_renderer::borrow_source_campaign_candidate_runtime_v61(c,e)||!c.application||!self->construct_source_controls_v108(c.application->events14(),e))return false;
  } //Actual cold GetInstance executes the SAME C1, even before LoadMenu3.
  out={self,reinterpret_cast<std::uintptr_t>(&self->attack_controls_v46)};e.clear();return true;
 };
 out.hud_controls_flush=[weak](const loader::LevelReleaseReceiverV1& captured,std::string& e){
  auto self=weak.lock();if(!self||!self->source_controls_constructed_v62||captured.owner.get()!=self.get()||
   captured.owner.owner_before(self)||self.owner_before(captured.owner)||captured.identity!=reinterpret_cast<std::uintptr_t>(&self->attack_controls_v46)){
   e="HUDControls Flush addressed a different constructor receiver";return false;
  }
  //Original418240 is BXLR. It neither cancels input nor destroys caches.
  e.clear();return true;
 };
 e.clear();return true;
}
bool OriginalUiSession::hud_controls_native_event_v68(ui::SwfEvent48& event,std::string& e){
 auto& self=*impl_;if(!self.movie){e="Required actual HUD native event movie";return false;}
 struct Event {Impl& self;ui::SwfEvent48& event;static bool run(void* raw,ui::SwfAsGraph&,std::string& e){auto& q=*static_cast<Event*>(raw);return q.self.source_control_native_event_v68(q.event,e);}} call{self,event};
 return self.movie->menu_action_script(&call,Event::run,e);
}
bool OriginalUiSession::hud_controls_update_v62(std::string& e){
 auto& self=*impl_;auto services=self.source_controls_v62;
 if(!self.source_controls_constructed_v62){e="Required actual HUDControls C1 event registration";return false;}
 //Cache queries are captureless C ABI, so use a forwarding record rather than
 //changing contexts of Input/PM/Cmd providers belonging to the actual World.
 struct Forward{Impl& self;ui::HudAttackServicesV46 actual;};Forward f{self,services.prefix};
 auto& p=services.prefix;p.context=&f;
 p.input_blocked30=[](void* raw,auto& out,auto& e){auto& q=*static_cast<Forward*>(raw);return q.actual.input_blocked30&&q.actual.input_blocked30(q.actual.context,out,e);};
 p.store_controller_blocked=[](void* raw,auto value,auto& e){auto& q=*static_cast<Forward*>(raw);return q.actual.store_controller_blocked&&q.actual.store_controller_blocked(q.actual.context,value,e);};
 p.current_level=[](void* raw,auto& id,auto*& byte,auto& e){auto& q=*static_cast<Forward*>(raw);return q.actual.current_level&&q.actual.current_level(q.actual.context,id,byte,e);};
 p.cached_root658=[](void* raw,auto& out,auto& e){out=static_cast<Forward*>(raw)->self.source_controls_root658_v62;e.clear();return true;};
 p.cached_chars8=[](void* raw,auto& out,auto& e){out=static_cast<Forward*>(raw)->self.source_controls_cache8_v62;e.clear();return true;};
 p.init_cached_chars=[](void* raw,auto& e){return static_cast<Forward*>(raw)->self.source_controls_initialize_v63(e);};
 p.local_player=[](void* raw,auto index,auto flag,auto& out,auto& e){auto& q=*static_cast<Forward*>(raw);return q.actual.local_player&&q.actual.local_player(q.actual.context,index,flag,out,e);};
 p.use_ooi=[](void* raw,auto actor,auto target,auto& e){auto& q=*static_cast<Forward*>(raw);return q.actual.use_ooi&&q.actual.use_ooi(q.actual.context,actor,target,e);};
 p.attack=[](void* raw,auto actor,auto target,auto& e){auto& q=*static_cast<Forward*>(raw);return q.actual.attack&&q.actual.attack(q.actual.context,actor,target,e);};
 return ui::hud_controls_update_v62(self.attack_controls_v46,self.joystick,services,e);
}
bool OriginalUiSession::construct_hud_controls_v62(std::shared_ptr<events::EventManagerOwnerV12> actual_events,std::string& e){
 return impl_->construct_source_controls_v108(std::move(actual_events),e);
}
bool OriginalUiSession::loadmenu3_info_store_v62(std::uintptr_t expected,std::string& e){
 auto& self=*impl_;if(!self.movie||expected!=reinterpret_cast<std::uintptr_t>(self.movie.get())){e="LoadMenu3 InfoHUD store requires SAME newly loaded primary3";return false;}
 self.source_info_hud_v62.source_render_store_v62(expected); //source mutation before initialization failure
 self.source_hud_core_v62.reset();self.source_hud_base_core_v132.reset();self.source_hud_advance_v62=ui::HudAdvanceOwner{};
 return self.source_info_run_v62(ui::HudManagerEntry::initialize,e);
}
bool OriginalUiSession::loadmenu3_controls_store_v62(std::uintptr_t expected,std::string& e){
 auto& self=*impl_;if(!self.movie||expected!=reinterpret_cast<std::uintptr_t>(self.movie.get())){e="LoadMenu3 HUDControls store requires SAME newly loaded primary3";return false;}
 self.source_controls_root658_v62=expected;
 return self.source_controls_initialize_v63(e);
}
