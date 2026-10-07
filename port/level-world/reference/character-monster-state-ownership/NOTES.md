# Native per-character StateInfo ownership and frame entry

Original ELF: `.local-inputs/libDungeonHunter2.so`, SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Function manifests and assembly in this directory identify exact source bytes.
This closes state/event **storage and dispatch** ownership. It does not implement
all twenty behavior bodies, full target perception, or the complete monster AI.

## Construction and shared behavior identity

Character C2 `0x3a9340` calls FSM SetCharacter `0x3c1600` at `0x3a97fc`,
then RegisterState `0x3c7318` for IDs0..19 at `0x3a9820`. C1 `0x3aa1b4`
does the same at `0x3aa670/0x3aa694`. The embedded source machine starts at
Character+0x4fc. FSM ctor `0x3c1ac4/0x3c1b58` initializes current+0x20 null,
elapsed+0x60 zero, masks+0x2c zero and the map header. Registering states does
not select a current state.

The factory table at `0x9666c0` has exactly20 signed-ID/function pairs. Each
GetNewState<T> factory returns the PIC address of Singleton<T>::s_inst;
it does **not** allocate a per-character state behavior. For example Idle's
factory `0x3c04a8` returns `0x9a2aa8`; Move `0x3c04c4` returns `0x9a2ab0`;
Attack `0x3c04e0` returns `0x9a2ab8`; Dead `0x3c05a4` returns `0x9a2af0`.
The static initializer capture records the original vtable construction. These
addresses are provenance keys in the native module, never ARM32 function or
object pointers cast into the native process. This corrects earlier
`character-native-fsm/NOTES.md` wording that called the factories allocations.

RegisterState searches the owned map, ignores an already-present ID, scans
those20 factory pairs and ignores an unknown ID. It obtains StateInfo through
STL operator[] `0x3c71e0`, stores ID and shared behavior, then calls its OnInit.
All20 OnInit bodies register109 events in the source call order recorded in
`registration-plan.json`. Idle OnInit also clears the byte at Character+0x538
(machine+0x3c, the idle-suppression projection). SM_RegisterEvent `0x3c7b18`
ignores a missing state, creates/replaces the event's member-pointer words and
target, using signed ordered keys. No shared per-character mutable event map.

`registration-probe.json` executes original ctor, SetCharacter, RegisterState,
all factories, OnInit and SM_RegisterEvent:20 states,109 ordered events,
129 explicitly serviced STL insertions,24 duplicate/unknown no-ops, current
still null. STL insertion/allocation, initialized singleton vtables and logger
string construction are declared services; source searches and field stores
execute. The original static initializer is captured, not fully emulated.

## Current-state producer and synchronous transition

Level._LoadCharStates `0x3eff98` traverses ObjectManager's actual Character list.
It queries GetPreSetAIState `0x3a5784`; preset0 or17 selects PreSpawn17 via
SM_SetPreSpawnState `0x3c1a64`. Every other preset selects Idle3 via
SM_SetIdleState(false) `0x3c1a00`, first clearing machine+0x3c. The Level
load-process call is at `0x3f7700`, followed by load-stage increment at3f7704.
The preset getter recognizes exact owned strings Limbus→0, PreSpawn→17, other
or empty→3. The upstream authored-string population remains external; this
module accepts its resolved integer and does not invent a Crypt preset.

_SetState `0x3c1938` captures previous and requested IDs, invokes outgoing
OnBlur **before** lookup, then reloads owner. Missing next sets current null,
keeps elapsed, and still raises Character event0x1d with previous ID. Existing
next sets current, clears elapsed only when previous!=requested, invokes incoming
OnFocus, then raises0x1d. Same-state transitions still blur/focus and notify.
Previous/requested survive callback changes to current or owner.

Do not assume initial Idle focus always occurs with active AIS null.
Character.InitPost has an InitScriptProcess `0x3ce7c0` branch at `0x3b54c8`.
CharAI.OnStateChanged `0x3d0bec` reads active+0x1c: null skips, nonnull calls
active AIS virtual+0x20. This is the genuine source gate for event0x1d; the
parent must preserve the actual loading phase/active identity.

RaiseStateEvent `0x3c5684` clears mask1/2/4 first for2a/2b/2c, pins a present
physical body on30 when current is Idle3, Interact13 or LiftingIdle18, invokes
current OnEvent, reloads current, checks its event map, performs logger services,
reloads again, captures target, and invokes a nonnull member predicate. The
predicate can replace target through its int& argument. Accepted events call
_SetState with that captured/rewritten target. Returning0 from the native event
kernel means no registry transition; delivered OnEvent effects still occurred.

Original _GetEvent `0x3c1694` is const and does **not** insert a missing record.
If callbacks invalidate current or the later lookup, original code can access
null/sentinel data. Native returns-3 explicitly. A required-service error returns
-2 at the delivered prefix, with no invented rollback. Native input guards are
additional validation, not claimed original defensive behavior.

## Native integration API

`CharacterStateOwner` in `character_state_owner.hpp/.cpp` is noncopy/nonmove and
owns State, NativeFsm24 and twenty independent event maps. Keep it alive for
all bound VM/FSM/timer callers. `state()`, `native_fsm()`, `machine()` provide
stable borrowed views. Populate external Character projections and physical
identity explicitly. `initialize_level(preset,services)` applies the source
Level selection. `transition` and `event` own nullable current selection;
do not set State.current directly in gameplay wiring.

StateOwnerServices16 synchronously delivers blur/focus/event/predicate,
Character.RaiseEvent, pin and profiling requests with exact source-method keys.
Its callback must bind a real backend, or fail visibly. Metadata is not a
behavior implementation. Parent's genuine four-state focus/blur-only exports
can deliver IDs3/4/5/12 without repeating outer transition/notification.
Remaining virtual bodies and CSM_Spawn are explicit required boundaries.
Callbacks may reenter; owner/services/borrowed facts must survive the call.

Seven pure predicates are also native, keyed by original function address:

- Attack3ad22c: accept iff mask528 bit0 is clear.
- Injured3ad23c: accept1.
- Interrupted3ad244: ID6 flags520 bit16; ID10 inverted mask528 bit5;
  ID5 raw can-interrupt byte441; other IDs1.
- StoppedAttacking3ad280: ID5 raw byte442; others0.
- StopSkill3ad290: flags520 bit15.
- CanStopInteraction3ad29c: interaction544=4 or5 returns0; others1.
  Actual two-byte table at8c3784 is00,00.
- AfterInteraction3ad2c8: accept1; interaction4 writes next18.

PredicateFacts16 retains genuine live flags/mask/interaction and byte values.
CSM_Spawn3ad2e4 remains-2: current0 tailcalls Character.CanSpawn3a5248;
current17 queries zone3d24fc when present then byte1430; other IDs accept1.
Do not turn the unresolved branches into a generic accepted spawn.

## After Init: exact frame and target-event boundaries

Existing source frame captures show scene/timeline/root updates before Level
physics Step, then Character.Update3abe98: optional TimerUtil, Character timers,
CharAI.Update, FSM.Update, CharAnimator.Update, GameObject.UpdatePath. Retain
this order; Initialize success does not justify directly running Lua OnUpdate
or a guessed FSM state. The original Lua named-state registry is separate from
native StateInfo; actual monster/rene scripts contain no RegisterAIState calls.

CharAI.Update3cfbf4 already has the recovered pause/global/lock/forced gates and
ordered target→master→aggro→virtual OnUpdate services. _UpdateTarget3cb908
in this directory adds the necessary producer evidence: source machine policy
gates3c0230/3c01c0, live target40, target virtual88 validity and34 dead state,
alive-history48 and visibility-history49, visibility helper3d4ed8, owner ranged
weapon v124, range helpers3d63d8/3d6604 or melee3d6188. It reloads target and
owner around event callbacks. It emits alive transitions0xa/0xb and visibility
transitions0xc/0xd; failed target virtual88 clears target40 and44 before0xc.
When currently visible, range notifications are emitted from the live helpers;
do not substitute a one-time threshold crossing based on cached positions.

Character.RaiseEvent→CharAI.RaiseAIEvent3cbb34 remains the route. Event9 invokes
EnemySpotted virtual34 before reloading owner/FSM and forwarding. Events0xa..19
invoke their selected AI notification virtuals and terminate there; they are
not direct FSM commands. Events0/1 map to state events0xc351/0xc352. Event1d
queries the live FSM before the source active-AIS OnStateChanged gate. Regen33,
DoT34 and BuffExpired36 stay genuine timer/property-owner paths; the state owner
supplies the live nullable FSM getter required by regen. No duplicated buff
registry or elapsed-based effect synthesis here.

Minimal next integration: retain this owner per Character before script/facts
bindings; perform genuine Level preset selection at its loading phase; route
state commands/events through its registry; bind the bounded behavior/predicate
backends; compose existing AI frame and event modules with required target,
master and aggro producers. Full perception, unimplemented state behavior bodies,
application load/save phases and complete monster gameplay remain unclaimed.

## Evidence and reproduction

`reports/character-state-owner-arm64-differential.json`:1152 actual original↔O2
ARM64 cases,3703 exact ordered callback requests,109 registration comparisons.
`reports/character-state-owner-predicates-arm64-differential.json`:2016 actual
original↔O2 cases covering all seven pure predicates, including nonboolean
bytes, signed interaction IDs, bit boundaries and int& target writes.
`reports/character-state-owner-host-audit.json`:same1152+2016 gold cases,
independent maps for two owners,31 ownership/guard checks, one actual nested
native transition; ASan/UBSan/leak detection zero diagnostics. The nested native
transition check is additional host evidence, not a new original recursive
instruction-parity claim. Pointers/context identities exceed4GiB.

Host reproduction (isolated source, no central DSO rebuild):
`python port/level-world/tests/character_state_owner_host.py --output NEW.json`.
CMake audit invocation can use `tests/character_state_owner.cpp` linked with
actual world target; argv are `reference/character-monster-state-ownership/`
`state-owner-fixtures.bin` and `predicate-fixtures.bin` (two full paths).
Proofs bind source, original ELF, compiled executable/ARM64 ELF and golden bytes.
No APK/device/live/full-AI claims arise from these isolated tests.
