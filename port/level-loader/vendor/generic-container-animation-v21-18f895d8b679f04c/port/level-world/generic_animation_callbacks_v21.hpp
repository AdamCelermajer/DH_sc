#pragma once
#include "visual_timeline.hpp"
#include "../engine-animation/events.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>
namespace dh2::world {
struct GenericAnimationCallbacksV21 {
 std::shared_ptr<void> completion_owner,event_owner;
 std::function<bool(timeline::State&,std::string&)> completion;
 std::function<bool(const animation::TriggeredEvent&,std::string&)> event;
};
// Native source callback-field owner for ONE actual generic animator. Timeline,
// completion pending/extra and event track remain on its retained visual.
class GenericAnimatorCallbackFieldsV21 {
 GenericAnimationCallbacksV21 fields_;
 animation::EventCursor event_cursor_; // setEventsTrack60fab8 source+10 = -1
public:
 GenericAnimatorCallbackFieldsV21()=default; // AnimApplicator C1 callback34/38 NULL
 void set(GenericAnimationCallbacksV21 fields){fields_=std::move(fields);}
 const GenericAnimationCallbacksV21& fields()const noexcept{return fields_;}
 animation::EventCursor& event_cursor()noexcept{return event_cursor_;}
 // Actual controller C1 installs source DoNothing endpoints on existing list.
 static GenericAnimationCallbacksV21 source_do_nothing();
 bool check(timeline::Completion&,timeline::State&,std::string&);
};
struct GenericAnimatorCallbackBorrowV21 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 GenericAnimatorCallbackFieldsV21* actual_fields{};
};
// Exact ordered SetCallbacksOnAll474cac over the actual root animator list.
// No independently manufactured animator IDs/list entries are accepted.
bool generic_animation_set_callbacks_all_v21(const std::vector<GenericAnimatorCallbackBorrowV21>& actual_list,GenericAnimationCallbacksV21,std::string&);
}
