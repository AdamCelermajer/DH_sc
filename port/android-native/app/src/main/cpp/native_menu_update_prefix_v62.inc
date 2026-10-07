// Include from native_app after native_character_menu_v4.inc and before
// native_gslevel_menu_v27.inc, with renderer_native_menu_prefix_v62.hpp and
// application_player_manager_bootstrap_v59.hpp/application_save_files_owner_v61.hpp.
// No second MenuManager/HUD/actor/profile is created here.
namespace {
const auto source_menu_process_v62=std::make_shared<dh2::ui::MenuManagerProcessV62>();
std::weak_ptr<dh2::ui::MenuDebugHudServicesV62> source_gameplay_debug_services_v67;
struct NativeMenuPrefixProviderV62 {
 std::weak_ptr<void> world;
 std::weak_ptr<dh2::ui::MenuStackOwnerV1> manager;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 bool process_scope_v104{};
 std::function<bool(std::uint32_t,dh2::ui::MenuMovieBorrowV58&,std::string&)> movie_slot;
 bool resolve(NativeCharacterMenuV4*& out,std::string& e)const{
  out=nullptr;auto actual_world=world.lock();auto actual_manager=manager.lock();
  const auto app=application.lock();
  const bool process=process_scope_v104&&authored_character_menu&&app&&
   authored_character_menu->process_directory_ready_v104&&authored_character_menu->process_application_v104.lock()==app;
  if(!actual_manager||!authored_character_menu||authored_character_menu->shared_stack_v27!=actual_manager||
     (!process&&(!actual_world||authored_character_menu->leases->world!=actual_world||!authored_character_menu->bootstrap_complete_v27))){
   e="Menu prefix provider no longer belongs to SAME actual World/manager";return false;
  }
  out=authored_character_menu.get();e.clear();return true;
 }
 std::shared_ptr<void> current_world()const{
  return process_scope_v104&&authored_character_menu?authored_character_menu->leases->world:world.lock();
 }
 bool movie(std::uint32_t slot,const dh2::ui::MenuMovieBorrowV58& source,
  dh2::ui::SwfMovie*& out,std::shared_ptr<void>& lease,std::string& e)const{
  out=nullptr;lease.reset();NativeCharacterMenuV4* native{};if(!resolve(native,e))return false;
  if(!source.identity||!source.actual_owner){e="Required real positive MenuFX slot before typed facade borrow";return false;}
  if(slot==0){if(!front_ui.base_movie_borrow_v4(out,lease,e))return false;}
  else if(slot==2){if(!front_ui.main_movie_borrow_v4(out,lease,e))return false;}
  else if(slot==3){if(!original_ui.hud_movie_borrow_v4(out,lease,e))return false;}
  else if(slot==1){auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
   if(!panel||!panel->source_movie_v91()||panel->source_render_identity_v91()!=source.identity){e="Required SAME source character-menu movie slot";return false;}
   out=panel->source_movie_v91();lease=source.actual_owner;return true;
  }else{e="Invalid original MenuManager movie slot";return false;}
  if(!out||source.identity!=reinterpret_cast<std::uintptr_t>(out)){e="Actual MenuFX slot/facade identity differs";return false;}
  return true;
 }
 bool debug_load(std::string& e)const{NativeCharacterMenuV4* native{};if(!resolve(native,e))return false;std::int32_t ignored{};return native->debug(nullptr,ignored,e);}
 bool debug_switch(const char* key,bool& out,std::string& e)const{NativeCharacterMenuV4* native{};if(!resolve(native,e))return false;std::int32_t value{};if(!native->debug(key,value,e))return false;out=value!=0;return true;}
 bool register_debug(dh2::ui::MenuDebugHudOwnerV62& debug,std::string& e){
  return register_base(debug.fields(),debug.state(),debug.projection(),e);
 }
 bool register_base(dh2::ui::AuthoredMenuFieldsV1& fields,dh2::ui::AuthoredCharacterStateV1& state,dh2::ui::MenuStackCharacterV1& projection,std::string& e){
  using namespace dh2::ui;NativeCharacterMenuV4* native{};if(!resolve(native,e))return false;
  auto stack=native->shared_stack_v27;
  if(!movie_slot){e="Required actual slots0..3 for DebugHUD.RegisterMenu";return false;}
  for(std::uint32_t slot=0;slot<4;++slot){
   MenuMovieBorrowV58 source;if(!movie_slot(slot,source,e))return false;if(!source.identity)continue;
   SwfMovie* actual{};std::shared_ptr<void> lease;if(!movie(slot,source,actual,lease,e))return false;
   struct Base {AuthoredMenuFieldsV1& f;AuthoredCharacterStateV1& s;MenuStackCharacterV1& p;auto& fields(){return f;}auto& state(){return s;}auto& projection(){return p;}} base{fields,state,projection};
   struct Registration {Base& debug;MenuStackOwnerV1& stack;SwfMovie& movie;std::shared_ptr<void> lease;std::uintptr_t render;bool found{};
    static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Registration*>(raw);SwfAsValue root,value;
     if(!graph.root_value(root,e))return false;AuthoredMenuSearchIndexV1 index;if(!index.initialize(graph,root,e)||!index.find(c.debug.fields().name,value,c.found,e))return false;
     if(!c.found)return true;auto* render=c.stack.render(c.render);if(!render){e="DebugHUD registration lacks SAME actual render directory";return false;}
     AuthoredCharacterRegistrationServicesV1 services;services.owner=c.lease;services.render=c.render;services.source_reassignment_v67=true;
     services.append_state=[&c](auto& f,auto& e){return c.stack.register_menu_live_v27(f.identity,f.render,f.name,&c.debug.projection(),0,0,e);};
     services.find=[&index](const char* name,auto& out,bool& found,auto& e){return index.find(name,out,found,e);};
     services.bind_weak_context=[&c,render,&graph](auto&,const auto& value,auto& e){gameswf::as_object* object{};return graph.borrow_object(value,object,e)&&authored_menu_stack_character_v4(object,render->flags,{},c.debug.projection(),e);};
     services.create=[](auto&,auto&){return true;}; // inherited source MenuBase.Create literal leaf
     if(!authored_character_register_state_v1(graph,c.debug.state(),nullptr,services,e))return false;
     auto* projected=c.stack.registered_menu_v27(c.debug.fields().identity);if(!projected){e="DebugHUD RegisterMenu publication vanished";return false;}
     projected->valid_menu=c.debug.fields().valid7c;return true;
    }} call{base,*stack,*actual,std::move(lease),source.identity};
   if(!actual->menu_action_script(&call,Registration::run,e))return false;if(call.found)return true;
  }
  // Source RegisterMenu with no Find result delivers the actual trace query
  // and leaves render4/valid7c NULL/zero. This is an observed graph absence.
  bool ignored{};return debug_load(e)&&debug_switch("isTracingMenuManager",ignored,e);
 }
 bool find_base(dh2::ui::AuthoredMenuFieldsV1& fields,dh2::ui::AuthoredCharacterStateV1& state,const char* path,dh2::ui::AuthoredMenuWeakCharacterV59& out,std::string& e){
  using namespace dh2::ui;
  //FindFrom7a8a84 accepts NULL render/root and returns actual absence.
  if(!fields.render){out.reset();e.clear();return true;}
  for(std::uint32_t slot=0;slot<4;++slot){MenuMovieBorrowV58 source;if(!movie_slot||!movie_slot(slot,source,e))return false;
   if(source.identity!=fields.render)continue;SwfMovie* actual{};std::shared_ptr<void> held;if(!movie(slot,source,actual,held,e))return false;
   struct Find{AuthoredCharacterStateV1& state;const char* path;AuthoredMenuWeakCharacterV59& out;
    static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Find*>(raw);SwfAsValue root,value;bool live{},found{};
     if(!q.state.weak_character_v59.borrow(graph,root,live,e))return false;
     if(!live)return q.out.source_assign_null_v68(graph,e);
     AuthoredMenuSearchIndexV1 index;if(!index.initialize(graph,root,e)||!index.find(q.path,value,found,e))return false;
     if(found)return q.out.source_assign_v67(graph,value,e);return q.out.source_assign_null_v68(graph,e);
    }} find{state,path,out};return actual->menu_action_script(&find,Find::run,e);
  }
  e="Map Init lost its actual registered movie generation";return false;
 }
 bool refresh_debug(dh2::ui::MenuDebugHudOwnerV62& debug,dh2::ui::DebugCachedCharacterV62& cache,const char* path,std::string& e){
  using namespace dh2::ui;
  for(std::uint32_t slot=0;slot<4;++slot){MenuMovieBorrowV58 source;if(!movie_slot||!movie_slot(slot,source,e))return false;
   if(source.identity!=debug.fields().render)continue;SwfMovie* actual{};std::shared_ptr<void> lease;if(!movie(slot,source,actual,lease,e))return false;
   struct Refresh {MenuDebugHudOwnerV62& debug;DebugCachedCharacterV62& cache;const char* path;
    static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& c=*static_cast<Refresh*>(raw);SwfAsValue root,value;bool live{},found{};
     if(!c.debug.state().weak_character_v59.borrow(graph,root,live,e))return false;
     if(!live&&!graph.root_value(root,e))return false;AuthoredMenuSearchIndexV1 index;
     if(!index.initialize(graph,root,e)||!index.find(c.path,value,found,e))return false;
     if(found){if(!c.cache.weak28.source_assign_v67(graph,value,e))return false;}else if(!c.cache.weak28.source_assign_null_v68(graph,e))return false;
     c.cache.path8=c.path;c.cache.render20=c.debug.fields().render;c.cache.root24=live?root.identity():0;return true;
    }} call{debug,cache,path};return actual->menu_action_script(&call,Refresh::run,e);
  }
  e="DebugHUD RefreshCache lost its actual registered movie";return false;
 }
 bool load_debug_flags(std::array<std::uint8_t,22>& out,bool& written,std::string& e)const{
  written=false;auto app=application.lock();if(!app||!app->source_save_files_v61()){e="Required SAME App FileManager for original DebugStats.savegame";return false;}
  bool found{};std::vector<std::uint8_t> bytes;
  if(!app->source_save_files_v61()->read_save("DebugStats.savegame",found,bytes,e))return false;
  if(!found){e.clear();return true;} // actual openSavefile NULL branch
  if(bytes.size()<4){e="Original DebugStats int read assertion required";return false;}
  const auto count=std::uint32_t(bytes[0])|(std::uint32_t(bytes[1])<<8)|(std::uint32_t(bytes[2])<<16)|(std::uint32_t(bytes[3])<<24);
  if(count!=22){e.clear();return true;} // original closes stream without stores
  for(std::size_t i=0;i<out.size();++i){if(i+4>=bytes.size()){e="Original DebugStats bool read assertion required after "+std::to_string(i)+" stores";return false;}out[i]=bytes[i+4];}
  written=true;e.clear();return true;
 }
};
}
dh2::ui::AuthoredMenuFieldsV1* model_renderer::native_debug_menu_fields_v62(std::uintptr_t id)noexcept{
 auto* debug=source_menu_process_v62->existing_debug_hud();return debug&&debug->fields().identity==id?&debug->fields():nullptr;
}
bool model_renderer::bind_native_menu_prefix_v62(std::shared_ptr<void> world,
 dh2::ui::MenuManagerUpdateServicesV58& update,std::string& e){
 using namespace dh2::ui;
 std::shared_ptr<void> manager;if(!borrow_native_menu_manager_v58(world,manager,e))return false;
 if(update.source_prefix||!update.actual_manager||manager.get()!=update.actual_manager.get()||
    manager.owner_before(update.actual_manager)||update.actual_manager.owner_before(manager)){
  e="Source menu prefix requires unbound SAME actual manager";return false;
 }
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app;if(!borrow_actual_application_services_v5(app,e))return false;
 if(!app->source_player_manager_v59()||!app->source_player_manager_v59()->belongs_to_application(app)){
  e="Required actual pre-Gear App PlayerManager before MenuManager.Update";return false;
 }
 auto provider=std::make_shared<NativeMenuPrefixProviderV62>();provider->world=world;provider->manager=authored_character_menu->shared_stack_v27;
 provider->process_scope_v104=authored_character_menu->process_directory_ready_v104;
 provider->application=app;provider->movie_slot=update.movie_slot;
 MenuManagerPrefixServicesV62 services;services.actual_manager=manager;services.process=source_menu_process_v62;
 services.current_level=[](auto& out,auto& e){dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;
  out={};if(current){out.actual_owner=current.level();out.identity=current.identity();out.byte198=&current.level()->constructor_fields_v3().byte198;}return true;};
 services.local_player_character=[provider](auto& out,auto& e){auto app=provider->application.lock();NativeCharacterMenuV4* menu{};out=0;
  if(!provider->resolve(menu,e)||!app||!app->source_player_manager_v59()){if(e.empty())e="Required SAME App PM";return false;}
  dh2::player::PlayerInfoFieldsV1* info{};if(!app->source_player_manager_v59()->get_local_player(0,true,info,e)||!info)return false;out=info->character660;return true;};
 services.character=[provider](auto id,auto& out,auto& e){out={};out.character=id;auto world=provider->current_world();return borrow_menu_character_ooi_type_v62(world,id,out.receiver,out.signed_ooi_type14a8,e);};
 services.action_cache=[](auto& out,auto& e){out={};return original_ui.source_action_icon_cell_v62(out.cache108,out.receiver,e);};
 services.hud_root=[provider](auto& out,auto& e){out={};MenuMovieBorrowV58 hud;if(!provider->movie_slot||!provider->movie_slot(3,hud,e))return false;
  if(!hud.identity)return true;SwfMovie* movie{};std::shared_ptr<void> held;if(!provider->movie(3,hud,movie,held,e))return false;
  if(!original_ui.authored_action_cache_v62(out.facade,out.receiver,e))return false;std::shared_ptr<void> cache;
  return original_ui.source_action_icon_cell_v62(out.cache108,cache,e);};
 services.set_wire_frame=[](bool value,auto& e){return original_ui.renderer_set_wire_frame_v62(value,e);};
 services.debug_load=[provider](auto& e){return provider->debug_load(e);};
 services.debug_switch=[provider](const char* key,bool& value,auto& e){return provider->debug_switch(key,value,e);};
 services.application_dt8c=model_renderer::borrow_application_dt_v93;
 services.listener_fields=[provider](auto*& out,auto& e){NativeCharacterMenuV4* native{};if(!provider->resolve(native,e))return false;out=&native->fields;return true;};
 services.fill_leaderboard=[](auto& e){e="Required genuine Matching leaderboard671c + authored list callback for fillLeaderBoard441dcc";return false;};
 services.debug_hud.actual_manager=manager;services.debug_hud.debug_load=services.debug_load;services.debug_hud.debug_switch=services.debug_switch;
 services.debug_hud.register_menu=[provider](auto& debug,auto& e){return provider->register_debug(debug,e);};
 services.debug_hud.refresh_cache=[provider](auto& debug,auto& cache,const char* key,auto& e){return provider->refresh_debug(debug,cache,key,e);};
 services.debug_hud.load_flags=[provider](auto& flags,bool& written,auto& e){return provider->load_debug_flags(flags,written,e);};
 auto gameplay_debug=std::make_shared<MenuDebugHudServicesV62>(services.debug_hud);
 source_gameplay_debug_services_v67=gameplay_debug;
 auto prefix=std::make_shared<MenuManagerPrefixV62>(std::move(services));
 const auto visible=update.menu_is_visible;const auto selected_update=update.menu_virtual1c;
 update.menu_is_visible=[visible](auto id,bool& out,auto& e){if(auto* f=native_debug_menu_fields_v62(id)){out=f->visible74!=0;e.clear();return true;}return visible&&visible(id,out,e);};
 update.menu_virtual1c=[selected_update](auto id,auto& e){if(native_debug_menu_fields_v62(id)){auto* debug=source_menu_process_v62->existing_debug_hud();return debug&&debug->update(e);}return selected_update&&selected_update(id,e);};
 update.source_prefix=[prefix,gameplay_debug](bool& early,std::int32_t& dt,auto& e){return prefix->update(early,dt,e);};e.clear();return true;
}
bool model_renderer::prepare_actual_gameplay_debug_v67(const std::shared_ptr<void>& world,std::string& e){
 if(!world||!authored_character_menu||authored_character_menu->leases->world!=world){e="Required SAME current gameplay/menu World";return false;}
 auto services=source_gameplay_debug_services_v67.lock();
 if(!services){e="Required actual retained DebugHUD GetInstance services";return false;}
 dh2::ui::MenuDebugHudOwnerV62* debug{};
 return source_menu_process_v62->debug_hud(*services,debug,e)&&debug&&debug->prepare_level_frame_v67(e);
}
