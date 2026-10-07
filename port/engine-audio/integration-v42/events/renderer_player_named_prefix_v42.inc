// Staged private include after renderer_animation_sound_v4.inc.
static int source_player_named_prefix_v42(PlayerSkillsRuntime& t,const char* name,bool& handled){
 handled=name&&!std::strncmp(name,"sfx_",4);if(!handled)return 0;
 using namespace dh2::character::skills;
 MeleeAnimationServicesV1 services{&t,[](void* raw,const MeleeAnimationRequestV1* q,MeleeAnimationResponseV1* out){
  auto& r=*static_cast<PlayerSkillsRuntime*>(raw);if(!q||!out)return -1;
  if(q->operation==melee_event_step_index){out->value=prince_locomotion.step_index();return 0;}
  if(q->operation==melee_event_step_count){out->value=prince_locomotion.step_count(actor_animation_tables);return 0;}
  if(q->operation==melee_event_sound_fx)return source_named_animation_sound_v38(r,r.fsm.character,q->text);
  r.error="Required actual authored event receiver";return -1;
 },nullptr};
 MeleeAnimationOutputV1 output{};
 return dh2_character_melee_animation_event_v1(&output,name,&services);
}
