#pragma once
#include "audio_application_manager_v42.hpp"
#include <memory>
#include <thread>
namespace dh2::audio {class AudioCampaignBridgeV46;class AudioLevelGameplayV67;}
namespace model_renderer {
class CampaignAudioRetirementV102 final {
 friend bool capture_campaign_audio_retirement_v102(const std::shared_ptr<void>&,
  std::shared_ptr<CampaignAudioRetirementV102>&,std::string&);
 friend bool stop_campaign_sound_v106(const std::shared_ptr<void>&,int,int,std::string&);
 friend bool stop_campaign_sound_3d_v112(const std::shared_ptr<void>&,int,int,const float*,float,std::string&);
 // Root retains this independent request storage outside the World. These
 // weak control blocks preserve captured identities without pinning the World.
 std::weak_ptr<void> world_;
 std::weak_ptr<dh2::audio::AudioCampaignBridgeV46> playback_,level_bridge_;
 std::weak_ptr<dh2::audio::AudioLevelGameplayV67> level_;
 dh2::audio::AudioApplicationBorrowV42 manager_;
 std::thread::id producer_{std::this_thread::get_id()};
 bool had_playback_{},had_level_{},detached_{};
 CampaignAudioRetirementV102()=default;
public:
 const dh2::audio::AudioApplicationBorrowV42& captured_manager()const noexcept{return manager_;}
 bool providers_absent_at_capture()const noexcept{return !had_playback_&&!had_level_;}
 // Root calls after the original StopAllSounds(500) and drains actual receipts
 // through this captured manager. No current-Level/GS borrow occurs here.
 // Success with done=false means callback channel/sample retirement is pending.
 bool final_detach(bool&done,std::string&);
};
bool capture_campaign_audio_retirement_v102(const std::shared_ptr<void>&actual_world,
 std::shared_ptr<CampaignAudioRetirementV102>&out,std::string&);
}
