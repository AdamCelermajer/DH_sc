#pragma once
#include "audio_spatial_v34.hpp"
#include "audio_mixer_v34.hpp"
#include <string>
namespace dh2::audio {
struct VoxListenerAuthorityV38 {
 AudioListenerV34 listener;
 std::int32_t reference_distance{},maximum_distance{};
 float rolloff{};
};
// Update only fields written by PlaySoundPackSound36a7c0. Existing source
// emitter fields (velocity/general/doppler/etc.) retain their genuine owner.
bool vox_source_emitter_fields_v38(const VoxListenerAuthorityV38&,
 int source_position_type,const float source_position[3],float reference_override,
 float maximum_override,AudioSpatialSourceV34&,std::string&);
// The caller supplies the actual emitter/group gain and pitch from the source
// DSP owner, not inferred settings or XML labels. No level/ready gate changes.
bool vox_source_spatial_command_v38(const VoxListenerAuthorityV38&,
 const AudioSpatialSourceV34&,float actual_gain,float actual_pitch,
 AudioCommandV34&,std::string&);
}
