# mudlord3 report: Mud Lord (SwampKing) B158 / B159 / B160, branch v21.8-mudlord3

Worktree: `C:/Users/adamc/Desktop/workspace/DH_sc/.claude/worktrees/mudlord3` (subst `Z:`, one junction `Z:\.local-inputs` to the primary `.local-inputs`, removed at the end with `cmd /c rmdir`). Base: windows-foundation `0eea5e65`. Build dir `C:/dh218/mudlord3-build` (Release, llvm-mingw). Harness: `C:/dh217/tools/run-case.ps1` with the continuous-run scripts `C:/dh218/tools/run-c-follow.ps1` and `run-c-follow300.ps1` (outside the repo).

## Original investigated first
- `swampking_core.luac` in `.local-inputs/assets-extra/android/data/scripts/ai/` is plain Lua source (not bytecode). Read in full.
- Dive (`SetSwampkingInDiveState`): `SetIsTargetable(false)`, `PlayAnim(swampking_dive)`, dive sound. On the dive clip end (`swampking_OnEndOfAnim`): `searchForEfficientTarget()` (nearest Player inside RANGED_RANGE, `AddAggro` 10, `LookAt`), `MoveToPosition` after 500 ms (`CheckForRandomSpawnPoint`: uniform pick from the 5 spawn points, the first one when weak under 25%), emerge after `DIVE_DELAY` 1000 ms (`DoSkill(3)`), `TargetAgain` (`SetIsTargetable(true)`) 3000 ms after the dive.
- The script never calls a move, flee or approach operation. Its range callbacks are OnTargetInCloseRange and OnTargetInRangedRange only.
- Range events come from `CharAI::_UpdateTarget` (IDA `pseudocode-all.c` line 146525, 0x3cb908): 14 out of range, 15 ranged (600..6000 for this boss), 16 close (under 600). Port mapping `ranged_range_event_v1` agrees.
- `MarkAsFlying(true)` sets PFObject flag bit 0 (`GameObject::_MarkAsFlying` 0x390900, `PFObject::SetFlying` 0x5241f4, `+20`). The port does not model the flag (B155).

## B158: relocation, aggro, targetability, and the continuous kill
Findings (log evidence in the cases named below):
1. Aggro and targeting match the script. After the dive the boss's target is the hero. The hero's ActorId is `18446744073709551615` (a valid id, not `invalid_actor_id` which is 0), so the `DoSkill ... target=18446744073709551615` lines are the hero, not an invalid target. `SetActorPosition` to `_prim_BossSpawnPoint_03/02/05/04` uses the same waypoints as the script. `SetIsTargetable` false/true follows the dive and the 3 s retarget.
2. The boss drifts away from the hero, and the drift is not clip root motion. The boss's `motionRoot` is cleared (`main.cpp` near line 2256, MarkAsFlying not modelled) and the motion receipt reads `sourceXY=0 worldXY=0` for the boss (`C:/dh217/cases/m3f1/run.log`, "Motion final actor=14582633645715090783"). The generic enemy controller skips the boss (`runtime_enemy_controller_v1.cpp` line 401, AI script must be `monster`). Control run `C:/dh217/cases/m3d2` (hero standing still at the fight spot, no follow aid): the boss moves north from y 11570 to 11722 (distance 182 to about 355) and stops; the hero stays at -19369,11390.
   INFERENCE (not proven): body contact between the boss body (radius 224.06) and the hero (radius 65.45, run log "Ground controller ... radius=65.4533") pushes the boss out to about 290 units. The original rule that stops a Mud Lord from being pushed by the player has not been identified.
3. The follow aid at distance 170 (`run-c-follow.ps1`, m3f1) logs `placed=1` but the hero does not move: the hero stayed at -19497,11379 for about 600 frames while the placement asked for -19514,11573. Placements inside the body contact were not applied and the boss kept pushing away. Fixed in code: `--follow-actor` refuses distances under 290 (`kFollowContactDistance`, `features/test_aids/follow_actor_aid_v1.hpp`). Test `follow_actor_aid_v1` extended (170 and 289.9 refused, 290 accepted).
4. Continuous runs from the afterwitch seed, real keys, `--follow-actor SwampKing@2960-6400:300`:
   | case | result |
   |---|---|
   | m3f2 | boss killed at frame 5407 (`Damage frame=5407 ... dead=1`); the exit teleport was at 6500 (before the kill), so no Darkwood load in this run; quest banner at 7556 |
   | m3f3 | hero died at about frame 6513 with the boss at 254.8/900 HP; no kill |
   | m3f4 | boss killed (`[boss-ai] died actor=14582633645715090783`, log line 6390); `Quest banner kind=QUEST COMPLETED row=50` (line 7569, sound frame 7520); `Exit zone entered=_prim_ExitLevelZone_001 target=DARKWOOD frame=7800`; `Transition character loaded ... 003_darkwood.mlx` (line 8119); 0 Foundation errors |
   | m3f5 | boss killed (`[boss-ai] died`, line 6387); `Quest banner kind=QUEST COMPLETED row=50` (line 7584); `Exit zone entered=_prim_ExitLevelZone_001 target=DARKWOOD frame=7800`; `Transition character loaded ... 003_darkwood.mlx` (line 8134); 0 Foundation errors (same script as m3f4, confirmation run with captures) |
   | m3f1 | distance 170, not kill (the placement problem in item 3) |
   Frames viewed: `C:/dh217/cases/m3f5/f7600.png` (quest banner "Quest Completed / Escape the swamp with Rene and Celeste. / Reward 100 EXP"), `C:/dh217/cases/m3f5/f8200.png` (Darkwood map, Celeste dialogue "ada? What's wrong?"), `C:/dh217/cases/m3f5/f5400.png` (fight), `C:/dh217/cases/m3f4/f6800.png` (Rene outro after the boss death), `C:/dh217/cases/m3f4/final.png` (Darkwood arrival). The chain runs in one process from the afterwitch seed through the lever, intro and fight. It is not deterministic: with the same 300-distance script, 3 of 4 runs killed the boss and 2 of 4 (m3f4, m3f5) reached the Darkwood load in the same run; m3f3 lost the hero.
5. Caveats: the hero is moved by the `--follow-actor` teleport aid (same owner as `--player-to`), not walked. The bot does not use skills or Tab. In m3f4 frames 4430 to 4730 the placements did not move the hero, yet the hero still landed hits from frame 4980 to 5401. The boss drift in item 2 is the reason the aid is needed; the original rule was not found.

Status B158: PARTIAL. A continuous kill to the Darkwood load is shown in two runs (m3f4, m3f5) with the teleport follow aid. The hero walking to a relocated boss is not solved; the drift cause in the original is not identified.

## B159: damage calibration and AOE pushback
- Level: `swampking_OnInit` clamp applied (mudlord2 b1). Calibration: live resolved sheet, class applied in place (mudlord2 b2), and the AOE push class applied by `SwampKingSkillAOEPushback` (mudlord2 b3).
- Formula check: `runtime_swampking_damage_calibration_v1` (unit, in CTest) passes all 72079 checks in this branch's build. It recomputes the CF__CalcDamage range independently (type 0 isSkill, type 2 spit), checks every landed hit lies inside it, checks coverage of the range, and checks the push-outcome rates. Expected values recorded by mudlord2 at level 2 vs the Knight 169.195 HP: bite SwampKingAttack1 65.6..81.0, bite 2 70.6..86.0, emerge AOE 55.0..65.4, spit 46.2..56.6. This session did not re-measure these in a real run.
- Status B159: WORKED (unit test; real-run push evidence from mudlord2 m3s4/m3u3). Not re-measured on this branch.

## B160: awake latch
- NOT CLOSED. The original latch is the camera frustum (`Character::CanUpdate` 0x3a52a4, `TestCullingBeforeUpdate` 0x33de90). The port latches on ViewRadius 3500. Building a frustum owner was not in scope for this pass. The continuous kill runs awake at ViewRadius before the fight, so no run depended on the difference. `resetonhubchange` is still not modelled. Status: DID NOT WORK (not attempted, reason above).

## Tests
- Baseline on the untouched base in this worktree (`C:/dh218/mudlord3-ctest-base.log`): 197/199. Two pre-existing failures:
  - `original_melee_bindings`: "Original binding table counts changed". The test reads `.local-inputs/windows-melee-bindings/original-melee-bindings.xml` through the shared `.local-inputs` junction. That file was rewritten at 16:23 today (shared input drift, not from this branch; no code change yet).
  - `runtime_skill_activation_session_v1`: "Skill request CharacterState does not identify this retained session actor" (Rogue session identity). Not touched by this branch.
- After the follow-aid change (final, second full run `C:/dh218/mudlord3-ctest-final2.log`): 197/199 pass. The same two failures, nothing new. `follow_actor_aid_v1` passes.
- A first full run (`mudlord3-ctest-final.log`) also reported `schema_v4` failed under `-j4`. It passed alone and in the second full run, so it is a parallel-run flake, not a branch change.
- Final CTest: 197/199 (2 failures, both pre-existing on this base as described above). The baseline of 199 is not reached in this worktree because of the shared `.local-inputs` drift in `original-melee-bindings.xml`, which this branch did not cause.

## Files changed
- `port/windows-foundation/features/test_aids/follow_actor_aid_v1.hpp` (contact-distance floor 290, B158 finding 3)
- `port/windows-foundation/features/test_aids/follow_actor_aid_v1_tests.cpp` (boundary tests)
- `docs/BUGS-AND-IMPLEMENTATION.md` (B158, B159, B160 rows)
- `coordination/v21.8/mudlord3-report.md` (this report)

Harness note: `C:/dh218/tools/run-c-follow.ps1` passes `:170`, which the executable now refuses at parse time. Use `run-c-follow300.ps1` (`:300`) or `run-c-follow300-cap.ps1` (adds captures at 5400/7600/8200).
