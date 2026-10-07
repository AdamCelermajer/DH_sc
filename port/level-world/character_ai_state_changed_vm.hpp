#pragma once
#include "character_script_owner.hpp"
namespace dh2::character {
// Captured AISExternal identity selects its retained private VM/alias map even
// after active changes. Do not destroy owner/session during a synchronous call.
// Optional scope must be live and belong to that same captured session VM.
// 0 delivered, -1 malformed/unknown/nonExternal session; runtime errors preserved.
int character_ais_external_end_anim(ScriptOwner&,std::uintptr_t captured_identity,
 const dh2_script_callback_scope* scope=nullptr);
}
