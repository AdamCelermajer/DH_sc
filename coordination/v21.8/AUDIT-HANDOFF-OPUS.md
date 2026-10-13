# Full code audit: handoff and prompt for a fresh Opus 5.5 (high effort) session

Written 2026-10-13. Act 1 of the Dungeon Hunter II Windows reconstruction is complete. The goal of this session is an AUDIT and refactor plan (then the refactors) so that acts 2+ are much cheaper to port. Nothing here should change shipped behaviour: every refactor must keep the full CTest green and the act-1 runtime behaviour identical.

## Repo facts
- Repo: `C:\Users\adamc\Desktop\workspace\DH_sc`, branch `windows-foundation`. C++ in `port/windows-foundation/` (~82 top-level .cpp files, ~652 .cpp files under `features/`, `main.cpp` is ~9600 lines).
- Read first: `AGENTS.md`, `RECONSTRUCTION-HANDOFF.md`, `docs/BUGS-AND-IMPLEMENTATION.md` (bug tracker, B001..B157), `port/windows-foundation/MILESTONES.md`, `coordination/v21.7/*` and `coordination/v21.8/*` (reports, Mud Lord handoff `B155-B156-HANDOFF.md`, `CHARACTER-COVERAGE-MATRIX.md` if present).
- Build: llvm-mingw clang++ (in `.local-inputs/windows-toolchain/`), CMake 3.22 + Ninja from the Android SDK, Release. CMake needs forward-slash paths. A `subst U:` drive for the worktree path avoids length problems. See `coordination/v21.7/B155-B156-HANDOFF.md` section 4 for exact commands. Tests: CTest, 183 expected to pass. Several tests read `.local-inputs/<dir>` inputs, which must exist in whatever worktree you use (make them with junctions: `New-Item -ItemType Junction`; remove with `cmd /c rmdir`, NEVER recursive-delete through them).
- `.local-inputs/` holds original game data and tools; it is not versioned and may contain junctions. Do not recurse into it with delete/format tools.
- Worktrees live in `.claude/worktrees/*`; one branch per task; merge into `windows-foundation` by hand; nothing is pushed.
- Process rules from the owner: for each bug, investigate the original (IDA via the rea MCP tools and the gameplay video) for the full logic first, then find what is wrong in our code, then fix. Sonnet-class agents for bugs, Haiku-class for mechanical implementation, a fresh session plus an independent reviewer per change.

## What the audit must cover
1. `main.cpp` (~9.6k lines): inventory every block by responsibility. Identify what should be moved behind feature owners in `features/*` (boss admission/targetability/AI binding, world restore and lifecycle/limbus restore, level transition, save/load, HUD, input, projectile/FX drawing). Propose and carry out safe extractions in small, test-covered steps.
2. Duplication and ad hoc code across features: skills, faery/companion behaviour, enemy AI (`features/enemy_ai`), bosses (`features/bosses`: `runtime_boss_encounter_plan_v1`, `runtime_swampking_core_ai_v1`, `runtime_timer_trap_v1`), status effects, projectiles, timers. Goal: a REUSABLE boss framework driven by the original Lua scripts and data tables (AI script parse, skill tables, phase plan, timer traps, intro/outro cutscene hooks) so a new boss in acts 2+ needs data + a small script binding, not new main.cpp code. Use the Mud Lord (SwampKing) and the witch (B154) as the two worked examples and compare how each was wired.
3. Level transition / level loading path (`Level::_LoadFromXML`, LoadTemplate providers, campaign host, save components such as `campaign_limbus_v1`): document it, remove special cases, and make adding a level a data change.
4. Naming, file layout, header hygiene, dead code (old preview/hotfix scaffolding, `*_v1` suffix proliferation, unused CMake targets, stale tests that only mirror implementation), build time, and test-suite structure (tests that depend on `.local-inputs` paths: centralise the asset-root resolution).
5. Documentation: one short architecture doc (`docs/ARCHITECTURE.md`) with the ownership map, data flow for a frame, how to port a new level / enemy / boss / skill / faery, and a checklist for "porting act N".

## Deliverables
- `coordination/v21.8/AUDIT-REPORT.md`: findings ranked by value/risk, with file:line anchors, and the proposed refactor sequence.
- Refactors as separate small commits on a branch `v21.8-audit` (each keeping ctest green and the hidden-window act-1 smoke run unchanged), plus `docs/ARCHITECTURE.md`.
- Update `docs/BUGS-AND-IMPLEMENTATION.md` with any behaviour bug discovered (do not silently fix behaviour).

## State of the other work (do not collide)
- Act-1 wrap-up (Mud Lord damage calibration, Darkwood LoadTemplate provider) is on branch `v21.7-mudlord` and is being merged into `windows-foundation` with the commit "end of act1". Start the audit from `windows-foundation` AFTER that commit.
- Character completeness work (classes/subclasses/skills/faeries/sound) runs in parallel on separate branches and merges after "end of act1". Avoid large moves in `features/generic_skills`, `features/skills_animation`, `features/companions`, `features/faery_menu` until that work has merged, or coordinate: do `main.cpp` and bosses/levels/transitions first.

## Prompt to paste into the new session
```
You are auditing and cleaning up the Dungeon Hunter II Windows reconstruction at C:\Users\adamc\Desktop\workspace\DH_sc (branch windows-foundation, after the commit "end of act1"). Act 1 is finished; the goal now is to make the codebase clean, simple and reusable so that porting acts 2+ (levels, enemies, bosses, skills, faeries) is far easier. Read coordination/v21.8/AUDIT-HANDOFF-FABLE.md completely, then AGENTS.md, RECONSTRUCTION-HANDOFF.md, docs/BUGS-AND-IMPLEMENTATION.md and the v21.7/v21.8 coordination reports. Work in a worktree on branch v21.8-audit. First produce coordination/v21.8/AUDIT-REPORT.md (ranked findings, refactor sequence, risks), then execute the refactors in small commits, keeping the full CTest (183+) green and the act-1 behaviour identical; verify with the hidden-window harness where a refactor touches runtime paths. Priorities: (1) shrink main.cpp by moving responsibilities into features/*, (2) a reusable data/script-driven boss framework using SwampKing and the witch as worked examples, (3) a data-driven level transition/loading path, (4) dead code and naming cleanup, (5) docs/ARCHITECTURE.md with a "port act N" checklist. Do not change gameplay behaviour; record any bug you find in the tracker. Never recursive-delete under .local-inputs (junctions). Use independent reviewers for each refactor batch.
```
