#pragma once
#include "character_script_owner_v2.hpp"
#include "../script-runtime/script_object_bridge.h"
namespace dh2::character {
// SAME ScriptOwnerV2 retained constructor/VM counterpart of existing V1
// character_ais_death_vm_v2. No ScriptOwner/VM creation or owner conversion.
int character_ais_death_vm_v56(ScriptOwnerV2&,std::uintptr_t receiver,
 std::uintptr_t source_method,std::uintptr_t attacker,
 const dh2_script_callback_scope*,std::string&);
}
