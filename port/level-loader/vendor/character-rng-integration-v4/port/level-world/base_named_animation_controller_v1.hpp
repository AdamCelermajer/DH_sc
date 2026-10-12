#pragma once
#include "visual_timeline.hpp"
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
struct BaseNamedAnimationBorrowV1 {
 std::shared_ptr<void> owner;
 timeline::State* timeline{};
 std::uint32_t* scene_flags11c{};
 // Actual timeline library count/name lookup, distinct from animator selection.
 std::int32_t timeline_library_count{};
 std::function<bool(const char*,std::int32_t&,std::string&)> timeline_find_name;
 // Original CTimeline vtable +44 is getLoop666c30, not an ended predicate.
 std::function<bool(bool&,std::string&)> timeline_get_loop;
 std::function<bool(std::int32_t&,std::string&)> animator_current;
 std::function<bool(const char*,std::int32_t&,std::string&)> animator_find_name;
 std::function<bool(std::int32_t,std::string&)> animator_select;
 const std::int32_t* applicator_extra_ms{}; // null source applicator => zero
 std::function<bool(bool,std::string&)> root_new_anim;
};
// Full positive named source AnimController::PlayClip4749f4 tail over the SAME
// animator/timeline/scene. Caller supplies original GetAnim(group) result.
bool base_named_animation_play_v1(BaseNamedAnimationBorrowV1&,const char*,bool loop,bool& accepted,std::string&);
// Exact base virtual SetCallbacks474d10 contains bx lr; no callback writes.
void base_named_animation_set_callbacks_v1() noexcept;
}
