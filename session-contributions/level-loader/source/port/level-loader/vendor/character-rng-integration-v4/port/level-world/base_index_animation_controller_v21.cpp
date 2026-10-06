#include "base_index_animation_controller_v21.hpp"
#include <cstring>
namespace dh2::world {
bool base_index_animation_play_v21(BaseNamedAnimationBorrowV1& b,std::uint32_t index,bool loop,bool& accepted,std::string& e){
 accepted=false;if(!b.owner||!b.timeline||!b.scene_flags11c||!b.animator_current||!b.animator_select||!b.timeline_get_loop||!b.root_new_anim){e="Required actual index-animation animator/timeline/scene owner";return false;}
 if(std::uint32_t(b.timeline_library_count)<=index)return true;
 std::int32_t old{},selected{};std::memcpy(&selected,&index,4);
 if(!b.animator_current(old,e)||!b.animator_select(selected,e))return false;
 if(old==selected){bool looped{};if(!b.timeline_get_loop(looped,e))return false;if(!looped){auto extra=b.applicator_extra_ms?*b.applicator_extra_ms:0;auto raw=std::uint32_t(b.timeline->start_ms)+std::uint32_t(extra);std::int32_t jump{};std::memcpy(&jump,&raw,4);if(dh2_timeline_jump(b.timeline,jump)){e="Source index timeline jump rejected";return false;}}}
 if(dh2_timeline_loop(b.timeline,loop?1u:0u)||dh2_timeline_scale(b.timeline,1.f)){e="Source index timeline loop/scale rejected";return false;}if(!b.root_new_anim(false,e))return false;*b.scene_flags11c|=0x200u;accepted=true;return true;
}
}
