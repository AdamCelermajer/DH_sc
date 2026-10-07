// Original CharAnimator callsite defaults: false, integer1, floats-1/-1.
// Uses SAME retained Vox and exact owner target position; reached bank/audio
// continuations stay mandatory, sharing the current combat sound boundary.
static int source_animation_sound_v4(PlayerSkillsRuntime& t,std::int32_t id){
 if(!t.world||!t.world->player_object){t.error="Required same animation Vox/Character owner";return -1;}
 CapturedProducerAudioV46 captured{t,{},dh2::audio::AudioCategoryV46::attack};
 if(!borrow_actual_application_audio_v42(captured.application,t.error))return -1;
 const float* position=nullptr;if(!PlayerSkillsRuntime::heading_position(&t,t.world->player_object->identity,position,t.error))return -1;
 return submit_captured_audio_v46(captured,dh2::audio::audio_world_request_v38(captured.application.identity(),0,id,{position[0],position[1],position[2]}));
}
static int source_named_animation_sound_v38(PlayerSkillsRuntime& t,std::uintptr_t character,const char* suffix){
 if(t.audio_source_bindings_v38.rows().empty()){t.error="Required generated source sound-name table";return -1;}
 CapturedProducerAudioV46 captured{t,{},t.world&&t.world->player_object&&t.world->player_object->identity==character?dh2::audio::AudioCategoryV46::skill:dh2::audio::AudioCategoryV46::monster};
 dh2::audio::AudioNamedAnimationSoundServicesV38 services{};services.context=&captured;services.actual_character=character;
 services.manager=[](void* p,std::uintptr_t& out){auto& c=*static_cast<CapturedProducerAudioV46*>(p);if(!borrow_actual_application_audio_v42(c.application,c.t.error))return -1;out=c.application.identity();return 0;};
 services.target_position=[](void* p,std::uintptr_t id,std::array<float,3>& out){auto& r=static_cast<CapturedProducerAudioV46*>(p)->t;const float* position=nullptr;
  if(!PlayerSkillsRuntime::heading_position(&r,id,position,r.error))return -1;std::copy_n(position,3,out.begin());return 0;};
 services.play=[](void* p,const dh2::character::CombatSoundPlayV1& request){return submit_captured_audio_v46(*static_cast<CapturedProducerAudioV46*>(p),request);};
 dh2::audio::AudioNamedAnimationSoundResultV38 result{};
 const int status=dh2::audio::audio_named_animation_sound_v38(suffix,t.audio_source_bindings_v38,services,result);
 if(status)t.error=std::string(result.required?result.required:"Required original named animation sound")+" phase "+std::to_string(result.phase)+"; "+t.error;
 return status;
}
