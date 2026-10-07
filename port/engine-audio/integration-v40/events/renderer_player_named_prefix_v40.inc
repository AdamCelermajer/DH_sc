// Staged include inside model_renderer's private namespace AFTER
// renderer_animation_sound_v4.inc. This handles ONLY sfx_ named prefixes;
// it does not accept missing melee/range/cast/FX/interaction gameplay owners.
static int source_player_named_prefix_v40(PlayerSkillsRuntime& t,const char* name,
 bool& handled){
 handled=name&&!std::strncmp(name,"sfx_",4);
 if(!handled)return 0;
 using namespace dh2::character::skills;
 dh2::audio::AudioAuthoredEventBorrowV40 borrow;
 borrow.actual_names=&t.audio_source_bindings_v38;
 borrow.actual_named_source.context=&t;
 borrow.actual_named_source.actual_character=t.fsm.character;
 borrow.actual_named_source.manager=[](void* raw,std::uintptr_t& out){
  auto& r=*static_cast<PlayerSkillsRuntime*>(raw);if(!r.vox_music)return -1;
  out=r.vox_music->identity();return 0;};
 borrow.actual_named_source.target_position=[](void* raw,std::uintptr_t actor,
  std::array<float,3>& out){auto& r=*static_cast<PlayerSkillsRuntime*>(raw);
  const float* position=nullptr;if(!PlayerSkillsRuntime::heading_position(&r,actor,position,r.error))return -1;
  std::copy_n(position,3,out.begin());return 0;};
 borrow.transport=t.audio_producers_v40;
 borrow.remaining={&t,[](void* raw,const MeleeAnimationRequestV1* q,MeleeAnimationResponseV1* out){
  auto& r=*static_cast<PlayerSkillsRuntime*>(raw);if(!q||!out||!r.world||!r.world->player_object)return -1;
  if(q->operation==melee_event_step_index){out->value=prince_locomotion.step_index();return 0;}
  if(q->operation==melee_event_step_count){out->value=prince_locomotion.step_count(actor_animation_tables);return 0;}
  r.error="Required genuine player authored-event owner operation "+std::to_string(q->operation);return -1;
 },nullptr};
 MeleeAnimationOutputV1 output{};
 return dh2::audio::audio_dispatch_actual_authored_event_v40(output,name,borrow,t.error);
}
