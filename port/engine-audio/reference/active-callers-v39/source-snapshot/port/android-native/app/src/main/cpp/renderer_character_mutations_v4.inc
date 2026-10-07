// Include after renderer_player_character_panel_v3.inc in model_renderer.
// Call supplement_character_menu_mutations_v4(out,error) as the final bind
// step in connect_character_panel_runtime_impl, preserving existing queries.
static bool supplement_character_menu_mutations_v4(
 dh2::android_ui::CharacterPanelGameplayServicesV2& out,std::string& error){
 if(!player_skills_runtime||!player_skills_runtime->player_save_load||!out.owner){
  error="Required same retained Save profile authority for authored mutations";return false;
 }
 auto preceding=out.queries;
 out.queries=[preceding](const auto& p,auto& actions,auto& graph,auto& e){
  if(preceding&&!preceding(p,actions,graph,e))return false;
  auto* t=player_skills_runtime.get();
  if(!t||t->world!=p.world_owner||t->player.get()!=p.skills||!t->player_save_load||
    &t->player_save_load->save()!=p.save||player_equipment.get()!=p.gear){e="Authored mutation runtime replaced";return false;}
  dh2::character::CharacterMenuMutationServicesV4 services;services.owner=t->world;
  services.constant=[](const char* group,const char* key,auto& value,auto& error){
   if(!player_skills_runtime||!player_skills_runtime->world||!player_skills_runtime->world->design){error="Required same menu GameDesign";return false;}
   auto actual=player_skills_runtime->world->design->borrow();const auto* d=actual.design();
   if(!d||!d->lookup||d->lookup(d->context,0,group,key,&value)!=0){error="Required menu source constant "+std::string(group)+"."+key;return false;}return true;
  };
  services.is_player=[](std::uintptr_t character,bool& value,auto& error){
   if(!player_skills_runtime||player_skills_runtime->fsm.character!=character||!player_skills_runtime->player){error="Menu IsPlayer requires same selected Character";return false;}
   std::uint32_t source{};if(player_skills_runtime->player->session().source_is_player(source)!=0){error=player_skills_runtime->player->session().error();return false;}value=source!=0;return true;
  };
  services.is_local_player=[](std::uintptr_t character,bool& value,auto& error){
   if(!player_skills_runtime){error="Menu IsLocalPlayer requires same runtime";return false;}
   return player_network_is_local_v4(*player_skills_runtime,character,value,error);
  };
  services.online=[](bool& value,auto& error){
   if(!equipment_platform||!player_skills_runtime||equipment_platform->world!=player_skills_runtime->world){error="Required same live menu GetOnline";return false;}
   value=equipment_platform->online_mode;return true;
  };
  services.achievement=[](std::uintptr_t character,const char* key,auto& error){
   if(!player_skills_runtime||player_skills_runtime->fsm.character!=character||!player_skills_runtime->trophy_bindings){error="Required same native trophy manager";return false;}
   const int status=player_skills_runtime->trophy_bindings->unlock_named(key);
   if(status!=0){error=player_skills_runtime->trophies->error();if(error.empty())error="Required reached menu trophy continuation";return false;}return true;
  };
  services.save.owner=t->world;services.save.invoke=[](const auto& q,auto&,auto& error){
   error="Required actual campaign Save writer continuation "+std::to_string(unsigned(q.operation));return false;
  };
  // No drop packet/world callback is invented. Source item transfer preserves
  // its actual prefix if these required production endpoints are reached.
  auto save=std::shared_ptr<dh2::data::PlayerSaveLoadOwnerV1>(t->world,t->player_save_load.get());
  return dh2::character::bind_character_menu_mutations_v4(actions,graph,*t->player,
   std::move(save),std::move(services),e);
 };
 error.clear();return true;
}
