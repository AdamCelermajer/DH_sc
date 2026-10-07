static bool player_animation_step_fx_v2(void*,dh2::actor::BlendedPlayback&,const dh2::data::AnimationStep&,std::string&);
#include "renderer_authored_fx_anchor_v4.inc"
#include "renderer_target_marker_v28.inc"
// Include with other PlayerSkillsRuntime handoffs after source helpers.
int PlayerSkillsRuntime::combat_fx_rotation(void* raw,std::uintptr_t id,float out[3]){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 dh2::character::skills::WorldTargetActorBorrowV1 actual{};
 if(!out||!t.targets||t.targets->actor(id,&actual))return -1;
 const auto found=t.combat_fx_rotations.find(id);
 if(found==t.combat_fx_rotations.end()||!found->second)return -1;
 std::copy_n(found->second->rotation,3,out);return 0;
}
bool PlayerSkillsRuntime::combat_fx_remaining(void* raw,dh2::fx::MeshFxRequestV1& q,std::string& error){
 const int anchor=source_player_fx_anchor_v4(*static_cast<PlayerSkillsRuntime*>(raw),q,error);if(anchor)return anchor>0;
 if(q.operation==dh2::fx::MeshFxOperationV1::floor_normal){
  const auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
  if(!t.world||!equipment_platform||equipment_platform->world!=t.world){
   error="Required same live World floor query for CombatFX";return false;
  }
  // Original SyncIrrData492aa0 initializes unanchored normal to zero, invokes
  // PFWorld.GetFloorHeightAt525508(includeSpecial=false), ignores its hit flag
  // and discards the normal. q.point initially contains the actual FX position.
  const float* anchor_normal=nullptr;
  if(q.identity){
   dh2::character::skills::WorldTargetActorBorrowV1 actual{};
   if(!t.targets||t.targets->actor(q.identity,&actual)){error="Required registered PF normal anchor";return false;}
   if(t.world->player_object&&q.identity==t.world->player_object->identity)anchor_normal=prince_runtime.object.motion.normal;
   else {const auto found=t.world_actors.find(q.identity);
    if(found==t.world_actors.end()||!found->second.npc||!found->second.object||found->second.object->identity!=q.identity){error="Required SAME retained anchor PFObject normal1ec";return false;}
    anchor_normal=found->second.npc->runtime.object.motion.normal;
   }
  }
  float normal[3]{};
  if(!dh2::fx::character_fx_floor_sync_v29(level.native_floor?&level.native_floor->collision_world:nullptr,q.point,anchor_normal,normal,error))return false;
  std::copy_n(normal,3,q.point);return true;
 }
 error="Required original CombatFX anchor/floor continuation "+std::to_string(unsigned(q.operation));return false;
}
bool PlayerSkillsRuntime::combat_fx_asset(void* raw,const char* uri,
 std::vector<std::uint8_t>& bytes,std::string& error){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 if(!equipment_platform||equipment_platform->world!=t.world||!uri){error="Required original CombatFX asset owner";return false;}
 bool found=false;if(!equipment_platform->cache.read(uri,found,bytes,error))return false;
 if(!found){error="Required exact authored CombatFX resource "+std::string(uri);return false;}
 return true;
}
bool PlayerSkillsRuntime::combat_fx_camera(void*,float view[16],float position[3],std::string& error){
 if(!view||!position||submitted_width<=0||submitted_height<=0){error="Required actual submitted camera for particle FX";return false;}
 std::copy_n(submitted_view.data(),16,view);std::copy_n(submitted_eye.data(),3,position);return true;
}
bool PlayerSkillsRuntime::combat_fx_driver(void*,std::uint32_t& value,std::string&){
 // Original COpenGLES2Driver::getDriverType5aefa4 is literal8. This renderer
 // uses its actual GLES2 context; source onAnimate's (type&7)==0 writes white.
 value=8;return true;
}
void PlayerSkillsRuntime::bind_combat_fx(PlayerSkillsRuntime& t){
 using namespace dh2::character::skills;
 if(!t.world||!t.world->player_object||!t.targets||!t.world->effects_owner||!t.world->debug)
  throw std::runtime_error("Required same World/Effects/Debug CombatFX graph");
 // Destroy the old owner while its CPU Scene and borrowed services are alive.
 // Reload retains gameplay actors, but source resource/scene lifetimes change.
 clear_combat_fx_gpu(false);
 prince_locomotion.selection_fx_v2={};
 t.target_marker_v28.reset();
 t.combat_fx.reset();
 t.combat_fx_particles.reset();
 t.combat_fx_forces.reset();
 t.combat_fx_rotations.clear();
 if(!equipment_platform||!t.combat_sound_tables.load(read(equipment_platform->manager,"sounds_pyarray.bin","data"),t.error))throw std::runtime_error("Required combat sound tables: "+t.error);
 {
  std::vector<std::uint8_t> records,names;bool found=false;
  if(!equipment_platform->cache.read(dh2::audio::AudioSourceBindingsV38::records_uri,found,records,t.error)||!found)
   throw std::runtime_error("Required original generated sound bindings: "+t.error);
  if(!equipment_platform->cache.read(dh2::audio::AudioSourceBindingsV38::names_uri,found,names,t.error)||!found)
   throw std::runtime_error("Required original generated sound names: "+t.error);
  if(!t.audio_source_bindings_v38.load(records.data(),records.size(),names.data(),names.size(),t.error))
   throw std::runtime_error("Required original sound binding schema: "+t.error);
 }
 t.combat_fx_rotations.emplace(t.world->player_object->identity,&prince_runtime.rotation);
 for(auto& entry:t.world_actors){auto& actor=entry.second;
  if(actor.npc&&actor.object)
   t.combat_fx_rotations.emplace(actor.object->identity,&actor.npc->runtime.rotation);
 }
 t.combat_fx_forces=std::make_unique<dh2::fx::CharacterAuthoredFxForceFactoryOwnerV4>();
 t.combat_fx_particles=std::make_unique<dh2::fx::CharacterAuthoredResourceFactoryV6>(dh2::fx::CharacterBloodFxSceneServicesV2{&t,combat_fx_camera,combat_fx_driver},t.combat_fx_forces->factory());
 t.combat_fx=std::make_unique<CharacterCombatFxRuntimeV4>(*t.targets,
  t.world->effects_owner->borrow(),current_scene,*t.world->debug,t.world->debug_files,
  CombatFxRuntimeServicesV2{{&t,combat_fx_asset},t.combat_fx_particles->factory(),{&t,combat_fx_rotation},{&t,combat_fx_remaining}});
 if(t.combat_fx->initialize())throw std::runtime_error(t.combat_fx->error());
 initialize_target_marker_v28(t);
 prince_locomotion.selection_fx_v2={&t,player_animation_step_fx_v2};
}
int PlayerSkillsRuntime::combat_fx_application(const dh2::character::skills::SkillApplyRequestV6& q,
 const dh2::data::CombatResult& result){
 if(!combat_fx)return 0;
 const int status=combat_fx->application(q,result);
 if(status<0)error=combat_fx->error();return status;
}
int PlayerSkillsRuntime::combat_fx_frame(std::int32_t absolute_ms,std::int32_t app_dt){
 if(!combat_fx)return 0;
 if(!update_target_marker_v28(*this))return -2;
 if(combat_fx->frame(absolute_ms,app_dt)){error=combat_fx->error();return -2;}
 std::vector<dh2::fx::CharacterParticleDrawSourceV3> parts;
 if(!combat_fx->manager().particle_draw_sources_v3(parts,error))return -2;
 if(!equipment_platform||equipment_platform->world!=world){error="Required CombatFX GPU asset lifetime";return -2;}
 std::vector<dh2::fx::CharacterFxMeshDrawSourceV4> meshes;if(!combat_fx->manager().mesh_draw_sources_v4(meshes,error))return -2;
 std::vector<AuthoredEffectDrawSourceV5> actual;for(auto& p:parts)actual.push_back(authored_effect_source_v5(std::move(p)));for(auto& m:meshes)actual.push_back(authored_effect_source_v5(std::move(m)));
 try{sync_authored_combat_fx_draw_sources_v5(std::move(actual),equipment_platform->manager,equipment_platform->cache);}
 catch(const std::exception& e){error=e.what();return -2;}
 return 0;
}
