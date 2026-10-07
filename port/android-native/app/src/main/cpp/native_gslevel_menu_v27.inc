// Include after native_character_menu_v4.inc. Root header declaration:
// borrow_native_gs_menu_v27(world, loading, actual_debug, services, error).
bool model_renderer::unload_native_menu_resource_v91(std::shared_ptr<void> world,
 std::uintptr_t id,dh2::ui::SwfMovie& movie,std::string& error){
 std::shared_ptr<void> manager;if(!borrow_native_menu_manager_v58(world,manager,error))return false;
 auto* stack=authored_character_menu->shared_stack_v27.get();
 if(!id||!stack->render(id)){error="Required SAME actual owned MenuFX render";return false;}
 if(id!=reinterpret_cast<std::uintptr_t>(&movie)){
  auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
  if(!panel||panel->source_stack_v4()!=stack||panel->source_render_identity_v91()!=id||
     panel->source_movie_v91()!=&movie){
   error="MenuFX resource facade differs from SAME owned render";return false;
  }
 }
 dh2::ui::SwfMenuUnloadServicesV91 services;services.owner=manager;
 // Original GOT99844c points to gameswf::s_render_handler (BSS9fc994).
 // Query the ACTUAL global even outside a facade Scope; never infer nullptr
 // from a missing service. Scope restores that same global before this call.
 services.renderer=dh2::ui::swf_actual_renderer_borrow_v91;
 services.renderer_virtual_a4=[](auto,auto& e){e="Required actual nonnull extended GameSWF renderer virtualA4 unload";return false;};
 services.clear_render_context_and_flags=[stack,id](auto& e){return stack->clear_render_fields_v91(id,e);};
 services.clear_catalog_104=[stack,id](auto& e){return stack->clear_catalog_104_v91(id,e);};
 services.clear_active_states_114=[stack,id](auto& e){return stack->clear_active_states_114_v91(id,e);};
 return movie.unload_menu_resource_v91(services,error);
}
bool model_renderer::borrow_native_menu_manager_v58(std::shared_ptr<void> world,
 std::shared_ptr<void>& out,std::string& error){
 out.reset();
 if(!authored_character_menu||!authored_character_menu->shared_stack_v27||
    (world?(authored_character_menu->leases->world!=world||!authored_character_menu->bootstrap_complete_v27):
     (!authored_character_menu->process_directory_ready_v104||authored_character_menu->process_application_v104.lock()!=application_services_v5))){
  error="Required same live bootstrapped World/MenuManager";return false;
 }
 out=authored_character_menu->shared_stack_v27;return true;
}
bool model_renderer::bind_native_menu_lifecycle_v58(std::shared_ptr<void> world,
 dh2::ui::MenuManagerUnloadServicesV58 unload,
 dh2::ui::MenuManagerUpdateServicesV58 update,std::string& error){
 std::shared_ptr<void> actual;if(!borrow_native_menu_manager_v58(world,actual,error))return false;
 const auto same=[&](const auto& owner){return owner&&owner.get()==actual.get()&&
  !owner.owner_before(actual)&&!actual.owner_before(owner);};
 if(!same(unload.actual_manager)||!same(update.actual_manager)){
  error="Native menu lifecycle requires SAME published MenuManager owner";return false;
 }
 if(authored_character_menu->source_unload_v58||authored_character_menu->source_update_v58){
  error="Native source menu lifecycle already bound; no frame-time replacement";return false;
 }
 // Supply the actual shared directory and registered receiver calls here.
 // Resource/PM/Debug providers remain with their real platform owners. Flash
 // timing and lifetime below use the existing process backend and GS global.
 const auto weak=std::weak_ptr<void>(world);
 const auto native_weak=std::weak_ptr<NativeCharacterMenuV4>(authored_character_menu);
 const auto manager_weak=std::weak_ptr<dh2::ui::MenuStackOwnerV1>(authored_character_menu->shared_stack_v27);
 auto stack=[weak,native_weak,manager_weak](std::string& e)->dh2::ui::MenuStackOwnerV1*{
  auto native=native_weak.lock();auto expected=manager_weak.lock();
  if(!native||native!=authored_character_menu||!expected||native->shared_stack_v27!=expected){e="Source menu callback lost its actual manager/context";return nullptr;}
  auto world=weak.lock();std::shared_ptr<void> actual;
  if(!model_renderer::borrow_native_menu_manager_v58(world,actual,e))return nullptr;
  return authored_character_menu->shared_stack_v27.get();
 };
 auto fields=[stack](std::uintptr_t id,std::string& e)->dh2::ui::AuthoredMenuFieldsV1*{
  auto* directory=stack(e);if(!directory)return nullptr;
  auto* f=authored_character_menu->shared_roster_v27->receiver_fields_v59(id);
  if(f)return f;
  if(auto* debug=model_renderer::native_debug_menu_fields_v62(id))return debug;
  if(auto* map=model_renderer::native_map_menu_fields_v62(id))return map;
  if(auto* derived=model_renderer::native_derived_menu_fields_v67(id))return derived;
  auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
  if(panel&&panel->source_stack_v4()==directory)f=panel->receiver_fields_v59(id);
  if(!f)e="Required SAME live registered MenuBase source fields";return f;
 };
 unload.registry_count=[stack](auto& out,auto& e){auto* s=stack(e);if(!s)return false;out=s->registry_count_v58();return true;};
 unload.registry_at=[stack,fields](auto index,auto& out,auto& e){auto* s=stack(e);if(!s)return false;
  auto* projected=s->registry_at_v58(index);if(!projected){e="Required live registry receiver at reached index";return false;}
  auto* f=fields(projected->identity,e);if(!f)return false;
  out={f->identity,f->render,f->owned7d};return true;
 };
 unload.clear_render=[stack,fields](auto id,auto& e){auto* s=stack(e);auto* f=fields(id,e);if(!s||!f)return false;
  auto* projected=s->registered_menu_v27(id);
  if(!projected||f->render!=(projected->render?projected->render->identity:0)){e="MenuBase render differs from SAME registry projection";return false;}
  f->render=0;projected->render=nullptr;return true;
 };
 unload.read_owned7d=[fields](auto id,auto& out,auto& e){auto* f=fields(id,e);if(!f)return false;out=f->owned7d;return true;};
 unload.deleting_virtual4=[stack](auto id,auto& e){auto* s=stack(e);if(!s)return false;
  if(authored_character_menu->shared_roster_v27->receiver_fields_v59(id))
   return authored_character_menu->shared_roster_v27->delete_receiver_v59(id,e);
  auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
  if(!panel||panel->source_stack_v4()!=s){e="Required SAME actual deleting MenuBase owner";return false;}
  return panel->delete_receiver_v59(id,e);
 };
 unload.erase_registry=[stack](auto index,auto id,auto& e){auto* s=stack(e);return s&&s->erase_registry_v58(index,id,e);};
 update.get_num_menus=[stack](auto& out,auto& e){auto* s=stack(e);if(!s)return false;
  const auto bits=s->registry_count_v58();std::memcpy(&out,&bits,sizeof(bits));return true;};
 update.registry_at=[stack](auto index,auto& out,auto& e){auto* s=stack(e);if(!s)return false;
  auto* projected=s->registry_at_v58(index);if(!projected){e="Required live registry receiver at reached update index";return false;}out=projected->identity;return true;};
 update.menu_is_visible=[fields](auto id,bool& out,auto& e){auto* f=fields(id,e);if(!f)return false;out=f->visible74!=0;return true;};
 update.menu_virtual1c=[stack](auto id,auto& e){auto* s=stack(e);if(!s)return false;
  if(model_renderer::native_map_menu_fields_v62(id))return model_renderer::native_map_menu_update_v62(id,e);
  if(model_renderer::native_derived_menu_fields_v67(id))return model_renderer::native_derived_menu_update_v67(id,e);
  if(authored_character_menu->shared_roster_v27->receiver_fields_v59(id))
   return authored_character_menu->shared_roster_v27->update_receiver_v59(id,e);
  auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
  if(!panel||panel->source_stack_v4()!=s){e="Required SAME actual MenuBase Update owner";return false;}
  return panel->update_receiver_v59(id,e);
 };
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 if(!model_renderer::borrow_actual_application_services_v5(application,error))return false;
 struct FlashFrameBorrowV93 {dh2::loader::CanonicalCurrentLevelBorrowV1 current;};
 auto frame=std::make_shared<FlashFrameBorrowV93>();
 auto flash=original_ui.flash_anim_manager_v92();
 if(!flash){error="Required actual process Flash animation owner";return false;}
 dh2::ui::FlashAnimApplicationServicesV92 clock;clock.actual_application=application;
 clock.current_level=[frame](auto& out,auto& e){
  if(!model_renderer::borrow_current_native_level_v27(frame->current,e))return false;
  out=frame->current.identity();return true;
 };
 clock.level_phase=[frame](auto id,auto& out,auto& e){
  const auto& level=frame->current.level();
  if(!level||level->identity()!=id){e="Flash phase query differs from SAME borrowed current Level";return false;}
  dh2::loader::CanonicalLevelContextV1::LoadingFieldsV26 fields;
  if(!level->loading_fields_v26(fields,e)||!fields.state130)return false;
  const auto bits=*fields.state130;std::memcpy(&out,&bits,sizeof(out));return true;
 };
 clock.application_dt=model_renderer::borrow_application_dt_v93;
 // A duplicate scanner's assertion branch remains required until the real
 // process assertion policy is supplied. Ordinary reset/reload never infers it.
 if(!flash->claim_menu_update(actual,std::move(clock),error))return false;
 authored_character_menu->source_flash_v93=flash; // retain reached bind prefix
 unload.reset_scan_for_anims=[flash](const auto& movie,auto& e){
  return flash->reset_scan_for_anims(movie.identity,e);
 };
 update.flash_anim_update=[flash,frame](auto& e){
  // Pin the original current Level only for this synchronous update. Keeping
  // the borrow in a process manager across frames would delay Level teardown.
  struct Release {std::shared_ptr<FlashFrameBorrowV93> frame;~Release(){frame->current={};}} release{frame};
  return flash->update(e);
 };
 update.current_level=[](auto& out,auto& e){
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;
  if(!model_renderer::borrow_current_native_level_v27(current,e))return false;
  out={};if(!current){e.clear();return true;}
  out.actual_owner=current.level();out.identity=current.identity();
  out.byte198=&current.level()->constructor_fields_v3().byte198;return true;
 };
 if(!model_renderer::bind_native_menu_prefix_v62(world,update,error))return false;
 auto unloading=std::make_unique<dh2::ui::MenuManagerUnloadV58>(std::move(unload));
 auto updating=std::make_unique<dh2::ui::MenuManagerUpdateV58>(std::move(update));
 authored_character_menu->source_unload_v58=std::move(unloading);
 authored_character_menu->source_update_v58=std::move(updating);return true;
}
bool model_renderer::borrow_native_script_menu_v62(std::shared_ptr<void> world,const char* name,
 std::shared_ptr<void>& receiver,std::uintptr_t& identity,std::string& error){
 receiver.reset();identity=0;
 std::shared_ptr<void> manager;
 if(!name||!borrow_native_menu_manager_v58(world,manager,error)){
  if(error.empty())error="Required actual Script menu name/manager";return false;
 }
 const auto owner=authored_character_menu;
 auto* menu=owner->shared_stack_v27->menu(name);
 if(!menu){error.clear();return true;}
 if(!owner->shared_roster_v27->receiver_fields_v59(menu->identity)){
  error="Script menu receiver needs its actual non-roster lifetime owner";return false;
 }
 identity=menu->identity;
 receiver=std::shared_ptr<void>(owner,reinterpret_cast<void*>(identity));
 error.clear();return true;
}
bool model_renderer::unload_native_menu_v58(std::shared_ptr<void> world,
 std::int32_t id,std::string& error){
 std::shared_ptr<void> actual;if(!borrow_native_menu_manager_v58(world,actual,error))return false;
 if(!authored_character_menu->source_unload_v58){error="Required actual source menu unload providers";return false;}
 return authored_character_menu->source_unload_v58->unload(id,error);
}
bool model_renderer::borrow_native_gs_frame_menu_v58(std::shared_ptr<void> world,
 dh2::loader::GSLevelUpdateServicesV44<dh2::loader::CanonicalLevelContextV1>& services,
 std::string& error){
 std::shared_ptr<void> actual;if(!borrow_native_menu_manager_v58(world,actual,error))return false;
 if(!prepare_native_menu_lifecycle_v93(world,error))return false;
 const auto weak=std::weak_ptr<void>(world);
 services.menu_instance=[weak](auto& out,auto& e){out={};auto world=weak.lock();std::shared_ptr<void> manager;
  if(!model_renderer::borrow_native_menu_manager_v58(world,manager,e))return false;
  out.actual_owner=manager;out.identity=reinterpret_cast<std::uintptr_t>(manager.get());return true;};
 services.menu_update=[weak](const auto& borrowed,bool hud_only,auto& e){
  auto world=weak.lock();std::shared_ptr<void> actual;
  if(!model_renderer::borrow_native_menu_manager_v58(world,actual,e))return false;
  if(!borrowed.actual_owner||borrowed.actual_owner.get()!=actual.get()||
     borrowed.identity!=reinterpret_cast<std::uintptr_t>(actual.get())||
     borrowed.actual_owner.owner_before(actual)||actual.owner_before(borrowed.actual_owner)){
   e="GS frame menu receiver differs from SAME live MenuManager";return false;
  }
  if(!authored_character_menu->source_update_v58){e="Required actual source menu update providers";return false;}
  return authored_character_menu->source_update_v58->update(hud_only,e);
 };
 return true;
}
bool model_renderer::bind_native_menu_application_ec_v58(std::shared_ptr<void> world,
 std::shared_ptr<void> application,std::uint8_t* ec,std::string& error){
 if(!world||!authored_character_menu||authored_character_menu->leases->world!=world||
    !authored_character_menu->bootstrap_complete_v27){
  error="Required same bootstrapped World/MenuManager for Application+ec";return false;
 }
 return authored_character_menu->fields.bind_application_ec_v58(std::move(application),ec,error);
}
bool model_renderer::borrow_native_gs_menu_v27(std::shared_ptr<void> world,
 const dh2::ui::LoadingMenuStateServicesV1& loading,
 std::function<bool(const char*,std::int32_t&,std::string&)> actual_debug,
 dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1>& services,
 std::string& error){
 if(!world||!character_panel){error="Required actual World/native UI session before GS loading";return false;}
 if(authored_character_menu&&authored_character_menu->leases->world!=world&&!authored_character_menu->process_directory_ready_v104){
  if(!authored_character_menu->release_flash_for_teardown_v93(error))return false;
  character_panel->reset_authored_v4();authored_character_menu.reset();
 }
 if(!authored_character_menu)authored_character_menu=std::make_unique<NativeCharacterMenuV4>();
 // GS construction precedes the player, but PostLoad needs the actual HUD
 // movie. Load its CPU graph without selecting the health inspection view.
 dh2::ui::SwfMovie* hud{};std::shared_ptr<void> hud_lease;
 if(!original_ui.hud_movie_borrow_v4(hud,hud_lease,error)){
  if(front_directory.empty()){error="Required actual private directory for shared HUD PostLoad";return false;}
  if(!original_ui.load_health_panel(front_directory,error))return false;
  original_ui.deactivate();
 }
 front_ui.bind_loading_state_services(loading);
 if(!authored_character_menu->bootstrap_loading_v27(world,std::move(actual_debug),error))return false;
 const auto weak=std::weak_ptr<void>(world);
 auto menu=[weak](std::uintptr_t identity,dh2::ui::MenuStackMenuV1*& out,std::string& e){
  const auto actual=weak.lock();out=nullptr;
  if(!actual||!authored_character_menu||authored_character_menu->leases->world!=actual||!authored_character_menu->bootstrap_complete_v27){e="Required same live GS MenuManager/World";return false;}
  out=authored_character_menu->shared_stack_v27->registered_menu_v27(identity);
  if(!out){e="Required actual registered GS MenuBase receiver";return false;}return true;
 };
 services.get_menu=[weak](const char* name,auto& out,auto& e){auto actual=weak.lock();
  if(!actual||!authored_character_menu||authored_character_menu->leases->world!=actual||!authored_character_menu->bootstrap_complete_v27){e="Required actual GS MenuManager name lookup";return false;}
  auto* found=authored_character_menu->shared_stack_v27->menu(name);out=found?found->identity:0;return true;
 };
 services.overlay_contains_menu=[menu](auto id,bool& contains,auto& e){dh2::ui::MenuStackMenuV1* actual{};if(!menu(id,actual,e))return false;std::uint32_t value{};
  if(dh2_menu_stack_contains_v1(authored_character_menu->shared_stack_v27->view(),actual,&value)){e="Actual GS overlay membership query failed";return false;}contains=value!=0;return true;
 };
 services.push_menu=[menu](auto id,auto& e){dh2::ui::MenuStackMenuV1* actual{};if(!menu(id,actual,e))return false;
  if(authored_character_menu->shared_stack_v27->push(actual->name,authored_character_menu->shared_roster_v27->stack_services())){e=authored_character_menu->shared_roster_v27->error();return false;}return true;
 };
 services.menu_render_fx=[menu](auto id,auto& out,auto& e){dh2::ui::MenuStackMenuV1* actual{};if(!menu(id,actual,e)||!actual->render)return false;out=actual->render->identity;return true;};
 services.check_menu_weak_proxy=[menu](auto id,auto& e){dh2::ui::MenuStackMenuV1* actual{};return menu(id,actual,e)&&authored_character_menu->shared_roster_v27->check(id,e);};
 services.menu_character=[menu](auto id,auto& out,auto& e){dh2::ui::MenuStackMenuV1* actual{};if(!menu(id,actual,e)||!actual->character||!actual->character->live){e="Required live actual GS menu character";return false;}out=actual->character->identity;return true;};
 services.invoke_as_no_arguments=[weak](auto render,auto character,const char* method,auto& e){auto actual=weak.lock();
  if(!actual||!authored_character_menu||authored_character_menu->leases->world!=actual){e="Expired actual GS RenderFX receiver";return false;}
  auto* stack=authored_character_menu->shared_stack_v27->view();
  for(std::uint32_t i=0;i<stack->registry_count;++i){auto* receiver=stack->registry[i];if(receiver->render&&receiver->render->identity==render&&receiver->character&&receiver->character->identity==character)
    return authored_character_menu->shared_roster_v27->invoke(receiver->identity,method,e);}
  e="Required SAME GS RenderFX/menu character invocation";return false;
 };
 services.menu_virtual10=[menu](auto id,auto& e){dh2::ui::MenuStackMenuV1* actual{};if(!menu(id,actual,e))return false;
  dh2::ui::MenuStackRequestV1 request{};request.operation=dh2::ui::MenuStackOperationV1::menu_hide;request.menu=actual;request.render=actual->render;bool handled{};
  return authored_character_menu->shared_roster_v27->route(*authored_character_menu->shared_stack_v27->view(),request,handled,e)&&handled; // original MenuBase.Hide424af4 vslot10
 };
 return true;
}
