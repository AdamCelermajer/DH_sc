# Native Character FSM producer trace (read-only)

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
No production module, owner, CMake, renderer or APK was changed for this trace.
The new captured manifests/assembly and original probe are in this directory.

The exact cache monster/rene/commons scripts contain no CharAIScript
RegisterAIState/ChangeAIState calls. Their GetState queries refer to the native
Character/CharStateMachine, not the separate Lua-named state registry. Source
monster compares native Idle/Move constants; rene reads native Move for its
movement decisions and supplies an OnUpdate alias. Native gameplay providers
and remaining FSM state bodies must be recovered rather than manufacturing
script state callbacks.

## Script-facing getters and clock

- `Character::_GetState` **0x3b6d78** ignores argument count, uses the machine
  embedded at Character+4fc, calls `SM_GetState` **0x3c01ac**, then
  `ReturnValues::pushInteger(int)` **0x37cb24**. `SM_GetState` reads machine+20:
  null current returns -1; otherwise reads StateInfo's first signed ID.
- `Character::_GetStateTime` **0x3b6d6c** ignores arguments and directly pushes
  Character+55c (machine+60) through that same signed integer function. High-bit
  elapsed words become negative signed integers, then float32 Lua numbers.
  Value's integer constructor **0x37ca9c** calls signed `__aeabi_i2f` at
  **0x30e964**. A provider must bit-preserve the elapsed word into int32 before
  conversion, not convert uint32 directly to float.
- Existing bounded `State.current` and `State.elapsed_ms` correspond to these
  fields. `ScriptStates.runtime.current` is unrelated and is not a substitute.

Full `CharStateMachine::Update` **0x3c628c** calls profiling begin, captures old
elapsed+60, obtains raw application GetDt **0x31f66c**, and stores wrapping
32-bit old_elapsed+dt **before any mask enforcement or current OnUpdate**.
There is no dt clamp in this routine. The existing bounded
`dh2_character_state_update` already adds dt; a later full prelude composition
must dispatch that bounded current-state tail with dt0 after this increment,
rather than advancing elapsed twice. This is an integration proposal, not a
production change or complete frame oracle.

## Pending effect enforcement and reloads

Machine+2c is Character+528 (the existing attack_gate/mask field). If bit2 is
set, Update calls `SM_IsStunned(false)` **0x3c0378**, which compares actual
current ID9. If not9 it requests `SM_SetStunState(UINT_MAX, (machine+24>>11)&1,
null, false)` at **0x3c6340**. It then **reloads** mask+2c before checking bit4.
Scare checks `SM_IsScared(false)` **0x3c034c**, actual current ID8, then requests
`SM_SetScareState(UINT_MAX,(machine+24>>10)&1,null,false)` at **0x3c637c**.
Both pending masks can therefore dispatch in order. The current StateInfo is
reloaded afterward, and a nonnull state invokes its actual virtual+14 OnUpdate
with (state ID, Character, machine). Profile end follows.

SetStun **0x3c5ffc** and SetScare **0x3c6144** are substantive: Boss rejection,
animation-table ID range, authored animation selection, timers/mask setting,
event/direct transition and corresponding policy bits. They are captured but
not reconstructed by this read-only probe. Replacing them with a state-ID
assignment would omit these source behaviors. The probe explicitly supplies
them as state-mutation service fixtures to establish surrounding order/reloads;
it does not call that a complete native stun/scare backend.

## Registration and preset state

`RegisterState` **0x3c7318** checks the owned integer-keyed map. An existing
entry is retained. A missing ID is searched in exactly20 source table pairs at
**0x9666c0**; unknown IDs are not registered with a fallback. For a match the
source obtains a StateInfo map entry, writes its ID, allocates its exact state
class, writes behavior, then calls that behavior's virtual+8 OnInit. OnInit
registers actual event/predicate transitions. `state-factory-table.json` binds
the table's bytes and resolves its actual factory symbols:

|ID|Class|ID|Class|
|---|---|---|---|
|0|Limbus|10|KnockedBack|
|1|Spawn|11|Injured|
|2|Despawn|12|Dead|
|3|Idle|13|Interact|
|4|Move|14|Anim|
|5|Attack|15|Reviving|
|6|Skill|16|Revived|
|7|Cast|17|PreSpawn|
|8|Scared|18|LiftingIdle|
|9|Stunned|19|LiftingMove|

`GetPreSetAIState` **0x3a5784** reads an owned string at Character+13cc, with
end/begin at+13dc/+13e0, comparing through original std::string.compare
**0x3a5720**. Empty/other string gives Idle3, exact `Limbus` gives0, exact
`PreSpawn` gives17. Case and full equality matter. The original loading producer
of that string has not been recovered here; do not publish fabricated authored
preset values. `SetCharacter` **0x3c1600** writes the borrowed owner pointer at
machine+4, with original assertion behavior for null; it does not itself select
one of these preset states.

`_SetState` **0x3c1938** and `RaiseStateEvent` **0x3c5684** match the existing
bounded four-state notes: captured previous ID, outgoing Blur, requested-state
lookup, assignment, elapsed reset only for a different ID, incoming Focus,
then Character event1d with previous ID. RaiseStateEvent calls current OnEvent,
then reloads current before registered-event/predicate lookup and transition.
The existing four-state coordinator is not a full20-state registration or event
backend. Source monster/rene skill/buff/timer behavior can reach additional
states, so those omissions remain explicit.

## Executed evidence and next adapter boundaries

`probe.py` / `native-fsm-probe.json` PASS actual original instructions:
60 getter cases including high-bit elapsed, 1,536 full Update prelude cases
covering both masks/priority/current reload/wrapping dt, six owned preset strings.
Explicit services are profile hooks, raw engine dt, ReturnValues integer push,
substantive stun/scare calls, current-state virtual body and imported libc
string comparison primitives. Actual state predicates and surrounding wrapper
instructions execute. No native/ARM64 implementation or full VM/frame parity
is claimed by this read-only trace.

Minimal next interfaces should bind GetState/GetStateTime to the actual borrowed
Character state and supply a full Update prelude with genuine state/effect
services, retaining original dt/current reloads. Existing source four-state
handlers can be used only for their supported IDs. Real source namespace
providers still need GetPosition/master/target/path, animation table and
Character gameplay commands/skills/buffs. Native CharAIScript registry source
recovery does not satisfy these separate gameplay producers.
