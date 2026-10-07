#pragma once
#include "../audio_application_manager_v42.hpp"
#include <jni.h>
namespace dh2::android_audio {
bool ensure_application_audio_v42(JNIEnv*,jobject actual_java_assets,std::string&);
bool borrow_application_audio_v42(dh2::audio::AudioApplicationBorrowV42&,std::string&);
// Actual producer publishes every live World/GS/RNG/command receiver together.
// Missing publication stays required; no general ready/phase is invented.
bool publish_actual_playback_v42(const dh2::audio::AudioGameplaySourcesV40&,
                               std::shared_ptr<void> complete_world_provider_lease,std::string&);
bool unpublish_actual_playback_v101(const std::shared_ptr<void>&exact_provider_lease,std::string&);
bool detach_expected_playback_v102(const dh2::audio::AudioApplicationBorrowV42&captured_manager,
 const std::shared_ptr<void>&expected_provider,bool&detached,std::string&);
std::uint64_t application_audio_owner_v42();
std::uint64_t reserve_application_audio_v42();
void request_application_audio_close_v42(std::uint64_t expected_owner);
bool shutdown_application_audio_v42(std::uint64_t expected_owner,std::string&);
}
