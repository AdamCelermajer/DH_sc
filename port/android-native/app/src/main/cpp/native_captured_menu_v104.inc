bool model_renderer::capture_native_process_menu_v104(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const char* name,dh2::ui::CapturedMenuLeaseV101& out,std::string& e){
 out={};auto native=authored_character_menu;
 if(!app||app!=application_services_v5||!name||!native||!native->shared_stack_v27||!native->shared_roster_v27||
    (!native->process_directory_ready_v104&&!native->bootstrap_complete_v27)){
  e="Required actual Application/MenuManager captured receiver directory";return false;
 }
 auto* menu=native->shared_stack_v27->menu(name);
 if(!menu){e.clear();return true;}
 std::shared_ptr<void> receiver;
 std::function<bool(std::uintptr_t,bool&,std::string&)> visible;
 if(!native->shared_roster_v27->capture_receiver_v104(menu->identity,receiver,visible,e))return false;
 // Native owns the real stack callback context; receiver owns the exact
 // MenuBase storage. Adapter menu/render projections have their own leases.
 return dh2::ui::capture_menu_lease_v101(native->shared_stack_v27,name,std::move(receiver),native,
  native->shared_roster_v27->stack_services(),std::move(visible),out,e);
}
