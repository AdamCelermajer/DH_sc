// Include with other PlayerSkillsRuntime handoffs after source helpers.
int PlayerSkillsRuntime::combat_fx_rotation(void* raw,std::uintptr_t id,float out[3]){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 dh2::character::skills::WorldTargetActorBorrowV1 actual{};
 if(!out||!t.targets||t.targets->actor(id,&actual))return -1;
 const auto found=t.combat_fx_rotations.find(id);
 if(found==t.combat_fx_rotations.end()||!found->second)return -1;
 std::copy_n(found->second->rotation,3,out);return 0;
}
bool PlayerSkillsRuntime::combat_fx_remaining(void*,dh2::fx::MeshFxRequestV1& q,std::string& error){
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
void PlayerSkillsRuntime::bind_combat_fx(PlayerSkillsRuntime& t){
 using namespace dh2::character::skills;
 if(!t.world||!t.world->player_object||!t.targets||!t.world->effects_owner||!t.world->debug)
  throw std::runtime_error("Required same World/Effects/Debug CombatFX graph");
 // Destroy the old owner while its CPU Scene and borrowed services are alive.
 // Reload retains gameplay actors, but source resource/scene lifetimes change.
 t.combat_fx.reset();
 t.combat_fx_rotations.clear();
 t.combat_fx_rotations.emplace(t.world->player_object->identity,&prince_runtime.rotation);
 t.combat_fx=std::make_unique<CharacterCombatFxRuntimeV1>(*t.targets,
  t.world->effects_owner->borrow(),current_scene,*t.world->debug,t.world->debug_files,
  CombatFxRuntimeServicesV1{{&t,combat_fx_asset},{&t,combat_fx_rotation},{&t,combat_fx_remaining}});
 if(t.combat_fx->initialize())throw std::runtime_error(t.combat_fx->error());
}
int PlayerSkillsRuntime::combat_fx_application(const dh2::character::skills::SkillApplyRequestV6& q,
 const dh2::data::CombatResult& result){
 if(!combat_fx)return 0;
 const int status=combat_fx->application(q,result);
 if(status<0)error=combat_fx->error();return status;
}
int PlayerSkillsRuntime::combat_fx_frame(std::int32_t absolute_ms,std::int32_t app_dt){
 if(!combat_fx)return 0;
 if(combat_fx->frame(absolute_ms,app_dt)){error=combat_fx->error();return -2;}
 std::vector<dh2::skinning::VisualDrawPartV6> parts;
 if(combat_fx->draw_parts(parts)){error=combat_fx->error();return -2;}
 if(!equipment_platform||equipment_platform->world!=world){error="Required CombatFX GPU asset lifetime";return -2;}
 try{sync_combat_fx_draws(std::move(parts),equipment_platform->manager,equipment_platform->cache);}
 catch(const std::exception& e){error=e.what();return -2;}
 return 0;
}
