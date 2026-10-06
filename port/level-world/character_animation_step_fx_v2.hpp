#pragma once
#include "../game-data/animation_tables.hpp"
#include <string>
namespace dh2::character {
struct AnimationStepFxServicesV2 {
 void* context{};
 int(*owner)(void*,std::uintptr_t&){};
 // Whole original Swoosh selection is required only for Swoosh=true rows.
 // Output is source r7 at3ca8e4 after the actual equipment/PlaySwoosh calls.
 int(*swoosh_fx_gate)(void*,const data::AnimationStep&,bool&){};
 int(*target_position)(void*,std::uintptr_t,float*){};
 int(*rotation)(void*,std::uintptr_t,float*){};
 int(*play)(void*,std::int32_t,const float*,const float*,std::uintptr_t){};
};
// Source _SetAnimStep3ca8e4..3ca91c, including anchored495f04 and
// unanchored495888. The source manager owns lifecycle; caller does not retain
// or stop an invented clip-local FX handle. All invalid IDs other than -1 are
// delegated to the actual source manager guard.
int character_animation_step_fx_v2(const data::AnimationStep&,
 const AnimationStepFxServicesV2&,std::string&);
}
