#pragma once
#include "animation_tables.hpp"
namespace dh2::data {
struct AnimationFrame {std::int32_t sequence=-1,loops=0;std::uint32_t step=0;};
enum class AnimationSwap {rejected=-1,no_op=0,metadata=1,restart=2};
class AnimationScheduler;
struct AnimationSelectionServices {
 void* context=nullptr;
 void(*event)(void*,AnimationScheduler&,std::uint32_t)=nullptr;
 bool(*prepare)(void*,AnimationScheduler&)=nullptr;
};
// CharAnimator::Update's synchronous services. Event return values are ignored
// in the original. prepare/finish return bool solely to propagate native backend
// errors; they do not implement an accepted/veto selection policy.
struct AnimationCompletionServices {
 void* context=nullptr;
 void(*event)(void*,AnimationScheduler&,std::uint32_t)=nullptr;
 bool(*has_pending)(void*)=nullptr;
 bool(*prepare)(void*,AnimationScheduler&)=nullptr;
 bool(*finish)(void*,AnimationScheduler&)=nullptr;
 // Optional observed _SetAnim(sequence,depth) repeat boundary BEFORE its
 // deeper selection/redirect services. No value/return veto is introduced.
 void(*initialize)(void*,AnimationScheduler&,std::int32_t,std::uint32_t)=nullptr;
 const AnimationSelectionServices* selection=nullptr;
};
class AnimationScheduler {
 std::vector<AnimationFrame> stack;
 AnimationStep current;
 bool playing=false;
 void enter(const AnimationTables&,std::int32_t,AnimationRandom&,bool);
 void activate(const AnimationTables&,AnimationRandom&,bool);
 bool enter_with_services(const AnimationTables&,std::int32_t,std::size_t,AnimationRandom&,bool,const AnimationSelectionServices&);
 bool activate_with_services(const AnimationTables&,std::uint32_t,AnimationRandom&,bool,const AnimationSelectionServices&);
public:
 bool start(const AnimationTables&,std::int32_t,AnimationRandom&,std::string&,bool random_enabled=true);
 // Live _SetAnim/_SetAnimStep order: frame metadata ->24 -> RNG -> stored
 // step ->26 -> retained step redirect/leaf -> prepare. Errors do not roll
 // synchronous callback effects back. Tables remain immutable throughout.
 bool start_with_services(const AnimationTables&,std::int32_t,AnimationRandom&,std::string&,
                          const AnimationSelectionServices&,bool random_enabled=true);
 void set_step(std::uint32_t index){if(!stack.empty())stack.back().step=index;}
 void skip_next_step(){if(!stack.empty())++stack.back().step;}
 bool complete(const AnimationTables&,AnimationRandom&,std::string&,bool random_enabled=true);
 // Separate source-live choreography; historical complete/gold stay unchanged.
 // Callback ANIM_Set queues in the owner while pending49 is set. Metadata swap
 // may change live frames. Resources/tables remain immutable during callbacks.
 bool complete_with_services(const AnimationTables&,AnimationRandom&,std::string&,
                             const AnimationCompletionServices&,bool random_enabled=true);
 // ANIM_Swap metadata branch does not select/replay a clip. A restart result
 // delegates ANIM_Set and restoration of the existing global speed to owner.
 AnimationSwap swap_sequences(const AnimationTables&,std::int32_t desired,std::int32_t old,std::string&);
 void stop_loop(){if(!stack.empty())stack.back().loops=0;}
 // CharAnimator::ANIM_Stop writes depth0 without selecting another frame.
 // Preserve frame0 metadata; do not run completion/selection callbacks.
 void reset_depth_v1(){if(stack.size()>1)stack.resize(1);}
 bool active()const{return playing;}
 const AnimationStep& clip()const{return current;}
 const std::vector<AnimationFrame>& frames()const{return stack;}
};
}
// Original completion decision only: 0 repeat, 1 next step, 2 finish,
// 3 invalid caller state. Caller dispatches selection, events and parent unwind.
extern "C" unsigned dh2_animation_complete(std::int32_t type,std::uint32_t count,std::uint32_t* step,std::int32_t* loops);
