# Delayed Character script load and later Init phases

Original ELF SHA256: `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The focused manifest/assembly in this directory captures the six actual routines.
The broader existing character-script-lifecycle capture supplies the other called
wrappers needed by the executable fixture. Both are original ELF captures.

## Source distinction and eligibility

`CharAI::LoadScriptProcess` (`0x3cf1f0`) executes stages 0..6. Stage5 calls
`CharAI::InitScriptStep` (`0x3cb314`): Character AI OnInit, then pending AIS InitVCB
when the external-name pointer is nonnull. Its actual OnInit wrapper initializes
the Character's source timers and invokes pending AIS OnInit. Stage6 copies
pending `AI+0x20` to active `AI+0x1c` at `0x3cf268..0x3cf270`. Publication does not
run Character InitScriptProcess.

`LoadNInitScriptProcess` (`0x3cf3a4`) first tests active. Nonnull returns false
without loading or initialization. Otherwise it runs LoadScriptProcess, reloads
active, and returns false if still null. A newly nonnull active executes
InitScriptProcess(final) and returns true. This source guard, rather than an
invented native completed flag, prevents subsequent InitProcess calls.

`InitScriptProcess` (`0x3ce7c0`) delivers, in order:

1. Character `_InitHpMp` (`0x3b3a70`) using its entry-time owner.
2. `CharAI::SetSkillsAndSpells` (`0x3ce044`).
3. `CharAI::UpdateAllSkills` (`0x3d8894`).
4. CharAI OnInitPost (`0x3d0b80`), which reloads active and calls its virtual+12.
5. If final is true, CharAI OnInitFinal (`0x3d0ba4`), which separately reloads
   active and calls its virtual+16.

The Post and Final wrappers are genuine null-active no-ops. A callback's active
mutation is observed at the next wrapper; it does not erase captured earlier
receivers. The source delayed budget is queried only for a delayed non-Character.
Actual Crypt monster Characters execute all seven load stages without that budget.

All current Crypt kinds use actual decoded AI rows40/68 with delayed_load1.
Original InitPost's nonplayer/delayed branch stores Character+0x3ec1 and skips
the immediate load/Init. It still performs GrabAnimFX/RegisterAnimFX/SetAnimationSet,
position/AABB/Revive/physical initialization and initial vitals. Level stage18
loads authored/default Idle3 before frame updates, while active AIS may be null.
Exact upstream static instruction addresses and executed constructor/FX proofs
are retained in `reference/character-idle-startup` and `reference/character-init-fx`.

This helper does not decide Character.Update eligibility. The actual Update
prefix checks CanUpdate, current state exclusions0/12/2, byte+0x1480, active AIS,
then manager/UI/queue work before `LoadNInit(true)` at `0x3ac404`. The caller must
reproduce those gates and source continuation. The independent outer-prefix
module owns that composition; this helper is not a complete Character.Update.

## Native API and remaining services

`CharacterDeferredScript` borrows a nonmoving `CharacterScriptSession`. Its
`load_and_init(final, CharacterInitServices16)` reads active, invokes the existing
genuine session.advance, reloads active, then executes the exact InitProcess
projection. The caller supplies mandatory `_InitHpMp`, skills configuration and
skill update services; this adapter does not pretend fixture callbacks implement
those bodies. Session's existing private VM, alias map, ownership, registration,
cache, timers and source OnInit are reused. Selected external Post/Final call
the genuine source-derived virtual/alias/VM implementation. No new AIS identity,
second script owner or synthetic publication is created.

Results are source0 for already active/incomplete, source1 for newly loaded and
initialized, native-1 for malformed input and native-2 for a required failure.
An unsupported provider stops the native prefix and preserves its mutations.
There is no rollback. If publication preceded that failure, the next call still
hits the original active guard and returns0; it must not retry initialization.
Failure before publication quarantines this adapter. Providers must keep the
session and all captured identities/contexts alive across synchronous callbacks.
VM destruction, session replacement and asynchronous callback delivery are not
supported by this borrowed adapter.

Source root Lua load statuses remain available through owner.last_source_load_status
(positive parser/pcall diagnostics differ from negative native failures). Existing
selected-virtual calls retain their established generic runtime convention -2
for a protected Lua error; this adapter records and delivers those diagnostics,
then continues to Final as the original Call does. It does not relabel that
legacy return as a positive Lua status. Other negative selected-call results
are native failures. Earlier source load errors remain in adapter.error even
when later initial callbacks succeed.

Initial Idle must use the existing actual StateOwner/_SetState transition with
genuine Idle Focus and Character RaiseEvent services. Event1d's actual CharAI
active-null gate skips AIS delivery. `CharacterIdleEvents.initialize_idle` has
an explicitly narrower published-AIS precondition; that is an adapter contract,
not the original delayed startup producer. This new helper does not change it
or claim an initial Idle execution proof. Source InitPost/FX/scene/body/zone
services still precede LevelLoadStates.

## Evidence and scope

`character-deferred-script-arm64-differential.json`: 1,165 original/O2 ARM64
comparisons, 5,773 ordered requests, two synchronous nested LoadNInit active-guard
probes, zero mismatches. Complete original load, pending Init, active Post/Final
wrappers execute. Explicit fixture services provide allocation/Lua/skills/vitals/
timers. The corpus includes callback mutations of pending, active, owner, external
name, step and script flags, and final=false. Destroyed pending pointers are
excluded from live virtual calls; this is a borrowed receiver validity condition.

`character-deferred-script-host-audit.json`: O2 ASan/UBSan replay of the same
1,165 cases/5,773 requests, 13 atomic guards and five required failure prefixes.
Separate real private Session/VM composition executes all11 actual Crypt monster
initializations from exact common/monster bytes. Fourteen active Post calls,
thirteen Final calls, 29 nested active-guard deliveries, an explicit native
service failure prefix, a delivered positive source parse error, a delivered
Post Lua error, final=false and actual returned-table `_this` projection effects
pass. Source Lua Post callback reenters this helper while the VM is busy; the
already-published source active guard returns0 without generic VM reentry.

Host/current-level/cache projections and required vitals/skills are named test
fixtures. DebugSwitches is the genuine native backend with an explicit missing-file
fixture. Central64 dependencies are privately copied and hash-bound in the host
report, not rebuilt or rebound to unrelated current source. Sanitizer diagnostics
are zero. No APK/physical-device/full-AI/full-scene/whole-session ARM32 parity claim.

Build production sources: `character_deferred_script.cpp` and
`character_deferred_script_session.cpp`, linking existing lifecycle, session,
virtual and runtime modules. Independent oracle only needs the first source,
frozen lifecycle.cpp and its recorded virtual-return fixture. Host audits are
`tests/character_deferred_script.cpp` (gold path argument) and
`tests/character_deferred_script_session.cpp` (real-cache inputs, commons, monster,
DACT arguments). Exact compiler commands, inputs and binaries are bound in the
host report; no shared CMake or frozen sources were modified.
