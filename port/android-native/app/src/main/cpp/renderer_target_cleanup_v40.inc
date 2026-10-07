// Include AFTER renderer_player_target_frame_v2.inc and NPC command/aggro
// adapters. Add a prototype before target-frame include; its clear_aggro case
// calls this function. No duplicate target/map/heading/death publication.
static int player_target_clear_aggro_v40(PlayerSkillsRuntime& t,std::uintptr_t owner,std::uintptr_t other){
 using namespace dh2::character;
 WorldClearAggroServicesV40 s{};s.context=&t;
 s.actor=[](void* raw,std::uintptr_t id,WorldClearAggroActorV40& out,std::string& error){
  auto& r=*static_cast<PlayerSkillsRuntime*>(raw);
  if(!r.world||!r.targets||r.world_actors.find(id)==r.world_actors.end()){error="Required SAME registered target cleanup actor";return false;}
  if(r.world->player_object&&id==r.world->player_object->identity){
   PlayerTargetDiedBorrowV2 b{};if(player_target_died_borrow_v2(&r,PlayerTargetDiedPhaseV2::controller,&b)){error="Required actual player cleanup controller";return false;}
   out={id,&prince_combat.aggro.outgoing_table,&prince_combat.aggro.incoming_table,&r.world->player_object->binding,b.controller.controller,r.world};return true;
  }
  for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.identity==id&&actor.script){
   auto& m=*actor.script;if(!m.object||!m.controller||m.object->identity!=id){error="Required actual retained NPC cleanup receiver";return false;}
   out={id,&actor.aggro.outgoing_table,&actor.aggro.incoming_table,&m.object->binding,m.controller->identity(),m.context};return true;
  }
  error="Required canonical actor threat/TargetBindings/controller cleanup producer";return false;
 };
 s.on_deaggro=[](void* raw,std::uintptr_t receiver,std::uintptr_t other,const dh2_script_callback_scope*,std::string& error){auto& r=*static_cast<PlayerSkillsRuntime*>(raw);if(player_aggro_callback_v2(&r,dh2::data::aggro_notify_target_cleared,other,receiver)){error=r.error;return false;}return true;};
 s.command_stop=[](void* raw,std::uintptr_t id,std::uintptr_t controller,const dh2_script_callback_scope* scope,std::string& error){
  auto& r=*static_cast<PlayerSkillsRuntime*>(raw);
  if(id==r.world->player_object->identity){PlayerTargetDiedBorrowV2 b{};if(player_target_died_borrow_v2(&r,PlayerTargetDiedPhaseV2::controller,&b)||b.controller.controller!=controller||!b.control){error="Required live same player Cmd_Stop controller";return false;}if(dh2_character_controller_character(&b.controller,controller_stop,0,b.control)!=1){error=r.error;return false;}return true;}
  for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.identity==id&&actor.script){auto& m=*actor.script;
   if(!m.controller||m.controller->identity()!=controller||!m.script_commands||RendererNpcCommandsV1::refresh(m.script_commands.get())){error="Required live same NPC Cmd_Stop graph";return false;}
   auto& b=m.script_commands->owner().bindings();const auto* old=b.scope;struct Restore{ScriptCommandBindings40& b;const dh2_script_callback_scope* old;~Restore(){b.scope=old;}} restore{b,old};b.scope=scope;
   const ScriptCommandRequest40 q{script_controller_stop,0,controller,0,{},0};const float* out{};
   if(b.services.invoke(b.services.context,b.state,&q,&out)){error=m.script_commands->owner().error();return false;}return true;
  }
  error="Required target cleanup Cmd_Stop retained actor";return false;
 };
 CharacterWorldClearAggroV40 cleanup(s);const auto* scope=t.world&&t.world->player_object?t.world->player_object->binding.scope:nullptr;
 const int result=cleanup.clear(owner,other,scope);if(result)t.error=cleanup.error();return result;
}
