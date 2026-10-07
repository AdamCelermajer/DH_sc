// Actual source target phase; no independent death publisher or target owner.
static int player_target_path_policy_v2(void* raw,std::uint32_t* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);if(!out)return -1;
 std::uint32_t profile=0;
 if(dh2_character_debug_load(t.world->debug.get(),&t.world->debug_files)!=1||
  dh2_character_debug_get(&profile,t.world->debug.get(),"PF_ProfileSearches",&t.world->debug_files)!=1)return -1;
 if(profile){t.error="Required source PFWorld search profiling statistics owner";return -1;}
 return dh2_fx_debug_module_get(out,t.world->effect_modules.get(),"PathFinding_Search")==1?0:-1;
}
static int player_target_find_path_v2(void* raw,const dh2::character::PathToRequest32* q,std::uint32_t* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);if(!q||!out||q->owner!=t.fsm.character||!level.native_floor)return -1;
 const std::uint64_t required=std::uint64_t(level.native_floor->graph.node_count)+1;
 if(required>UINT32_MAX){t.error="Required bounded source route output capacity";return -1;}
 // Result storage is distinct from PFObject-owned segments and from the
 // world's internal search scratch. Preserve this caller-owned allocation
 // through every synchronous source query; resize for an actual level reload.
 t.target_route_path_v2.resize(std::size_t(required));
 dh2::navigation::RouteResult result{};result.search.path=t.target_route_path_v2.data();
 result.search.path_capacity=unsigned(t.target_route_path_v2.size());
 dh2::navigation::FindRequest request{&level.native_floor->route_world,&level.native_floor->collision_world,
  &prince_runtime.path,&result,&level.native_floor->route_workspace,{q->target[0],q->target[1],q->target[2]},q->limit,0,0};
 const dh2::navigation::FindSourceServicesV1 services{&t,player_target_path_policy_v2};
 const int status=dh2_nav_find_path_source_v1(&request,&services);
 if(status){const auto detail=t.error;t.error="Source PFWorld.FindPath status "+std::to_string(status)+" node_count "+std::to_string(level.native_floor->graph.node_count)+" segment_capacity "+std::to_string(prince_runtime.path.capacity)+" result_capacity "+std::to_string(result.search.path_capacity);if(!detail.empty())t.error+="; "+detail;return -1;}
 *out=result.found;return 0;
}
static int player_target_move_point_v2(void* raw,const float* point){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 // Complete Cmd_MoveTo(Point)4054e4 gates and Character Ctrl3ada30.
 if(!prince_controller_forced&&(controller_global_blocked||prince_state.controller_locked))return 0;
 bool remote=false;if(!PlayerSkillsRuntime::heading_remote(&t,remote,t.error))return -1;if(remote)return 0;
 // Source GameObject ctor writes static84=0 and PathTo's default limit26c=0.
 const auto& path=prince_runtime.path;
 dh2::character::PathToState40 state{t.fsm.character,0,path.count!=0,0,0,{path.target[0],path.target[1],path.target[2]},0};
 dh2::character::PathToResult16 result{};const dh2::character::PathToServices16 services{&t,player_target_find_path_v2};
 const int status=dh2_character_path_to(&result,&state,point,&services);
 if(status){if(t.error.empty())t.error="Source GameObject.PathTo status "+std::to_string(status);return -1;}return 0;
}
static int player_target_interaction_spot_v2(void* raw,float* point){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);const auto id=t.world->player_object->target.target;
 auto it=t.world_actors.find(id);if(it==t.world_actors.end()||!point)return -1;auto& a=it->second;
 const auto visual=a.npc?a.npc->source_visual():t.animation_visual;
 if(a.interaction_checked&&visual!=a.interaction_visual){t.error="Required source interaction cache visual lifetime refresh";return -1;}
 if(!a.interaction_checked){
  a.interaction_checked=true;a.interaction_visual=visual;
  if(visual){dh2::character::skills::WorldTargetActorBorrowV1 borrow{};
   if(t.targets->actor(id,&borrow)||!borrow.scene||dh2::character::animation_specific_node_v1(*borrow.scene,UINT32_MAX,"interaction_position",a.interaction_node))return -1;
  }
 }
 if(a.interaction_node>=0){t.error="Required actual interaction SceneNode absolute-position producer";return -1;}
 const float* actual=nullptr;if(!PlayerSkillsRuntime::heading_position(&t,id,actual,t.error))return -1;
 std::copy_n(actual,3,point);return 0;
}
static int player_target_out_range_binding_v2(PlayerSkillsRuntime& t){
 using namespace dh2::character;
 PlayerTargetOutRangeServicesV2 s{};s.context=&t;
 s.seeking=[](void* raw,bool& out){auto& r=*static_cast<PlayerSkillsRuntime*>(raw);out=r.attack_fields.seeking!=0&&!(prince_state.flags&0x1000);return 0;};
 s.state=[](void* raw,std::int32_t& out){auto& r=*static_cast<PlayerSkillsRuntime*>(raw);return dh2_character_native_fsm_get_integer(&out,&r.fsm,0)==1?0:-1;};
 s.has_path=[](void*,bool& out){out=prince_runtime.path.count!=0;return 0;};
 s.interaction_spot=player_target_interaction_spot_v2;s.move_point=player_target_move_point_v2;
 s.clear_target=[](void* raw){auto& r=*static_cast<PlayerSkillsRuntime*>(raw);return dh2_character_ai_set_target(&r.world->player_object->target,0,0,&r.world->player_object->binding.services);};
 s.sync_last=[](void* raw){auto& r=*static_cast<PlayerSkillsRuntime*>(raw);return dh2_character_ai_sync_last_target(&r.world->player_object->target);};
 s.stop_seeking=[](void* raw){static_cast<PlayerSkillsRuntime*>(raw)->attack_fields.seeking=0;return 0;};
 const auto previous=t.error;t.error.clear();std::string coordinator;
 const int result=player_target_out_range_v2(s,coordinator);
 if(result){const auto detail=t.error;t.error=coordinator;if(!detail.empty())t.error+="; "+detail;if(!previous.empty())t.error+="; "+previous;}return result;
}
static int player_target_control_v2(void* raw,const dh2::character::CharacterControlRequest32* q,dh2::character::CharacterControlResponse16* out){
 using namespace dh2::character;auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 if(!q||!out||q->subject!=t.fsm.character)return -1;
 if(q->service==control_is_remotely_updated){bool value;if(!PlayerSkillsRuntime::heading_remote(&t,value,t.error))return -1;out->word=value;return 1;}
 if(q->service==control_stop_object){t.heading_body=prince_body.body?&prince_body:nullptr;return t.heading_owner&&t.heading_owner->object_stop(t.error)?1:-1;}
 if(q->service==control_character_event)return PlayerSkillsRuntime::heading_event(&t,q->argument,t.error)?1:-1;
 return -1;
}
static int player_target_died_borrow_v2(void* raw,dh2::character::PlayerTargetDiedPhaseV2 phase,dh2::character::PlayerTargetDiedBorrowV2* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);if(!out||!t.world||!t.world->player_object)return -1;
 *out={};if(phase==dh2::character::PlayerTargetDiedPhaseV2::controller){
  out->controller={reinterpret_cast<std::uintptr_t>(&prince_runtime.controller),t.world->player_object->identity,controller_global_blocked,prince_state.controller_locked,prince_controller_forced,0};
  out->control=&t.target_control;
 }
 out->target=&t.world->player_object->target;out->target_services=&t.world->player_object->binding.services;return 0;
}
static int player_target_ais_v2(PlayerSkillsRuntime& t,unsigned event,std::uintptr_t selected){
 using namespace dh2::character;using namespace dh2::character::skills;
 if(!selected)return 0;
 if(selected!=t.events.active||!t.events.ais_virtuals){t.error="Required same selected AIS target receiver";return -1;}
 const unsigned slot=event-0xa+0x40/4;const auto key=t.events.ais_virtuals[slot];
 if(event==0xa&&key==0x3dd84c)return player_target_died_v2({&t,player_target_died_borrow_v2},t.error);
 if((event==0xb&&key==0x3dbeb0)||(event==0xc&&key==0x3dbeb4)||(event==0xd&&key==0x3dbeb8))return 0; // exact bx-lr bodies
 if(event==0xe&&key==0x3de1b4)return player_target_out_range_binding_v2(t);
 if(event==0x11&&key==0x3de1a8){
  // IPhone tail3de1a8 -> AISDefault.OnTargetInMeleeRange3dc284.
  auto& target=t.world->player_object->target;std::uintptr_t enemy=0;
  if(t.targets->relationship(target.owner->identity,target.target,true,&enemy)){t.error=t.targets->error();return -1;}
  if(!enemy){t.error="Required original nonenemy AISDefault melee interaction continuation";return -1;}
  dh2::character::PlayerTargetDiedBorrowV2 b{};
  if(player_target_died_borrow_v2(&t,PlayerTargetDiedPhaseV2::controller,&b)||dh2_character_controller_character(&b.controller,controller_stop,0,b.control)!=1)return -1;
  // Source reloads Character+408 after Cmd_Stop, not a stale target snapshot.
  return t.attack_owner?t.attack_owner->command(t.world->player_object->target.target):-1;
 }
 t.error="Required selected AIS target event "+std::to_string(event)+" source "+std::to_string(key);return -1;
}
struct PlayerTargetEventContextV2 {
 PlayerSkillsRuntime& t;dh2::character::TargetEventState32* state{};std::string debug_name;
 bool committed{};dh2::audio::AudioApplicationBorrowV42 application;
};
static int player_target_event_service_v2(void* raw,dh2::character::TargetEventState32* state,
 const dh2::character::TargetEventRequest64* q,dh2::character::TargetEventResponse24* out){
 using namespace dh2::character;using namespace dh2::character::skills;
 auto& c=*static_cast<PlayerTargetEventContextV2*>(raw);auto& t=c.t;if(!q||!out||state!=c.state)return -1;
 // Keep this ephemeral ABI projection synchronized at each source callback.
 if(q->service!=target_event_active_dispatch)state->continued=std::uint8_t(t.attack_fields.continued);
 switch(q->service){
 case target_event_debug_load:return dh2_character_debug_load(t.world->debug.get(),&t.world->debug_files)==1?0:-1;
 case target_event_debug_construct:if(!q->text)return -1;c.debug_name=q->text;return 0;
 case target_event_debug_query:return dh2_character_debug_get(&out->word,t.world->debug.get(),c.debug_name.c_str(),&t.world->debug_files)==1?0:-1;
 case target_event_debug_destroy:c.debug_name.clear();return 0;
 case target_event_is_target_character:case target_event_get_target_character:{
  const auto id=state->target->target;if(!id){out->word=0;out->identity=0;return 0;}
  dh2::target_providers::Handle16 local{},*shared=nullptr;dh2::target_providers::Registry24* registry=nullptr;
  if(t.targets->get_handle(id,&local)||t.targets->handle_borrow(id,&shared,&registry))return -1;
  auto services=t.targets->targets().query_services();std::uintptr_t character=0;
  if(dh2::target_providers::dh2_target_handle_character(&character,&local,shared,registry,&services))return -1;
  out->identity=character;out->word=character!=0;return 0;
 }
 case target_event_owner_ai_id:{std::int32_t id;if(dh2_character_target_ai_id(&id,t.world->player_object->properties->resolved.data(),t.world->tables.ai()->rows.size()))return -1;std::memcpy(&out->word,&id,4);return 0;}
 case target_event_owner_position:{const float* point;if(!PlayerSkillsRuntime::heading_position(&t,q->subject,point,t.error))return -1;std::copy_n(point,3,out->position);return 0;}
 case target_event_clear_aggro:return player_target_clear_aggro_v40(t,q->subject,q->other);
 case target_event_play_sound:{
  CombatSoundPlayV1 play{};play.manager=q->subject;play.sound_id=q->sound;std::copy_n(q->position,3,play.position.begin());play.source_bool=q->flag!=0;play.source_integer=q->integer;play.source_float0=q->parameters[0];play.source_float1=q->parameters[1];
  return dh2::audio::audio_submit_application_v42(c.application,t.audio_source_clock_v42,play,t.error)?0:-1;
 }
 case target_event_active_dispatch:
  if(q->event==0xa)t.attack_fields.continued=state->continued;c.committed=true;
  return player_target_ais_v2(t,q->event,q->subject);
 default:t.error="Required original target handler provider "+std::to_string(q->service);return -1;
 }
}
int PlayerSkillsRuntime::target_character_event(unsigned event,std::uintptr_t payload){
 using namespace dh2::character;if(event<0xa||event>0x11)return -1;
 // Actual Character RaiseEvent dispatch through CharAI virtual callable table.
 const AIEventPayload24 data{payload,0,0,0};AIEventResult16 result{};
 const AIEventServices24 services{this,PlayerSkillsRuntime::event_service,63,0};
 return dh2_character_ai_event(&result,&events,event,&data,&services)?-1:0;
}
int PlayerSkillsRuntime::target_ai_event(unsigned event,std::uintptr_t callee){
 using namespace dh2::character;if(event<0xa||event>0x11||callee!=ai_virtuals[event-0xa+0x40/4])return -1;
 auto& target=world->player_object->target;TargetEventState32 state{&target,events.active,std::uint8_t(attack_fields.continued),{},0};
 PlayerTargetEventContextV2 context{*this,&state,{},false,{}};
 if(!borrow_actual_application_audio_v42(context.application,error))return -1;
 TargetEventServices40 services{&context,player_target_event_service_v2,target_ai_sounds.data(),unsigned(target_ai_sounds.size()),0,context.application.identity()};
 const int result=dh2_audio_target_event_v38(&state,event,&services);
 if(!result&&event==0xa&&!context.committed)attack_fields.continued=state.continued;
 return result?-1:0;
}
void PlayerSkillsRuntime::bind_target_frame(PlayerSkillsRuntime& t){
 using namespace dh2::character::skills;t.target_control={&t,player_target_control_v2};
 t.target_ai_sounds.clear();for(const auto& row:t.world->tables.ai()->rows)t.target_ai_sounds.push_back(row.on_aggro_sfx);
  WorldTargetFrameServicesV2 services{};services.context=&t;
  services.close_range=player_target_close_range_service_v38;
 services.machine_state=[](void* raw,std::uintptr_t id,std::uint32_t* out){auto& r=*static_cast<PlayerSkillsRuntime*>(raw);std::int32_t value;if(!out||id!=r.fsm.character||dh2_character_native_fsm_get_integer(&value,&r.fsm,0)!=1)return -1;std::memcpy(out,&value,4);return 0;};
 services.raise_event=[](void* raw,std::uintptr_t id,unsigned event,std::uintptr_t payload){auto& r=*static_cast<PlayerSkillsRuntime*>(raw);return id==r.fsm.character?r.target_character_event(event,payload):-1;};
 t.target_frame=std::make_unique<CharacterWorldTargetFrameV2>(*t.targets,*t.attack_geometry,*t.world->tables.ai(),services);
}
int PlayerSkillsRuntime::update_target_frame(){
 // Source CharAI.Update prefix for this player: paused/command policy and
 // actual flags gate. Player.IsZonable returnsfalse; no guessed zone bytes.
 if(events.paused||(!prince_controller_forced&&(controller_global_blocked||prince_state.controller_locked))||!(prince_state.flags&0x100))return 0;
 if(!target_frame){error="Required same-player target-frame owner";return -1;}
 source_target_updated88=1;
 const int result=target_frame->update(world->player_object->target);
 if(result){const auto backend=error;error=target_frame->error();if(!backend.empty())error+="; "+backend;return -1;}return 0;
}
