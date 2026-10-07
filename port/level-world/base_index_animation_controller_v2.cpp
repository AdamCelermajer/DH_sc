#include "base_index_animation_controller_v2.hpp"
#include <cstring>
namespace dh2::world {
bool base_index_animation_play_v2(BaseNamedAnimationBorrowV1& b,std::uint32_t index,bool loop,bool& accepted,std::string& e){
 accepted=false;
 if(!b.timeline||!b.scene_flags11c||!b.animator_current||!b.animator_select||!b.root_new_anim){e="Required same index-animation animator/timeline/scene";return false;}
 // Original unsigned libraryCount <= index rejects before selection.
 if(static_cast<std::uint32_t>(b.timeline_library_count)<=index)return true;
 std::int32_t old=-1;std::int32_t selected;std::memcpy(&selected,&index,4);
 if(!b.animator_current(old,e)||!b.animator_select(selected,e))return false;
 if(old==selected){
  if(!b.timeline_get_loop){e="Required actual index-animation getLoop";return false;}
  bool looped=false;if(!b.timeline_get_loop(looped,e))return false;
  if(!looped){const auto extra=b.applicator_extra_ms?*b.applicator_extra_ms:0;auto raw=std::uint32_t(b.timeline->start_ms)+std::uint32_t(extra);std::int32_t jump;std::memcpy(&jump,&raw,4);
   if(dh2_timeline_jump(b.timeline,jump)){e="Actual index-animation jump rejected";return false;}}
 }
 if(dh2_timeline_loop(b.timeline,loop?1u:0u)||dh2_timeline_scale(b.timeline,1.f)){e="Actual index-animation loop/scale rejected";return false;}
 if(!b.root_new_anim(false,e))return false;
 *b.scene_flags11c|=0x200u;accepted=true;return true;
}
}
