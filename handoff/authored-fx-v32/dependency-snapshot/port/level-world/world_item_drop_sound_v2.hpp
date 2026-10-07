#pragma once
#include "vox_play3d_owner_v2.hpp"
namespace dh2::character {
// Item.InitAgain3ec1c4..3ec210: same Vox manager and cached GameObject+1a8.
bool world_item_drop_sound_v2(sound::VoxPlay3DOwnerV2&,std::uintptr_t manager,std::uintptr_t item,std::int16_t sound,const float* cached_position1a8,std::string&);
}
