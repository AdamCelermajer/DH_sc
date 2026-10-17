# AISExternal construction and Character association

## Implemented caller scope

`ais_external_initialization.hpp/.cpp` reconstructs the complete 108-byte
CharAIScript constructor and 68-byte AISExternal constructor callers (both C1/C2
ABI aliases), plus the valid, nonnull gameplay branch of the 192-byte
CharAIScript::SetCharacter caller. These are two complete constructor callers and
one bounded association caller, not three complete resource implementations.

The adjacent manifest pins 14 original function ranges, their ELF addresses,
sizes, SHA-256 values, three class vtables/address points, and the original path
literal. Original library SHA-256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
All addresses are ELF virtual addresses translated through PT_LOAD segments.

## Constructor order and fields

CharAIScript C2 at `0x3d8fb0` first calls LuaScript C2(bool) at `0x37c674`.
Only after that returns it clears owner `+0x98`, installs the CharAIScript class
dispatch table, clears registry parent `+0xa0` and color byte `+0x9c`, sets right
`+0xa8` to its own registry header, clears current state `+0xb4`, sets left
`+0xa4` to that header, and clears registry count `+0xac`. When `skip_bind` is
false it then calls CharAIScriptBindFunction. A true argument skips that call.
C1 at `0x3d8f44` performs the same sequence.

AISExternal C1 at `0x3dd0e4` and C2 at `0x3dd128` pass the bool unchanged to
CharAIScript C2, then clear `+0xc0`, clear callback flags `+0xb8`, install the
AISExternal dispatch table, and clear counter `+0xbc`, in that order. Constructors
return the captured AIS identity, regardless of the resource callback return
register. The native projections use stable owned identities and supplied native
class tables; they are not overlays of the ARM object and never call ELF vptrs.
Registry left/right point to the projection's own registry header.

Provider writes to fields that the caller subsequently initializes are overwritten.
Writes made by the optional state binding service remain, except for the four
AISExternal stores that follow it. The source does not initialize an active AI
state through this association step: `+0xb4` starts null, and SetCharacter does not
change it. Later `_ChangeState` ownership remains external to this module.

## The real plain-script factory

The existing source selector routes plain `monster` to the external factory.
The original fresh factory branch at `0x3ccaf4` allocates `0xc4` bytes through
the defined four-byte `_Znwj15MemoryHintState` wrapper at `0x310570`, with
MemoryHintState argument zero, writes
`r1=1` at `0x3ccb60`, directly calls AISExternal C1 at `0x3ccb68`, and installs the
captured allocation in pending CharAI `+0x20` at `0x3ccb6c`, after construction.
Therefore this actual selection uses `skip_bind=true`. It does not invoke the
optional constructor state binding service. Replacement/destruction of an older
pending script and the subsequent lifecycle stages are owned by the existing
selector/lifecycle adapter, not reconstructed by this module. The comparison
runner separately executes the original fresh factory branch to verify these
facts; that is not an additional new factory-body implementation claim.

## Association and exact registration names

For nonnull Character, SetCharacter at `0x3d90f8` writes AIS owner `+0x98` first,
then invokes the captured argument Character's virtual `+0xc` with Binder
`AIS+0x10`, then assigns the exact 16-byte string `data/scripts/ai/` to its owned
path storage `AIS+0x68`. The Character vtable address point is full vtable `+8`:
`0x965f38+0xc` resolves to Character::createBindings at `0x3b56bc`. A callback
mutation of owner `+0x98` does not change the captured Character subject or cause
a later owner-store replay. Null Character enters a diagnostic/assert path and
may ultimately dereference null; the port rejects it before effects and does not
claim parity with those invalid-object diagnostics.

The separate original 100-byte CharAIScriptBindFunction caller executes two
function registrations in this exact order:

| Lua name | Original callback | Context |
| --- | --- | --- |
| `RegisterAIState` | `_RegisterState`, `0x3da144` | captured AIS |
| `ChangeAIState` | `_ChangeState`, `0x3d9710` | captured AIS |

Both use Binder `AIS+0x10`, through concrete Binder::bindFunction at `0x31a4d4`.
These are native function registrations, not VFTable alias registrations. The
original 24-byte BindFunction lifecycle caller runs LuaScript::BindFunction first
and then these two additions. The real source VM/backend must retain their
contexts and registrations; recording these operations in a service alone does
not create a working VM.

## Dependency and ownership boundary

Named resource services provide the real LuaScript constructor/resources,
optional CharAIScript binding, Character createBindings, and per-AIS string
assignment. Their 240/100/5332/192-byte full dependency bodies are not implemented
here. The owner must retain the AIS, its actual Binder/string/VM, captured
Character and any retired backing until the synchronous caller finishes. Stable
identities and class-table facts cannot change while it runs. Scalar source
facts may change; subsequent source stores preserve the original overwrite order.
Services are captured at entry. One owning thread is required; same State/output
reentry is forbidden, while independent states and outputs may nest.

Invalid alignment, missing stable identities and overlapping control records
reject before effects. A missing service reports unavailable at the point where
it is needed. Service failures or exceptions stop with earlier source/provider
effects retained; there is no rollback, added initialization, destructor or
cleanup. These are port error boundaries, not claims of original C++ exception
or allocation-failure handling. Resource teardown remains the owning adapter's
policy. This module is not yet wired to the native application.

## Validation

`tests/ais_external_initialization.cpp` checks constructor/association field and
call order, both bool branches, provider field mutation, failure at each resource
phase, exceptions, captured services, independent nested objects, alignment and
alias guards. The runner compiles it with warnings treated as errors and compares
the source module against actual original ARM constructor/association instructions.

The ARM comparison models the four explicitly named resource services. It does
not mock caller branches, field stores or dispatch-table identities. Separate
evidence executes actual CharAIScriptBindFunction instructions to recover names,
callback pointers and context, with only Binder insertion modeled. The factory
evidence executes the fresh original allocation branch and both constructor
callers with allocator and Lua resources modeled. No floating-point imports are
modeled. Reports distinguish host guards, original comparisons and native wiring.
The verified report contains 101 host cases and 90 original ARM comparisons with
zero mismatches.

Run:

```powershell
python port/level-world/tests/run_ais_external_initialization_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so
```
