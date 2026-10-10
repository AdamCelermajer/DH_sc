//Include at native_menu_postmovie_v62.inc end, after source_postmovie exists.
//This is MenuManager.LoadMenu(1) sequencing on the SAME process directory.
bool model_renderer::bind_native_primary1_loader_v98(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 NativePrimary1LoadServicesV98 services,std::string& e){
 auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};
 if(!provider||!provider->directory->resolve(native,e)||!app||provider->directory->application.lock()!=app||
  !native->process_directory_ready_v104||native->process_application_v104.lock()!=app||!services.owner||!services.facts||!services.load_primary_scoped){if(e.empty())e="Required SAME existing process scoped primary1 loader and actual layout globals";return false;}
 if(provider->loadmenu1_services_v98.owner){e="Actual primary1 movie loader already enrolled; preserve existing provider";return false;}
 const auto same=[&](const auto& containing){return containing&&!services.owner.owner_before(containing)&&!containing.owner_before(services.owner);};
 if(same(native->shared_stack_v27)||same(provider->directory->current_world())){e="Primary1 resource provider must not own its containing manager/World";return false;}
 provider->loadmenu1_services_v98=std::move(services);e.clear();return true;
}
bool model_renderer::compose_native_primary1_scope_v98(dh2::ui::AuthoredCharacterPanelServicesV2& services,NativePrimary1ContinueV98 scope,std::string& e){
 if(!scope){e="Required actual primary1 loading/process continuation";return false;}
 const auto incoming=services.source_continue_v98;
 services.source_continue_v98=[incoming,scope=std::move(scope)](auto& e){if(!scope(e))return false;
  const bool ok=!incoming||incoming(e);if(!scope(e))return false;return ok;
 };return true;
}
bool model_renderer::source_native_loadmenu1_v98(std::shared_ptr<void> world,std::string& e){
 using namespace dh2::ui;
 auto provider=source_postmovie_v62.lock();NativeCharacterMenuV4* native{};try{
 if(!provider||!provider->directory->resolve(native,e)||provider->directory->current_world()!=world){if(e.empty())e="Required SAME current process MenuManager/campaign";return false;}
 dh2::loader::CanonicalCurrentLevelBorrowV1 current;
 if(!borrow_current_native_level_v27(current,e)||!current||current.level()->constructor_fields_v3().field130!=12){if(e.empty())e="LoadMenu1 requires actual published Level state12";return false;}
 const auto prior=provider->loadmenu1_level_v98.lock();
 if(prior&&prior.get()==current.level().get()&&!prior.owner_before(current.level())&&!current.level().owner_before(prior)&&provider->loadmenu1_attempted_v98){if(provider->loadmenu1_failure_v98.empty())provider->loadmenu1_failure_v98="Source primary1 loading occurrence already attempted";e=provider->loadmenu1_failure_v98;return false;}
 const bool earlier_attempt=provider->loadmenu1_attempted_v98;
 provider->loadmenu1_level_v98=current.level();provider->loadmenu1_attempted_v98=true;provider->loadmenu1_failure_v98.clear();
 const auto level_pin=current.level();const auto native_pin=authored_character_menu;
 const auto stack_pin=native->shared_stack_v27;const auto roster_pin=native->shared_roster_v27.get();const auto app_pin=provider->directory->application.lock();
 struct DeliveryV98 {bool active{true},successful{};};
 const auto delivery=std::make_shared<DeliveryV98>();
 struct EndDeliveryV98 {std::shared_ptr<DeliveryV98> receipt;~EndDeliveryV98(){receipt->active=false;}} end_delivery{delivery};
 auto fail=[&](){if(provider->loadmenu1_failure_v98.empty())provider->loadmenu1_failure_v98=e.empty()?"Actual primary1 source loading failed":e;e=provider->loadmenu1_failure_v98;return false;};
 //This gate may be retained by the SAME Panel. Engine captures are weak;
 //only the adapter's completed/inflight delivery receipt has independent storage.
 NativePrimary1ContinueV98 continuation=[delivery,wp=std::weak_ptr<NativeMenuPostMovieV62>(provider),
  wn=std::weak_ptr<NativeCharacterMenuV4>(native_pin),ww=std::weak_ptr<void>(world),
  wl=std::weak_ptr<dh2::loader::CanonicalLevelContextV1>(level_pin),
  ws=std::weak_ptr<dh2::ui::MenuStackOwnerV1>(stack_pin),roster_identity=reinterpret_cast<std::uintptr_t>(roster_pin),
  wa=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(app_pin)](std::string& e){
  auto p=wp.lock();auto native=wn.lock();auto world=ww.lock();auto level=wl.lock();auto stack=ws.lock();auto app=wa.lock();
  auto reject=[&](const char* reason){if(p){if(p->loadmenu1_failure_v98.empty())p->loadmenu1_failure_v98=e.empty()?reason:e;e=p->loadmenu1_failure_v98;}else if(e.empty())e=reason;return false;};
  if(p&&!p->loadmenu1_failure_v98.empty()){e=p->loadmenu1_failure_v98;return false;}
  if(!delivery->active&&!delivery->successful)return reject("Actual primary1 source delivery failed before completion");
  const auto callback_error=e;NativeCharacterMenuV4* live{};dh2::loader::CanonicalCurrentLevelBorrowV1 now;
  const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
  if(!p||!native||!world||!level||!stack||!roster_identity||!app||!same(source_postmovie_v62.lock(),p)||
   !p->directory->resolve(live,e)||live!=native.get()||!same(authored_character_menu,native)||
   !same(p->directory->current_world(),world)||!same(p->directory->application.lock(),app)||
   !same(native->shared_stack_v27,stack)||reinterpret_cast<std::uintptr_t>(native->shared_roster_v27.get())!=roster_identity||
   !same(p->loadmenu1_level_v98.lock(),level)||!p->loadmenu1_attempted_v98||
   !borrow_current_native_level_v27(now,e)||!same(now.level(),level)||
   (delivery->active&&level->constructor_fields_v3().field130!=12)){
   return reject("Primary1 callback changed actual process/campaign/Level loading scope");
  }e=callback_error;return true;
 };
 auto reached=[&](){return continuation(e);};
 auto same_movie=[&](std::uint32_t slot,const MenuMovieBorrowV58& expected){MenuMovieBorrowV58 now;
  const bool ok=provider->directory->movie_slot(slot,now,e);if(!reached())return false;
  // Front slots0/2 return a fresh pair<Impl,SwfMovie> pin on each borrow.
  // Its shared_ptr identity is the lease wrapper, not the movie; the stable
  // movie pointer in identity is sufficient because expected pins the old
  // resource, preventing pointer reuse. Primary1's identity is its RenderFX,
  // so also retain the stable source-movie lease check there.
  const bool same_primary_lease=slot!=1||
   (now.actual_owner.get()==expected.actual_owner.get()&&
    !now.actual_owner.owner_before(expected.actual_owner)&&!expected.actual_owner.owner_before(now.actual_owner));
  if(!ok||!expected.actual_owner||!now.actual_owner||now.identity!=expected.identity||!same_primary_lease){
   if(e.empty())e="Primary loading callback changed actual movie resource in slot "+std::to_string(slot);return fail();
  }return true;
 };
 if(!reached())return false;
 MenuMovieBorrowV58 old;const bool old_ok=provider->directory->movie_slot&&provider->directory->movie_slot(1,old,e);if(!reached())return false;
 if(!old_ok)return fail();
 //New campaign occurrence requires real Stage2 retirement, never only a reset of bookkeeping.
 if(earlier_attempt&&old.identity){e="Previous primary1 prefix remains live before new loading occurrence";return fail();}
 const auto services=provider->loadmenu1_services_v98;
 if(!services.owner||!services.facts||!services.load_primary_scoped){e="Required Main CharacterPanelSession scoped primary1 movie loader permitting HUD3NULL";return fail();}
 NativePrimaryMenuFactsV98 facts;const bool facts_ok=services.facts(facts,e);if(!reached())return false;
 if(!facts_ok||!facts.receiver||!facts.width_screen){if(e.empty())e="Required actual layout global owner/Width_Screen cell";return fail();}
 const char* uri="data/menus/dqcharmenu.swf";
 if(*facts.width_screen==854)uri="data/menus/dqcharmenu_droid.swf";
 else if(*facts.width_screen==800){
  if(!facts.htc_devices){e="Required actual HTC_DEVICES source byte";return fail();}
  if(!*facts.htc_devices){if(!facts.no_igp){e="Required actual NO_IGP source byte";return fail();}(void)*facts.no_igp;}
  //All three source800 Character1 rows select this SAME literal.
  uri="data/menus/dqcharmenu_i9000.swf";
 }
 const bool loaded=services.load_primary_scoped(uri,1,continuation,e);if(!reached())return false;if(!loaded)return fail();
 MenuMovieBorrowV58 primary;const bool primary_ok=provider->directory->movie_slot(1,primary,e);if(!reached())return false;
 if(!primary_ok||!primary.actual_owner||!primary.identity){if(e.empty())e="Primary1 Load did not publish a genuine retained resource";return fail();}
 {auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
 if(!panel||panel->source_stack_v4()!=native->shared_stack_v27.get()||panel->source_render_identity_v91()!=primary.identity||!panel->source_movie_v91()){e="Loaded primary1 differs from actual CharacterPanelSession/process directory";return fail();}}
 SwfMovie* primary_movie{};std::shared_ptr<void> primary_pin;
 const bool primary_borrowed=provider->directory->movie(1,primary,primary_movie,primary_pin,e);if(!reached())return false;
 if(!primary_borrowed||!primary_movie||!primary_pin||!same_movie(1,primary))return fail();
 //Multi.LoadSWF437df0 creates its SAME native paired camera after Load/text buffering.
 if(!facts.height_screen){e="Required actual Height_Screen cell for MenuFlash2DCamera C1";return fail();}
 if(native_pin->character_camera_v93){e="Primary1 camera exists before original post-Load camera C1 boundary";return fail();}
 native_pin->character_camera_v93=std::make_shared<MenuFlash2DCameraOwnerV93>(primary.identity,*facts.width_screen,*facts.height_screen);
 auto* render=stack_pin->render(primary.identity);if(!render){e="Required pre-Load actual RenderFX publication";return fail();}
 render->flags=0x84; //SetInputBehavior7a7c98 before original virtual10(1,false).
 //Deliver through the independently retained SAME movie; no raw panel survives native callback delivery.
 const bool initial_update=primary_movie->source_update_v93(1,false,e);if(!reached())return false;
 if(!initial_update||!same_movie(1,primary))return fail();
 //Historical name: this existing body implements common LoadMenu singleton
 //weak-context refresh, with no primary3-only operation or loading-state gate.
 const bool singletons=source_native_loadmenu3_singletons_v67(world,e);if(!reached())return false;
 if(!singletons||!same_movie(1,primary))return fail();
 const auto first=stack_pin->registry_count_v58();
 for(std::uint32_t slot=0;slot<4;++slot){MenuMovieBorrowV58 q;const bool slot_ok=provider->directory->movie_slot(slot,q,e);if(!reached())return false;if(!slot_ok)return fail();if(!q.identity)continue;
  SwfMovie* movie{};std::shared_ptr<void> lease;const bool borrowed=provider->directory->movie(slot,q,movie,lease,e);if(!reached())return false;
  if(!borrowed||!movie||!lease||!same_movie(slot,q))return fail();auto* r=stack_pin->render(q.identity);if(!r){e="PostLoad lacks SAME current primary render directory";return fail();}
  const auto flags=r->flags;const bool posted=roster_pin->post_load(*movie,q.identity,lease,flags,e);if(!reached())return false;
  if(!posted||!same_movie(slot,q)||!same_movie(1,primary))return fail();
 }
 for(auto i=first;i<stack_pin->registry_count_v58();++i){auto* q=stack_pin->registry_at_v58(i);
  if(!q||!roster_pin->receiver_fields_v59(q->identity)){e="New source menu lacks actual PostLoad receiver";return fail();}
  const auto id=q->identity;const bool dragged=roster_pin->register_drag_and_drops_v68(id,e);if(!reached())return false;
  if(!dragged||!same_movie(1,primary))return fail();
  q=stack_pin->registry_at_v58(i);if(!q||q->identity!=id||!roster_pin->receiver_fields_v59(id)){e="Drag callback retired/replaced appended actual menu";return fail();}
  const bool hidden=roster_pin->hide_receiver_v68(id,e);if(!reached())return false;
  if(!hidden||!same_movie(1,primary))return fail();
 }
 if(!reached())return false;delivery->successful=true;e.clear();return true; //Dispatcher alone commits12->13.
 }catch(const std::exception& x){
  if(provider){if(provider->loadmenu1_failure_v98.empty())provider->loadmenu1_failure_v98=std::string("Actual primary1 native delivery threw: ")+x.what();e=provider->loadmenu1_failure_v98;}
  else e=std::string("Actual primary1 native delivery threw: ")+x.what();return false;
 }catch(...){if(provider){if(provider->loadmenu1_failure_v98.empty())provider->loadmenu1_failure_v98="Actual primary1 native delivery threw";e=provider->loadmenu1_failure_v98;}else e="Actual primary1 native delivery threw";return false;}
}
