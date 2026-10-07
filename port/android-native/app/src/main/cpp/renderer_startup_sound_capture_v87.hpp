#pragma once
#include <source_startup_sound_v87.hpp>
#include <application/native_audio_application_v42.hpp>
namespace model_renderer {
// Bind the existing actual process SoundManager. Native absence is delivered
// only by that singleton borrower, never inferred from missing input services.
template<class Self,class Startup>
void bind_startup_sound_capture_v87(std::weak_ptr<Self> weak,Startup& output){
 if(output.borrow_sound_manager)return;
 output.borrow_sound_manager=[weak](dh2::world::StartupSoundManagerBorrowV87& out,std::string& e){
  out={};auto self=weak.lock();if(!self||!self->current(e))return false;
  dh2::audio::AudioApplicationBorrowV42 actual;
  if(!dh2::android_audio::borrow_application_audio_v42(actual,e))return false;
  if(!self->current(e))return false;
  if(!actual.manager){e.clear();return true;}
  auto manager=actual.manager;out.receiver=manager;out.identity=actual.identity();
  out.load_sound=[weak,manager=std::weak_ptr<dh2::audio::AudioApplicationManagerV42>(manager)](std::int32_t uid,std::string& e){
   auto self=weak.lock();auto actual=manager.lock();if(!self||!self->current(e)||!actual){if(e.empty())e="Released actual startup SoundManager snapshot";return false;}
   // The retained snapshot deliberately does not reread the global after
   // source data/table or sound callbacks. It is exactly the captured owner.
   if(!actual->precache_raw_uid(uid,e))return false;return self->current(e);
  };
  e.clear();return true;
 };
}
}
