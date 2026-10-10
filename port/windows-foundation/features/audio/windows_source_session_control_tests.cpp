#include "windows_source_session_control_v1.hpp"
#include "feature_audio.hpp"
#include "../../../engine-audio/integration-v42/audio_native_session_v42.hpp"
#include <chrono>
#include <iostream>
#include <thread>

using namespace dh2::audio;
using namespace dh::foundation::audio;

static int fail(int code,const std::string& message){std::cerr<<message<<"\n";return code;}
static bool wait_ready(AudioNativeSessionV42& session,bool expected,std::chrono::milliseconds limit){
 const auto deadline=std::chrono::steady_clock::now()+limit;
 do {if(session.ready_for_current_source()==expected)return true;std::this_thread::sleep_for(std::chrono::milliseconds(10));}
 while(std::chrono::steady_clock::now()<deadline);
 return false;
}

int main(int argc,char** argv){
 if(argc!=2)return 2;
 AudioFilesystem files{argv[1]};AudioGameplaySourcesV40 sources;
 sources.exact_assets={&files,AudioFilesystem::read,{}};
 auto provider=std::make_shared<unsigned>(1);
 AudioLifecycleGateV40 missing_gate;std::string error;
 AudioNativeSessionV42 missing_default(reinterpret_cast<std::uintptr_t>(&files),sources,provider,missing_gate);
 if(missing_default.initialize(error)||error!="Required actual platform audio control factory")
  return fail(3,"Windows default control absence was not reported honestly: "+error);

 AudioLifecycleGateV40 gate;
 if(!gate.publish_activity(1,1,true,true,true,false))return fail(4,"Could not publish actual focused lifecycle fixture");
 auto factory=windows_source_session_control_v1();
 if(!factory.valid()||!factory.context_owner)return fail(5,"Windows production control factory lacks its pinned context owner");
 std::weak_ptr<void> context_pin=factory.context_owner;
 AudioNativeSessionV42 session(reinterpret_cast<std::uintptr_t>(&files),sources,provider,gate,std::move(factory));
 if(!session.initialize(error))return fail(6,"WinMM native session startup failed: "+error);
 if(!wait_ready(session,true,std::chrono::seconds(5)))return fail(7,"WinMM output never opened/focused and published a ready source epoch: "+session.control_error());

 auto* runtime=session.runtime_on_producer();if(!runtime)return fail(8,"Producer did not retain the exact V42 runtime");
 AudioDeviceClockV40 focused_sample;
 if(!runtime->clock().snapshot(focused_sample)||!focused_sample.ready||focused_sample.generation==0||
    focused_sample.monotonic_ns<=0||focused_sample.position<0||focused_sample.rate!=48000)
  return fail(9,"Focused WinMM control did not publish the actual sample/QPC/generation clock");

 if(!gate.publish_activity(1,2,true,true,false,false))return fail(10,"Could not publish actual focus loss");
 if(!wait_ready(session,false,std::chrono::seconds(2)))return fail(11,"WinMM control did not pause when the shared lifecycle gate lost focus");
 AudioDeviceClockV40 unfocused_sample;
 if(!runtime->clock().snapshot(unfocused_sample)||unfocused_sample.ready||unfocused_sample.generation!=focused_sample.generation)
  return fail(12,"Focus loss did not publish an unready clock for the same output generation");

 if(!gate.publish_activity(1,3,true,true,true,false))return fail(13,"Could not publish focus restoration");
 if(!wait_ready(session,true,std::chrono::seconds(3)))return fail(14,"WinMM control did not resume from the shared lifecycle gate");
 AudioDeviceClockV40 resumed_sample;
 if(!runtime->clock().snapshot(resumed_sample)||!resumed_sample.ready||resumed_sample.generation!=focused_sample.generation||
    resumed_sample.monotonic_ns<=focused_sample.monotonic_ns)
  return fail(15,"Focus resume did not publish a newer QPC sample from the same WinMM generation");

 if(context_pin.expired())return fail(16,"Factory context owner was not retained while its control was live");
 if(!session.shutdown(error))return fail(17,"Checked WinMM close/join/V42 final-drain failed: "+error);
 if(session.runtime_on_producer()!=nullptr||!context_pin.expired())
  return fail(18,"Runtime or factory context outlived successful close/join/final-drain");
 std::cout<<"PASS WindowsSourceSessionControlV1: explicit factory; same V42 mixer/clock/gate; real WinMM open/focus/QPC; focus pause/resume; checked close, worker join, and final V42 drain; no second runtime\n";
 return 0;
}
