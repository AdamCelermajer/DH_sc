#pragma once
#include "feature_audio.hpp"
#include "../../retained_animation_owner.hpp"
#include "../../original_campaign_runtime.hpp"
#include "../../../engine-audio/audio_source_bindings_v38.hpp"
#include "../../../engine-audio/audio_world_producer_v38.hpp"
#include <functional>
#include <set>
namespace dh::foundation::audio {
struct SourceAudioRuntimeServices {
 std::uintptr_t actual_manager{},actual_actor{};
 std::function<bool(std::array<float,3>&,std::string&)> target_position;
 // Required original Play3D gates and emitter/listener authority. Called once
 // after named owner manager-before-position prefix. No source args invented.
 std::function<bool(const dh2::character::CombatSoundPlayV1&,const dh2::audio::AudioSoundV34&,const dh2::audio::AudioGroupV34&,dh2::audio::AudioCommandV34&,bool& admitted,std::string&)> prepare_3d;
 // Original regular Play36b80c body, distinct from event-marked Play3D.
 std::function<bool(int,bool,int,int,bool,const dh2::audio::AudioSoundV34&,const dh2::audio::AudioGroupV34&,dh2::audio::AudioCommandV34&,bool& admitted,std::string&)> prepare_plain;
 std::function<bool(std::int64_t wall_ms,std::uint64_t& frame,std::string&)> source_frame;
 std::function<bool(int,bool,bool,int,std::string&)> play_music;
 std::function<bool(int,std::string&)> stop_music;
 std::function<bool(std::string&)> trace_script;
 dh2::audio::AudioRandomV34 random;
 unsigned output_rate{};
};
class SourceAudioRuntime {
 SourceAudioRouter& router_;const dh2::audio::AudioCatalogV34& catalog_;
 const dh2::audio::AudioSourceBindingsV38& bindings_;SourceAudioRuntimeServices services_;
 std::map<std::uint64_t,std::vector<std::pair<int,std::uint64_t>>> generation_voices_;
 std::map<std::uint64_t,std::set<std::uint64_t>> generation_producers_;
 using EventIdentity=std::tuple<std::uint64_t,std::uint64_t,std::string,std::uint32_t,std::uint32_t,std::int32_t,std::uint64_t>;
 std::map<EventIdentity,std::uint64_t> event_occurrences_;
 std::uint64_t next_occurrence_{1};
public:
 SourceAudioRuntime(SourceAudioRouter& r,const dh2::audio::AudioCatalogV34& c,const dh2::audio::AudioSourceBindingsV38& b,SourceAudioRuntimeServices s):router_(r),catalog_(c),bindings_(b),services_(std::move(s)){}
 // ONLY sound_fx leaf after existing whole melee-event routing. Caller gives
 // detached-batch occurrence ordinal; wall_timestamp_ms minus source lag_ms.
 bool named_sound(const RetainedAnimationEvent&,std::uint64_t producer,std::uint64_t occurrence,std::string&);
 bool campaign(CampaignCommandPhase,const OriginalCampaignCommand&,bool received,std::uint64_t generation,std::uint64_t producer,std::uint64_t occurrence,std::int64_t wall_ms,bool& handled,bool& blocking,std::string&);
 bool retire(std::uint64_t generation,std::int64_t wall_ms,std::string&);
 void observe_receipt(const dh2::audio::AudioReceiptV34&);
};
}
