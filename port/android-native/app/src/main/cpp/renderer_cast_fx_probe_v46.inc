// Include after PlayerSkillsRuntime and the V46 cast trace. Opt-in, bounded,
// read-only probe. Call arm from real state6 focus; sample AFTER the one real
// combat_fx_frame. Never runs scene_frame/manager_frame/animation twice.
struct CastFxProbeV46 {PlayerSkillsRuntime* owner{};unsigned remaining{};};
static CastFxProbeV46 cast_fx_probe_v46;
static void arm_cast_fx_probe_v46(PlayerSkillsRuntime& t,bool enabled){
 cast_fx_probe_v46={enabled?&t:nullptr,enabled?12u:0u};
}
static void sample_cast_fx_probe_v46(PlayerSkillsRuntime& t){
 if(cast_fx_probe_v46.owner!=&t||!cast_fx_probe_v46.remaining||!t.combat_fx)return;
 const unsigned sample=13-cast_fx_probe_v46.remaining--;
 auto& manager=t.combat_fx->manager();
 source_cast_target_trace_v46(t,"fx.frame.sample");
 unsigned records=0;
 for(const auto& view:manager.views()){
  if(view.pooled||!view.state.visible)continue;
  if(records++>=4)break;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source cast FX state | sample %u | scene %u | identity %016llx | set %d | resource %s | anchor %016llx | local input %.9g %.9g %.9g | retained rotation %.9g %.9g %.9g | source bytes30/31 %u %u | timeline %d",
   sample,unsigned(scene_clock),static_cast<unsigned long long>(view.identity),view.set,view.uri.c_str(),static_cast<unsigned long long>(view.state.anchor),double(view.state.position[0]),double(view.state.position[1]),double(view.state.position[2]),double(view.state.rotation[0]),double(view.state.rotation[1]),double(view.state.rotation[2]),view.state.orient_once,view.state.orient_with_anchor,view.current_ms);
 }
 std::string error;std::vector<dh2::fx::CharacterParticleDrawSourceV3> particles;
 if(!manager.particle_draw_sources_v3(particles,error)){
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source cast FX probe metadata unavailable | particles | %s",error.c_str());
 }else{
  unsigned count=0;
  for(const auto& source:particles){
   if(count++==4)break;
   const auto& m=source.emitter_world;const auto& w=source.part.world;
   const auto& p=source.part.positions;
   // These are actual baked vertex bytes. This is not a guessed particle
   // center, actor origin or a new transform producer.
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source cast FX particle draw | sample %u | fx %016llx | particle %016llx | emitter %u | emitter world basis %.9g %.9g %.9g %.9g | emitter translation %.9g %.9g %.9g | draw world translation %.9g %.9g %.9g | first vertex known %u | first vertex %.9g %.9g %.9g",
    sample,static_cast<unsigned long long>(source.fx_identity),static_cast<unsigned long long>(source.particle_identity),source.emitter_node,double(m[0]),double(m[1]),double(m[4]),double(m[5]),double(m[12]),double(m[13]),double(m[14]),double(w[12]),double(w[13]),double(w[14]),unsigned(!p.empty()),!p.empty()?double(p[0][0]):0.,!p.empty()?double(p[0][1]):0.,!p.empty()?double(p[0][2]):0.);
  }
 }
 std::vector<dh2::fx::CharacterFxMeshDrawSourceV4> meshes;error.clear();
 if(!manager.mesh_draw_sources_v4(meshes,error)){
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source cast FX probe metadata unavailable | meshes | %s",error.c_str());
 }else{
  unsigned count=0;
  for(const auto& source:meshes){
   if(count++==4)break;const auto& w=source.part.world;
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source cast FX mesh draw | sample %u | fx %016llx | node %u | primitive %u | world basis %.9g %.9g %.9g %.9g | world translation %.9g %.9g %.9g",
    sample,static_cast<unsigned long long>(source.fx_identity),source.node,source.primitive,double(w[0]),double(w[1]),double(w[4]),double(w[5]),double(w[12]),double(w[13]),double(w[14]));
  }
 }
 if(!cast_fx_probe_v46.remaining)cast_fx_probe_v46.owner=nullptr;
}
