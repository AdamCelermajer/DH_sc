// Included by model_renderer.cpp after prince_event and PlayerSkillsRuntime.
// See renderer_player_attack_v1_handoff.md for declarations and call sites.
int PlayerSkillsRuntime::attack_inventory(void* p,std::uintptr_t id,
 dh2::character::skills::WorldAttackInventoryBorrowV1* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!out)return -1;
 const auto at=t.world_actors.find(id);if(at==t.world_actors.end()||!at->second.object)return -1;
 if(id==t.fsm.character){
  if(!player_equipment||!player_equipment->ready()||!player_equipment->inventory())return -1;
  *out={};out->owned=player_equipment->inventory();return 0;
 }
 const auto inventory=t.attack_npc_inventories.find(id);
 if(inventory==t.attack_npc_inventories.end()||!inventory->second)return -1;
 *out=inventory->second->borrow();return 0;
}
int PlayerSkillsRuntime::attack_kind(void* p,std::uintptr_t id,std::int32_t* out){
 auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!out)return -1;
 dh2::character::skills::WorldTargetActorBorrowV1 a{};
 if(t.targets->actor(id,&a)||!a.character)return -1;
 // SAME native ObjectBase domain as CharacterWorldRuntimeV1::world_ai_kind.
 // This callback is only used after original ObjectHandle.AsCharacter.
 *out=0;return 0;
}
int PlayerSkillsRuntime::attack_backend(void* p,
 const dh2::character::AttackRequest32* q,dh2::character::AttackResponse16* out){
 using namespace dh2::character;auto& t=*static_cast<PlayerSkillsRuntime*>(p);
 if(!q||!out||!t.world||!t.targets)return -1;
 switch(q->service){
 case attack_network_mode:
  if(!equipment_platform||equipment_platform->world!=t.world)return -1;
  out->word=equipment_platform->online_mode;return 0;
 case attack_frontal_angle:{
  const auto* design=t.world->tables.design();std::int32_t value{};
  if(!design||!design->lookup||design->lookup(design->context,0,"CharacterDesign","Attack_FrontalAngle",&value))return -1;
  std::memcpy(&out->word,&value,4);return 0;
 }
 case attack_diagnostic:{
  const char* key=nullptr;
  switch(q->argument0){
   case 0x3d0294:case 0x3d03c0:key="isTracingCharAICommands";break;
   case 0x3d0354:case 0x3d0630:case 0x3d06b4:case 0x3d0538:key="isTracingChar_MeleePotentialTarget";break;
   default:return -1;
  }
  if(dh2_character_debug_load(t.world->debug.get(),&t.world->debug_files)!=1||
   dh2_character_debug_get(&out->word,t.world->debug.get(),key,&t.world->debug_files)!=1)return -1;
  return 0;
 }
 case attack_set_attack_state:{
  if(q->subject!=t.fsm.character)return -1;
  // Original SM_SetAttackState routes c354 to this SAME source machine.
  // prince_event owns its real State gate/focus/body/animation continuation.
  const int result=prince_event(0xc354,q->payload);
  if(result<0)return -1;out->word=std::uint32_t(result);return 0;
 }
 case attack_range_redirect:
  t.error="Required original AI_DoRangeAttack controller/sequence/projectile continuation";return -1;
 case attack_network_send:
  t.error="Required source attack network packet queue continuation";return -1;
 default:t.error="Required source attack backend "+std::to_string(q->service);return -1;
 }
}
void PlayerSkillsRuntime::bind_attack(PlayerSkillsRuntime& t){
 using namespace dh2::character;using namespace dh2::character::skills;
 if(!t.world||!t.world->player_object||!t.targets||!t.world->tables.ai()||!t.world->debug)
  throw std::runtime_error("Required same player/world attack owners");
 const auto id=t.world->player_object->identity;
 // These are the recovered constructor stores only. The source animator owns
 // index/last/finisher and their later writes; no attack-entry reset is added.
 t.attack_fields.owner=id;t.attack_fields.object_of_interest=0;
 t.attack_fields.object_of_interest_type=-1;t.attack_fields.seeking=1;
 t.attack_npc_inventories.clear();
 for(const auto& entry:t.world_actors)if(entry.first!=id&&entry.second.npc){
  const auto* resolved=entry.second.object->properties->resolved.data();
  t.attack_npc_inventories.emplace(entry.first,std::make_unique<CharacterAttackEmptyInventoryV1>(resolved));
 }
 t.attack_geometry=std::make_unique<CharacterWorldAttackGeometryV1>(*t.targets,
  *t.world->tables.ai(),*t.world->debug,t.world->debug_files,
  WorldAttackGeometryServicesV1{&t,attack_inventory,attack_kind,nullptr});
 t.attack_controller.controllable=reinterpret_cast<std::uintptr_t>(&prince_runtime.controller);
 t.attack_controller.character=id;
 t.attack_owner=std::make_unique<CharacterWorldPlayerAttackOwnerV1>(*t.targets,
  t.attack_fields,t.world->player_object->target,t.attack_controller,prince_state,
  t.world->player_object->binding.services,
  PlayerAttackBackendsV1{&t,attack_backend,t.attack_geometry->queries(),
   [](void* p,std::uintptr_t id,float& value,std::string& error){
    auto& same=*static_cast<PlayerSkillsRuntime*>(p);
    return same.attack_geometry&&same.attack_geometry->melee_radius(id,value,error);
   }});
}
// Replace the old nearest-development-target player_attack body with this call.
// Explicit inspected object IDs are mapped by the existing actual actor lookup;
// absent target is passed as null to original source acquisition.
std::string source_player_attack_v1(int supplied_target){
 if(!world_mode||!native_actor_ready||!player_skills_runtime||!player_skills_runtime->attack_owner)
  return "Player attack owner unavailable";
 auto& t=*player_skills_runtime;
 std::uintptr_t requested{};
 if(supplied_target>=0){auto* actor=player_target(supplied_target);if(!actor)return "Selected target unavailable";requested=actor->identity;}
 t.attack_controller.blocked=controller_global_blocked;
 t.attack_controller.locked=prince_state.controller_locked;
 t.attack_controller.forced=prince_controller_forced;
 t.error.clear();
 const int result=t.attack_owner->command(requested);
 if(result)return "Attack source provider required: "+t.attack_owner->error()+(t.error.empty()?"":"; "+t.error);
 __android_log_print(ANDROID_LOG_INFO,"DH2Native",
  "Original player attack command | request %llu | target %llu | last %llu | state %d | root %d | clip %d | same World/Gear/Target/FSM",
  static_cast<unsigned long long>(requested),static_cast<unsigned long long>(t.world->player_object->target.target),
  static_cast<unsigned long long>(t.world->player_object->target.last_target),prince_state.current,
  prince_state.current_animation,prince_locomotion.current_clip());
 return prince_state.current==5?"Attacking":"Attack command completed";
}
