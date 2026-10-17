# Original external monster Lua callback session

This bounded session executes the unchanged recovered original AI scripts with
the existing source-built Adam Lua VM and source VFTable alias implementation.
It makes two original monster callback bodies executable through borrowed actor
services. It does not implement a complete AI, native candidate search, or the
whole AISExternal lifecycle.

## Actual data and selection

Crypt_Ghost character property rows 35/37 resolve AI row 68, named Ugly_Dog,
whose Type is 4 and Script is exactly `monster`. The existing selector sends
plain `monster` to AISExternal; only the builtin `__monster__` spelling selects
AISMonster. Selection is recorded separately in the existing
`character-script-selection` evidence. This session therefore uses the original
Lua callbacks and adds no native AISMonster policy.

The preserved original scripts are:

| Input | Bytes | SHA-256 |
| --- | ---: | --- |
| `recovered/scripts/original/data/scripts/ai/_commons.luac` | 13,535 | `20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c` |
| `recovered/scripts/original/data/scripts/ai/monster.luac` | 7,393 | `84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d` |

Despite the extension, these inputs are readable source text. They remain
unchanged in their original recovered location; the fixture reads and pins
their exact bytes. Original game scripts remain attributed to the original
game's authors (Gameloft). The source VM reuses Lua 5.1.4 with its retained MIT
license at `port/adam-script-runtime/vendor/lua-5.1.4/COPYRIGHT`; Adam's recovered
VM/alias work is reused directly. The identity transport, session ownership and
host fixtures are new port code, not original script edits.

## Source callbacks and fresh reads

Load order is `_commons`, then `monster`. The real source `AddToVFTable` producer
registers all original aliases into the session-owned map. Only the two events
below are dispatchable by this bounded native API. `source_alias(Event)` exposes
their map-owned names for lifecycle/event adapters; its text is borrowed until
successful reinitialization or reset.

| Native event | Original source alias | Exact original behavior |
| --- | --- | --- |
| enemy_spotted | `OnEnemySpotted` → `monster_OnEnemySpotted` | Read `HasTarget()`. Only when false, call `SetTarget(enemy)` then `HeadTo(enemy)`. The original enemy parameter is retained across target mutation; it is not replaced by a later target lookup. |
| target_out_of_range | `OnTargetOutOfRange` → `monster_OnTargetOutOfRange` | Read `GetState()`, then `GetPyCst("AIStates","Idle")`. Only if equal, read `HasPath()`. Only if false, read the current `GetTarget()` and call `MoveTo` with that result. No target/path cache or candidate search is added. |

Initialization performs the script's existing top-level
`FromFixed(GetProp(GetPyStruct("CharacterProperties","SkillTree")))` expression.
Named lookup/property providers remain borrowed services; the fixture uses the
original SkillTree field ID 28 and an authored property value. FromFixed reuses
the existing source numeric leaf, including its two return values. The session
does not call `monster_OnInit`: dynamic monster level, buffs, position, skill
tree consumers, combat, timers and other registered callbacks remain outside
this source slice and cannot be dispatched through its Event API.

The original Character GetTarget/HasTarget bridges read target at `+0x408`.
GetState reads the state machine. The source SetTarget/HeadTo/MoveTo object
overloads accept sfc Value type 2 or 7; their numeric-coordinate overloads are
outside this adapter. The complete native actor action bodies are delegated,
not claimed reconstructed by this session.

## Identity transport and lifetime

The original AISExternal enemy callback passes an object table to Lua. Source
native argument projection reads that table's `_this` field and labels it type
7. The new transport helper supplies neutral `{_this=lightuserdata}` tables
for the event parameter and for nonnull GetTarget results. Native bindings
therefore receive type 7 and the complete 64-bit identity, without converting
a pointer to a Lua number. The host gate counts the actual projected type 7
arguments in SetTarget, HeadTo and MoveTo. These neutral tables provide identity
transport only; original entity methods and metatables are not fabricated.

Null GetTarget becomes Lua nil. This follows original Value::_pushOnStack's
nonnull test at `0x31cb20`, whose null branch reaches pushnil at `0x31cc24`.
`MoveTo(nil)` takes the original non-object no-op overload; it does not call a
native movement service with an invented target. Enemy dispatch requires a
valid nonzero externally retained actor identity as a bounded port contract.

Each session owns its VM and alias map. Binding records and a copy of Services
remain at stable addresses. The VM is closed before the alias map is destroyed,
so closures never outlive borrowed registration state. Service context, actor
owners and target lifetimes remain external, including any target stored by a
native SetTarget service. The caller keeps them alive through reset/destruction
and must not destroy the session during a callback. One owning thread is
required; initialize, dispatch and reset reject synchronous reentry.

## Errors, unsupported scope and tests

Unknown events fail before entering Lua. Missing used services and invalid
adapter bools become protected Lua errors. Native exceptions are caught inside
the C++ service adapter before returning through the C VM trampoline. Prior
effects, such as an already changed target, remain; there is no rollback.
A failed callback faults the session and subsequent dispatch is rejected until
explicit initialization. A failed candidate initialization destroys only its
own VM/map and preserves an earlier session, while retaining provider effects.

The reused VM enforces an allocation cap. Source inputs are limited to 128 KiB
each and bytecode is rejected by this session. There is no instruction cap or
claim of a sandbox for untrusted mods. No original ARM VM execution or whole
historical interpreter equivalence is established here.

The 29 host cases execute the exact original scripts for gameplay checks and
use clearly authored malformed/memory-stress inputs for error cases. They
verify ordered actions, both short-circuit gates, fresh target mutation,
original table projection, null target handling, service-table copy, independent
VM/map teardown, reentry rejection, native exception/failure boundaries,
actual Lua allocation failure, failed candidate preservation and recovery, and
the zero-result arity of unsupported `BitAnd` forms alongside valid bitwise
calls.

## Staged pending-AIS VM

The external AIS path can stage one VM in the source lifecycle order:
`Session::create`, `bind_functions`, `load_common`, `load_external`. The VM is
created with deferred libraries. `ais_native_bindings::bind_all` then opens
base, math, table and string in the recovered order and registers the complete
35-name table (33 base names followed by `RegisterAIState` and
`ChangeAIState`). The `OnTargetDied` name retains its observed binding to
`_IsPlayerCharacter`. This preserves registration behavior without claiming
all registered native function bodies are reconstructed.

The session has real closures for the recovered numeric helpers, VFTable
operations, `GetPyCst` and `GetPyStruct`; its eight additional property/target/
controller globals are borrowed port adapters used by the bounded monster
callbacks. Registered base APIs without a recovered implementation are bound
to closures that raise a Lua error when called. They do not return fabricated
success values. A stage failure faults that pending VM and prevents retry or
fallback; the older `initialize()` helper remains an atomic convenience path.

`ghost_ai_session::ActorSession::prepare_staged` prepares the stable service
adapter, returns its copied callback table and a shared lifetime lease, and
`adopt_staged` checks `Session::uses_services` before accepting the already
loaded pending VM. This lets the GhostOwner callbacks and the promoted AIS use
the same Lua state. The lease retains the adapter context; actor/world
projections remain borrowed and must outlive VM use. The host composition test
verifies search, event, original Lua, SetTarget and PathTo through the adopted
VM, then detaches the wrapper and dispatches again through that same VM.

The Session host gate now reports 28 cases; the actor composition gate reports
9. The separate deferred-runtime check reports 464 assertions and verifies
that each standard library is absent until its matching ordered open call.
These are host validations, not Android AI-pursuit or complete AIS parity
claims.

```powershell
python port/level-world/tests/run_monster_external_script_session_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe
```

Native Character ownership, the existing CharAI OnEnemySpotted owner/group
gates, event 9 candidate production, controller/path actions and Android
lifecycle integration remain pending. This module adds one bounded session
adapter and executes two original Lua callbacks; the supporting original C++
ranges are evidence, not full-body source implementation counts.

The host gate links only `port/adam-script-runtime/lua`. The separate existing
`port/lua-runtime` core exports the same `lua_*` symbols and uses a different
Lua integer configuration (int32 versus Adam's ptrdiff_t). Both raw cores must
not be linked into the same production image. Future native integration must
select one compatible shared core/header configuration for both wrappers or
explicitly isolate the engines. This task makes no CMake/Android build changes.
