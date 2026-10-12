#include "base_named_animation_controller_v1.hpp"
#include <cstring>
namespace dh2::world {
void base_named_animation_set_callbacks_v1() noexcept{}
bool base_named_animation_play_v1(BaseNamedAnimationBorrowV1& b,const char* name,bool loop,bool& accepted,std::string& e){
 accepted=false;
 if(!name||!b.timeline||!b.scene_flags11c||!b.animator_current||!b.animator_find_name||!b.animator_select||!b.root_new_anim){e="Required actual named-animation animator/timeline/scene owner";return false;}
 if(b.timeline_library_count>0){
  if(!b.timeline_find_name){e="Required actual timeline named-clip lookup";return false;}
  std::int32_t found=-1;if(!b.timeline_find_name(name,found,e))return false;
  if(found==-1)return true;
 }
 std::int32_t old=-1,selected=-1;
 if(!b.animator_current(old,e)||!b.animator_find_name(name,selected,e)||!b.animator_select(selected,e))return false;
 bool jump_needed=false;
 if(old==selected){if(!b.timeline_get_loop){e="Required actual timeline getLoop virtual";return false;}
  bool looped=false;if(!b.timeline_get_loop(looped,e))return false;jump_needed=!looped;
 }
 if(jump_needed){
  const auto extra=b.applicator_extra_ms?*b.applicator_extra_ms:0;
  auto raw=std::uint32_t(b.timeline->start_ms)+std::uint32_t(extra);std::int32_t jump=0;std::memcpy(&jump,&raw,4);
  if(dh2_timeline_jump(b.timeline,jump)){e="Original timeline jump rejected actual binding";return false;}
 }
 if(dh2_timeline_loop(b.timeline,loop?1u:0u)||dh2_timeline_scale(b.timeline,1.f)){e="Original timeline loop/scale rejected actual binding";return false;}
 if(!b.root_new_anim(false,e))return false;
 *b.scene_flags11c|=0x200u;accepted=true;return true;
}
}
