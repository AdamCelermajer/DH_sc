# w3-pose report (wave 3): monster stun/scare POSE and the player's own stun/scare/knockback

Branch `v21.8-w3-pose`, worktree `.claude/worktrees/w3-pose`. Base: `windows-foundation` @ 28dfa273 (w3-status), later merged with
`windows-foundation` @ ed0f2bfc (adds w3-dot, w3-ally), no conflicts. Subst `V:` (released at the end), build dir `C:/dh218/w3-pose-build`,
package `.local-inputs/windows-source-clock-v21.8-staged3`. Never pushed, not merged into `windows-foundation`.

Executables used for the real runs (all kept in `C:/dh218/w3-pose/`):
`dh-foundation-w3-pose-3.exe` (sha 4E838854...), `-4.exe` (1804A267...), `-5.exe` (6F80C133...; adds the pose trace line),
`-6.exe` (merged tree, final).

## Verdict per item

| # | Item | Verdict | Evidence |
|---|------|---------|----------|
| 1a | Monster STUN pose (ViciousStrike, Rogue) | **WORKED** | real run `w3p-vic-2`: both lizards enter state 9 from Move4 / Attack5 at the stun outcome, pose frames viewed |
| 1b | Monster STUN pose (PinShot, Archer) | **WORKED** | real run `w3p-pin-1`: stun from Attack5, 3234 ms, pose trace + dense frames viewed |
| 1c | Monster stun via the generic seam (no skill), duration, return to Idle | **WORKED** | `w3p-nstun-2` pose trace: pre_stunned then the stunned loop for exactly the stun duration, `end ... stateAfter=3` at frame 3524 |
| 1d | Stun that arrives while the target cannot take it (Injured) is re-raised; a hit during the stun leaves it and the stun comes back | **WORKED** | `w3p-vic-2` (`start ... stateBefore=3 leftMs=264 frame=3501`), `w3p-nstun-1` (`start ... leftMs=1672 frame=3419`); unit test |
| 1e | Monster SCARE pose | **PARTIAL** | engine path, unit test and a real run that enters state 8 with the authored looping walk leaf; no random heading (see gaps); visual evidence inconclusive for the monster, clear for the player |
| 2a | Player STUN: input lock, pose, duration, state return | **WORKED** (seam, not an Act 1 source) | `w3p-seam-pstun`: state 9, flags 8706, controller lock 1, attack taps ignored, stun pose frames viewed, back to Idle with flags 9088 and lock 0 |
| 2b | Player SCARE: pose, lock, return | **WORKED** (seam) | `w3p-pscare-1` frames viewed, state 8 then 3 |
| 2c | Player KNOCKBACK (through the existing Push consumer) | **WORKED** (seam) | `w3p-pknock-1`: great knockback, state 10, flight/landing/rise frames viewed |
| 2d | Player stunned WHILE MOVING (state 4 -> 9) | **DID NOT WORK / NOT VERIFIED** | two real-run attempts: the injected D key / floor click did not start a walk in this harness, the player was Idle at the stun |
| 2e | A real Act 1 monster skill that stuns/scares the player | **NOT PRESENT in the data** | no Act 1 script applies it (w3-status found none; rechecked); the consumer is general, the seam stands in |

## 1. What the original does (IDA, `pseudocode-all.c`)

- `CharStateMachine::SM_SetStunState` 0x3c5ffc (line 142855): `IsBoss` returns first; valid CharAnimTable row required; on the first stun it
  starts timer 43 for the duration and sets sm+44 bit 0x2; pose index = `CharAnimTable[row] +140` plus `GetAnimStance` when the
  `AnimStancedAnim` constant has bit 0x200; then `_SetState(9, 50012)` (5th arg true) or `RaiseStateEvent(50012)`; persist flag sets sm+36 bit 0x800.
  `SM_SetScareState` 0x3c6144 (142895) is the same with timer 44, bit 0x4, `+124`, stance bit 0x100, state 8, event 50013.
- Field mapping: the CharAnimTable schema is alphabetical; **Stunned = field 32 (dword +140), Scared = field 29 (+124)** (fields after Interact
  15 and Spells 31 are array-sized, so dword = field + 2 / + 3; GreatKnockedBack 8 -> +36 and KnockedBack 16 -> +72 follow the same rule).
- `CSStunned::OnFocus` 0x3c3cc0 (141721): flags dword +0x520 = 8706 (0x2202), `CharAnimator::ANIM_Set(+140 [+stance])`, **for a Player
  (vtable +40) sets the controller lock byte `*(*(a3+888)+8) = 1`**, CancelSneaking, unpins the physics object. `CSStunned::OnBlur` 0x3c3b4c
  (141678) clears the lock byte (unconditionally) and pins again. **`CSStunned::OnUpdate` 0x3c549c (142438): when the sm+44 stun bit is clear it
  calls `SM_SetIdleState`** - this is the original Stunned -> Idle exit (the earlier port comment called it an inference).
- `CSStunned::OnInit` 0x3c8730 registers only 50008, 50010 (CSM_Injured) and 50011: **no event 34 (animation end)**, so a finite stun clip never
  leaves the state; a hit moves to Injured11.
- `CSScared::OnFocus` 0x3c45c4 (142003): flags 8768 (0x2240), `ANIM_Set(+124 [+stance])`, a random `Cmd_HeadTowards`, CancelSneaking, unpin.
  `OnUpdate` (142064): bit clear -> `ANIM_StopLoop`. `OnInit` 0x3c861c registers 44, 34 (-> Idle), 50008, 50010, 50011, 50012.
- `CharStateMachine::Update` 0x3c628c (142934): while the stun/scare bit is set and the state is not Stunned/Scared, it calls
  `SM_Set*State(0xFFFFFFFF, persist, nullptr, 0)` again, i.e. a **re-raise** of the event (no timer restart).
- Event admission (the OnInit tables, IDA 144504-144700). 50012: Spawn1, Idle3, Move4, Attack5, Skill6, Scared8, Interact13, Reviving15,
  LiftingIdle18, LiftingMove19 (not Stunned9, Cast7, KnockedBack10, Injured11, Dead12). 50013: Idle3, Move4, Attack5, Skill6, Interact13,
  Reviving15, LiftingIdle18, LiftingMove19 (not Spawn1, Scared8, Stunned9). `CSM_Interrupted` 0x3ad244 guards the Attack5 (byte +0x441) and
  Skill6 (bit 16 of +0x520) registrations.
- `F_ApplyResult` 0x3b10b4 gates the stun/scare/knockback tail on `IsDead`, not on `IsPlayer` (w3-status): the player takes it too.

The authored stun clip (lizard row 34, seq 381): `type1/loop0 [lizardman_pre_stunned, seq 379 (type0, loop -1) [lizardman_stunned],
lizardman_post_stunned]`; scare (seq 383): `type0/loop -1 [lizardman_walk_01]`. Player (Knight row): stun seq 278 `[prince_pre_stunned,
276 loop -1 [prince_stunned], prince_post_stunned]`, scare seq 277 `[prince_feared]` (loop -1). Moth/Troll/big lizard rows have the same shape.
So the pose is **entry clip once, then an infinite loop**; the post clip sits after an infinite container and is not reached by anything found
in the export (stun end is `SM_SetIdleState`).

## 2. Engine change (general, in the shared Session / status owner; no per-skill code)

- `combat_session.{hpp,cpp}`: `play_actor_source_sequence` now admits original states **9 (Stunned) and 8 (Scared)** as generic source states:
  focus flags 0x2202 / 0x2240, admission by `source_status_event_accepted_v1(state, 50012|50013, flags520)` (table above; direct/forced
  requests skip it), and a new policy shape for "entry clip, then infinite leaf" (`loop_state`, `loop_leaf_path`, `loop_only`, `loop_root_motion`):
  the Session plays the entry selection through the existing retained source path, and on its close seeds the infinite leaf (`seed_sequence`,
  repeat -1 from the source container) while the state stays 9; a state-8 pose with no entry clip seeds the leaf at once. The state ends only by
  an admitted departure (`cancel_actor_source_sequence`, a hit, knockback, death). Hit while Stunned/Scared accepts 50010 (CSM_Injured) and
  goes to Injured through the existing injury transition.
- `features/combat/session_status_pose_v1.{hpp,cpp}` (new): `build_session_status_animation_bank_v1` resolves an actor's own CharAnimTable
  Stunned/Scared roots (stance bits 0x200/0x100) and derives the entry/loop program from the source hierarchy; `play_session_status_pose_v1`
  plays it.
- `features/status_states/status_states_v1.{hpp,cpp}` (owner): `external state authority` mode. The owner keeps the bits, timers and the
  immediate-request flag only; it no longer switches a private state in that mode. New accessors: `sync_state`, `tracked_actors`, `stun_forced`,
  `scare_forced`, `clear_forced`, `refuse_pose`, `pose_refused`.
- `features/status_states/status_states_session_v1.{hpp,cpp}` (shared status owner, session side): `StatusPoseServiceV1`,
  `advance(CombatSession&, dt)`. Each frame after the Session update it mirrors the REAL Session state into the owner and then applies the
  original rules: exits first (bit clear while in state 9 / 8 -> admitted departure to Idle3), then starts (bit set, real state not the status
  state, event accepted by the real state or forced). A stun on a scared actor ends the scare pose first. Dead actors drop their status. The
  player is no longer skipped in `on_outcome` (counter `player_targets`).
- `features/physics/session_actor_transition_v1.cpp`: transition consumer cases for 8 and 9: Blur clears the controller lock and pins the body,
  Focus writes flags 8768/8706, the Focus suffix sets the lock for a Player and unpins. The Player lock for Scared is an input adaptation:
  CSScared registers none of the Move/Attack events (only 44, 34, 50008, 50010-50012), so the original state machine ignores those requests,
  but the lock byte itself is only set in `CSStunned::OnFocus`.
- `main.cpp`: per-profile monster status banks and per-stance player banks (clips join each Session visual before initialize, like the Push
  banks), the pose service (player: current equipment stance; monster: its profile), body-checkpoint flags for states 8/9, and the TEST seam
  `--test-apply-status KIND:TARGET:MS@FRAME` (KIND `stun|scare|knockback`, TARGET `player|nearest` (nearest hostile), MS the duration, or the great flag for
  knockback; applies through `script_status` / the Push sink at the given frame). Log lines: `[status-pose] start|end|trace`, `[test-status]`.

## 3. Tests

- `status_states_v1` (existing, +`test_external_state_authority`): bits, forced flag, refusal, expiry in external mode.
- `status_states_pose_session_v1` (new, real original tables and clips, a live CombatSession with the Knight and a lizard):
  acceptance table (50012/50013 by state, Skill6 needs flag bit 16); lizard stun from Idle plays pre_stunned (pose moving, far from idle) then the
  infinite stunned loop (state stays 9 after the entry clip, pose keeps animating), a repeated stun does not restart the pose or the timer,
  expiry returns to Idle3 and the idle clip; a stun while the real state rejects 50012 (PreSpawn17) is recorded and re-raised once Idle; scare plays the
  looping leaf; a stun replaces a scare; the player takes both on the same path; an actor without a bank keeps `can_act=false` but is not retried every
  frame; death while stunned leaves the pose and drops the status. The program derivation is asserted (lizard stun entry {0} + loop {1,0}, scare loop-only).
- Full CTest, llvm-mingw bin on PATH, `ctest -j4`: see section 6.

## 4. Real runs (hidden window, real injected keys, `act1rogue-run-rogue2.ps1`, package staged3; drivers in `coordination/v21.8/w3-pose-evidence/`)

Scene: Rogue (or Archer) seed at the Act 1 lizard zone, two lizards (`4660011647479645029` attacks the player, `11259224883678223927` is the
player's target). All frames below were VIEWED (contact sheets in `coordination/v21.8/w3-pose-evidence/*.jpg`, raw frames in the case folders).

**ViciousStrike (Rogue), `C:\dh218\act1rogue\cases\w3p-vic-2` (exe 3).** Cast at 3337; stun outcome on the second volley:
```
[status-pose] start actor=4660011647479645029 kind=stun forced=0 stateBefore=4 ok=1 started=1 stateAfter=9 leftMs=2184 frame=3362
[status-pose] start actor=11259224883678223927 kind=stun forced=0 stateBefore=5 ok=1 started=1 stateAfter=9 leftMs=2184 frame=3381
[status-pose] end   actor=4660011647479645029 kind=stun reason=stun timer end departed=1 ok=1 stateAfter=3 frame=3499
[status-pose] start actor=11259224883678223927 kind=stun ... stateBefore=3 leftMs=264 frame=3501   (stun re-raised after a hit/Injured)
[status-pose] end   actor=11259224883678223927 ... frame=3518
```
Frames `f3330 ... f3560` (sheet `vic2-sheet.jpg`): before the stun the lizards stand upright with the claws forward; during 3420-3505 the lizard is
hunched with the head down and the arms hanging and stays so for the window; the player's hits during it interrupt it (Injured) and the pose returns.
Both lizards left Move4 and Attack5 (not only Idle). The run ends at about frame 3720 with the known pre-existing
`Foundation error: Live combat: Source result identity/target eligibility rejected` (es-status item 7), unrelated to this work.

**PinShot (Archer), `...\w3p-pin-1` (exe 5, seed `w2-archer-PinShot`).** `Source skill key=2 ... frame=3424 ... skill=PinShot`; then
`start actor=11259224883678223927 kind=stun forced=0 stateBefore=5 ... leftMs=3234 frame=3461`, pose traces every 20 frames
(`clip=monster-source-status-Swamp_LizadMan_Type1/source-skill-sequence-381/phase-1 ms=263 ... ms=1623`, the loop wraps, no interruption), and
`end ... stateAfter=3 frame=3664` (3234 ms at 16 ms per frame). Frames (`pin1-sheet2.jpg`): upright at 3421/3445, hunched and stationary at 3469-3637, upright again at 3669/3685.

**Stun by the seam, no skill, `...\w3p-nstun-2` (exe 5, no player attacks).** `[test-status] stun ... ms=3000 frame=3336`, `start ... frame=3337`, traces
`phase-0 ms=31 frame=3340` (pre_stunned), `phase-1 ms=327 frame=3360 ... ms=1608 frame=3440`, wrap `ms=408 frame=3460`, `end ... frame=3524`
(3000 ms). This run has no useful frames (the lizard is at the top edge of the camera). `w3p-nstun-1` (with attack taps; frames `nstun-sheet2.jpg`): the target lizard
bends forward at 3342-3352, is hit at 3353, re-stunned at 3419 (`leftMs=1672`) and killed while stunned at 3438 (`dead=1`) with no error.

**Player STUN, `...\w3p-seam-pstun` (exe 3).** `[test-status] stun target=18446744073709551615 ms=2500 frame=3336`; `start ... stateAfter=9`, `end ... frame=3493`.
`STATE_TRACE` at 3360-3480: `sourceState=9 ownsPose=1 flags520=8706 localBlock=1 clip=player-source-status-2/source-skill-sequence-278/phase-1`; at 3510 `sourceState=3
flags520=9088 localBlock=0`. Between 3338 and 3495 the player's animation events are only the numeric end-of-entry-clip event (34 at frame 3347)
with `controllerBlocked=1`; no `attack_mainhand/offhand/do_skill` marker, although the 1.5 s injected attack taps fell inside the window; the first attack markers are at 3533 and 3538. Frames (`pstun-sheet3.jpg`):
dagger-ready stance before (3320-3338), crouched slumped pose with the daggers lowered during 3342-3460, stance again at 3500/3520.

**Player SCARE, `...\w3p-pscare-1` (exe 4).** `start ... kind=scare ... stateAfter=8 frame=3337`, `end ... frame=3493`. Frames (`pscare-sheet.jpg`): mid-stride running pose with the
daggers at the sides during 3342-3460, back to the stance at 3500.

**Player KNOCKBACK, `...\w3p-pknock-1` (exe 4).** `[push] KnockBack target=18446744073709551615 attacker=11259224883678223927 source=test-apply-status great=1 ... stance=2 frame=3337`,
`[test-status] knockback ... stateBefore=3 ok=1 stateAfter=10`. Frames (`pknock-sheet.jpg`): the player is thrown backwards, lies on the floor at 3420 and is back on the feet at 3460-3500. This is the existing Push consumer
(unchanged), run for the player for the first time this wave.

**ViciousStrike again on the final merged exe (exe 6), `...\w3p-vic-3`.** Cast 3337; `start` for both lizards at frame 3371 (`stateBefore=4` and `stateBefore=5`, `leftMs=2184`),
`end ... stateAfter=3 frame=3508` for both; the second cast (`frame=3673`) stuns the surviving lizard again (`start ... frame=3681`), then the same pre-existing
`Foundation error: ... Source result identity/target eligibility rejected`. Frames (`vic3-sheet.jpg`, every 6 frames): upright claws-forward attack stance at 3326, a static hunched stunned pose at
3428, 3452, 3476, 3500 (head down, back arched), normal again from 3512.

**Monster SCARE, `...\w3p-nscare-2` (exe 6, pose trace) and `w3p-nscare-1` (exe 4).** `[test-status] scare ... ms=2500 frame=3336`, `start ... kind=scare ... stateAfter=8 frame=3337`,
`trace ... state=8 clip=monster-source-status-Swamp_LizadMan_Type1/source-skill-sequence-383/phase-0 ms=41 frame=3340` (the authored looping `lizardman_walk_01` leaf),
a hit at 3354 sends it to Injured, the scare is re-raised at 3420 (`leftMs=1156`) and the lizard is killed at 3444 while scared. Frames (`nscare2-sheet.jpg`): the lizard
walks in place in a forward-bent gait (3344-3350, 3422-3440). The walk clip is itself a bent-over lizard gait, so the frames alone do not prove the scare pose against the normal
walk; the pose trace is the proof, hence PARTIAL.

## 5. Uncertainties and gaps (not turned into rules)

1. `lizardman_post_stunned` is never played: the only exit found (`CSStunned::OnUpdate` -> `SM_SetIdleState`) goes straight to Idle and no `ANIM_StopLoop` is called for stun. If the original
   plays it, the trigger was not found.
2. Attack5 admission (`CSM_Interrupted` reads byte +0x441) is an inference: the writer of that byte is not in the IDA export (the only `#0x441` hits are the read; the `#0x440` stores are NetStruct vtables). Skill6 is modelled
   (needs flag bit 16, never set by the port's skill flags 0x6341, so a stun waits for the skill to end, same as the original re-raise). KnockedBack10 stays refused for stun.
3. Scared8: the original runs the actor with a random `Cmd_HeadTowards`; the port has no heading owner for it, so the scare pose is pose-only (root motion of the walk leaf suppressed) - a scared monster plays the authored walk in place.
   Type2 scare roots (moth) use the first authored child.
4. `can_act` gates only `RuntimeEnemyControllerV1`. Ranged-skill monsters (`RuntimeRangedSkillMonsterV1`) and the boss AI owner are not gated, so such a monster stunned mid-cast still ticks (bosses are immune anyway).
5. While a stun/scare pose is active the Session checkpoint refuses (`Active source skill/spell playback is not persisted by the checkpoint`), exactly as for a skill or knockback in progress; durations are 2-3 s.
6. Player stunned while Moving (4) is not verified (2d). The Blur(4) path is the same as for a moving player that is knocked back.
7. Potion use while stunned was not checked (the gate covers attack, skill keys, spell key, target select and movement input).
8. No original gameplay footage was available for these clips; "differs from idle, is a slumped stagger / flee run" is a visual judgement of the authored clip names (`*_pre_stunned`, `*_stunned`, `prince_feared`), not a comparison with the original.
9. The pose trace and `[status-pose]` lines are diagnostics, kept in the log (one per 20 frames per posed actor).

## 6. Test status

Final tree (after merging `windows-foundation`): **218/218 passed**, `ctest --test-dir C:/dh218/w3-pose-build -j4` with the llvm-mingw bin on PATH, 633 s, log
`C:/dh218/w3-pose/ctest-final.log`. (A first full run on the same tree was 217/218: only my new `status_states_pose_session_v1` failed because its CTest asset root pointed at the
`v19-preview-12` package, which lacks the stun clips; it now uses the staged3 assets and passes.) No wall-clock flakes occurred. The new test needs
`.local-inputs/windows-source-clock-v21.8-staged3/assets` (local-only, like the other asset-backed tests).

## 7. Where things are

- Runs and logs: `C:\dh218\act1rogue\cases\w3p-*` (`w3p-vic-2`, `w3p-vic-3`, `w3p-pin-1`, `w3p-nstun-1`, `w3p-nstun-2`, `w3p-seam-pstun`, `w3p-pscare-1`, `w3p-pknock-1`, `w3p-nscare-1`, `w3p-nscare-2`, `w3p-pmove-*` (the failed moving-player attempts)).
- Drivers and the contact-sheet script: `coordination/v21.8/w3-pose-evidence/` (`run-vicious.sh`, `run-seam.sh`, `run-pin.sh`, `sheet.ps1`) and `C:\dh218\w3-pose\`.
- IDA: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c` (SM_SetStunState 142855, SM_SetScareState 142895, CharStateMachine::Update 142934,
  CSStunned::OnFocus 141721 / OnBlur 141678 / OnUpdate 142438, CSScared::OnFocus 142003 / OnUpdate 142064 / OnBlur 142081, OnInit 144620/144634, CSM_Interrupted 127007, SM_SetIdleState 140652).
