# w3-status report (wave 3, ES-STATUS follow-up): branch `v21.8-w3-status`

Base: `windows-foundation` @ 3e5d6dca (no newer commits there at merge time, so the merge step was a no-op).
Worktree: `.claude/worktrees/w3-status`. Subst `V:` (released at the end). Build dir `C:/dh218/w3-status-build`.
Package for runs: `.local-inputs/windows-source-clock-v21.8-staged3` (worktree junctions, 936 dirs).
Exe used for real runs: `C:/dh218/w3-status/dh-foundation-w3-status-1.exe` (same tree as the commit, comment-only edit after it).

## Summary

- Item 2 (knockback admission) is fixed generally: the non-direct push gate in `CombatSession::play_actor_source_sequence` now follows the original event-50011 admission table instead of "Idle or Injured only". Real run: the archer SideStep knockback on an attacking lizard (state 5) is admitted at the first check; the same case before the fix was refused 7 times.
- Item 4 (stale log string) removed; the push log line now prints the target's pre-hit state.
- Item 1 (stun pose) and item 3 (player stun/scare/knockback) are NOT implemented this round. IDA evidence for both is recorded below with the concrete next steps.
- Full CTest 216/216 green (`C:/dh218/w3-status/ctest-full.log`).

## 1. Stun/scare pose (item 1): DID NOT WORK (not implemented)

Original (IDA `pseudocode-all.c`, read this round):
- `CharStateMachine::SM_SetStunState` 0x3c5ffc: IsBoss refused; on the first stun it starts timer 43 (`CharTimers::TMR_Start`) and sets value 2 in the state-machine word at +44; clip index = `CharAnimTable` dword +140; stance bit 0x200 (`AnimStancedAnim`); `_SetState(9, 50012)` (or `RaiseStateEvent(50012)` when the 5th argument is false); sets the stun stance flag bit 0x800 when `a3` is nonzero.
- `CSStunned::OnFocus` 0x3c3cc0 (the state entry): sets the Focus flag dword `+0x520` (`*(a3+328)`) to `0x2202`; `CharAnimator::ANIM_Set(a3+1180, stanceOffset + CharAnimTable[+140])`; for a Player (vtable +40) sets `*(*(a3+888)+8) = 1` (the input lock; `CSStunned::OnBlur` 0x3c3b4c clears it and pins the physics object again); `CancelSneaking`; unpins the physics object.
- Event 50012 is registered with the `CSM_Interrupted` handler in several states (for example Idle, Move, Scared and Injured in the OnInit functions); the exact per-state table was not extracted this round.

Port state: `ANIM_Set`-style one-clip playback does not exist for an actor; the only path is the source-sequence machinery used by Push. A stun bank would be built like `build_session_push_animation_bank_v1` with field 32 (stance bit 0x200, state name "Stunned"). Playing it needs: a generic `original_state=9` policy in `play_actor_source_sequence` (its own admission, state_flags 0x2202), a `case 9` in the body checkpoint flags switch in `main.cpp` (~3047), a transition-handler check for `to=9`, and a status owner hook that starts the clip on stun and cancels it on expiry (`cancel_actor_source_sequence` to Idle). Not done: the session state machine change is shared and would need its own real-run verification.

Visual: no original footage was used for this item (not compared with the original).

## 2. Knockback admission (item 2): WORKED for non-direct pushes (real run)

Original evidence:
- `CharStateMachine::SM_SetKnockBackState` 0x3c5ea0: refuses only a Boss (`AiProps` flag bit 2 via `IsBoss`) and an invalid CharAnimTable row. It does NOT test Idle or Injured. It sets the pose pointer (great: +36 with stance 0x800; normal: +72 with stance 0x400) and then calls `_SetState(10, 50011)` when the 4th argument (direct) is nonzero, or `RaiseStateEvent(50011)` otherwise. A non-direct push is therefore admitted by the target's current state table.
- Event 50011 is registered by SM_RegisterEvent (0x3c7b18) in Spawn1, Idle3, Move4, Attack5, Skill6, Scared8, Stunned9, Injured11, Interact13, Reviving15, LiftingIdle18, LiftingMove19 (IDA OnInit functions 0x3c7d0c .. 0x3c8ee0).
- `Character::CSM_Interrupted` 0x3ad244 is the handler for several of these. Its asm (`full-listing.asm`, around 0x3ad244) compares the state id: 6 reads bit 16 of dword +0x520; 10 reads bit 5 of +0x528 (inverted); 5 (Attack) reads byte +0x441; any other state returns 1 (accepted).
- F_ApplyResult 0x3b10b4: the stun, scare and knockback tail is gated on `Character::IsDead` (vtable +52), not on IsPlayer (vtable +40). So monsters take the tail too.

Port change (`combat_session.cpp`, `combat_session.hpp`, `features/combat/b035_session_push_effect_v1.{cpp,hpp}`):
- The table `source_knockback_event_accepted_v1` (1,3,4,5,6,8,9,11,13,15,18,19) moved from the b035 module into `combat_session.cpp` (namespace `dh::foundation::combat`), so the session can use it. The b035 declaration remains.
- The play gate changed from `state!=3 && state!=11` to `!source_knockback_event_accepted_v1(state)` for non-direct requests. Direct requests (`mask & 0x18000000`) are unchanged (forced `_SetState`).

Uncertainties (stated, not turned into rules):
- Attack5 and Skill6 are admitted without their CSM_Interrupted flag guard (+0x441 and +0x520 bit 16). The writers of +0x441 are not in the IDA export (one `LDRB` in the listing, no store). So the guard is unknown; the admission for these states is an inference, and the real run is the only evidence for it.
- KnockedBack10 (state 10) is still refused for a non-direct re-knock; the +0x528 bit-5 guard is not modelled.
- No original gameplay footage of a SideStep knockback exists in the repo; visual comparison with the original is not available.

Real runs (hidden window, real keys, `coordination/v21.8/act1rogue-run-rogue2.ps1` with the staged3 package):
- **w3s-side-1** (archer SideStep rank 10 via `--test-skill-rank`, seed `C:/dh218/seeds/w2-archer-MultiHitArrow`, lizard zone `-2900,-100,255@700`, same clicks as `w2-archer2/cases/p4-side`). Log `C:/dh218/act1rogue/cases/w3s-side-1/run.log`.
  - `[push] KnockBack monster target=11259224883678223927 profile=Swamp_LizadMan_Type1 ... source=SideStep/deflection great=1 direct=0 state=5 outcomes=0x80 mask=0x100000 frame=3339`
  - `Archer passive SideStep check attacker=11259224883678223927 knockback=requested stateAfter=10` (the first check, no retry)
  - Further pushes on attacking and idle lizards: frame 3543 (state 3), 3658 (state 5), 3874 (state 5), all `requested`.
  - Count over the run: `knockback=refused` 0, `requested` 4. Baseline (`w2-archer2/cases/p4-side/run.log`, before the fix): `refused` 7, `requested` 9; the baseline's first refusal at frame 3338 had `stateAfter=5`.
  - Frames viewed: `f3340.png` (lizard at its red ring, "Block" text) and `f3350.png` (the same lizard has moved up and away from the ring). This shows displacement (inference from two frames 10 apart). The KnockedBack pose itself was NOT captured in a dense series.
- **w3s-jk-3** (JumpKick, Rogue, `rogue-jumpkick` seed): `[push] KnockBack monster target=4660011647479645029 ... direct=1 state=5 outcomes=0x80 mask=0x8335535 frame=3731` (an attacking lizard, direct path, so the gate change is not exercised by this line; it confirms the direct path still admits state 5). `w3s-jk-1` and `w3s-jk-2` had no push roll (0x0 outcomes at the second volley).

## 3. Player's own stun/scare/knockback (item 3): DID NOT WORK (not implemented)

- `StatusStateSessionV1::on_outcome` still skips player targets (`player_skipped`). Nothing gates player input on a status.
- Original: the player's input lock is `*(*(a3+888)+8)` set in `CSStunned::OnFocus` and cleared in `OnBlur` (see item 1). Not modelled in the port.
- Act 1 sources: none found for player stun or scare. The lizard, Swamp King and Witch AI scripts (`assets-extra/android/data/scripts/ai/`, `level/`, `objects/`) do not call SetStunState, SetScareState or SetKnockBackState; the monster status comes only from skill-row outcome bits. The Swamp King AOE push is a player knockback through the existing Push consumer (`sourcePushSink`, player branch), which was not re-run this round.
- Player knockback through the Push consumer is unchanged by this round except that its admission now uses the same table (a player target in state 5 or 6 is now admitted for non-direct pushes, same as monsters). Not run.

## 4. Stale log string (item 4): WORKED

- `main.cpp`: removed "stun/push have no host state, only hurt drives a reaction" from the `Source skill hit` line (the bits list is kept).
- `[push] KnockBack monster ...` now prints `state=<pre-hit original state>`; used as evidence above.
- Check: `grep "have no host state"` over `w3s-side-1/run.log` and `w3s-jk-3/run.log` returns 0 lines (the hit lines are present in both logs).
- Stale comment in `b035_session_push_effect_v1.hpp` ("gates the status block on the defender's pre-effect Player/Idle state") corrected: the tail is gated on IsDead only.

## 5. Tests

- Targeted CTest after the admission change: `b035_session_push_effect_v1`, `combat_session`, `combat_session_source_departure`, `combat_session_actor_transition`, `source_session_program`, `session_skill_binding`, `runtime_skill_activation_session_v1`, `status_states_v1` and the other combat/session tests: 19/19 passed.
- Full CTest on the tree before the final comment-only edits: **216/216 passed** (`C:/dh218/w3-status/ctest-full.log`, 700 s). After the comment-only edits to `b035_session_push_effect_v1.hpp` and the log string, the build is OK and `b035_session_push_effect_v1` + `combat_session` pass.
- No new unit test was added for the gate itself: the existing `b035_session_push_effect_v1` test covers the table (accepted 1,3,4,5,6,8,9,11,13,15,18,19; rejected -1,0,2,7,10,12,14,16,17,20,99). The gate is covered by the real run above, not by a unit test (a fixture with an attacking target would need a new session setup).

## 6. Remaining gaps (for the lead)

1. Stun pose and the stun clip (item 1): needs the state-9 generic path, the body checkpoint case 9 (flags 0x2202), and the status owner hook (section 1).
2. Attack5 flag guard (+0x441 writer unknown) and Skill6 guard (+0x520 bit 16): unresolved; the admission for these states is an inference. The writers should be found in the IDA listing (store to `#0x441`, possibly via a register offset) before this is called original behaviour.
3. KnockedBack10 re-knock guard (+0x528 bit 5): not modelled (refused).
4. Player stun/scare and the input lock: not wired (item 3).
5. MultiHitArrow knockback (the other non-direct case in the brief): not run this round; the archer seed exists in `C:/dh218/seeds/w2-archer-MultiHitArrow`.
6. Pre-existing (not this round): `Source result identity/target eligibility rejected` on a second ViciousStrike volley to a dead lizard (es-status report item 7); not reproduced here.

## 7. Where the evidence is

- Runs: `C:/dh218/act1rogue/cases/w3s-side-1/` (log, frames f3330..f3380), `C:/dh218/act1rogue/cases/w3s-jk-1..3/`, baseline `C:/dh218/w2-archer2/cases/p4-side/`.
- Runner and drivers: `coordination/v21.8/act1rogue-run-rogue2.ps1`; `C:/dh218/w3-status/run-side.sh`, `run-jk.sh`.
- IDA: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c` (SM_SetKnockBackState 142800, SM_SetStunState 142855, CSM_Interrupted 127007, CSStunned::OnFocus 141721, OnInit registrations 144504-144700); `full-listing.asm` (CSM_Interrupted at the `ANIM` `LDRBEQ #0x441` line 473064).
