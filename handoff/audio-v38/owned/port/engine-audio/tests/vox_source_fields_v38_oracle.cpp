#include "../vox_source_fields_v38.hpp"
#include <cstring>
extern "C" unsigned dh2_vox_source_fields_v38(const unsigned char*in,unsigned char*out){
 dh2::audio::VoxListenerAuthorityV38 authority;static_assert(sizeof authority==60);std::memcpy(&authority,in,60);int type;std::memcpy(&type,in+60,4);float position[3],ref,max;std::memcpy(position,in+64,12);std::memcpy(&ref,in+76,4);std::memcpy(&max,in+80,4);dh2::audio::AudioSpatialSourceV34 source;std::memcpy(&source,in+84,52);std::string error;if(!dh2::audio::vox_source_emitter_fields_v38(authority,type,position,ref,max,source,error))return 0;std::memcpy(out,&source,52);return 52;
}
