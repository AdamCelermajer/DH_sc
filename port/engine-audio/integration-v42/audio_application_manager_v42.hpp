#pragma once
#include "audio_native_session_v42.hpp"
#include <map>
#include "../../level-world/vox_music_state_owner_v1.hpp"
#include "../audio_listener_rows_v38.hpp"
#include "../integration-v40/runtime/default-source-fields/default_source_fields_v40.hpp"
#include <optional>
#include "../audio_process_initialize_v100.hpp"
namespace dh2::audio {
// Application process owner; actual nullable global is a shared_ptr to this
// object published only after create succeeds. It survives graphical resets.
class AudioApplicationManagerV42 {
 const std::thread::id producer_{std::this_thread::get_id()};
 mutable std::mutex session_mutex_;
 std::shared_ptr<AudioNativeSessionV42> session_;
 std::shared_ptr<AudioNativeSessionV42> session()const;
 bool disabled_{};std::atomic<bool> closing_{false};
 dh2::sound::VoxMusicFieldsV1 music_fields_;
 // Original UpdateListener's static row pointer is selected once. Keep the
 // immutable row in the SAME process manager, across graphical/Level resets.
 std::optional<AudioListenerRowV38> listener_row_;
 std::optional<VoxListenerAuthorityV38> listener_authority_;
 OriginalVoxGeneralV40 general_;
 std::array<float,32> source_group_gain_;
 unsigned settings_initialize_phase_{};
 std::weak_ptr<void> initialize_application_v100_;
 std::string music_title38_v100_,initialize_error_v100_;
 std::vector<int> preload_uids_v100_;
 bool initialize_attempted_v100_{},initialize_busy_v100_{},initialize_complete_v100_{};
 bool producer(std::string&)const;
 explicit AudioApplicationManagerV42(bool actual_disabled):disabled_(actual_disabled){source_group_gain_.fill(1.f);}
public:
#ifdef DH2_AUDIO_NATIVE_SESSION_FIXTURE
 using Factory=std::unique_ptr<AudioSessionControlOwnerV40>(*)(void*,AudioMixerV34&,AudioClockV40&,AudioLifecycleGateV40&);
#endif
 static std::shared_ptr<AudioApplicationManagerV42> create(AudioGameplaySourcesV40,
  std::shared_ptr<void> actual_provider_lease,bool actual_disabled,std::string&
#ifdef DH2_AUDIO_NATIVE_SESSION_FIXTURE
  ,Factory fixture_factory=nullptr,void* fixture_context=nullptr
#endif
 );
 // Explicit production platform control. The exact factory context is pinned
 // by AudioNativeSessionV42 until output closure, worker join, and final drain.
 static std::shared_ptr<AudioApplicationManagerV42> create(AudioGameplaySourcesV40,
  std::shared_ptr<void> actual_provider_lease,bool actual_disabled,std::string&,
  AudioSessionControlFactoryV42);
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 bool disabled()const noexcept{return disabled_;}
 bool source_producer_ready_v94(std::string&)const;
 bool set_actual_disabled(bool,std::string&);
 // Reached original LoadSound(rawUID): no ordinal conversion or Play3D gates.
 bool precache_raw_uid(int,std::string&);
 bool submit_actual_play(const dh2::character::CombatSoundPlayV1&,std::int64_t,std::string&);
 dh2::sound::VoxMusicFieldsV1& music_fields_on_producer()noexcept{return music_fields_;}
 std::optional<AudioListenerRowV38>& listener_row_on_producer()noexcept{return listener_row_;}
 std::optional<VoxListenerAuthorityV38>& listener_authority_on_producer()noexcept{return listener_authority_;}
 bool initialize_source_settings_v68(int actual_fx,int actual_music,std::string&);
 bool set_source_volume_v68(int selector,float percent,std::string&);
 bool get_source_volume_v68(int selector,float&percent,std::string&);
 bool source_general_v68(OriginalVoxGeneralV40&,bool&completed,std::string&);
 bool source_bus_volumes_v68(std::array<float,3>&,std::string&);
 bool initialize_source_v100(const AudioInitializeServicesV100&,std::string&);
 bool set_music_state_v101(const char*actual_state,std::string&);
 bool stop_sound_v106(int source_ordinal,int fade_ms,
  const std::function<bool(int,std::string&)>&actual_platform_stop,std::string&);
 bool stop_3d_v112(int source_ordinal,int fade_ms,const float* center,float radius,
  const std::function<bool(int,std::string&)>&actual_platform_stop,std::string&);
 bool source_initialize_complete_v100()const noexcept{return initialize_complete_v100_;}
 const std::vector<int>& source_preload_uids_v100()const noexcept{return preload_uids_v100_;}
 void request_output_close();
 bool shutdown(std::string&);
 AudioGameplayRuntimeV42* runtime_on_producer()noexcept;
 std::string control_error()const;
};
// Root must retain this snapshot across GetSound, including nested callbacks.
struct AudioApplicationBorrowV42 {
 std::shared_ptr<AudioApplicationManagerV42> manager;
 std::uintptr_t identity()const noexcept{return manager?manager->identity():0;}
 bool precache_raw_uid(int uid,std::string&error)const{
  if(!manager){error="Required captured nonnull Application SoundManager";return false;}
  return manager->precache_raw_uid(uid,error);
 }
};
}
