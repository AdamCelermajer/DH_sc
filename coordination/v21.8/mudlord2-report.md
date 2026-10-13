# mudlord2 report: Mud Lord (SwampKing) finish-up, branch v21.8-mudlord2

Worktree: `C:/Users/adamc/Desktop/workspace/DH_sc/.claude/worktrees/mudlord2` (base commit 93f4823f).
Build dir: `C:/dh218/mudlord2-build` (Release, llvm-mingw). Subst: `U:` (see deviation below).
Harness scripts (outside the repo): `C:/dh218/tools/run-d-darkwood.ps1`, `run-k-kill.ps1`, `run-k-chain.ps1`, `run-c-afterwitch.ps1`.
Exe for real runs: `C:/dh218/mudlord2-after.exe` (sha256 26D66BBD15088AB3787609439EA74F8489E9013959FDC089B501392CCA6EA15A).

## Deviations and process notes
- `D:` could not be used for subst: it is a real 2 TB fixed volume (`big-chungus`). `U:` was used instead.
- The two Sonnet investigations (damage formula, trigger persistence) were stopped by the coordinator. Both were redone by reading the local IDA pseudocode dump (`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`). No sub-agents are running.
- Worktree `.local-inputs` holds 929 junctions to the primary's directories (ignored by git). They must be removed with `cmd /c rmdir` before any worktree delete.

## Test counts
- Baseline at 93f4823f (before edits): 187/187 pass.
- After the changes: 187/187 pass (full CTest, same build dir).

## Item #4: saveLevelWorld persists campaign trigger state. WORKED (unit + inference)
- Original (IDA pseudocode dump): `Trigger::Serialize` (0x398978) writes the delay (+952) and the activation count (+948). `Trigger::CanActivate` (0x3987ac) blocks once the count reaches `triggercount` (+936, default 1, set in the constructor at 0x398f44). `Trigger::Activate` (0x398794) increments the count. So the fired state is part of the saved object state.
- Port: `original_campaign_runtime.cpp:124` and `campaign_host.cpp persist_lifecycle` write the same counts into the world save. `saveLevelWorld` is the existing save path for transitions and quicksaves, so the new persistence matches the original save rule. INFERENCE: the original restores the level object state on revisit (the port's LevelSavegame rule, P17).
- Test added: `fired_once_trigger_stays_fired_across_level_world` (campaign_host_tests.cpp). It fires a default once-zone, persists, restores into a fresh session, and checks the count is restored and the zone does not re-fire. Passes.
- A persist failure in the transition path throws `Level world save: ...`, the same pattern as the existing save failure in that block. Kept.
- Open: the `resetonhubchange` property (Trigger::DeclareProperties 0x398a88) is not modelled (B160).

## Item #7: empty-clip admission narrowed. WORKED (unit)
- `animation_markers.cpp`: `load()` again rejects `end <= start` for every track. The new `load_no_keys()` accepts only a track with zero keys. `has_no_key_sentinel()` checks the BRES root words +28/+32 for INT32_MAX/INT32_MIN.
- `original_character.cpp`: an empty clip is admitted only when `allow_missing_animation_targets` is set AND it has no segments, no unbound and no skipped targets AND the no-key sentinel. Any other empty clip is rejected again, as before B155.
- `retained_animation_owner.cpp` (sentinel + zero range) was left as is: it needs the sentinel to match.
- Tests: `animation_markers_tests` checks zero-length rejected by `load()`, no-key accepted by `load_no_keys()`, keyed track rejected, and with argv[2] the real `swampking_hurt_01.bdae` has the sentinel and `swampking_attack_01.bdae` does not (CMake passes the data root). Passes.
- Real run m2c1: 0 `Actor marker track` / `has no bound tracks` / `Empty authored clip` errors.

## Item #8: select_actor_state_leaf nested suppression. WORKED (narrowed; unit)
- Change: the runtime interruption is suppressed only when the actor is dead (`find_actor(id)->is_dead()`), not for every caller. Living actors keep the runtime's own interruption prefix (as before B155). Interrupt failures are returned, not dropped (before B155 `interrupt(id)` ignored the result).
- Caller trace:
  - `main.cpp:3808/3826/3830` (lifecycle state selection): always pass `request.state` (>= 0). With a bound transition handler the admitted Blur/publish/Focus prefix is delivered before the interruption, so only the redundant nested prefix is removed, and only for dead actors.
  - `features/companions/source_companion_campaign_animation_v1.cpp:238`: default `lifecycle_to_state=-1`. With a handler bound this already fails ("Legacy lifecycle leaf selection..."). Without a handler the suppression is inert (`transition_state` returns -1), so only the error propagation applies.
  - `tests/combat_session_tests.cpp`, `tests/source_session_program_tests.cpp`, `features/generic_skills/...` pass the default -1 with no handler: suppression inert.
- Observation, not changed: `play_actor_clip` (combat_session.cpp, near 2314) and the blocks near 2263/2396 still suppress nested transitions for all callers. This predates B155.
- Test added: `run_dead_select_no_nested_interruption` (combat_session_actor_transition_tests.cpp). Kills the player (retained Died pose), selects Idle with lifecycle state 3, and checks the admitted transition is delivered and no interruption-cause receipt is. Passes (CTest combat_session_actor_transition passes with the new section).
- Gap: the living-actor path is not covered by a new test.

## Item #10: timer traps. PARTIAL
- Never-expire case: FIXED by refusal. `validate_timer_trap_definition_v1` (runtime_timer_trap_v1.cpp) requires the script's count event to be authored inside (activate_start, activate_end]. It runs in main.cpp before the definition is cached, so a bad trap cannot be summoned. Tests: the authored steam cloud validates; a model without the event, an event at activate_start (offset 0 never counts), and a script with no count event are refused. Passes. The real trap still expires after 6 activations (existing test).
- `Do_RangedAttackPhase1` roll: NOT a bug. `swampking_core.luac` lines 246-255 call `local prob_pct = GetRand(1,100);` and never use it. The port's draw is faithful; the comment stays.
- Awake latch: NOT fixed. The original uses the camera frustum (Character::CanUpdate 0x3a52a4, TestCullingBeforeUpdate 0x33de90); the port latches on AI ViewRadius. Logged as B160.

## Item (b): damage calibration and AOE knockback. DID NOT WORK (not calibrated)
- Formula located (IDA dump): `Character::F_SkillAttack` 0x3b31a4 calls `_F_CalculateResult` 0x3b2638, which calls `CF__CalcDamage` 0x3b1fb8 (pseudocode lines 130158-130345). The structure: random in [min,max] from property IDs 79/80 (or 81/82 for the other type), plus bonus damage, state and combo bonuses, crit adds max, block scales, defence `25*prop71>>8` subtracted, 8.8 fixed point, elemental range 95/96 or 98/99.
- Port: damage goes through `PlayableActorWorld::calculate_source_result` (playable_actor_world.cpp:189-230). Calibration needs the monster property sheet at the Mud Lord's dynamic level (LevelMin/Max 5-6), which the port does not apply. Not done.
- AOE pushback: `swamp_king_attack_4.luac` line 53 `ApplyPropClass(SwampKingSkillAOEPushback)` (SkillTree class). The port records the class token (runtime_boss_encounter_plan_v1.cpp:12) but does not apply it. Not implemented (B159).

## Item (c): continuous run afterwitch -> lever -> intro -> fight killable. DID NOT WORK
- Run m2c2 (afterwitch seed, same first half as `runfull.ps1`, fight taps added, exit teleport at frame 8000): the lever opens (`Lever script=open_SwampKingDoor`, log ~line 1998), the intro runs (~2151-2266), the boss AI is active (DoSkill 1/2/3, dive, relocation). The hero landed 16 hits, but the boss was alive when the hero's potions ran out. The hero died (`Death flow admitted frame=8378`), then a checkpoint restore (frame 8481). The exit teleport happened at frame 8000 while the boss was alive.
- Harness fix NOT done: relocation coordinates (spawn points 03/04/01) are not logged by the port, so a scripted follow route could not be built reliably in this session. Logged as B158.
- The kill itself was shown from the post-intro seed bossroom4 (run m2c1, see item d).

## Item (d): kill -> quest banner -> Darkwood load. WORKED (real run)
- Run m2c1: seed bossroom4 (post-intro), real keys (`run-k-chain.ps1`), exe sha above. Evidence:
  - Kill: `[boss-ai] died actor=14582633645715090783` (run.log ~2022), `Quest event kind=0 property=358 applied=1`.
  - Banner: `Quest banner kind=QUEST COMPLETED row=50` at frame 4754 (`Quest banner sound ... frame=4754`). Screenshot `C:/dh217/cases/m2c1/f4800.png` shows "Quest Completed / Escape the swamp with Rene and Celeste / Reward 100 EXP".
  - Exit: `Exit zone entered=_prim_ExitLevelZone_001 target=DARKWOOD frame=5400`; `Transition confirmed key=E ... frame=5643`; `Transition character loaded level=data/scene/003_darkwood.mlx frame=5644`.
  - Darkwood load: 0 `Foundation error` lines (the B155 handoff's `LoadTemplate assertion(9) provider` failure is gone). Screenshot `C:/dh217/cases/m2c1/f6200.png` shows the Darkwood map with the arrival dialogue.
- Also run m2d1 (quest condition forced with `--condition-active IsAfter_Swamp_Escape`, no kill): Darkwood level loads, 0 errors, `character.world-003_darkwood.gsave` written.

## Files changed (branch v21.8-mudlord2)
- `port/windows-foundation/animation_markers.{hpp,cpp}`, `original_character.cpp` (#7)
- `port/windows-foundation/combat_session.cpp` (#8)
- `port/windows-foundation/features/bosses/runtime_timer_trap_v1.{hpp,cpp}`, `main.cpp` one line (#10)
- `port/windows-foundation/features/campaign_host/campaign_host_tests.cpp` (#4)
- Tests: `tests/animation_markers_tests.cpp`, `tests/combat_session_actor_transition_tests.cpp`, `features/bosses/runtime_swampking_core_ai_v1_tests.cpp`
- `port/windows-foundation/CMakeLists.txt` (animation_markers test gets the android data root as argv[2])
- `docs/BUGS-AND-IMPLEMENTATION.md`: B158 (continuous-run follow), B159 (damage calibration and pushback), B160 (awake latch, resetonhubchange)

## Summary
| Item | Status | Evidence |
|---|---|---|
| #4 trigger persistence | WORKED | IDA 0x398978/0x3987ac/0x398794; campaign_host test; CTest 187/187 |
| #7 empty clips | WORKED | animation_markers tests incl. real hurt_01 sentinel; m2c1 0 marker errors |
| #8 nested suppression | WORKED (narrowed) | dead-select test; caller trace; CTest 187/187 |
| #10 timer traps | PARTIAL | validator tests; GetRand unused in original (not a bug); awake latch open (B160) |
| (b) damage | DID NOT WORK | formula located; no calibration; pushback not applied (B159) |
| (c) continuous run | DID NOT WORK | m2c2: boss alive, hero dies; no follow aid (B158) |
| (d) kill -> banner -> Darkwood | WORKED | m2c1 log lines + f4800.png, f6200.png |

---
# Sonnet escalation (items b and c) and HANDOFF
Branch v21.8-mudlord2, exe for real runs `C:/dh218/mudlord2-after5.exe` (sha256 535b3fb8e1366797fede3835b6895e34b47c4e71d366c518519b4da2b4beb63f). Subst `U:` + one junction `U:\.local-inputs` -> primary `.local-inputs` (remove with `cmd /c rmdir`).

## (b1) Level applied to the Mud Lord: the earlier claim "dynamic level not applied" is WRONG
- Original: swampking_OnInit (swampking_core.luac) = monster_OnInit: level = host level clamped to `GetCurrentLevelRange(difficulty)` (LuaScript::_GetCurrentLevelRange 0x37f1f0 pushes LevelList row +60 (min) then +48 (max) for difficulty 0). Only when no range is returned does the row's own LevelMin/LevelMax (CharacterTable SwampKing: 5..6) apply. Then + LevelOffset (0) and SetLevel.
- Port: merged f9fc0d2a applies it. Real run log: `[boss-ai] dynamic level profile=SwampKing host_level=2 current_range=1..4 level_raw=512` (hero level 2 -> boss level 2, 900 HP; a level-5 hero would clamp the boss to 4). Test `runtime_swampking_damage_calibration_v1` asserts host 2 -> 2, host 9 -> 4, no range -> row LevelMin 5.

## (b2) Damage calibration: one real divergence found and FIXED (unit + real run)
- Formula (IDA): CF__CalcDamage 0x3b1fb8 type 0 (isSkill): rand[SnS_Min 174, SnS_Max 175) + bonus - (25*Defense71>>8) + rand[95,96)*factor when the ATTACKER's property 97 (element) is set; type 2 (skill flag 0x40000, the spit): rand[174,175) * element factor + attacker 166+element - (51*d73>>8). The port's dh2_combat_damage matches both branches line by line.
- Divergence: ApplyPropClass(CLASS_ID) is Character::_ApplyPropClass 0x3babdc -> PROPS_ApplyClass 0x3df3b8 -> _LoadClass 0x3e2e20 on the attacker's OWN resolved sheet (CharProperties+2708); class 225 SnS_CleanUp resets only SnS props 173..189. The port's `ranged_skill_formula_sheet_v1` started from the property DEFAULTS, dropping the monster's own element 1 / elemental range 8.4..13.6 (props 95/96), attack rating 50 and level 19. Fix: start from the live resolved sheet (class applied in place, source reads from the live snapshot). Used by the Mud Lord and the Bogwitch.
- Result at level 2 vs the level-2 Knight (169.195 HP, no gear): bite SwampKingAttack1 65.6..81.0 (was 57..67), bite 2 70.6..86.0, emerge AOE 55.0..65.4 (was 46.6..51.8), spit 46.2..56.6 (type 2, unchanged). Real-run values: bites 70.4/67.7/74.0/75.3/77.4, emerge 53.3/60.8 (hero gear defence), spit 46.8..56.2 (logs m3k1, m3c3, m3s4). The test runs dh2_combat_result over 6000 seeds x 3 defenders (no gear, Defense 20, 100% resist), checks every landed hit lies in an independently coded range, that draws cover the range, and push-outcome rates (bites < 1/8, emerge > 1/3).
- Consequence: the boss now one- or two-shots a 57-HP hero and kills a 169-HP hero in about 3 hits; harness heroes need full HP and potions.

## (b3) AOE pushback: IMPLEMENTED, seen in real runs (WORKED), with one deviation from the earlier B035 work
- Original: swamp_king_attack_4.luac ApplyPropClass(SwampKingSkillAOEPushback) (SnS_Push_Target_Chance 158 vs 108 for bites) + SkillTable flag 0x100000 -> outcome bit 0x80 (CF__CalcPush) -> Character::F_ApplyResult 0x3b10b4 -> CharStateMachine::SM_SetKnockBackState 0x3c5ea0 (great = mask&0x100000 -> CharAnimTable +36 GreatKnockedBack, else +72 KnockedBack, stanced by AnimStancedAnim bits 0x800/0x400; a Boss defender is a no-op) -> `_SetState(10, 50011)`.
- IDA finding (deviation): F_ApplyResult applies the 0x80 flag for every defender; the "Player && SM_IsIdle" test only synthesises the Injured/Dodge bit. Event 50011 is registered (OnInit 0x3c7d0c..0x3c8ee0) by Spawn1, Idle3, Move4, Attack5, Skill6, Scared8, Stunned9, Injured11, Interact13, Reviving15, LiftingIdle18, LiftingMove19 (Attack5/Skill6 with the CSM_Interrupted 0x3ad244 flag condition). The existing b035 admission accepted Idle3 only; the hero was mid-combo at the AOE so no push happened in m3p1-3. Changed to `source_knockback_event_accepted_v1(state)` (b035_session_push_effect_v1.{hpp,cpp}, test in b035_session_push_effect_v1_tests). The Attack5/Skill6 flag condition is left to the Session transition admission (unverified; the Session accepted the transition in the real run).
- Wiring in main.cpp (B035 was unbound in the normal EXE): per-stance push banks (`build_session_push_animation_bank_v1`; clips prefixed `player-source-push-<stance>/` merged into the player's sourceAnimationClips before Session initialize; stances from the new `RuntimePlayerLocomotionLibraryV1::stances()`); `bindSourcePushEffect` (every frame and after every restore, because a restore clears the Session handler) binds `bind_session_push_effect_v1` whose sink calls `play_session_push_animation_v1`; physical-transition consumer hooks `knockback_read/write_gate528`, `knockback_controller_lock` (characterControllerBlocked) and `knockback_look_at_cancel_sneaking` (LookAt attacker + motor facing); checkpoint flag case 10 -> 0x2341 (INFERENCE: equals the policy.state_flags of the push sequence).
- Real-run evidence (m3s4, m3u3; exe after4): `[push] KnockBack target=<hero> attacker=<boss> source=SwampKingAttack4 great=1 direct=1 outcomes=0x80 mask=0x8335535 stance=1 frame=1176`; hero position -19349.7,11410.5 -> -19203.3,10936.6 -> -19146.7,10696.9 (about 700 units away from the boss), then idle; controller blocked during the pose (potion tap refused, `controllerBlocked=1`), combo resumed afterwards. Frames `C:/dh217/cases/m3u3/f1160..f1260.png`, sheet `C:/dh217/cases/m3u3/push-sheet.png`: blue shockwave ring around the hero, hero thrown back in the great-knockback pose, then back on his feet.
- Original footage (video Part 1, 1134.5-1137.5 s, 4 fps crop): blue ring around the hero, hero staggers with arms out. Direct observation; no large throw visible, so the port's ~700 units may exceed the original (MoveGO root motion is authored in the clip; exact original displacement NOT verified).
- Not done: monsters pushed by the player (no bank); the Player-gate deviation should be reviewed by the B035 owner.

## (c) Continuous run: DID NOT WORK (diagnosed, harness not finished)
- Findings (m3c2, m3c3: afterwitch seed, real keys, exe after5):
  1. The seed hero reaches the boss room at 57/169 HP; with calibrated damage he dies in two hits. Harness fix: potion taps (`3.5:-53`, `6.5:-53`) after the S2 sync (works: `Source potion key=5 ... HPraw=14659->43314`, frame 923).
  2. S3 of run-c-afterwitch has 22 interact taps over 44 s, so S4 was reached ~1000 frames before the fight segment (hold attack + potion taps) began. Fixed by cutting S3 to 12 taps (the intro ends ~frame 2900 drawn).
  3. With those fixes the hero fights ~2700 frames, uses all 5 potions and still loses (death at 5713, m3c3): after drawn frame ~3170 he stops hitting because the boss drifts north (y 11722 -> 12221) away from the hero, who stands at -19544,11381 (pushed there by body collision). Every `--follow-actor` placement reports placed=1 but the hero does not move (target spots over the water have no floor). Following the boss is not possible there; why the boss drifts away from an off-platform hero is NOT explained (possible port issue: unreachable-target pathing).
  4. Next idea, not completed (stopped on the coordinator's order): teleport the hero at drawn frame 2905 to the bossroom4 fight spot `-19369,11390.6,-113.968` (where kills were proven: m3k1, m3p*, m3s*) with `--player-to`. Script `C:/dh218/tools/run-c-tp.ps1`. Run: `powershell -File C:\dh218\tools\run-c-tp.ps1 -Case m3c5 -Exe C:\dh218\mudlord2-after5.exe -ExitFrame 6500 -Frames 7600`; success = `[boss-ai] died`, `Quest banner kind=QUEST COMPLETED`, `Transition character loaded ... 003_darkwood`.
- Code kept: `--follow-actor PROFILE@FROM-TO[:DIST]` (main.cpp option + per-frame block, `features/test_aids/follow_actor_aid_v1.hpp`, CTest `follow_actor_aid_v1`). Inert unless passed.
- A boss kill from ONE continuous run is NOT demonstrated. The kill -> banner -> Darkwood chain from the post-intro seed is re-confirmed with the new exe: run m3k1 (`[boss-ai] died`, `Quest banner kind=QUEST COMPLETED row=50` frame 4747, `character.world-003_darkwood.gsave` written, 0 Foundation errors).

## Tests and state
- New/changed CTests: `runtime_swampking_damage_calibration_v1` (new, 72k checks), `follow_actor_aid_v1` (new), `b035_session_push_effect_v1` (state predicate), `runtime_player_locomotion_library_v1` (stances()). Targeted run (swampking x2, ranged_skill, follow_actor, b035_session_push, spawn_character): 6/6 pass. Full `ctest -j4` started with the llvm-mingw bin on PATH, log `C:/dh218/mudlord2-ctest-final2.log` (expected 189 = 187 + 2 new); check it. Without that PATH some tests fail with exit 0xc0000135 (missing DLL), an environment issue.
- WIP commit: not fully verified (full CTest result not confirmed at commit time; docs/BUGS-AND-IMPLEMENTATION.md B158/B159/B035 rows not updated).

## Recipes
- Build: `bash C:/dh218/mudlord2-build.sh` (needs `subst U:` to the worktree and the `.local-inputs` junction); `TARGET=<name>` for one target.
- Real runs: `C:/dh218/tools/run-k-chain2.ps1 -Case X -Exe <exe> -Shift <s>` (post-intro seed, boss kill; the push appears in about 1 of 3 runs because RNG outcomes vary with tap timing), `run-k-chain3.ps1` adds dense captures at frames 1160-1260, `run-c-follow.ps1` / `run-c-tp.ps1` the continuous run. Cases land in `C:/dh217/cases/<Case>`. The harness scripts live outside the repo in `C:/dh218/tools`.
- Next steps: (1) finish (c) with run-c-tp; (2) confirm full CTest 189/189; (3) update docs/BUGS-AND-IMPLEMENTATION.md B158/B159/B035; (4) B035-owner review of the push gate; (5) the player-side generic skill coordinator likely builds its formula sheet from defaults too (same ApplyPropClass finding), not changed.
