// Include after renderer_player_manager_boot_v25.inc and complete runtime
// definitions. No SourceCampaign/Menu/global file is replaced by this helper.
// Application owns the sole typed publication; Worlds borrow it weakly.
static bool prepare_source_application_player_manager_v59(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& application,
 std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& out,std::string& e){
 using Boot=dh2::player::ApplicationPlayerManagerBootstrapV59;
 if(!application){e="Required actual retained Application before PlayerManager";return false;}
 if(application->source_player_manager_v59()){
  out=application->source_player_manager_v59();
  if(!out->belongs_to_application(application)||!out->runtime()||!out->network()){e="Existing App PlayerManager owner differs/was invalidated";return false;}
  return true; // no C1/AddPlayer replay during GL/World recreation
 }
 auto prior=live_script_world?live_script_world->player_manager_boot_v25:nullptr;
 std::shared_ptr<Boot> created;
 if(prior&&prior->actual_manager){
  if(!prior->prepared||!prior->actual_network){e="Required actual produced legacy PM/network before App adoption";return false;}
  if(prior->transferred){
   if(!player_skills_runtime||player_skills_runtime->world!=live_script_world||
      player_skills_runtime->player_manager.get()!=prior->actual_manager||
      player_skills_runtime->player_network_local.get()!=prior->actual_network){
    e="Transferred legacy PM runtime owner changed before App adoption";return false;
   }
   auto& runtime=*player_skills_runtime;
   const auto expected=runtime.player_manager->source_services_v59();
   if(expected.context!=prior.get()||expected.invoke!=RendererPlayerManagerBootV25::service){
    e="Legacy PlayerManager provider no longer belongs to observed boot";return false;
   }
   if(!Boot::adopt_existing(application,runtime.player_manager,runtime.player_network_local,expected,prior,created,e))return false;
   // Preserve the existing actor's unique_ptr consumers and original660 until
   // whole RemoveCharacter executes. SAME objects are loaned back immediately;
   // their provider is now App/COnline, and return goes to App rather than World.
   if(!created->lend_to_runtime(runtime.player_manager,runtime.player_network_local,
       runtime.player_manager_boot_return_v25,e)){out=created;return false;}
  }else{
   if(prior->manager.get()!=prior->actual_manager||prior->network.get()!=prior->actual_network){
    e="Legacy boot ownership changed before App adoption";return false;
   }
   const auto expected=prior->manager->source_services_v59();
   if(expected.context!=prior.get()||expected.invoke!=RendererPlayerManagerBootV25::service){e="Unexpected previous boot provider";return false;}
   if(!Boot::adopt_existing(application,prior->manager,prior->network,expected,prior,created,e))return false;
  }
 }else{
  if(player_skills_runtime&&(player_skills_runtime->player_manager||player_skills_runtime->player_network_local)){
   e="Required exact existing PM/network owner adoption; fresh allocation would duplicate it";return false;
  }
  if(!Boot::create_fresh(application,source_matching_local_v4,created,e)){out=created;return false;}
 }
 out=created;
 return application->publish_source_player_manager_v59(std::move(created),e);
}
[[maybe_unused]] static bool bootstrap_source_first_local_player_v59(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& application,
 const dh2::player::FirstLocalControllerServicesV59& controllers,
 std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& out,std::string& e){
 if(!prepare_source_application_player_manager_v59(application,out,e))return false;
 return out->source_first_local_add_prefix(controllers,e);
}
