#pragma once
#include "base_named_animation_controller_v1.hpp"
namespace dh2::world {
// Original AnimController::PlayClip(unsigned,bool,int,unsigned)474b50.
// Borrow is SAME animator/timeline; caller already resolved GetAnim(group).
bool base_index_animation_play_v2(BaseNamedAnimationBorrowV1&,std::uint32_t index,bool loop,bool& accepted,std::string&);
}
