# Spawn delay selection and PreSpawn permission

Original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json`/`reference/original-functions.asm` bind the exact
SM_SetSpawnState `0x3c2734` (220B), Random clone `0x3c26a0` (148B), TMR_Start
`0x3dbe24`, _SetState `0x3c1938`, CSM_Spawn `0x3ad2e4`, Character constructor
`0x3aa1b4`, and DeclareProperties `0x3a9fe4`.

## The fields are delay bounds, not counts

Character+`0x1434` and+`0x1438` are signed integer `spawn_delay` bounds.
`delay-producer-probe.json` executes the original DeclareProperties routine with
explicit inherited-property, allocator, string-constructor and registration
services. It observes the exact name `spawn_delay`, destination+`0x1434`, and
the actual descriptor vtable `0x965eb0`,
`SimpleTypeProperty<Point2D<int>>`. The adjacent source property `auto_spawn`
registers at+`0x1430`; `spawn_view_radius` registers at+`0x143c` as float.
This is original descriptor/name/address evidence, not a full property manager
or text-parser reconstruction.

The source constructor establishes r8=0 at `0x3aa240`; the actual store block
`0x3aa414..0x3aa420` initializes both delay words to0. Four original store-block
probes verify this. The rest of the constructor remains separately owned.
Supply actual decoded integer values when a caller loads authored properties;
do not invent a delay from movement speed, spawn count, animation duration or dt.
The words pass directly to the millisecond TMR_Start argument without scaling.

## SM_SetSpawnState behavior

The first bool is truthy at the instruction level. The second bool is unused;
even its high bits cannot affect any branch in this routine.

1. First bool false: leave delay fields and shared RNG unchanged; tailcall
   _SetState(`1`, `-1`, NULL). It does not directly assign current state.
2. True: clamp minimum to0 if negative, then raise maximum to minimum if lower.
   Source writes the minimum correction before the maximum correction.
3. Equal zero bounds: tailcall the same _SetState(`1`, `-1`, NULL).
4. Equal positive bounds: TMR_Start on Character+`0x3b4`, duration=the bound,
   repeat0, event`0x2d`, referenceNULL. No RNG call.
5. Unequal bounds: execute Random(maximum-minimum), add the captured minimum,
   reload the Character receiver from machine+4, then start the same timer.
   The normalized range is positive and no larger than INT_MAX. The maximum
   endpoint is excluded by the source random remainder.

**Unequal [0,N] bounds always create a timer, even when its selected duration is
zero.** They do not select Spawn1 immediately. The genuine TimerStore updater
does not expire a duration-zero timer. Timer return values, including UINT_MAX,
are ignored by the original caller; a native provider/storage failure is still
an explicit failed service rather than a successful missing effect.

The Random clone consumes the shared source seed, calculates wrapping unsigned
`seed*59051+177149`, reduces modulo14348907, takes the signed remainder against
the positive bound, and increments the wrapping debug-call counter. Native
selection reuses `dh2_combat_random` for precisely this nonnegative-bound domain.
The original instruction proof executes all original multiplication, unsigned
reduction, stores and counter arithmetic; only the compiler signed
quotient/remainder helper is an exact service. This proof does not broaden the
existing combat random helper's contract to negative bounds or reconstruct the
Application seed/initialization producer. Borrow the real shared RNG projection,
not a new seed per monster.

## APIs and owner integration

`character_spawn_select.hpp/.cpp` defines NativeSpawn24:
`{NativeFsm24* fsm, CombatRandom* random, int32 minimum_ms, maximum_ms}` and
exports `dh2_character_spawn_select(view, delay_enabled, ignored_mode, services)`.
SpawnSelectServices16 synchronously delivers a SpawnSelectRequest32 with full
64-bit Character identity and null payload. Service0 is actual _SetState:
arguments state1,eventUINT_MAX,0. Service1 is actual TMR_Start: arguments
duration,repeat0,event2d. The response word is ignored. Return1 completes,
`-1` rejects malformed inputs before effects, and `-2` stops on a required
provider failure at its delivered prefix without rollback. Keep borrowed FSM,
field projection, RNG and providers alive across reentry.

The frozen PreSpawn OnEvent9's `pre_spawn_set_spawn` delivery may call this
adapter with true,false. Route service0 through the genuine owned
StateInfo transition, preserving outgoing Blur, incoming Focus and Character
event1d. Route service1 through the genuine Character TimerStore with caller
expiry services. A later source event2d must go through actual Character/CharAI
delivery and the owned StateInfo event registry; PreSpawn17 has the recovered
event2d→Spawn1 registration with no predicate. Do not manufacture a transition
when a timer is created, skip current-state event routing at expiry, or restart
the animation directly. Repeated calls can create multiple timers exactly as
the source does; there is no duplicate-timer suppression in this routine.

The complete Spawn1 Focus/Blur/Event bodies are still required dependencies.
Their metadata and empty Update do not make the virtual bodies implemented.
These modules do not override the previously proved Level initialization order:
completed InitPost stage10 precedes _LoadCharStates stage18; initial state
selection remains owned and source script publication is not assumed absent.

## Source CSM_Spawn current17 permission

`predicate-dependencies/` captures GroupInfo::CanSpawn `0x3d24fc`,
CanRespawn `0x3a5248`, and GroupInfo::CanRespawn `0x3d2a34`.
Actual GroupInfo::CanSpawn is exactly `mov r0,0 / bx lr`.

For current17, CSM_Spawn loads live Character+`0x3fc` group pointer. A nonnull
group calls that genuine always-false body and returns0. A null group returns
the raw `auto_spawn` byte at Character+`0x1430` (0..255). It never changes the
by-reference next state. `character_spawn_permission.hpp/.cpp` exports
`dh2_character_pre_spawn_permission(out, SpawnPermission16*)` for this exact
branch: `{uintptr group, uint32 auto_spawn, reserved0}`;1 delivered,-1 malformed
atomic. The helper rejects overlapping result/view storage. The caller must
supply actual live group identity; an unimplemented group producer cannot be
silently represented by null merely to accept Spawn.

The frozen PreSpawn predicate delivery can fill response.word with this helper;
retain its initial next1 untouched. This helper is deliberately limited to
current17. Other nonzero/non17 CSM_Spawn states return1 in the source; Limbus0
delegates Character::CanRespawn. That checks byte+1481, resolved property11>0,
then live group ownership. GroupInfo::CanRespawn can inspect the member vector,
live member FSM state getters and mutate group word+24. These deeper respawn
and group producers remain explicit required backends, not fabricated booleans.

## Proof and reproducibility

- `character-spawn-select-arm64-differential.json`:3456 original↔O2 ARM64 cases,
  3456 ordered requests,1080 actual original RNG executions,243 selected
  zero-duration timers, zero mismatches. Includes signed extremes, negative and
  reversed intervals, truthy/ignored bool words, seed and counter wrap, and
  ignored timer results. Gold `spawn-select-fixtures.bin` SHA256
  `2b3300fb372b3379d9c4a88c79a2db16f4fc637f98613dc8d4dd7462b3e0734a`.
- `character-spawn-permission-arm64-differential.json`:512 original↔O2 ARM64
  current17 cases,256 actual GroupInfo rejection calls,512 preserved source
  next words, zero mismatches. Gold `spawn-permission-fixtures.bin` SHA256
  `dcc708268a7acb732f0c0b5c9000b64d77d55387768b5df9bd575c7e2cf53f22`.
- `character-spawn-select-host-audit.json`:same two corpora,10 selection
  guard/failure-prefix checks,5 permission atomic guards, and four genuine
  TimerStore/owned StateInfo routing compositions. The latter verify positive
  expiry, zero-duration non-expiry, immediate selection and synchronous nested
  selection before outer timer creation. Twelve remaining state-method
  deliveries are explicitly fixtures; this does not claim original Spawn1
  behavior or a complete AI-event/Level/frame pipeline. ASan/UBSan/leak0.

The proof builds are isolated, not central DSO or APK/device evidence. No frozen
modules, CMake, renderer, Android projects, prior reports or checkpoint were
edited. Native source uses no fast math or float contraction. Parent host target
can compile `tests/character_spawn_select.cpp`, link the new select/permission
sources plus existing game-data combat, state owner and timer kernels, and pass
the two gold files in select-then-permission order. The standalone reproducer is
`tests/character_spawn_select_host.py --output <new-report-path>`.
