#pragma once
#include "canonical_sound_emitter_v32.hpp"
#include <array>
#include <cmath>
#include <cstring>
namespace dh2::world {
struct SoundEmitterLevelV92 {std::shared_ptr<void> receiver;const std::uint32_t* state130{};};
struct SoundEmitterPlayerV92 {std::shared_ptr<void> receiver;const std::uintptr_t* character660{};};
struct SoundEmitterPositionV92 {std::shared_ptr<void> receiver;const float* position160{};};
struct SoundEmitterAudioV92 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};
 std::function<bool(std::int32_t,const std::array<float,3>&,bool,std::int32_t,float,float,std::string&)> play3d;
 std::function<bool(std::int32_t,std::int32_t,const float*,float,std::string&)> stop3d;
 std::function<bool(std::int32_t,std::int32_t,std::string&)> stop;
 bool valid_nullable()const noexcept{return receiver?identity&&play3d&&stop3d&&stop:!identity&&!play3d&&!stop3d&&!stop;}
 explicit operator bool()const noexcept{return bool(receiver);}
};
struct SoundEmitterSourceServicesV92 {
 std::shared_ptr<void> owner;
 std::function<bool(SoundEmitterLevelV92&,std::string&)> current_level;
 std::function<bool(std::int32_t,bool,SoundEmitterPlayerV92&,std::string&)> local_player;
 std::function<bool(std::uintptr_t,SoundEmitterPositionV92&,std::string&)> character_position;
 std::function<bool(SoundEmitterAudioV92&,std::string&)> sound_manager;
 std::function<bool(CanonicalSoundEmitterV32&,std::string&)> destroy_gameobject_base;
};
inline float sound_source_distance_v92(const float* a,const float* b){
 // Original float sub/mul/add (rounded separately), f2d, sqrt, d2f.
 volatile float x=a[0]-b[0],y=a[1]-b[1],z=a[2]-b[2];
 volatile float xx=x*x,yy=y*y,zz=z*z;volatile float xy=xx+yy;volatile float sum=xy+zz;
 return static_cast<float>(std::sqrt(static_cast<double>(sum)));
}
inline bool sound_emitter_update_v92(CanonicalSoundEmitterV32& actual,const SoundEmitterSourceServicesV92& s,std::string& e){
 if(!s.owner||!s.current_level){e="Required SAME SoundEmitter current App/Level authority";return false;}
 SoundEmitterLevelV92 first;if(!s.current_level(first,e))return false;
 if(!first.receiver){if(first.state130){e="Foreign NULL current Level receipt";return false;}e.clear();return true;}
 SoundEmitterPlayerV92 player;if(!s.local_player||!s.local_player(0,true,player,e))return false;
 if(!player.receiver||!player.character660){e="Original SoundEmitter NULL PlayerInfo before660 read";return false;}
 if(!*player.character660){e.clear();return true;}
 SoundEmitterLevelV92 second;if(!s.current_level(second,e))return false;
 if(!second.receiver||!second.state130){e="Original SoundEmitter current Level changed to NULL before130";return false;}
 if(*second.state130!=38){e.clear();return true;}
 // Source reborrows localPlayer0 BEFORE position copy/distance; prior id is
 // not reused. A real changed-to-NULL player/Character is an unsafe edge.
 player={};if(!s.local_player(0,true,player,e))return false;
 if(!player.receiver||!player.character660||!*player.character660){e="Original SoundEmitter fresh local Character NULL before160";return false;}
 SoundEmitterPositionV92 position;if(!s.character_position||!s.character_position(*player.character660,position,e)||!position.receiver||!position.position160){if(e.empty())e="Required actual local Character160";return false;}
 const std::array<float,3> player_point{position.position160[0],position.position160[1],position.position160[2]};
 const auto* emitter=actual.base().vector3(0x160);if(!emitter){e="Required actual SAME SoundEmitter160";return false;}
 const float distance=sound_source_distance_v92(emitter,player_point.data());
 if(actual.source_byte39c()){
  const float maximum=actual.distance_max();
  // __aeabi_fcmple(maximum,distance), followed by fcmpgt(maximum,0).
  if(!(maximum<=distance)||!(maximum>0.0f)){e.clear();return true;}
  actual.source_playing39c_v92()=0; // source39530c BEFORE singleton/audio
  SoundEmitterAudioV92 manager;if(!s.sound_manager||!s.sound_manager(manager,e)||!manager.valid_nullable()){if(e.empty())e="Required actual nullable captured SoundManager";return false;}
  if(manager&&!manager.stop3d(actual.sound_id(),250,player_point.data(),maximum,e))return false;
  if(actual.destruction_started_v92()){e="SoundEmitter D1 entered during Stop3D";return false;}
  // Source rereads39c after callback; if changed positive it returns now.
  if(actual.source_byte39c()){e.clear();return true;}
 }
 // Source rereads398 after Stop3D, preserving native side-effect prefixes.
 const float maximum=actual.distance_max();
 if(!(maximum>distance)||!(maximum>0.0f)){e.clear();return true;}
 actual.source_playing39c_v92()=1; // source395378 BEFORE singleton/loop/pos
 SoundEmitterAudioV92 manager;if(!s.sound_manager||!s.sound_manager(manager,e)||!manager.valid_nullable()){if(e.empty())e="Required actual nullable SoundManager before Play3D";return false;}
 if(!manager){e.clear();return true;}
 if(!actual.loop()){e="Actual SoundEmitter374 unproduced at reached Play3D";return false;}
 emitter=actual.base().vector3(0x160);const std::array<float,3> point{emitter[0],emitter[1],emitter[2]};
 // Source integer1 and both floats -1; distMin394 never enters this body.
 return manager.play3d(actual.sound_id(),point,*actual.loop()!=0,1,-1.0f,-1.0f,e);
}
inline bool sound_emitter_destroy_v92(CanonicalSoundEmitterV32& actual,const SoundEmitterSourceServicesV92& s,std::string& e){
 if(!s.owner){e="Required independent SoundEmitter D1 authority";return false;}
 if(!actual.source_audio_stop_complete_v92()){
  if(actual.source_byte39c()){
   SoundEmitterAudioV92 manager;if(!s.sound_manager||!s.sound_manager(manager,e)||!manager.valid_nullable()){if(e.empty())e="Required actual captured SoundManager at SoundEmitterD1";return false;}
   // Original Stop's negative ordinal guard precedes any this dereference.
   // Positive UID plus native NULL singleton is not a fabricated success.
   if(actual.sound_id()>=0){if(!manager){e="Original SoundEmitterD1 positive Stop reached NULL manager";return false;}if(!manager.stop(actual.sound_id(),0,e))return false;}
  }
  actual.source_audio_stop_complete_v92()=true; // authentic completed/unused prefix
 }
 if(!actual.source_string_released_v92()){std::string{}.swap(actual.source_sound378_v92());actual.source_string_released_v92()=true;}
 if(!s.destroy_gameobject_base){e="Required SAME inherited GameObjectD2 after audio/String D1";return false;}
 return s.destroy_gameobject_base(actual,e); // Main native journal resumes partial D2
}
}
