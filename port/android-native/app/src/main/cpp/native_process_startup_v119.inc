// Include after native_main_menu_load/source_main_menu providers. One retained
// source GSInit, one actual MenuManager initialization counter, no World owner.
#include "android_intro_movie_v119.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "native_process_menu_actions_v119.inc"
namespace {
struct NativeProcessStartupV119:std::enable_shared_from_this<NativeProcessStartupV119>{
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> app;
 std::string directory,first_error;
 std::unique_ptr<dh2::android_ui::SourceGSInitProcessOwnersV100> source;
 std::uint32_t menu_stage_bc{}; //MenuManager C1 initializes original bc=0.
 bool menu_busy{};unsigned attach_index{};
 bool avatar_services_bound_v121{};
 bool movies_selected_v132{};
 struct MainRenderedBGCallbackV119 {std::weak_ptr<NativeProcessStartupV119> owner;std::uintptr_t render{},character{};};
 std::map<std::uintptr_t,MainRenderedBGCallbackV119> main_rendered_bg_callbacks_v119;
 std::map<std::uintptr_t,std::unique_ptr<dh2::ui::AuthoredMenuDragV68>> drag_owners;
 struct DerivedDeadZonesV119 {std::uint8_t registered{};std::vector<dh2::ui::AuthoredMenuDeadZoneV3> zones;};
 std::map<std::uintptr_t,DerivedDeadZonesV119> deadzone_owners;
 bool current(std::string& e)const{
  if(!first_error.empty()){e=first_error;return false;}
  if(app.lock()!=application_services_v5||directory!=front_directory){e="Replaced actual first-boot Application/private directory";return false;}return true;
 }
 bool fields(std::uintptr_t id,dh2::ui::AuthoredMenuFieldsV1*& f,dh2::ui::AuthoredCharacterStateV1*& s,std::string& e){
  f=nullptr;s=nullptr;if(!current(e))return false;
  for(auto& owner:source_singleton_v67)if(owner&&owner->fields.identity==id){f=&owner->fields;s=&owner->state;return true;}
  if(source_map_menu_v62&&source_map_menu_v62->fields().identity==id){f=&source_map_menu_v62->fields();s=&source_map_menu_v62->state();return true;}
  if(auto* debug=source_menu_process_v62->existing_debug_hud();debug&&debug->fields().identity==id){f=&debug->fields();s=&debug->state();return true;}
  e="Required exact retained derived MenuBase receiver";return false;
 }
 bool scoped(dh2::ui::AuthoredMenuFieldsV1& f,const std::function<bool(dh2::ui::SwfAsGraph&,std::string&)>& fn,std::string& e){
  return scoped_movie(f,[&](dh2::ui::SwfMovie&,dh2::ui::SwfAsGraph& graph,std::string& error){return fn(graph,error);},e);
 }
 bool scoped_movie(dh2::ui::AuthoredMenuFieldsV1& f,const std::function<bool(dh2::ui::SwfMovie&,dh2::ui::SwfAsGraph&,std::string&)>& fn,std::string& e){
  auto p=source_postmovie_v62.lock();if(!p||!current(e))return false;
  for(unsigned slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!p->directory->movie_slot(slot,q,e))return false;if(q.identity!=f.render)continue;
   dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;if(!p->directory->movie(slot,q,movie,pin,e))return false;
   struct Invoke{dh2::ui::SwfMovie& movie;const std::function<bool(dh2::ui::SwfMovie&,dh2::ui::SwfAsGraph&,std::string&)>& fn;
    static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& call=*static_cast<Invoke*>(raw);return call.fn(call.movie,graph,e);}} invoke{*movie,fn};
   return movie->menu_action_script(&invoke,Invoke::run,e);
  }e="Derived MenuBase lost its actual RenderFX resource";return false;
 }
 static bool main_rendered_bg_display_v119(void* raw,const dh2::ui::SwfDraw&,std::string& e){
  auto* callback=static_cast<MainRenderedBGCallbackV119*>(raw);auto owner=callback?callback->owner.lock():nullptr;
  if(!owner||!owner->current(e)||!callback->render||!callback->character){if(e.empty())e="Retired actual Main.Show RenderedBG callback owner";return false;}
  // IDA: MenuMainMenu::RenderCharacterPane at 0x42bb44 is exactly `bx lr`.
  // Its faithful display callback is therefore a checked source no-op; the
  // retained main-menu scene is submitted by its existing scene owner.
  e.clear();return true;
 }
 bool register_drag(std::uintptr_t id,std::string& e){
  auto n=authored_character_menu;if(!n){e="Missing actual process MenuManager";return false;}
  if(n->shared_roster_v27->receiver_fields_v59(id))return n->shared_roster_v27->register_drag_and_drops_v68(id,e);
  dh2::ui::AuthoredMenuFieldsV1* f{};dh2::ui::AuthoredCharacterStateV1* s{};if(!fields(id,f,s,e))return false;
  std::int32_t ignored{};if(!n->debug(nullptr,ignored,e)||!n->debug("isTracingMenuBase",ignored,e))return false;
  drag_owners.erase(id);f->drag5c=0;
  if(!f->render){e.clear();return true;}
  return scoped(*f,[&](auto& graph,auto& e){dh2::ui::SwfAsValue value;bool live{};if(!s->weak_character_v59.borrow(graph,value,live,e))return false;if(!live)return true;
   auto& drag=drag_owners[id];drag=std::make_unique<dh2::ui::AuthoredMenuDragV68>();f->drag5c=reinterpret_cast<std::uintptr_t>(drag.get());
   dh2::ui::AuthoredMenuCharacterProjectionV4 projection;dh2::ui::AuthoredMenuCharacterBorrowV3* root{};
   if(!projection.root(graph,f->name,root,e))return false;
   const auto dimensions=[](auto& w,auto& h,auto& e){if(surface_width<=1||surface_height<=1){e="Required actual native drag screen";return false;}w=surface_width;h=surface_height;return true;};
   std::vector<dh2::ui::AuthoredMenuCharacterBorrowV3*> nodes;if(!dh2::ui::authored_menu_collect_characters_v3(root,"dragable",0,nodes,e))return false;
   for(auto* node:nodes){std::vector<dh2::ui::AuthoredMenuCharacterBorrowV3*> children;if(!dh2::ui::authored_menu_collect_characters_v3(node,nullptr,0,children,e))return false;
    dh2::ui::AuthoredMenuCharacterBorrowV3* limits{};for(auto* child:children)if(std::strstr(child->name,"draglimits")){limits=child;break;}
    if(!drag->add_drag(graph,f->render,*node,limits,projection,dimensions,e))return false;
   }
   nodes.clear();if(!dh2::ui::authored_menu_collect_characters_v3(root,"drop",0,nodes,e))return false;
   for(auto* node:nodes)if(!drag->add_drop(graph,f->render,*node,e))return false;
   if(drag->empty()){if(!n->debug(nullptr,ignored,e)||!n->debug("isTracingMenuBase",ignored,e))return false;drag_owners.erase(id);f->drag5c=0;}return true;
  },e);
 }
 bool base_hide(std::uintptr_t id,std::string& e){
  auto n=authored_character_menu;if(!n||!current(e))return false;
  if(n->shared_roster_v27->receiver_fields_v59(id))return n->shared_roster_v27->hide_receiver_v68(id,e);
  dh2::ui::AuthoredMenuFieldsV1* f{};dh2::ui::AuthoredCharacterStateV1* state{};if(!fields(id,f,state,e))return false;
  dh2::ui::AuthoredMenuLifecycleServicesV1 services;services.owner=shared_from_this();
  services.invoke=[&](auto& actual,const auto& q,auto& result,auto& e){using O=dh2::ui::AuthoredMenuOperationV1;
   if(&actual!=f){e="Changed actual MenuBase.Hide receiver";return false;}
   switch(q.operation){
   case O::debug_load:case O::debug_query:return n->debug(q.text,result,e);
   case O::clear_manager_60:n->fields.manager60()=0;return true;
   case O::store_igm_opened:n->fields.igm_opened()=std::uint8_t(q.value);return true;
   case O::store_application_ec:n->fields.store_application_ec_v58(std::uint8_t(q.value));return true;
   case O::unregister_listener:return n->fields.unregister_listener(id,[&](auto& e){std::int32_t ignored{};return n->debug(nullptr,ignored,e);},e);
   case O::reset_drag_positions:return scoped(*f,[&](auto& graph,auto& e){auto at=drag_owners.find(id);if(at==drag_owners.end()||f->drag5c!=reinterpret_cast<std::uintptr_t>(at->second.get())){e="Required SAME derived MenuBase drag5c";return false;}return at->second->reset_positions(graph,e);},e);
   case O::set_visible:case O::invoke_as:return scoped(*f,[&](auto& graph,auto& e){return dh2::ui::authored_character_movie_operation_v1(graph,*state,q,n->bootstrap_services_v27.localization,e);},e);
   default:e="Unsupported reached derived MenuBase.Hide source operation";return false;
   }
  };
  return dh2::ui::authored_menu_hide_v1(*f,services,e);
 }
 bool base_show(std::uintptr_t id,std::string& e){
  auto n=authored_character_menu;if(!n||!current(e))return false;
  dh2::ui::AuthoredMenuFieldsV1* f{};dh2::ui::AuthoredCharacterStateV1* state{};if(!fields(id,f,state,e))return false;
  dh2::ui::AuthoredMenuLifecycleServicesV1 services;services.owner=shared_from_this();
  services.invoke=[&](auto& actual,const auto& q,auto& result,auto& error){using O=dh2::ui::AuthoredMenuOperationV1;
   if(&actual!=f){error="Changed actual qualified MenuBase.Show receiver";return false;}
   switch(q.operation){
   case O::debug_load:case O::debug_query:return n->debug(q.text,result,error);
   case O::store_rollover_event_enabled:n->fields.rollover()=std::uint8_t(q.value);return true;
   case O::store_application_ec:n->fields.store_application_ec_v58(std::uint8_t(q.value));return true;
   case O::store_igm_opened:n->fields.igm_opened()=std::uint8_t(q.value);return true;
   case O::set_visible:case O::invoke_as:case O::localize:return scoped(*f,[&](auto& graph,auto& error){
    auto localization=n->bootstrap_services_v27.localization;
    localization.set_context=[&](const auto& value,auto& error){
     auto* menu=n->shared_stack_v27->registered_menu_v27(id);auto* render=n->shared_stack_v27->render(f->render);
     if(!menu||!render||!menu->character||menu->character->identity!=value.identity()){error="Qualified MenuBase.Show lost SAME localization character";return false;}
     render->context=menu->character;return true;
    };
    return dh2::ui::authored_character_movie_operation_v1(graph,*state,q,localization,error);
   },error);
   case O::register_deadzones:return scoped(*f,[&](auto& graph,auto& error){
    auto& zones=deadzone_owners[id];dh2::ui::AuthoredMenuCharacterProjectionV4 projection;dh2::ui::AuthoredMenuCharacterBorrowV3* root{};
    if(!projection.root(graph,f->name,root,error))return false;
    dh2::ui::AuthoredMenuDeadZoneServicesV3 source;
    source.debug=[&](auto& error){std::int32_t ignored{};return n->debug(nullptr,ignored,error);};
    source.bounds=[&](auto& node,auto& out,auto& error){return projection.absolute_bounds(node,out,error);};
    return dh2::ui::authored_menu_register_deadzones_v3(zones.registered,root,source,zones.zones,error);
   },error);
   default:error="Unsupported reached qualified MenuBase.Show source operation";return false;
   }
  };
  return dh2::ui::authored_menu_show_v1(*f,services,e);
 }
 bool menu_init(bool& complete,std::string& e){
  complete=false;if(!current(e)||menu_busy){if(e.empty())e="Recursive actual MenuManager.Init";return false;}
  menu_busy=true;struct Busy{bool& v;~Busy(){v=false;}} busy{menu_busy};
  auto application=app.lock();auto native=authored_character_menu;
  //Application owns the original PM before Menu initialization. Construct or
  //adopt the actual existing manager only; no controller/AddPlayer/profile tail.
  if(menu_stage_bc==0&&!model_renderer::prepare_actual_process_player_manager_v119(application,e))return false;
  if(!native){
   auto weak=std::weak_ptr<NativeProcessStartupV119>(shared_from_this());
   if(!model_renderer::prepare_native_menu_process_v104(application,[weak](const char* key,auto& value,auto& e){
    auto self=weak.lock();if(!self||!self->current(e))return false;
    dh2::android_ui::ProcessTrophyServicesV100 services;if(!original_ui.process_trophy_services_v119(self->app.lock(),services,e))return false;
    if(!services.debug||!services.debug_files||!services.debug_file_owner){e="Required actual MenuManager Debug/FileManager";return false;}
    const int loaded=dh2_character_debug_load(services.debug.get(),services.debug_files);if(loaded<0){e="MenuManager Debug.Load failed";return false;}
    if(!key){value=0;e.clear();return true;}std::uint32_t word{};
    if(dh2_character_debug_get(&word,services.debug.get(),key,services.debug_files)<0){e="MenuManager Debug.GetSwitch failed";return false;}
    std::memcpy(&value,&word,4);return true;
   },e))return false;
   native=authored_character_menu;
  }
  if(!native||native->process_application_v104.lock()!=application||!native->shared_stack_v27||!native->shared_roster_v27){e="Required SAME process MenuManager source C1";return false;}
  auto provider=source_postmovie_v62.lock();if(!provider){e="Required actual process menu resource providers";return false;}
  if(!avatar_services_bound_v121){
   dh2::ui::MenuAvatarPreviewServicesV1 avatar;std::shared_ptr<void> adapter;
   //Borrow only actual process/cache owners here. Scene/camera/player C1
   //remain source Show/Hide/NativeSetSlot effects; first movie callbacks must
   //already have the genuine weak adapter instead of persona-camera no-op.
   if(!model_renderer::native_menu_avatar_services_v121(application,avatar,adapter,e)||
      !front_ui.bind_menu_avatar_services_v121(avatar,std::move(adapter),e))return false;
   avatar_services_bound_v121=true;
  }
  auto singleton=[&](const char* name){std::uintptr_t id{};return model_renderer::create_native_process_menu_singleton_v104(application,name,id,e);};
  switch(menu_stage_bc){
  case 0:
   if(!movies_selected_v132){
    if(!provider->loadmenu1_services_v98.facts){e="Required actual process source movie facts";return false;}
    model_renderer::NativePrimaryMenuFactsV98 facts;if(!provider->loadmenu1_services_v98.facts(facts,e))return false;
    auto settings=application->source_settings4c_v67();
    if(!facts.receiver||!facts.width_screen||!settings){e="Required SAME Width_Screen/SavegameManager source selector";return false;}
    const auto width=*facts.width_screen;
    if(width==800&&(!facts.htc_devices||(!*facts.htc_devices&&!facts.no_igp))){e="Required reached HTC_DEVICES/NO_IGP source selector cells";return false;}
    const auto variant=dh2::ui::source_menu_variant_v132(width,settings->language(),width==800&&*facts.htc_devices,width==800&&!*facts.htc_devices&&*facts.no_igp);
    if(!front_ui.select_process_movies_v132(variant,e)||!original_ui.select_process_hud_v132(variant,e))return false;
    movies_selected_v132=true;
   }
   if(!front_ui.load_base_movie_slot_v105(e)||!native->publish_process_movie_v104(0,e,false))return false;break;
  case 1:{ //Actual primary1 and camera1 remain constructor NULL, not loaded here.
   dh2::ui::MenuMovieBorrowV58 slot;if(!provider->directory->movie_slot(1,slot,e))return false;
   if(slot.identity){e="MenuManager.Init1 addressed a nonfresh primary1";return false;}break;}
  case 2:
   if(!front_ui.load_main_movie_slot_v93(e)||!native->publish_process_movie_v104(2,e,false))return false;break;
  case 3:
   if(!original_ui.source_load_hud_primary_v63(directory,e)||!native->publish_process_movie_v104(3,e,false))return false;break;
  case 4:{
   if(!native->hud){e="Required real Init4 HUD primary3";return false;}
   const auto id=reinterpret_cast<std::uintptr_t>(native->hud);
   if(!original_ui.loadmenu3_info_store_v62(id,e)||!original_ui.construct_hud_controls_v62(application->events14(),e)||
      !original_ui.loadmenu3_controls_store_v62(id,e)||!original_ui.source_scan_loaded_hud_v63(id,e))return false;
   break;}
  case 5:if(!front_ui.load_process_property_names_v119(e))return false;break;
  case 7:if(!singleton("menu_MainMenu"))return false;break;
  case 9:if(!singleton("menu_SelectClass"))return false;break;
  case 11:
   if(!singleton("menu_InventorySheetMain")||!singleton("menu_MapSheet")||!singleton("menu_FastTravel"))return false;break;
  case 13:if(!singleton("menu_EnterName"))return false;break;
  case 14:if(!singleton("menu_MultiplayerLobbyMulti"))return false;break;
  case 17:if(!singleton("menu_Debug")||!singleton("menu_DebugHUD"))return false;break;
  case 18:if(!singleton("menu_Merchant"))return false;break;
  case 19:if(!singleton("menu_Confirm"))return false;break;
  case 20:if(!singleton("menu_HUDConfirm"))return false;break;
  case 23:{
   native->shared_roster_v27->bind_screen_dimensions_v68([](auto& w,auto& h,auto& e){if(surface_width<=1||surface_height<=1){e="Required produced source drag dimensions";return false;}w=surface_width;h=surface_height;return true;});
   for(unsigned slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!provider->directory->movie_slot(slot,q,e))return false;if(!q.identity)continue;
    dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;if(!provider->directory->movie(slot,q,movie,pin,e))return false;
    auto* render=native->shared_stack_v27->render(q.identity);if(!render){e="Init23 PostLoad lacks SAME actual RenderFX";return false;}
    if(!native->shared_roster_v27->post_load(*movie,q.identity,std::move(pin),render->flags,e))return false;
   }
   front_ui.unload_process_property_names_v119();
   if(native->hud){
    std::weak_ptr<NativeMenuPostMovieV62> weak=provider;
    if(!bind_native_hud_fs_v114(provider,e)||!native->hud->source_set_event_listener_v68(reinterpret_cast<std::uintptr_t>(native->shared_stack_v27.get()),provider,
      [weak](auto& event,auto& e){auto p=weak.lock();if(!p){e="Expired process HUD event provider";return false;}return source_hud_menu_event_v68(*p,event,e);},
      [weak](auto&,bool& out,auto& e){auto p=weak.lock();NativeCharacterMenuV4* n{};if(!p||!p->directory->resolve(n,e))return false;out=true;return true;},e))return false;
   }
   if(!native->main){e="Required actual Init23 main2 font preload";return false;}
   for(unsigned i=1;i<=6;++i){const auto path="_root.preload_font.preload_font"+std::to_string(i);
    for(const char* codes:{"0123456789","ABCDEFGHIJKLMNOPQRSTUVWXYZ","abcdefghijklmnopqrstuvwxyz"})
     if(!native->main->source_preload_glyphs_v119(path.c_str(),codes,e))return false;
   }break;}
  case 24:{
   auto front=native_main_menu_front_v114;if(!front){e="Required actual Init24 selected Hide provider";return false;}
   //Re-read the live end after callbacks, as original24 does.
   for(std::uint32_t i=0;i<native->shared_stack_v27->registry_count_v58();++i){
    auto* menu=native->shared_stack_v27->registry_at_v58(i);if(!menu){e="Invalid live MenuManager registry";return false;}
    if(!register_drag(menu->identity,e)||!front->hide(*menu,e))return false;
   }break;}
  case 25:
   //The native reconstruction has one global facade per original player.
   //Register the shipping names against each SAME already-owned AS provider.
   for(unsigned slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!provider->directory->movie_slot(slot,q,e))return false;if(!q.identity)continue;
    dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;if(!provider->directory->movie(slot,q,movie,pin,e)||!movie->source_register_native_actions_v119(process_menu_actions_v119,e))return false;
   }break;
  case 26:{
   auto events=application->events14();if(!events){e="Required actual App14 EventManager";return false;}
   dh2::events::EventReceiverV12 receiver;receiver.identity=reinterpret_cast<std::uintptr_t>(native->shared_stack_v27.get());
   receiver.context=this;receiver.lifetime=shared_from_this();receiver.on_event=[](void* raw,const auto& event,auto&,std::int32_t& result,auto& e){
    auto& self=*static_cast<NativeProcessStartupV119*>(raw);result=0;if(!self.current(e))return false;
    auto n=authored_character_menu;if(!n||!n->shared_stack_v27){e="Missing actual App input MenuManager";return false;}
    return source_menu_application_event_v119(self.app.lock(),*n,event,result,e);
   };
   constexpr std::int32_t types[]{5,4,7};for(;attach_index<3;++attach_index){bool inserted{};if(!events->attach(types[attach_index],receiver,10,inserted,e))return false;}
   complete=true;e.clear();return true;}
  default:
   if(menu_stage_bc>26){e="MenuManager.Init source stage outside original domain";return false;}
   break; //Captured default branches increment the counter only.
  }
  ++menu_stage_bc;e.clear();return true;
 }
};
std::shared_ptr<NativeProcessStartupV119> native_process_startup_v119;
bool begin_native_process_startup_v119(std::string& e){
 if(native_process_startup_v119)return native_process_startup_v119->current(e);
 if(!application_services_v5||!gameplay_assets||surface_width<=1||surface_height<=1||front_directory.empty()){e="Required actual process App/assets/resized surface/private directory";return false;}
 auto owner=std::make_shared<NativeProcessStartupV119>();owner->app=application_services_v5;owner->directory=front_directory;
 dh2::android_ui::GSInitStageServicesV100 stages;stages.owner=std::make_shared<std::uint8_t>(0);
 if(!bind_process_text_startup_v101(stages,e)||!front_ui.prepare_process_settings_v105(application_services_v5,front_directory,
   dh2::android_ui::platform_language_v119,e)||!front_ui.prepare_process_front_v119(front_directory,surface_width,surface_height,e))return false;
 std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61> files;
 if(!dh2::application::ApplicationSaveFilesOwnerV61::acquire(application_services_v5,front_directory,files,e))return false;
 dh2::android_ui::ProcessTrophyServicesV100 trophy;
 if(!original_ui.process_trophy_services_v119(application_services_v5,trophy,e))return false;
 std::weak_ptr<NativeProcessStartupV119> weak=owner;
 stages.device_time=[](auto& out,auto& e){out=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());e.clear();return true;};
 stages.audio_create=[](auto& e){dh2::audio::AudioApplicationBorrowV42 captured;return dh2::android_audio::borrow_application_audio_v42(captured,e);};
 stages.start_movie=dh2::android_ui::start_android_intro_movie_v119;
 stages.movie_finished=dh2::android_ui::intro_finished_v119;
 stages.load_splash_texture=[](auto language,auto& e){return front_ui.load_process_splash_v119(language,e);};
 stages.menu_init=[weak](auto& done,auto& e){auto p=weak.lock();if(!p){e="Expired actual GSInit MenuManager provider";return false;}return p->menu_init(done,e);};
 stages.reset_startup_clocks=[weak](auto& e){auto p=weak.lock();if(!p||!p->current(e))return false;auto app=p->app.lock();
  const auto now=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
  auto random=app->source_random_v62();random->channel(0).seed=now;random->channel(1).seed=0;
  application_frame_time_v93=now;application_frame_time_started_v93=true;
  app->source_loading_v55().dt8c=0;app->source_loading_v55().native_dt_produced_v93=true;e.clear();return true;
 };
 stages.enter_main_state=[weak](auto& e){auto p=weak.lock();if(!p||!p->current(e))return false;auto n=authored_character_menu;
  if(!n||!n->shared_stack_v27){e="Required real initialized process MenuManager";return false;}
  const auto manager=reinterpret_cast<std::uintptr_t>(n->shared_stack_v27.get());
  if(!reset_source_main_menu_manager_v114(manager,e))return false;
  std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings;if(!front_ui.gameplay_settings_borrow_v67(settings,e)||!settings)return false;
  std::uintptr_t menu{};if(!get_source_main_menu_v114(manager,settings->new_settings()?"menu_language":"menu_splash",menu,e))return false;
  dh2::loader::MainMenuStateBorrowV114 state;if(!front_ui.borrow_main_menu_state_v114(state,e)||!state.menu_c||!state.source29)return false;
  *state.menu_c=menu; //Original global first-menu request, consumed by GSFlash Update.
  if(!front_ui.enter_main_menu_state_v114(state,e)||!front_ui.bind_profile_application_v114(p->app.lock(),e))return false;
  //Android orientation endpoint31f748 is literal BXLR. Online CX/GC/DRM
  //operations are excluded by the explicit offline reconstructed platform.
  return true;
 };
 owner->source=std::make_unique<dh2::android_ui::SourceGSInitProcessOwnersV100>(application_services_v5,std::move(stages),std::move(trophy));
 native_process_startup_v119=owner;e.clear();return true;
}
bool tick_native_process_startup_v119(std::string& e){
 auto p=native_process_startup_v119;if(!p||!p->source){e="Required genuine first-boot GSInit driver";return false;}
 if(!p->current(e))return false;if(p->source->complete()){e.clear();return true;}
 const bool ok=p->source->update(e);if(!ok)p->first_error=e;
 return ok;
}
bool native_process_ready_v119(){return native_process_startup_v119&&native_process_startup_v119->source&&native_process_startup_v119->source->complete();}
}
bool model_renderer::native_process_menu_base_hide_v119(std::uintptr_t id,std::string& e){
 auto p=native_process_startup_v119;if(!p){e="Required retained process MenuBase lifecycle owner";return false;}return p->base_hide(id,e);
}
bool model_renderer::native_process_derived_menu_show_v119(std::uintptr_t id,std::uintptr_t render,std::string& e){
 auto p=native_process_startup_v119;auto n=authored_character_menu;
 if(!p||!n||!n->shared_stack_v27||!p->current(e)){if(e.empty())e="Required retained process derived Show owners";return false;}
 std::shared_ptr<NativeMenuSingletonV67> owner;
 for(const auto& candidate:source_singleton_v67)if(candidate&&candidate->fields.identity==id){owner=candidate;break;}
 auto* menu=n->shared_stack_v27->registered_menu_v27(id);
 if(!owner||!owner->constructed||!menu||!menu->render||menu->render->identity!=render||owner->fields.render!=render){
  e="Derived Show differs from SAME constructed MenuBase/RenderFX";return false;
 }
 //The selected Show virtual must remain distinct from qualified MenuBase.Show.
 //EnterName42aef0 consists solely of its validity gate and the qualified call.
 if(owner->kind==MenuSingletonKindV67::enter_name){
  if(!owner->fields.valid7c){e.clear();return true;}return p->base_show(id,e);
 }
 if(owner->kind==MenuSingletonKindV67::select_class){
  // IDA MenuCharacterSelect::Show 0x428f38 starts by calling
  // MenuMainMenu::DestroyScene and DestroyAvatarCamera, transfers
  // mSavegameSlot into this+0x160, marks the preview destroyed and clears
 // MainMenu's slot before its IsValidMenu gate and qualified MenuBase.Show.
  auto app=p->app.lock();std::int32_t saved_slot=-1;
  if(!app||!model_renderer::native_menu_preview_select_show_v121(app,saved_slot,e))return false;
  owner->select160=saved_slot;
  if(!owner->fields.valid7c){e.clear();return true;}
  if(!p->base_show(id,e))return false;
  // Preserve the Front owner’s already authored show animation/OnShow from
  // MenuStack::Push; this adds the source class title/arrows/CurrentClass and
  // the actual class_select RenderFX display callback to that SAME movie.
  return front_ui.process_class_select_show_v87(render,e);
 }
 if(owner->kind!=MenuSingletonKindV67::main){e="Required actual selected Show override for "+owner->fields.name;return false;}
 if(!owner->fields.valid7c){e.clear();return true;}
 //42c62c: GameCenter operations are excluded by the existing offline platform
 //policy. The actual menu_bg virtual Show precedes qualified MenuBase.Show.
 auto* background=n->shared_stack_v27->menu("menu_bg");
 if(!background||!n->shared_roster_v27->receiver_fields_v59(background->identity)){
  e="Original Main.Show requires actual generated menu_bg Show receiver";return false;
 }
 dh2::ui::MenuStackRequestV1 show{};show.operation=dh2::ui::MenuStackOperationV1::menu_show;show.menu=background;show.render=background->render;
 bool handled{};if(!n->shared_roster_v27->route(*n->shared_stack_v27->view(),show,handled,e)||!handled)return false;
 if(!p->base_show(id,e))return false;
 std::int32_t trace{};
 if(!n->debug(nullptr,trace,e)||!n->debug("isTracingMenuMainMenu",trace,e)||!n->debug(nullptr,trace,e)||!n->debug("MenuMainMenu",trace,e))return false;
 if(trace){e="Required reached Main.Show DBG_TraceContextInformation provider";return false;}
 //SetAlpha7aa058 resets the actual cxform to identity, then alpha multiplier0.
 //RenderFX.Find absence is a source no-op rather than a fabricated receiver.
 if(!p->scoped(owner->fields,[&](auto& graph,auto& error){
  dh2::ui::SwfAsValue context;bool live{};if(!owner->state.weak_character_v59.borrow(graph,context,live,error)||!live){if(error.empty())error="Main.Show lost SAME weak context";return false;}
  dh2::ui::AuthoredMenuSearchIndexV1 index;if(!index.initialize(graph,context,error))return false;
  for(const char* name:{"btn_Debug1","btn_debug2"}){
   dh2::ui::SwfAsValue value;bool found{};if(!index.find(name,value,found,error))return false;
   if(!found){if(!graph.find_target(context,name,value,error))return false;found=value.kind()==dh2::ui::SwfAsValue::Kind::object;}
   if(!found)continue;gameswf::as_object* object{};
   if(!graph.borrow_object(value,object,error)||!object||!object->is(gameswf::character::m_class_id)){if(error.empty())error="Main.Show alpha receiver is not actual character";return false;}
   gameswf::cxform color;color.m_[3][0]=0.f;static_cast<gameswf::character*>(object)->set_cxform(color);
  }return true;
 },e)||!model_renderer::native_menu_preview_prepare_main_v121(p->app.lock(),e))return false;
 //Main.Show binds its native RenderCharacterPane callback to the actual
 //menu_bg.RenderedBG child in this retained movie. The ARM32 implementation
 //is a return-only callback, so preserve that behavior while using the movie's
 //real display-hook lifecycle and the same registered MainMenu RenderFX.
 if(!p->scoped_movie(owner->fields,[&](auto& movie,auto& graph,auto& error){
  dh2::ui::SwfAsValue root,rendered_bg;if(!graph.root_value(root,error)||!graph.find_target(root,"menu_bg.RenderedBG",rendered_bg,error)||!rendered_bg.identity()){
   if(error.empty())error="Required actual Main.Show menu_bg.RenderedBG character";return false;
  }
  gameswf::as_object* target{};if(!graph.borrow_object(rendered_bg,target,error)||!target||!target->is(gameswf::character::m_class_id)){
   if(error.empty())error="Main.Show RenderedBG is not the actual display character";return false;
  }
  auto& callback=p->main_rendered_bg_callbacks_v119[owner->fields.render];
  callback.owner=p;callback.render=owner->fields.render;callback.character=rendered_bg.identity();
  return movie.menu_display_callback("_root.menu_bg.RenderedBG",&callback,NativeProcessStartupV119::main_rendered_bg_display_v119,error);
 },e))return false;
 // IDA byte_99F7D7 is set at the successful tail of MenuMainMenu::Show and
 // gates only slot-2 input. Keep it separate from the animation preference.
 source_main_menu_input_enabled_v120=true;
 e.clear();return true;
}
bool model_renderer::native_process_derived_menu_focus_v119(std::uintptr_t id,std::uintptr_t render,std::string& e){
 auto p=native_process_startup_v119;auto n=authored_character_menu;
 if(!p||!n||!n->shared_stack_v27||!p->current(e)){if(e.empty())e="Required retained process derived focus owners";return false;}
 std::shared_ptr<NativeMenuSingletonV67> owner;
 for(const auto& candidate:source_singleton_v67)if(candidate&&candidate->fields.identity==id){owner=candidate;break;}
 auto* menu=n->shared_stack_v27->registered_menu_v27(id);
 if(!owner||!owner->constructed||!menu||menu->identity!=id||!menu->name||!menu->render||
    menu->render->identity!=render||owner->fields.render!=render){
  e="Derived GotFocus differs from SAME registered constructed process menu/RenderFX";return false;
 }
 const bool enter_name=owner->kind==MenuSingletonKindV67::enter_name&&
  !std::strcmp(menu->name,"menu_EnterName")&&owner->fields.name=="menu_EnterName";
 const bool select_class=owner->kind==MenuSingletonKindV67::select_class&&
  !std::strcmp(menu->name,"menu_SelectClass")&&owner->fields.name=="menu_SelectClass";
 if(enter_name||select_class){
  // Both vtables inherit MenuBase::GotFocus (0x41b3e8), a literal bx lr.
  e.clear();return true;
 }
 auto* globals=n->shared_stack_v27->view()?n->shared_stack_v27->view()->globals:nullptr;
 if(owner->kind!=MenuSingletonKindV67::main||std::strcmp(menu->name,"menu_MainMenu")||
    owner->fields.name!="menu_MainMenu"||!globals){
  e="Derived GotFocus differs from SAME registered constructed MenuMainMenu/MenuManager";return false;
 }
 //Original ARM32 MenuMainMenu::GotFocus (0x42ba98) stores lastOpenMenuID=1
 //and clears Is_In_Multiplayer_IGM and Is_In_Multiplayer_Mode. Its target
 //process globals are the ones retained by this same MenuManager adapter.
 globals->last_open_menu=1;globals->multiplayer_igm=0;globals->multiplayer=0;
 e.clear();return true;
}
bool model_renderer::native_process_derived_menu_blur_v119(std::uintptr_t id,std::uintptr_t render,std::string& e){
 auto p=native_process_startup_v119;auto n=authored_character_menu;
 if(!p||!n||!n->shared_stack_v27||!p->current(e)){if(e.empty())e="Required retained process derived LostFocus owners";return false;}
 std::shared_ptr<NativeMenuSingletonV67> owner;
 for(const auto& candidate:source_singleton_v67)if(candidate&&candidate->fields.identity==id){owner=candidate;break;}
 auto* menu=n->shared_stack_v27->registered_menu_v27(id);
 if(!owner||!owner->constructed||!menu||menu->identity!=id||!menu->name||!menu->render||
    menu->render->identity!=render||owner->fields.render!=render){
  e="Derived LostFocus differs from SAME registered constructed process menu/RenderFX";return false;
 }
 const bool enter_name=owner->kind==MenuSingletonKindV67::enter_name&&
  !std::strcmp(menu->name,"menu_EnterName")&&owner->fields.name=="menu_EnterName";
 const bool select_class=owner->kind==MenuSingletonKindV67::select_class&&
  !std::strcmp(menu->name,"menu_SelectClass")&&owner->fields.name=="menu_SelectClass";
 if(!enter_name&&!select_class){e="Required actual selected LostFocus override for "+owner->fields.name;return false;}
 // Both menu vtables inherit MenuBase::LostFocus (0x41b3ec), a literal bx lr.
 e.clear();return true;
}
bool model_renderer::native_process_derived_menu_callback_v119(std::uintptr_t id,std::uintptr_t render,const char* method,std::string& e){
 auto p=native_process_startup_v119;auto n=authored_character_menu;
 if(!p||!n||!method||!n->shared_stack_v27){e="Required retained process derived MenuBase callback owners";return false;}
 dh2::ui::AuthoredMenuFieldsV1* f{};dh2::ui::AuthoredCharacterStateV1* state{};
 if(!p->fields(id,f,state,e))return false;
 auto* menu=n->shared_stack_v27->registered_menu_v27(id);
 if(!menu||!menu->render||menu->render->identity!=render||f->render!=render){e="Derived MenuBase callback differs from SAME registered RenderFX";return false;}
 //MultiMenu invokes OnShow/OnHide on the registered weak character even
 //when its native receiver is a derived singleton rather than roster MenuBase.
 //Missing AS methods retain InvokeASCallback's source success semantics.
 const dh2::ui::AuthoredMenuRequestV1 request{dh2::ui::AuthoredMenuOperationV1::invoke_as,method,0};
 return p->scoped(*f,[&](auto& graph,auto& error){
  return dh2::ui::authored_character_movie_operation_v1(graph,*state,request,n->bootstrap_services_v27.localization,error);
 },e);
}
bool model_renderer::native_process_derived_menu_animation_v119(std::uintptr_t id,std::uintptr_t render,
 std::uintptr_t character,const char* label,bool& accepted,std::string& e){
 accepted=false;auto p=native_process_startup_v119;auto n=authored_character_menu;
 if(!p||!n||!label||!n->shared_stack_v27){e="Required retained process derived animation owners/label";return false;}
 dh2::ui::AuthoredMenuFieldsV1* f{};dh2::ui::AuthoredCharacterStateV1* state{};
 if(!p->fields(id,f,state,e))return false;
 auto* menu=n->shared_stack_v27->registered_menu_v27(id);
 if(!menu||!menu->render||menu->render->identity!=render||f->render!=render){e="Derived animation differs from SAME registered RenderFX";return false;}
 //MultiMenu's PlayAnimation uses its supplied weak character, returning false
 //for null/non-sprite receivers or absent labels. Derived native receivers use
 //the same authored sprite operation as discovered MenuBase roster entries.
 if(!character){e.clear();return true;}
 if(!menu->character||menu->character->identity!=character){e="Derived animation differs from SAME stack character";return false;}
 return p->scoped(*f,[&](auto& graph,auto& error){
  dh2::ui::SwfAsValue value;bool live{};
  if(!state->weak_character_v59.borrow(graph,value,live,error))return false;
  if(!live||value.identity()!=character){error="Derived animation lost SAME registered weak character";return false;}
  return dh2::ui::authored_menu_play_animation_v4(graph,value,label,accepted,error);
 },e);
}
