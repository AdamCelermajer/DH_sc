#pragma once
#include "audio_campaign_bridge_v46.hpp"
#include "../level-loader/level_gameplay_update_v66.hpp"
#include "../level-world/gameplay_camera_application_v23.hpp"
#include "../level-world/canonical_level_config_module_v1.hpp"
#include "../level-world/character_player_aggro_owner_v1.hpp"
namespace dh2::audio {
struct AudioListenerDriverV67 {
 camera::CameraPickingViewV20 picking;
 std::int32_t viewport_bottom{},screen_width{},screen_height{};
};
// Actual reached service leaves, owned by the selected App/World. No private
// current-Level, camera, player or settings projection is allocated here.
struct AudioLevelBindingsV67 {
 std::shared_ptr<camera::GameplayCameraApplicationV23> camera;
 std::shared_ptr<world::GameObjectSceneRootRegistryV1> roots;
 std::function<const world::CanonicalLevelConfigV1*(std::uintptr_t)> config;
 std::function<bool(bool&,std::string&)> application_byte_b4;
 std::function<bool(float&,std::string&)> source_music_volume;
 // Whole fresh emitter/GetEmitterInfo/SetDSP/SetGroup/Play prefix.
 // Original Play stack argument0/2 gates SetGroup(XML group) when in1..30.
 // It is not a native state-array index. Preserve the reached emitter group,
 // including the fresh default when SetGroup skips.
 std::function<bool(bool,int,const AudioSoundV34&,const AudioGroupV34&,AudioCommandV34&,std::string&)> plain_command;
 std::function<bool(int,bool,int,std::string&)> platform_play; // type1 music,type2 Play
 std::function<bool(int,std::string&)> platform_stop;
 std::function<bool(AudioListenerDriverV67&,std::string&)> driver;
 std::function<bool(std::uintptr_t&,std::string&)> local_character0;
 std::function<bool(std::uintptr_t,camera::PointV2&,std::string&)> target_position,look_at,visual_up;
};
class AudioLevelGameplayV67:public std::enable_shared_from_this<AudioLevelGameplayV67> {
 AudioLevelBindingsV67 bindings_;
 std::shared_ptr<AudioCampaignBridgeV46> bridge_;
 std::shared_ptr<loader::CanonicalLevelContextV1> level_;
 std::vector<AudioListenerRowV38> rows_;
 std::thread::id producer_{std::this_thread::get_id()};
 bool released_v101_{};
 bool producer(std::string&)const;
 bool current(std::string&);
 bool fade(int,std::uint32_t&,std::string&);
 bool play(int,bool,int,int,bool,std::string&);
 bool stop_music(int,std::string&);
 bool listener_vectors(const AudioListenerRowV38&,float*,float*,float*,std::string&);
public:
 // Publishes ONE V46 bridge and fills only the three audio/ambient V66 tails.
 // Existing gameplay providers remain unchanged. Call once on same producer.
 static std::shared_ptr<AudioLevelGameplayV67> compose(AudioCampaignServicesV46,
  AudioLevelBindingsV67,loader::LevelGameplayServicesV66&,std::string&);
 bool start_level_sound(std::string&);
 bool play_music(int,bool,bool,int,std::string&);
 bool play_plain_v115(int ordinal,bool enabled,int fade_ms,int group,bool bypass_online,std::string&);
 bool stop_music_v117(int fade_ms,std::string&);
 bool set_in_safe_zone_music(bool,std::string&);
 bool set_ambient(std::string&);
 bool update_listener(std::string&);
 bool stop_sound_v106(int source_ordinal,int fade_ms,std::string&);
 bool stop_3d_v112(int source_ordinal,int fade_ms,const float* center,float radius,std::string&);
 bool borrow_aggro_level_v101(character::PlayerAggroLevelBorrowV1&,std::string&);
 bool release_source_providers_v101(const std::shared_ptr<void>&actual_world,std::string&);
 bool detach_captured_providers_v102(const std::shared_ptr<AudioCampaignBridgeV46>&expected,std::string&);
 const std::shared_ptr<AudioCampaignBridgeV46>& bridge()const noexcept{return bridge_;}
};
}
