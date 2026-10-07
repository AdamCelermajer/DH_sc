#pragma once
#include "integration-v42/application/native_audio_application_v42.hpp"
#include "audio_source_command_v40.hpp"
#include "../level-loader/canonical_level_context_v1.hpp"
#include <functional>
namespace dh2::audio {
enum class AudioCategoryV46 {attack,skill,monster,container,item,ambience,music};
struct AudioCampaignServicesV46 {
 std::shared_ptr<void> actual_world,actual_gs,settings_owner,rng_owner,camera_owner;
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> selected_level;
 std::function<bool(std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,std::string&)> current_level;
 std::function<bool(const char*,bool&,std::string&)> actual_debug;
 std::function<bool(bool&,std::string&)> actual_online,actual_network_muted;
 std::function<bool(const character::CombatSoundPlayV1&,std::string&)> actual_platform_play;
 std::function<bool(const character::CombatSoundPlayV1&,int,std::string&)> actual_trace;
 // Read the actual completed source general/settings owner. False is required.
 std::function<bool(OriginalVoxGeneralV40&,bool& completed_source_initialize,std::string&)> actual_general;
 // Executes reached group/settings/RNG prefix, preserving setter side effects.
 std::function<bool(const character::CombatSoundPlayV1&,const AudioSoundV34&,const AudioGroupV34&,float&,float&,int&,std::string&)> actual_emitter_modifiers;
 // Effective MUSIC/SFX/VFX bus gains including actual master/sliders. Emitter
 // gain returned above must exclude these, avoiding double application.
 std::function<bool(std::array<float,3>&,std::string&)> actual_group_volumes;
 std::function<bool(const AudioListenerRowV38&,float*,float*,float*,std::string&)> actual_update_listener;
 AudioRandomV34 actual_vox_random;
 std::shared_ptr<const std::vector<std::uint8_t>> exact_legacy_listener_stream;
};
// One actual campaign lease publishes providers to the persistent Application
// manager. Lifecycle/phase/settings facts are read, never copied into readiness.
class AudioCampaignBridgeV46:public std::enable_shared_from_this<AudioCampaignBridgeV46> {
 AudioApplicationBorrowV42 manager_;AudioCampaignServicesV46 services_;
 std::vector<AudioListenerRowV38>listeners_;std::uint64_t epoch_{};
 std::array<float,3>posted_volumes_{};bool volumes_posted_{};
 std::thread::id producer_{std::this_thread::get_id()};std::string failure_;
 static int gates(void*,const dh2::sound::VoxPlay3DRequestV2&,dh2::sound::VoxPlay3DResponseV2&);
 static bool random(void*,int&);
 static bool command(void*,const character::CombatSoundPlayV1&,const AudioSoundV34&,const AudioGroupV34&,AudioCommandV34&,std::string&);
 bool current(std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,std::string&);
public:
 static std::shared_ptr<AudioCampaignBridgeV46> publish(AudioCampaignServicesV46,std::string&);
 bool submit(AudioCategoryV46,const character::CombatSoundPlayV1&,std::int64_t actual_event_ns,std::string&);
 bool source_prefix_without_clock(const character::CombatSoundPlayV1&,std::string&);
 bool take_receipt(AudioReceiptV34&);
 bool native_music_state(std::uint64_t actual_started_token,const char*exact_state,std::string&);
 const AudioApplicationBorrowV42& manager()const noexcept{return manager_;}
 const std::shared_ptr<void>& world()const noexcept{return services_.actual_world;}
 const AudioCampaignServicesV46& services_on_producer()const noexcept{return services_;}
 const std::string& failure()const noexcept{return failure_;}
};
// Actual caller supplies monotonic time after its authored lag calculation.
// No now(), dt conversion, or private clock is introduced at sound leaves.
class AudioAuthoredEventScopeV46 {
 std::int64_t previous_{};
public:
 explicit AudioAuthoredEventScopeV46(std::int64_t actual_event_ns);
 ~AudioAuthoredEventScopeV46();
 AudioAuthoredEventScopeV46(const AudioAuthoredEventScopeV46&)=delete;
 AudioAuthoredEventScopeV46&operator=(const AudioAuthoredEventScopeV46&)=delete;
 static std::int64_t current()noexcept;
};
}
