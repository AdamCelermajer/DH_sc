#pragma once
#include "platform_source_audio.hpp"
#include "../../retained_animation_owner.hpp"
#include "../../original_campaign_runtime.hpp"
#include <functional>
#include <map>
namespace dh::foundation::audio {
struct CanonicalAudioLeaves {
 // Presence check for the actual World/GS/Level gates and same-Level
 // listener/emitter source-command owner. No playback or readiness is guessed.
 std::function<bool(std::string&)> play3d_authorities_ready;
 std::function<bool(std::uintptr_t,std::array<float,3>&,std::string&)> target_position;
 std::function<bool(int,bool,int,int,bool,const dh2::audio::AudioSoundV34&,const dh2::audio::AudioGroupV34&,dh2::audio::AudioCommandV34&,std::string&)> plain_command;
 std::function<bool(std::string&)> trace_script;
 std::function<bool(int,bool,bool,int,std::string&)> play_music;
 std::function<bool(int,std::string&)> stop_music;
 // Source Execute ignores Play's return. Preserve operation failures here for
 // diagnostics while campaign execution continues.
 std::function<void(const std::string&)> audio_diagnostic;
};
// Production adapter: consumes the actual V42 manager/channel owner bound to
// WinMM. Its Play3D owns all original gates/selection/decoder/mixer/receipts.
class CanonicalSourceAudioAdapter {
 dh2::audio::AudioGameplayRuntimeV42& runtime_;CanonicalAudioLeaves leaves_;
 using Key=std::tuple<std::uint64_t,std::uintptr_t,std::string,unsigned,unsigned,int,std::uint64_t>;
 std::map<Key,std::uint64_t> delivered_;
public:
 CanonicalSourceAudioAdapter(dh2::audio::AudioGameplayRuntimeV42& r,CanonicalAudioLeaves s):runtime_(r),leaves_(std::move(s)){}
 // sampled_frame_ns is genuine QPC time paired with root's sampled source frame.
 // Only caller-resolved source lag is subtracted; no private now()/timer.
 bool named_sound(const RetainedAnimationEvent&,std::uintptr_t actual_actor,std::uint64_t occurrence,std::int64_t sampled_frame_ns,std::string&);
 bool campaign(CampaignCommandPhase,const OriginalCampaignCommand&,bool received,std::int64_t authored_event_ns,bool& handled,bool& blocking,std::string&);
 bool retire(std::uint64_t generation,std::uintptr_t actor,std::string&);
 void observe_receipt(const dh2::audio::AudioReceiptV34&);
};
}
