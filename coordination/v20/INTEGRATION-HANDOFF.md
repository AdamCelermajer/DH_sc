# v20 integration handoff — stop checkpoint

This is a WIP checkpoint for the handoff to Claude. It is not an accepted release and does not claim the v20 runtime audit is complete.

- Worktree: `C:/Users/adamc/Desktop/workspace/DH_wt/v20int`, branch `v20/integrate`.
- Parent HEAD before this checkpoint: `37c617f8`.
- Frozen diagnostic executable: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v20/integration/condition-lifecycle-fb1/dh-foundation.exe`.
- SHA-256: `FB1EFF2FDD29E3E852F0B99015AA6614342258C119FFEDAEAFAF04BFA62A6271`.
- The executable was built from parent HEAD plus the uncommitted integration changes listed below. The exact patch is committed by the WIP checkpoint commit that contains this file.

## WIP changes committed here

- `port/windows-foundation/main.cpp`: retains condition-gated Character declarations, evaluates source enable/disable lifecycle on initial watch and condition flips, and writes a selective source checkpoint snapshot on checkpoint-zone contact. It does not yet wire the death screen or selective checkpoint restore.
- `port/windows-foundation/features/world_gates/world_gate_v1.{hpp,cpp}` and `_tests.cpp`: add pure source TestEnable/TestDisable state helpers and branch tests.
- `port/windows-foundation/CMakeLists.txt`: registers the death checkpoint snapshot/zone and fade sources/tests.
- `coordination/v20/INTEGRATION-report.md`: records source evidence, visual attribution limits, current integration state, and remaining gates.

## Verification performed before the stop request

- `p14_build.ps1 -Name v20int -Jobs 4`: exit 0; incremental invocation reported `ninja: no work to do`. The full Ninja invocation had linked the main target successfully (`[9/9]`).
- Focused executables passed: `world_gate_v1_tests.exe`, `death_runtime_v1_tests.exe`, `death_checkpoint_zone_v1_tests.exe`, `death_checkpoint_snapshot_v1_tests.exe`, `door_collision_v1_tests.exe`, `door_collision_geometry_v1_tests.exe`, and `door_runtime_restore_v1_tests.exe`. Full CTest was not run.
- Two hidden, silent, fresh-save runs used `quiet_run.ps1`, normal compiled-bank `001_swamp` with `--campaign-triggers`, and the source-stage assets. Both completed 2,000 frames and exited 0:
  - `.../condition-lifecycle-fb1/before_kill_true/run.log`: `BeforeKillWitch` row 52 = 3; log reports `_prim_NPC_05` condition true and enabled, retains name lookup, and starts `Swamp_Witch_Activate`.
  - `.../condition-lifecycle-fb1/before_kill_false/run.log`: row 52 = 13; log reports `_prim_NPC_05` condition false and disabled.
  - Summary: `.../condition-lifecycle-fb1/summary.json`. Captures are present at frames 30 through 2000. They were not visually inspected in this stop handoff, so these runs verify startup/lifecycle logs, not the rendered NPC appearance.

## Source correction that must remain explicit

The latest B068 source audit (`fix/b068` report commit `35c93c6e`, clean B068 HEAD `48c721f8`) confirms the source semantics: `TestEnableCondition` calls `SetEnable(false)` when its activation condition evaluates false; it is not a no-op. `TestDisableCondition` is separate and calls `SetEnable(false)` when its deactivation condition evaluates false. ObjectManager calls enable testing before disable testing. Preserve those distinct ordered rules, plus the source PlayerSave readiness/current-level/inhibit/tested guards. The integration helper's false-enable branch currently disables, and the false-disable branch also disables; no behavior changes were made after the stop request.

## Outstanding work; do not mark complete

- Review exact source guard coverage for TestEnable/TestDisable; the current main adapter has not reproduced every owner guard or every authored family.
- Visually inspect the true/false NPC captures and run the authored Witch condition transition after the root-requested stop is lifted.
- Main death-screen presentation/input, READY-state admission, frame-88 native restore, checkpoint-zone producer persistence and selective restore are not production-complete. Current checkpoint code stores a companion snapshot, but no same-session source restore path is wired.
- Script PlaySound/StopSound/SaveGame integration has helper contracts but main bindings and normal source sequence checks remain incomplete.
- Swamp cage effect sets 119/120 need per-cue target/opaque-anchor/result logs and adjacent captures; the earlier E8F1 300-frame check stopped before them.
- Door traversal, full Door_Teleport normal approach, Witch framing, follower/fresh-resume, full cinematic acceptance, and final full CTest remain open. Do not package, publish, or claim gameplay closure from this checkpoint.
- No additional game runs or implementation were started after root's stop request. No user game window or accepted package was changed.
