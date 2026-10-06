// PlayerSkillsRuntime member:
// dh2::character::CharacterCombatSoundTablesV2 combat_sound_tables;
// Load exact data/sounds_pyarray.bin once on the same World bind, preserving it
// across actor frames. No sound-table IDs are inferred from names or classes.
int combat_sound_application_v2(PlayerSkillsRuntime& t,
 const dh2::character::skills::SkillApplyRequestV6& q,const dh2::data::CombatResult& result){
 using namespace dh2::character;using namespace dh2::character::skills;
 if(q.service!=skill_apply_sound_v6)return 0;
 CombatSoundServicesV1 s{};s.context=&t;
 s.row=[](void* p,std::uintptr_t id,const CombatSoundRowV1** out){auto& r=*static_cast<PlayerSkillsRuntime*>(p);
  auto actor=r.world_actors.find(id);if(!out||actor==r.world_actors.end()||!actor->second.object||!actor->second.object->properties)return -1;
  *out=r.combat_sound_tables.get(actor->second.object->properties->resolved[9]);return *out?0:-1;};
 s.minimal_randoms=[](void* p,std::uint32_t* out){auto& r=*static_cast<PlayerSkillsRuntime*>(p);if(!out||!r.world->debug)return -1;
  if(dh2_character_debug_load(r.world->debug.get(),&r.world->debug_files)!=1)return -1;
  return dh2_character_debug_get(out,r.world->debug.get(),"MP_MinimalRandoms",&r.world->debug_files)==1?0:-1;};
 s.dead=[](void* p,std::uintptr_t id,bool* out){auto& r=*static_cast<PlayerSkillsRuntime*>(p);WorldTargetActorBorrowV1 a{};
  if(!out||!r.targets||r.targets->actor(id,&a)||!a.life)return -1;*out=a.life->dead!=0;return 0;};
 s.manager=[](void* p,std::uintptr_t* out){auto& r=*static_cast<PlayerSkillsRuntime*>(p);
  // Same retained Vox music-domain projection identity. This does not assert
  // native bank/device initialization; Play3D checks actual Level phase first.
  if(!out||!r.vox_music)return -1;*out=r.vox_music->identity();return 0;};
 s.random=[](void*,std::uint32_t count,std::uint32_t* out){if(!out)return -1;*out=dh2_combat_random(&combat_random,count);return 0;};
 s.position=[](void* p,std::uintptr_t id,std::array<float,3>* out){auto& r=*static_cast<PlayerSkillsRuntime*>(p);WorldTargetActorBorrowV1 a{};
  if(!out||!r.targets||r.targets->actor(id,&a)||!a.target_node)return -1;
  if(*a.target_node&&!a.target_enabled)return -1;
  const float* position=dh2_world_target_position_v1(a.position,a.cached_target_position,*a.target_node,a.target_enabled?*a.target_enabled:0);
  if(!position)return -1;std::copy_n(position,3,out->begin());return 0;};
 s.play=[](void* p,const CombatSoundPlayV1* play){auto& r=*static_cast<PlayerSkillsRuntime*>(p);if(!play)return -1;
  dh2::sound::VoxPlay3DOwnerV2 owner({&r,[](void* raw,const dh2::sound::VoxPlay3DRequestV2& request,dh2::sound::VoxPlay3DResponseV2& out){
   auto& runtime=*static_cast<PlayerSkillsRuntime*>(raw);using O=dh2::sound::VoxPlay3DOperationV2;
   if(!runtime.vox_music||request.play->manager!=runtime.vox_music->identity())return -1;
   if(request.operation==O::disabled){std::uint32_t value=0;if(dh2_character_debug_load(runtime.world->debug.get(),&runtime.world->debug_files)!=1||dh2_character_debug_get(&value,runtime.world->debug.get(),"IsDisablingSounds",&runtime.world->debug_files)!=1)return -1;out.value=std::int32_t(value);return 0;}
   if(request.operation==O::current_level){out.identity=reinterpret_cast<std::uintptr_t>(&level);out.value=std::int32_t(runtime.world->source_level_load_phase);return 0;}
   runtime.error="Required original Vox Play3D reached operation "+std::to_string(unsigned(request.operation));return -1;
  }});
  const int status=owner.play(*play);if(status)r.error=owner.error()+"; "+r.error;return status;};
 CombatSoundOutputV1 out{};const int status=character_combat_sound_v1(&out,&result,q.attacker,q.target,true,&s);
 if(status){t.error="Required source combat sound phase "+std::to_string(out.phase)+"; "+t.error;return -2;}return 1;
}
