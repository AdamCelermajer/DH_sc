# Preview 14 implementation brief (read with your stream's survey report)

User direction (2026-10-10): feature work resumes; the user approved the root's recommendations for all open decisions:
1. No HUD minimap in this preview (the original has none during play). Map page later.
2. Faery scope: Celest + Hotty; locked Faery rejected in provider AND UI.
3. Saves: legacy slots show blank metadata until next save; save format v4 is allowed (older EXEs need not read it); F5 in combat also rewrites the slot file.
4. Pickup on PC: walking into an item targets it (as the original); pick up with a key press while targeted (PC adaptation; label it as such).
5. Missing pickup/drop WAVs: stay silent, document, never substitute.
6. Quests: thin Windows runtime (wave 2).

## Where you work (isolation, important)
You have your OWN git worktree and build directory; never touch another worktree or the main tree `C:/Users/adamc/Desktop/workspace/DH_sc` (except reading `.local-inputs`, which every worktree sees through a junction; NEVER delete/rename/recursively remove anything inside `.local-inputs` or the junction itself).
- Worktree: `C:/Users/adamc/Desktop/workspace/DH_wt/<name>` on branch `p14/<name>` (a full checkout; edit files there with absolute paths under it). The source is `<worktree>/port/windows-foundation`.
- Build + tests: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name <name> [-Test]` (private build dir `DH_wt/build-<name>`, first build about 75 s, incremental fast; `-Test` runs ctest). You ARE allowed to build and run ctest in your own build dir. The EXE is `DH_wt/build-<name>/dh-foundation.exe`.
- Run the game only through the quiet runner `<worktree>/port/windows-foundation/tools/quiet_run.ps1` (hidden desktop, silent, parallel; see coordination/claude-preview13/QUIET-RULES.md). Use absolute paths in args files; use the Preview 13 package assets for content: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/windows-source-clock-v19-preview-13/assets` (+ `audio-assets`, `ui-assets`); copy args patterns from `.local-inputs/claude-preview13/verify-final/runs/*/run.args` and `.local-inputs/claude-preview13/quiet-test/`. Never write into the Preview packages or the user's saves; use copies in your own scratch folder `.local-inputs/claude-preview14/<name>/`.
- Commit your work on your branch in small commits (`git -C <worktree> add -A; git -C <worktree> commit`), never push, never switch branches, never run git commands that touch other branches. The root merges all branches. Keep your diff focused; avoid reformatting or moving code so merges stay clean. Before you finish, make sure `p14_build.ps1 -Test` is green (all ctests pass) and that you have committed.
- `main.cpp` and `CMakeLists.txt` are shared by every stream: add small, clearly commented hunks with unique anchors; put logic in your own new files under `features/<yours>/`.
- Save-format changes: ONLY the `schema` stream edits CharacterState/SaveStore/GameSave schema. Other streams must not change save layouts; if you need new persisted fields, write them down in your report under "Needs from schema v4" and use a clearly named temporary default.

## Rules that still apply (COMMON-BRIEF.md, P13-BRIEF.md lessons)
- Investigate original behaviour first (IDA pseudocode `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`, reference video frames via ffmpeg, the survey), implement the original rules, no invented values/sounds/art; unknown behaviour stays an explicit, logged limitation.
- Reusable systems (no map/class-specific hacks), minimal diffs, tests for outcome branches and failures, regenerate generated files only through their exporters and diff to prove unrelated output is unchanged, list "Package files required" (assets the fix needs at runtime).
- Verify in the real EXE: run quiet batches with your build's EXE, LOOK at captured frames, quote log lines. Say plainly what is not verified.
- Keep your context lean: grep/limited reads; write your report skeleton early.

## Report (required)
`coordination/claude-preview14/<NAME>-report.md` (inside YOUR worktree's copy of that folder, plus commit it): what you changed (files/hunks), tests run with real output, evidence (log lines, image paths with what you saw), "Needs from schema v4", "Package files required", "Verifier script" (exact quiet-batch jobs and expected observations), open risks. Return a summary under 200 words.

## CONTINUATION NOTE (read this if you were started as a continuation worker)
The previous worker for your stream was killed by an API rate limit mid-task. Its work is committed as a WIP snapshot on your branch (`git -C <worktree> log --oneline cedb3a99..HEAD`, `git -C <worktree> diff cedb3a99 HEAD --stat`). Review that diff FIRST (limited output), decide what is finished, correct, and consistent, build it (`p14_build.ps1 -Name <name> -Test`), fix compile/test failures, and continue with the remaining tasks of your original assignment. Do not redo finished work; do not restart from scratch. Commit early and often (rate limits can strike again; uncommitted work is at risk). Write your report skeleton to `<worktree>/coordination/claude-preview14/<NAME>-report.md` EARLY and keep it updated, so a successor can continue from it.
