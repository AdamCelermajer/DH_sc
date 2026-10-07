#pragma once
#include "../integration-v42/audio_application_manager_v42.hpp"
#include <functional>
namespace dh2::audio {
struct AudioStage0SoundV43 {
 AudioApplicationBorrowV42 captured;
 // Original disabled branch calls531940(-1); missing real receiver required.
 std::function<bool(int,std::string&)> disabled_song_control;
 bool stop_all(std::uintptr_t actual_receiver,int fade_ms,std::string&error)const{
  if(!captured.manager||actual_receiver!=captured.identity()){error="Required SAME captured Application SoundManager for Stage0";return false;}
  if(fade_ms!=500){error="Required exact source Stage0 StopAllSounds(500) argument";return false;}
  if(captured.manager->disabled()){
   if(!disabled_song_control){error="Required actual disabled StopAllSounds song control531940(-1)";return false;}
   return disabled_song_control(-1,error);
  }
  auto*runtime=captured.manager->runtime_on_producer();
  if(!runtime){error="Required same active Application audio producer";return false;}
  AudioCommandV34 command;command.kind=AudioCommandKindV34::stop_all;
  AudioDeviceClockV40 clock;
  if(runtime->clock().snapshot(clock)&&clock.ready&&clock.rate>=8000&&clock.rate<=192000){
   // Original i2f(500)/1000.0 ==0.5 seconds exactly. Real output rate.
   command.fade_frames=clock.rate/2;
  }else command.source_fade_seconds=.5f;
  // Duration-only StopAll needs no authored event/device timestamp. If focus
  // invalidated the clock, the SAME callback converts .5s using its real rate
  // when it resumes; samples remain pinned until terminal receipts are drained.
  command.start_frame=runtime->mixer().output_frame();
  if(!runtime->mixer().post(command)){error="Stage0 source stop queue full; retain owner and loading state";return false;}
  error.clear();return true;
 }
};
}
