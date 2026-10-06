#pragma once
#include "retained_gameobject_visual_v1.hpp"
#include "base_index_animation_controller_v21.hpp"
namespace dh2::world {
// Sole callback fields for the actual generic animator already owned by visual.
// No additional timeline, event track, animation pose or Character state.
class RetainedGenericAnimatorV21 {
 std::shared_ptr<RetainedGameObjectVisualV1> visual_;
 GenericAnimatorCallbackFieldsV21 callbacks_;
 bool initialized_{},failed_{};
public:
 explicit RetainedGenericAnimatorV21(std::shared_ptr<RetainedGameObjectVisualV1> v):visual_(std::move(v)){}
 bool initialize(std::string&);
 bool list(std::vector<GenericAnimatorCallbackBorrowV21>&,std::string&);
 bool set_callbacks(GenericAnimationCallbacksV21,std::string&);
 bool frame(std::uint32_t,std::string&);
 bool count(std::uint32_t&,std::string&);
 bool play_index(std::uint32_t,bool,bool&,std::string&);
 void clear()noexcept{callbacks_.set({});}
 GenericAnimatorCallbackFieldsV21& actual_fields()noexcept{return callbacks_;}
};
}
