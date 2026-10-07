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
 std::map<std::uintptr_t,std::unique_ptr<dh2::ui::AuthoredMenuDragV68>> drag_owners;
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
  auto p=source_postmovie_v62.lock();if(!p||!current(e))return false;
  for(unsigned slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!p->directory->movie_slot(slot,q,e))return false;if(q.identity!=f.render)continue;
   dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;if(!p->directory->movie(slot,q,movie,pin,e))return false;
   struct Invoke{const std::function<bool(dh2::ui::SwfAsGraph&,std::string&)>& fn;
    static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){return static_cast<Invoke*>(raw)->fn(graph,e);}} invoke{fn};
   return movie->menu_action_script(&invoke,Invoke::run,e);
  }e="Derived MenuBase lost its actual RenderFX resource";return false;
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
    if(!native->shared_roster_v27->post_load(*movie,std::move(pin),render->flags,e))return false;
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
