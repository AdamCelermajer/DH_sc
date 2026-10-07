// Included INSIDE NativeCharacterMenuV4. One player-independent manager
// bootstrap, followed later by the same character-panel stack adoption.
bool bootstrap_loading_v27(std::shared_ptr<void> actual_world,
 std::function<bool(const char*,std::int32_t&,std::string&)> actual_debug,
 std::string& error){
 if(process_directory_ready_v104){
  if(!actual_world||!actual_debug||(leases->world&&leases->world!=actual_world)){
   error="Required actual process MenuManager World attachment after prior source unload";return false;
  }
  leases->world=std::move(actual_world);bootstrap_debug_v27=std::move(actual_debug);
  for(const auto slot:{0u,2u,3u})if(!publish_process_movie_v104(slot,error))return false;
  if(!base||!main||!hud){error="World loading requires the actual base/main/HUD movie resources";return false;}
  bootstrap_attempted_v27=true;bootstrap_complete_v27=true;error.clear();return true;
 }
 if(bootstrap_complete_v27){if(leases->world!=actual_world){error="Loading MenuManager belongs to another World";return false;}return true;}
 if(bootstrap_attempted_v27){error="Native MenuManager bootstrap already attempted; source prefix retained";return false;}
 if(!actual_world||!actual_debug||platform){error="Required fresh real loading MenuManager/world/debug";return false;}
 bootstrap_attempted_v27=true;
 leases->world=std::move(actual_world);bootstrap_debug_v27=std::move(actual_debug);
 if(!original_ui.hud_movie_borrow_v4(hud,leases->hud,error)||!front_ui.base_movie_borrow_v4(base,leases->base,error)||!front_ui.main_movie_borrow_v4(main,leases->main,error))return false;
 auto project=[](dh2::ui::SwfMovie& movie,std::uint32_t flags,auto& out,auto& e){
  struct Call {std::uint32_t flags;dh2::ui::MenuStackCharacterV1& out;
   static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& error){auto& c=*static_cast<Call*>(raw);dh2::ui::SwfAsValue root;gameswf::as_object* object{};
    return graph.root_value(root,error)&&graph.borrow_object(root,object,error)&&dh2::ui::authored_menu_stack_character_v4(object,c.flags,{},c.out,error);}} call{flags,out};
  return movie.menu_action_script(&call,Call::run,e);
 };
 if(!project(*base,0x84,base_root,error)||!project(*main,0x84,main_root,error)||!project(*hud,4,hud_root,error))return false;
 shared_stack_v27=std::make_shared<dh2::ui::MenuStackOwnerV1>(256); // native bounded storage policy
 if(!shared_stack_v27->add_render(reinterpret_cast<std::uintptr_t>(base),0x84,&base_root,nullptr,error)||
    !shared_stack_v27->add_render(reinterpret_cast<std::uintptr_t>(main),0x84,&main_root,nullptr,error)||
    !shared_stack_v27->add_render(reinterpret_cast<std::uintptr_t>(hud),4,&hud_root,nullptr,error))return false;
 dh2::ui::AuthoredCharacterPanelPlatformServicesV4 backend;backend.owner=leases;backend.fields=&fields;backend.touch=&touch;
 backend.native_drm=[this](bool& value,auto&){value=drm.source_global();return true;};
 backend.debug=[this](const char* key,auto& value,auto& e){return debug(key,value,e);};
 backend.scoped_graph=[](void* raw,auto fn,auto& e){if(!character_panel){e="Required actual character callback graph";return false;}return character_panel->authored_graph_scope_v4(raw,fn,e);};
 backend.deadzone_root=[this](auto& graph,const auto& name,auto*& out,auto& e){return projection.root(graph,name,out,e);};
 backend.absolute_bounds=[this](auto&,auto& node,auto& out,auto& e){return projection.absolute_bounds(node,out,e);};
 backend.required_lifecycle={leases,[](auto&,const auto& q,auto&,auto& e){e="Required native MenuBase lifecycle "+std::to_string(unsigned(q.operation));return false;}};
 backend.required_stack=[this](auto& stack,auto& q,auto& e){bool handled{};
  if(shared_roster_v27&&!shared_roster_v27->route(stack,q,handled,e))return false;
  if(handled)return true;e="Required native shared MenuManager service "+std::to_string(unsigned(q.operation));return false;};
 platform=std::make_unique<dh2::ui::AuthoredCharacterPanelPlatformV4>(std::move(backend));
 if(!platform->bind(bootstrap_services_v27,error))return false;
 // This provider only lends the actual resource/text services to already
 // loaded movies. New character callbacks are connected later by bind().
 dh2::ui::SwfServices app;app.context=this;app.native_owner=leases;app.native_action=native;
 if(!original_ui.character_panel_services(app,bootstrap_services_v27,error))return false;
 dh2::ui::AuthoredSharedMenuServicesV27 roster;roster.owner=leases;roster.fields=&fields;roster.localization=bootstrap_services_v27.localization;
 roster.debug=[this](const char* key,auto& out,auto& e){return debug(key,out,e);};
 roster.remaining=[this](auto& stack,auto& q,auto& e){auto& s=bootstrap_services_v27.remaining_stack;
  const int status=s.invoke?s.invoke(s.context,&stack,&q):-1;
  if(status){e=platform->failure();return false;}return true;};
 shared_roster_v27=std::make_unique<dh2::ui::AuthoredSharedMenuRosterV27>(shared_stack_v27,std::move(roster));
 // Source PostLoad order: base0, current-character1(absent), main2, HUD3.
 if(!shared_roster_v27->post_load(*base,leases->base,0x84,error)||
    !shared_roster_v27->post_load(*main,leases->main,0x84,error)||
    !shared_roster_v27->post_load(*hud,leases->hud,4,error))return false;
 if(!shared_stack_v27->seal(reinterpret_cast<std::uintptr_t>(hud),reinterpret_cast<std::uintptr_t>(base),*bootstrap_services_v27.globals,error))return false;
 bootstrap_complete_v27=true;return true;
}
