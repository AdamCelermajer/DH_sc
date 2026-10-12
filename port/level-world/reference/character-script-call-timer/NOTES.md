# Selected script timer call composition

`character_script_call_timer.hpp/.cpp` supplies an additive borrowed
`ScriptTimerCall16 { vm, aliases }` and
`dh2_character_script_call_timer(call, uint32_t timer_id)`. Both pointers must
belong to the same actual LuaScript/session and remain alive through the call.
The pair does not allocate, publish, select or own an AIS. The source active
AIS gate remains in the existing event35 -> CharAI.OnScriptTimer relay; a
caller must not use the adapter to bypass that gate.

The selected iPhone inherits AISDefault::OnScriptTimer at `0x3dcc80`, which
constructs Arguments, pushes one signed integer at `0x3cdd78`, then calls
LuaScript's discarded-return overload `0x37c41c` with literal `OnTimer`.
`Value(int)` at `0x37ca9c` calls signed32-to-float32 helper `0x30e964` and
number setter `0x31b5e8`. Timer ID bits are therefore interpreted as signed32;
UINT32_MAX becomes -1, and IDs above float32's integer precision round as in
the source. The adapter bit-copies to int32 before the float conversion.

LuaScript.Call enters `0x37c390` and `_GetFuncName 0x37c314` once. The resolver
uses the per-script hash tree; it does not recursively follow alias chains.
Instance.pCall then looks up the resulting name in current Lua globals.
The adapter invokes the existing genuine alias-aware discard-source path,
preserving all returned Value projections rather than requesting zero Lua
results. A returned table's normal `_this` lookup can execute `__index` and
start a real Character timer even when the return is discarded.

## Original and native proof

`probe_original.py` executes 576 actual original composed calls, with 288
active selected calls: 96 timer IDs, three alias configurations, and active
or absent AIS. It executes the actual CharAI gate, AISDefault body, integer
Arguments/Value conversion bodies, discarded LuaScript.Call wrapper, alias
resolver and Instance global lookup. Original allocation/container/string
cleanup and complete VM call execution are explicit services. The arithmetic
helper uses the existing common AEABI signed32 -> float32 import contract.

Gold SHA-256:
`5c1bcf22c656d9fdfc15243b9b16af08f0f6fed6e817f1124be9a4bedec232d8`.
Original ELF SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The new capture manifest and assembly bind all eight focused routines.

`tests/character_script_call_timer.cpp` replays all 576 gold cases through
the real native event35/relay, current alias source and actual float32 Lua VM.
There are 7,500 corpus checks and 108 integration checks, zero mismatches.
Integration loads exact AI commons, uses real CharacterTimers expiry, replaces
aliases and globals while preserving one-pass selection, and verifies forty
discarded table projections in order. The fortieth projection starts a timer
in the actual slot cleared before expiry. Body errors, projection errors,
missing/empty aliased globals and malformed borrowed pairs remain explicit
protected diagnostics. Mutations before errors survive. The inactive AIS gate
suppresses calls even with a bad alias; global-blocked/controller-locked facts
do not suppress this source event35 branch.

`tests/character_script_call_timer_host.py` compiles only this new bridge/test
and current alias source, then links the existing genuine world and Lua DSOs.
It records exact source/binary/compiler/dependency bindings and leaves those
DSOs unchanged. Its dedicated report is
`reports/character-script-call-timer-host-audit.json`: PASS under ASan/UBSan,
zero findings. Current alias source is compiled directly into the audit; this
does not claim it is already rebuilt into the production runtime DSO. No
APK execution, full original VM parity or original manager ownership is proved.

## Integration and lifetime

The owner supplies the actual selected session's VM/map pair to its virtual
0x90 service. The existing dispatcher supplies GetID's uint32 word. Diagnostics
are native protected-call results, not an invented original AIS return value.
No CMake, renderer, existing timer binding or runtime API was changed.

The separately recovered LuaScript destructor clears backup/main aliases
before Instance destruction. The additive
`dh2_script_alias_clear_contents(map)` preserves the wrapper and tracking flag;
an owner must clear contents, close its VM, then destroy the wrapper. This
keeps native alias callback context alive if actual Lua `__gc` invokes it.
Full source loaded-set, path and Arguments teardown remain owner/provider
responsibilities. This timer-call pair deliberately supplies no teardown policy.
