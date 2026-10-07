# Owned Character FSM frame composition

Original ELF SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The focused manifest/assembly beside this file binds complete FSM Update
`0x3c628c`, Idle Update `0x3c0e80`, Move Update `0x3c1184`, Attack Update
`0x3c14f8`, and empty Dead Update `0x3c004c`. The twenty actual Update method
addresses come from the original factory/vtable registration probe in
`../character-monster-state-ownership/registration-plan.json`.

## Native API and ordering

`character_state_owner_frame.hpp/.cpp` adds
`dh2_character_state_owner_frame(StateOwnerFrameContext56*)`. The context
borrows the real owner's machine, persistent Facts/Services, outer FSM services
and a mandatory provider for the other sixteen virtual Update methods. It owns
no Character, physical body, animation, timer, AI or state storage.

It runs the existing source `dh2_character_native_fsm_update` once. Source
profiling begin precedes capture of previous elapsed; engine GetDt follows;
wrapping old_elapsed+dt is stored before pending stun then scare enforcement.
Both enforcement calls retain their original duration/policy/subject/payload.
Current is reloaded after those synchronous calls. A nullable current skips
virtual Update, then source profiling end still executes.

The current method comes from the owned current-index StateInfo. Every row's
Update address is checked against the original twenty-method table. Bounded
3/4/5/12 invoke the existing genuine bounded body with **dt0**, avoiding a second
elapsed increment. Other families deliver `StateOwnerUpdateRequest24` containing
the captured original function address, signed ID, current native Character
identity and elapsed word. They must have a real provider; the adapter does not
pretend their bodies are empty. Addresses are dispatch/provenance keys, not
callable native pointers. Character identities retain all64 bits.

No whole transition or event implementation is duplicated. `RaiseEvent` from a
body is a caller service, which must use the real Character/AI route and the
owner's registered-event machinery. Service reentry may transition the owner
before the frame continues. Facts must be refreshed before returning after a
producer changes. Context/machine/fsm/state/backing rows and providers must
remain alive; fsm/state bindings cannot be replaced during the outer call.
Malformed input returns-1 before callbacks. A required failure stops with-2 at
the already delivered prefix, retaining source elapsed/effects without rollback.

## Commands and remaining boundary

New `commands/` captures preserve actual source command facts without silently
implementing a different policy. `SM_SetIdleState` `0x3c1a00` stores its boolean
at **machine+3c**, then calls `_SetState(3,-1,NULL)` unconditionally. The machine
is embedded at **Character+4fc**, so **4fc+3c=538**: this is exactly the same
source byte as bounded `State.idle_suppressed`. An earlier version of these NEW
notes incorrectly described two distinct fields. The focused initialization
alias probe in `../character-state-owner-initialization-alias` records the
actual identical addresses and source writes; no owner correction is needed.

`SM_SetAttackState` `0x3c6488` sends registered event0xc354 with the payload for
false, or force-selects5 with event0xc354 for true. Move selection is source
event0xc351 (for example Character event0 mapping through CharAI). Caller command
services must choose the genuine source path, then call owner.event/transition;
direct assignment of current does not own StateInfo or preserve Blur/Focus.

`SM_SetDeadState` `0x3c58c8` is488B, including authored table/stance selection,
machine+38/+3e/+3f policy fields and current clearing when the source IsDead
predicate succeeds. It is **not** replaced by a generic transition to12. Source
death-command selection and the remaining16 behavior families still require
their genuine backends. Spawn remains the existing required predicate service.

Use this frame after source Character timer and CharAI updates and before
CharAnimator Update and GameObject UpdatePath; scene/timeline/root and Level
physics Step precede that Character phase. This module does not schedule those
outer phases or prove a complete monster AI frame.

## Proof and reproduction

`character-state-owner-frame-arm64-differential.json` executes actual ARM32
original complete FSM Update and bounded virtual bodies against optimized NDK
ARM64: **906 frame Update cases**, with879 ordered body requests. Unchanged
transition/event/getter regressions bring the total to3910. The generated corpus
is byte-identical to historical `state-reference.bin`, SHA256
`9e11a899a29f9682959c00e988ad4fc04ee4a3f610de9044e933d47997d27a57`.

`character-state-owner-frame-outer-arm64-differential.json` executes the new
frame directly as ARM64 over **1536 original complete Update traces**, preserving
5808 externally visible clock/profile/effect requests. It checks1104 original
other-family method deliveries. The old fixture-only current-virtual marker is
omitted from both traces because the public adapter executes that call rather
than exporting an extra logging effect; bounded bodies execute, while their
exact state/requests are checked by the906-case proof. Effect mutations and
unbounded Update bodies in this corpus remain explicit fixtures.

`character-state-owner-frame-host-audit.json` binds the actual optimized
ASan/UBSan/leak-enabled executable and final source bytes. It replays both
corpora, then checks16 malformed/prefix boundaries, captured elapsed after dt
callback mutation, nested Move->Idle registered routing, null-current selection
during profiling begin, and genuine owner transitions during stun/scare service
delivery. No sanitizer diagnostics. These extra reentry checks use native owner
composition, not a claimed complete original Character/AI backend.

Build the isolated oracle with
`tools/build_character_state_owner_frame_oracle.ps1`. Run the two new differential
scripts with `--library` pointing to that ELF; the bounded script also takes
`--engine`, `--reference-output`, `--report`. The outer script takes `--output`.
Host reproduction is `tests/character_state_owner_frame_host.py --output <report>`.
Main CMake host target should compile `tests/character_state_owner_frame.cpp`,
link the genuine production world library and pass the historical
`reference/character-state/state-reference.bin` and
`reference/character-native-fsm/native-update-fixtures.bin` paths (argc3).

No frozen module, historical report, main CMake, renderer, APK or ADB operation
was changed. This is isolated native source composition proof; actual packaged
instruction parity, full AI/physics/playback backends and physical ARM64 gameplay
remain outside this report.
