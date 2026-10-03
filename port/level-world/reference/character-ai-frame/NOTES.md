# Outer CharAI frame dispatcher

The complete original `CharAI::Update` (`0x3cfbf4`, 372 bytes) is reconstructed by
`character_ai_frame.hpp/.cpp`. Its target, master and aggro bodies remain explicit
synchronous services. The final virtual service can bind the existing complete
`dh2_character_ai_update`, which in turn binds the selected AIS kernel. This is a
complete bounded dispatcher, not a replacement for the full Character frame,
AI scheduler, targets, Lua or world backend.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The [six-function manifest](original-functions.json) and its
[assembly](reference/original-functions.asm) preserve the dispatcher, three
borrowed bodies and release profiler bodies. [Producer captures](producers/original-functions.json)
preserve application dt/frame production, Character ordering, timers, collision
accumulation, pause and expiry consumers. The original profiler Push/Pop bodies
(`0x3136b4/0x3136b8`) are each one actual `bx lr`; the original comparison executes
them. Omitting their native state effects follows those instructions.

## Source gates and exact order

| Source instructions | Operation |
|---|---|
| `0x3cfc0c..0x3cfc18` | Any nonzero AI paused byte `+0x18` skips the dispatcher. |
| `0x3cfc1c..0x3cfc2c` | Capture AI owner `+4` and owner controller `+0x378`; nonzero controller forced byte `+9` bypasses both global and local lock checks. |
| `0x3cfc30..0x3cfc5c` | When unforced, global blocked byte or controller locked byte `+8` skips. |
| `0x3cfc60..0x3cfc68` | Captured owner flags `+0x520` must have bit `0x100`. Forced does not bypass this check or pause. |
| `0x3cfc6c..0x3cfc8c`, `0x3cfd34..0x3cfd40` | Query captured owner virtual `+0xc4` IsZonable. If nonzero, read that same owner after the callback: nonzero zoned `+0x2ee` with zero in-zone `+0x2f0` skips. |
| `0x3cfc94..0x3cfca4` | Reload current AI owner `+4` and store its updated byte `+0x88=1`. |
| `0x3cfcb4` | `_UpdateTarget` `0x3cb908`. |
| `0x3cfcd4` | `_UpdateMaster` `0x3cc5a4`. |
| `0x3cfcf4` | `_UpdateAggro` `0x3cf3f0`. |
| `0x3cfd0c..0x3cfd18` | AI virtual `+0x18`, complete OnUpdate `0x3d1050` for this class. |

The old owner stays captured across IsZonable. Replacing AI owner in that callback
does not change which object's zone bytes are read, although the following
updated store uses the new owner. Both owners can be synchronously mutated.
There is no pause, block, forced, lock or flags recheck between target/master/
aggro/OnUpdate. A callback that changes pause affects the next dispatcher call.

The [GOT producer](debug-producer.json) resolves `0x994a98 + 0x3650` to slot
`0x9980e8`, whose target is `0x9a318b`, exact symbol
`_ZN12v2Controller9s_blockedE`. Its BSS initial byte is zero. This is the shared
controller blocked byte, not an assumed generic debug switch or permission.
[debug_producer.py](debug_producer.py) reproduces that resolution from the hash-
identified ELF. Source live-AI [producer evidence](../prince-live-ai/NOTES.md)
establishes actual script lock/unlock writes and transient forced command
intervals; those values must come from their real producers in live integration.
The complete source CharAI C1 (`0x3ced50`) writes paused=0 at `0x3ced90`; C2
(`0x3cebf0`) writes it at `0x3cec30`. HUD controller constructor initializes locked
and forced to zero. This wrapper does not establish the live initial owner
flags520 value or a complete Character initialization policy.

Correction to the previous `character-ai-update` outer trace: `0x3cf3f0` is actual
`CharAI::_UpdateAggro`, not script loading. Its first owner IsPlayer virtual call
at `0x3cf424` returns immediately when true. That particular source branch can
be used only with a genuine IsPlayer result; other aggro branches are not empty.

## Frame, dt and pause boundaries

The outer dispatcher has no dt argument, no Application clock/frame read and no
own timer increment. Application.ComputeDt (`0x320da4`) reads real milliseconds,
subtracts the previous word with 32-bit wraparound, applies its scale/truncation
sequence and stores the unscaled dt word at App `+0x8c`. Actual GetDt
(`0x31f66c`) returns that word. Application.Update calls ComputeDt at `0x32cda0`
and passes `App+0x8c` to `_Update` at `0x32cde4`. There is no new AI-specific dt
clamp. `_Update` increments the App frame stamp `+0x74` at `0x32c7ec/0x32c7f0`
on its reached normal tail; this is not proof that every skipped outer application
invocation increments it.

Eligible Character.Update calls optional TimerUtil at `0x3ac024`, its CharTimers
at `0x3ac02c`, then this dispatcher at `0x3ac034`, then FSM, CharAnimator and
GameObject at `0x3ac03c/0x3ac048/0x3ac054`. Thus an already scheduled pause timer
can expire before AI in the same actor phase. CharTimers.Update obtains GetDt at
`0x3db6a4`; its timer RaiseEvent call `0x3db7f0` passes the actual timer pointer,
not a synthesized user-reference payload. Timer allocation, expiry iteration and
Character event routing are not implemented inside this wrapper.

Selected AISDefault.OnUpdate consumes collision-produced unsigned `AIS+0xbc`.
OnCollisionPersist's tail compares last stamp `+0xc0` to App frame `+0x74` at
`0x3dc02c..0x3dc034`, skips when AI pause `owner+0x3e0` is nonzero at
`0x3dc03c..0x3dc044`, then stores the stamp and adds GetDt with wrapping word
arithmetic at `0x3dc048..0x3dc058`. Collision/type/state gates precede this tail.
It must not be replaced by an unconditional per-frame counter increment.

At counter>199, the actual selected update clears that counter, pauses the
embedded AI and schedules event31 for 1000ms, then reloads controller and Stop.
Actual AI RaiseAIEvent event31 clears paused and returns without AIS/FSM forwarding.
The composed proof executes this real event31 branch and resumes a following
frame; it does not fabricate elapsed timer delivery. General application/level
pause and object eligibility gates occur outside this module, as documented by
the [frame-order trace](../frame-order/NOTES.md). Aggro's deeper branches read
GetDt at `0x3cf7e8`, `0x3cf8a0`, `0x3cf8b4`, but those clocks belong to the
borrowed service body, not to a guessed dispatcher input.

## Native binding and caller contract

`AIFrameState32` holds AI identity, pointer to the current `AIFrameOwner48`, raw
paused/global bytes and zero reserves. Owner projection contains the owner and
controller identities, flags520, forced/locked/zoned/in-zone/updated bytes.
Borrowed old and new projections must remain valid throughout callbacks; the
current owner can be replaced synchronously. Callbacks must maintain coherent
valid projections. The wrapper has no allocation or ownership side effects.

Service IDs are IsZonable0, Target1, Master2, Aggro3, OnUpdate4. Only IsZonable's
full32-bit result affects this dispatcher. `AIFrameRequest16` preserves subject
identity above4GiB. IsZonable receives the captured owner identity; the remaining
four calls receive the AI identity. Services mask31 expresses actual available
bindings. Required unavailable services return2; callback failure returns3,
preserving prior synchronous effects. Neither condition becomes a silent no-op.

`dh2_character_ai_frame(out,state,services)` returns0 on completion or a source
skip,1 for atomic malformed rejection,2 unavailable,3 callback failed.
`AIFrameResult16` reports phase(service+1 while attempting;6 complete), skip,
last attempted service and entered callback count. Nulls, overlapping objects,
missing identities, invalid byte ranges, reserves and unknown masks reject
before output/state changes. Callback replacements are a caller contract, not
untrusted pointer validation or transactional rollback.

## Verification and practical handoff

`character-ai-frame-arm64-differential.json` passes 6,761 actual original versus
NDK29 O2 ARM64 cases, 5,642 ordered request snapshots, zero differences. The
matrix tests raw0/1/255 byte gates, flags0/0x100/UINT_MAX, full-width query results,
old/new owner zone writes, pause/block/policy mutations and no mid-frame recheck.
120 cases are composed: actual original outer dispatcher, full original
OnUpdate, selected AISDefault, PauseUpdate and their native kernels execute.
28 paused next frames make no service requests;28 actual event31 comparisons
clear pause before resumed dispatch. IEEE saved position words (-0, NaN payload,
infinity) remain exact through the composed callbacks. Native-only contracts
independently pass27 atomic guards,5 unavailable bindings and5 failure prefixes.

Gold `frame-fixtures.bin` SHA256:
`7289e74b57fe85339626f9b8dd6c9d77779b2ad4c1ae9c6d01b573131e4eb52c`.
The host sanitizer audit replays the identical gold and verifies those same
contracts, plus the actual source-built pause-expiry helper. Its full OnUpdate
and selected kernel resolve via dladdr to the actual current
`libdh2_level_world.so`; after parent central integration the final frame kernel
also resolves to this same production DSO. The final report has
`main_world_library_executed=true`. Reports bind actual binaries, original corpus/instruction
reports, sources and producer captures. ASan/UBSan findings are zero.

Parent central CMake integration requires adding `character_ai_frame.cpp` to the
world library and `character_ai_frame_audit` from `tests/character_ai_frame.cpp`,
linked to `dh2_level_world` and `${CMAKE_DL_LIBS}`. No shared build/CMake changes
were performed by this worker. The host runner default builds only its own
standalone audit DSO/executable; `--main-linked` runs an already built central
target and checks all three kernel addresses belong to the main world library.

```
python port/level-world/tests/character_ai_frame_host.py
python port/level-world/tests/character_ai_frame_host.py --main-linked
```

Production source is stable: header SHA256
`6f93867b30aeb651c68787493a7ea377670a86f0f11e0e4f1f406e03f3ac883f`;
cpp SHA256 `9694d48f363c07102988223ccb1c89c932b0693afa919f95d491f70099993e04`.
No renderer, APK, Android build, emulator, complete Character frame or full
target/master/aggro/Lua backend equivalence is claimed.
