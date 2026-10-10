# Preview 12 verification brief (read after COMMON-BRIEF.md)

You are a **verifier**, not an implementer. You do not fix code. You independently decide whether the claimed fix works
in the **integrated, frozen candidate EXE**, with evidence. If it does not, report exactly what you observed.

Candidate (frozen, do not modify): `.local-inputs/windows-source-clock-v19-preview-12-candidate/`
(EXE SHA256 `4E872FE0BC27112B45AA84A7F5FE3196235942A534820C808E8DCD3F9913DABE`; verify the hash first).
Preview 11 (`.local-inputs/windows-source-clock-v19-preview-11`) is the baseline for A/B comparison; do not modify it.
The user may be playing Preview 11 right now (a `dh-foundation.exe` process): never kill, focus or automate that process.
Only start your own processes and always stop them yourself afterwards (by PID that you started).

## How to run the game headlessly-ish
- The EXE accepts scripted input and capture options (see `main.cpp` option parser, `grep -n "spaceKeyIntervals\|skillKeyFrames\|attackStartFrame\|targetFrame\|--capture\|--frames" port/windows-foundation/main.cpp`, and the
  existing patterns in `port/windows-foundation/tools/verify_combo_checkpoint.py`, `verify_enemy_checkpoint.py`,
  `verify_preview.py`, and `.local-inputs/windows-source-clock-v19-preview-11/*.args`). Typical: `--frames N`,
  `--capture file.ppm`, scheduled Space intervals (`--space-key START:COUNT` style), scheduled skill key frames,
  target-select frame, fixed step. Read the real option names from the parser before using them.
- Build your own working folder `.local-inputs/claude-preview12/verify-<yourname>/`. Copy `swamp.args` and the `.save`
  files there, set every save/output path to files inside that folder, and run the candidate EXE with
  `--assets <absolute path to candidate>/assets` (and any other relative path made absolute). Never write inside the
  candidate folder or reuse its `*.save` files in place.
- Convert PPM captures to PNG if needed (`.local-inputs/windows-foundation-build/ppm_png.py` or the Codex Python from
  the common brief) and LOOK at them with the Read tool. Describe what you actually see.
- Reference video frames: see COMMON-BRIEF (ffmpeg). Only claim a comparison if you looked at both images.
- Stdout/stderr of the EXE contains diagnostic lines (e.g. `Source skill key=`, `Source target command`, `Damage frame=`);
  save logs in your folder and quote the relevant lines.

## Output
Write `coordination/claude-preview12/verify-<name>-report.md` with: what you ran (exact command lines), results per
check with PASS / FAIL / NOT-RUN and the evidence (log lines, image paths and what they show), the A/B result against
Preview 11 where relevant, and a clear final verdict: APPROVE / REJECT / INCONCLUSIVE with reasons.
Return a summary under 150 words. Stay lean (small model, cost grows with context): grep, do not dump big files.
