#include "platform_source_audio.hpp"
#include "canonical_source_audio_adapter.hpp"
#include "retained_frame_audio_observer.hpp"
#include "../../../engine-audio/integration-v40/runtime/default-source-fields/default_source_fields_v40.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
using namespace dh::foundation::audio;
using namespace dh::foundation;

static int fail(int code,const std::string& what){std::cerr<<what<<"\n";return code;}

static bool test_retained_frame_observer(std::string& error){
 RetainedAnimationEvent event{"sfx_WeaponSwoosh1","retained-observer-fixture",2,111,9,4};
 RetainedFrameAudioClock clock{7,36987,9876543210LL,true};unsigned calls{};
 std::uint32_t seen_index=std::numeric_limits<std::uint32_t>::max();ActorId seen_actor=invalid_actor_id;
 RetainedFrameAudioClock seen_clock{};
 RetainedFrameAudioObserver observer=[&](ActorId actor,const RetainedAnimationEvent& actual,std::uint32_t index,
  const RetainedFrameAudioClock* sample,std::string& detail){
  ++calls;seen_actor=actor;seen_index=index;if(actual.name!=event.name||!sample){detail="observer identity/sample mismatch";return RetainedFrameAudioObserverStatus::playback_diagnostic;}
  seen_clock=*sample;return RetainedFrameAudioObserverStatus::dispatched;
 };
 auto result=observe_retained_frame_audio(observer,42,event,13,&clock);
 if(result.status!=RetainedFrameAudioObserverStatus::dispatched||calls!=1||seen_actor!=42||seen_index!=13||
    seen_clock.output_generation!=7||seen_clock.device_samples!=36987||seen_clock.qpc_monotonic_ns!=9876543210LL||!seen_clock.valid()){
  error="Retained observer did not preserve actor, actual event ordinal, and exact supplied sample";return false;
 }
 RetainedFrameAudioClock rejected=clock;rejected.ready=false;
 result=observe_retained_frame_audio(observer,42,event,14,&rejected);
 if(result.status!=RetainedFrameAudioObserverStatus::unavailable_clock||calls!=1||result.event_index!=14){
  error="Rejected/unpublished clock reached audio observer or lost event index";return false;
 }
 result=observe_retained_frame_audio(observer,42,event,15,nullptr);
 if(result.status!=RetainedFrameAudioObserverStatus::unavailable_clock||calls!=1){error="Absent clock reached audio observer";return false;}
 RetainedFrameAudioObserver required_missing=[](ActorId,const RetainedAnimationEvent&,std::uint32_t,
  const RetainedFrameAudioClock*,std::string& detail){detail="required source owner unavailable";return RetainedFrameAudioObserverStatus::required_owner_unavailable;};
 result=observe_retained_frame_audio(required_missing,42,event,16,&clock);
 if(result.status!=RetainedFrameAudioObserverStatus::required_owner_unavailable||result.detail!="required source owner unavailable"){
  error="Required audio-owner failure lost its distinct typed result";return false;
 }
 RetainedFrameAudioObserver playback_failure=[](ActorId,const RetainedAnimationEvent&,std::uint32_t,
  const RetainedFrameAudioClock*,std::string& detail){detail="source playback operation failed";return RetainedFrameAudioObserverStatus::playback_diagnostic;};
 result=observe_retained_frame_audio(playback_failure,42,event,17,&clock);
 if(result.status!=RetainedFrameAudioObserverStatus::playback_diagnostic||result.detail!="source playback operation failed"){
  error="Playback failure did not remain a non-veto diagnostic result";return false;
 }
 std::vector<int> order;RetainedFrameAudioObserverDiagnostic diagnostic;
 const bool frame_continues=dispatch_retained_frame_event_with_audio_observer(
  [&](std::string&){order.push_back(1);return true;},
  [&](ActorId,const RetainedAnimationEvent&,std::uint32_t,const RetainedFrameAudioClock*,std::string& detail){
   order.push_back(2);detail="output operation failed";return RetainedFrameAudioObserverStatus::playback_diagnostic;
  },42,event,18,&clock,diagnostic,error);
 if(!frame_continues||order!=std::vector<int>{1,2}||diagnostic.status!=RetainedFrameAudioObserverStatus::playback_diagnostic){
  error="Gameplay callback order/frame continuation changed when the optional audio sidecar failed";return false;
 }
 order.clear();
 const bool gameplay_rejected=dispatch_retained_frame_event_with_audio_observer(
  [&](std::string& e){order.push_back(1);e="required gameplay event failed";return false;},
  observer,42,event,19,&clock,diagnostic,error);
 if(gameplay_rejected||order!=std::vector<int>{1}){error="Optional audio sidecar ran after a failed gameplay callback";return false;}
 // CombatSession calls this helper with the actual frame.events index, after
 // the original gameplay callback and before motion handling.
 error.clear();return true;
}

int main(int argc,char** argv){
 if(argc!=2)return 2;
 std::string error;
 AudioFilesystem files{argv[1]};
 dh2::audio::AudioGameplaySourcesV40 sources;
 sources.exact_assets={&files,AudioFilesystem::read,{}};

 // This single session is the sole recovered V42 runtime and sole WinMM
 // output. Test fixtures supply original caller-owned fields; no second
 // router/runtime or fabricated Play3D gate is installed.
 PlatformSourceAudio session(reinterpret_cast<std::uintptr_t>(&files),sources);
 if(!session.initialize_and_open(error))return fail(5,error);
 RetainedFrameAudioClock captured_clock,published_clock;
 if(!session.publish_device_clock(captured_clock,error)||captured_clock.valid()||captured_clock.ready)
  return fail(28,"Unfocused WinMM sample unexpectedly became a valid gameplay-paired clock: "+error);
 if(session.published_device_clock(published_clock,error)||published_clock.valid()||published_clock.output_generation||
    published_clock.device_samples!=-1||published_clock.qpc_monotonic_ns)
  return fail(29,"Unready V42 clock snapshot was exposed as a usable sample");
 if(!session.set_actual_focus(true,error))return fail(6,error);
 if(!session.publish_device_clock(captured_clock,error)||!captured_clock.valid()||
    !session.published_device_clock(published_clock,error)||
    captured_clock.output_generation!=published_clock.output_generation||
    captured_clock.device_samples!=published_clock.device_samples||
    captured_clock.qpc_monotonic_ns!=published_clock.qpc_monotonic_ns)
  return fail(30,"Returned TIME_SAMPLES/QPC pair differs from the exact published V42 clock: "+error);
 dh2::audio::AudioDeviceClockV40 v42_sample;
 if(!session.runtime().clock().snapshot(v42_sample)||!v42_sample.ready||
    captured_clock.output_generation!=v42_sample.generation||captured_clock.device_samples!=v42_sample.position||
    captured_clock.qpc_monotonic_ns!=v42_sample.monotonic_ns)
  return fail(31,"RetainedFrameAudioClock is not the hardware pair published into this V42 runtime");
 if(!test_retained_frame_observer(error))return fail(32,error);
 session.runtime().clock().invalidate(captured_clock.output_generation);
 if(session.published_device_clock(published_clock,error)||published_clock.valid()||published_clock.output_generation||
    published_clock.device_samples!=-1||published_clock.qpc_monotonic_ns)
  return fail(33,"Invalidated/unpublished V42 clock produced a valid retained-frame sample");
 if(!session.publish_device_clock(captured_clock,error)||!captured_clock.valid())return fail(34,"Could not republish actual WinMM clock after invalidation: "+error);
 const auto* swing=session.runtime().bindings().row(232);
 const auto* moth=session.runtime().bindings().row(363);
 const auto* troll=session.runtime().bindings().row(328);
 if(!swing||swing->uid!=22||!moth||moth->uid!=459||!troll||troll->uid!=252)
  return fail(7,"Generated source ordinal/UID mapping changed");

 unsigned target_queries{},trace_queries{},plain_calls{},diagnostics{};
 std::string last_diagnostic;
 CanonicalAudioLeaves leaves;
 leaves.play3d_authorities_ready=[](std::string&){return true;}; // presence fixture
 leaves.target_position=[&](std::uintptr_t actor,std::array<float,3>& position,std::string&){
  if(actor!=0x1234)return false;++target_queries;position={1.f,2.f,3.f};return true;
 };
 leaves.trace_script=[&](std::string&){++trace_queries;return true;}; // original query fixture
 leaves.plain_command=[&](int ordinal,bool flag,int argument,int zero,bool final,
  const auto& sound,const auto& group,auto& command,std::string& e){
  ++plain_calls;
  if(ordinal!=232||flag||argument!=0||zero!=0||final||sound.uid!=22){
   e="Command13 did not preserve exact source callback arguments/row";return false;
  }
  command.left=command.right=dh2::audio::original_fresh_emitter_gain_v40();
  command.pitch=dh2::audio::original_fresh_emitter_pitch_v40();
  command.volume_group=group.volume_group;
  return true;
 }; // whole original Play fields fixture
 leaves.audio_diagnostic=[&](const std::string& e){++diagnostics;last_diagnostic=e;};
 CanonicalSourceAudioAdapter adapter(session.runtime(),std::move(leaves));

 bool handled{},blocking{};
 dh::foundation::OriginalCampaignCommand command;
 command.kind=13;command.scalars={{8,0},{12,0},{13,0},{16,232}};
 std::int64_t event_ns{};
 if(!winmm_monotonic_ns(event_ns,error))return fail(8,error);
 if(!adapter.campaign(dh::foundation::CampaignCommandPhase::execute,command,false,event_ns,handled,blocking,error)||
    !handled||blocking||plain_calls!=1||trace_queries!=1||session.runtime().last_xml_uid()!=22)
  return fail(9,"Command13 did not reach one recovered V42 source submit: "+error);
 const auto token=session.runtime().last_token();
 bool started{};
 for(unsigned i=0;i<100&&!started;++i){
  if(!session.pump(error))return fail(10,error);
  dh2::audio::AudioReceiptV34 receipt;
  while(session.runtime().take_receipt(receipt)){
   adapter.observe_receipt(receipt);
   if(receipt.token==token&&receipt.kind==dh2::audio::AudioReceiptKindV34::started)started=true;
  }
  if(!started)std::this_thread::sleep_for(std::chrono::milliseconds(5));
 }
 if(!started)return fail(11,"Recovered V42 source voice did not acknowledge started through WinMM");

 // Stop is routed through the same initialized runtime and source row. The
 // original dispatcher ignores Script_StopSound::Execute's return.
 command.kind=14;command.scalars={{8,0},{12,0},{16,232}};
 if(!winmm_monotonic_ns(event_ns,error))return fail(12,error);
 if(!adapter.campaign(dh::foundation::CampaignCommandPhase::execute,command,false,event_ns,handled,blocking,error)||
    !handled||blocking||trace_queries!=2)
  return fail(13,"Command14 did not use same V42 Stop owner: "+error);
 for(unsigned i=0;i<100&&session.runtime().voice(token);++i){
  if(!session.pump(error))return fail(14,error);
  dh2::audio::AudioReceiptV34 receipt;
  while(session.runtime().take_receipt(receipt))adapter.observe_receipt(receipt);
  if(session.runtime().voice(token))std::this_thread::sleep_for(std::chrono::milliseconds(5));
 }
 if(session.runtime().voice(token))return fail(15,"Command14 did not retire its same-runtime source voice");

 // Saturate the actual mixer command ring after all required owners are
 // bound. The source Play operation returns a channel-queue failure, but its
 // void script Execute caller ignores that result; campaign execution must
 // continue and preserve the failure as a diagnostic.
 dh2::audio::AudioCommandV34 filler;filler.kind=dh2::audio::AudioCommandKindV34::volume;
 unsigned queued{};while(session.runtime().mixer().post(filler))++queued;
 if(queued<500)return fail(16,"Could not reach actual V42 channel command-capacity boundary");
 command.kind=13;command.scalars={{8,0},{12,0},{13,0},{16,232}};
 if(!winmm_monotonic_ns(event_ns,error))return fail(17,error);
 if(!adapter.campaign(dh::foundation::CampaignCommandPhase::execute,command,false,event_ns,handled,blocking,error)||
    !handled||blocking||trace_queries!=3||diagnostics!=1||!error.empty()||last_diagnostic!="Audio source command queue full")
  return fail(18,"Reached source channel queue failure must diagnose and continue command13: "+error+" | "+last_diagnostic);
 for(unsigned i=0;i<30;++i){
  std::this_thread::sleep_for(std::chrono::milliseconds(15));
  if(!session.pump(error))return fail(19,error);
 }

 // Authentic source ordinal328 maps to UID252 but its exact original WAV is
 // absent. The source manager returns normally after missing datasource; the
 // void Script_PlaySound caller does not let this veto later visual commands.
 command.kind=13;command.scalars={{8,0},{12,0},{13,0},{16,328}};
 if(!winmm_monotonic_ns(event_ns,error))return fail(20,error);
 if(!adapter.campaign(dh::foundation::CampaignCommandPhase::execute,command,false,event_ns,handled,blocking,error)||
    !handled||blocking||trace_queries!=4||diagnostics!=2||!error.empty()||
    last_diagnostic.find("Unavailable original audio asset: data/sounds/sfx_cave_troll_smash_ground.wav")==std::string::npos)
  return fail(21,"Exact absent Troll datasource must diagnose and continue source command13: "+error+" | "+last_diagnostic);

 // A reached but failing named Play3D still observes the real missing-gate
 // provider boundary. Failure must not be mistaken for missing media or
 // poison the retained occurrence identity.
 dh::foundation::RetainedAnimationEvent marker{"sfx_WeaponSwoosh1","adapter-regression",0,1000,7,0};
 if(!winmm_monotonic_ns(event_ns,error))return fail(22,error);
 if(adapter.named_sound(marker,0x1234,1,event_ns,error)||
    error.find("Required actual World/GS/Vox gate")==std::string::npos||target_queries!=1)
  return fail(23,"Unavailable required actual Play3D service must remain fatal: "+error);
 error.clear();
 if(adapter.named_sound(marker,0x1234,1,event_ns,error)||
    error.find("Required actual World/GS/Vox gate")==std::string::npos||target_queries!=2)
  return fail(24,"Failed named dispatch must not deduplicate/relabel missing authority: "+error);

 // The source runtime can exist while its required whole-Play caller is
 // absent. Provider absence remains fatal, in contrast to a reached missing
 // datasource above.
 CanonicalAudioLeaves absent_play;absent_play.trace_script=[](std::string&){return true;};
 CanonicalSourceAudioAdapter missing_owner(session.runtime(),std::move(absent_play));
 command.kind=13;command.scalars={{8,0},{12,0},{13,0},{16,232}};
 if(missing_owner.campaign(dh::foundation::CampaignCommandPhase::execute,command,false,event_ns,handled,blocking,error)||
    error.find("Required whole original regular Play emitter/property authority")==std::string::npos)
  return fail(25,"Missing required regular-Play owner must fail closed: "+error);

 // A present operation callback that returns a provider failure is different
 // from the absent callback and remains fatal; do not reinterpret it as media.
 CanonicalAudioLeaves rejecting_play;
 rejecting_play.trace_script=[](std::string&){return true;};
 rejecting_play.plain_command=[](int,bool,int,int,bool,const auto&,const auto&,auto&,std::string& e){e="Required fixture whole original Play provider operation";return false;};
 CanonicalSourceAudioAdapter rejecting_owner(session.runtime(),std::move(rejecting_play));
 error.clear();
 if(rejecting_owner.campaign(dh::foundation::CampaignCommandPhase::execute,command,false,event_ns,handled,blocking,error)||
    error.find("Required fixture whole original Play provider operation")==std::string::npos)
  return fail(26,"Failed required Play provider operation must not be softened: "+error);

 if(!session.set_actual_focus(false,error)||session.published_device_clock(published_clock,error)||published_clock.valid()||
    !session.close_and_drain(error))return fail(27,error.empty()?"Focus loss did not invalidate the published audio sample":error);
 if(session.publish_device_clock(captured_clock,error)||captured_clock.valid()||captured_clock.output_generation||
    captured_clock.device_samples!=-1||captured_clock.qpc_monotonic_ns)
  return fail(35,"Failed closed-output publication did not clear the returned sample");
 std::cout<<"PASS linked canonical adapter: one recovered V42 + real WinMM; exact TIME_SAMPLES/QPC/generation publication and invalidation; retained observer preserves supplied ordinal/sample, missing clock and playback failures remain non-veto typed diagnostics; source command13 starts row232/UID22, command14 stops the same voice; actual absent Troll328 reports diagnostic without veto; required owners remain fatal; no gameplay-enrollment claim\n";
 return 0;
}
