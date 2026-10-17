# Source AIS native registration callers

The native implementation reconstructs three complete registration callers:
`LuaScript::BindFunction` (1252 bytes),
`CharAIScript::CharAIScriptBindFunction` (100 bytes), and
`CharAIScript::BindFunction` (24 bytes). It implements their call order and
arguments using native provider keys; original ELF function pointers are never
installed into the new process.

The base caller opens **base, math, table, string**, then registers 33 names.
The Character AI caller adds **RegisterAIState, ChangeAIState**, for **35 total**.
An observed source pairing is preserved: the name **OnTargetDied** registers
`LuaScript::_IsPlayerCharacter`, rather than a function inferred from its name.

The original `this+4` is a Lua VM wrapper, not the raw VM. Each original
8-byte library wrapper reads its `+4` raw VM field and tail-calls the
corresponding bundled Lua library opener. The caller captures the script,
wrapper and Binder (`this+0x10`) identities once. Native identities are retained
opaque values and tested above 4 GiB; they are not ARM32 memory overlays.

The maintained differential executes all three callers, including the actual
library wrappers, and compares their events with the compiled native source.
Fresh VM mutation between library opens proves the wrapper resolves the current
VM. ELF hash, all caller/wrapper bytes, strings, target symbols and wrapper
branch destinations are independently checked before running the oracle.

Actual library bodies and Binder registration remain explicit providers. This
unit does **not** reconstruct the bodies of the 35 registered callbacks or claim
that live Ghosts have initialized. The runtime must install real implementations
for supported callbacks and return explicit errors for unavailable operations.
Host failure tests verify retained earlier effects and stopping at the missing
provider; those failure statuses are native adapter contracts, not new original
ARM branches.
