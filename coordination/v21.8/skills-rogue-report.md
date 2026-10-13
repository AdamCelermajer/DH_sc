# v21.8 skills-rogue: report and handoff (WIP, stopped on usage limit)

Branch `v21.8-skills-rogue`, worktree `.claude/worktrees/skills-rogue`, base `windows-foundation` @ aa2e8624.
Scope: active skills of the Archer and Assassin trees (Rogue base actives JumpKick, ViciousStrike, Roundhouse, Quickness are done and excluded).

Status: WIP. Code compiles (build OK). Only the profile unit test was executed (PASS). The activation session test, full CTest and every real hidden-window run were NOT executed. Nothing in this report is runtime-verified.

## Per-skill table

Evidence key: "unit" = executed test; "static" = Lua/class-table reading only.

| Skill | Tree | Status | Evidence |
|---|---|---|---|
| Blindness | Assassin | PARTIAL (code written, not executed) | static: prince_assassin_blindness.luac (BUFF_ID Skill_Assassin_Blindness, BUFF_FX spell_dh2_assassin_blindness_persist, BUFF_TIME = SnS_Duration cell 176; class projects 8000+2000/level ms, rank 1 = 10000 ms). Row in runtime_skill_use_profile_rogue_v1.hpp. unit: profile test PASS (transcription). Activation fixture case written, NOT run. Gaps: Lua SetIsTargetable(false) on caster not wired (class Special_Sneak cell 198 is applied and read by the existing target-facing check at runtime_skill_target_query_v1.cpp ~172); OnTargetHit break on own hit not wired. |
| PoisonSting | Assassin | PARTIAL (code written, not executed) | static: prince_assassin_poison_sting.luac (BUFF_TIME = Dot_Duration cell 125, fallback 4000; class projects 3000+750/level, rank 1 = 3750 ms). Row + duration_prop_index 125. unit: profile test PASS. Activation case written, NOT run. Gaps: weapon DOT tick and Slow tails; the port's consumption of Dot_*/Slow_* class cells on hits is unverified. |
| Assassination | Assassin | DID NOT WORK (not admitted) | static: OnSkill_ kill branch (en_level < level+2 and not boss -> target:Kill(self)). The Session has no kill primitive and no port boss/level gate. Admitting only the damage branch would hit the wrong outcome for weaker monsters, so no row. Unit test asserts it stays out. |
| GrapplingHook | Assassin | BLOCKED-PROJECTILE | static: SpawnSkillProjectile(GrapplingHook) + delayed pull/stun. |
| ExplodingArrows | Archer | BLOCKED-PROJECTILE | static: SpawnSkillProjectile(ArrowExplode), delayed explosion timer. |
| MultiHitArrow | Archer | BLOCKED-PROJECTILE | static: SpawnSkillProjectile(ArrowDefault) x3 volleys. |
| PinShot | Archer | BLOCKED-PROJECTILE | static: SpawnSkillProjectile(ArrowPinShot) + delayed second/third target timers. |
| PoisonArrows | Archer | BLOCKED-PROJECTILE | static: SpawnSkillProjectile(ArrowPoison). |
| Volley | Archer | BLOCKED-PROJECTILE | static: SpawnSkillProjectile(ArrowDefault) + rebound projectile. |

The projectile executor is `features/effects/runtime_session_projectile_v1` and is present on base. Only enemy ranged monsters (`features/enemy_ai/runtime_ranged_skill_monster_v1`) spawn through it. The player skill path (coordinator, main.cpp) does not. Wiring it is out of this batch's scope.

## Changes on the branch (WIP)

- `port/windows-foundation/features/generic_skills/runtime_skill_use_profile_rogue_v1.hpp` (new): `kBlindnessBuffV1`, `kPoisonStingBuffV1`, `kRuntimeSkillUseProfilesRogueV1` (2 rows), `find_skill_use_profile_rogue_v1`. Header comment lists the non-admitted rows and their reasons.
- `runtime_skill_use_profile_v1.hpp`: `RuntimeSkillBuffProfileV1` gets `int duration_prop_index = 176` (default keeps Quickness unchanged).
- `runtime_skill_cast_coordinator_v1.cpp`: include of the rogue header; `source_supported_skill` falls back to `find_skill_use_profile_rogue_v1`; buff duration reads `formula_sheet[profile.buff->duration_prop_index]`.
- `runtime_skill_use_profile_v1_tests.cpp`: rogue-table checks (row count 2, names/class tokens/buff classes in the authored name tables, exact pair lookup, Lua numbers, non-admitted skills stay out). Executed: PASS.
- `runtime_skill_activation_session_v1_tests.cpp`: `RogueSkillFixtureSpecV1` gets `class_id`, `buff_class`, `buff_duration_ms`, `buff_speed_cells` (trailing, defaults keep old call sites valid). The fixture uses `spec.class_id` (was the literal RoguePlayerBase). The buff branch is generalized; Quickness keeps its speed-cell checks. Two new specs: Blindness (anim root 518, min level 15, class RoguePlayerBase_Assassin) and PoisonSting (anim root 520, min level 18). NOT executed.

## Verification done

- Build: `bash /c/dh218/skills-rogue-build2.sh` (targets runtime_skill_use_profile_v1_tests, runtime_skill_activation_session_v1_tests, dh-foundation): BUILD_OK.
- `runtime_skill_use_profile_v1_tests.exe <worktree>/port/windows-foundation/../../.local-inputs/windows-shared-assets`: PASS.
- Not run: `runtime_skill_activation_session_v1` (CTest), full CTest (baseline 190), any EXE hidden-window cast.

## Half-done and exact next steps

1. Run `runtime_skill_activation_session_v1_tests` from bash (ctest -R runtime_skill_activation_session_v1 or run the exe with args `<worktree>/port/windows-foundation/../..` `1 <scratch>`). Expect `PASS Rogue Blindness` and `PASS Rogue PoisonSting` lines. If the subclass visual plan for `RoguePlayerBase_Assassin` fails in `build_original_combat_visual_plan` or the SkillTree lookup, that is the first failure to investigate (the fixture now takes the class from spec.class_id). Two failed attempts on the same reason: stop and report.
2. Confirm the class table values by runtime evidence: Blindness rank 1 = 10000 ms, PoisonSting rank 1 = 3750 ms (both are static projections, not yet measured).
3. Real hidden-window runs: NOT STARTED. No Assassin-subclass save exists (searched C:/dh217/seeds, C:/dh218/cases and .local-inputs for RoguePlayerBase_Assassin: none). The seeds are binary length-prefixed saves; the skill list must match the subclass tree. Needs a test-only save writer or the matrix Step 3 flags (`--test-class-subclass ROW`, `--test-grant-skill`, `--test-level`), which are design-only in SKILL-VERIFICATION-MATRIX.md. C:/dh217/tools/run-case.ps1 is hard-wired to windows-source-clock-v21.7; use skill_cast_runner_template.ps1 and the v21.8 staged package `.local-inputs/windows-source-clock-v21.8-staged`.
4. Full CTest from bash (~50 min, baseline 190). Not run.
5. Update the runtime-verified column of SKILL-VERIFICATION-MATRIX.md for the 9 skills. Not touched (the file says it is generated; do it by rerunning skill_static_check.py or by hand after the runs).
6. Commit: WIP commit made on this branch (see git log). Not merged (the lead merges).

## Known failures and blockers

- Assassination: DID NOT WORK (no kill primitive; see table).
- GrapplingHook and the five Archer actives: BLOCKED-PROJECTILE. Needs player-side wiring of the shared projectile executor (ProjectileTable rows ArrowDefault, ArrowExplode, ArrowPinShot, ArrowPoison, GrapplingHook; main.cpp spawn; contact/damage path). Not started.
- Blindness: untargetable (SetIsTargetable) and break-on-hit not reproduced.
- PoisonSting: DOT and slow tails not reproduced.
- The AGENTS.md "investigate before implementing" evidence (visual footage) was not gathered for any skill in this session. Static Lua and class-table evidence only.

## Recipes

- Worktree: `git worktree add .claude/worktrees/skills-rogue -b v21.8-skills-rogue windows-foundation` (done).
- subst: `subst R: <worktree absolute path>` (PowerShell). Remove with `subst R: /D`.
- Junctions: 25 junctions in `<worktree>/.local-inputs` from `C:/dh218/skills-a-junctions.txt`. Remove only with `cmd /c rmdir <junction>`. Never recursive-delete through them.
- Build: `bash /c/dh218/skills-rogue-build.sh` (configure + full build, from R:). Incremental targets: `bash /c/dh218/skills-rogue-build2.sh`.
- Build dir: `C:/dh218/skills-rogue-build`.
