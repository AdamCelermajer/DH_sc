#pragma once
#include "character_script_owner.hpp"
#include "character_script_owner_v2.hpp"
#include "../script-runtime/script_object_bridge.h"
namespace dh2::character {
// Actual captured selected C1 vtables at+b0. Selection comes exclusively from
// the privately retained Session kind, never the renderer actor category.
bool character_ais_kill_method_v23(const ScriptSessionView&,std::uintptr_t&)noexcept;
int character_ais_kill_vm_v23(ScriptOwner&,std::uintptr_t actual_selected_receiver,
 std::uintptr_t captured_source_method,std::uintptr_t killed,
 const dh2_script_callback_scope*,std::string&);
int character_ais_kill_vm_v23(ScriptOwnerV2&,std::uintptr_t actual_selected_receiver,
 std::uintptr_t captured_source_method,std::uintptr_t killed,
 const dh2_script_callback_scope*,std::string&);
}
