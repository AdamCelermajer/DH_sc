#include "audio_source_command_v40.hpp"
namespace dh2::audio {
bool audio_source_command_v40(const AudioSourceCommandAuthorityV40&a,
 const dh2::character::CombatSoundPlayV1&play,const AudioSoundV34&sound,
 const AudioGroupV34&group,AudioCommandV34&out,std::string&error){
 if(!a.manager||a.manager!=play.manager||!a.actual_current_level||!a.world_epoch||a.listener_world_epoch!=a.world_epoch||!a.soundpack_initialized||!a.manager_general_initialized||!a.listener_initialized){error="Required actual same World/GS/Vox/listener completed source authorities";return false;}
 auto emitter=original_fresh_emitter_fields_v40(a.general);
 if(!vox_source_emitter_fields_v38(a.listener,group.position_type,play.position.data(),play.source_float0,play.source_float1,emitter,error))return false;
 AudioCommandV34 next=out;
 if(!vox_source_spatial_command_v38(a.listener,emitter,a.actual_gain,a.actual_pitch,next,error))return false;
 if(sound.format==2){if(a.actual_native_initial_state<0){error="Required source native initial state from actual music owner";return false;}next.native_state=a.actual_native_initial_state;}
 out=next;error.clear();return true;
}
}
