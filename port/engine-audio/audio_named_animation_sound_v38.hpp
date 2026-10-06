#pragma once
#include "audio_source_bindings_v38.hpp"
#include "audio_world_producer_v38.hpp"
#include <cstring>
namespace dh2::audio {
struct AudioNamedAnimationSoundServicesV38 {
 void* context{};
 std::uintptr_t actual_character{};
 int(*manager)(void*,std::uintptr_t&){};
 int(*target_position)(void*,std::uintptr_t,std::array<float,3>&){};
 int(*play)(void*,const character::CombatSoundPlayV1&){};
};
struct AudioNamedAnimationSoundResultV38 {
 std::int32_t source_id{-1};unsigned phase{};const char* required{};
};
// Handles ONLY melee_event_sound_fx: the existing whole melee-event owner
// has already queried step_index then step_count and removed the sfx_ prefix.
// Whole original name leaf3d44f0..4598; names are genuine source ordinals.
inline int audio_named_animation_sound_v38(const char* suffix,
 const AudioSourceBindingsV38& bindings,const AudioNamedAnimationSoundServicesV38& s,
 AudioNamedAnimationSoundResultV38& result){
 result={};
 auto fail=[&](const char* required){result.required=required;return -2;};
 if(!suffix){result.required="Required authored sfx_ name suffix";return -1;}
 result.phase=1;
 const auto& names=bindings.names();
 for(std::size_t i=0;i<names.size();++i){
  if(!std::strcmp(suffix,names[i].c_str())){result.source_id=static_cast<std::int32_t>(i);break;}
 }
 if(result.source_id<0)return 0; // original miss: no manager/position/play
 result.phase=2;std::uintptr_t manager{};
 if(!s.manager||s.manager(s.context,manager)||!manager)return fail("Required same actual VoxSoundManager");
 // Capture manager before the actual GetTargetPosition callback. Do not
 // query it again if that callback changes the surrounding global manager.
 result.phase=3;std::array<float,3> position{};
 if(!s.actual_character||!s.target_position||s.target_position(s.context,s.actual_character,position))return fail("Required actual Character GetTargetPosition");
 const auto request=audio_world_request_v38(manager,s.actual_character,result.source_id,position);
 result.phase=4;
 if(!s.play||s.play(s.context,request))return fail("Required actual Vox Play3D named animation sound");
 return 0;
}
}
