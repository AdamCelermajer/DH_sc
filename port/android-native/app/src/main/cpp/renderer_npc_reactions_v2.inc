// Include after PlayerSkillsRuntime, NPC animation callbacks, native World and
// actor body owners. The parent TU includes npc_skill_reactions_v2.hpp,
// character_head_object_v2.hpp, native_physical_filter_v1.hpp and
// canonical_point3d_globals_v1.hpp. Own this alongside the SAME retained NPC.
struct RendererNpcReactionInputsV2 {
 // Actual constructor byte118 backing; ObjectBaseC1 33f2c8 stores0. This must
 // live on the retained base receiver, not be inferred from renderer visibility.
 const std::uint8_t* remote118{};
 // Actual source PhysicalObject pointer and its own shape/saved-filter fields.
 // A null body is legal only when the SAME machine.physical is also null.
 dh2::physical::NativeBody* body{};
 dh2::physical::NativePhysicalFilterBorrowV1 filter{};
 dh2::character::BuffOwner* buffs{};
 // Construct once from SAME NPC base/CharAI/state/property backing and the
 // retained World combat/shared-CF/debug owners (npc_recurring_effects_v1).
 dh2::character::NpcRecurringEffectsV1* recurring{};
 // Actual remaining AI timer/death services. Reached absence is an error.
 void* context{};
 int(*timer)(void*,MonsterScriptHandle&,int,dh2::character::Timer32*){};
 int(*dead)(void*,MonsterScriptHandle&,bool,std::uintptr_t,bool){};
 int(*stop_animation)(void*,MonsterScriptHandle&){};
};
class RendererNpcReactionsV2 {
 MonsterScriptHandle& m_;PlayerSkillsRuntime& world_;
 RendererNpcReactionInputsV2 inputs_;
 dh2::physical::NativeBody* body_{};
 dh2::character::CharacterHeadingServicesV1 heading_services_{};
 std::unique_ptr<dh2::character::CharacterHeadingOwnerV1> heading_;
 std::unique_ptr<dh2::character::CharacterCollisionLifecycleV1> collisions_;
 dh2::character::TimerServices32 timer_services_{};
 std::unique_ptr<dh2::character::NpcSkillReactionsV2> reactions_;
 std::string error_;
 bool timer_failed_{};
 bool refresh(){
  if(!m_.machine||!m_.controller||!m_.session||!m_.object||!m_.animation||
     world_.world!=m_.context){error_="Required SAME NPC reaction graph";return false;}
  const auto physical=m_.machine->owner().machine().physical;
  body_=inputs_.body;
  if(physical!=reinterpret_cast<std::uintptr_t>(body_)||
     (body_&&!body_->body)){error_="NPC reaction physical pointer disagrees with actual source owner";return false;}
  auto* c=m_.controller->command_state(controller_global_blocked);
  if(!c)return false;
  // State.controller_locked is the existing FSM body's logical projection of
  // the SAME v2Controller byte8, not a second command gate.
  c->locked=m_.machine->state().controller_locked;
  return true;
 }
 static bool position(void* p,std::uintptr_t id,const float*& out,std::string& error){
  return PlayerSkillsRuntime::heading_position(&static_cast<RendererNpcReactionsV2*>(p)->world_,id,out,error);
 }
 static bool remote(void* p,bool& out,std::string& error){
  auto& t=*static_cast<RendererNpcReactionsV2*>(p);
  if(t.m_.machine->combat_fields().network_id!=-1){out=true;return true;}
  if(!t.inputs_.remote118){error="Required retained ObjectBase constructor byte118";return false;}
  out=*t.inputs_.remote118!=0;return true;
 }
 static bool physics(void* p,bool& out,std::string&){out=(static_cast<RendererNpcReactionsV2*>(p)->m_.machine->state().flags&2)!=0;return true;}
 static bool raise(void* p,std::uint32_t event,std::string& error){
  auto& t=*static_cast<RendererNpcReactionsV2*>(p);if(!t.refresh()){error=t.error_;return false;}
  auto& m=t.m_;m.runtime.rotation.heading_angle=m.runtime.controller.heading.angle;
  m.machine->state().heading_active=m.runtime.controller.heading.active;
  auto* controller=m.controller->command_state(controller_global_blocked);
  m.ai_owner.locked=controller->locked;m.ai_owner.forced=controller->forced;m.ai_events.global_blocked=controller->global_blocked;
  const dh2::character::AIEventPayload24 payload{};dh2::character::AIEventResult16 result{};
  const dh2::character::AIEventServices24 services{&t,ai_service,63,0};
  if(dh2_character_ai_event(&result,&m.ai_events,event,&payload,&services)){error="Required actual NPC heading CharAI event "+std::to_string(event);return false;}return true;
 }
 static int constant(void* p,const char* group,const char* key,int* out){
  auto& t=*static_cast<RendererNpcReactionsV2*>(p);const auto* d=t.m_.context->tables.design();
  return d&&d->lookup&&out?d->lookup(d->context,0,group,key,out):-1;
 }
 static int is_player(RendererNpcReactionsV2& t,std::uintptr_t id,std::uint32_t* out){
  if(!out||!t.world_.targets)return -1;const auto services=t.world_.targets->targets().query_services();
  const dh2::target_providers::Request24 q{dh2::target_providers::virtual_player,0,id,0};std::uintptr_t value{};
  if(!services.invoke||services.invoke(services.context,&q,&value))return -1;*out=value!=0;return 0;
 }
 static int stance(void* p,std::uintptr_t id,int* out){
  auto& t=*static_cast<RendererNpcReactionsV2*>(p);std::uint32_t player{};int count{};
  if(!out||id!=t.m_.object->identity||is_player(t,id,&player)||player||constant(p,"AnimStances","COUNT_IPHONE",&count))return -1;
  // Whole nonplayer branch of Character.GetAnimStance3a53e0: candidate0,
  // actual COUNT_IPHONE lookup, and return0 for every signed count.
  *out=0;return 0;
 }
 static int event(void* p,int event,std::uintptr_t payload){return static_cast<RendererNpcReactionsV2*>(p)->m_.machine->event(event,payload)<0?-1:0;}
 static int transition(void* p,int state,int event,std::uintptr_t payload){return static_cast<RendererNpcReactionsV2*>(p)->m_.machine->transition(state,event,payload)<0?-1:0;}
 static int debug(void* p,const char* key){
  auto& t=*static_cast<RendererNpcReactionsV2*>(p);std::uint32_t value{};auto& context=*t.m_.context;
  return dh2_character_debug_load(context.debug.get(),&context.debug_files)==1&&
   dh2_character_debug_get(&value,context.debug.get(),key,&context.debug_files)==1?0:-1;
 }
 static int animation(void* p,int sequence){auto& t=*static_cast<RendererNpcReactionsV2*>(p);return MonsterScriptHandle::select_animation(&t.m_,&t.m_.machine->state(),sequence);}
 static int pin(void* p){auto& t=*static_cast<RendererNpcReactionsV2*>(p);return t.refresh()&&t.body_?dh2_native_body_pin(t.body_):-1;}
 static int unpin(void* p){auto& t=*static_cast<RendererNpcReactionsV2*>(p);return t.refresh()&&t.body_?dh2_native_body_unpin(t.body_):-1;}
 int change_filter(int group,int category,int mask,bool secondary,bool reset){
  if(!refresh()||!body_)return -1;const auto& b=inputs_.filter;
  if(b.body!=body_||!b.primary||!b.secondary||!b.saved||!b.disabled||!b.world||!b.world->backend())return -1;
  for(auto slot:{b.primary,b.secondary}){if(slot==b.secondary&&!reset&&!secondary)continue;if(auto* shape=*slot){
   if(shape->GetBody()!=body_->body)return -1;b2FilterData value;
   if(reset)value=*b.saved;else{value.groupIndex=std::int16_t(group);value.categoryBits=std::uint16_t(category);value.maskBits=std::uint16_t(mask);}
   shape->SetFilterData(value);b.world->backend()->Refilter(shape);
  }}*b.disabled=0;return 0;
 }
 static int filter(void* p,int group,int category,int mask,bool secondary){return static_cast<RendererNpcReactionsV2*>(p)->change_filter(group,category,mask,secondary,false);}
 static int reset(void* p){return static_cast<RendererNpcReactionsV2*>(p)->change_filter(0,0,0,true,true);}
 static dh2::physical::NativeBody* physical(void* p){auto& t=*static_cast<RendererNpcReactionsV2*>(p);return t.refresh()?t.body_:nullptr;}
 static int collision_method(void* p,dh2::character::WorldNpcObjectMethodV1 method,std::string& error){
  auto& t=*static_cast<RendererNpcReactionsV2*>(p);
  if(method!=dh2::character::WorldNpcObjectMethodV1::EnablePhysicalFilter&&method!=dh2::character::WorldNpcObjectMethodV1::DisablePhysicalFilter)return -1;
  return dh2::physical::native_physical_filter_v1(t.inputs_.filter,method==dh2::character::WorldNpcObjectMethodV1::EnablePhysicalFilter,error)?0:-1;
 }
 static int ai_service(void* p,dh2::character::AIEventState64* ai,const dh2::character::AIEventRequest40* q,std::uint32_t* out){
  using namespace dh2::character;auto& t=*static_cast<RendererNpcReactionsV2*>(p);auto& m=t.m_;
  if(!q||!out||ai!=&m.ai_events)return -1;
  if(q->service==ai_event_virtual&&q->subject==ai->ai&&
    ((q->operation==0x84&&q->callee==0x3d0c34)||(q->operation==0x88&&q->callee==0x3d0c58))){
   // Whole original CharAI expiry relay: null active returns; actual selected
   // AISExternal inherits source literal bx-lr Default expiry methods.
   ScriptSessionView selected{};if(!m.session->owner().active(selected)){*out=0;return 0;}
   if(ai->active!=selected.identity||!ai->ais_virtuals)return -1;
   const auto callback=ai->ais_virtuals[q->operation/4];
   if(callback!=(q->operation==0x84?0x3dbee0u:0x3dbee4u))return -1;
   *out=0;return 0;
  }
  if(q->service==ai_event_helper&&q->operation==0x394a3c&&q->subject==m.object->identity){
   if(!t.collisions_||!level.native_floor)return -1;
   t.collisions_->rebind_geometry(&level.native_floor->collision_world);
   if(!t.collisions_->enable()){t.error_=t.collisions_->error();return -1;}*out=0;return 0;
  }
  return MonsterScriptHandle::animation_service(&m,ai,q,out);
 }
 static int look(void* p,std::uintptr_t target){auto& t=*static_cast<RendererNpcReactionsV2*>(p);return dh2::character::npc_controller_look_v1({t.world_.targets.get(),t.m_.object->identity,&t.m_.runtime.rotation.heading_angle},target);}
 static int cancel(void* p){return static_cast<RendererNpcReactionsV2*>(p)->m_.injury->cancel_sneaking();}
 static int dead_query(void* p,bool* out){auto& t=*static_cast<RendererNpcReactionsV2*>(p);if(!out||!t.m_.object->life)return -1;*out=t.m_.object->life->dead!=0;return 0;}
 static int stop(void* p){
  auto& t=*static_cast<RendererNpcReactionsV2*>(p);auto& playback=t.m_.animation->playback();
  // ANIM_Stop3c9924 is not StopLoop: it stops the actual visual controller,
  // resets stack depth and synchronously raises22 iff sequence was open.
  // The retained instance IS this NPC's actual visual controller. Its observer
  // is the existing CharAI -> selected AIS -> same FSM event22 route.
  return playback.stop_immediate_v1(true,t.error_)?0:-1;
 }
 static int set_dead(void* p,bool mode,std::uintptr_t payload,bool force){auto& t=*static_cast<RendererNpcReactionsV2*>(p);return t.inputs_.dead?t.inputs_.dead(t.inputs_.context,t.m_,mode,payload,force):-1;}
 static int remaining(void* p,dh2::character::NativeEffects32*,const dh2::character::NativeEffectRequest32* q,std::uint32_t* out){
  using namespace dh2::character;auto& t=*static_cast<RendererNpcReactionsV2*>(p);if(!q||!out||!t.refresh())return -1;
  switch(q->service){
   case effect_is_player:return is_player(t,q->character,out);
   case effect_stop_loop:t.m_.animation->playback().stop_loop(false);return 0;
   case effect_random:*out=std::uint32_t(dh2_combat_random(&combat_random,std::int32_t(q->argument0)));return 0;
   case effect_heading_point:{float direction[3];const std::uint32_t bits[]{q->argument0,q->argument1,q->argument2};std::memcpy(direction,bits,sizeof direction);return t.heading_->character_head_towards(direction,t.error_)?0:-1;}
   case effect_heading_object:{
    CharacterHeadObjectBorrowV2 b{t.m_.controller->command_state(controller_global_blocked),t.heading_.get(),&t.m_.runtime.controller.heading.active,dh2::world::canonical_vec3_origin_v1().data(),t.heading_services_};
    return character_command_head_object_v2(b,q->payload,t.error_)?0:-1;
   }
   default:return -1;
  }
 }
 static void expiry(void* p,std::uintptr_t id,int event,dh2::character::Timer32* timer){
  auto& t=*static_cast<RendererNpcReactionsV2*>(p);
  if(id!=t.m_.object->identity||!timer){t.timer_failed_=true;t.error_="Required same NPC timer receiver";return;}
  // Full original callback must include CharAI virtual84/88 before FSM43/44.
  // Existing NPC animation_service has no timer-helper receiver yet. Caller
  // supplies genuine whole dispatch; never silently skip timer33/34/29.
  if(event==0x33||event==0x34){
   dh2::character::TimerEffectResult24 result{};
   if(!t.inputs_.recurring||t.inputs_.recurring->event(std::uint32_t(event),result)!=1){
    t.timer_failed_=true;t.error_=t.inputs_.recurring?t.inputs_.recurring->error():"Required SAME NPC recurring owner";
   }
  }else if(event==0x2b||event==0x2c){
   if(!t.raise(&t,std::uint32_t(event),t.error_))t.timer_failed_=true;
  }else if(!t.inputs_.timer||t.inputs_.timer(t.inputs_.context,t.m_,event,timer)){
   t.timer_failed_=true;if(t.error_.empty())t.error_="Required actual NPC CharAI timer event "+std::to_string(event);
  }
 }
public:
 RendererNpcReactionsV2(MonsterScriptHandle& m,PlayerSkillsRuntime& world,RendererNpcReactionInputsV2 inputs):m_(m),world_(world),inputs_(inputs){
  if(!refresh()||!m.injury||!m.injury->valid())throw std::runtime_error(error_.empty()?"Required existing actual NPC injury receiver":error_);
  heading_services_={this,position,remote,physics,raise};
  if(level.native_floor){dh2::character::WorldNpcObjectServicesV1 s{};s.context=this;s.physical=physical;s.method=collision_method;
   s=dh2::character::character_pf_services_v1(s);
   collisions_=std::make_unique<dh2::character::CharacterCollisionLifecycleV1>(m.runtime.object,&level.native_floor->collision_world,live_registry,m.object->identity,s);
  }
  heading_=std::make_unique<dh2::character::CharacterHeadingOwnerV1>(*m.controller->command_state(controller_global_blocked),m.machine->state(),m.object->target,m.object->binding.services,m.runtime.controller,m.runtime.path,m.runtime.subobjects.position,m.runtime.subobjects.destination,body_,heading_services_);
  timer_services_={this,expiry,nullptr,0};
  dh2::character::KnockbackReactionServicesV1 k{this,constant,stance,event,transition,debug,animation,filter,reset,pin,unpin,look,cancel,dead_query,stop,set_dead};
  dh2::character::SlowReactionBorrowV1 slow{&m.session->property_view(),inputs.buffs,m.context->tables.ai(),m.context->tables.classes(),m.context->effects_owner?m.context->effects_owner->borrow():dh2::data::EffectsTables::Borrow{}};
  dh2::character::NpcSkillReactionsBorrowV2 b{m.machine.get(),m.injury.get(),&m.session->property_view(),m.context->tables.ai(),&actor_animation_tables,&m.context->tables,&m.session->timers(),&timer_services_,slow};
  reactions_=std::make_unique<dh2::character::NpcSkillReactionsV2>(b,dh2::character::NpcSkillReactionsServicesV2{k,{this,remaining}});
  if(!reactions_->valid())throw std::runtime_error("NPC reaction same-owner borrow invalid");
 }
 dh2::character::NpcSkillReactionsV2& owner(){return *reactions_;}
 int application(const dh2::character::skills::SkillApplyRequestV6& q,dh2::character::skills::SkillApplyResponseV6* out){if(!refresh())return -1;const auto result=reactions_->application(q,out);return refresh()?result:-1;}
 int body(dh2::character::StateOwnerMachine40* machine,const dh2::character::StateOwnerRequest48& q){if(!refresh())return -1;const auto result=reactions_->body(machine,q);return refresh()?result:-1;}
 int update(dh2::character::StateOwnerMachine40* machine,const dh2::character::StateOwnerUpdateRequest24& q){if(!refresh())return -1;const auto result=reactions_->update(machine,q);return refresh()?result:-1;}
 int frame_effect(const dh2::character::NativeFsmRequest32& q){if(!refresh())return -1;const auto result=reactions_->frame_effect(q);return refresh()?result:-1;}
 const dh2::character::TimerServices32& timer_services()const{return timer_services_;}
 int update_timers(std::uint32_t dt,std::uint32_t source_blocked){
  timer_failed_=false;const auto result=dh2_character_timers_update(&m_.session->timers(),dt,source_blocked,&timer_services_);
  return result==1&&!timer_failed_?1:-2; // diagnostics retained; scan continues
 }
 const std::string& error()const{return error_;}
};
