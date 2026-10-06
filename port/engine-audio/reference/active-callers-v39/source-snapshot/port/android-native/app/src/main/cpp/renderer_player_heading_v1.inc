// See renderer_player_heading_v1_handoff.md: include after PlayerSkillsRuntime.
bool PlayerSkillsRuntime::heading_position(void* raw,std::uintptr_t id,const float*& out,std::string& error){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);dh2::character::skills::WorldTargetActorBorrowV1 actor{};
 if(!t.targets||t.targets->actor(id,&actor)||!actor.position||!actor.target_node){error="Required actual GetTargetPosition actor backing";return false;}
 const auto node=*actor.target_node;
 if(node&&!actor.target_enabled){error="Required actual target enabled byte";return false;}
 out=dh2::character::skills::dh2_world_target_position_v1(actor.position,actor.cached_target_position,node,actor.target_enabled?*actor.target_enabled:0);
 if(!out){error="Required actual selected target position backing";return false;}return true;
}
bool PlayerSkillsRuntime::heading_remote(void* raw,bool& out,std::string&){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 out=t.combat_fields.network_id!=-1||t.heading_remote_updated_byte!=0;return true;
}
bool PlayerSkillsRuntime::heading_physics(void*,bool& out,std::string&){out=(prince_state.flags&2)!=0;return true;}
bool PlayerSkillsRuntime::heading_event(void* raw,std::uint32_t event,std::string& error){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 // Refresh all compatibility projections BEFORE synchronous original relay.
 prince_runtime.rotation.heading_angle=prince_runtime.controller.heading.angle;
 prince_state.heading_active=prince_runtime.controller.heading.active;refresh_prince_facts();
 t.event_owner.forced=prince_controller_forced;t.event_owner.locked=prince_state.controller_locked;
 t.events.global_blocked=controller_global_blocked;
 const dh2::character::AIEventPayload24 payload{};dh2::character::AIEventResult16 result{};
 const dh2::character::AIEventServices24 services{&t,PlayerSkillsRuntime::event_service,63,0};
 if(dh2_character_ai_event(&result,&t.events,event,&payload,&services)){error="Required Character heading RaiseAIEvent: "+t.error;return false;}return true;
}
void PlayerSkillsRuntime::bind_heading(PlayerSkillsRuntime& t){
 if(!t.world||!t.world->player_object)throw std::runtime_error("Required same Character heading graph");
 t.heading_command={reinterpret_cast<std::uintptr_t>(&prince_runtime.controller),t.world->player_object->identity,controller_global_blocked,prince_state.controller_locked,prince_controller_forced,0};
 t.heading_body=prince_body.body?&prince_body:nullptr;
 t.heading_owner=std::make_unique<dh2::character::CharacterHeadingOwnerV1>(t.heading_command,prince_state,t.world->player_object->target,t.world->player_object->binding.services,
  prince_runtime.controller,prince_runtime.path,prince_runtime.subobjects.position,prince_runtime.subobjects.destination,t.heading_body,
  dh2::character::CharacterHeadingServicesV1{&t,heading_position,heading_remote,heading_physics,heading_event});
}
bool source_player_heading_v1(const float* scaled_heading,bool stop,std::string& error){
 if(!player_skills_runtime||!player_skills_runtime->heading_owner){error="Required original player heading owner";return false;}
 auto& t=*player_skills_runtime;t.heading_body=prince_body.body?&prince_body:nullptr;
 t.heading_command.global_blocked=controller_global_blocked;t.heading_command.locked=prince_state.controller_locked;t.heading_command.forced=prince_controller_forced;
 const bool result=stop?t.heading_owner->command_stop(error):t.heading_owner->command_head_towards(scaled_heading,error);
 prince_runtime.rotation.heading_angle=prince_runtime.controller.heading.angle;
 prince_state.heading_active=prince_runtime.controller.heading.active;refresh_prince_facts();return result;
}
