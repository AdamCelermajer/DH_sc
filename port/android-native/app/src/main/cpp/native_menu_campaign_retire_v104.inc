//Include after native_menu_postmovie_v62.inc in native_app global scope.
bool model_renderer::retire_native_menu_campaign_v104(const std::shared_ptr<void>& world,std::string& e){
 if(!world||!authored_character_menu){e="Required actual old World/process MenuManager for final detach";return false;}
 auto& native=*authored_character_menu;
 auto provider=source_postmovie_v62.lock();
 if(!native.process_directory_ready_v104||!native.shared_stack_v27||!native.shared_roster_v27||
    !provider||!provider->directory||!provider->directory->process_scope_v104){
  e="Final campaign detach requires actual retained process menu ownership";return false;
 }
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 //Pending retirement may be delivered again. The NULL lease means this SAME
 //process directory is already detached; do not touch a subsequently attached World.
 if(!native.leases->world){
  if(!same(native.retired_campaign_v104.lock(),world)){e="No matching prior campaign detach receipt on process manager";return false;}
  e.clear();return true;
 }
 NativeCharacterMenuV4* resolved{};
 if(!same(native.leases->world,world)||!provider->directory->resolve(resolved,e)||resolved!=&native){
  if(e.empty())e="Menu final detach addressed another actual World/manager";return false;
 }
 //Root's retirement admission has already completed GS/Level destruction
 //and cleared34/s_level. Never borrow a destroyed Level here.
 //Preflight all alias holders before committing; no old callback runs here.
 if(!original_ui.can_retire_hud_campaign_v104(world,provider,e)||
    (character_panel&&!character_panel->can_retire_campaign_v104(world,e)))return false;
 if(character_panel&&!character_panel->retire_campaign_v104(world,e))return false;
 if(!original_ui.retire_hud_campaign_v104(world,provider,e))return false;
 provider->world_operations={};provider->canonical_controls={};provider->world_transport.reset();provider->world_bound=false;
 provider->loadmenu3_attempted=false;provider->loadmenu3_failure.clear();
 provider->loadmenu1_attempted_v98=false;provider->loadmenu1_failure_v98.clear();provider->loadmenu1_level_v98.reset();
 provider->directory->world.reset();
 //Process transport remains retained in OriginalUI. Its world functions are
 //installed again only by the next real bind_native_menu_hud_world_v62.
 native.retired_campaign_v104=world;
 native.gameplay_bound_world_v109.reset();native.gameplay_bound_character_v109=0;
 front_ui.bind_campaign_language_scene_v109({},{});
 //Return to the exact pre-World process Debug provider; retaining the old
 //World-specific callback would poison the subsequent Main menu/retry flow.
 if(native.process_debug_v115)native.bootstrap_debug_v27=native.process_debug_v115;
 native.leases->world.reset(); //Last: nextWorld can now attach to SAME directory.
 e.clear();return true;
}
