# Source Spawn1 methods

Original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The focused manifest and complete assembly in this directory bind these
shipping methods and the target/sneaking/fade dependencies:

| Method | Address | Size |
| --- | --- | --- |
| CSSpawn::OnFocus | `0x3c35ec` | 424B |
| CSSpawn::OnBlur | `0x3c2f7c` | 164B |
| CSSpawn::OnUpdate | `0x3bfff0` | 4B `bx lr` |
| CSSpawn::OnEvent | `0x3c0b04` | 76B |

## Complete recovered method ordering

Focus first executes DebugSwitches.load/GetSwitch(`isTracingCharState`), then
load/GetSwitch(`isTracingCSSpawn`). Both results are ignored, but the source
owned debug-map/file effects still execute. String construction/destruction is
an explicit native STL boundary in the original instruction harness; the
native requests retain the exact query identity/order.

After those calls, Focus captures whether previous state is17 and live
Character flags+520 has bit2000. It writes flags`0x241`. It captures the current
CharAnim row-array pointer **before** GetCharAnimTableId`0x3a3228`, reads word32
(`+0x80`) of the selected160B row, then queries exact constants
`AnimStancedAnim/SL__LIST_IPHONE`. Mask bit1 adds genuine GetAnimStance
`0x3a53e0`; other mask bits cannot enable stance. Base plus signed stance wraps
as a32-bit word. This base is held across the constants/stance callbacks.

Then, in order:

1. ANIM_Set`0x3cacb0` with the recomputed sequence.
2. AI_SetTarget`0x3d6890`(NULL,false).
3. AI_SyncLastTarget`0x3d49c4`, copying the live target to last-target.
4. CancelSneaking`0x3bc6b8`.
5. If the previously captured PreSpawn flag was present, reread live flags and
   OR2000. Preserve all other effects a callback made to those live flags.
6. Reload live VisualObject at Character+2d8. When nonnull, call
   StartFadeIn`0x470ce4` with raw float32 word at Character+1440.

No explicit speed setter, Revive or physical creation call occurs in Focus.
The existing actual ANIM_Set implementation owns any internal animation/speed
initialization. In particular, do not inherit the PreSpawn fallback's separate
SetSpeed(0) call as a Spawn instruction.

The original shipping StartFadeIn is exactly `bx lr`. Its empty behavior is
proved; this stage does not invent a visual fade algorithm. Preserve the actual
nonnull branch, receiver and float argument delivery. NaN/signed-zero words are
copied unchanged because the method performs no float arithmetic.

Blur executes DebugSwitches.load/GetSwitch(`isTracingCharState`), then reloads
live flags. If2000 is absent, it calls actual InitPhysicalObject`0x3b4088`.
If present it finishes with no physical creation call. Update is genuinely
empty and uses the already-proved empty-method helper for state1/source3bfff0.

OnEvent only handles28. It compares the borrowed payload string against the
exact original literal `is_interactive` at8c4b70. An exact match rereads flags,
ORs2000, and tailcalls InitPhysicalObject. Other strings/events do nothing.
The outer owned StateInfo event mapping remains separate; for example state1's
animation22→Idle3 mapping is not implemented by directly selecting Idle inside
this virtual method.

## Stable native API

`character_spawn_body.hpp/.cpp` exports
`dh2_character_spawn_body(SpawnBody48*, operation, previous, event, payload,
SpawnBodyServices16*)`. Operations are independent StateMethod selectors
Focus0/Blur1/Update2/Event3; translate the outer StateOwnerOperation enum.
SpawnBody48 borrows State, full64-bit Character identity, actual decoded
40-signed-word row array/count, and live VisualObject/fade-word projections.
Two reserved words must be zero. Current1 is required on entry. The State and
Character binding are captured for the entire call, matching held source
registers even if a synchronous provider changes the live view.

The callback returns0 on delivery; nonzero fails at its delivered prefix.
Return1 means complete,`-1` malformed input before mutation,`-2` required provider
failure,`-3` invalid source row/null event28-string boundary. No rollback.
Borrowed Character/State/provider/table backings must remain alive across
synchronous callbacks, including a table captured before an index callback
changes the live table pointer. Services may reenter the actual owner; this body
never selects a current StateInfo, resets outer elapsed, or emits Character1d.

Service IDs:

| ID | Required delivery |
| --- | --- |
| 0 | DebugSwitches.load |
| 1 | GetSwitch; argument0=0 `isTracingCharState`,1 `isTracingCSSpawn` |
| 2 | actual GetCharAnimTableId |
| 3 | exact stance constants; response mask |
| 4 | actual GetAnimStance; response signed word bits |
| 5 | ANIM_Set; argument0 sequence bits |
| 6 | actual AI_SetTarget(NULL,false) |
| 7 | actual AI_SyncLastTarget |
| 8 | actual CancelSneaking |
| 9 | source-empty StartFadeIn; payload VisualObject,argument0 float32 bits |
| 10 | actual InitPhysicalObject |

Every SpawnBodyRequest32 retains the captured64-bit Character identity;
argument1/2 are zero. The fade callback can use
`dh2_character_spawn_fade_in_empty()`, which returns1 for the proved empty
method; convert that completion status to callback delivery0. Existing genuine
`dh2_character_ai_set_target`/`sync_last_target` and DebugSwitches owned APIs can
be composed by the caller at the exact delivery points. CancelSneaking,
animation and physical creation still require their real Character/backends.

Route the owned state1 method addresses into this module from the remaining
behavior service; route Update through the existing empty-method dispatcher.
Keep source owned transition ordering, event registry and source Character
RaiseEvent services. The frozen Spawn selection adapter may request state1 or
start timer2d; it must not replace this Focus/Blur choreography. The previously
proved Level InitPost-before-LoadCharStates ordering and actual script ownership
are retained; this module does not fabricate an initialization phase.

## Evidence and limits

`reports/character-spawn-body-arm64-differential.json` binds5458 complete original
method executions versus optimized standalone ARM64 and60104 ordered service
deliveries, zero mismatches. Seven helper-mutation modes exercise captured row
pointer/base, debug-before-flag capture, post-ANIM flags, live flags/visual/fade
after CancelSneaking, and held Character identity. Cases include previous-state
conditions, signed stance extremes, both rows, raw fade NaN words, strings,
Blur's physical gate and genuine empty Update. Actual original SyncLastTarget
and StartFadeIn bodies execute. AI_SetTarget, CancelSneaking, animation,
physical and debug implementation bodies are explicit service boundaries in
that corpus. Original compiler canary/string/libc ABI boundaries remain
ordinary harness/runtime services, not a stack-corruption parity claim.

Gold `spawn-body-fixtures.bin` SHA256
`a23ddde403a56dae258956782ea9e166752de12622b9d501b959a9febea8c70f`.
`reports/character-spawn-body-host-audit.json` replays the same5458 cases/60104
requests with ASan/UBSan/leak detection, adds23 malformed/failure-prefix/source
invalid checks and one synchronous interactive callback followed by Blur.
The interactive callback's2000 flag survives Focus, and Blur skips a duplicate
physical call. This supplemental nested host check is not an additional
original-instruction reentry parity claim.

The host composition also executes genuine existing native target Set/Sync and
DebugSwitches owned-map kernels. Target candidate/current/last become null;
source false-mode clears owner14d0 but retains source alive/sight/changed bytes
on the null-target branch. DebugSwitches contains9 genuine map keys; one
explicit missing-file provider fixture supplies ENOENT-like delivery. Animation
and CancelSneaking remain two declared required fixture effects in that
composition. No real filesystem, full physical/scene/AI/gameplay, central DSO,
APK or device claim is made.

Reproduce with `tests/character_spawn_body_host.py --output <new-report-path>`.
A parent host target can compile `tests/character_spawn_body.cpp`, link world
containing the new body plus existing empty/target/debug kernels, and pass
`reference/character-spawn-body/spawn-body-fixtures.bin` as its only argument.
The isolated host build uses function-section garbage collection solely to
discard unused Lua wrapper entrypoints; it does not replace used algorithms.
No frozen/shared source, central CMake, renderer, Android project, prior report
or checkpoint was edited.
