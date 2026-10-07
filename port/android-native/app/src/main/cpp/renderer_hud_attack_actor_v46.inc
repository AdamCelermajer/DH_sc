// Include after PlayerSkillsRuntime and its genuine attack owner. Public
// declarations belong to model_renderer.hpp (root applies them coherently).
bool source_hud_attack_actor_v46(std::int32_t index,bool include_dummy,
 dh2::ui::HudAttackActorBorrowV46& out,std::string& error){
 error.clear();
 if(!player_skills_runtime||!player_skills_runtime->player_manager){error="Required actual HUD PlayerManager";return false;}
 auto& t=*player_skills_runtime;dh2::player::PlayerInfoFieldsV1* record{};
 if(!t.player_manager->manager().get_local_player(index,include_dummy,record,error))return false;
 if(!record||!record->character660){out={};return true;}
 if(!t.world||!t.world->player_object||!t.world->player_canonical||record->character660!=t.world->player_object->identity){
  error="Required SAME local PlayerInfo Character HUD receiver";return false;
 }
 dh2::ui::HudAttackActorBorrowV46 actual{record->character660,t.attack_controller.controllable,
  &t.world->player_canonical->ooi14a4,t.world_touch?&t.world_touch->pending.click_target413:nullptr};
 out=actual;return true;
}
bool source_hud_attack_command_v46(std::uintptr_t controller,std::uintptr_t requested,std::string& error){
 error.clear();
 if(!player_skills_runtime||!player_skills_runtime->attack_owner){error="Required same source attack owner";return false;}
 auto& t=*player_skills_runtime;
 if(!controller||controller!=t.attack_controller.controllable){error="Required SAME HUD controller378 receiver";return false;}
 t.attack_controller.blocked=controller_global_blocked;
 t.attack_controller.locked=prince_state.controller_locked;
 t.attack_controller.forced=prince_controller_forced;
 t.error.clear();
 if(t.attack_owner->command(requested)){error=t.attack_owner->error();if(!t.error.empty())error+="; "+t.error;return false;}
 return true;
}
