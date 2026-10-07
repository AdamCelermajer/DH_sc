// Include after PlayerSkillsRuntime is complete, before prince_look_service.
// Forward ONLY actual position/heading services; original Cmd-vs-Ctrl gating
// remains at the Character state call site. No camera-follow mutation.
static int player_target_facing_service_v38(const dh2::character::CharacterControlRequest32* q,
 dh2::character::CharacterControlResponse16* out){
 if(!q||!out||!player_skills_runtime||!player_skills_runtime->targets){return -1;}
 auto& t=*player_skills_runtime;auto s=t.targets->targets().control_services_v38();
 const int result=s.invoke(s.context,q,out);
 if(result!=1)t.error=t.targets->targets().error();return result;
}
static int player_target_close_range_service_v38(void* p,std::uintptr_t owner,std::uintptr_t target,std::uint32_t* out){
 if(!p||!out)return -1;auto& t=*static_cast<PlayerSkillsRuntime*>(p);std::int32_t value;
 if(!t.attack_geometry||!t.world||!t.world->player_object||owner!=t.world->player_object->identity)return -1;
 if(!t.attack_geometry->close_range_v38(owner,target,t.world->player_object->target.target,value,t.error))return -1;
 std::memcpy(out,&value,4);return 0;
}
// bind_target_frame insertion: services.close_range=player_target_close_range_service_v38;
