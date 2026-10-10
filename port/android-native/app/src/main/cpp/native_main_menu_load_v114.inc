// Include after native_main_menu_front_v114.inc, before native_source_main_menu_v114.inc.
// Reuses SAME process NativeCharacterMenu/directory/Front and source singletons.
#include <menu_main_menu_load_v114.hpp>
#include "source_process_arrays_v101.hpp"
#include <owned_hud_settings_v1.hpp>
namespace {
struct NativeMainMenuLoadV114:std::enable_shared_from_this<NativeMainMenuLoadV114> {
 std::weak_ptr<NativeCharacterMenuV4> native;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::uintptr_t manager{};
 dh2::ui::MenuMainLoadV114 body;
 unsigned post_slot_v114{};
 bool current(std::shared_ptr<NativeCharacterMenuV4>& n,std::string& e)const{
  auto app=application.lock();n=native.lock();
  if(!app||app!=application_services_v5||!n||n!=authored_character_menu||!n->process_directory_ready_v104||
     n->process_application_v104.lock()!=app||!n->shared_stack_v27||!n->shared_roster_v27||
     reinterpret_cast<std::uintptr_t>(n->shared_stack_v27.get())!=manager){
   e="Required SAME process LoadMainMenu receiver/App";return false;}return true;
 }
 NativeMenuSingletonV67* receiver(const dh2::ui::MainMenuReceiverV114& loan,std::string& e)const{
  std::shared_ptr<NativeCharacterMenuV4> n;if(!current(n,e))return nullptr;
  for(const auto& p:source_singleton_v67)if(p&&p->fields.identity==loan.identity&&
    loan.owner.get()==p.get()&&!loan.owner.owner_before(p)&&!p.owner_before(loan.owner)&&p->constructed)return p.get();
  e="Replaced actual source main-menu singleton";return nullptr;
 }
 bool movie(const dh2::ui::MenuMovieBorrowV58& loan,dh2::ui::SwfMovie*& out,std::string& e)const{
  out=nullptr;std::shared_ptr<NativeCharacterMenuV4> n;if(!current(n,e))return false;
  dh2::ui::MenuMovieBorrowV58 actual;if(!front_ui.source_main_movie_borrow_v114(actual,e))return false;
  if(!loan.identity||loan.identity!=actual.identity||!loan.actual_owner||loan.actual_owner.get()!=actual.actual_owner.get()||
    loan.actual_owner.owner_before(actual.actual_owner)||actual.actual_owner.owner_before(loan.actual_owner)){
   e="Replaced actual source RenderFX2 resource";return false;}
  std::shared_ptr<void> context;if(!front_ui.main_movie_borrow_v4(out,context,e))return false;
  return out&&reinterpret_cast<std::uintptr_t>(out)==loan.identity;
 }
 dh2::ui::MenuMainLoadServicesV114 services(const std::shared_ptr<NativeMainMenuLoadV114>& self){
  using namespace dh2::ui;MenuMainLoadServicesV114 s;s.owner=self;
  s.current=[self](auto& e){std::shared_ptr<NativeCharacterMenuV4> n;return self->current(n,e);};
  s.facts=[self](auto& out,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;auto p=source_postmovie_v62.lock();
   if(!self->current(n,e)||!p||!p->loadmenu1_services_v98.facts){if(e.empty())e="Required existing process native source display/device facts";return false;}
   model_renderer::NativePrimaryMenuFactsV98 facts;if(!p->loadmenu1_services_v98.facts(facts,e))return false;
   out={facts.receiver,facts.width_screen,facts.htc_devices,facts.no_igp};return true;};
  s.save_language=[self](auto& out,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;std::shared_ptr<OwnedHudSettingsV1> settings;
   if(!self->current(n,e)||!front_ui.gameplay_settings_borrow_v67(settings,e)||!settings)return false;
   out=settings->language();return true;};
  s.load_slot2=[self](const char* uri,auto& out,auto& e){
   std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;
   auto layout=[self](std::int32_t& width,std::int32_t& height,std::string& e){
    std::shared_ptr<NativeCharacterMenuV4> n;auto p=source_postmovie_v62.lock();
    if(!self->current(n,e)||!p||!p->loadmenu1_services_v98.facts){if(e.empty())e="Required actual source camera display borrower";return false;}
    model_renderer::NativePrimaryMenuFactsV98 facts;if(!p->loadmenu1_services_v98.facts(facts,e))return false;
    if(!facts.receiver||!facts.width_screen||!facts.height_screen){e="Required same live source camera Width/Height cells";return false;}
    width=*facts.width_screen;height=*facts.height_screen;return true;
   };
   if(!front_ui.source_load_main_movie_v114(uri,layout,out,e)||!self->current(n,e))return false;
   // Publish only existing RenderFX/root projection. Original PostLoad remains
   // after singleton repair, rather than publish_process_movie's eager PostLoad.
   if(!n->shared_stack_v27->render(out.identity)){
    SwfMovie* actual{};if(!self->movie(out,actual,e))return false;
    struct Root {NativeCharacterMenuV4& n;
     static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& x=*static_cast<Root*>(raw);SwfAsValue root;gameswf::as_object* obj{};
      return graph.root_value(root,e)&&graph.borrow_object(root,obj,e)&&authored_menu_stack_character_v4(obj,0,{},x.n.main_root,e);}
    } root{*n};
    if(!actual->menu_action_script(&root,Root::run,e)||!self->current(n,e)||
      !n->shared_stack_v27->register_render_live_v27(out.identity,0,&n->main_root,nullptr,e))return false;
    n->main=actual;n->leases->main=out.actual_owner;
   }
   return true;
  };
  s.set_behavior=[self](const auto& loan,auto flags,auto& e){SwfMovie* actual{};std::shared_ptr<NativeCharacterMenuV4> n;
   if(!self->movie(loan,actual,e)||!actual->menu_input_behavior(flags,e)||!self->current(n,e))return false;
   auto* render=n->shared_stack_v27->render(loan.identity);if(!render){e="Actual behavior setter lost SAME RenderFX directory";return false;}
   render->flags=flags;return true;};
  s.update_render=[self](const auto& loan,auto dt,bool flag,auto& e){SwfMovie* actual{};
   return self->movie(loan,actual,e)&&front_ui.movie_virtual10_v93(2,loan.identity,dt,flag,e);};
  s.singleton=[self](auto kind,auto& out,auto& e){
   out={};std::shared_ptr<NativeCharacterMenuV4> n;auto p=source_postmovie_v62.lock();if(!self->current(n,e)||!p)return false;
   MenuSingletonKindV67 k;
   switch(kind){
   case MainMenuSingletonV114::debug:k=MenuSingletonKindV67::debug;break;
   case MainMenuSingletonV114::main_menu:k=MenuSingletonKindV67::main;break;
   case MainMenuSingletonV114::character_select:k=MenuSingletonKindV67::select_class;break;
   case MainMenuSingletonV114::enter_name:k=MenuSingletonKindV67::enter_name;break;
   case MainMenuSingletonV114::lobby:k=MenuSingletonKindV67::lobby;break;
   case MainMenuSingletonV114::leaderboard:k=MenuSingletonKindV67::leaderboard;break;
   default:e="Invalid original main-menu singleton kind";return false;
   }
   NativeMenuSingletonV67* record{};if(!source_singleton_get_v67(*p,k,record,e)||!record)return false;
   auto pin=source_singleton_v67[static_cast<unsigned>(k)];
   if(!pin||pin.get()!=record){e="Singleton constructor changed its same source owner";return false;}
   out={pin,record->fields.identity};return true;
  };
  s.weak_live=[self](const auto& loan,bool& out,auto& e){auto* p=self->receiver(loan,e);if(!p)return false;
   out=p->state.weak_character_v59.source_live_v114();return true;};
  s.initialize=[self](auto kind,const auto& loan,auto& e){auto* p=self->receiver(loan,e);auto provider=source_postmovie_v62.lock();
   if(!p||!provider)return false;
   if((kind==MainMenuSingletonV114::debug&&p->kind!=MenuSingletonKindV67::debug)||
      (kind==MainMenuSingletonV114::character_select&&p->kind!=MenuSingletonKindV67::select_class)||
      (kind!=MainMenuSingletonV114::debug&&kind!=MainMenuSingletonV114::character_select)){
    e="Qualified source main-menu Init receiver mismatch";return false;}
   return source_singleton_init_v67(*provider,*p,e);
  };
  s.register_menu=[self](const auto& loan,auto& e){auto* p=self->receiver(loan,e);auto provider=source_postmovie_v62.lock();
   return p&&provider&&provider->directory&&provider->directory->register_base(p->fields,p->state,p->projection,e);};
  s.hide_debug_character=[self](const auto& loan,auto& e){
   auto* record=self->receiver(loan,e);auto p=source_postmovie_v62.lock();if(!record||!p||record->kind!=MenuSingletonKindV67::debug)return false;
   for(unsigned slot=0;slot<4;++slot){MenuMovieBorrowV58 source;if(!p->directory->movie_slot(slot,source,e))return false;
    if(source.identity!=record->fields.render)continue;SwfMovie* actual{};std::shared_ptr<void> pin;if(!p->directory->movie(slot,source,actual,pin,e))return false;
    struct Hide {NativeMenuSingletonV67& record;
     static bool run(void* raw,SwfAsGraph& graph,std::string& e){auto& x=*static_cast<Hide*>(raw);SwfAsValue value;bool live{};
      if(!x.record.state.weak_character_v59.borrow(graph,value,live,e))return false;
      if(!live){e="Unsupported original NULL Debug character9b store";return false;}
      gameswf::as_object* obj{};if(!graph.borrow_object(value,obj,e)||!obj||!obj->is(gameswf::character::m_class_id)){
       if(e.empty())e="Required actual Debug gameswf.character";return false;}
      static_cast<gameswf::character*>(obj)->set_visible(false);x.record.projection.visible=0;return true;
     }
    } hide{*record};return actual->menu_action_script(&hide,Hide::run,e);
   }
   e="Original Debug character lost its SAME actual movie";return false;
  };
  s.registry_count=[self](auto& out,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;out=n->shared_stack_v27->registry_count_v58();return true;};
  s.post_load=[self](auto& e){std::shared_ptr<NativeCharacterMenuV4> n;auto p=source_postmovie_v62.lock();if(!self->current(n,e)||!p)return false;
   n->shared_roster_v27->bind_screen_dimensions_v68([](auto& w,auto& h,auto& x){
    if(surface_width<=0||surface_height<=0){x="Required produced native source drag dimensions";return false;}w=surface_width;h=surface_height;return true;});
   for(;self->post_slot_v114<4;){const auto slot=self->post_slot_v114;MenuMovieBorrowV58 q;if(!p->directory->movie_slot(slot,q,e))return false;if(!q.identity){++self->post_slot_v114;continue;}
    SwfMovie* actual{};std::shared_ptr<void> pin;if(!p->directory->movie(slot,q,actual,pin,e)||!self->current(n,e))return false;
    auto* r=n->shared_stack_v27->render(q.identity);if(!r){e="Native PostLoad lost SAME positive render slot";return false;}
    if(!n->shared_roster_v27->post_load(*actual,q.identity,std::move(pin),r->flags,e))return false;
    ++self->post_slot_v114;if(!self->current(n,e))return false;
   }return true;
  };
  s.registry_at=[self](auto index,auto& out,auto& e){
   out={};std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;
   auto* menu=n->shared_stack_v27->registry_at_v58(index);if(!menu){e="Native live appended registry index disappeared";return false;}
   struct Pin{std::shared_ptr<void> receiver,menu,render;};auto pin=std::make_shared<Pin>();
   std::function<bool(std::uintptr_t,bool&,std::string&)> visibility;
   if(!n->shared_roster_v27->capture_receiver_v104(menu->identity,pin->receiver,visibility,e)||
      !n->shared_stack_v27->capture_storage_v101(menu,pin->menu,pin->render,e))return false;
   out={pin,menu->identity};return true;
  };
  s.register_drag_and_drops=[self](const auto& loan,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;
   return n->shared_roster_v27->register_drag_and_drops_v68(loan.identity,e);};
  s.hide_virtual10=[self](const auto& loan,auto& e){std::shared_ptr<NativeCharacterMenuV4> n;if(!self->current(n,e))return false;
   auto* menu=n->shared_stack_v27->registered_menu_v27(loan.identity);if(!menu||!loan.owner){e="Required actual appended source menu pin";return false;}
   auto front=native_main_menu_front_v114;if(!front){e="Required same native selected Hide provider";return false;}
   return front->hide(*menu,e);
  };
  return s;
 }
};
std::weak_ptr<NativeMainMenuLoadV114> native_main_menu_load_v114;
bool bind_source_main_menu_load_v114(std::string& e){
 auto n=authored_character_menu;auto app=application_services_v5;
 if(!n||!app||!n->process_directory_ready_v104||n->process_application_v104.lock()!=app||!n->shared_stack_v27){
  e="Required existing initialized native process menu/Front";return false;}
 if(!native_main_menu_load_v114.expired()){e="Actual main-menu loader already enrolled";return false;}
 auto owner=std::make_shared<NativeMainMenuLoadV114>();owner->native=n;owner->application=app;owner->manager=reinterpret_cast<std::uintptr_t>(n->shared_stack_v27.get());
 std::weak_ptr<NativeMainMenuLoadV114> weak=owner;
 if(!bind_source_main_menu_front_v114(owner,[weak](auto manager,auto& e){auto p=weak.lock();std::shared_ptr<NativeCharacterMenuV4> n;
  if(!p||!p->current(n,e)||manager!=p->manager){if(e.empty())e="Replaced actual source main-menu resource body";return false;}
  if(p->body.complete())p->post_slot_v114=0;
  return p->body.load(p->services(p),e);
 },e))return false;
 native_main_menu_load_v114=owner;e.clear();return true;
}
// Call ONLY for an explicit resume of the SAME failed parent command, BEFORE
bool ensure_native_main_menu_load_v114(std::string& e){
 if(auto existing=native_main_menu_load_v114.lock()){
  std::shared_ptr<NativeCharacterMenuV4> actual;return existing->current(actual,e);
 }
 return bind_source_main_menu_load_v114(e);
}
// Call ONLY for an explicit resume of the SAME failed parent command, BEFORE
// parent ApplicationGoToMainMenu.resume. This arms; it does not eagerly execute
// nested Ctor/Reset/Load or start a new source occurrence.
bool arm_source_main_menu_front_resume_v114(std::string& e){
 auto p=native_main_menu_load_v114.lock();auto front=native_main_menu_front_v114;
 std::shared_ptr<NativeCharacterMenuV4> actual;
 if(!p||!front||!p->current(actual,e)){if(e.empty())e="Required retained SAME main-menu recovery providers";return false;}
 return front->reset_body.arm_resume(e)&&p->body.arm_resume(e)&&front_ui.arm_main_menu_state_resume_v114(e);
}
}
