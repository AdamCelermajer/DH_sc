# v20 integration sync report (2026-10-11)

Scope: bring the not-yet-integrated `+` commits of `fix/b068`, `codex/v20-death-flow` and `codex/v20-gate-teleport-fx` into `v20/integrate` (worktree `DH_wt/v20int`). Start HEAD `56a77705`. Nothing pushed; main and CMake were not edited.

## Commit decisions

`git cherry v20/integrate <branch>` counts 13 `+` commits on `fix/b068` (not 14), 2 on `codex/v20-death-flow`, and 8 on `codex/v20-gate-teleport-fx`. Each was checked against the touched files, not by subject.

### fix/b068

Code:
- `e5d7a2a9` (preserve source logical death, checkpoint v4): SKIPPED, semantically present. The code hunks in `actor_combat_runtime.cpp`, `actor_state.{hpp,cpp}`, `game_save.{hpp,cpp}` and the tests are textually identical to `c0962a2e`, which is already on `v20/integrate` (same subject, different patch id because the parent differs). Its report hunks are contained in the integrated B068 report snapshot (see below).
- `3b821a9b` (script_source_services): already integrated (not a `+` commit).

Report/evidence only:
- `3efea2ae`, `0da8654e`, `ac1e4aae`: SKIPPED, content already present. `coordination/claude-preview161/B068-report.md` in `v20/integrate` was identical to `fix/b068` at `3b821a9b`, which is a descendant of these three; every added line was found in it.
- `344fd43b` -> `fc0b0d1f` (per-scene ledger). CHERRY-PICKED.
- `a853bbb5` -> `e5a6cbbf` (Witch trigger/dense reference boundary). CHERRY-PICKED.
- `7ad2c04b` -> `29e46f1e` (stable target cache). CHERRY-PICKED for the inventory hunk. Its report hunk was NOT applied: the integrated report already contains it, and the 3-way conflict was resolved by keeping HEAD. Because of that, `29e46f1e` has a different patch id from `7ad2c04b`, so `git cherry` still lists `7ad2c04b`.
- `45954168` -> `4ab90513` (per-scene combined outcomes). CHERRY-PICKED.
- `15ea7550` -> `66bb7871` (facing regression, plain emitter contract). CHERRY-PICKED.
- `ca051f25` -> `6c0031ab` (deactivation polarity, prisoner runtime failure). CHERRY-PICKED.
- `8598a938` -> `d98e09dc` (Serpent match, prisoner gate). CHERRY-PICKED.
- `35c93c6e` -> `8cecdc52` (source initial enabled state, condition side effects). CHERRY-PICKED.
- `48c721f8` -> `99d8e416` (B068 stop handoff). CHERRY-PICKED.

Verification: `coordination/claude-preview161/B068-report.md`, `B068-Act1-cinematic-inventory.md` and `B068-package-assets.json` now equal `fix/b068` byte-for-byte (`git diff` empty). The package-assets file was already identical.

Process note: a first pass of this replay took the commit's full file version for `7ad2c04b`'s report conflict, which dropped 15 lines. That was caught before any push; the local unpushed commits were reset to `56a77705` and replayed with the corrected rule. The history above is the replayed one.

### codex/v20-death-flow

- `9a5a3aa6` -> `4aa3a127` (full video death-screen candidate scan). CHERRY-PICKED, clean.
- `afd9fca1` -> `8c5e3b1d` (TriggerObject checkpoint eligibility). CHERRY-PICKED, clean.
- `coordination/v20/DEATH-FLOW-report.md` now equals the branch tip. Both commits touch only that file. No code changed on this branch in the `+` set.

### codex/v20-gate-teleport-fx

Code:
- `88754883` (name FX deletion anchor state): SKIPPED, semantically present as `24496896` (identical hunks in `source_fx_anchor_registry_v1.{hpp,cpp,_tests.cpp}`).

Report/evidence/assets:
- `9f8de152`: SKIPPED, content present. `coordination/v20/GATE-TELEPORT-FX-report.md` in `v20/integrate` equals the branch snapshot at `88754883`, which is after `9f8de152`.
- `bb3c5070` -> `240986d6`, `368cabba` -> `22d1a666` (also adds `GATE-TELEPORT-FX-assets.json` and the python check), `8ea549cc` -> `f281c989`, `3067e94c` -> `e3d7e5b8`, `0afbe5bd` -> `9360b417`, `60e60e85` -> `8c5bb59c`. CHERRY-PICKED, all clean.
- The branch's three evidence files now equal the branch tip.

Note on the description in the task: the `+` set contains no registry code beyond `88754883`. The other items named (full-width anchor IDs, native dead check, lazy dummy/waypoint generation, rotation/scale normalization) came from commits that are already integrated (`65765da4`, `82ac8ff2`, `49099359`, `11db4b75` and related). They were not re-audited in this session.

Asset check: `py -3 -I port/windows-foundation/features/effects/source_script_fx_assets_v1_tests.py` exit 0; final line `PASS | exact set 132, 119, and 120 projections and finite I028 asset manifest`. This python file is not registered in CMake.

## Conflicts resolved

Only the B068 report/inventory and the gate/death evidence files conflicted, all append-style or one-line rewrites, resolved as described above. No conflict touched `main.cpp`, `CMakeLists.txt`, or the `v20int` WIP. `git diff 56a77705 HEAD -- port/windows-foundation/main.cpp port/windows-foundation/CMakeLists.txt` is empty. The WIP condition lifecycle and selective checkpoint snapshot code are unchanged.

## World gate audit (`world_gate_v1` helpers and main adapter)

Full detail with exact pseudocode and source lines is in `coordination/v20/INTEGRATION-report.md`, section "World gate TestEnable/TestDisable audit against source". Summary:

- Agrees: enable runs before disable; a false activation disables; a false deactivation disables and latches; a true deactivation does not re-enable; the skip rule (both tests idle); latches reset on bind; ctor initial enabled=true.
- Defect, FIXED (commit `e57a4e0a`): `source_test_enable_condition_v1` left `enabled` unchanged for an absent activate list. Source IsTrue (`0x33e5f8`) returns 1 for an absent list, so TestEnable calls SetEnable(true). The old test could not catch it because the object was already enabled. The test now starts disabled. The helper is test-only; main uses only the lifecycle step, so runtime behaviour is unchanged.
- Open, not changed: PlayerSave readiness and current-level guards (TestEnable `0x33e6d4` l.41171-41181, TestDisable `0x33e61c` l.41140-41145) are not modelled; the adapter runs the tests only at bind and on flips, not per ObjectManager update; unresolved conditions fail closed.

## Build and test

Command: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name v20int -Jobs 4 -Test`.

- Build: `build exit 0`, all targets built (`[1/9]`..`[9/9]`, including `world_gate_v1` and `dh-foundation.exe`).
- The script's own wrapper returned exit 1 and printed only `Errors while running CTest`. Its ctest pipeline turns ctest stderr into a PowerShell error record and hides the summary. This is a wrapper issue; ctest itself was rerun directly (below).
- CTest, run directly with the llvm-mingw bin on PATH and `--timeout 120 -j 6`: exit 8. 161 tests: 160 passed, 1 failed.
- Failed: `session_skill_binding` (test 50). Message `Supply repository root`, the known worktree asset-root case. Expected per handoff.
- `world_gate_v1` passed under ctest (test 15).

## Final HEAD

Commits in this session, oldest first: `fc0b0d1f`, `e5a6cbbf`, `29e46f1e`, `4ab90513`, `66bb7871`, `6c0031ab`, `d98e09dc`, `8cecdc52`, `99d8e416`, `4aa3a127`, `8c5e3b1d`, `240986d6`, `22d1a666`, `f281c989`, `e3d7e5b8`, `9360b417`, `8c5bb59c`, `e57a4e0a`, `ed7da7c6`, and this report commit (final HEAD, see `git log -1`).

Still listed by `git cherry` (intentionally skipped as content-present): `fix/b068` `3efea2ae`, `e5d7a2a9`, `0da8654e`, `ac1e4aae`, `7ad2c04b` (report hunk); `codex/v20-gate-teleport-fx` `9f8de152`, `88754883`.
