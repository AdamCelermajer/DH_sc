// Include after the attack command/animation helpers. Declarations and retained
// member requirements are described in renderer_player_melee_event_v1_handoff.md.
void PlayerSkillsRuntime::bind_melee_event(PlayerSkillsRuntime& t){
 using namespace dh2::character::skills;
 if(!t.execution||!t.targets||!t.attack_geometry||!t.world||!t.world->player_object||
    !player_equipment||!player_equipment->inventory())throw std::runtime_error("Required same-world melee event owners");
 auto& object=*t.world->player_object;
 t.melee_on_attack=t.ais_virtuals[0xa8/4];
 t.melee_event=std::make_unique<CharacterWorldMeleeAttackV1>(t.execution->combat(),*t.targets,
  t.combat_context,combat_random,
  WorldMeleeAttackBorrowV1{object.identity,&object.target.target,&object.target.target,
   &t.events.active,&t.melee_on_attack,player_equipment->inventory()},
  t.attack_geometry->queries(),WorldMeleeAttackBackendsV1{});
}
int PlayerSkillsRuntime::melee_event_service(void* raw,
 const dh2::character::skills::MeleeAnimationRequestV1* q,
 dh2::character::skills::MeleeAnimationResponseV1* out){
 using namespace dh2::character;using namespace dh2::character::skills;
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);if(!q||!out||!t.world||!t.world->player_object)return -1;
 switch(q->operation){
 case melee_event_step_index:out->value=prince_locomotion.step_index();return 0;
 case melee_event_step_count:out->value=prince_locomotion.step_count(actor_animation_tables);return 0;
 case melee_event_relay:
  if(t.animation_events&&t.animation_events->relay(q->text))return 0;
  t.error=t.animation_events?t.animation_events->error():"Required original AIS animation-event owner";return -1;
 case melee_event_state:out->value=prince_state.current;return 0;
 case melee_event_can_range:{
  std::int32_t result=0;
  if(!t.attack_geometry||!t.attack_geometry->read(t.world->player_object->identity,
      t.world->player_object->target.target,WorldAIAttackQueryV1::CharacterCanRangeAttack,result,t.error))return -1;
  out->value=result;
  // Original CanRangeAttack also writes the projectile row. A true result
  // requires the full projectile-output adapter before spawn is available.
  if(result){t.error="Required source CanRangeAttack projectile output borrow";return -1;}
  return 0;
 }
 case melee_event_attack:{
  if(!t.melee_event){t.error="Required original melee event owner";return -1;}
  t.melee_on_attack=t.ais_virtuals[0xa8/4];
  const int result=t.melee_event->attack(t.attack_fields.index,q->step,q->offhand);
  t.execution->publish_compatibility_fields();
  if(result)t.error=t.melee_event->error();return result;
 }
 case melee_event_skill:{std::uint32_t result=0;return t.player->skill_ai(skill_ai_event_v3,0,&result);}
 default:t.error="Required original authored animation event provider "+std::to_string(q->operation)+" text "+(q->text?q->text:"");return -1;
 }
}
int PlayerSkillsRuntime::authored_melee_event(const char* name){
 using namespace dh2::character::skills;
 if(!execution)return -1;error.clear();
 MeleeAnimationOutputV1 output{};
 const MeleeAnimationServicesV1 services{this,melee_event_service,execution->combat().application_services().debug};
 const int result=dh2_character_melee_animation_event_v1(&output,name,&services);
 __android_log_print(result?ANDROID_LOG_ERROR:ANDROID_LOG_INFO,"DH2Native",
  "Original authored attack event | name %s | state %d | step %d | phase %u | status %d | target %016llx | amount %d | apply phase %u | same World/Gear/HP/RNG | %s",
  name,output.state,output.step,output.phase,result,
  static_cast<unsigned long long>(world->player_object->target.target),
  melee_event?melee_event->result().amount:0,melee_event?melee_event->application().phase:0,error.c_str());
 return result;
}
