// Included inside model_renderer's private namespace after retained owners.
// Connects the original Idle/Injure FSM and CPU banks. The explicit development
// Attack/Walk/Dead renderer path remains until the full NPC AI frame is bound.
void npc_include_bound(MonsterScriptHandle& m,const float* point){
 float clip[4]{};
 for(unsigned row=0;row<4;++row)for(unsigned column=0;column<4;++column)clip[row]+=submitted_camera[column*4+row]*(column==3?1.f:point[column]);
 if(std::isfinite(clip[3])&&clip[3]>0){
  const float x=(clip[0]/clip[3]+1)*.5f*submitted_width,y=(1-clip[1]/clip[3])*.5f*submitted_height,z=clip[2]/clip[3];
  if(std::isfinite(x)&&std::isfinite(y)&&std::isfinite(z)&&z>=-1&&z<=1){
   if(!m.rendered_screen_bounds_ready){m.rendered_screen_bounds={x,x,y,y};m.rendered_screen_bounds_ready=true;}
   else{auto& b=m.rendered_screen_bounds;b[0]=std::min(b[0],x);b[1]=std::max(b[1],x);b[2]=std::min(b[2],y);b[3]=std::max(b[3],y);}
  }
 }
 if(!m.rendered_bounds_ready){std::copy_n(point,3,m.rendered_bounds.begin());std::copy_n(point,3,m.rendered_bounds.begin()+3);m.rendered_bounds_ready=true;return;}
 for(unsigned i=0;i<3;++i){m.rendered_bounds[i]=std::min(m.rendered_bounds[i],point[i]);m.rendered_bounds[i+3]=std::max(m.rendered_bounds[i+3],point[i]);}
}
int MonsterScriptHandle::select_animation(void* p,dh2::character::State* state,int sequence){
 auto& m=*static_cast<MonsterScriptHandle*>(p);
 if(!m.machine||state!=&m.machine->state())return -1;
 if(sequence==-1){sequence=state->animation_override;state->animation_override=-1;}
 std::string error;
 if(!m.animation||!m.animation->start(actor_animation_tables,sequence,actor_random,1.f,error)){
  __android_log_print(ANDROID_LOG_ERROR,"DH2Native","NPC CPU selection failed | %s | %s",m.object->name.c_str(),error.c_str());return -1;
 }
 state->current_animation=sequence;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source NPC animation selected | %s | state %d | sequence %d | clip %d | same retained FSM/CPU actor",m.object->name.c_str(),state->current,sequence,m.animation->playback().current_clip());return 0;
}
int MonsterScriptHandle::native_frame(void* p,dh2::character::NativeFsm24* fsm,const dh2::character::NativeFsmRequest32* q,std::uint32_t* out){
 using namespace dh2::character;auto& m=*static_cast<MonsterScriptHandle*>(p);
 if(!q||!out||!m.machine||fsm!=&m.machine->native_fsm()||!m.diagnostics)return -1;
 if(q->service==fsm_engine_dt){*out=m.application_dt;return 0;}
 if(q->service==fsm_profile_begin||q->service==fsm_profile_end){
  StateOwnerRequest48 request{};request.operation=q->service==fsm_profile_begin?state_owner_profile_begin:state_owner_profile_end;
  request.source_function=q->service==fsm_profile_begin?0x337888u:0x318254u;
  return m.diagnostics->invoke(request);
 }
 return -1;
}
int MonsterScriptHandle::idle_update(void* p){
 using namespace dh2::character;auto& m=*static_cast<MonsterScriptHandle*>(p);
 if(!player_skills_runtime||player_skills_runtime->world!=m.context)return -1;
 IdleCharacter40 character{m.object->identity,m.controller->identity(),{},m.machine->state().elapsed_ms,m.machine->state().heading_active,0};
 std::copy(m.object->position.begin(),m.object->position.end(),character.position);
 const IdleUpdateServices16 services{&m,[](void* p,IdleCharacter40* c,const IdleUpdateRequest32* q,IdleUpdateResponse16* out){
  auto& m=*static_cast<MonsterScriptHandle*>(p);if(!c||!q||!out||c->identity!=m.object->identity||!player_skills_runtime)return -1;
  if(q->service!=idle_is_player)return -1;
  const auto queries=player_skills_runtime->targets->targets().query_services();
  const dh2::target_providers::Request24 request{dh2::target_providers::virtual_player,0,q->subject,0};std::uintptr_t value{};
  if(queries.invoke(queries.context,&request,&value))return -1;out->word=std::uint32_t(value);return 0;
 }};
 return dh2_character_idle_update(&character,&services)==1?0:-1;
}
void MonsterScriptHandle::animation_state(void* p,dh2::character::AnimationAIState96* ai,const dh2::character::AnimationAIRequest32* q,dh2::character::AnimationAIResponse16* out){
 using namespace dh2::character;auto& m=*static_cast<MonsterScriptHandle*>(p);
 if(!q||!out||ai!=&m.animation_ai||q->subject!=m.object->identity||q->service!=ai_animation_state)
  throw std::runtime_error("Required NPC animation state/consumer producer");
 std::int32_t state{};
 if(dh2_character_native_fsm_get_integer(&state,&m.machine->native_fsm(),0)!=1)
  throw std::runtime_error("NPC animation current-state getter failed");
 out->word=std::uint32_t(state);
}
int MonsterScriptHandle::animation_service(void* p,dh2::character::AIEventState64* ai,const dh2::character::AIEventRequest40* q,std::uint32_t* out){
 using namespace dh2::character;auto& m=*static_cast<MonsterScriptHandle*>(p);
 if(!q||!out||ai!=&m.ai_events)return -1;
 const AIEventServices24 services{&m,animation_service,63,0};
 if(q->service==ai_event_state_event&&q->subject==reinterpret_cast<std::uintptr_t>(&m.machine->native_fsm())){
  const auto result=m.machine->event(int(q->event),q->payload);*out=std::uint32_t(result);return result<0?-1:0;
 }
 if(q->service==ai_event_virtual&&q->subject==ai->ai&&q->operation==0x98&&q->callee==0x3d0ce8)
  return dh2_character_ai_end_anim(ai,&services);
 if(q->service==ai_event_ais_virtual&&q->operation==0x98&&q->callee==0x3dccd0)
  return character_ais_external_end_anim(m.session->owner(),q->subject);
 if(q->service==ai_event_helper&&q->subject==ai->ai){
  if(q->operation==0x3d3d4c||q->operation==0x3d3d30){
   std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&m.machine->native_fsm(),0)!=1)return -1;*out=1;return 0;
  }
  if(q->operation==0x3d4204||q->operation==0x3d3ff8){
   if(m.machine->state().current!=3&&m.machine->state().current!=11)return -1;
   const AnimationAIServices16 animation{&m,animation_state};
   const auto result=dh2_character_animation_ai(&m.animation_ai,q->operation==0x3d4204?ai_step_begin:ai_step_end,&animation);
   *out=std::uint32_t(result);return result==1?0:-1;
  }
  if(q->operation==0x3d4434){
   using namespace skills;
   MeleeAnimationServicesV1 authored{&m,[](void* context,const MeleeAnimationRequestV1* request,MeleeAnimationResponseV1* response){
    auto& m=*static_cast<MonsterScriptHandle*>(context);if(!request||!response)return -1;
    if(request->operation==melee_event_step_index){response->value=int(m.animation->playback().step_index());return 0;}
    if(request->operation==melee_event_step_count){response->value=int(m.animation->playback().step_count(actor_animation_tables));return 0;}
    if(request->operation==melee_event_state){response->value=m.machine->state().current;return 0;}
    return -1;
   },&m.injury_debug};
   const auto* event=reinterpret_cast<const dh2::animation::TriggeredEvent*>(q->payload);
   MeleeAnimationOutputV1 result{};if(!event||dh2_character_melee_animation_event_v1(&result,event->name,&authored))return -1;*out=1;return 0;
  }
 }
 return -1;
}
void MonsterScriptHandle::observe(void* p,dh2::actor::BlendedPlayback&,const dh2::actor::BlendedPlaybackEvent& event){
 using namespace dh2::character;auto& m=*static_cast<MonsterScriptHandle*>(p);
 auto* controller=m.controller->command_state(controller_global_blocked);if(!controller)throw std::runtime_error("NPC animator controller unavailable");
 m.ai_owner.forced=controller->forced;m.ai_owner.locked=controller->locked;m.ai_events.global_blocked=controller->global_blocked;
 const auto& handoff=event.event.handoff;
 const dh2::animation::TriggeredEvent authored{handoff.lag_ms,handoff.payload};
 const AIEventPayload24 payload{handoff.event_id==0x28?reinterpret_cast<std::uintptr_t>(&authored):0,0,0,0};
 const AIEventServices24 services{&m,animation_service,63,0};AIEventResult16 result{};
 if(dh2_character_ai_event(&result,&m.ai_events,handoff.event_id,&payload,&services))
  throw std::runtime_error("Required original NPC animator/AI/FSM event: "+m.machine->error());
 ++m.source_animation_events;
 if(handoff.event_id==0x22)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Source NPC sequence closed | %s | state %d | clip %d | actual external OnEndOfAnim then FSM | gate %.9g",m.object->name.c_str(),m.machine->state().current,event.event.clip,double(m.injury_gate));
}
void initialize_npc_visual(ObjectActor& actor){
 auto& m=*actor.script;if(m.visual_initialized)return;
 float scale[3];dh2::physical::ObjectVisualTransformV1 transform{};
 if(dh2_character_visual_scale(scale,actor.properties().base.data()+12)||
    dh2_object_visual_transform_v1(&transform,actor.position.data(),actor.rotation_degrees.data(),scale))
  throw std::runtime_error("Source NPC GameObject visual transform unavailable");
 std::copy_n(transform.rotation_radians,3,m.runtime.rotation.rotation);
 m.runtime.rotation.heading_angle=m.runtime.rotation.rotation[2];
 std::copy(actor.position.begin(),actor.position.end(),m.runtime.subobjects.position);
 auto& visual=m.animation->visual();
 std::copy(actor.position.begin(),actor.position.end(),visual.root.position);
 std::copy_n(transform.root_quaternion,4,visual.root.quaternion);
 std::copy_n(transform.effective_scale,3,visual.root.scale);
 std::string error;if(!visual.update_world(m.animation->scene(),error))throw std::runtime_error(error);
 std::copy_n(transform.root_matrix,16,m.bounds_root_matrix.begin());
 m.bounds_root_ready=true;
 m.visual_initialized=true;
}
void bind_npc_injuries(PlayerSkillsRuntime& t){
 for(auto& entry:t.world_actors){auto& actor=entry.second;if(!actor.npc)continue;auto& m=*actor.npc;
  m.injury_debug={&t,PlayerSkillsRuntime::regen_debug};
  m.animation_ai.owner=m.object->identity;m.animation_ai.controller=m.controller->identity();m.animation_ai.target_sticky=1;
  const dh2::character::NpcInjuryBorrowV1 borrow{m.machine.get(),m.session.get(),&m.session->property_view(),&actor_animation_tables,&m.context->tables,&m.object->target,&actor.character,&m.injury_gate,m.object->life.get()};
  const dh2::character::NpcInjuryServicesV1 services{&m.injury_debug,t.targets->targets().query_services(),&m,nullptr,MonsterScriptHandle::select_animation,
   [](void* p,std::uintptr_t owner,std::uintptr_t target){auto& m=*static_cast<MonsterScriptHandle*>(p);if(owner!=m.object->identity||!player_skills_runtime)return -1;return dh2::character::npc_controller_look_v1({player_skills_runtime->targets.get(),owner,&m.runtime.rotation.heading_angle},target);}};
  m.injury=std::make_unique<dh2::character::NpcInjuryRuntimeV1>(borrow,services);
  if(!m.injury->valid())throw std::runtime_error("Source NPC injury shared owners disagree");
  m.animation->playback().observer={&m,MonsterScriptHandle::observe};
 }
}
void npc_scene_phase(unsigned dt){
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1&&actor.script){
  auto& m=*actor.script;std::string error;m.scene_clock+=dt;
  std::copy(actor.position.begin(),actor.position.end(),m.animation->visual().root.position);
  if(!m.animation->scene_phase(m.scene_clock,error))throw std::runtime_error("NPC scene phase: "+error);
 }
}
void npc_animator_phase(unsigned dt){
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1&&actor.script){
  auto& m=*actor.script;std::string error;m.application_dt=dt;
  if(!m.injury||m.injury->tick(dt)!=1||m.machine->update()<0)
   throw std::runtime_error("Source NPC injury/FSM frame: "+m.machine->error());
  if(!m.animation->animator_phase(actor_animation_tables,actor_random,1.f,error))throw std::runtime_error("NPC animator phase: "+error);
 }
}
