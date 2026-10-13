# v21.8 mage report (items 4 and 6, Mage part)

Branch `v21.8-mage`, worktree `.claude/worktrees/mage`, build dir `C:/dh218/mage-build`, subst `M:`.
Base: `windows-foundation` @ 93f4823f.

## Summary per item

- **Item 1 (Mage basic staff attack + Tab target, I008): PARTIAL.**
  The staff attack now starts, plays the wand clip and reaches its state-5 projectile marker. Tab target
  selection worked once in a real run and was not reproduced in the final-exe runs.
  - Unit: `combat_system` gained `projectile_delivery` tests (validation, no melee damage at a marker); passes.
  - Real run (baseline, `mage-before1`): zero attack begins, zero markers, zero Tab selections.
  - Real run (`mage-step1c`/`step1d`, `mage-final-src`): attack begins at frames 40-164, marker `do_skill` at state 5 each time.
  - Real run (`mage-step1d`): `Source target command ... selected=<lizard>` once at frame 404. Not reproduced in `mage-final-tab`/`tab2` (timing-sensitive hidden-window runs).
- **Item 2 (shared ranged/projectile executor): WORKED for launch, flight and visual; contact/damage NOT bound.**
  - Unit: `runtime_session_projectile_v1` and `runtime_session_projectile_render_v1` now compile, link, are registered in CMake and pass (they were not in CMake before). Both now use the promoted `load_runtime_session_projectile_tables_v1`.
  - Real run (`mage-final-src`): `launch accepted id=1 row=20 model=elemental_bolt_fire.bdae`, then expiry impact (kind=1), no crash.
  - Real run (`mage-step2i`, captured frames 44-104): the fire bolt is visible in flight at frame 72, separate from the caster.
  - Not done: physical contacts. The three providers (enemy query, owner hit, hit semantics) return explicit errors, so a bolt cannot damage anything. Damage results trigger a loud throw if they ever appear.
- **Item 3 (ColdRay, ChainLightning, StoneEscape, Cyclone via the executor): DID NOT WORK / NOT STARTED.**
  No skill was admitted, no skill was routed through the executor, and no skill was verified. Reasons are listed below.

## Root cause of the item 1 gap (original vs port)

- Original: Staff01 (`equipment_type` 4/5) is a ranged source weapon. Its attack clip `skill_dh2_prince_mage_wand_attack.bdae` has a `do_skill` marker. The state-5 ranged event spawns ProjectileTable row 20 (FireWandProjectile, `elemental_bolt_fire.bdae`, speed 20, timer 1000 ms). `CharAI::melee` redirects ranged owners through `attack_range_redirect` (`level-world/character_ai_attack.cpp:39`).
- Port (before): the frontend "Selected class" block gave every non-Knight class an animation-only profile (`main.cpp` ~1119-1130). `main.cpp` ~7938 then forced attack and target-select off for animation-only profiles. The Rogue-only attack-bank block did not run for the Mage.
- Port gaps found once the Mage joined the attack-bank block: (a) `validate_attack_definition` requires melee damage markers; (b) the `attack_range_redirect` service was unbound (rejected); (c) the bridge classified the state-5 projectile marker and dropped it.

## What changed

- `combat_session.hpp/.cpp`: `CombatSessionProfile::rangedProjectileMarkers` (opt-in). Such an actor binds no melee damage marker and no melee source. Its state-5 `projectile`-kind markers are queued per actor; `CombatSession::take_projectile_markers(id)` drains them after the update. `attack_range_redirect` begins the same source attack only for ranged profiles; other owners keep the rejection.
- `combat_system.hpp/.cpp`: `AttackDefinition::projectile_delivery`. Validation accepts zero markers for it, and `consume_marker` never applies melee damage for it. Covered by `combat_system_tests` (`projectile_delivery_has_no_melee_damage`).
- `main.cpp`:
  - MagePlayerBase joins the existing Rogue attack-bank block with `rangedProjectileMarkers=true`. Rogue behaviour is unchanged.
  - Per frame, queued markers go to `launchPlayerRangedProjectile`: it builds the source inventory (main hand from `options.combat.mainItemId`), runs `runtime_source_range_selection_v1`, and calls `launch_state5`. The executor is then updated each frame. Visuals join the existing projectile-FX path under key domain 0xB156.
  - New option `--projectile-source-assets <dir>` (see below).
  - A refused launch is logged (`Mage projectile launch refused: ...`) and the game keeps running.
- `runtime_session_projectile_v1.hpp/.cpp`: promoted `load_runtime_session_projectile_tables_v1(AssetCatalog, error)`. The test's private copy was replaced by it.
- `CMakeLists.txt`: new `foundation_runtime_session_projectile` (the four `runtime_session_projectile_*` sources plus `runtime_source_projectile_v1.cpp`, `android-native/.../source_process_arrays_v101.cpp` and `game-data/combat_events.cpp`), linked into `dh-foundation`. Two new test targets: `runtime_session_projectile_v1` and `runtime_session_projectile_render_v1`, both pointed at `.local-inputs/runtime-session-projectile-v1/assets`.

## Packaging gap (needs the lead)

- The v21.7 package carries only 55 of the PyData files the ordered source loader needs, and no `data/3D/projectiles/elemental_bolt_fire.bdae`. The complete set is in `.local-inputs/runtime-session-projectile-v1/assets/original-cache` and `.../data/3d/projectiles`.
- Until the v21.8 staging adds them, a Mage run needs `--projectile-source-assets <that assets dir>`. Without it the attack still plays but no bolt is launched (logged as refused).
- Recommended follow-up for the staging script: add the 79 missing pydata files and `elemental_bolt_fire.bdae` to the package.

## Mage test profile (how it was made)

- No Mage seed exists in `C:/dh217/seeds` (all are Knight). I did NOT run the character-creation flow.
- Used the profile file from the existing creation output: `.local-inputs/claude-preview13/verify-play/p12/h-mage.save` (944 bytes, MagePlayerBase), copied to `C:/dh218/seeds/mage/profile.save`.
- Launch: the B026 Mage candidate args (`.local-inputs/b026-mage-current-candidate/run.args`, copied to `C:/dh218/seeds/mage/template-run.args`). They start the menu with `--menu-actions menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER` and `--save <case>/profile.save`. Paths are rebound to the v21.7 package.
- Runner: `C:/dh218/tools/run-mage.ps1` (clicks, frames, captures, extra args; the same `quiet_click_sync.ps1` injector as `run-case.ps1`).
- Test-only: `--test-player-hp,1000` keeps the Mage alive against lizards.

## Real-run evidence (hidden window, real keys)

| Case | exe | begin | marker | launch | Tab | note |
|---|---|---|---|---|---|---|
| mage-before1 | before (pre-change) | 0 | 0 | 0 | 0 | baseline: "outgoing attacks await" |
| mage-step1c | step1 | 1 | 1 | - | 0 | attack and marker reached |
| mage-step1d | step1 | 2 | 2 | - | 1 | Tab selected lizard |
| mage-step2c | step2 (no model) | 2 | 3 | 3 | 0 | launches accepted, model not found |
| mage-step2i | step2d | 2 | 2 | 2 | 0 | frames 44-104 captured, bolt visible at f72 |
| mage-final-src | final | 1 | 1 | 1 | 0 | launch, expiry impact |
| mage-final-nosrc2 | final | 3 | 3 | 0 | - | refused x3, no crash |
| mage-final-tab / tab2 | final | 1 / 0 | - | - | 0 | Tab not reproduced |

## Tests

- Full CTest on the final branch state (before the last launch-refusal change, which is `main.cpp` only): **189/189 passed**.
  Registered tests total 189 (HEAD had 187 by subtraction of my two new projectile registrations). The coordination matrix's "184" is stale.
- `combat_system` (new test case) passes. `runtime_session_projectile_v1` and `runtime_session_projectile_render_v1` pass.
- Gap: no unit test covers the session-level queue and `attack_range_redirect` path. It is verified only by the real runs above.

## Item 3: why not done

- The skill cast whitelist (`generic_skills/runtime_skill_cast_coordinator_v1.cpp:115-145`) is owned by skills-a. I added no admission.
- The skill casts are state-6 `do_skill` markers. Those are classified as `skill` kind, not `projectile`, so they do not reach the new drain.
- Per the matrix, ColdRay is rejected by the coordinator, and ChainLightning and StoneEscape have no linked cue or FX.
- **Merge point for skills-a:** the drain in `main.cpp` after `combatSession->update` (the `take_projectile_markers` loop calling `launchPlayerRangedProjectile`). Skill projectiles would need a state-6 sink plus the skill row (Cyclone: `projectile_dh2_prince_mage_cyclone.bdae`; ChainLightning: `projectile_dh2_chain_lightning.bdae`, both staged in v21.8).

## Audit notes (for the later audit; not touched here)

- Witch: `enemy_ai/runtime_ranged_skill_monster_v1` keeps its own projectile list and draws under 0xB154. It is not routed through the shared executor, as instructed.
- Mud Lord: `bosses/runtime_swampking_core_ai_v1` (branch-only) keeps its own projectiles under 0xB155. Not routed.
- The shared executor still has no contact owner; any physical-contact work will need the three providers bound.

## Known gaps / inferences (marked)

- Inference: the `attack_range_redirect` implementation begins the same source attack (`request(requested)`). I inferred this from the control flow of `CharAI::melee`; I did not verify the body of the original redirect callee in IDA.
- Inference: projectile origin is the actor position with source heading (`{sin h, -cos h, 0}`, the same convention as `enemy_ai::source_look_vector_v1`). The original uses the live projectile node when present; not done.
- Gap: the staff attack keeps the melee reach as `maximum_range`. The ranged band is not used for approach or range gating.
- Gap: the Tab selection is not reproduced in the final runs.
- Gap: no `RuntimeSessionProjectileLaunchV1` tests with the actual Mage fixture in the session test; the existing fixture uses an animation-only player.

## Cleanup

- Junctions: 929 junctions to primary `.local-inputs` directories, plus about 4,040 hardlinks of primary root files, all created in `.claude/worktrees/mage/.local-inputs` (list: `C:/dh218/mage-junctions.txt`). Junctions removed with `cmd /c rmdir` (no recursive delete, all verified as reparse points); hardlinks unlinked (primary files unaffected).
- `subst M:` removed.

---
# Sonnet escalation: results and HANDOFF (branch v21.8-mage)

State: branch v21.8-mage = merge of v21.8-skills-a (clean; use-profile table and cast coordinator merged without conflicts) plus later commits.
Full CTest was NOT re-run after the last commits (usage limit). Targeted ctest passing earlier: runtime_session_projectile_v1 (extended), runtime_session_projectile_render_v1, combat_system (extended), combat_session_source_attack_bank, combat_session_ranged_attack (new, registered), runtime_skill_use_profile_v1 (now 10 rows; built, not re-run after the 3 external-owner rows were added). dh-foundation builds (BUILD_OK) at the last WIP commit.

## Item 1 - projectile contact, damage, impact FX: WORKED (real run)
- Original evidence: features/effects/runtime-session-projectile-native-contact-evidence.md (_HandleProjectile 0x3cc280: owner CharAI::OnProjectileHit, then F_MeleeAttack/F_ApplyResult); docs/ida-fidelity-audit-2026-10-11/weapon-range.md (AI_DoRangeAttack 0x3d076c: LookAt + SM_SetAttackState, no range gate); AISPlayer has no OnProjectileHit override, so the player owner callback is AISDefault's single bx lr (no-op). Live IDA was unavailable (provider not configured); notes and asm under port/level-world/reference/ranged-attack-v41 were used.
- Code: main.cpp registers a real sensor per bolt in the actor NativeWorld (RuntimeSessionProjectileContactBodiesV1), binds AI_IsEnemy, the owner hit (no-op) and F_MeleeAttack semantics; impact FX set (ProjectileTable impact_fx 241 = spell_dh2_hurt_fire) and sound go through the source effects/audio owners. Bug fixed: the actor body filter is consulted while the sensor is created, so the projectile identity must resolve before creation (runtime_session_projectile_contacts_v1.cpp; unit test in runtime_session_projectile_v1_tests.cpp). Ranged attacks LookAt and begin at any distance (combat_session.cpp, combat_system.cpp; tests combat_session_ranged_attack, combat_system).
- Real runs (hidden window, real keys): C:/dh218/cases/flight2..flight4, tab-b1..3. flight3: launch id=7 aimErrorDeg=0; frame 1141 "Mage projectile impact id=7 kind=4 target=4141719441445850732 fx=241 pos=-6860.95,732.27 damage=32.5977 applied=1"; "Damage ... marker=do_skill removed=32.5977"; "Mage projectile impact FX set=241 created=..."; hurt voice cues. The bolt travelled 128 units in 5 frames = genuine in-flight contact; ids 4 and 9 kill (died=1). flight4/f1141.png shows the target health bar reduced.
- Gaps: many bolts hit at the launch point (sensor radius 162 game units from the bolt BDAE AABB overlaps adjacent enemies); origin is the actor position, not the live projectile node; row 20 has no impact sound.

## Item 2 - Tab target: WORKED (cause was the harness, not code), reproducible 3/3
- Cause: earlier runs injected keys by wall clock from window creation; the first keys were consumed by the title splash / loading, so Tab often fell before gameplay. Original conditions (nearest eligible enemy, sticky target) match ours (cycle_player_target); no code change.
- Fix: gameplay-frame-anchored injection: the game writes {CASE}\S1.sync at gameplay frame 40 (--capture-at 40:{CASE}\S1.sync); injector clause W|{CASE}\S1.sync restarts its clock; plus --skip-boot. Evidence: C:/dh218/cases/tab-b1, tab-b2, tab-b3 each log "Source target command frame=78|79 ... selected=7118915781085668844" (3 of 3); all later runs also select.

## Item 3 - staging: WORKED
- stage_assets_v21_8.py gained per-group destination roots (group_destinations); asset-selection-v21.8.json adds groups pydata_complete_projectile_executor (39 PyData files missing from assets/original-cache/data/pydata; identical to the android originals), mage_projectile_fire_bolt (elemental_bolt_fire.bdae, spell_dh2_hurt_fire.bdae), mage_hurt_fx.
- Generated .local-inputs/windows-source-clock-v21.8-staged-mage2 (manifest verified). Runs on it with NO --projectile-source-assets launch the bolt and play FX 241 (cases cold8, contact3). staged-mage1 is an older throwaway copy (delete from the primary .local-inputs; real directory). The lead should regenerate the final staged folder from the selection JSON under a new folder name (the tool refuses to overwrite).

## Item 4 - Mage skills
- ColdRay: WORKED in real runs (cases cold6, cold8). Profile row from prince_mage_coldray.luac (650, 90 degree cone, FrontalFirst, objects rolled, no cooldown, Post ClearTarget). Log: "Source skill key=2 ... phase=1 skill=ColdRay"; "Audio step source ... sequence=413 sound=228 status=1"; "Source player step FX ... sequence=413 sets=150, dispatched=1"; "phase=2 hits=1"; "Damage ... marker=do_skill removed=7.27"; "Source skill target FX name=elem_dmg_02_water set=125" (ice FX visible in frames). Also fixed: player FX anchor without --campaign-triggers (main.cpp); per-target hit FX (profile field target_hit_fx_name). Not reproduced: Dot/freeze tails.
- ChainLightning, Cyclone, StoneEscape: DID NOT WORK / NOT VERIFIED (code written, not wired in main.cpp, never run). Done: executor skill projectiles (launch_skill, delayed_check/delayed, target lock (inference), expiry, projectile_position; unit-tested); runtime_skill_external_owner_v1.hpp + coordinator hooks + the 3 profile rows; SkillTargetOriginV1 search; features/generic_skills/runtime_mage_skill_owner_v1.{hpp,cpp} (no unit test yet).
- NEXT STEPS: (1) main.cpp: construct RuntimeMageSkillOwnerV1 with services (projectiles -> playerProjectiles; projectile_tables -> playerProjectileTables loaded at session start; projectile_launched -> NativeWorld sensor via the bounds-provider overload (BDAE bounds via read_content + decode_original_scene); play_effect/stop_effect -> sourceEffectsFactory->manager().play_set / drop_set_by_id_v117 by AnimatedEffectTable name; play_sound -> runtimeAudio->play_named_plain; move_actor -> nativeBodies.set_position + motor->sync_source_position; teleport_path -> sourceFloors trace); set request.external_owner at the hotbar cast site; call owner.advance after the projectile update; owner.reset on session change; skip drawing invisible_bolt. (2) saves: C:/dh218/tools/skills-a/make_rogue_skill_save.exe seeds/mage/profile.save out.save ChainLightning 0 1 9 (also Cyclone, StoneEscape). (3) real runs with run-mage2.ps1; verify animation (sequences 412/414/423), FX, sound (Cyclone 537, StoneEscape 526; chain plays its own sounds), effect. (4) owner unit tests and full CTest.
- Gaps/inferences: DoT/stun/slow tails, LOCK/UNLOCK, Dummy actor (FX stand-in), TeleportTool projectile (trace stand-in), steering law of target_lock projectiles, chain victims compared by identity.

## Recipes
- Build: bash C:/dh218/build-mage2.sh (env TARGET="dh-foundation ..."), build dir C:/dh218/mage-build; needs subst M: of the worktree and M:/.local-inputs (single junction to the primary .local-inputs). ctest: bash C:/dh218/ct.sh -R name --output-on-failure.
- Real run: powershell -File C:/dh218/tools/run-mage2.ps1 -Case X -Exe C:/dh218/skill3.exe -Save C:/dh218/seeds/mage/coldray.save -Clicks "W|{CASE}\S1.sync,0.5:-9:0:80,2:-50:0:80" -Frames 480 -Captures "170,176" -Extra "--skip-boot;--test-player-hp;5000;--capture-at;40:{CASE}\S1.sync" (-9 Tab, -32 Space, -50 key 2; Extra separated by semicolons). run-mage3.ps1 uses the production startup.args (needs --campaign-skip-frame; starts 7600 units from the lizards; not used for verification).
- Scratch probes (uncommitted): C:/dh218/probe/prow.exe <assets> <ProjectileRow...>, crow.exe <assets> <ClassRow...>. Evidence: C:/dh218/cases/ (flight*, tab-b*, cold*, contact*).
