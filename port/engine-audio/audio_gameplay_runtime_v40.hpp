#pragma once
#include "audio_clock_v40.hpp"
#include "audio_source_bindings_v38.hpp"
#include "audio_bank_v34.hpp"
#include "../level-world/vox_play3d_owner_v2.hpp"
#include <map>
namespace dh2::audio {
struct AudioGameplaySourcesV40 {
 // Every source early/platform/trace gate comes from actual current World/GS.
 dh2::sound::VoxPlay3DServicesV2 gates;
 AudioAssetServicesV34 exact_assets;
 AudioRandomV34 random;
 void* context{};
 bool(*output_ready)(void*,std::string&){};
 bool(*source_command)(void*,const dh2::character::CombatSoundPlayV1&,
  const AudioSoundV34&,const AudioGroupV34&,AudioCommandV34&,std::string&){};
};
struct AudioVoiceStateV40 {int xml_uid{-1};bool started{},control_required{};};
struct AudioControlClosedProofV40 {
 const AudioMixerV34* mixer{};
 const std::atomic<bool>* close_completed{};
 const std::atomic<bool>* control_joined{};
};
// One GL/gameplay producer owns all public mutable methods except clock()
// publication; AAudio consumes mixer(). Control lifecycle never posts commands.
class AudioGameplayRuntimeV40 {
 const std::uintptr_t manager_;AudioGameplaySourcesV40 sources_;
 AudioCatalogV34 catalog_;AudioMixerV34 mixer_;AudioSampleBankV34 bank_;
 AudioSourceBindingsV38 bindings_;AudioClockV40 clock_;
 std::map<std::uint64_t,AudioVoiceStateV40> voices_;
 std::vector<AudioReceiptV34> observations_;
 bool initialized_{},in_submit_{},quiescing_{};
 std::int64_t event_ns_{};std::uint64_t last_token_{};int last_uid_{-1},last_status_{};unsigned last_phase_{};
 std::string error_;
 static int invoke(void*,const dh2::sound::VoxPlay3DRequestV2&,dh2::sound::VoxPlay3DResponseV2&);
 int emit(const dh2::character::CombatSoundPlayV1&,int,bool);
public:
 AudioGameplayRuntimeV40(std::uintptr_t actual_manager,AudioGameplaySourcesV40);
 // Exact original-selected pack and registered generated members. IO remains
 // on this producer; initialization is permitted only with closed output.
 bool initialize_exact_source(std::string&);
 bool submit_actual_play(const dh2::character::CombatSoundPlayV1&,
  std::int64_t source_event_monotonic_ns,std::string&);
 void pump_receipts();
 bool take_receipt(AudioReceiptV34&);
 bool load_actual_xml_uid(int original_xml_uid,std::string&);
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
