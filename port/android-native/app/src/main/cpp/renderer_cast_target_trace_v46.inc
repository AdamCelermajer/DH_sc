// Include after PlayerSkillsRuntime is complete, before cast/step FX includes.
// Event-bound read-only evidence; no per-frame output, setters or fake target.
static void source_cast_target_trace_v46(PlayerSkillsRuntime& t,const char* phase){
 if(!t.world||!t.world->player_object)return;
 const auto& ai=t.world->player_object->target;
 const auto ooi=t.world->player_canonical?t.world->player_canonical->ooi14a4:0;
 const auto log=[&](const char* selector,std::uintptr_t identity){
  const auto found=t.world_actors.find(identity);
  const auto* object=found==t.world_actors.end()?nullptr:found->second.object.get();
  const bool props=object&&object->properties;
  dh2::character::skills::WorldTargetActorBorrowV1 actor{};
  const bool borrowed=identity&&t.targets&&t.targets->actor(identity,&actor)==0;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source cast target trace | phase %s | selector %s | target %016llx | registered %u | life known %u | dead %u | HP known %u | HP36 %d | current %016llx | last %016llx | OOI %016llx | projection %016llx %016llx | state %d | heading %.9g | raw rotation %.9g %.9g %.9g | visual root %.9g %.9g %.9g | root quaternion %.9g %.9g %.9g %.9g",
   phase,selector,static_cast<unsigned long long>(identity),unsigned(borrowed),unsigned(borrowed&&actor.life),unsigned(borrowed&&actor.life?actor.life->dead:0),unsigned(props),props?object->properties->resolved[36]:0,
   static_cast<unsigned long long>(ai.target),static_cast<unsigned long long>(ai.last_target),static_cast<unsigned long long>(ooi),static_cast<unsigned long long>(t.skill_state.target),static_cast<unsigned long long>(t.skill_state.last_target),prince_state.current,
   double(prince_runtime.rotation.heading_angle),double(prince_runtime.rotation.rotation[0]),double(prince_runtime.rotation.rotation[1]),double(prince_runtime.rotation.rotation[2]),
   double(prince_visual.root.position[0]),double(prince_visual.root.position[1]),double(prince_visual.root.position[2]),double(prince_visual.root.quaternion[0]),double(prince_visual.root.quaternion[1]),double(prince_visual.root.quaternion[2]),double(prince_visual.root.quaternion[3]));
 };
 log("current408",ai.target);
 if(ai.last_target!=ai.target)log("last40c",ai.last_target);
 if(ooi!=ai.target&&ooi!=ai.last_target)log("OOI14a4",ooi);
}
static void source_cast_fx_play_trace_v46(PlayerSkillsRuntime& t,std::int32_t set,
 std::uintptr_t anchor,const float* position,const float* rotation){
 if(prince_state.current!=6)return;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source cast step FX input | set %d | anchor %016llx | position known %u | position %.9g %.9g %.9g | rotation supplied %u | rotation %.9g %.9g %.9g",
  set,static_cast<unsigned long long>(anchor),unsigned(position!=nullptr),position?double(position[0]):0.,position?double(position[1]):0.,position?double(position[2]):0.,unsigned(rotation!=nullptr),rotation?double(rotation[0]):0.,rotation?double(rotation[1]):0.,rotation?double(rotation[2]):0.);
 source_cast_target_trace_v46(t,"step.fx.play");
}
