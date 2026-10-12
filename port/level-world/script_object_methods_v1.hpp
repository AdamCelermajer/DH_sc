#pragma once

// Shared Lua source for GameObject identity tables.
//
// The original Binder exposes every GameObject returned to a script (the
// UserData ReturnValues entries of GetSelf/GetTargetListTop/GetTarget and the
// attacker/defender arguments of the AIS combat callbacks) as a Lua table whose
// `_this` field holds the native GameObject pointer and whose metatable carries
// the object methods. The port projects such a table to a source-value type 7
// identity when it is passed back into a bound native function
// (dh2_script_vm_bind_source_values reads `_this`).
//
// This prelude is only the table convention: the object-method table, its
// metatable, and the `__dh2_object(identity)` constructor. Each VM then adds
// its own method entries to `__dh2_object_methods`:
//   * monster/follower VMs forward to the `__dh2_obj_*` globals
//     (monster_external_script_session.cpp);
//   * the Player skill VM binds the source `Kind::method` rows of
//     character_native_bindings as hidden natives and installs them directly
//     (player_skill_session_v1.cpp).
//
// The macro expands to a plain string literal so that it can be concatenated
// with a VM-specific raw string by adjacent-literal concatenation. The Lua text
// is byte-for-byte what monster_external_script_session.cpp loaded before it was
// factored out; changing it changes the monster VM.
#define DH2_SCRIPT_OBJECT_METHODS_PRELUDE_V1 R"lua(
local __dh2_object_methods = {}
function __dh2_object_methods:GetID() return self._this end
local __dh2_object_mt = {__index = __dh2_object_methods}
local function __dh2_object(identity)
    return setmetatable({_this = identity}, __dh2_object_mt)
end
)lua"
