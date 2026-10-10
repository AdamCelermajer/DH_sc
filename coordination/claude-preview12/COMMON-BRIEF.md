# Preview 12 bug-fix batch: common brief for every worker

Repo: `C:/Users/adamc/Desktop/workspace/DH_sc`, branch `windows-foundation`, HEAD `1db50a30`.
Read first: `AGENTS.md` (investigate-before-implement rule), `docs/BUGS-AND-IMPLEMENTATION.md`
(your bug rows), and `docs/CLAUDE-BUG-INVESTIGATION-B037-B041-2026-10-10.md` (root-cause analysis with IDA
addresses; treat it as a strong lead, but verify what you rely on). The user wants **bug fixes only**, no new features.
Preview 11 (`.local-inputs/windows-source-clock-v19-preview-11`) is accepted and immutable.

## Roles in this flow
1. **Implementer** (you, if you were given a bug): investigate original behaviour (IDA + reference video),
   patch, run focused tests, write a report. Do not claim the bug closed.
2. **Verifier** (separate agent, later): independently re-runs tests, reviews the diff against the evidence, and
   launches the game in an isolated window to check. 
3. **Fidelity** (separate agent, later): compares behaviour/visuals with the reference video.
The root (Claude) integrates, builds the main EXE, packages the preview and updates the tracker.

## Evidence sources
- IDA pseudocode (ARM32 original, symbols present):
  `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c` (36 MB; use Grep with
  function names such as `CharAI::AI_BeginSkill`), plus `full-listing.asm`, `elf-symbols.jsonl`.
  Handoff: `.local-inputs/ida-apk-export-2026-10-07/HANDOFF-TO-MAIN-SESSION.txt`.
- Reference video (Act 1 walkthrough, v1.0.3, 640x360 30 fps, 22 min):
  `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`
  Extract frames with ffmpeg and look at them with the Read tool (images are viewable). Use a short run of frames
  when timing matters, e.g.
  `"C:/Users/adamc/AppData/Local/Microsoft/WinGet/Packages/Gyan.FFmpeg_Microsoft.Winget.Source_8wekyb3d8bbwe/ffmpeg-8.1.2-full_build/bin/ffmpeg.exe" -y -ss 377 -i "<video>" -t 3 -vf fps=6 out/f_%02d.png`
  Put extracted frames under `.local-inputs/claude-preview12/<your-bug>/` (git-ignored).
  Pre-made contact sheets (6 frames, 4 s apart, timestamp burned in; sheet = floor(t/24)+1) are in
  `C:/Users/adamc/Desktop/dh2_video_research/sheets1/sNNN.jpg`. The video is v1.0.3; the recovered code is v1.0.2.
  Never claim a video comparison unless you actually looked at frames; say what you saw and at which timestamp.
- Existing feature reports under `port/windows-foundation/features/**` and `reports/`; search before re-deriving.

## Build and test rules (important: many workers share one tree)
- Compiler: `.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe`. Follow the pattern of
  the existing `run_*_tests.ps1` runners (standalone compile of the feature + its test into
  `.local-inputs/<name>-build/`, strict `-Wall -Wextra -Werror`). Create your own uniquely named build folder and
  runner; never reuse another worker's folder.
- **Do NOT run ninja/cmake on `.local-inputs/windows-foundation-build`** and do not rebuild `dh-foundation.exe`.
  The root does the integrated build serially. To check that your `main.cpp` change compiles, run
  `ninja -C .local-inputs/windows-foundation-build -t commands dh-foundation.exe` (ninja is at
  `%LOCALAPPDATA%/Android/Sdk/cmake/3.22.1/bin/ninja.exe`), take the compile line for `main.cpp`, and run it with
  `-fsyntax-only` and without `-o`. Put the toolchain `bin` on PATH first.
- Python (if needed): `C:/Users/adamc/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe`.
- Never kill, launch against, or automate the user's live game (a `dh-foundation.exe` from preview-11 may be running).
  Do not touch any `*.save`, `*.dhsave` or profile files outside a new test folder. Use new/copied saves only.
- If you need a real window: run an isolated process with a fresh output folder and `--frames N`, capture with the
  existing `--capture` option, and read the PPM/PNG. Copy the preview-11 `swamp.args` pattern, but point saves at your
  own folder.
- `port/windows-foundation/main.cpp` is ~290 KB and edited by several workers at once. Edit it **only with small,
  surgical Edit calls** (unique anchors, re-read the region immediately before editing). Never rewrite or reformat
  the whole file, never `git checkout`/`git stash`/`git reset` anything. Edit only your own files plus the minimal
  hunks you must add elsewhere. If a hunk is large, put the logic in your own feature file and call it from main.
- `port/windows-foundation/CMakeLists.txt` is shared: add your new sources/tests with a minimal, clearly-commented
  addition only if required.
- Do not edit `docs/BUGS-AND-IMPLEMENTATION.md`, `docs/RESOLVED-BUGS.md`, or MILESTONES. Do not `git commit`.
- No fabricated values, no invented sounds/art/timings. Unknown behaviour stays an explicit, logged limitation.
- Keep it reusable (no map/class/weapon-specific hacks) and keep the diff as small as correctness allows.

## Report (required)
Write `coordination/claude-preview12/<BUG>-report.md` containing:
1. **Evidence** - Visual (video timestamps and what you actually saw; mark inference vs observation) and Logic
   (IDA functions/addresses, callers, gates).
2. **Expected behaviour** and the minimal implementation.
3. **Changes** - every file touched; for `main.cpp` and other shared files list each hunk with a nearby anchor so the
   root can review it.
4. **Tests** - exact commands run and their real output (pass/fail). Include failure and duplicate/edge cases.
5. **Uncertainties / not verified** - and what the verifier and the root must check in the integrated EXE
   (what to launch, what to look at).
Return a short summary (under 200 words) with the report path, files changed, test result, and open risks.

## Budget and hygiene (you run on a small model; context cost grows fast)
- Keep your total context lean (aim well under ~100k tokens). Use Grep with `head_limit`, Read with offset/limit,
  never dump more than ~120 lines of pseudocode at once, never read whole large files (main.cpp, pseudocode-all.c,
  hud-source.json). Do not re-read files you already read.
- Write your report file EARLY (skeleton after investigation) and update it as you go, so work is not lost if you run
  out of budget. Finish with the summary even if incomplete; say exactly what is done and what is not.
- A previous attempt by another worker may have left partial edits in files you own (check `git diff` on your own
  files first). Build on or repair them; never revert or discard others' changes.
