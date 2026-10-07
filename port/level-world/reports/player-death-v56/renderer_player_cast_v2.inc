// Included inside renderer's private namespace; borrows only its live owners.
// Called at original SkillState service24, after the authored do_skill event.
int player_skill_animation_event_v6(PlayerSkillsRuntime& t){
 t.error.clear();
 // Read-only, event-local snapshots of the same registered property owners.
 // They establish damage delivery without introducing an HP projection.
 std::map<std::uintptr_t,std::int32_t> source_hp_before;
 for(const auto& entry:t.world_actors)if(entry.second.object&&entry.second.object->properties)
  source_hp_before.emplace(entry.first,entry.second.object->properties->resolved[36]);
 source_cast_target_trace_v46(t,"do_skill.before");
 std::uint32_t result=0;const auto status=t.player->native_skill_animation_event(&result);
 source_cast_target_trace_v46(t,"do_skill.after");
 for(const auto& entry:source_hp_before){const auto actor=t.world_actors.find(entry.first);
  if(actor==t.world_actors.end()||!actor->second.object||!actor->second.object->properties)continue;
  const auto after=actor->second.object->properties->resolved[36];
  if(after!=entry.second)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Source skill event HP delivery | actor %llx | name %s | resolved36 %d -> %d | source status %d",static_cast<unsigned long long>(entry.first),actor->second.object->name.c_str(),entry.second,after,status);
 }
 if(status){const auto backend=t.error;t.error="Required source AI_Event: "+t.player->error();
  if(!backend.empty())t.error+="; "+backend;
  if(t.execution&&!t.execution->error().empty())t.error+="; "+t.execution->error();
  if(t.native&&!t.native->error().empty())t.error+="; "+t.native->error();
 }
 return status;
}
int PlayerSkillsRuntime::regen_debug(void* p,const dh2::character::skills::SkillAttackNativeRequestV6* q,std::uintptr_t* out){
 using namespace dh2::character::skills;auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!q||!out)return -1;
 if(q->service==skill_attack_debug_load_v6)return dh2_character_debug_load(t.world->debug.get(),&t.world->debug_files)==1?0:-1;
 if(q->service==skill_attack_string_construct_v6&&q->name){*out=++t.debug_serial;t.debug_tokens[*out]=q->name;return 0;}
 const auto at=t.debug_tokens.find(q->subject);if(at==t.debug_tokens.end())return -1;
 if(q->service==skill_attack_debug_get_v6){std::uint32_t value;if(dh2_character_debug_get(&value,t.world->debug.get(),at->second.c_str(),&t.world->debug_files)!=1)return -1;*out=value;return 0;}
 if(q->service==skill_attack_string_destroy_v6){t.debug_tokens.erase(at);return 0;}return -1;
}
int PlayerSkillsRuntime::event_service(void* p,dh2::character::AIEventState64* state,const dh2::character::AIEventRequest40* q,std::uint32_t* out){
 using namespace dh2::character;auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!q||!out||state!=&t.events)return -1;*out=0;
 if(q->service==ai_event_virtual&&q->operation==0x24&&q->callee==0x3d1000)return player_death_ai_v56(t,*q);
 if(q->service==ai_event_state_event)return prince_event(q->event,q->payload)<0?-1:0;
  if(q->service==ai_event_state_getter){*out=prince_state.current;return 0;}
  if(q->service==ai_event_helper&&q->callee==0x394a3c)return t.enable_collisions_source(q->subject);
 if(q->service==ai_event_helper&&(q->callee==0x3cb77c||q->callee==0x3df3f0)){
  const dh2::character::skills::SkillAttackNativeServicesV6 debug{&t,regen_debug};
  dh2::android_ui::PlayerTimerServicesV1 services{};services.context=&t;services.debug=&debug;
  services.remote_updated=[](void* c,std::uintptr_t id,bool* answer){auto& runtime=*static_cast<PlayerSkillsRuntime*>(c);if(id!=runtime.fsm.character||!answer)return -1;*answer=false;return 0;}; // ObjectBase IsRemotelyUpdated is source constant false.
  services.aggro=[](void* c,std::uintptr_t id,bool* has,bool* incoming){auto& runtime=*static_cast<PlayerSkillsRuntime*>(c);if(id!=runtime.fsm.character||!has||!incoming)return -1;*has=prince_combat.aggro.outgoing_table.count!=0;*incoming=prince_combat.aggro.incoming_table.count!=0;return 0;};
  // Positive source DoT remains required until actual self/self combat backend
  // is connected. Zero/no-dead damage passes source body without a callback.
  return dh2::android_ui::player_timer_helper_v1(player_gameplay_binding(),unsigned(q->callee),services,t.error);
 }
 if(q->service==ai_event_helper&&(q->callee==0x3d8038||q->callee==0x3d8b28)&&t.cast)return t.cast->character_event(player_gameplay_binding(),q->callee==0x3d8038?0x20:0x21);
 // Actual CharAI relay -> selected AISPlayerIPhone inherits these literal
 // AISDefault bx-lr bodies, verified original functions, not empty providers.
 if(q->service==ai_event_virtual){
  unsigned slot=q->operation/4;if(slot>=51||q->callee!=t.ai_virtuals[slot])return -1;
  if(q->event>=0xa&&q->event<=0x11)return t.target_ai_event(q->event,q->callee);
  if(!t.events.active)return 0;
  const auto target=t.ais_virtuals[slot];
  if((q->operation==0x20&&target==0x3dbe8c)||(q->operation==0x8c&&target==0x3dbee8)||(q->operation==0x98&&target==0x3dbeec))return 0;
 }
 t.error="Required original player AI event service "+std::to_string(q->service)+" callee "+std::to_string(q->callee);return -1;
}
void PlayerSkillsRuntime::bind_cast(PlayerSkillsRuntime& t){
 dh2::android_ui::FaeryCastServicesV2 s{};s.context=&t;s.state_services=&prince_owner_services;
 s.faery_type=[](void* p,std::uintptr_t id,unsigned selected,unsigned* out){auto& runtime=*static_cast<PlayerSkillsRuntime*>(p);if(id!=runtime.fsm.character||!out)return -1;
  const auto rows=runtime.world->script_files.faeries();int list=runtime.player->session().property_view().resolved[29];if(list<0)list=0;
  if(unsigned(list)>=rows.lists().size()||selected>=rows.lists()[list].size())return -1;const auto index=rows.lists()[list][selected];
  if(index<0||unsigned(index)>=rows.faeries().size()||rows.faeries()[index].scalar.words[8]!=selected)return -1;*out=rows.faeries()[index].scalar.words[7];return 0;};
 // GetCharAnimTableId 3a3228 reads raw property2, source fallback17.
 // SM_SetCastState 3c6394 reads CharAnimTable row+a0, Spells count84/data88.
 s.cast_animation=[](void* p,std::uintptr_t id,unsigned selected,int* out){auto& runtime=*static_cast<PlayerSkillsRuntime*>(p);if(id!=runtime.fsm.character||!out)return -1;
  int table=runtime.player->session().property_view().resolved[2];if(table<0||unsigned(table)>=actor_animation_tables.characters.size())table=17;
  if(unsigned(table)>=actor_animation_tables.characters.size())return 1;
  const auto& spells=actor_animation_tables.characters[unsigned(table)].fields[31];if(selected>=spells.size())return 1;*out=spells[selected];return 0;};
 s.stance=[](void* p,std::uintptr_t id,unsigned* out){auto& runtime=*static_cast<PlayerSkillsRuntime*>(p);if(id!=runtime.fsm.character||!out||!player_equipment)return -1;
  dh2::character::StanceFacts16 facts{};std::string error;
  std::int32_t count;if(runtime.player->session().constant("AnimStances","COUNT_IPHONE",count))return -1;
  if(!player_equipment->stance_facts(true,count,facts,error))return -1;std::int32_t value;
  if(dh2_character_anim_stance(&value,&facts)!=1)return -1;*out=unsigned(value);return 0;};
 s.raise_event=[](void* p,std::uintptr_t id,unsigned event,std::uintptr_t){auto& runtime=*static_cast<PlayerSkillsRuntime*>(p);if(id!=runtime.fsm.character||!runtime.cast)return -1;return runtime.cast->character_event(player_gameplay_binding(),event);};
 s.animation=[](void*,std::uintptr_t,int sequence){if(sequence==-1){sequence=prince_state.animation_override;prince_state.animation_override=-1;}const dh2::character::Request q{dh2::character::set_animation,{sequence,0,0},0,0,0};character_service(nullptr,&prince_state,&q);return 0;};
 s.speed=[](void*,std::uintptr_t,float value){std::string error;return prince_locomotion.set_speed(value,error)?0:-1;};
 s.disable_heading=[](void*,std::uintptr_t){prince_runtime.controller.heading.active=0;prince_state.heading_active=0;return 0;};
 s.cancel_sneaking=[](void* p,std::uintptr_t){auto& runtime=*static_cast<PlayerSkillsRuntime*>(p);return runtime.player->native_cancel_sneaking(runtime.source_byte415);};
 s.animation_stop_loop=[](void*,std::uintptr_t,bool immediate){prince_locomotion.stop_loop(immediate);return 0;};
 s.network_enabled=[](void*,bool* out){std::uintptr_t identity=0;std::int32_t online=0;std::string error;if(!out||!EquipmentPlatform::query(equipment_platform.get(),dh2::player::EquipmentWorldQueryV1::online,0,identity,online,error))return -1;*out=online!=0;return 0;};
 t.cast=std::make_unique<dh2::android_ui::FaeryCastOwnerV2>(s);
}
