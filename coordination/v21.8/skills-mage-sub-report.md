# v21.8 skills-mage-sub: Illusionist and Necromancer active skills (handoff, WIP)

Branch `v21.8-skills-mage-sub` (worktree `.claude/worktrees/skills-mage-sub`, subst `L:`), base `windows-foundation` @ aa2e8624.
Scope: the 8 active Illusionist/Necromancer skills (Eclipse, Figments, Terror, TimeControl; DeathClaw, FireSkullRain,
Pestilence, Rise). The base Mage skills (ColdRay, ChainLightning, StoneEscape, Cyclone) are owned by v21.8-mage and were skipped.
Status: STOPPED EARLY at the owner's usage limit. This is work in progress, not a completed batch.

## Per-skill table

| Skill | Tree | Status | Evidence / reason |
|---|---|---|---|
| DeathClaw | Necromancer | PARTIAL (admitted in code, unit test written, NOT compiled or run) | Lua read (`prince_necromancer_deathclaw.luac`). Row in `runtime_skill_use_profile_magesub_v1.hpp`. Gaps: knockback state, search origin offset (0,-40,0). Cooldown is the Lua constant 1500 ms via `fixed_cooldown_ms`, because the ClassTable `Skill_Necromancer_DeathClaw` has no SnS_Cooldown row (classdump probe). Real run: NOT STARTED (no Mage-subclass save). |
| Figments | Illusionist | PARTIAL (admitted in code, unit test written, NOT compiled or run) | Lua read (`prince_illusionist_figments.luac`). Row admitted; cooldown from class projection (SnS_Cooldown raw 640000 = 2500 ms per classdump). Gaps: SetStunState on targets, per-target enemy buff FX and duration, DoT class damage. Real run: NOT STARTED. |
| Eclipse | Illusionist | DID NOT WORK (not implemented) | Needs an enemy-target buff arm (CreateBuff/ApplyBuff on each enemy in 600 range), per-target PlayFX, and the "cannot move or target MC" effect, which has no AI state in the port. |
| TimeControl | Illusionist | DID NOT WORK (not implemented) | Needs enemy slow buff on non-boss, non-miniboss targets (target-side buff arm; boss filter), plus self ApplyPropClass. Unit-only path is possible; real run not attempted. |
| Terror | Illusionist | DID NOT WORK (not implemented) | Needs an AI faction flip (FactionBuff plus AIStates) and a 3-slot conversion memory with timers. No faction state in the port. |
| FireSkullRain | Necromancer | DID NOT WORK (not implemented) | Needs a repeating 1000 ms tick timer (max 3 ticks, stops when empty). No owner for that. Not a projectile in the Lua (PlayFX on target), so no BLOCKED-PROJECTILE. |
| Pestilence | Necromancer | DID NOT WORK (not implemented) | Needs an on-death hook (OnCheckDead_) that explodes in radius 200, plus DoT class ticks. Not implemented. |
| Rise | Necromancer | BLOCKED-PROJECTILE (plus summon) | Rise uses `SpawnSkillProjectile(TeleportTool)` and the spawn position comes from its OnDelayedSkill callback, so it needs the shared projectile executor (on v21.8-mage, not merged). Also needs a summon arm for 3 RisenZombie. |

Admission is also covered by a negative test: the six not-admitted active rows and the base Mage rows must return nullptr.

## What changed (uncommitted until the WIP commit below)

- `features/generic_skills/runtime_skill_use_profile_v1.hpp`: one trailing field `float fixed_cooldown_ms = 0.0f` on `RuntimeSkillUseProfileV1` (existing rows unchanged).
- `features/generic_skills/runtime_skill_use_profile_magesub_v1.hpp` (NEW): `kRuntimeSkillUseProfilesMageSubV1` (DeathClaw, Figments) and `find_skill_use_profile_magesub_v1`.
- `features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`: include the new header; `source_supported_skill` falls back to the magesub lookup (one line); the cast start applies `fixed_cooldown_ms` after the class projection (one line).
- `features/generic_skills/runtime_skill_use_profile_magesub_v1_tests.cpp` (NEW) and CMake target `runtime_skill_use_profile_magesub_v1` (added to `CMakeLists.txt`).
- `features/generic_skills/runtime_skill_activation_session_v1_tests.cpp`: `RogueSkillFixtureSpecV1` gained `class_row`, `profile_row`, `skin` (defaults keep the Rogue values); the function uses them; two new fixture runs for DeathClaw (`MagePlayerBase_Necromancer`, root 415, level 21) and Figments (`MagePlayerBase_Illusionist`, root 417, level 18). These are NOT yet compiled or run. The existing `"first volley did not hit two distinct targets"` check may not fit single-target skills.

## Not yet verified (honest state)

- NOTHING from this branch has been compiled since the edits. The baseline build (before any edit) finished OK (`C:/dh218/skills-mage-sub-build`, 1127 targets).
- Baseline CTest on the unmodified tree (`C:/dh218/skills-mage-sub-ctest-base.log`) was still running at handoff: 126/190 started, no failures seen in the log so far. It was not completed.
- No hidden-window real run for any skill in this batch (see blockers).

## Real-run blockers (not solved)

1. No Mage-subclass save exists. The only Mage save is `C:/dh218/cases/mage-final-src/profile.save` (class `MagePlayerBase`, base tree). Subclass skills need class `MagePlayerBase_Necromancer` (CharacterTable 292) or `MagePlayerBase_Illusionist` (291), and the skill row must be in that SkillList.
2. There is no test flag for grant-skill or class/subclass. `main.cpp` has `--test-skill-points`, `--test-xp`, `--test-player-hp`, `--spawn-test`, `--skill-key-frame` only. The matrix Step-3 design (`--test-grant-skill`, `--test-class-subclass`) is NOT implemented.
3. The "test tool" that built the skills-a Rogue saves is not in the repo. Use the `save_character` / `load_character` path as the activation fixture does.

## Data facts found (for the next agent)

- classdump probe: `C:/dh218/probe/classdump.exe <pydata dir> <name substring>` (needs the llvm-mingw bin on PATH). Output for `Skill_Necromancer_DeathClaw`: SnS_ManaCost, SnS_MinDamage/MaxDamage, TempProp1 (range), no SnS_Cooldown row. `Skill_Illusionist_Figments`: mana, damage, DoT min/max and duration, SnS_Cooldown 640000 raw. `Skill_Illusionist_Figments_buff` has no property rows (FX-only).
- Mesh skin for mage: `_default_mage-mesh-skin` (found in staged assets).
- SkillTable mask word[7] for DeathClaw and Figments is 0x5554A, so the paired-hand bit 0x800000 is not set.

## Exact next steps

1. Commit state (done in WIP commit). Then `cmake --build` in `C:/dh218/skills-mage-sub-build` (configure is cached; the new CMake target needs a re-configure, which runs automatically).
2. Run `runtime_skill_use_profile_magesub_v1_tests.exe <L:/.local-inputs/windows-shared-assets>` and fix compile errors. Check the `kRuntimeSkillUseProfilesV1` table size check still passes (7).
3. Build and run `runtime_skill_activation_session_v1_tests` (args: repo root, 1, scratch dir). Check the DeathClaw and Figments fixture runs. Fix the "two distinct targets" check if it applies to single-target skills.
4. Full CTest from bash (`ctest --test-dir C:/dh218/skills-mage-sub-build`, toolchain bin on PATH). Target: baseline 190.
5. Then decide the grant/subclass test flags (matrix Step 3) for real runs, or accept unit-only evidence for DeathClaw and Figments.
6. Escalate the six DID NOT WORK / BLOCKED skills to a stronger model. Eclipse and TimeControl need a target-buff arm. FireSkullRain needs a tick timer. Pestilence needs a death hook. Terror needs AI faction. Rise needs projectile (v21.8-mage) plus summon.

## Build / test / run recipes

- Subst: `L:` mapped to the worktree (PowerShell `subst L: <worktree>`; remove with `subst L: /D`).
- Junctions: created for every top-level dir of primary `.local-inputs` under the worktree `.local-inputs`. Remove each with `cmd /c rmdir <path>` (never recursive delete).
- Build: `bash /c/dh218/skills-mage-sub-build.sh` (writes `skills-mage-sub-build.log`; `BUILD_OK` on success).
- CTest: `PATH=<llvm-mingw bin>:$PATH ctest --test-dir C:/dh218/skills-mage-sub-build -j4 --timeout 900` from bash.

## Known failures / risks

- Two base CTest targets (`private_save_transport_win32`, `source_level_config`) failed on skills-a for missing inputs; the junctions in this worktree were made to cover the primary `.local-inputs` dirs, so they may pass. Not verified here.
- The new fixture runs for DeathClaw and Figments are untested and may fail on the Mage visual/combat binding path (`MagePlayerBase` melee bindings exist; the skin is assumed).
