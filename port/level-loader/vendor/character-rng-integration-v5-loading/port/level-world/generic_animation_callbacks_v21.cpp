#include "generic_animation_callbacks_v21.hpp"
namespace dh2::world {
GenericAnimationCallbacksV21 GenericAnimatorCallbackFieldsV21::source_do_nothing(){GenericAnimationCallbacksV21 f;f.completion=[](timeline::State&,std::string&){return true;};f.event=[](const animation::TriggeredEvent&,std::string&){return true;};return f;}
bool GenericAnimatorCallbackFieldsV21::check(timeline::Completion& pending,timeline::State& t,std::string& e){
 // CheckCallback36440c leaves pending set if callback34 is NULL. It clears
 // only AFTER callback delivery, including a reentrant source Play selection.
 if(!pending.pending||!fields_.completion)return true;auto fn=fields_.completion;auto lease=fields_.completion_owner;
 if(!fn(t,e))return false;pending.pending=0;return true;
}
bool generic_animation_set_callbacks_all_v21(const std::vector<GenericAnimatorCallbackBorrowV21>& list,GenericAnimationCallbacksV21 f,std::string& e){
 for(const auto& item:list){if(!item.identity||!item.owner||!item.actual_fields){e="Required actual retained root animator callback receiver";return false;}item.actual_fields->set(f);}return true;
}
}
