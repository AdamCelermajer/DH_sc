# Owned Spawn composition

This new adapter composes recovered kernels. There is no original function named
`character_spawn_owner_extensions`; its isolated host proof is not packaged ARM64
instruction parity or a complete scene AI proof. Original ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The existing frozen state-owner extensions, bodies, source corpora and reports
were read and linked without modification.

## Source routing and ordering

Owned StateInfo1 routes Focus `3c35ec`, Blur `3c2f7c`, OnEvent `3c0b04` and
Update `3bfff0`. The adapter validates the live machine, current StateInfo,
Character identity and method source key before routing a body. Update is the
proved empty body. Other methods remain delegated through the frozen extension,
including complete PreSpawn17 and source-proved empty methods. Nonempty unknown
families require a real provider and fail at that provider when unavailable.

For PreSpawn event9, the adapter supplies exact current17 `CSM_Spawn3ad2e4`:
nonnull group calls the proved false `GroupInfo::CanSpawn3d24fc` result; a null
group retains the raw `auto_spawn` byte0..255. The response's initial next1 is
preserved. Limbus0 CanRespawn is a separate backend and is not substituted here.
Accepted event9 invokes source `SM_SetSpawnState3c2734(true,false)` through the
frozen selection kernel. Zero equal bounds perform the real owned transition to
1; nonzero/equal or unequal bounds create a real TimerStore timer with repeat0,
event2d and reference0. Unequal bounds consume the caller's shared CombatRandom.
An unequal interval can generate duration0; that timer remains active and never
expires through the genuine TimerStore update. No immediate transition replaces it.

State selection uses `dh2_character_state_owner_transition`, preserving outgoing
Blur, owned current lookup/write, elapsed reset only when ID changes, incoming
Focus and Character event1d notification. Even a same-state request delivers
Blur/Focus/notification and retains elapsed. The full outer behavior service chain
is used, never a partial Spawn-only chain or direct current assignment.

PreSpawn resumes its captured body after selection, even when nested callbacks
have moved the live state to1 or3. Its live `ai_kind==3` continuation then clears
the supplied AI word. If a required Spawn provider fails, the already selected1
and prior effects remain, and that later continuation is not executed. Outer
event routing reloads the live current row after the body; event9 has no mapping
on Spawn/Idle and therefore returns0 after the real nested transition.

Spawn Focus executes actual target-null selection/synchronization through the
caller's genuine target backend. DebugSwitches queries remain real required
services. ANIM_Set, CancelSneaking, Revive, Character enabled/collision effects and
InitPhysicalObject are mandatory scene providers. Fade requests are delivered to
the Spawn provider using `dh2_character_spawn_fade_in_empty()`, the exact original
`VisualObject::StartFadeIn470ce4` bx-lr body. There is no fabricated fade clock.

## Installation and lifetime

`SpawnOwnerExtensions72` borrows machine, frozen extension, Spawn body/services,
selection/RNG, permission, TimerStore/services and full outer methods. Store all
of them persistently for the Character's lifetime. Their binding identities must
not be replaced or destroyed during callbacks. Refresh mutable source fields
through existing provider projections before returning to source reload points.
Captured table backings must remain alive even if a live row pointer changes.

Set `StateOwnerBehaviorContext40.remaining` to
`{&spawn_extensions, dh2_character_spawn_owner_extensions_method}`, then bind the
outer behavior services and store their stable address in `outer_methods`.
Set `StateOwnerFrameContext56.other_updates` to
`{&spawn_extensions, dh2_character_spawn_owner_extensions_update}`.
Initialize via the existing owned `initialize_level(preset, outer)` command.
The additive `dh2_character_spawn_owner_select` is the genuine Spawn selection
entry; its two inputs retain the source delay truthiness and ignored second bool.

TimerServices must contain a genuine caller expiry provider. This adapter starts
timer records but does not fabricate Character/CharAI event routing or bypass
active-script/blocked dispatch gates. Caller storage exhaustion and provider
failures are explicit prefix failures. Timer delivery must end before contexts
are destroyed. Each PreSpawn call uses an independent stack service wrapper and
copy of the frozen extension, so synchronous reentry never mutates its wrapper.

## Evidence

`reports/character-spawn-owner-extensions-host-audit.json` binds new/frozen source,
the optimized sanitizer executable, compiler command, original ELF and existing
original/O2 ARM64 kernel reports. The new adapter replay covers:

- 5,458 original Spawn body cases and 60,104 exact ordered requests through the
  new owned metadata method adapter.
- 3,456 original selection cases through genuine owner transitions/TimerStore,
  including 243 duration0 timers; 512 actual group/raw-byte permission cases.
- PreSpawn→Spawn→Idle with nested interactive event during ANIM_Set, required
  physical prefix, source carried flags2241, genuine target/debug effects and
  source continuation after nested notification→Idle.
- Empty Spawn Update with elapsed advancing exactly once and unsigned wrap,
  same-state selection retaining elapsed, timer2d expiry through explicit caller
  event delivery, storage exhaustion, unavailable scene provider and unsupported
  state8 method failures, and ten malformed owner guards.
- Address/undefined/leak sanitizer diagnostics0; full scene backends false.

The missing DebugSwitches filesystem result and remaining scene effects in this
composition test are explicitly fixtures. The DebugSwitches/target kernels and
owned state/timer kernels execute genuinely. Timer expiry routes owner.event in
the declared test provider; that is not proof of the full production AI relay.
Reproduce with `tests/character_spawn_owner_extensions_host.py --output` pointing
to a new report. The C++ executable takes Spawn body, selection, permission gold
paths in that order. No CMake, renderer, central binary, APK or device was changed.
