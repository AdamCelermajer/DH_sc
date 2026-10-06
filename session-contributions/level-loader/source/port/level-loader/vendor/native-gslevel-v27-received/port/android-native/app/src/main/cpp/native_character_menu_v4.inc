// GL-thread transport for the original shared + character movies. All profile
// operations borrow the existing World/Save/Gear; this owns only menu state.
dh2::ui::CharacterMenuTutorialQueueV4 character_tutorial_queue;
struct NativeCharacterMenuV4 {
 struct Leases {std::shared_ptr<void> world,hud,base,main,text;};
 std::shared_ptr<Leases> leases=std::make_shared<Leases>();
 dh2::ui::SwfMovie* hud{};dh2::ui::SwfMovie* base{};dh2::ui::SwfMovie* main{};
 dh2::ui::MenuStackCharacterV1 hud_root{},base_root{},main_root{};
 dh2::ui::AuthoredMenuApplicationFieldsV3 fields;
 dh2::ui::AuthoredMenuTouchScreenV3 touch{static_cast<std::int16_t>(surface_width),static_cast<std::int16_t>(surface_height)};
 dh2::ui::AuthoredMenuNativeDrmV4 drm;
 dh2::ui::AuthoredMenuCharacterProjectionV4 projection;
 std::shared_ptr<dh2::ui::MenuStackOwnerV1> shared_stack_v27;
 std::unique_ptr<dh2::ui::AuthoredSharedMenuRosterV27> shared_roster_v27;
 dh2::ui::AuthoredCharacterPanelServicesV2 bootstrap_services_v27;
 std::function<bool(const char*,std::int32_t&,std::string&)> bootstrap_debug_v27;
 bool bootstrap_attempted_v27{},bootstrap_complete_v27{};
 std::unique_ptr<dh2::ui::AuthoredCharacterPanelPlatformV4> platform;
 std::shared_ptr<dh2::ui::AuthoredCharacterApplicationV1> application;
 std::unique_ptr<dh2::ui::CharacterMenuAsBridgeV1> application_bridge;
 std::unique_ptr<dh2::ui::MenuRolloverInputV1> rollover;
 dh2::ui::MenuRolloverBindingsV1 rollover_bindings{};
 std::uintptr_t current_render{};
 bool opened{},request_close{};int width{},height{};
 std::array<std::int32_t,4> stage_rect{};
 std::chrono::steady_clock::time_point frame_time{};
 bool debug(const char* key,std::int32_t& result,std::string& error){
  if(bootstrap_debug_v27)return bootstrap_debug_v27(key,result,error);
  const auto live=model_renderer::player_gameplay_binding();std::uint32_t value{};
  if(!live.debug||!live.debug_files||dh2_character_debug_load(live.debug,live.debug_files)!=1){error="Required same live menu Debug owner/files";return false;}
  if(key&&dh2_character_debug_get(&value,live.debug,key,live.debug_files)!=1){error="Required same live menu Debug switch";return false;}
  result=static_cast<std::int32_t>(value);return true;
 }
 static bool native(void* raw,const char* name,const gameswf::fn_call& call,std::string& error){
  auto& self=*static_cast<NativeCharacterMenuV4*>(raw);
  if(dh2::ui::AuthoredCharacterApplicationV1::owns(name))return self.application_bridge->dispatch(name,call,error);
  if(!std::strcmp(name,"NativeChangeRolloverInputBehavior"))return self.application_bridge->dispatch(name,call,error);
  if(!std::strcmp(name,"NativePlaySoundFX"))return front_ui.character_menu_sound_v4(call,error);
  if(!std::strcmp(name,"NativeGetParsedString"))return original_ui.character_parsed_string_v4(call,error);
  if(!std::strcmp(name,"NativeGetStringNumPotions")||!std::strcmp(name,"NativeGetNumPotions"))return self.application_bridge->dispatch(name,call,error);
  if(!std::strcmp(name,"NativeBackToHud")){self.request_close=true;return true;}
  dh2::ui::CharacterMenuApplicationServicesV4 services;services.owner=self.leases;services.tutorial=&character_tutorial_queue;services.application=&self.fields;
  services.localized=[](std::int32_t id,std::string& text,std::string& e){
   dh2::ui::HudTextV1* actual{};std::shared_ptr<void> owner;
   if(!original_ui.localization_borrow_v4(actual,owner,e))return false;
   const auto live=model_renderer::player_gameplay_binding();bool null=false;
   if(!actual->integer_string(id,live.text_environment.localization,text,null,e))return false;
   if(null){e="Required tutorial string ID resolves to null";return false;}return true;
  };
  services.invoke_tutorial=[](const char* function,std::string& e){
   struct Call {const char* function;static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(raw);dh2::ui::SwfAsValue root,result;bool callable{};
    if(!graph.root_value(root,e)||!graph.invoke(root,root,c.function,{},result,callable,e))return false;
    if(!callable){e="Required actual tutorial skip function";return false;}return true;}} invocation{function};
   return character_panel->authored_graph_scope_v4(&invocation,Call::run,e);
  };
  bool handled=false;if(!dh2::ui::character_menu_application_native_v4(name,call,services,handled,error))return false;
  if(handled)return true;
  error=std::string("Required connected character-menu application callback: ")+name;return false;
 }
 bool bind(std::string& error){
  if(bootstrap_attempted_v27&&!bootstrap_complete_v27){error="Native MenuManager bootstrap failed; retained source prefix cannot be replayed";return false;}
  const auto live=model_renderer::player_gameplay_binding();
  if(!live.active||!live.world_owner){error="Character menu requires the current live player";return false;}
  leases->world=live.world_owner;
  if(!original_ui.hud_movie_borrow_v4(hud,leases->hud,error)||!front_ui.base_movie_borrow_v4(base,leases->base,error)||!front_ui.main_movie_borrow_v4(main,leases->main,error))return false;
  dh2::ui::HudTextV1* text{};
  if(!original_ui.localization_borrow_v4(text,leases->text,error)||!model_renderer::borrow_character_menu_application_v4(*text,leases->text,application,error))return false;
  dh2::ui::MenuRolloverGraphV1 rollover_graph;rollover_graph.owner=leases;
  rollover_graph.integer=[](double value,std::int32_t& out,auto& e){if(!std::isfinite(value)||value<double(INT32_MIN)||value>double(INT32_MAX)){e="Rollover index outside source conversion domain";return false;}out=static_cast<std::int32_t>(value);return true;};
  rollover_graph.instance=[this](auto*& out,auto& e){auto* panel=character_panel->authored_native_v4();auto* stack=panel?panel->source_stack_v4():nullptr;
   if(!stack){e="Required current MenuManager RenderFX roster";return false;}
   rollover_bindings.renders[0]=stack->render(reinterpret_cast<std::uintptr_t>(base));
   rollover_bindings.renders[1]=stack->render(current_render);
   rollover_bindings.renders[2]=stack->render(reinterpret_cast<std::uintptr_t>(main));
   rollover_bindings.renders[3]=stack->render(reinterpret_cast<std::uintptr_t>(hud));out=&rollover_bindings;return true;};
  rollover=std::make_unique<dh2::ui::MenuRolloverInputV1>(std::move(rollover_graph));
  application_bridge=std::make_unique<dh2::ui::CharacterMenuAsBridgeV1>(leases,[this](const char* name,auto& call,auto& e){
   if(!std::strcmp(name,"NativeChangeRolloverInputBehavior"))return rollover->dispatch(call,e);
   if(!std::strcmp(name,"NativeGetStringNumPotions")||!std::strcmp(name,"NativeGetNumPotions")){
    dh2::ui::CharacterMenuPotionServicesV4 services;services.owner=leases;
    services.player=[](auto index,auto remote,auto& identity,auto& e){return model_renderer::character_menu_player_v4(index,remote,false,identity,e);};
    services.local_player=[](auto index,auto include,auto& identity,auto& e){return model_renderer::character_menu_player_v4(index,include,true,identity,e);};
    services.count=[](auto identity,auto& count,auto& e){const auto p=model_renderer::player_gameplay_binding();if(!p.active||p.character!=identity||!p.gear||!p.gear->inventory()){e="Required same player's retained potion inventory";return false;}count=p.gear->inventory()->num_potions();return true;};
    services.capacity=[](auto identity,auto& capacity,auto& e){const auto p=model_renderer::player_gameplay_binding();if(!p.active||p.character!=identity||!p.gear||!p.gear->inventory()){e="Required same player's potion capacity byte";return false;}capacity=p.gear->inventory()->potion_capacity_v4();return true;};
    services.localized_format=[](auto& format,auto& null,auto& e){dh2::ui::HudTextV1* actual{};std::shared_ptr<void> pin;if(!original_ui.localization_borrow_v4(actual,pin,e))return false;const auto p=model_renderer::player_gameplay_binding();return dh2::ui::character_menu_potion_localized_format_v4(*actual,p.text_environment,format,null,e);};
    services.parse_integer=[](const char* format,auto count,auto& out,auto& e){dh2::ui::HudTextV1* actual{};std::shared_ptr<void> pin;if(!original_ui.localization_borrow_v4(actual,pin,e))return false;const auto p=model_renderer::player_gameplay_binding();return dh2::ui::character_menu_potion_integer_text_v4(*actual,p.text_environment,format,count,out,e);};
    bool handled=false;return dh2::ui::character_menu_potions_call_v4(name,call,services,handled,e)&&handled;
   }
   return application->dispatch(name,call,e);
  });
  auto project=[](dh2::ui::SwfMovie& movie,std::uint32_t flags,dh2::ui::MenuStackCharacterV1& out,std::string& e){
   struct Call {std::uint32_t flags;dh2::ui::MenuStackCharacterV1& out;
    static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(raw);dh2::ui::SwfAsValue root;gameswf::as_object* object{};
     return graph.root_value(root,e)&&graph.borrow_object(root,object,e)&&dh2::ui::authored_menu_stack_character_v4(object,c.flags,{},c.out,e);}} call{flags,out};
   return movie.menu_action_script(&call,Call::run,e);
  };
  if(!project(*hud,4,hud_root,error)||!project(*base,0x84,base_root,error)||!project(*main,0x84,main_root,error))return false;
  dh2::ui::AuthoredCharacterPanelServicesV2 services;
  dh2::ui::SwfServices app;app.context=this;app.native_owner=leases;app.native_action=native;app.native_actions={"NativeReloadSkills"};
  if(!original_ui.character_panel_services(app,services,error))return false;
  services.localization.set_context=[this](const dh2::ui::SwfAsValue& value,std::string& e){
   struct Call {NativeCharacterMenuV4& self;const dh2::ui::SwfAsValue& value;
    static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(raw);gameswf::as_object* object{};
     if(!graph.borrow_object(c.value,object,e)||!object)return false;
     auto* panel=character_panel->authored_native_v4();auto* stack=panel?panel->source_stack_v4():nullptr;auto* render=stack?stack->render(c.self.current_render):nullptr;
     if(!render){e="Required current character RenderFX localization context";return false;}
     const auto identity=reinterpret_cast<std::uintptr_t>(object);
     if(render->root&&render->root->identity==identity){render->context=render->root;return true;}
     for(std::uint32_t i=0;i<render->catalog_count;++i){auto* menu=render->catalog[i];if(menu&&menu->character&&menu->character->identity==identity){render->context=menu->character;return true;}}
     e="Original localization context absent from the same MenuFX catalog";return false;
    }} call{*this,value};return character_panel->authored_graph_scope_v4(&call,Call::run,e);
  };
  services.maximum_occurrences=256; // Native storage budget; no menu behavior replacement.
  services.hud={reinterpret_cast<std::uintptr_t>(hud),4,&hud_root,nullptr};
  services.base={reinterpret_cast<std::uintptr_t>(base),0x84,&base_root,nullptr};
  services.additional_renders.push_back({reinterpret_cast<std::uintptr_t>(main),0x84,&main_root,nullptr});
  services.character=[](gameswf::as_object* object,auto& out,auto& e){return dh2::ui::authored_menu_stack_character_v4(object,0x84,{},out,e);};
  services.panel_render_flags=[](auto& flags,auto&){flags=0x84;return true;};
  services.publish_render=[this](auto id,auto&){current_render=id;return true;};
  dh2::ui::AuthoredCharacterPanelPlatformServicesV4 backend;
  backend.owner=leases;backend.fields=&fields;backend.touch=&touch;
  backend.native_drm=[this](bool& result,auto&){result=drm.source_global();return true;};
  backend.scoped_graph=[](void* raw,auto apply,auto& e){return character_panel->authored_graph_scope_v4(raw,apply,e);};
  backend.debug=[this](const char* key,auto& result,auto& e){return debug(key,result,e);};
  backend.deadzone_root=[this](auto& graph,const auto& name,auto*& out,auto& e){return projection.root(graph,name,out,e);};
  backend.absolute_bounds=[this](auto&,auto& node,auto& out,auto& e){return projection.absolute_bounds(node,out,e);};
  backend.required_lifecycle={leases,[](auto&,const auto& request,auto&,auto& e){e="Required character-menu lifecycle continuation "+std::to_string(unsigned(request.operation));return false;}};
  backend.required_stack=[](auto&,auto& request,auto& e){e="Required character-menu stack continuation "+std::to_string(unsigned(request.operation));return false;};
  if(!platform){platform=std::make_unique<dh2::ui::AuthoredCharacterPanelPlatformV4>(std::move(backend));if(!platform->bind(services,error))return false;}
  else{services.globals=bootstrap_services_v27.globals;services.lifecycle=bootstrap_services_v27.lifecycle;services.remaining_stack=bootstrap_services_v27.remaining_stack;}
  services.shared_stack_v27=shared_stack_v27;
  dh2::android_ui::CharacterPanelMovieRuntimeV3 movie;
  movie.owner=leases;
  movie.player_index=[](double number,std::int32_t& index,std::string& e){
   if(!std::isfinite(number)||number<double(INT32_MIN)||number>double(INT32_MAX)){e="Character-menu player index outside native conversion domain";return false;}
   index=static_cast<std::int32_t>(number);return true;
  };
  movie.current_menu_fx=[this](auto& id,auto& e){if(!current_render){e="Required published current character MenuFX";return false;}id=current_render;return true;};
  movie.invoke=[this](auto id,const char* path,const char* method,bool optional,std::string& e){
   if(id!=current_render){e="Character callback addressed a different MenuFX";return false;}
   struct Call {const char* path;const char* method;bool optional;
    static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Call*>(raw);dh2::ui::SwfAsValue root,receiver,result;bool callable{};
     if(!graph.root_value(root,e)||!graph.find_target(root,c.path,receiver,e)||!graph.invoke(receiver,receiver,c.method,{},result,callable,e))return false;
     if(!callable&&!c.optional){e="Required actual character movie callback";return false;}return true;}} call{path,method,optional};
   return character_panel->authored_graph_scope_v4(&call,Call::run,e);
  };
  dh2::android_ui::CharacterPanelGameplayServicesV2 gameplay;
  if(!model_renderer::connect_character_panel_runtime(movie,gameplay,error))return false;
  const auto preceding_actions=gameplay.actions;
  gameplay.actions=[preceding_actions](const auto& p,auto& graph,auto& e){
   if(preceding_actions&&!preceding_actions(p,graph,e))return false;
   graph.swap_hud=[](const char* callback,auto& e){return original_ui.character_swap_hud_v4(callback,e);};return true;
  };
  character_panel->bind_gameplay_services(std::move(gameplay));
  if(!character_panel->initialize_authored(live,services,error))return false;
  if(!original_ui.character_inventory_preview_v4(*character_panel->authored_native_v4()->movie()->movie(),error))return false;
  return viewport(error);
 }
 bool viewport(std::string& error){
  if(width==surface_width&&height==surface_height)return true;
  const auto live=model_renderer::player_gameplay_binding();dh2::ui::ViewportState64 seed{};
  if(!character_panel->authored_graph_scope_v4(&seed,[](void* raw,auto& graph,auto& e){return dh2::ui::authored_menu_movie_rect_v4(graph,static_cast<dh2::ui::ViewportState64*>(raw)->movie_rect,e);},error))return false;
  seed.viewport[2]=seed.bounds[2]=surface_width;seed.viewport[3]=seed.bounds[3]=surface_height;seed.pixel_scale=1;
  seed.player_receiver=character_panel->authored_native_v4()->movie()->movie()->player_identity();
  std::copy_n(seed.movie_rect,4,stage_rect.begin());
  dh2::ui::SwfViewportDriver driver{this,[](void*,auto& out,auto&){out=0;return true;},[](void*,auto& w,auto& h,auto&){w=surface_width;h=surface_height;return true;}};
  if(!character_panel->authored_viewport(live,seed,driver,error))return false;
  auto* panel=character_panel->authored_native_v4();dh2::ui::FlashCamera40 camera{};
  const std::int32_t bounds[]{0,0,surface_width,surface_height};
  if(!panel->movie()->movie()->update_viewport(camera,error)||!panel->movie()->movie()->set_source_bounds(bounds,2,error))return false;
  width=surface_width;height=surface_height;return true;
 }
 bool open(std::string& error){
  if(!character_panel->authored_bound()&&!bind(error))return false;
  std::string command;if(!original_ui.hud_pointer(3,-1,0,0,command,error))return false;
  if(!character_panel->authored_open(model_renderer::player_gameplay_binding(),error))return false;
  model_renderer::world_touch_cancel();model_renderer::set_character_panel_open(true);
  opened=true;request_close=false;frame_time=std::chrono::steady_clock::now();
  __android_log_print(ANDROID_LOG_INFO,tag,"Original character menu opened | actual stats/skills/inventory artwork | same live player");return true;
 }
 void close(){opened=false;request_close=false;model_renderer::set_character_panel_open(false);}
 bool frame(std::string& error){
  const auto live=model_renderer::player_gameplay_binding();
  if(live.world_owner!=leases->world){error="Character menu world was replaced";close();return false;}
  if(!viewport(error))return false;
  const auto now=std::chrono::steady_clock::now();const float dt=std::chrono::duration<float>(now-frame_time).count();frame_time=now;
  if(!character_panel->authored_frame(live,dt,error))return false;
  auto* stack=character_panel->authored_native_v4()->stack();
  if(request_close||(stack&&stack->view()->count==0)){close();return true;}
  return character_panel->authored_source_display_v4(live,error);
 }
 bool pointer(int phase,int id,float x,float y,std::string& error){
  const auto live=model_renderer::player_gameplay_binding();
  if(phase==3&&id<0){return character_panel->authored_back(live,error);}
  float logical[]{x,y};
  if(!character_panel->authored_native_v4()->movie()->movie()->screen_to_logical(logical,error))return false;
  const bool inside=logical[0]*20.f>=stage_rect[0]&&logical[0]*20.f<=stage_rect[1]&&logical[1]*20.f>=stage_rect[2]&&logical[1]*20.f<=stage_rect[3];
  if(!inside){if(phase==0)return true;return character_panel->authored_pointer(live,3,id,x,y,error);}
  const int source_phase=phase==1?2:phase==2?1:phase;
  return character_panel->authored_pointer(live,source_phase,id,x,y,error);
 }
 #include "native_menu_loading_bootstrap_v27.inc"
};
std::unique_ptr<NativeCharacterMenuV4> authored_character_menu;
