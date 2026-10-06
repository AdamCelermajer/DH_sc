#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
namespace dh2::world {
struct VisualAabbDispatchV3 {CanonicalGameObjectBaseOwnerV1* receiver{};void* context{};bool(*apply)(void*,const float* input6,bool flat,std::string&){};};
// Dynamic native callback capability for the actual synchronous virtual+9c
// call inside VisualObject construction. No base/visual fields are mirrored.
class VisualAabbDispatchScopeV3 {
 const VisualAabbDispatchV3* previous_{};VisualAabbDispatchV3 binding_;
public:
 explicit VisualAabbDispatchScopeV3(VisualAabbDispatchV3);
 ~VisualAabbDispatchScopeV3();
 VisualAabbDispatchScopeV3(const VisualAabbDispatchScopeV3&)=delete;
};
bool visual_aabb_dispatch_v3(CanonicalGameObjectBaseOwnerV1&,const float*,bool,bool& handled,std::string&);
// Actual Item virtual3ebfa0 ignores input bbox, scales existing XY by1.5 only
// when flat=false, then calls UpdateAbsoluteAABB38aac8. No PF call occurs.
bool item_relative_aabb_v3(CanonicalGameObjectBaseOwnerV1&,bool flat,std::string&);
}
