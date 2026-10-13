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
