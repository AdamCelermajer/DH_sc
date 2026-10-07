// Staged private include BEFORE combat, animation and target event services.
struct CapturedProducerAudioV42 {PlayerSkillsRuntime& t;dh2::audio::AudioApplicationBorrowV42 application;};
static int source_animation_request_v42(CapturedProducerAudioV42& captured,const dh2::character::CombatSoundPlayV1& play){
 return dh2::audio::audio_submit_application_v42(captured.application,captured.t.audio_source_clock_v42,play,captured.t.error)?0:-1;
}
