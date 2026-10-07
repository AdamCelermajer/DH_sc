#include <optional>
// Included after native_gslevel_menu_v27.inc. One directory and four actual
// primary/camera slots serve Update, Unload and front draw transport.
namespace {
struct NativeMenuResourcesV93 {
 std::weak_ptr<NativeCharacterMenuV4> owner;
 std::weak_ptr<void> world;
 struct Primary1ClearReceiptV98 {
  dh2::ui::MenuMovieBorrowV58 movie;
  std::weak_ptr<dh2::ui::MenuStackOwnerV1> manager;
  bool field_cleared{},render_retired{},prefix_retired{},camera_deleted{};
  dh2::ui::MenuCameraBorrowV93 deleted_camera;
 };
 std::optional<Primary1ClearReceiptV98> primary1_clear_v98;
 std::shared_ptr<NativeCharacterMenuV4> resolve(std::string& e)const{
  auto native=owner.lock();auto actual=world.lock();
  const bool process=native&&application_services_v5&&native->process_directory_ready_v104&&native->process_application_v104.lock()==application_services_v5;
  if(!native||native!=authored_character_menu||!native->shared_stack_v27||
     (!process&&(!actual||native->leases->world!=actual||!native->bootstrap_complete_v27))){
   e="Expired or replaced actual menu resource directory";return {};
  }
  return native;
 }
 bool movie_slot(std::uint32_t slot,dh2::ui::MenuMovieBorrowV58& out,std::string& e)const{
  out={};auto native=resolve(e);if(!native)return false;
  if(slot==0||slot==2)return front_ui.movie_slot_v93(slot,out,e);
  if(slot==3)return original_ui.hud_movie_slot_v93(out,e);
  if(slot!=1){e="Invalid original primary movie slot";return false;}
  auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
  if(!panel){e.clear();return true;}
  const auto id=panel->source_render_identity_v91();
  if(id){
   if(panel->source_stack_v4()!=native->shared_stack_v27.get()||!panel->source_movie_v91()){
    e="Character movie slot differs from same retained manager";return false;
   }
   out={panel->source_movie_lease_v94(),id};
   if(!out.actual_owner){e="Character movie slot lacks its actual resource lease";return false;}
  }
  e.clear();return true;
 }
 bool typed_movie(const dh2::ui::MenuMovieBorrowV58& q,std::uint32_t& slot,
                  dh2::ui::SwfMovie*& movie,std::shared_ptr<void>& pin,std::string& e)const{
  movie=nullptr;pin.reset();
  if(!q.identity||!q.actual_owner){e="Required live retained movie receiver";return false;}
  for(slot=0;slot<4;++slot){
   dh2::ui::MenuMovieBorrowV58 current;if(!movie_slot(slot,current,e))return false;
   if(current.identity!=q.identity)continue;
   pin=std::move(current.actual_owner);
   if(slot==1){movie=character_panel->authored_native_v4()->source_movie_v91();return movie!=nullptr;}
   std::shared_ptr<void> facade;
   return slot==0?front_ui.base_movie_borrow_v4(movie,facade,e):
          slot==2?front_ui.main_movie_borrow_v4(movie,facade,e):
                  original_ui.hud_movie_borrow_v4(movie,facade,e);
  }
  e="Menu operation receiver is no longer in its actual primary slot";return false;
 }
 bool finish_primary1_clear_v98(std::string& e){
  if(!primary1_clear_v98){e="Missing admitted actual primary1 post-D0 clear receipt";return false;}
  auto native=resolve(e);auto manager=primary1_clear_v98->manager.lock();
  if(!native||!manager||native->shared_stack_v27.get()!=manager.get()||native->shared_stack_v27.owner_before(manager)||manager.owner_before(native->shared_stack_v27)){
   if(e.empty())e="Primary1 clear continuation belongs to replaced actual MenuManager";return false;
  }
  auto& p=*primary1_clear_v98;dh2::ui::MenuMovieBorrowV58 current;if(!movie_slot(1,current,e))return false;
  if(!p.field_cleared){
   if(current.identity!=p.movie.identity||current.actual_owner.get()!=p.movie.actual_owner.get()||current.actual_owner.owner_before(p.movie.actual_owner)||p.movie.actual_owner.owner_before(current.actual_owner)){
    e="Primary1 field-clear continuation receiver replaced after actual D0";return false;
   }
   auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
   if(!panel||!panel->clear_movie_slot_v93(e))return false;
   p.field_cleared=true;native->current_render=0;native->width=native->height=0;
  }else if(current.identity){e="Cleared primary1 replaced while native retirement remains pending";return false;}
  if(!p.render_retired){if(!manager->retire_render_resource_v93(p.movie.identity,e))return false;p.render_retired=true;}
  if(!p.prefix_retired){if(!character_panel||!character_panel->retire_failed_authored_prefix_v98(e))return false;p.prefix_retired=true;}
  e.clear();return true;
 }
 bool camera_slot(std::uint32_t slot,dh2::ui::MenuCameraBorrowV93& out,std::string& e)const{
  out={};auto native=resolve(e);if(!native)return false;
  if(slot==0||slot==2)return front_ui.camera_slot_v93(slot,out,e);
  if(slot==3)return original_ui.hud_camera_slot_v93(out,e);
  if(slot!=1){e="Invalid original paired camera slot";return false;}
  if(native->character_camera_v93)
   out={native->character_camera_v93,reinterpret_cast<std::uintptr_t>(native->character_camera_v93.get())};
  e.clear();return true;
 }
 bool draws(std::vector<dh2::android_ui::FrontMovieDrawV93>& out,std::string& e)const{
  out.clear();auto native=resolve(e);if(!native)return false;
  const auto* active=native->shared_stack_v27->view();
  if(!active||active->count>active->capacity){e="Required actual active render array";return false;}
  for(std::uint32_t i=0;i<active->count;++i){
   auto* render=active->renders[i];if(!render){e="Null active MenuFX occurrence";return false;}
   std::uint32_t slot{};
   for(;slot<4;++slot){dh2::ui::MenuMovieBorrowV58 q;if(!movie_slot(slot,q,e))return false;if(q.identity==render->identity)break;}
   if(slot==4){e="Active render no longer belongs to actual movie slots";return false;}
   if(slot!=0&&slot!=2)continue;
   if(render->count>render->capacity){e="Invalid actual front render state count";return false;}
   for(std::uint32_t j=0;j<render->count;++j){
    auto* state=render->states[j];
    if(!state||state->render!=render){e="Front state lost its actual render";return false;}
    auto* fields=native->shared_roster_v27->receiver_fields_v59(state->identity);
    const auto* path=native->shared_roster_v27->receiver_path_v93(state->identity);
    if(!fields||!path||path->empty()){e="Required actual PostLoad front clip path";return false;}
    if(fields->visible74)out.push_back({slot,render->identity,*path});
   }
  }
  e.clear();return true;
 }
 dh2::ui::MultiMenuResourceServicesV93 services(const std::shared_ptr<NativeMenuResourcesV93>& self,
                                             const std::shared_ptr<void>& manager){
  dh2::ui::MultiMenuResourceServicesV93 s;s.actual_manager=manager;
  s.movie_slot=[self](auto slot,auto& out,auto& e){return self->movie_slot(slot,out,e);};
  s.active_count=[self](auto& out,auto& e){auto native=self->resolve(e);if(!native)return false;
   auto* active=native->shared_stack_v27->view();if(!active){e="Missing actual active render array";return false;}
   const auto bits=active->count;std::memcpy(&out,&bits,sizeof(out));return true;};
  s.active_at=[self](auto i,auto& out,auto& e){auto native=self->resolve(e);if(!native)return false;
   auto* active=native->shared_stack_v27->view();if(!active||i>=active->count||!active->renders[i]){
    e="Actual active render index changed";return false;}
   out=active->renders[i]->identity;return true;};
  s.remove_active=[self](auto i,auto id,auto& e){auto native=self->resolve(e);
   return native&&native->shared_stack_v27->remove_active_render_v93(i,id,e);};
  s.movie_virtual_c=[self](const auto& q,auto& e){std::uint32_t slot{};dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;
   return self->typed_movie(q,slot,movie,pin,e)&&model_renderer::unload_native_menu_resource_v91(self->world.lock(),q.identity,*movie,e);};
  s.deleting_virtual4=[self](auto slot,const auto& q,auto& e){auto native=self->resolve(e);if(!native)return false;
   if(slot==0||slot==2)return front_ui.deleting_movie_v93(slot,q.identity,e);
   if(slot==3)return original_ui.deleting_hud_movie_v93(q.identity,e);
   auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
   if(slot!=1||!panel){e="Missing same character movie deleting receiver";return false;}
   return panel->deleting_movie_v93(q.identity,e);};
  s.clear_movie_slot=[self](auto slot,auto& e){auto native=self->resolve(e);if(!native)return false;
   dh2::ui::MenuMovieBorrowV58 before;if(!self->movie_slot(slot,before,e))return false;
   auto retire=[&](){return !before.identity||native->shared_stack_v27->retire_render_resource_v93(before.identity,e);};
   if(slot==0||slot==2){
    if(!front_ui.clear_movie_slot_v93(slot,e))return false;
    if(slot==0){native->base=nullptr;native->leases->base.reset();}
    else {native->main=nullptr;native->leases->main.reset();}
    return retire();
   }
   if(slot==3){
    if(!original_ui.clear_hud_movie_slot_v93(e))return false;
    native->hud=nullptr;native->leases->hud.reset();return retire();
   }
   auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
   if(slot!=1||!panel||!before.identity||!before.actual_owner){e="Missing same character primary clear receiver";return false;}
   if(self->primary1_clear_v98){e="Primary1 post-D0 clear already admitted; use retained continuation";return false;}
   self->primary1_clear_v98.emplace();
   self->primary1_clear_v98->movie=before;self->primary1_clear_v98->manager=native->shared_stack_v27;
   return self->finish_primary1_clear_v98(e);};
  s.resume_movie_clear_v98=[self](auto slot,bool& resumed,auto& e){resumed=false;
   if(slot!=1){e.clear();return true;}
   if(!self->primary1_clear_v98){
    dh2::ui::MenuMovieBorrowV58 actual;if(!self->movie_slot(1,actual,e))return false;
    //No native publication means no movie D0: only retire proven zero-prefix
    //Session storage at its own quiescent boundary, then preserve orphan camera.
    if(!actual.identity&&character_panel&&!character_panel->retire_failed_authored_prefix_v98(e))return false;
    e.clear();return true;
   }
   if(!self->finish_primary1_clear_v98(e))return false;resumed=true;return true;};
  s.camera_slot=[self](auto slot,auto& out,auto& e){return self->camera_slot(slot,out,e);};
  s.deleting_camera_virtual4=[self](auto slot,const auto& q,auto& e){auto native=self->resolve(e);if(!native)return false;
   if(slot==0||slot==2)return front_ui.deleting_camera_v93(slot,q.identity,e);
   if(slot==3)return original_ui.delete_hud_camera_v93(q.identity,e);
   auto camera=native->character_camera_v93;
   if(slot==1&&self->primary1_clear_v98&&self->primary1_clear_v98->camera_deleted){
    const auto& prior=self->primary1_clear_v98->deleted_camera;
    if(!camera||!camera->deleted||q.identity!=prior.identity||q.actual_owner.get()!=prior.actual_owner.get()||q.actual_owner.owner_before(prior.actual_owner)||prior.actual_owner.owner_before(q.actual_owner)){
     e="Primary1 paired camera replaced after completed actual D0";return false;
    }e.clear();return true;
   }
   if(slot!=1||!camera||camera->deleted||reinterpret_cast<std::uintptr_t>(camera.get())!=q.identity){
    e="Character camera deleting receiver differs from actual slot";return false;}
   camera->deleted=true;
   if(self->primary1_clear_v98){self->primary1_clear_v98->deleted_camera=q;self->primary1_clear_v98->camera_deleted=true;}
   e.clear();return true;};
  s.clear_camera_slot=[self](auto slot,auto& e){auto native=self->resolve(e);if(!native)return false;
   if(slot==0||slot==2)return front_ui.clear_camera_slot_v93(slot,e);
   if(slot==3)return original_ui.clear_hud_camera_slot_v93(e);
   if(slot!=1||(native->character_camera_v93&&!native->character_camera_v93->deleted)){
    e="Character camera field clear requires actual deleting destructor";return false;}
   native->character_camera_v93.reset();self->primary1_clear_v98.reset();e.clear();return true;};
  return s;
 }
};
}
bool model_renderer::prepare_native_menu_lifecycle_v93(std::shared_ptr<void> world,std::string& error){
 std::shared_ptr<void> manager;if(!borrow_native_menu_manager_v58(world,manager,error))return false;
 auto native=authored_character_menu;
 if(native->source_unload_v58&&native->source_update_v58){error.clear();return true;}
 if(native->source_lifecycle_attempted_v93){error="Native menu lifecycle retained failed bind prefix; teardown required";return false;}
 native->source_lifecycle_attempted_v93=true;
 auto resources=std::make_shared<NativeMenuResourcesV93>();resources->owner=native;resources->world=world;
 dh2::ui::MenuManagerUnloadServicesV58 unload;unload.actual_manager=manager;
 dh2::ui::MenuManagerUpdateServicesV58 update;update.actual_manager=manager;
 unload.movie_slot=update.movie_slot=[resources](auto slot,auto& out,auto& e){return resources->movie_slot(slot,out,e);};
 auto multi=std::make_shared<dh2::ui::MultiMenuResourceServicesV93>(resources->services(resources,manager));
 unload.unload_swf_file=[multi](auto id,auto& e){return dh2::ui::multi_menu_unload_swf_v93(*multi,id,e);};
 update.movie_virtual10=[resources](const auto& q,auto dt,auto flag,auto& e){
  auto native=resources->resolve(e);if(!native||!native->ensure_source_gameplay_binding_v109(e))return false;
  std::uint32_t slot{};dh2::ui::SwfMovie* movie{};std::shared_ptr<void> pin;
  if(!resources->typed_movie(q,slot,movie,pin,e))return false;
  if(slot==0||slot==2)return front_ui.movie_virtual10_v93(slot,q.identity,dt,flag,e);
  if(slot==3)return original_ui.hud_movie_virtual10_v93(q.identity,dt,flag,e);
  auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
  if(!panel){e="Missing actual character source update owner";return false;}
  return panel->source_movie_virtual10_v93(q.identity,dt,flag,e);
 };
 if(!bind_native_menu_postmovie_v62(world,unload,update,error))return false;
 if(!front_ui.claim_menu_transport_v93(manager,[resources](auto& draws,auto& e){return resources->draws(draws,e);},error))return false;
 native->source_front_claimed_v93=true;
 if(!original_ui.claim_menu_transport_v93(manager,error))return false;
 native->source_hud_claimed_v93=true;
 return bind_native_menu_lifecycle_v58(std::move(world),std::move(unload),std::move(update),error);
}
bool model_renderer::borrow_native_campaign_drm_v93(std::shared_ptr<void> world,bool& value,std::string& error){
 std::shared_ptr<void> actual;if(!borrow_native_menu_manager_v58(world,actual,error))return false;
 value=authored_character_menu->drm.source_global();error.clear();return true;
}
bool model_renderer::prepare_native_menu_process_v104(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::function<bool(const char*,std::int32_t&,std::string&)> debug,std::string& e){
 if(!app||app!=application_services_v5||!debug){e="Required actual process Application/MenuManager Debug enrollment";return false;}
 if(!authored_character_menu)authored_character_menu=std::make_shared<NativeCharacterMenuV4>();
 if(!authored_character_menu->prepare_process_directory_v104(app,std::move(debug),e))return false;
 if(!prepare_native_menu_lifecycle_v93({},e))return false;
 if(!authored_character_menu->primary1_enrolled_v98){
  if(!enroll_native_process_primary1_v98(app,e))return false;
  authored_character_menu->primary1_enrolled_v98=true;
 }
 if(!ensure_native_main_menu_load_v114(e))return false;
 e.clear();return true;
}
bool model_renderer::publish_native_menu_process_movie_v104(std::uint32_t slot,std::string& e){
 if(!authored_character_menu||!application_services_v5||authored_character_menu->process_application_v104.lock()!=application_services_v5){
  e="Required actual process Application/movie publication";return false;
 }
 return authored_character_menu->publish_process_movie_v104(slot,e);
}
bool model_renderer::borrow_native_process_menu_directory_v104(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::shared_ptr<dh2::ui::MenuStackOwnerV1>& out,std::string& e){
 out.reset();
 if(!app||app!=application_services_v5||!authored_character_menu||!authored_character_menu->process_directory_ready_v104||
    authored_character_menu->process_application_v104.lock()!=app){e="Required same existing process MenuManager directory";return false;}
 out=authored_character_menu->shared_stack_v27;e.clear();return true;
}
bool model_renderer::validate_native_campaign_license_v93(std::shared_ptr<void> world,std::int32_t argument,std::string& error){
 std::shared_ptr<void> actual;if(!borrow_native_menu_manager_v58(world,actual,error))return false;
 error="Required actual ALicenseCheck_ValidateLicense("+std::to_string(argument)+") positive source branch";
 return false;
}
