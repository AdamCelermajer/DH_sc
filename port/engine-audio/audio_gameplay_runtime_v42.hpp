#pragma once
#include "audio_gameplay_runtime_v40.hpp"
#include "audio_clock_v40.hpp"
#include "audio_source_bindings_v38.hpp"
#include "audio_bank_v34.hpp"
#include "../level-world/vox_play3d_owner_v2.hpp"
#include <map>
#include <functional>
namespace dh2::audio {
// One GL/gameplay producer owns all public mutable methods except clock()
// publication; AAudio consumes mixer(). Control lifecycle never posts commands.
class AudioGameplayRuntimeV42 {
 const std::uintptr_t manager_;AudioGameplaySourcesV40 sources_;
 AudioCatalogV34 catalog_;AudioMixerV34 mixer_;AudioSampleBankV34 bank_;
 AudioSourceBindingsV38 bindings_;AudioClockV40 clock_;
 struct VoiceV42:AudioVoiceStateV40 {std::array<float,3> position{};
  VoiceV42(int uid,const std::array<float,3>& p):AudioVoiceStateV40{uid,false},position(p){} };
 std::map<std::uint64_t,VoiceV42> voices_;
 std::vector<AudioReceiptV34> observations_;
 bool initialized_{},in_submit_{},quiescing_{};
 std::int64_t event_ns_{};std::uint64_t last_token_{};int last_uid_{-1},last_status_{};unsigned last_phase_{};
 std::string error_;
 static int invoke(void*,const dh2::sound::VoxPlay3DRequestV2&,dh2::sound::VoxPlay3DResponseV2&);
 int emit(const dh2::character::CombatSoundPlayV1&,int,bool);
public:
 AudioGameplayRuntimeV42(std::uintptr_t actual_manager,AudioGameplaySourcesV40);
 // Exact original-selected pack and registered generated members. IO remains
 // on this producer; initialization is permitted only with closed output.
 bool initialize_exact_source(std::string&);
 bool submit_actual_play(const dh2::character::CombatSoundPlayV1&,
  std::int64_t source_event_monotonic_ns,std::string&);
 // Original Play36b80c uses generated row.uid directly, including event rows.
 // Its fresh emitter/property body is separate from Play3D's listener gates.
 using PlainCommand=std::function<bool(const AudioSoundV34&,const AudioGroupV34&,AudioCommandV34&,std::string&)>;
 bool submit_plain_source(int source_ordinal,std::int64_t,const PlainCommand&,std::string&);
 bool stop_source_ordinal(int source_ordinal,std::uint32_t fade_frames,std::string&);
 bool stop_source_sound_v106(int source_ordinal,int fade_ms,std::string&);
 bool stop_source_3d_v112(int source_ordinal,int fade_ms,const float* center,float radius,std::string&);
 bool source_ordinal_playing(int source_ordinal,bool&,std::string&);
 bool resume_source_ordinal(int source_ordinal,std::uint32_t fade_frames,std::string&);
 bool fresh_native_state_v68(int actual_xml_uid,int&,std::string&);
 bool initialize_priority_banks_v100(std::string&);
 bool select_source_event_v100(int actual_event_uid,AudioRandomV34,int&,std::string&);
 bool source_static_bus_routing_v100(const char*,std::string&);
 bool source_dynamic_bus_routing_v94(const char*,std::string&);
 bool source_slot_ready_v94(int uid)const{return bool(bank_.source_slot_sample_v101(uid));}
 const std::string* source_slot_unavailable_v100(int uid)const noexcept{return bank_.source_slot_unavailable_v100(uid);}
 std::uintptr_t source_slot_identity_v101(int uid)const noexcept{return bank_.source_slot_identity_v101(uid);}
 bool first_source_channel_v101(int actual_xml_uid,std::uint64_t&,std::string&);
 bool native_state_owned_v101(std::uint64_t actual_owned_token,const char*,std::string&);
 std::size_t owned_voices_v101()const noexcept{return voices_.size();}
 int source_operation_v101(const dh2::sound::VoxPlay3DRequestV2&,dh2::sound::VoxPlay3DResponseV2&,std::int64_t actual_event_ns,std::string&);
 void pump_receipts();
 bool take_receipt(AudioReceiptV34&);
 bool load_actual_xml_uid(int original_xml_uid,std::string&);
 std::shared_ptr<const AudioSampleV34> load_sample_actual_xml_uid(int,std::string&);
 const AudioCatalogV34& catalog()const noexcept{return catalog_;}
 bool stop_world(std::string&);
 // Only after the real output control thread has closed and joined. This
 // producer becomes sole consumer temporarily to drain muted stop commands.
 bool finalize_after_output_closed(const AudioControlClosedProofV40&,std::string&);
 bool native_state(std::uint64_t actual_active_token,const char* exact_state,std::string&);
 const AudioVoiceStateV40* voice(std::uint64_t token)const noexcept;
 AudioMixerV34& mixer()noexcept{return mixer_;}
 AudioClockV40& clock()noexcept{return clock_;}
 bool source_data_initialized()const noexcept{return initialized_;}
 std::uintptr_t manager()const noexcept{return manager_;}
 const AudioSourceBindingsV38& bindings()const noexcept{return bindings_;}
 std::uint64_t last_token()const noexcept{return last_token_;}
 int last_xml_uid()const noexcept{return last_uid_;}
 int last_source_status()const noexcept{return last_status_;}
 unsigned last_source_phase()const noexcept{return last_phase_;}
 std::size_t pinned_samples()const noexcept{return bank_.pinned_samples();}
};
}
