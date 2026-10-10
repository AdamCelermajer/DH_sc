#include "linux_sdl2_output.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

using namespace dh::foundation::audio;
using dh2::audio::AudioDeviceClockV40;
using dh2::audio::AudioMixerV34;

int main(int argc,char** argv) {
 if(argc!=2)return 2;
 const std::string mode=argv[1];
 if(mode=="failure"){
  setenv("SDL_AUDIODRIVER","driver-that-does-not-exist",1);
  auto mixer=std::make_unique<AudioMixerV34>();LinuxSdl2AudioOutput output(*mixer);std::string error;
  if(output.open(error)||error.empty()||output.opened()){std::cerr<<"Unavailable SDL driver was accepted\n";return 3;}
  return 0;
 }
 if(mode!="dummy")return 2;
 setenv("SDL_AUDIODRIVER","dummy",1);
 auto mixer=std::make_unique<AudioMixerV34>();LinuxSdl2AudioOutput output(*mixer);std::string error;
 if(!output.open(error)){std::cerr<<error<<'\n';return 4;}
 if(mixer->output_frame()!=0){std::cerr<<"Open rendered before update\n";return 5;}
 const auto services=output.services();
 if(!services.context||!services.open(services.context,error)||!services.update(services.context,error)){
  std::cerr<<"AudioOutputServices pump failed: "<<error<<'\n';return 6;
 }
 if(mixer->output_frame()<512){std::cerr<<"SDL queue did not consume mixer frames\n";return 7;}
 AudioDeviceClockV40 first;
 if(!output.device_clock(9,first,error)||!first.ready||first.generation!=9||first.rate!=48000||first.monotonic_ns<=0){
  std::cerr<<"Estimated SDL clock failed: "<<error<<'\n';return 8;
 }
 const auto paused_at=mixer->output_frame();
 if(!output.focus(false,error)||!services.update(services.context,error)||!output.opened()){
  std::cerr<<"Focus loss handling failed: "<<error<<'\n';return 9;
 }
 AudioDeviceClockV40 unfocused;
 if(output.device_clock(9,unfocused,error)||unfocused.ready){std::cerr<<"Clock remained ready while unfocused\n";return 10;}
 if(mixer->output_frame()!=paused_at){std::cerr<<"Mixer consumed while output focus was lost\n";return 11;}
 if(!output.focus(true,error)){std::cerr<<"Focus resume failed: "<<error<<'\n';return 12;}
 std::this_thread::sleep_for(std::chrono::milliseconds(120));
 if(!services.update(services.context,error)||mixer->output_frame()<=paused_at){
  std::cerr<<"Mixer did not resume after focus returned: "<<error<<'\n';return 13;
 }
 if(!services.close){return 14;}
 services.close(services.context);
 if(output.opened()||!output.open(error)||!output.opened()){
  std::cerr<<"SDL close/reopen failed: "<<error<<'\n';return 15;
 }
 output.close();
 if(output.opened()){std::cerr<<"SDL output remained open after close\n";return 17;}
 return 0;
}
