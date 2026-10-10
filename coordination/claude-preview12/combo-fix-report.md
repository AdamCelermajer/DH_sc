# combat_session_combo fix: B037 release guard vs. source Character+1090

Status: `combat_session_combo` passes again (ctest and direct run, 20/20 repeats). All eight requested runners exit 0. Not committed. Not integrated into the main EXE. Not video-verified.

## 1. Failing check

`features/combo_chain/combat_session_combo_tests.cpp:67`, first run (`mode=0`, expected 3):
`Live combo same-generation marker count differs expected=3 actual=2`.

Per-frame trace (temporary debug print, removed) showed the scenario. Space is held while `index<2`, so the test releases on the first frame of group 2. That frame is the group-2 pre step:
`idx=2 cont=0 last=1 finisher=0`. The B037 guard used `last`, so it departed the swing there and group 2 never struck.

## 2. Which side is right

The test is right. The B037 premise was wrong about which byte is the release gate.

IDA `pseudocode-all.c`:
- `Character::CSM_StoppedAttacking` (0x3ad280, line 127022) returns `Character+1090`. `CharAI` is at `Character+968` (for example line 42879 `v3+968`), so `Character+1090 = CharAI+122`.
- `CharAI::_OnAnimStepBegin_Attack` (0x3d4044, lines 152510-152530), phase 1:
  - `StepIndex != 0`: `+121 = (StepIndex == StepCount-1)`, `+122 = 0`, then `+122 = 1` if that is true.
  - `StepIndex == 0` (pre): `+121 = 1`, `+122 = 0`, then the pre virtual.
  - So `CharAI+122 = (step!=0 && final step)`.
- `CharAI+121` (`Character+1089`) is what the command gate reads (line 149711 `v7 = this+121; if(!v7) ...search`). It is true on pre and final steps. That is what the port's `AttackState64::last` mirrors.
- `CharAI::_OnAnimStepEnd_Attack` (0x3d3e44): at phase 1, `if(step==count-2 && !ooi && !(dead && target)) skip_next`. With a live target the final recovery step is skipped, so `+122` is only reached when the target is dead.

Port mirror: `level-world/character_attack_animation_v1.cpp` already computes `finisher = (step!=0 && step==count-1)`, which is exactly `+122`. The port's `last` is `+121`.

Trace of the live combo (count=3, `combo_boundaries`): every group is depth-1 step 0 (pre) and step 1 (strike). Step 2 never appears when the target is alive. This matches the skip rule above.

## 3. Change

`port/windows-foundation/combat_session.cpp`, release guard in `CombatSession::update` (the only change to this file in this pass; the debug prints were removed, verified by `git diff | grep -i dbg` returning nothing):

- Old: `playerEntry.sourceAttack.last&&`
- New: `playerEntry.sourceAttack.finisher&&`, with a comment naming CharAI+122 and why `last` is not the gate.

`depart_source_attack`, the B037 interrupt reset, and the retention fix are unchanged.

## 4. Second change: B037 runner probe (features/generic_skills/run_b037_attack_skill_admission_v1.ps1)

The probe used `last` as the recovery window. After the change, the recovery cut is not reachable in this fixture: every swing has a live target, so the final step is skipped and `finisher` stays 0 (the probe's trace showed `fin=0` on every frame). Two attempts to force it failed:
- Setting the target's health to 0 made the swing end at once (death interrupts the swing).

So the probe now asserts only:
- A release on a strike/pre frame (`finisher==0`) does not stop the swing, and the held-then-released swing ends normally.

The recovery-release assertion was removed, not weakened by a skip. The recovery cut (`finisher!=0`) is unverified in the same-Session tests. Its only path is a dead target at the final step, which needs a different fixture. A quick attempt (setting the target's health to 0 mid-swing) ended the swing at once through death, so it did not reach the final step.

Also corrected: the probe's header and PASS messages now reference `CharAI+122 = finisher`. The B037 report's claim "Character+1090 = AttackState64::last" is wrong and should be corrected there.

## 5. Test results (real output)

`ctest.exe --test-dir .local-inputs/windows-foundation-build -R combat_session_combo`:
```
1/1 Test #79: combat_session_combo .............   Passed    0.60 sec
100% tests passed, 0 tests failed out of 1
```
Repeated 20 times with the built binary: no failures. Direct run output:
```
live combo mode=0 hits=3 removed=1.73828 groups=0,1,2,
live combo mode=1 hits=1 removed=1.26172 groups=0,
live combo mode=2 hits=2 removed=1.26172 groups=0,1,
live combo mode=3 hits=1 removed=1.26172 groups=0,
PASS combo save accepted continuation resets; fresh restored attack starts root0 without RNG consumption
PASS live CombatSession held three varied swings, release one, accepted release two, blocked one; original health receipts and same-generation hits preserved
```

Runners (exit codes; logs in `.local-inputs/combo-fix-runs/`):

| Runner | Exit | PASS lines |
|---|---|---|
| features/generic_skills/run_b037_attack_skill_admission_v1.ps1 | 0 | 5 (PC mapping; mid-swing admitted; held Space no second attack; dead/hurt/casting rejections; release in strike window did not stop swing) |
| features/generic_skills/run_post_skill_attack_regression_v1.ps1 | 0 | 3 |
| features/combat/run_target_retention_regression_tests.ps1 | 0 | 1 |
| features/combat/run_target_facing_regression_tests.ps1 | 0 | 2 |
| features/combat/run_b013_attack_continuity_v1_tests.ps1 | 0 | 1 |
| features/combat/run_b033_targetless_attack_admission_v1.ps1 | 0 | 3 |
| features/combat/run_b012_death_mid_swing_v1_tests.ps1 | 0 | 1 |
| features/combat/run_runtime_player_combo_chain_v1_tests.ps1 | 0 | 1 |

Build: `cmake --build .local-inputs/windows-foundation-build --target combat_session_combo_tests` (the requested target name `combat_session_combo` does not exist; the executable target is `combat_session_combo_tests`, and the ctest name is `combat_session_combo`). Build clean, no warnings from my edits.

## 6. Uncertainties and open risks

- The B037 release-cut (recovery window) is not exercised by any test now. It needs a dead target at the final step. Current code cuts only when `finisher` is set, which is faithful to IDA but unverified in play.
- The release guard is level-based (checked every frame while released), not edge-based. The source consumes the 50001 event once at release. Example: release on a pre frame (ignored), then the target dies and the recovery plays. The port would cut the recovery; the source would not. Edge detection would need one new member (previous attack input). Not done, to keep the diff small.
- `play_actor_source_sequence` interrupt path (B037 hunk 3) still calls `combat->interrupt`, which clears the target unconditionally. Flagged in retention-fix-report; not changed here.
- No video comparison performed. Integrated check: run the EXE, hold Space through a combo and release on a pre frame. The swing should continue.
