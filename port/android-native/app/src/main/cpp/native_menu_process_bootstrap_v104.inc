// Included inside the existing NativeCharacterMenuV4. This creates only its
// process directory; native movie loads and GSInit advancement stay with callers.
bool prepare_process_directory_v104(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::function<bool(const char*,std::int32_t&,std::string&)> actual_debug,std::string& e){
 if(process_directory_ready_v104){
  if(process_application_v104.lock()!=app){e="Process MenuManager addressed another Application";return false;}
  e.clear();return true;
 }
 if(!app||!actual_debug||process_directory_attempted_v104||leases->world||shared_stack_v27||platform){
  e="Required fresh actual process MenuManager directory and Debug provider";return false;
 }
 process_directory_attempted_v104=true;process_application_v104=app;
 process_debug_v115=actual_debug;bootstrap_debug_v27=std::move(actual_debug);
 if(!prepare_character_application_v109(e))return false;
 dh2::android_ui::OriginalUiGameplaySettingsActionsV1 settings_actions;
 settings_actions.load=[](std::string& error){return front_ui.load_process_settings_v105(false,error);};
 settings_actions.save=[](std::string& error){return front_ui.save_process_settings_v119(error);};
 settings_actions.enter=[](std::string&){front_ui.resume_process_music_v119();return true;};
 settings_actions.refresh_hud=[](std::string& error){return original_ui.refresh_settings_hud_manager_v1(error);};
 if(!original_ui.bind_process_application_v109(app,application,std::move(settings_actions),e))return false;
 shared_stack_v27=std::make_shared<dh2::ui::MenuStackOwnerV1>(256);
 dh2::ui::AuthoredCharacterPanelPlatformServicesV4 backend;
 backend.owner=leases;backend.fields=&fields;backend.touch=&touch;
 backend.native_drm=[this](bool& out,auto&){out=drm.source_global();return true;};
 backend.debug=[this](const char* key,auto& out,auto& e){return debug(key,out,e);};
 backend.scoped_graph=[](void* raw,auto fn,auto& e){
  if(!character_panel){e="Required actual character movie scope";return false;}
  return character_panel->authored_graph_scope_v4(raw,fn,e);
 };
 backend.deadzone_root=[this](auto& graph,const auto& name,auto*& out,auto& e){return projection.root(graph,name,out,e);};
 backend.absolute_bounds=[this](auto&,auto& node,auto& out,auto& e){return projection.absolute_bounds(node,out,e);};
 backend.required_lifecycle={leases,[](auto&,const auto& request,auto&,auto& e){
  e="Required actual native MenuBase lifecycle "+std::to_string(unsigned(request.operation));return false;
 }};
 backend.required_stack=[this](auto& stack,auto& request,auto& e){
 bool handled{};if(shared_roster_v27&&!shared_roster_v27->route(stack,request,handled,e))return false;
 if(handled)return true;
 // MenuBase::IsValidMenu is shared by the registered process singletons too.
 // Return the same source-backed valid7c field used by the shared roster.
 if(request.operation==dh2::ui::MenuStackOperationV1::menu_valid&&request.menu&&request.render){
  auto* fields=model_renderer::native_derived_menu_fields_v67(request.menu->identity);
  auto* registered=shared_stack_v27?shared_stack_v27->registered_menu_v27(request.menu->identity):nullptr;
  if(fields&&registered==request.menu&&request.menu->render==request.render&&
     fields->render==request.render->identity){request.result=fields->valid7c;return true;}
 }
 // MenuMainMenu::LostFocus (42bae8) is a literal bx lr.
  if(request.operation==dh2::ui::MenuStackOperationV1::menu_blur&&request.menu&&request.render){
   auto* fields=model_renderer::native_derived_menu_fields_v67(request.menu->identity);
   if(fields&&fields->render==request.render->identity&&fields->name=="menu_MainMenu")return true;
   if(fields&&fields->render==request.render->identity&&
      (fields->name=="menu_EnterName"||fields->name=="menu_SelectClass"))
    return model_renderer::native_process_derived_menu_blur_v119(request.menu->identity,request.render->identity,e);
  }
  if(request.operation==dh2::ui::MenuStackOperationV1::menu_show&&request.menu&&request.render&&
     model_renderer::native_derived_menu_fields_v67(request.menu->identity))
   return model_renderer::native_process_derived_menu_show_v119(request.menu->identity,request.render->identity,e);
  if(request.operation==dh2::ui::MenuStackOperationV1::menu_focus&&request.menu&&request.render&&
     model_renderer::native_derived_menu_fields_v67(request.menu->identity))
   return model_renderer::native_process_derived_menu_focus_v119(request.menu->identity,request.render->identity,e);
  if(request.operation==dh2::ui::MenuStackOperationV1::menu_hide&&request.menu&&request.render){
   auto* fields=model_renderer::native_derived_menu_fields_v67(request.menu->identity);
   auto* registered=shared_stack_v27?shared_stack_v27->registered_menu_v27(request.menu->identity):nullptr;
   if(fields&&fields->name=="menu_SelectClass"&&registered==request.menu&&request.menu->render==request.render&&
      fields->render==request.render->identity)
    return model_renderer::native_selected_menu_hide_v119(request.menu->identity,e);
  }
  if(request.operation==dh2::ui::MenuStackOperationV1::invoke_as&&request.menu&&request.render&&
     model_renderer::native_derived_menu_fields_v67(request.menu->identity))
   return model_renderer::native_process_derived_menu_callback_v119(request.menu->identity,request.render->identity,request.text,e);
  if(request.operation==dh2::ui::MenuStackOperationV1::play_animation&&request.menu&&request.render&&
     model_renderer::native_derived_menu_fields_v67(request.menu->identity)){
   bool accepted{};
   if(!model_renderer::native_process_derived_menu_animation_v119(request.menu->identity,request.render->identity,
      request.character?request.character->identity:0,request.text,accepted,e))return false;
   request.result=accepted;return true;
  }
  e="Required actual shared MenuManager operation "+std::to_string(unsigned(request.operation));return false;
 };
 platform=std::make_unique<dh2::ui::AuthoredCharacterPanelPlatformV4>(std::move(backend));
 if(!platform->bind(bootstrap_services_v27,e))return false;
 dh2::ui::SwfServices application_services;application_services.context=this;
 application_services.native_owner=leases;application_services.native_action=native;
 if(!original_ui.character_panel_services(application_services,bootstrap_services_v27,e,false))return false;
 dh2::ui::AuthoredSharedMenuServicesV27 roster;
 roster.owner=leases;roster.fields=&fields;roster.localization=bootstrap_services_v27.localization;
 roster.debug=[this](const char* key,auto& out,auto& e){return debug(key,out,e);};
 roster.level_running=[weak=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(app)](bool& out,auto& e){
  auto application=weak.lock();if(!application||application!=application_services_v5){e="Expired SAME process Application.IsLevelRunning owner";return false;}
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;
  if(!model_renderer::borrow_current_native_level_v27(current,e))return false;
  // Original320f7c..88 calls GetCurrentLevel31f594 and normalizes GSLevel::s_level.
  out=bool(current);e.clear();return true;
 };
 roster.process_lifecycle=[weak=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(app)](auto&,const auto& q,auto& result,auto& e){
  auto application=weak.lock();if(!application||application!=application_services_v5){e="Expired SAME process settings/GSInit owner";return false;}
  using O=dh2::ui::AuthoredMenuOperationV1;
  if(q.operation==O::clear_loading_screen)return front_ui.clear_process_splash_v119(e);
  if(q.operation==O::set_saved_language)return front_ui.set_process_language_v109(q.value,e);
  if(q.operation==O::save_settings)return front_ui.save_process_settings_v119(e);
  if(q.operation==O::get_saved_language){auto settings=application->source_settings4c_v67();if(!settings){e="Missing actual App4c settings";return false;}result=settings->language();return true;}
  e="Unsupported actual process startup lifecycle";return false;
 };
 roster.remaining=[this](auto& stack,auto& request,auto& e){
  const auto& source=bootstrap_services_v27.remaining_stack;
  const auto status=source.invoke?source.invoke(source.context,&stack,&request):-1;
  if(status){e=platform->failure();return false;}return true;
 };
 shared_roster_v27=std::make_unique<dh2::ui::AuthoredSharedMenuRosterV27>(shared_stack_v27,std::move(roster));
 if(!bootstrap_services_v27.globals||!shared_stack_v27->publish_source_c1_v104(*bootstrap_services_v27.globals,e)||
    !fields.bind_application_ec_v58(app,&app->source_loading_v55().byte_ec,e))return false;
 process_directory_ready_v104=true;
 const auto weak=weak_from_this();
 if(!front_ui.bind_process_font_cache_reset_v119([weak](std::string& e){auto owner=weak.lock();
    if(!owner){e="Expired SAME process MultiMenuManager renderer directory";return false;}
    return owner->reset_process_fonts_v119(e);},e))return false;
 e.clear();return true;
}
bool reset_process_fonts_v119(std::string& e){
 for(std::uint32_t slot=0;slot<4;++slot){
  dh2::ui::MenuMovieBorrowV58 receipt;
  if(slot==0||slot==2){if(!front_ui.movie_slot_v93(slot,receipt,e))return false;}
  else if(slot==3){if(!original_ui.hud_movie_slot_v93(receipt,e))return false;}
  else continue; //Process MenuManager publishes only primary0/2/3; slot1 is level-owned.
  if(!receipt.identity)continue;
  dh2::ui::SwfMovie* actual{};std::shared_ptr<void> lease;
  if(slot==0){if(!front_ui.base_movie_borrow_v4(actual,lease,e))return false;}
  else if(slot==2){if(!front_ui.main_movie_borrow_v4(actual,lease,e))return false;}
  else if(!original_ui.hud_movie_borrow_v4(actual,lease,e))return false;
  if(!actual||!lease||receipt.identity!=reinterpret_cast<std::uintptr_t>(actual)||!receipt.actual_owner){
   e="Process ResetFonts lost the SAME populated RenderFX slot";return false;
  }
  if(!actual->source_reset_fonts_v119(e))return false;
 }
 e.clear();return true;
}
bool publish_process_movie_v104(std::uint32_t slot,std::string& e,bool perform_postload=true){
 if(!process_directory_ready_v104||!shared_stack_v27||!shared_roster_v27){e="Required actual process movie directory";return false;}
 dh2::ui::MenuMovieBorrowV58 receipt;
 if(slot==0||slot==2){if(!front_ui.movie_slot_v93(slot,receipt,e))return false;}
 else if(slot==3){if(!original_ui.hud_movie_slot_v93(receipt,e))return false;}
 else {e="Process movie publication requires actual primary0/2/3";return false;}
 if(!receipt.identity){e.clear();return true;}
 dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;
 if(slot==0){if(!front_ui.base_movie_borrow_v4(movie,pin,e))return false;base=movie;leases->base=pin;}
 else if(slot==2){if(!front_ui.main_movie_borrow_v4(movie,pin,e))return false;main=movie;leases->main=pin;}
 else {if(!original_ui.hud_movie_borrow_v4(movie,pin,e))return false;hud=movie;leases->hud=pin;}
 if(!movie||reinterpret_cast<std::uintptr_t>(movie)!=receipt.identity||!receipt.actual_owner){e="Process movie facade differs from its actual slot";return false;}
 if(!shared_stack_v27->render(receipt.identity)){
  auto& root=slot==0?base_root:slot==2?main_root:hud_root;
  const auto flags=slot==3?4u:0x84u;
  struct Project {dh2::ui::MenuStackCharacterV1& out;std::uint32_t flags;
   static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){
    auto& p=*static_cast<Project*>(raw);dh2::ui::SwfAsValue root;gameswf::as_object* actual{};
    return graph.root_value(root,e)&&graph.borrow_object(root,actual,e)&&dh2::ui::authored_menu_stack_character_v4(actual,p.flags,{},p.out,e);
   }} project{root,flags};
  if(!movie->menu_action_script(&project,Project::run,e)||
     !shared_stack_v27->register_render_live_v27(receipt.identity,flags,&root,nullptr,e))return false;
 }
 if(slot==0&&!shared_stack_v27->publish_base_render_v104(receipt.identity,e))return false;
 if(slot==3&&!shared_stack_v27->publish_hud_render_v93(receipt.identity,e))return false;
 if(!perform_postload){e.clear();return true;}
 return shared_roster_v27->post_load(*movie,receipt.identity,std::move(pin),slot==3?4u:0x84u,e);
}
