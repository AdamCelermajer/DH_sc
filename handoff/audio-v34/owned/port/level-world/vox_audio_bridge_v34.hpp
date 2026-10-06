#pragma once
#include "vox_play3d_owner_v2.hpp"
#include "../engine-audio/audio_bank_v34.hpp"
namespace dh2::sound {
struct VoxAudioAuthoritiesV34 {
 // Every early/source platform gate is supplied by the genuine World/Vox.
 VoxPlay3DServicesV2 gates;
 void*context{};
 bool(*output_ready)(void*,std::string&){};
 bool(*event_frame)(void*,std::uint64_t&,std::string&){};
 // Resolves source actual listener, position, relative type, pitch/ref/max
 // distances and source overrides. Transport does not invent a listener.
 bool(*source_command)(void*,const character::CombatSoundPlayV1&,
  const audio::AudioSoundV34&,const audio::AudioGroupV34&,audio::AudioCommandV34&,std::string&){};
 audio::AudioRandomV34 random;
};
class VoxAudioBridgeV34 {
 audio::AudioCatalogV34&catalog_;audio::AudioSampleBankV34&bank_;
 std::vector<audio::AudioSoundAutoGenV34>bindings_;
 VoxAudioAuthoritiesV34 authorities_;std::uintptr_t manager_{};std::string error_;
 static int invoke(void*,const VoxPlay3DRequestV2&,VoxPlay3DResponseV2&);
 int emit(const character::CombatSoundPlayV1&,int uid,bool event);
public:
 VoxAudioBridgeV34(audio::AudioCatalogV34&c,audio::AudioSampleBankV34&b,
  std::uintptr_t actual_manager,VoxAudioAuthoritiesV34 a):catalog_(c),bank_(b),authorities_(a),manager_(actual_manager){}
 bool bind_source_autogen(std::vector<audio::AudioSoundAutoGenV34>,std::string&);
 VoxPlay3DServicesV2 services(){return {this,invoke};}
 const std::string&error()const noexcept{return error_;}
};
}
