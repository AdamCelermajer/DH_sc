# Preview 14 SKILLS report (stream: skills, branch p14/skills)

## Status
- Done and verified in the EXE: (1) Stats +/- spend, refuse at 0, persist, live recalc; (2) Skill Upgrade spend, saved rank, refuse at 0 points; (4) starter grant decision and fix; persistence fix for skill spends.
- Done and tested in ctest: stat training spend/refuse/rollback/recalc (`stat_training_v1`).
- NOT done: (3) level-up point table per class and level-up presentation (see Open items). (5) partially: fixtures and quiet batches done; no level-up batch.
- `ctest`: 102/103 pass. The one failure, `session_skill_binding`, is pre-existing in this worktree layout (see Open risks), not caused by this stream.

## Commits (branch p14/skills)
- `fbecf47d` WIP snapshot from the dead worker (reviewed, kept): `stat_training_v1.{hpp,cpp}`, CMake hunk, `source_composition.hpp` `register_stat_training_callbacks`, main.cpp stat registration, visit gate, direct profile load/projection.
- `7f8d4248` skill spends persisted after commit (main.cpp services.train). Starter grant only for a fresh direct character.
- `47b51f46` focused ctest `stat_training_v1` (`features/character_menu/stat_training_v1_tests.cpp`, CMake target after `runtime_skill_session_training`).

## What was changed and why
1. Stats +/-: the WIP wires `register_stat_training_v1` in main.cpp (after `register_page(skills)`, before `install_content`). Hit regions measured from the Stats frame: Strength/Dexterity/Endurance/Energy at authored x 3..63, y 134..174.5 / 180..219.9 / 224..263.4 / 270..309.9. Checked against `/runs/view-s3/out/stats-open.png`: the "+" boxes match these regions.
   - Spend: debit 148, add 149+stat, reset base from the CharacterTable row (keeping base level, same as profile projection), `dh2_class_recalc_base`, persist first, then `update_combat_properties`, then actor vitals and CharacterState. Failed persist leaves Session and CharacterState unchanged (tested).
   - Visit gate: one spend per menu visit (SWF `AddedStatsThisTurn`), reset on open.
2. Skill Upgrade: the click works (rank 1->2, points 3->2, Headsplitter damage 14-21 shown). Bug found: the authored prefix saves *before* the spend, so the on-disk save kept the pre-spend state (points 3, rank 1) until the next save. Fix: `save_character` after a successful commit as well (main.cpp, services.train). Verified by reload of the file (see evidence).
3. Starter grant (item 4): the row-0 starter debit in direct runs now happens only when `--save` does not exist yet (fresh character, which matches creation). Existing saves keep their saved points on load. Rationale: the original grants row 0 only at creation (`player_initial_grants_v2`, creation subtracts it); a debit on load double-counts a point. Verified: skill3 profile stays at 3 points on load (`Direct player source points ... skill=3`).
4. Direct profile load: `--fresh-player --save` now loads an existing profile, projects its points onto the live sheet before bootstrap, and drops file equipment so starter gear is not duplicated. Verified: stat3 profile loads with `stat=3`.

## Evidence (quiet batches, EXE `build-skills/dh-foundation.exe`, Preview 13 assets)
Scratch: `.local-inputs/claude-preview14/skills/` (runs/<name>/run.log, out/*.ppm/png, tools/dump_profile.exe).
- `click-s3` (profile stat=3, Strength release x2 at 30:154 frames 100/110): log `Source stat training stat=0 points=3->2 value=10->11 maxHP=165->165 maxMP=27->27`, then `Character menu action diagnostic: Stat point already assigned in this menu visit`. Capture `click-s3/out/stats-after.png`: Strength 10->11, Points left 2, R-Dam 13-17 -> 14-18, Attack rating 69 -> 72. Saved file: `stat=2 strength=11` (dump_profile).
- `click-zero` (stat=0): `Character menu action diagnostic: Source stat training refused: no Stat_Points remain`; file unchanged.
- `end-s3` (Endurance at 30:244): `endurance=9 hp=169.098` (was 165.098). Live max HP recalculated.
- `en-s3` (Energy at 30:290): `energy=4 mp=31.25` (was 27.25).
- `reload-s3` (profile = saved click-s3 file): loads `stat=2`, the points persist across runs.
- `skill-up` (skill=3, select row 0 at 30:82, Upgrade at 350:300): log `Source skill training row=0 rank=1->2 points=2`; `skills-after.png` shows Headsplitter 2, Skill points left 2, damage 14-21. Saved file (after fix): `skill=2 rank0=2`.
- `skill-zero` (skill=0): log `NativeSkillsTrainSkill source admission rejected training`; rank stays 1, points 0.
- Starter/fresh: `Direct player source points ... skill=1` on an existing save stays 1 (not debited to 0).

Not verified: the Upgrade's effect on a cast in the EXE (no cast run); the Stats glow/grey button states; the Skill Add button.

## Tests run
- `p14_build.ps1 -Name skills` build: exit 0.
- `ctest` (103 tests): `stat_training_v1` passes (`stat_training_v1 tests passed`: endurance spend recalc +4 HP, energy +4 MP, strength no vitals change, zero refusal, failed-save rollback, index 4 refused, hit regions, visit gate). `runtime_skill_session_training` passes. 102/103 overall; `session_skill_binding` fails at base too (see Open risks).

## Needs from schema v4
None. No save layout change. `save_store` fields used: `source_stat_points`, `source_skill_points`, `source_points_known`, `source_endurance_energy_known`.

## Package files required
None new. Uses the existing assets (`original-cache/data/pydata`, `windows-melee-bindings`, `windows-source-clock-v19-preview-9/assets` in the test).

## Verifier script
Profiles: `prof/stat3.save` (stat 3, skill 1), `prof/zero.save` (0,1), `prof/skill3.save` (0,3), `prof/nopts.save` (0,0), made with `tools/set_profile.exe in out stat skill`. Jobs: `runs/<name>/run.args` via `mkjob.sh`, batches via `mkjobs.sh` and `quiet_run.ps1 -JobsFile ... -Parallel N`.
Expected: `click-s3` log line above; `skill-up` `rank=1->2 points=2` and saved `skill=2`; `skill-zero` admission rejected.

## Level-up video observation (reference video Part 1)
- Contact sheets in `.local-inputs/claude-preview14/skills/video/`: quest completed "Kill 8 Bog Moths / Reward 20 EXP 150 GOLD" ~752 s; Stats page at ~778 s with "Warrior Lvl: 3", Points left 2 -> 1 -> 0 as strength goes 12 -> 13 -> 14 (the spend is one click per point; the points are shown decrementing live).
- Skills page ~782-790 s: Skill points left 2 -> 1, then at ~797 s a dialog "Confirm character point allocation?" (green check / red X) appears on menu close. This confirm is NOT implemented in the port (open item).
- The "LEVEL UP" banner itself was not located in the 735-778 s scan (1 s and 0.25 s samples). Not verified.

## Open items / gaps (not done)
- (3a) Per-level stat/skill point amounts per class: not extracted. The mechanism is the class recalc in `level_up`; no table written.
- (3b) Level-up presentation (MENU_LEVEL_UP status, portrait anim sprite 162, FX, tutorial flag) is not bound in the live `runtime_death_rewards_v1.cpp` path. Not implemented.
- The "Confirm character point allocation?" menu-close dialog seen at ~797 s is not implemented.
- Legacy saves with `source_skill_slots_known == false` and existing skill rows: `initialize_source_skill_rows_v1` refuses to reseed when rows exist, so a direct run would throw ("Direct Skills source rows"). Not changed (pre-existing); no legacy fixture exists to verify.
- Skill spends are saved per spend now; the menu-close path is not separately checked.
- Two level-up implementations (loot live path vs level-world) still differ.

## Open risks
- `session_skill_binding` ctest fails in this worktree: "Asset path escapes asset root" (`asset_catalog.cpp:81`). The worktree's `.local-inputs` junction resolves outside the root, so `within()` rejects it. It also fails in the base-commit build log (`.local-inputs`/`build-schema-ctest-base.log`), so it is environmental and not fixed here.
- Hit regions are measured from one frame (Points left 3); they match the captured boxes but were not checked on other stat layouts.
- Stat recalc keeps the current base level (profile projection rule) while the portable owner (`character_menu_stats_owner_v1`) resets it to the row; equal in practice here, but the two rules differ in principle.
- Line-ending warnings from git (LF/CRLF) on the new files.
