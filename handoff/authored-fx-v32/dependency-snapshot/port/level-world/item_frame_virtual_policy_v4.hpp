#pragma once
#include "world_item_object_owner_v1.hpp"
#include "actor_runtime.hpp"
namespace dh2::character {
// Actual Item vtable inherits these GameObject leaves, overrides speed3ebc04.
// Input is the real constructor/default/runtime owner, never Character flags.
bool item_frame_virtual_policy_v4(RetainedWorldItemObjectV1&,
 actor::GenericRuntimeRequestV4&,actor::RuntimePolicy&,std::string&);
}
