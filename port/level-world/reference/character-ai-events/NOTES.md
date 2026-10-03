# Complete CharAI event dispatcher

`character_ai_events.hpp/.cpp` reconstructs the complete original
CharAI.RaiseAIEvent (`0x3cbb34`, 1764 bytes) and its active-gated OnScriptTimer
relay (`0x3d0ca0`, 36 bytes). Target/state/animation/private helper and virtual
AIS bodies remain named synchronous services. An unavailable binding is an
explicit failure, rather than acceptance or an empty callback. This is a bounded
dispatcher proof, not a complete AI/FSM/VM/Character frame reconstruction.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
[Manifest](original-functions.json) and [assembly](reference/original-functions.asm)
bind sixteen exact routines. [dispatch-producer.json](dispatch-producer.json)
decodes both actual branch tables and all51 CharAI vtable slot identities from
address point `0x966798`; [discover.py](discover.py) reproduces it. Event numbering
comes from table indices, not inferred names or neighboring branch addresses.

## First switch: before generic controller gates

The source captures event and payload at `0x3cbb40/0x3cbb48`. Its unsigned
comparison dispatches0..63 through the first table; all other32-bit event words,
including signed negative encodings, enter the generic gated path.

| Event | Actual operation and forwarding |
|---|---|
| `0` | Replace event by50001 (`0xc351`), then owner FSM. |
| `1` | Replace event by50002 (`0xc352`), then owner FSM. |
| `2` | AI virtual `+0x24` OnDied(payload), then reload owner and FSM. |
| `3` | AI virtual `+0x28` OnRevived(payload), terminal. |
| `0x22`, `0x23` | Exact private end helpers return1, AI virtual `+0x98` OnEndOfAnim(), then reload owner and FSM. |
| `0x28` | `_OnAnimEvent(payload)` `0x3d4434`; only its nonzero result forwards to the current owner's FSM. |
| `0x29` | AI virtual `+0x80` OnTimer(payload), then reload owner and FSM. |
| `0x30` | Owner FSM directly. |
| `0x31` | Store AI paused `+0x18=0`, return without AIS or FSM. |
| `0x32` | Store AI seeking `+0x4a=1`, return without AIS or FSM. |
| `0x33` | `_UpdateRegen` `0x3cb77c`, terminal. |
| `0x34` | Owner CharProperties `+0x560` HandleDots `0x3df3f0`, terminal. |
| `0x35` | Capture AI virtual `+0x90`, obtain timer GetID or UINT_MAX, invoke captured OnScriptTimer(id), terminal. |
| `0x3f` | Owner EnableCollisions `0x394a3c`, then reload owner and FSM. |

The private helpers `_OnEndOfAnim` (`0x3d3aec`) and `_OnEndOfAnimSection`
(`0x3d3ae4`) are exact `mov r0,1; bx lr` bodies. The native dispatcher preserves
this actual result; it does not manufacture acceptance for other helpers.
The original comparison executes those two bodies instead of intercepting them.

## Generic path and second switch

At `0x3cbcfc` the source captures owner and controller. A nonzero controller
forced byte `+9` bypasses global `v2Controller::s_blocked` and locked byte `+8`.
Otherwise either block forwards directly to the owner FSM while suppressing
the generic AI consumer. There is no paused, seeking, flags520 or active-AIS
gate in this outer dispatcher. The direct first-switch branches above also
bypass generic global/locked gates. The [frame module](../character-ai-frame/NOTES.md)
independently resolves the exact shared global at `0x9a318b` and its GOT slot.

| Event | Source operation when generic gates pass |
|---|---|
| `4` | AI virtual `+0xb0` OnKill(payload), terminal. |
| `7`, `8` | FriendSpotted `+0x2c` / NeutralSpotted `+0x30` (payload), terminal. |
| `9` | EnemySpotted `+0x34` (payload), then reload owner and FSM. |
| `0xa..0x19` | No-argument target/master notification virtuals `+0x40..+0x7c`, terminal. Exact names are in dispatch-producer.json. |
| `0x1d` | Capture AI virtual `+0x20`; GetState on the already captured owner's FSM; invoke captured OnStateChanged(payload, live_state), terminal. |
| `0x1e`, `0x1f` | SkillFocus `0x3d808c` / SkillBlur `0x3d8b7c`, terminal. |
| `0x20`, `0x21` | SpellFocus `0x3d8038` / SpellBlur `0x3d8b28`, terminal. |
| `0x24`, `0x25` | SequenceBegin `0x3d3d4c` / SequenceEnd `0x3d3d30`; nonzero private-helper result forwards to current owner FSM. |
| `0x26`, `0x27` | StepBegin `0x3d4204` / StepEnd `0x3d3ff8`; nonzero private-helper result forwards to current owner FSM. |
| `0x2a`, `0x2b`, `0x2c` | AttackDelayExpired `+0x8c`, StunExpired `+0x84`, FearExpired `+0x88`; each reloads owner and forwards to FSM. |
| `0x37`, `0x38` | CollisionBegin `+0xbc` (payload,true/false), terminal. |
| `0x39`, `0x3a` | CollisionPersist `+0xc0` (payload,true/false), terminal. |
| `0x3b`, `0x3c` | CollisionEnd `+0xc4` (payload,true/false), terminal. |
| `0x3d`, `0x3e` | CollisionResult `+0xc8` (payload,true/false), terminal. |
| Other events | Captured owner FSM, without a generic AI callback. |

Important numbering distinction: **StateChanged is0x1d;0x2a is AttackDelayExpired**.
The collision sequence starts at0x37 with true, and ends at0x3e with false.
Event0x3f is the separate direct EnableCollisions branch. Table evidence prevents
incorrectly assigning adjacent handlers or source symbol names to event IDs.

## Captured calls, owner reloads and actual timer relay

Event35 loads the AI `+0x90` function into r4 at `0x3cbcc8`, before invoking
the payload timer's virtual GetID at `0x3cbcdc`. Replacing the AI table during
GetID does not replace this saved callable. It still invokes the same AI object
at `0x3cbce4`. A null timer uses the exact word UINT_MAX, not user_ref or0.
Afterward the function returns directly, without generic gates or FSM delivery.

Event1d similarly saves AI `+0x20` before GetState at `0x3cc050/0x3cc054`.
GetState uses the captured owner FSM even if a callback replaces current owner;
the saved callable then receives the exact returned state word. Expired2a/2b/2c,
Died, Timer29, EnemySpotted, EndOfAnim and EnableCollisions explicitly reload
owner before FSM. A private animation helper's nonzero result is retained across
its callbacks and followed by the source owner reload. Event and payload remain
captured for the entire invocation, including recursive event delivery.

The actual `CharAI.OnScriptTimer` reads active AIS `+0x1c`; null returns. Otherwise
it calls that AIS's virtual `+0x90` with the exact timer ID. The new
`dh2_character_ai_event_script_timer` reconstructs this entire36-byte wrapper.
It does not choose an active AIS or a VM when one is absent. The selected
AISPlayerIPhone source `+0x90` inherits AISDefault.OnScriptTimer `0x3dcc80`, whose
genuine VM bridge is `dh2_script_game_on_timer(vm,id)` from the
[script-runtime binding](../../../script-runtime/reference/game-bindings/NOTES.md).
It converts the uint32 ID as signed32 before passing the source float value to
Lua. The selected ordinary OnTimer `+0x80` is a different callback.

The native TimerStore expiry callback receives a Timer32 pointer; event35 must
query its ID. The event service return status and out-result word are distinct,
so diagnostic negative native timer errors can remain failures instead of being
accepted as IDs. Timer.user_ref is not this ID. Timer allocation/iteration,
Character.RaiseEvent and active AIS↔VM ownership remain explicit parent adapters.

## Native ABI and contracts

`AIEventState64` projects AI identity, borrowed current-owner projection,51-entry
AI callable table, active AIS identity,51-entry AIS callable table, raw paused/
seeking/global bytes and zero reserves. `AIEventOwner48` holds owner, controller,
FSM and properties identities plus forced/locked bytes. These are logical native
projections, not original ARM32 layouts or a reconstructed C++ CharAI class.
The AI object's identity is fixed during an invocation; callbacks can refresh
current owner, tables, active AIS and actual source field values. Captured old
owner objects and callable identities must remain valid through return.

`AIEventPayload24 {value,timer_get_id,reserved0,reserved1}` supplies the captured
payload and actual timer getter identity. `AIEventRequest40` carries service,
operation, event context, argument, subject, captured callee and payload. An AI
or AIS virtual operation is its original byte-slot number; a private helper
operation is its actual original entry address. Those identities are conveyed
to the service adapter; the kernel does not execute opaque pointer values.

Service IDs: FSM0, AIvirtual1, TimerGetID2, privateHelper3, GetState4, AISvirtual5.
The mask has six known bits. Callback integer return is operational status:
zero succeeds, nonzero fails. Query/acceptance words use the separate uint32
result. Ignored source returns remain ignored. Helper callbacks must execute
their genuine kernel/service and return its true result, not blanket acceptance.

Both APIs return0 complete,1 atomic malformed rejection,2 unavailable binding or
captured callable,3 callback failed. Completed phase is7; attempted phase is
service+1. Results retain last attempted service and entered callback count.
Source void returns are not repurposed as acceptance values. Previous effects
survive unavailable/failing later services. Valid aligned objects/table storage
and coherent synchronous replacements are caller prerequisites; supported
nulls/overlaps, invalid byte domains, identities, reserves and masks reject
without state/output/callback effects. Missing callable tables may be supplied
for unused paths; attempting an unbound call is explicitly unavailable. A null
active AIS completes the timer relay even with no AIS table/mask bit.

## Verification and integration handoff

Original versus NDK29 O2 ARM64 passes11,344 comparisons and13,381 ordered
request/state snapshots with zero differences. All first-switch and reached
second-switch branches are represented, including negative32-bit event words,
raw0/1/255 controller gates, null/non-null payload, query0/1/UINT_MAX and owner/
table/active/pause/seeking/global mutations.100 event35 cases execute the actual
original OnScriptTimer body and native relay, with both active absent/present,
signed-high IDs and captured-call mutation. Six reentry cases execute actual
recursive original/native event31 and verify outer forwarding sees the new pause.

Native ARM64 and host independently pass46 atomic malformed guards, six missing
services, three missing captured callable checks, six failure prefixes and
inactive relay without a service call. The host replays the same original gold
through the genuine production world DSO after central integration; both
dispatcher/relay resolve by dladdr to that library. ASan/UBSan findings are zero.
Gold SHA256:
`901646ae43be80f3afff0fdfba3480c1fa36199cc11a589567258ef0f8239202`.
Reports bind actual original, source, manifests, gold, executable and DSO hashes.

Central CMake adds `character_ai_events.cpp` to `dh2_level_world` and
`character_ai_events_audit` from `tests/character_ai_events.cpp`, linked with that
library and `${CMAKE_DL_LIBS}`. Parent owns those integration edits. Run:

```
python port/level-world/tests/character_ai_events_host.py --main-linked
```

Frozen header SHA256:
`e78a1542dd1316047414419bcea7d8ad9fee32a410959d2f80db4af47ef357c3`;
cpp SHA256 `fa459fb66755400887d4e042d1038013001ec2ebd3746962a7d2f29830baa5b0`.
Deep private-helper and virtual AIS/FSM/VM fixture outputs are service boundaries;
the report does not claim their arbitrary fixture words are full backend output.
No Android, APK, renderer, complete Lua/AI/timer/Character frame or live-game
equivalence follows from this component audit. Parent's genuine TimerStore→event35
→active AIS→VM composition is a separate integration proof.
