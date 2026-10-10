# MERGEFIX2 report (Preview 16): merge p16/opening2 into p16/integrate

## Investigation
- Conflicts were only in `features/cinematic_runner/cinematic_runner.{hpp,cpp}`. HEAD carried HUDART (original dqhud_droid SKIP and caption art via `hud_panels`, placeholder caption timing). p16/opening2 carried the decoded dialog box timing and `phase_` state machine, plus placeholder SolidRect boxes and SKIP quads.
- Grep: `kCaptionBase/PerChar/MaxMsPlaceholder`, `caption_duration_ms_placeholder`, `SolidRect`, `kSkipLabelPlaceholder` and `frame.rects` have no users outside `cinematic_runner.*`. Those are removed.

## Implementation (commit 89c0272f, "Merge p16/opening2 into p16/integrate")
- `cinematic_runner.hpp`: keeps the `hud_panels` include and HUDART SkipLayout (exported art bounds). Keeps OPENING2 timing constants, `Phase`, `set_auto_tap_ms`, `caption_tap`. Removes placeholder timing, `SolidRect`, and the placeholder SKIP comment.
- `cinematic_runner.cpp`: `build_frame` draws the caption art (`caption_batches_v1`, TextBox slot with the line) when `phase_ != Phase::Idle`. This covers Showing, WaitTap, Hiding and AutoRun, which is when the original dialog box is on screen. SKIP is drawn from `skip_batches_v1` and `kSkipLabelV1`.
- `campaign_host.cpp` comment and `main.cpp` comments updated (no placeholder box/SKIP wording left).
- `cinematic_runner_tests.cpp`: `frame_content` now also checks that the caption box stays up during the hide and is gone after the line ends (SKIP only).

## Isolated tests
- Build `p14_build.ps1 -Name p16int -Jobs 6`: exit 0.
- ctest (build-p16int, run directly): 132 of 133 pass. The only failure is `session_skill_binding`, which the brief allows.
- `cinematic_runner` and `campaign_host` pass (including `captions_block_then_release`, `unresolved_caption_is_explicit`).

## Integrated runtime verification (real EXE, quiet_run, hidden and silent)
- Runs: `base-prod.args` (OPENING2 production config) with `--condition-active GameStartOnly --caption-auto-tap-ms 300`, captured at frames 340, 420, 500, 1450 (`DH_wt/mergefix2_runs/<name>/frame.png`, run logs alongside). All four exit 0, not timed out.
- Frames 340, 420, 500: original SKIP art (red X and SKIP plate) and the original caption box (name-plate band above the text band), with the authored text ("Movement Tutorial", "If you've set your controls...", "If you're using the Touch controls..."). Viewed.
- Timing from `run.log`: `Movement_Tuto2` line starts at frames 315, 399, 483, which is 84 frames each (60 Hz fixed step). That matches 367 ms show + 300 ms auto tap + 667 ms hide (about 1.40 s).
- Frame 1450 (intro): the HUD is still drawn and SKIP is not, which is the known OPENING2 defect D2. The caption box is drawn over the HUD.

## Gaps
- D2 (HUD visible and SKIP not drawn during the opening) is unchanged by this merge.
- The three-line caption at frame 500 ("If you're using the Touch controls...") sits at the bottom edge of the text band. Not changed here; HUDART text slot geometry.
- Speaker name plate has no text (no name mapping), as in HUDART.
- Not verified: the chest-tutorial zone job from HUDART (`zone-inside`) was not rerun; the Movement tutorial captions stand in for the caption path.

## Placeholders
- None added. Removed: placeholder caption timing, placeholder caption band and SKIP quads.

## Package files required
- None.

## Verifier script
- None new. Runs used `DH_wt/mergefix2_runs/jobs.json` (quiet_run, parallel 4).
