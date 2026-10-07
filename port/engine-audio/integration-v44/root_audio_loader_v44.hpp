#pragma once
#include "../integration-v42/application/native_audio_application_v42.hpp"
#include "../integration-v43/audio_stage0_sound_v43.hpp"
#include "../../level-loader/stage_loader_v46_early.hpp"
namespace dh2::android_audio {
inline bool bind_root_stage0_sound_v44(dh2::loader::Stage0ServicesV46&services,
 std::string&error,std::function<bool(int,std::string&)> disabled_song_control={}){
 dh2::audio::AudioApplicationBorrowV42 captured;
 if(!borrow_application_audio_v42(captured,error))return false;
 if(!captured.manager){error="Required constructed Application SoundManager before source Stage0";return false;}
 auto sound=std::make_shared<dh2::audio::AudioStage0SoundV43>();
 sound->captured=std::move(captured);sound->disabled_song_control=std::move(disabled_song_control);
 services.sound_manager_owner=sound->captured.manager;
 services.sound_manager_identity=sound->captured.identity();
 services.stop_all_sounds=[sound](std::uintptr_t receiver,int fade,std::string&e){return sound->stop_all(receiver,fade,e);};
 error.clear();return true;
}
// A closure for either Openable core or ContainerPresentation services after
// the complete-prefix patch. Actual getter retains its SAME receiver/table.
inline std::function<bool(std::string&)> bind_root_container_precache_v44(
 std::function<bool(int&,std::string&)> actual_get_sound){
 return [getter=std::move(actual_get_sound)](std::string&error){
  dh2::audio::AudioApplicationBorrowV42 captured;
  if(!borrow_application_audio_v42(captured,error))return false;
  if(!captured.manager){error.clear();return true;}
  if(!getter){error="Required SAME Container GetSound raw-field getter";return false;}
  int raw_uid{};if(!getter(raw_uid,error))return false;
  return captured.precache_raw_uid(raw_uid,error);
 };
}
}
