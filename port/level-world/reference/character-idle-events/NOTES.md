# Retained original monster Init and owned Idle event composition

`character_idle_events.hpp/.cpp` is an additive adapter over the existing
source-derived kernels and private VM. It borrows one CharacterStateOwner,
one CharacterAnimationInstance, the caller's shared AnimationRandom, immutable
AnimationTables, live Facts, AIEventState64 and AnimationAIState96. It creates
no second FSM, controller, navigation result, physical body or frame clock.

## Original instructions and selected receivers

The original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json` and `reference/original-functions.asm` capture 23
complete routines. `vtables.json` captures the exact first 51 callable source
keys from CharAI `_ZTV6CharAI` at `0x966790` and AISExternal `_ZTV11AISExternal`
at `0x966a48`. The matching static arrays in `character_idle_event_tables.inc`
are widened provenance identities, never callable native pointers.

Character::RaiseEvent `0x3a4d5c` branches to its embedded CharAI dispatcher
`0x3cbb34` for all events except `0x36`, which targets the separate property
producer. The adapter uses the existing complete `dh2_character_ai_event`.
Event `0x1d` captures the AI virtual+20 key, queries the current state from the
captured owner's FSM, then passes new/current and previous signed words to
CharAI::OnStateChanged `0x3d0bec`. This captures active AIS and invokes +20.
Selected AISExternal's actual key is AISDefault::OnStateChanged `0x3dbe8c`,
a genuine `bx lr` body, handled by the frozen native helper.

Animation events `24/26/25/27/23/22` here are hexadecimal. Sequence begin/end
helpers `0x3d3d4c`/`0x3d3d30` query the current owner/FSM and return1. Step begin
`0x3d4204` and step end `0x3d3ff8` have real Move/Attack/Skill branches; actual
Idle3 is delivered through the existing native animation-AI consumer. Those
nonempty other branches require a supplied provider in this adapter.

Events `0x22`/`0x23` call AI virtual+98 before forwarding to the reloaded FSM,
even under controller/global event gates. CharAI::OnEndOfAnim `0x3d0ce8`
captures active AIS and invokes +98. Selected External's key is `0x3dccd0`,
which calls the Default empty body then unconditionally calls the private
LuaScript `OnEndOfAnim` with no arguments. It has no InitVCB availability gate.
The native adapter calls the frozen retained-owner/private-VM helper using the
captured script identity; alias and global function lookup remain fresh.

Idle3 registers decimal34 and35 (`0x22`,`0x23`) to Idle3. Therefore forwarding
either event runs real FSM diagnostics, IdleBlur, IdleFocus and a new
Character event1d. Selection can synchronously emit24/26, or queue through
the source animator's pending flag during actual animator completion. The
existing BlendedPlayback owns that queue/choreography. Nothing here implements
a second event cursor or recalculates completion overshoot. With gates blocked,
the newly generated1d takes the full router's direct-FSM path instead of the
StateChanged virtual path.

## Restored source diagnostics and historical naming caveat

Frozen StateOwner operation names `state_owner_profile_begin/end` (7/8) are
compatibility names. Their actual source keys are DebugSwitches::load
`0x337888` and GNU string destructor `0x318254`, not profiler functions.
RaiseStateEvent `0x3c5768..0x3c57a0` calls load, string constructor `0x3140ec`,
Debug query `0x337a88`, then destructor. Its literal is `isTracingCharSM`
at `0x8c4f38`; the query result is discarded before the event-map reload.
The earlier StateOwner oracle hid constructor/query as explicit services,
so its historical proof does not establish the complete diagnostic prefix.

The NEW `StateOwnerDebugDiagnostics` borrows the world's real retained
DebugSwitches/file providers and owns stable deque-backed temporary strings
between legacy7/8. It invokes genuine native load/get and destroys the owned
temporary at8. Unknown/misordered keys fail; existing-file parsing remains
unsupported by the existing debug loader, so it closes the handle and returns
`-3`, without turning failure into a false switch or successful FSM event.

There is also a diagnostic prefix in IdleFocus `0x3c3050..0x3c307c`, BEFORE
reading suppression byte538 or writing flags520, and in IdleBlur
`0x3c2d6c..0x3c2d98`, BEFORE clearing that byte. Both query
`isTracingCharState` at `0x8c4e58`. The new state-service wrapper executes these
complete owned diagnostic prefixes before the existing bounded behavior
kernel. Idle OnEvent `0x3c0004` itself is genuinely `bx lr`. This leaves the
frozen behavior source/reports unchanged and explicitly closes their hidden
diagnostic boundary for this narrow composition. GNU ARM32 allocation/COW
pointer identity is not claimed by the port's owned C++ strings.

## Caller contract and phase order

1. Keep a retained CharacterStateOwner and CPU AnimationInstance at stable
   addresses. Supply live authored Idle Facts, coherent source owner/controller/
   properties projections and actual CharAI source table keys. Gate/stance/AI
   constructor ownership is a caller producer, not inferred from animation.
2. Construct CharacterIdleEvents with caller-owned shared RNG and the world's
   StateOwnerDebugDiagnostics. `bind_input` installs the exact NativeFsm in
   CharacterScriptSessionInput BEFORE session creation/private VM startup.
   Do not replace that input's FSM binding afterward.
3. Create and attach the real CharacterScriptSession, then execute its original
   common+monster `start`/Init. Only afterward call `publish_external`, which
   verifies successful External publication and binds its actual private AIS
   identity/source table. `initialize_idle` calls owned LevelLoadStates preset3.
   This is the chosen Idle3 path; preset/Spawn17 selection is outside this class.
4. Drive `scene_phase(source_timestamp)` at the genuine shared scene phase.
   At the separate actor animator phase call `animator_phase()`. The existing
   instance uses the applicator's captured completion.extra_ms. This adapter
   advances neither FSM elapsed time nor Application dt; the future frame owner
   must deliver the genuine dt exactly once using its native frame kernel.
5. Stop driving before destruction. Close private VMs while their FSM/providers
   and property/design records remain alive, then detach/destroy the event
   adapter before the AnimationInstance. Shared immutable resource owners stay
   pinned by each instance; GL object lifetime has no authority over CPU state.

Callbacks are synchronous. An explicitly supplied live same-private-VM scope
is propagated through nested native events and synchronous selection observers;
generic VM reentry is not exposed. A void animation observer/body cannot abort
all work already being performed by its caller: provider failure latches the
first error, is returned after the enclosing operation, and preserves delivered
prefixes. Callers must stop driving the adapter afterward. No rollback/recovery
or successful unknown nonempty service is fabricated.

## Sanitized actual-input audit and precise scope

`tests/character_idle_events.cpp` reuses the frozen genuine Session fixture's
input readers without changing it. It uses the original GDO1 decoded design
inputs/constants, actual Crypt DACT eleven actors, real commons and monster
scripts, actual source animation tables and all four staged original banks.
It creates eleven real sessions, observes the same NativeFSM as -1 before
LevelLoadStates and3 afterward, and keeps a single borrowed RNG across actors.
Each CPU instance loads hash-verified original resources; immutable external
owners are released while retained sessions/instances continue running.

The audit runs 2,420 separate scene/animator phase pairs, 362 generated source
animation notifications, 950 full-router service deliveries, 68 genuine
commons OnEndOfAnim callbacks, and 215 genuine diagnostic queries during the
authored initialization/animation baseline. Every actor remains Idle3 with
flags2380, finite node matrices and untouched FSM elapsed time. One actor's
pose/clock does not advance while another is driven. Additional diagnostic Lua
globals AFTER the original baseline prove end-before-FSM order, exact recursive
Idle reselection and protected same-VM dispatch, including blocked/forced gates.
An explicit suppressed-Focus projection proves diagnostics still run before
suppression prevents flags/selection. Eight failure checks cover unknown AIS,
post-failure driving, missing pin, unresolved authored28, property36, incoherent
FSM input, destructor ordering and real existing-file rejection.

`reports/character-idle-events-host-audit.json` binds the new compiler inputs,
actual inputs, captured manifest/tables, executable, transitive private DSO
snapshot and ldd result. ASan/UBSan/LeakSanitizer report zero findings. The
snapshot is stable56 world SHA
`6ecf806974095c9ed4b3620483e2714cceb50d1a94b34f80d3ecffa513efc0a8`;
it was copied and verified before the parent's subsequent rebuild. Existing
DSO compiler provenance is not newly asserted by this isolated test.

Underlying kernels have separate original/O2 proofs: complete AI dispatcher
11,344 comparisons, StateOwner1,152, bounded behavior3,910, animation-AI2,197
consumer comparisons, StateChanged/End1,578, and Debug126 operations. The new
whole VM/resource/FSM composition is **not** a new complete original-instruction
differential. Host/player/session/difficulty projections and missing save-file
delivery are explicitly borrowed fixture services. No full Application startup,
non-Idle AI, AI_OnUpdate, AI timers, target/range/sight, navigation, physics Step,
root-to-body motion, GL rendering or complete original-frame parity is claimed.

Reproduce the isolated proof with the supplied Windows Python:

```
python port/level-world/tests/character_idle_events_host.py --reuse-snapshot
```

The native test target can be named `character_idle_events_audit` and linked
to the existing world/runtime/data/animation/scene libraries. CLI arguments:
GDO1 corpus, original commons, original monster, Crypt DACT, staged342-assets
root, original animation-table asset directory. No shared CMake or APK changed
as part of this task.
