// Include in native_app after native_menu_fs_command_v114.inc; before
// native_source_main_menu_v114.inc. No additional process menu/World owner.
#include <menu_main_menu_state_v114.hpp>
namespace {
struct NativeMainMenuFrontV114 {
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::weak_ptr<NativeCharacterMenuV4> native;
 std::uintptr_t manager{};
 dh2::ui::MenuManagerResetV114 reset_body;
 std::shared_ptr<void> resources; //independent real process resource provider
 std::function<bool(std::uintptr_t,std::string&)> load_main;
 bool current(std::shared_ptr<NativeCharacterMenuV4>& out,std::string& e)const{
  auto app=application.lock();out=native.lock();
  if(!app||app!=application_services_v5||!out||out!=authored_character_menu||
     !out->process_directory_ready_v104||out->process_application_v104.lock()!=app||
     !out->shared_stack_v27||!out->shared_roster_v27||
     reinterpret_cast<std::uintptr_t>(out->shared_stack_v27.get())!=manager){
   e="Required SAME retained process App/MenuManager";return false;
  }return true;
 }
 bool hide(dh2::ui::MenuStackMenuV1& menu,std::string& e)const{
  std::shared_ptr<NativeCharacterMenuV4> n;if(!current(n,e))return false;
  auto stack=n->shared_stack_v27;std::shared_ptr<void> a,b;
  if(!stack->capture_storage_v101(&menu,a,b,e))return false;
  if(model_renderer::native_derived_menu_fields_v67(menu.identity)||model_renderer::native_map_menu_fields_v62(menu.identity)||
     model_renderer::native_debug_menu_fields_v62(menu.identity))return model_renderer::native_selected_menu_hide_v119(menu.identity,e);
  dh2::ui::MenuStackRequestV1 q{};q.operation=dh2::ui::MenuStackOperationV1::menu_hide;q.menu=&menu;q.render=menu.render;
  const auto source=n->shared_roster_v27->stack_services();
  if(!source.invoke||source.invoke(source.context,stack->view(),&q)){
   e=n->shared_roster_v27->error().empty()?"Required selected actual virtual10 Hide":n->shared_roster_v27->error();return false;
  }return true;
 }
 dh2::ui::MenuManagerResetServicesV114 reset_services(const std::shared_ptr<NativeMainMenuFrontV114>& self){
  dh2::ui::MenuManagerResetServicesV114 s;s.owner=self;
  s.current=[self](auto& e){std::shared_ptr<NativeCharacterMenuV4> n;return self->current(n,e);};
  s.render_slot=[self](unsigned slot,auto*& out,auto& pin,auto& e){
   out=nullptr;pin.reset();std::shared_ptr<NativeCharacterMenuV4> n;auto p=source_postmovie_v62.lock();
   if(!self->current(n,e)||!p||!p->directory)return false;
   dh2::ui::MenuMovieBorrowV58 movie;if(!p->directory->movie_slot||!p->directory->movie_slot(slot,movie,e))return false;
   if(!movie.identity)return true;
   out=n->shared_stack_v27->render(movie.identity);if(!out){e="Positive source slot lacks SAME MenuFX storage";return false;}
   struct Pin{std::shared_ptr<void> movie,render;};auto actual=std::make_shared<Pin>();actual->movie=movie.actual_owner;
   if(!actual->movie||!n->shared_stack_v27->capture_render_storage_v114(out,actual->render,e))return false;
   pin=actual;return true;
  };
  s.pop_all=[self](auto& render,auto& e){
   std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;
   auto stack=n->shared_stack_v27;std::shared_ptr<void> pin;
   if(stack->render(render.identity)!=&render||!stack->capture_render_storage_v114(&render,pin,e))return false;
   dh2::ui::MenuFXPopAllServicesV114 p;p.owner=pin;
   p.current=[self](auto& x){std::shared_ptr<NativeCharacterMenuV4> n;return self->current(n,x);};
   p.hide_virtual10=[self](auto& menu,auto& x){return self->hide(menu,x);};
   p.resize_states=[self](auto& r,auto count,auto& x){std::shared_ptr<NativeCharacterMenuV4> n;return self->current(n,x)&&n->shared_stack_v27->resize_render_states_v114(r,count,x);};
   return dh2::ui::menu_fx_pop_all_v114(render,p,e);
  };
  s.registry_count=[self](auto& out,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;
   const auto bits=n->shared_stack_v27->registry_count_v58();std::memcpy(&out,&bits,4);return true;};
  s.registry_at=[self](auto i,auto*& out,auto& pin,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;
   out=n->shared_stack_v27->registry_at_v58(i);if(!out){e="Source Reset live registry index unavailable";return false;}
   struct Pin{std::shared_ptr<void> menu,render;};auto p=std::make_shared<Pin>();
   if(!n->shared_stack_v27->capture_storage_v101(out,p->menu,p->render,e))return false;pin=p;return true;};
  s.is_visible=[self](auto& menu,bool& out,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;
   // Original IsVisible41f3f4 directly reads SAME native receiver74.
   auto* fields=n->shared_roster_v27->receiver_fields_v59(menu.identity);
   if(!fields)fields=model_renderer::native_derived_menu_fields_v67(menu.identity);
   if(!fields)fields=model_renderer::native_map_menu_fields_v62(menu.identity);
   if(!fields)fields=model_renderer::native_debug_menu_fields_v62(menu.identity);
   if(!fields&&character_panel&&character_panel->authored_native_v4()&&character_panel->authored_native_v4()->source_stack_v4()==n->shared_stack_v27.get())
    fields=character_panel->authored_native_v4()->receiver_fields_v59(menu.identity);
   if(!fields){e="Required actual native MenuBase74 borrower";return false;}out=fields->visible74!=0;return true;};
  s.hide_virtual10=[self](auto& menu,auto& e){return self->hide(menu,e);};
  s.store_listener88=[self](auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;n->fields.source_reset_listener_tail_v62();return true;};
  return s;
 }
};
std::shared_ptr<NativeMainMenuFrontV114> native_main_menu_front_v114;
bool reset_source_main_menu_manager_v114(std::uintptr_t manager,std::string& e){
 auto p=native_main_menu_front_v114;std::shared_ptr<NativeCharacterMenuV4> n;
 if(!p||!p->current(n,e)||p->manager!=manager){if(e.empty())e="Required enrolled SAME process Reset owner";return false;}
 return p->reset_body.reset(p->reset_services(p),e);
}
bool get_source_main_menu_v114(std::uintptr_t manager,const char* name,std::uintptr_t& out,std::string& e){
 out=0;auto p=native_main_menu_front_v114;std::shared_ptr<NativeCharacterMenuV4> n;
 if(!name||!p||!p->current(n,e)||manager!=p->manager){if(e.empty())e="Required actual GetMenuByName CString/manager";return false;}
 auto* menu=n->shared_stack_v27->menu(name);out=menu?menu->identity:0;e.clear();return true;
}
bool bind_source_main_menu_front_v114(std::shared_ptr<void> resources,
 std::function<bool(std::uintptr_t,std::string&)> original_load_main,std::string& e){
 if(native_main_menu_front_v114){e="Process main-menu state resource provider already enrolled";return false;}
 auto app=application_services_v5;auto n=authored_character_menu;
 if(!app||!n||!resources||!original_load_main||!n->process_directory_ready_v104||n->process_application_v104.lock()!=app||!n->shared_stack_v27){
  e="Required actual process App/MenuManager and genuine LoadMainMenu provider";return false;}
 const auto same=[&](const auto& owner){return owner&&!resources.owner_before(owner)&&!owner.owner_before(resources);};
 if(same(app)||same(n)||same(n->shared_stack_v27)||same(n->leases->world)){e="Main resource provider must be independent of App/manager/World";return false;}
 auto p=std::make_shared<NativeMainMenuFrontV114>();p->application=app;p->native=n;p->manager=reinterpret_cast<std::uintptr_t>(n->shared_stack_v27.get());
 p->resources=std::move(resources);p->load_main=std::move(original_load_main);
 dh2::ui::GSFlashMenuServicesV114 s;s.owner=p;
 s.current=[p](auto& e){std::shared_ptr<NativeCharacterMenuV4> n;return p->current(n,e);};
 s.debug_load=[p](auto& e){std::shared_ptr<NativeCharacterMenuV4> n;auto q=source_postmovie_v62.lock();return p->current(n,e)&&q&&q->directory&&q->directory->debug_load(e);};
 s.debug_switch=[p](auto key,bool& out,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;auto q=source_postmovie_v62.lock();return p->current(n,e)&&q&&q->directory&&q->directory->debug_switch(key,out,e);};
 s.menu_manager=[p](auto& pin,auto& id,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!p->current(n,e))return false;pin=n->shared_stack_v27;id=p->manager;return true;};
 s.load_main_menu=[p](auto manager,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!p->current(n,e)||manager!=p->manager)return false;return p->load_main(manager,e);};
 s.get_menu=[p](auto manager,const char* name,auto& out,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!p->current(n,e)||manager!=p->manager||!name)return false;
  auto* menu=n->shared_stack_v27->menu(name);out=menu?menu->identity:0;return true;};
 s.push_menu=[p](auto manager,auto id,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!p->current(n,e)||manager!=p->manager)return false;
  auto* menu=n->shared_stack_v27->registered_menu_v27(id);if(!menu||!menu->name){e="Native GSFlashMenu Push receiver/name unavailable";return false;}
  const auto source=n->shared_roster_v27->stack_services();
  const auto status=dh2_menu_stack_push_v1(n->shared_stack_v27->view(),menu,&source);if(status){e=n->shared_roster_v27->error();return false;}return true;};
 s.pop_menu=[p](auto manager,auto id,bool above,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!p->current(n,e)||manager!=p->manager||above)return false;
  auto* menu=n->shared_stack_v27->registered_menu_v27(id);if(!menu||!menu->name){e="Native GSFlashMenu Pop receiver/name unavailable";return false;}
  const auto status=n->shared_stack_v27->pop_named_direct(menu->name,n->shared_roster_v27->stack_services());if(status){e=n->shared_roster_v27->error();return false;}return true;};
 if(!front_ui.bind_main_menu_state_services_v114(std::move(s),e))return false;
 native_main_menu_front_v114=std::move(p);e.clear();return true;
}
}
