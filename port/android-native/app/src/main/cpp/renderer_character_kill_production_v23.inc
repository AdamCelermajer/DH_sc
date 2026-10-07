// Include after PlayerSkillsRuntime/WorldActor are complete. Required header:
// character_kill_production_v23.hpp. Root retains ONE production owner on the
// actual runtime; do not publish this Hit8 callback until reward routes bind.
static int renderer_character_kill_hit_v23(PlayerSkillsRuntime& t,
 dh2::character::CharacterKillProductionV23& production,
 dh2::character::HitActor32* actor,const dh2::character::HitRequest32* q,
 std::uintptr_t* out){
 if(!q||q->service!=dh2::character::hit_controller_kill)return 0;
 if(!actor||!out){t.error="Required actual Hit8 actor/result";return -1;}
 const auto found=t.world_actors.find(actor->identity);
 if(found==t.world_actors.end()||!found->second.object||!found->second.object->life||
    !found->second.object->properties||!actor->properties||
    actor->properties->resolved!=found->second.object->properties->resolved.data()){
  t.error="Required SAME Hit8 Character property/life graph";return -1;
 }
 // command owns scoped actual-Level pins, source event4 capability, and the
 // whole CtrlKill tail. Never reset dead/HP on a required prefix failure.
 const int status=production.command(q->subject,actor->identity,q->target,q->force);
 if(status!=1){t.error=production.error();return -1;}
 *out=0;return 1;
}
