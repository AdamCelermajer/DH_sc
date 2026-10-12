// Include after native_character_menu_v4.inc. Root header declaration:
// borrow_native_gs_menu_v27(world, loading, actual_debug, services, error).
bool model_renderer::borrow_native_gs_menu_v27(std::shared_ptr<void> world,
 const dh2::ui::LoadingMenuStateServicesV1& loading,
 std::function<bool(const char*,std::int32_t&,std::string&)> actual_debug,
 dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1>& services,
 std::string& error){
 if(!world||!character_panel){error="Required actual World/native UI session before GS loading";return false;}
 if(authored_character_menu&&authored_character_menu->leases->world!=world){
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
