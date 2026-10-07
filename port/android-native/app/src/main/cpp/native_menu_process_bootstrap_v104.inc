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
  if(handled)return true;e="Required actual shared MenuManager operation "+std::to_string(unsigned(request.operation));return false;
 };
 platform=std::make_unique<dh2::ui::AuthoredCharacterPanelPlatformV4>(std::move(backend));
 if(!platform->bind(bootstrap_services_v27,e))return false;
 dh2::ui::SwfServices application_services;application_services.context=this;
 application_services.native_owner=leases;application_services.native_action=native;
 if(!original_ui.character_panel_services(application_services,bootstrap_services_v27,e,false))return false;
 dh2::ui::AuthoredSharedMenuServicesV27 roster;
 roster.owner=leases;roster.fields=&fields;roster.localization=bootstrap_services_v27.localization;
 roster.debug=[this](const char* key,auto& out,auto& e){return debug(key,out,e);};
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
 process_directory_ready_v104=true;e.clear();return true;
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
 return shared_roster_v27->post_load(*movie,std::move(pin),slot==3?4u:0x84u,e);
}
