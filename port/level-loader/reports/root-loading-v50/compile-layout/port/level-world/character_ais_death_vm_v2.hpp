#pragma once
#include "character_script_owner.hpp"
#include "../script-runtime/script_object_bridge.h"
namespace dh2::character {
// Captured selected AIS receiver. The original external OnDied is unconditional
// one source GameObject argument; Default/inherited bodies are genuine bx-lr.
int character_ais_death_vm_v2(ScriptOwner&,std::uintptr_t receiver,
 std::uintptr_t source_method,std::uintptr_t attacker,
 const dh2_script_callback_scope*,std::string& diagnostic);
}
