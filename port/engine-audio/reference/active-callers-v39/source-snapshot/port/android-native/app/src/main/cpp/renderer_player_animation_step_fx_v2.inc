// Bind once to the SAME retained PlayerSkillsRuntime; no extra FX manager.
static bool player_animation_step_fx_v2(void* raw,dh2::actor::BlendedPlayback& playback,
 const dh2::data::AnimationStep& step,std::string& error){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 if(&playback!=&prince_locomotion){error="Required same player CharAnimator effects leaf";return false;}
 if(prince_state.current==6){
  const auto index=playback.engine_index(step.anim);const auto* compiled=playback.compiled_clip_v4(step.anim);
  const auto bank=prince_attack_clips.find(step.anim);
  const auto use=compiled?dh2_events_time(&compiled->events.view(),"do_skill"):-1;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source skill step selection | clip %d | engine %d | range %d %d | do_skill %d | events %u | bank range %d %d | unbound %u | step FX %d | anchored %d",
   step.anim,index,compiled?compiled->start:-1,compiled?compiled->end:-1,use,compiled?compiled->events.view().count:0,
   bank==prince_attack_clips.end()?-1:bank->second.start,bank==prince_attack_clips.end()?-1:bank->second.end,
   bank==prince_attack_clips.end()?0:bank->second.unbound,step.fx,step.anchor_fx);
 }
 dh2::character::AnimationStepFxServicesV2 services{};services.context=&t;
 services.owner=[](void* p,std::uintptr_t& out){auto& r=*static_cast<PlayerSkillsRuntime*>(p);out=r.world->player_object->identity;return out==r.fsm.character?0:-1;};
 services.target_position=[](void* p,std::uintptr_t id,float* out){auto& r=*static_cast<PlayerSkillsRuntime*>(p);const float* actual=nullptr;if(!PlayerSkillsRuntime::heading_position(&r,id,actual,r.error))return -1;std::copy_n(actual,3,out);return 0;};
 services.rotation=PlayerSkillsRuntime::combat_fx_rotation;
 services.play=[](void* p,std::int32_t set,const float* position,const float* rotation,std::uintptr_t anchor){auto& r=*static_cast<PlayerSkillsRuntime*>(p);if(!r.combat_fx){r.error="Required retained player VisualFXManager";return -1;}return r.combat_fx->manager().play_set(set,position,rotation,anchor,nullptr,r.error)?0:-1;};
 services.swoosh_fx_gate=[](void* p,const dh2::data::AnimationStep& selected,bool& gate){
  auto& r=*static_cast<PlayerSkillsRuntime*>(p);dh2::character::AnimationSwooshServicesV4 sw{};sw.context=&r;
  sw.equipped=[](void* raw,std::int32_t kind,std::uintptr_t& id){auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
   if(!player_equipment||!player_equipment->inventory()){t.error="Required SAME equipped source inventory";return -1;}
   const auto& inv=*player_equipment->inventory();if(inv.character()!=t.world->player_object->identity||kind<0||kind>=9)return -1;
   const auto* slot=inv.equipment().at(inv.current_equipment()).at(kind);id=slot&&slot->item?reinterpret_cast<std::uintptr_t>(slot->item.get()):0;return 0;};
  sw.effects=[](void* raw,std::uintptr_t id,std::int32_t& sound,std::int32_t& fx){auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
   if(!player_equipment||!player_equipment->inventory())return -1;const auto& inv=*player_equipment->inventory();
   const auto found=std::find_if(inv.items().begin(),inv.items().end(),[&](const auto& slot){return slot&&reinterpret_cast<std::uintptr_t>(slot->item.get())==id;});
   if(found==inv.items().end()){t.error="Required same ItemInstance GetItem backing";return -1;}
   const auto* item=dh2::data::item(inv.table(),(*found)->item->id);if(!item)return -1;
   sound=item->record.words[5];fx=item->record.words[6];return 0;};
  sw.play_sound=[](void* raw,std::int32_t id){return source_animation_sound_v4(*static_cast<PlayerSkillsRuntime*>(raw),id);};
  sw.play_fx=[](void* raw,std::int32_t id,bool anchor){auto& t=*static_cast<PlayerSkillsRuntime*>(raw);if(!t.combat_fx)return -1;
   const auto owner=t.world->player_object->identity;const float* position=nullptr;float rotation[3]{};
   if(anchor){const auto& zero=dh2::world::canonical_vec3_origin_v1();return t.combat_fx->manager().play_set(id,zero.data(),nullptr,owner,nullptr,t.error)?0:-1;}
   if(!PlayerSkillsRuntime::heading_position(&t,owner,position,t.error)||PlayerSkillsRuntime::combat_fx_rotation(&t,owner,rotation))return -1;
   return t.combat_fx->manager().play_set(id,position,rotation,0,nullptr,t.error)?0:-1;};
  bool fallback_sound=false;std::string detail;const int status=dh2::audio::audio_animation_swoosh_v38(selected.anchor_fx,sw,fallback_sound,gate,detail);
  if(status){r.error=detail+"; "+r.error;return status;}if(fallback_sound)return source_animation_sound_v4(r,selected.sound);return 0;
 };
 if(!step.swoosh&&source_animation_sound_v4(t,step.sound)){error=t.error;return false;}
 std::string detail;const int status=dh2::character::character_animation_step_fx_v2(step,services,detail);
 if(status){error=detail;if(!t.error.empty())error+="; "+t.error;return false;}return true;
}
// Required insertion after source FX owner initialization and before gameplay
// animation selection: prince_locomotion.selection_fx_v2={&t,player_animation_step_fx_v2};
// Teardown BEFORE destroying t: prince_locomotion.selection_fx_v2={};
