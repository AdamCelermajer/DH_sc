#include "linux_sdl2_source_session_control_v1.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

using namespace dh::foundation::audio;
using namespace dh2::audio;

int main() {
 setenv("SDL_AUDIODRIVER","dummy",1);
 auto mixer=std::make_unique<AudioMixerV34>();AudioClockV40 clock;AudioLifecycleGateV40 gate;std::string error;
 if(!gate.publish_activity(1,1,false,false,false,false)){std::cerr<<"Initial unfocused activity rejected\n";return 2;}
 const auto epoch=gate.begin_source();
 if(!epoch||!gate.publish_source(epoch,true)||gate.permitted_for(epoch)){std::cerr<<"Source gate setup failed\n";return 3;}
 auto factory=linux_sdl2_source_session_control_v1();
 if(!factory.valid()){std::cerr<<"Linux SDL2 factory is invalid\n";return 4;}
 if(factory.construct(nullptr,*mixer,clock,gate,error)||error.empty()){
  std::cerr<<"Invalid factory context was accepted\n";return 5;
 }
 auto control=factory.construct(factory.context,*mixer,clock,gate,error);
 if(!control||!control->tick(error)||control->ready()){
  std::cerr<<"Initial unfocused control tick failed: "<<error<<'\n';return 6;
 }
 AudioDeviceClockV40 initially_paused;
 if(!clock.snapshot(initially_paused)||initially_paused.ready||mixer->output_frame()!=0){
  std::cerr<<"Unfocused startup published a clock or consumed mixer frames\n";return 7;
 }
 if(!gate.publish_activity(1,2,true,true,true,false)||!control->tick(error)||!control->ready()){
  std::cerr<<"Initial focus acquisition failed: "<<error<<'\n';return 8;
 }
 AudioDeviceClockV40 focused;
 if(!clock.snapshot(focused)||!focused.ready||focused.generation==0||focused.rate!=48000){
  std::cerr<<"Focused clock was not published\n";return 9;
 }
 const auto frame_before_pause=mixer->output_frame();
 if(!gate.publish_activity(1,3,false,false,false,false)||!control->tick(error)||control->ready()){
  std::cerr<<"Lifecycle gate pause failed: "<<error<<'\n';return 10;
 }
 AudioDeviceClockV40 paused;
 if(!clock.snapshot(paused)||paused.ready||mixer->output_frame()!=frame_before_pause){
  std::cerr<<"Paused clock remained valid or mixer consumed while paused\n";return 11;
 }
 if(!gate.publish_activity(1,4,true,true,true,false)){std::cerr<<"Activity resume rejected\n";return 12;}
 if(!control->tick(error)||!control->ready()){std::cerr<<"Lifecycle resume tick failed: "<<error<<'\n';return 13;}
 std::this_thread::sleep_for(std::chrono::milliseconds(120));
 if(!control->tick(error)||!control->ready()){std::cerr<<"Lifecycle resumed pump failed: "<<error<<'\n';return 13;}
 AudioDeviceClockV40 resumed;
 if(!clock.snapshot(resumed)||!resumed.ready||resumed.generation!=focused.generation||
    resumed.monotonic_ns<=focused.monotonic_ns||mixer->output_frame()<=frame_before_pause){
  std::cerr<<"Resumed clock/mixer did not advance\n";return 14;
 }
 if(!control->shutdown(error)||!control->close_succeeded()||control->ready()){
  std::cerr<<"Checked SDL close failed: "<<error<<'\n';return 15;
 }
 AudioDeviceClockV40 closed;
 if(!clock.snapshot(closed)||closed.ready){std::cerr<<"Clock remained ready after close\n";return 16;}
 if(!control->shutdown(error)||!control->close_succeeded()){
  std::cerr<<"Repeated checked close failed: "<<error<<'\n';return 17;
 }
 control.reset();return 0;
}
