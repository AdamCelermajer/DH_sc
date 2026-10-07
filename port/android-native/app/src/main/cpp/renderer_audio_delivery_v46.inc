// Audio-only shared endpoint, included before combat/animation sound fragments.
struct CapturedProducerAudioV46 {
 PlayerSkillsRuntime&t;dh2::audio::AudioApplicationBorrowV42 application;
 dh2::audio::AudioCategoryV46 category{dh2::audio::AudioCategoryV46::skill};
};
static int submit_captured_audio_v46(CapturedProducerAudioV46&captured,const dh2::character::CombatSoundPlayV1&play){
 return model_renderer::submit_campaign_audio_v46(captured.category,captured.application,captured.t.world,play,captured.t.error)?0:-1;
}
