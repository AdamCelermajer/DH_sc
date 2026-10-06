// Same retained AI target, supplemental fields and original playback session.
int PlayerSkillsRuntime::attack_animation_service(void* raw,
 dh2::character::AttackState64& ai,
 const dh2::character::AttackAnimationRequestV1& q,std::uint32_t& out){
 using namespace dh2::character;
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 if(&ai!=&t.attack_fields||!t.world||!t.world->player_object||!t.attack_geometry)return -1;
 auto& object=*t.world->player_object;auto& target=object.target;out=0;
 switch(q.service){
 case attack_anim_step_index_v1:out=prince_locomotion.step_index();return 0;
 case attack_anim_step_count_v1:out=prince_locomotion.step_count(actor_animation_tables);return 0;
 case attack_anim_has_combo_v1:{
  int table=t.player->session().property_view().resolved[2];
  if(table<0||std::size_t(table)>=actor_animation_tables.characters.size())table=17;
  if(std::size_t(table)>=actor_animation_tables.characters.size())return -1;
  const auto& field=actor_animation_tables.characters[table].fields[1];
  if(field.size()!=1)return -1;const auto sequence=field.front();
  out=sequence>=0&&std::size_t(sequence)<actor_animation_tables.sequences.size()&&
      actor_animation_tables.sequences[sequence].type==1;return 0;
 }
 case attack_anim_raise_v1:{
  t.event_owner.forced=prince_controller_forced;t.event_owner.locked=prince_state.controller_locked;
  t.events.global_blocked=controller_global_blocked;
  const AIEventPayload24 payload{q.subject,0,0,0};AIEventResult16 result{};
  const AIEventServices24 services{&t,PlayerSkillsRuntime::event_service,63,0};
  return dh2_character_ai_event(&result,&t.events,q.value,&payload,&services)?-1:0;
 }
 case attack_anim_controller_look_v1:{
  const ControllerCommandState32 controller{reinterpret_cast<std::uintptr_t>(&prince_runtime.controller),object.identity,
    controller_global_blocked,prince_state.controller_locked,prince_controller_forced,0};
  const CharacterControlServices16 services{nullptr,prince_look_service};
  return dh2_character_controller_character(&controller,controller_look_object,q.subject,&services)==1?0:-1;
 }
 case attack_anim_pre_attack_v1:{
  bool can=false;
  if(!world_ai_can_attack_v1(object.identity,0,target.target,t.attack_geometry->queries(),can,t.error))return -1;
  // CharAI::OnPreAttack calls AI_CanAttack before selected AIS virtual +a4.
  // AISPlayerIPhone inherits the verified AISDefault bx-lr body.
  if(can&&t.events.active&&t.ais_virtuals[0xa4/4]!=0x3dbef8){t.error="Required selected AIS OnPreAttack body";return -1;}
  return 0;
 }
 case attack_anim_clear_nonsticky_v1:
  if(!t.attack_sticky){
   if(dh2_character_clear_target(&target,&object.binding.services))return -1;
   ai.target=target.target;ai.last_target=target.last_target;
  }
  return 0;
 case attack_anim_set_step_v1:prince_locomotion.set_step(q.value);return 0;
 case attack_anim_skip_next_v1:prince_locomotion.skip_next_step();return 0;
 case attack_anim_can_range_v1:{
  std::int32_t value=0;
  if(!t.attack_geometry->read(object.identity,target.target,WorldAIAttackQueryV1::CharacterCanRangeAttack,value,t.error))return -1;
  out=std::uint32_t(value);return 0;
 }
 case attack_anim_target_dead_v1:{
  skills::WorldTargetActorBorrowV1 actor{};
  if(t.targets->actor(q.subject,&actor)||!actor.life)return -1;
  out=actor.life->dead;ai.target=target.target;return 0;
 }
 default:t.error="Required source attack animator service "+std::to_string(q.service);return -1;
 }
}
int PlayerSkillsRuntime::attack_animation_step(bool begin){
 if(!attack_owner||!attack_geometry||!world||!world->player_object)return -1;
 auto& target=world->player_object->target;
 attack_fields.target=target.target;attack_fields.last_target=target.last_target;
 const auto phase=prince_locomotion.animation_depth();
 const dh2::character::AttackAnimationBorrowV1 borrow{&attack_fields,&phase,&target.target};
 const dh2::character::AttackAnimationServicesV1 services{this,attack_animation_service};
 const int status=begin?dh2_character_attack_animation_begin_v1(&borrow,&services):
                      dh2_character_attack_animation_end_v1(&borrow,&services);
 __android_log_print(status==1?ANDROID_LOG_INFO:ANDROID_LOG_ERROR,"DH2Native",
  "Source attack animator | begin %u | status %d | phase %u | index %d | continued %u | last %u | finisher %u | target %016llx | same AI/FSM/playback",
  unsigned(begin),status,phase,attack_fields.index,attack_fields.continued,attack_fields.last,
  attack_fields.finisher,static_cast<unsigned long long>(target.target));
 return status==1?0:-1;
}
