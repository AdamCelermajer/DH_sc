#pragma once
#include "subobjects_update.hpp"
#include <cstddef>
#include <cstdint>
namespace dh2::visual {
struct Delta {std::uint32_t timestamp;float previous[3],value[3];};
struct Root {
 float position[3],quaternion[4],scale[3],animated[3],secondary[3],helper[3];
 std::uint32_t presence,flags,animated_flags,secondary_flags,helper_flags;
};
// presence: bit0 helper, bit1 secondary; all other bits rejected.
struct Displacement {Root* root;const float* deltas;std::uint32_t count,reserved;};
// State nullptr represents absent owner. Root nullptr represents absent scene.
// flags: bit0 owner has auxiliary, bit1 owner has this visual attached.
// AbsolutePosition event receives XYZ; backend rebuilds dirty node transforms.
enum Event : std::uint32_t {absolute_position=19};
struct Request {
 subobjects::State* state;Root* root;physical::BodyState* body;
 physical::TransformRequest* transform;const subobjects::Services* services;
 std::uint32_t flags,reserved;
};
static_assert(sizeof(Delta)==28&&sizeof(Root)==96&&sizeof(Displacement)==24&&sizeof(Request)==48);
}
extern "C" {
// 0 success, -1 malformed. IEEE values intentionally retain original behavior.
int dh2_visual_calculate_delta(dh2::visual::Delta*,std::uint32_t,const float* xyz);
int dh2_visual_reset_delta(dh2::visual::Delta*,std::uint32_t,const float* xyz);
// Returns original moved bool 0/1, -1 malformed before any mutation.
int dh2_visual_displace(const dh2::visual::Displacement*);
int dh2_visual_apply_position(const dh2::visual::Request*);
int dh2_visual_sync_position(const dh2::visual::Request*);
// Original VisualObject::SetRotation uses Euler (Y,-X,-Z) radians before
// constructing the normalized engine quaternion. SyncRotation passes owner's
// XYZ at +16c,+170,+174 (the third field is State::rotation).
int dh2_visual_rotation(float* quaternion4,const float* euler3);
}
#ifndef DH2_VISUAL_KERNEL_ONLY
#include "../engine-animation/animation.hpp"
namespace dh2::visual {
 // Owns root-motion history, borrows no scene storage. Caller supplies actual
 // timeline timestamp and explicit restart boundary when changing/replaying
 // clips. Restart reproduces NewAnim: sample clip start at timestamp+1, then
 // animate current clip time at timestamp. Timeline wrapping is external.
class SceneBinding {
 std::int32_t animated=-1;
 std::vector<std::string> identities;
 Delta history{};
public:
 Root root{{},{0,0,0,1},{1,1,1},{},{},{},1,0,0,0,0};
 bool bind(const scene::Scene&,std::string& error);
 bool set_rotation(const float* euler3);
 std::int32_t animated_node()const{return animated;}
 bool sample(scene::Scene&,const animation::Player&,std::int32_t milliseconds,
             std::uint32_t timestamp,bool reset,std::string& error);
 // Animation applicators still track delta when root displacement is disabled;
 // the explicit step policy controls whether HandleDisplacement consumes it.
 bool sample(scene::Scene&,const animation::Player&,std::int32_t milliseconds,
             std::uint32_t timestamp,bool reset,bool displacement,std::string& error);
 // Rebuilds owner * helper * authored graph transforms, including instances.
 bool update_world(scene::Scene&,std::string& error)const;
};
}
#endif
