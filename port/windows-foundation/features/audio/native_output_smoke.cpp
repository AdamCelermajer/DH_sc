#include "platform_source_audio.hpp"
#include "canonical_source_audio_adapter.hpp"
#include "../../../engine-audio/integration-v40/runtime/default-source-fields/default_source_fields_v40.hpp"
#include <fstream>
#include <iterator>
#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>
using namespace dh::foundation::audio;
int main(int argc,char** argv){if(argc!=2)return 2;std::string error;AudioFilesystem files{argv[1]};dh2::audio::AudioGameplaySourcesV40 sources;sources.exact_assets={&files,AudioFilesystem::read,{}};
 // Test owns the receiver identity and exercises the actual recovered channel
 // runtime. Missing World/Play3D authorities remain unbound, never successful.
 PlatformSourceAudio session(reinterpret_cast<std::uintptr_t>(&files),sources);if(!session.initialize_and_open(error)){std::cerr<<error;return 5;}if(!session.set_actual_focus(true,error)){std::cerr<<error;return 6;}
 dh2::audio::AudioDeviceClockV40 first,last;if(!session.output().device_clock(1,first,error)){std::cerr<<error;return 7;}std::int64_t authored{};if(!winmm_monotonic_ns(authored,error)){std::cerr<<error;return 8;}
 // Original emitter gain/pitch constructor constants are used for this direct
 // plain transport test. Whole gameplay/source Play prefix is not certified.
 if(!session.runtime().submit_plain_source(232,authored,[](const auto& sound,const auto& group,auto& command,std::string&){if(sound.uid!=22)return false;command.left=command.right=dh2::audio::original_fresh_emitter_gain_v40();command.pitch=dh2::audio::original_fresh_emitter_pitch_v40();command.volume_group=group.volume_group;return true;},error)){std::cerr<<error;return 10;}const auto token=session.runtime().last_token();
 bool started{};for(unsigned i=0;i<50;++i){if(!session.pump(error)){std::cerr<<error;return 11;}dh2::audio::AudioReceiptV34 receipt;while(session.runtime().take_receipt(receipt))started|=receipt.token==token&&receipt.kind==dh2::audio::AudioReceiptKindV34::started;std::this_thread::sleep_for(std::chrono::milliseconds(5));}
 if(!session.output().device_clock(1,last,error)||last.position<=first.position||!started){std::cerr<<"Actual output failed to progress/acknowledge source voice: "<<error;return 12;}
 std::int64_t campaign_ns{};if(!winmm_monotonic_ns(campaign_ns,error)){std::cerr<<error;return 22;}
 unsigned target_queries{},trace_queries{},plain_calls{};std::string playback_diagnostic;CanonicalAudioLeaves leaves;
 leaves.play3d_authorities_ready=[](std::string&){return true;};
 leaves.target_position=[&](std::uintptr_t actor,std::array<float,3>& position,std::string&){if(actor!=0x1234)return false;++target_queries;position={1.f,2.f,3.f};return true;};
 leaves.trace_script=[&](std::string&){++trace_queries;return true;};
 leaves.plain_command=[&](int,bool,int,int,bool,const auto&,const auto&,auto&,std::string& e){++plain_calls;e="Required whole original regular Play authority";return false;};
 leaves.audio_diagnostic=[&](const std::string& e){playback_diagnostic=e;};
 CanonicalSourceAudioAdapter adapter(session.runtime(),std::move(leaves));
 dh::foundation::RetainedAnimationEvent marker{"sfx_WeaponSwoosh1","native-output-retained-marker",0,1000,7,0};
 if(adapter.named_sound(marker,0x1234,1,authored,error)||error.find("Required actual World/GS/Vox gate")==std::string::npos||target_queries!=1){std::cerr<<"Retained marker must reach the required real Play3D gate and fail closed: "<<error;return 15;}
 error.clear();if(adapter.named_sound(marker,0x1234,1,authored,error)||error.find("Required actual World/GS/Vox gate")==std::string::npos||target_queries!=2){std::cerr<<"Failed marker dispatch must not poison retained occurrence lifetime: "<<error;return 16;}
 bool handled{},blocking{};dh::foundation::OriginalCampaignCommand campaign;campaign.kind=13;campaign.scalars={{8,0},{12,0},{13,0},{16,363}};
 if(!adapter.campaign(dh::foundation::CampaignCommandPhase::execute,campaign,true,campaign_ns,handled,blocking,error)||!handled||blocking||trace_queries){std::cerr<<"Received nonmusic command13 must skip before the source trace query: "<<error;return 17;}
 error.clear();if(adapter.campaign(dh::foundation::CampaignCommandPhase::execute,campaign,false,campaign_ns,handled,blocking,error)||trace_queries!=1||error.find("Required whole original regular Play") == std::string::npos){std::cerr<<"Regular command13 must require the original Play authority: "<<error;return 18;}
 error.clear();campaign.scalars[16]=328;
 if(!adapter.campaign(dh::foundation::CampaignCommandPhase::execute,campaign,false,campaign_ns,handled,blocking,error)||!handled||blocking||trace_queries!=2||plain_calls!=1||!error.empty()||playback_diagnostic.find("Unavailable original audio asset: data/sounds/sfx_cave_troll_smash_ground.wav")==std::string::npos){std::cerr<<"Initialized source Play with missing authored Troll datasource must diagnose and continue: "<<error<<" | "<<playback_diagnostic;return 21;}
 error.clear();campaign.kind=14;campaign.scalars={{8,0},{12,0},{16,232}};
 if(!adapter.campaign(dh::foundation::CampaignCommandPhase::execute,campaign,false,campaign_ns,handled,blocking,error)||!handled||trace_queries!=3){std::cerr<<"Command14 must pass the actual trace gate and same V42 Stop owner: "<<error;return 19;}
 if(!session.pump(error)){std::cerr<<error;return 20;}
 dh2::character::CombatSoundPlayV1 missing;missing.manager=session.runtime().manager();missing.sound_id=232;if(session.runtime().submit_actual_play(missing,authored,error)){std::cerr<<"Missing actual source gates silently accepted";return 13;}
 if(!session.set_actual_focus(false,error)||!session.close_and_drain(error)){std::cerr<<error;return 14;}std::cout<<"ONE recovered V42 source manager/mixer + real WinMM decoded original swing; hardware samples "<<first.position<<" -> "<<last.position<<"; started channel/focus/drain checked; canonical adapter linked; retained marker gate/lifetime, received command13 skip, missing Troll source328 diagnostic-only continuation, regular Play authority failure, and command14 stop verified; audible parity not measured\n";return 0;
}
