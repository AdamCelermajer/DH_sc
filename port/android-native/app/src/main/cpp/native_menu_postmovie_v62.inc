#include "gameswf/gameswf_character.h"
#include "renderer_native_menu_load_v98.hpp"
namespace {
std::unique_ptr<dh2::ui::MenuMapOwnerV62> source_map_menu_v62;
struct NativeMenuPostMovieV62 {
 std::shared_ptr<NativeMenuPrefixProviderV62> directory;
 std::shared_ptr<void> world_transport;
 model_renderer::MenuHudWorldOperationV62 world_operations;
 bool world_bound{};
 model_renderer::NativePrimary1LoadServicesV98 loadmenu1_services_v98;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> loadmenu1_level_v98;
 bool loadmenu1_attempted_v98{};std::string loadmenu1_failure_v98;
 bool loadmenu3_attempted{};std::string loadmenu3_failure;
 dh2::ui::HudControlsServicesV62 canonical_controls;
 dh2::ui::HudControlsServicesV62 controls(){
  auto result=canonical_controls;auto& p=result.prefix;p.context=this;
  p.input_blocked30=[](void* raw,auto& out,auto& e){auto& self=*static_cast<NativeMenuPostMovieV62*>(raw);auto app=self.directory->application.lock();
   if(!app||!app->source_script_manager_v52()){e="Required SAME process ScriptManager30";return false;}out=app->source_script_manager_v52()->fields().byte30;e.clear();return true;};
  p.store_controller_blocked=[](void*,auto value,auto& e){return model_renderer::source_store_menu_cut_screen_v62(value,e);};
  p.current_level=[](void*,auto& id,auto*& byte,auto& e){dh2::loader::CanonicalCurrentLevelBorrowV1 current;
   if(!model_renderer::borrow_current_native_level_v27(current,e))return false;id=current.identity();byte=current?&current.level()->constructor_fields_v3().byte198:nullptr;return true;};
  p.init_cached_chars=[](void* raw,auto& e){auto& p=static_cast<NativeMenuPostMovieV62*>(raw)->canonical_controls.prefix;return p.init_cached_chars&&p.init_cached_chars(p.context,e);};
  p.local_player=[](void* raw,auto index,auto flag,auto& out,auto& e){auto& p=static_cast<NativeMenuPostMovieV62*>(raw)->canonical_controls.prefix;return p.local_player&&p.local_player(p.context,index,flag,out,e);};
  p.attack=[](void* raw,auto owner,auto target,auto& e){auto& p=static_cast<NativeMenuPostMovieV62*>(raw)->canonical_controls.prefix;return p.attack&&p.attack(p.context,owner,target,e);};
  p.use_ooi=[](void* raw,auto owner,auto target,auto& e){auto& p=static_cast<NativeMenuPostMovieV62*>(raw)->canonical_controls.prefix;return p.use_ooi&&p.use_ooi(p.context,owner,target,e);};
  result.world_map_visible=[directory=directory](bool& out,auto& e){auto world=directory->current_world();return world&&model_renderer::native_worldmap_visible_v68(world,out,e);};
  return result;
 }
 int info(const dh2::ui::HudManagerRequest& q,dh2::ui::HudManagerResponse& out,std::string& e){
  NativeCharacterMenuV4* native{};if(!directory->resolve(native,e))return 0;
  using O=dh2::ui::HudManagerOperation;
  if(q.operation==O::current_level){dh2::loader::CanonicalCurrentLevelBorrowV1 current;
   if(!model_renderer::borrow_current_native_level_v27(current,e))return 0;
   out.identity=current.identity();out.value=current?current.level()->constructor_fields_v3().byte198:0;return 1;}
  if(q.operation==O::elapsed){std::uint32_t dt{};if(!model_renderer::borrow_application_dt_v93(dt,e))return 0;std::memcpy(&out.value,&dt,4);return 1;}
  //InfoHUD/HUDControls initCachedChars run during process MenuManager.Init,
  //before any World. Their saved options belong to Application+4c.
  if(q.operation==O::saved_option){auto app=directory->application.lock();
   auto settings=app?app->source_settings4c_v67():nullptr;
   if(!settings||!q.text){e="Required SAME process Application saved-option receiver";return 0;}
   out.value=settings->saved_option(q.text);e.clear();return 1;}
  if(!world_transport||!world_operations){e="Required SAME canonical InfoHUD operation "+std::to_string(static_cast<unsigned>(q.operation));return 0;}
  return world_operations(q,out,e);
 }
 bool map_instance(std::string& e){
  if(source_map_menu_v62){if(!source_map_menu_v62->constructed()){e=source_map_menu_v62->failure();return false;}return true;}
  NativeCharacterMenuV4* native{};if(!directory->resolve(native,e))return false;
  dh2::ui::MenuMapServicesV62 s;s.actual_manager=native->shared_stack_v27;
  s.debug_load=[self=directory](auto& e){return self->debug_load(e);};
  s.debug_switch=[self=directory](const char* key,bool& out,auto& e){return self->debug_switch(key,out,e);};
  s.register_menu=[self=directory](auto& f,auto& state,auto& p,auto& e){return self->register_base(f,state,p,e);};
  s.find_from=[self=directory](auto& f,auto& state,const char* path,auto& out,auto& e){return self->find_base(f,state,path,out,e);};
  source_map_menu_v62=std::make_unique<dh2::ui::MenuMapOwnerV62>();
  return source_map_menu_v62->construct(source_menu_process_v62->source_fs_commands(),s,e);
 }
 bool clear_map(std::string& e){
  if(!map_instance(e))return false;
  //Positive original AddIcon rows must be registered with a scoped actual
  //movie weak producer. There are no inferred Find-created icon entries.
  return source_map_menu_v62->icons().clear_all({}, {},e);
 }
};
std::weak_ptr<NativeMenuPostMovieV62> source_postmovie_v62;
bool source_hud_menu_event_v68(NativeMenuPostMovieV62& provider,dh2::ui::SwfEvent48& event,std::string& e){
 NativeCharacterMenuV4* native{};if(!provider.directory->resolve(native,e))return false;
 if(event.kind==6){if(!event.name){e="MenuManager.OnEvent requires actual event name";return false;}
  if(std::strstr(event.name,"btn_MENU_CONTINUE")){
   auto* stack=native->shared_stack_v27->view();if(!stack||!stack->globals){e="Required SAME menu source last_open_menu global";return false;}stack->globals->last_open_menu=9;
  }
 }
 auto app=provider.directory->application.lock();
 if(!app||!original_ui.construct_hud_controls_v62(app->events14(),e)||!original_ui.hud_controls_native_event_v68(event,e))return false;
 if(!native->hud){e="MenuManager.OnEvent requires current primary3";return false;}
 auto* render=native->shared_stack_v27->render(reinterpret_cast<std::uintptr_t>(native->hud));
 if(!render){e="MenuManager.OnEvent lost actual HUD directory";return false;}
 if(!render->count)return true; //MenuFX.OnEvent7adf20 literal empty states branch
 if(!render->states||render->count>render->capacity){e="Malformed actual MenuFX active state range";return false;}
 struct Select {dh2::ui::MenuStackRenderV1& render;dh2::ui::SwfEvent48& event;dh2::ui::MenuStackMenuV1* selected{};
  static bool run(void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Select*>(raw);dh2::ui::SwfAsValue value;gameswf::as_object* actual{};
   if(!graph.retain_object(reinterpret_cast<gameswf::as_object*>(q.event.character),value,e)||!graph.borrow_object(value,actual,e)||!actual||!actual->is(gameswf::character::m_class_id)){if(e.empty())e="Required SAME primary3 event character";return false;}
   for(auto i=q.render.count;i>0;--i){auto* state=q.render.states[i-1];if(!state){e="Null actual active MenuFX state";return false;}
    std::vector<gameswf::character*> visited;
    for(auto* cursor=static_cast<gameswf::character*>(actual);cursor;cursor=cursor->get_parent()){
     if(std::find(visited.begin(),visited.end(),cursor)!=visited.end()){e="Cyclic actual event character parent chain";return false;}visited.push_back(cursor);
     if(state->character&&state->character->live&&state->character->identity==reinterpret_cast<std::uintptr_t>(cursor)){q.selected=state;return true;}
    }
   }
   q.selected=q.render.states[q.render.count-1]; //source GetCurrentState miss tail
   return true;
  }} selected{*render,event};
 if(!native->hud->menu_action_script(&selected,Select::run,e))return false;
 if(native->shared_roster_v27->receiver_fields_v59(selected.selected->identity))return native->shared_roster_v27->native_event_v68(selected.selected->identity,event,e);
 e="Required selected derived MenuFX state OnEvent for "+std::string(selected.selected->name?selected.selected->name:"");return false;
}
}
dh2::ui::AuthoredMenuFieldsV1* model_renderer::native_map_menu_fields_v62(std::uintptr_t id)noexcept{
 return source_map_menu_v62&&source_map_menu_v62->fields().identity==id?&source_map_menu_v62->fields():nullptr;
}
bool model_renderer::native_map_menu_update_v62(std::uintptr_t id,std::string& e){
 auto provider=source_postmovie_v62.lock();
 if(!provider||!source_map_menu_v62||!source_map_menu_v62->constructed()||source_map_menu_v62->fields().identity!=id){e="Required actual constructed Map.Update receiver";return false;}
 for(std::uint32_t slot=0;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!provider->directory->movie_slot(slot,q,e))return false;
  if(q.identity!=source_map_menu_v62->fields().render)continue;
  dh2::ui::SwfMovie* movie{};std::shared_ptr<void> lease;if(!provider->directory->movie(slot,q,movie,lease,e))return false;
  return movie->menu_action_script(source_map_menu_v62.get(),[](void* raw,dh2::ui::SwfAsGraph& graph,std::string& e){auto& map=*static_cast<dh2::ui::MenuMapOwnerV62*>(raw);
   return map.state().weak_character_v59.update(graph,map.fields(),e);},e);
 }
 e="Visible Map.Update lost its actual registered movie";return false;
}
bool model_renderer::source_native_map_icon_v62(std::shared_ptr<void> world,std::uint32_t type,dh2::ui::MenuMapIconV62 icon,std::string& e){
 auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};
 if(!provider||!provider->directory->resolve(native,e)||native->leases->world!=world||!provider->map_instance(e))return false;
 return source_map_menu_v62->icons().source_append(type,std::move(icon),e);
}
bool model_renderer::bind_native_menu_postmovie_v62(std::shared_ptr<void> world,
 dh2::ui::MenuManagerUnloadServicesV58& unload,dh2::ui::MenuManagerUpdateServicesV58& update,std::string& e){
 std::shared_ptr<void> manager;if(!borrow_native_menu_manager_v58(world,manager,e))return false;
 const auto same=[&](const auto& q){return q&&q.get()==manager.get()&&!q.owner_before(manager)&&!manager.owner_before(q);};
 if(!same(unload.actual_manager)||!same(update.actual_manager)||!update.movie_slot||
    update.currently_in_game_view||update.info_hud_update||update.hud_controls_update||unload.clear_map_icons){e="Postmovie requires unbound SAME menu services";return false;}
 auto provider=std::make_shared<NativeMenuPostMovieV62>();provider->directory=std::make_shared<NativeMenuPrefixProviderV62>();
 provider->directory->world=world;provider->directory->manager=authored_character_menu->shared_stack_v27;
 provider->directory->process_scope_v104=authored_character_menu->process_directory_ready_v104;
 provider->directory->application=application_services_v5;provider->directory->movie_slot=update.movie_slot;
 if(!original_ui.bind_hud_world_v62(provider,[provider](const auto& q,auto& out,auto& e){return provider->info(q,out,e);},provider->controls(),e))return false;
 update.currently_in_game_view=[provider](bool& out,auto& e){NativeCharacterMenuV4* native{};if(!provider->directory->resolve(native,e))return false;
  return dh2::ui::menu_currently_in_game_view_v62([](auto& out,auto& e){dh2::loader::CanonicalCurrentLevelBorrowV1 current;
   if(!borrow_current_native_level_v27(current,e))return false;out={};if(current)out={current.level(),current.identity(),&current.level()->constructor_fields_v3().byte198};return true;},out,e);};
 update.info_hud_update=[provider](auto& e){NativeCharacterMenuV4* native{};return provider->directory->resolve(native,e)&&original_ui.info_hud_update_v62(e);};
 update.hud_controls_update=[provider](auto& e){NativeCharacterMenuV4* native{};auto app=provider->directory->application.lock();
  return provider->directory->resolve(native,e)&&app&&original_ui.construct_hud_controls_v62(app->events14(),e)&&original_ui.hud_controls_update_v62(e);};
 unload.clear_map_icons=[provider](auto& e){NativeCharacterMenuV4* native{};return provider->directory->resolve(native,e)&&provider->clear_map(e);};
 source_postmovie_v62=provider;e.clear();return true;
}
bool model_renderer::bind_native_menu_hud_world_v62(std::shared_ptr<void> world,std::shared_ptr<void> transport,
 MenuHudWorldOperationV62 operations,const dh2::ui::HudControlsServicesV62& controls,std::string& e){
 auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};
 if(!provider||!provider->directory->resolve(native,e)||native->leases->world!=world||!transport||!operations||provider->world_bound){
  if(e.empty())e="Required unbound SAME canonical HUD transport";return false;}
 provider->world_transport=std::move(transport);provider->world_operations=std::move(operations);
 provider->canonical_controls=controls;
 if(!original_ui.bind_hud_world_v62(provider,[provider](const auto& q,auto& out,auto& e){return provider->info(q,out,e);},provider->controls(),e,world))return false;
 provider->world_bound=true;return true;
}
bool model_renderer::source_native_loadmenu3_info_v62(std::shared_ptr<void> world,std::uintptr_t id,std::string& e){
 auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};
 return provider&&provider->directory->resolve(native,e)&&native->leases->world==world&&original_ui.loadmenu3_info_store_v62(id,e);
}
bool model_renderer::source_native_loadmenu3_controls_v62(std::shared_ptr<void> world,std::uintptr_t id,std::string& e){
 auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};
 return provider&&provider->directory->resolve(native,e)&&native->leases->world==world&&original_ui.loadmenu3_controls_store_v62(id,e);
}
#include "native_menu_fs_command_v114.inc"
bool model_renderer::source_native_loadmenu3_v62(std::shared_ptr<void> world,std::string& e){
 using namespace dh2::ui;
 auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};
 if(!provider||!provider->directory->resolve(native,e)||native->leases->world!=world){if(e.empty())e="Required SAME native LoadMenu3 owner";return false;}
 dh2::loader::CanonicalCurrentLevelBorrowV1 current;
 if(!borrow_current_native_level_v27(current,e)||!current){if(e.empty())e="LoadMenu3 requires actual published current Level";return false;}
 dh2::loader::CanonicalLevelContextV1::LoadingFieldsV26 loading;
 if(!current.level()->loading_fields_v26(loading,e)||!loading.state130||*loading.state130!=26){if(e.empty())e="LoadMenu3 is the original Level._LoadProcess stage26 leaf";return false;}
 if(provider->loadmenu3_attempted){e=provider->loadmenu3_failure.empty()?"LoadMenu3 source prefix cannot be replayed":provider->loadmenu3_failure;return false;}
 provider->loadmenu3_attempted=true;
 auto fail=[&](){provider->loadmenu3_failure=e;return false;};
 if(!original_ui.source_load_hud_primary_v63(front_directory,e)||!original_ui.hud_movie_borrow_v4(native->hud,native->leases->hud,e))return fail();
 const auto id=reinterpret_cast<std::uintptr_t>(native->hud);
 struct Project{MenuStackCharacterV1& target;static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Project*>(raw);SwfAsValue root;gameswf::as_object* object{};
  return graph.root_value(root,e)&&graph.borrow_object(root,object,e)&&authored_menu_stack_character_v4(object,0x84,{},q.target,e);}} project{native->hud_root};
 if(!native->hud->menu_action_script(&project,Project::run,e)||
    !native->shared_stack_v27->register_render_live_v27(id,0x84,&native->hud_root,nullptr,e))return fail();
 auto* render=native->shared_stack_v27->render(id);if(!render){e="LoadMenu3 actual RenderFX directory publication failed";return fail();}
 render->flags=4; //source SetInputBehavior84 then4, before cache services
 native->rollover_bindings.renders[3]=render;
 if(!native->shared_stack_v27->publish_hud_render_v93(id,e)||
    !original_ui.loadmenu3_info_store_v62(id,e))return fail();
 auto app=provider->directory->application.lock();
 if(!app||!original_ui.construct_hud_controls_v62(app->events14(),e)||
    !original_ui.loadmenu3_controls_store_v62(id,e)||!original_ui.source_scan_loaded_hud_v63(id,e))return fail();
 const auto weak=std::weak_ptr<NativeMenuPostMovieV62>(provider);
 if(!bind_native_hud_fs_v114(provider,e)||!native->hud->source_set_event_listener_v68(reinterpret_cast<std::uintptr_t>(native->shared_stack_v27.get()),provider,
    [weak](auto& event,auto& e){auto provider=weak.lock();if(!provider){e="Expired SAME MenuManager event receiver";return false;}return source_hud_menu_event_v68(*provider,event,e);},
    [weak](auto&,bool& out,auto& e){auto provider=weak.lock();NativeCharacterMenuV4* native{};if(!provider||!provider->directory->resolve(native,e))return false;
      out=true;return true; //MenuManager.CanHandleEvent42cc48 literal TRUE
    },e)||
    !original_ui.hud_movie_virtual10_v93(id,1,false,e))return fail();
 //Original then rebinds its derived menu singletons before PostLoad. Do not
 //invent those receivers or omit positive live context refresh operations.
 if(!source_native_loadmenu3_singletons_v67(world,e))return fail();
 //The original count is captured AFTER singleton constructors/Init, before
 //the four-resource PostLoad pass. Only its newly appended rows get Hide.
 const auto first_registry_count=native->shared_stack_v27->registry_count_v58();
 native->shared_roster_v27->bind_screen_dimensions_v68([](auto& width,auto& height,auto& e){
  if(surface_width<=0||surface_height<=0){e="Required produced native surface dimensions for source Dragable C1";return false;}
  width=surface_width;height=surface_height;e.clear();return true;
 });
 for(std::uint32_t slot=0;slot<4;++slot){MenuMovieBorrowV58 q;if(!provider->directory->movie_slot(slot,q,e))return fail();if(!q.identity)continue;
  SwfMovie* movie{};std::shared_ptr<void> lease;if(!provider->directory->movie(slot,q,movie,lease,e))return fail();
  auto* r=native->shared_stack_v27->render(q.identity);if(!r){e="PostLoad lacks actual resource directory";return fail();}
  if(!native->shared_roster_v27->post_load(*movie,q.identity,std::move(lease),r->flags,e))return fail();
 }
 for(auto i=first_registry_count;i<native->shared_stack_v27->registry_count_v58();++i){auto* q=native->shared_stack_v27->registry_at_v58(i);
  if(!q||!native->shared_roster_v27->receiver_fields_v59(q->identity)){e="PostLoad appended receiver lacks actual generated MenuBase";return fail();}
  if(!native->shared_roster_v27->register_drag_and_drops_v68(q->identity,e)||
     !native->shared_roster_v27->hide_receiver_v68(q->identity,e))return fail();
 }
 e.clear();return true;
}
#include "native_menu_load1_v98.inc"
