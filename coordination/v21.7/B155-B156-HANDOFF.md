# B155 / B156 handoff (WIP, branch v21.7-mudlord)

Worktree: `C:/Users/adamc/Desktop/workspace/DH_sc/.claude/worktrees/v217-mudlord`. It was branched from windows-foundation bc642f73. The WIP commit is 497d9b3e and has not been pushed. The final report `coordination/v21.7/B155-B156-report.md` is NOT written yet. The full ctest has NOT been rerun since the last edits (the baseline was 182 passing; the new test is `runtime_swampking_core_ai_v1`).

## 1. Verified in real hidden-window runs (after-*.exe, isolated seed copies, real keys)
- Sealed door: the lever (Space context button) opens it. The before.exe run also confirms this.
- The intro cutscene runs to the end. Before the fix it aborted at PutCharacterInLimbus ("Unknown original lifecycle actor") after the arrival world restore.
- The boss has a nameplate, can be targeted with Tab/Space, takes damage, and plays its hurt reaction (seq 673, zero-length clip, sound 517). It dies and the death outro plays: KillActor, the Swamp_Serpent_Defeated dialogue (Celeste/Rene/Edward/Gothicus lines identical to the video), and SaveGame.
- Quest Swamp_Escape (row 50) completes. `IsAfter_Swamp_Escape` becomes true. The banner reads "QUEST COMPLETED / Escape the swamp with Rene and Celeste / Reward 100 EXP" (boss12 frames f3320-f3440, sheet `C:/dh217/boss12-banner.jpg`), matching video 20:48.
- The exit prompt "Gothicus Darkwood" appears and E starts the transition. The load of `data/scene/003_darkwood.mlx` then fails with a Foundation error: "Level::_LoadFromXML service: required original LoadTemplate assertion(9) provider". This is the next-level handoff gap. It is probably pre-existing; this was not checked with before.exe.
- Boss AI (swampking_core.luac facts) with the overlay assets:
  - spotting, SwampKingMusic, intimidate;
  - bites DoSkill(0/1);
  - spit projectile (SwampkingSpit) with AOE 250;
  - SwampkingSteamCloud TimerTrap mist: 6 activations over about 7.6 s, matching video 19:17-19:24;
  - phase 2 at about 73% HP and weak at about 23%;
  - dive, relocate to spawn points 03/04/01, emerge DoSkill(3) AOE, untargetable while submerged (SetIsTargetable).
- Runs boss11 and boss12 show this. In boss12 the boss was killed with its AI active in about 12 s (potions used).

## 2. Compiled and unit-tested only, or not runtime-verified
- The `runtime_swampking_core_ai_v1` tests (parse, derive, timers, decisions) and the B155 checks in `original_actor_lifecycle_tests` and `animation_markers_tests` pass in the last targeted build. They have NOT been run as part of a full ctest after the final edits.
- Continuous run (afterwitch seed → lever → intro → fight, runs full4/full5): the AI works, but the scripted hero could not follow the relocated boss, so the boss was not killed in 28 s. A kill from a continuous run is therefore not shown.

## 3. Files (all in the WIP commit)
- `original_actor_lifecycle.*`: idle baseline survives `detach_for_restore`; `idle_baseline_ids()`, `restore_idle_or_limbus()`.
- `main.cpp` (localized B155 blocks):
  - restore of idle/Limbus lifecycle records after a world restore;
  - `live_limbus_actors` provider;
  - script-idled AI-type-4 monster admitted as a gameplay actor;
  - boss AI owner binding (`bindEnemyAI`) and TimerTrap owner;
  - targetability permission and refresh;
  - projectile/trap FX drawing;
  - `saveLevelWorld` now persists the campaign lifecycle before capture.
- `features/campaign_host/actor_clip_manifest.*`: `collect_script_idled_actors`.
- `features/campaign_host/campaign_host.*`: "campaign_limbus_v1" save component and `read_live_limbus_actors`.
- `combat_session.cpp`: suppresses the nested runtime transition in `select_actor_state_leaf`. This fixed "unfinished reached prefix" when the outro posed the dead boss.
- `original_character.cpp`, `animation_markers.cpp`, `retained_animation_owner.cpp`: admission of an empty authored clip with the INT32_MAX..INT32_MIN sentinel (swampking_hurt_01).
- `original_melee_bindings.*`: `add_actor_state`. `features/spawn/actor_profile_derivation_v1.*`: `append_sequence_state_v1`.
- `features/bosses/runtime_boss_encounter_plan_v1.*`: `CloseAttackRulesV1`, `ranged_attack`, `phase_after_update`.
- NEW `features/bosses/runtime_swampking_core_ai_v1.*` (+ `_tests.cpp`): script parsing and the RuntimeSwampkingCoreAiV1 owner.
- NEW `features/bosses/runtime_timer_trap_v1.*`: TimerTraps/GameObjectDamager tables, trap script, model clips, and the RuntimeTimerTrapsV1 owner.
- `CMakeLists.txt`: library `foundation_runtime_bosses` and test `runtime_swampking_core_ai_v1`. The temporary probe target was removed.

## 4. Build / test / harness
- Build: `subst U: <worktree>` (currently active; remove with `cmd /c subst U: /D`), then `bash C:/dh217/build.sh` (optional `TARGET=dh-foundation`). The build dir is `C:/dh217/build`, using llvm-mingw `.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64` and CMake/Ninja 3.22.1 from the Android SDK, Release.
- ctest: `PATH=<toolchain>/bin:$PATH ctest --test-dir C:/dh217/build` (the toolchain bin must be on PATH).
- Exes: `C:/dh217/before.exe` (sha da9ccf4c…) and `after-a..after-j.exe`; copy the new build to after-k.exe.
- Harness: `C:/dh217/tools/run-case.ps1` (adapted from C:/dh216/b154/tools/run-b154.ps1). It reads seeds from `C:/dh217/seeds` first.
  - Seeds: afterwitch, bossroom, bossroom2-4 (bossroom4 = after the intro, with the limbus component; best for a kill test, Continue).
  - Test aids: `--player-to` zone/coords and `--capture-at` (≤ about 150 values).
  - Inputs: real keys Space/Tab/W/E and 5 = potion. Use keydown hold=-1 / keyup hold=-2 to interleave potions with Space. Do NOT use `--test-player-hp` together with potions.
- Overlay assets: `C:/dh217/tools/make-overlay.ps1` → `C:/dh217/assets-overlay` (hardlinked package plus copied originals; junctions are rejected).
- Video frames: `C:/dh217/video` (f1, f4, f15a, sheets; w1 = witch 960-1002 s at 1 fps; w8 = 983.5-986 s at 8 fps).
- Cleanup still TODO: the worktree `.local-inputs` contains 18 junctions (list in `C:/dh217/junctions.txt`). Remove each with `cmd /c rmdir <path>` BEFORE any worktree delete. Never recursive-delete through them.

## 5. Evidence summary
- Video P1, observed:
  - 18:12-18:15 orb pedestal and door light;
  - 18:23 "Demon Lair - Mud Lord";
  - intro dialogue and lizard ambush; 18:45 blue ring blast;
  - 18:46-52 the serpent emerges and eats a lizard;
  - 18:53 "Mud Lord" plate; close-range bites at 18:56;
  - 19:08-19:09.5 dive, 19:12.5 emerge elsewhere with a blue splash;
  - 19:17.75-19:24 white mist on the hero;
  - 19:38 outro; 20:48 quest banner 100 EXP and LEVEL UP.
- IDA addresses:
  - OnEndOfAnim = events 34/35 (0x3cbb34);
  - PlayAnim 0x3ba388 / CSAnim 0x3c1a14;
  - SetIsTargetable 0x3b7a64 (player targeting only);
  - Script_SetActorPosition 0x45e5c8 → SM_SetIdleState;
  - TimerTrap 0x390c50/0x39d724/0x39d688/0x39eedc, zone bounds Zone::InitPost 0x39771c;
  - TMR_Start 0x3dbe24;
  - AI_IsSkillUsable 0x3d8358;
  - _UpdateAggro 0x3cf3f0;
  - culling 0x3a52a4/0x33de90.
- Data: CharacterTable SwampKing row 358 (AI 63 swampking_core, SkillTree 33 = 106..109, projectile 34, ClassID 243), AnimTable 70, TimerTraps SwampkingSteamCloud.
- B156 (witch abilities), evidence gathered and concluded but not yet written up in the report:
  - `witch.luac` uses only DoSkill(0) and a teleport every 10 s; SkillTree 35 = [122] only (`witch_ranged_00`, projectile 39 bogwitch, ImpactFX 76 `spell_dh2_hurt_bogwitch`).
  - Frames 966-1000 s: the green burst on the hero (the "poison cloud"/"bubble") is WitchImpactFX, already ported in B154. The white double flashes are the teleport FX, already ported.
  - The orange blast with a red crackle "web" (about 984.1 s) and the blue lightning strike (about 985.5 s, yellow 33) land ON the witch. Hotbar icons grey out at the same moment, so these are inferred to be player-side (hero skill / faery) effects, not witch abilities.
  - `rene.luac` has no heal: only Attack() and the Buff_Rene_Haste catch-up. The row WanderingPriest has projectile 19 FireMastery. The hero's HP gains match potion use (potion counter 7→6).
  - Conclusion: no missing witch-side ability. Rene's ranged attack is the known separate gap B077.

## 6. Remaining, in order
1. Run the full ctest and record the pass count (expect 183).
2. Optionally confirm with before.exe that the Darkwood load failure is pre-existing.
3. Write `coordination/v21.7/B155-B156-report.md` (evidence, root causes, changes, tests, runtime-verified vs unit-only, gaps, package file list) and copy a few evidence sheets to `coordination/v21.7/B155-evidence/`.
4. Commit; remove `subst U:`; remove the junctions with `cmd /c rmdir`.

## 7. Known gaps / risks
- TimerTrap damage is not applied: the `_colzone` point-cloud bounds are undecoded.
- MarkAsFlying is not modelled; the boss motionRoot is cleared instead.
- The boss dynamic level (LevelMin/Max 5-6) is not applied, so damage is uncalibrated (bites/spit 46-67 against the hero's 169 HP).
- The AOE pushback/knockback is not applied.
- The awake latch approximates culling. Engine timers are not modelled.
- SetIsTargetable also blocks damage in the port. In the original it only affects player targeting.
- The projectile expiry delayed callback is not called.
- The shockwave sound name includes ".wav".
- The Darkwood handoff is missing the LoadTemplate assertion(9) provider.

## 8. Original data files to add to the package
These are copied from `.local-inputs/assets-extra/android/data/`; make-overlay.ps1 prints their hashes.
- scripts/ai/swampking_core.luac
- scripts/skills/swamp_king_attack_1.luac, swamp_king_attack_2.luac, swamp_king_attack_4.luac, swampking_atk_spit.luac (and attack_3 if referenced)
- scripts/objects/swampking_trap.luac
- 3D/projectiles/elemental_bolt_water.bdae
- 3D/interface/fx_swampking_blue_shockwave.bdae, fx_swampking_attack_03_poisonspit.bdae, spell_dh2_hurt_steam.bdae
- 3D/GameObjects/go_trap_swampking_steam_cloud.bdae (if not already in the package)
