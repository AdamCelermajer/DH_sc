#pragma once
#include "audio_producer_transport_v40.hpp"
#include "../../audio_world_producer_v38.hpp"
namespace dh2::audio {
// Complete source emission leaf, NOT a replacement for an object's Init,
// interaction, attack, loot, animation or physics/lifecycle owner.
struct AudioSourceEmissionV40 {
 void* context{};std::uintptr_t actual_subject{};
 bool(*actual_manager)(void*,std::uintptr_t&,std::string&){};
 bool(*actual_sound_id)(void*,std::int32_t&,std::string&){};
 bool(*actual_source_position)(void*,std::array<float,3>&,std::string&){};
 AudioProducerTransportV40 transport;
};
// Container.Interact3a0bd4 captures manager before sound virtual+d8,
// then copies raw position160 and calls Play3D3a0c20. Actual producer
// invokes this at THAT reached emission site, after its real source prefix.
inline bool audio_emit_container_source_v40(const AudioSourceEmissionV40& s,
 std::string& error){
 std::uintptr_t manager{};std::int32_t sound{};std::array<float,3> position{};
 if(!s.actual_manager||!s.actual_manager(s.context,manager,error)){if(error.empty())error="Required actual container Vox manager";return false;}
 if(!s.actual_sound_id||!s.actual_sound_id(s.context,sound,error)){if(error.empty())error="Required actual container sound getter";return false;}
 if(!s.actual_source_position||!s.actual_source_position(s.context,position,error)){if(error.empty())error="Required actual container raw position160";return false;}
 return audio_submit_actual_producer_v40(s.transport,
  audio_world_request_v38(manager,s.actual_subject,sound,position),error);
}
// Item producer has already captured its actual manager, signed-short ID
// and source position at the genuine reached emission site. Drop uses
// cached1a8; pickup uses raw160. Never reinterpret one as the other.
inline bool audio_emit_actual_item_v40(const AudioProducerTransportV40& transport,
 std::uintptr_t captured_manager,std::uintptr_t actual_item,std::int16_t sound,
 const std::array<float,3>& actual_source_position,std::string& error){
 return audio_submit_actual_producer_v40(transport,
  audio_world_request_v38(captured_manager,actual_item,sound,actual_source_position),error);
}
}
