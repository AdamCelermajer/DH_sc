# STARTUP report (Preview 15, branch p15/startup)

Status: IN PROGRESS (skeleton written 2026-10-10, updated as work lands).

## 1. Goal
Original startup flow, portable by design: Gameloft logo -> intro movie (skip button) -> loading/title screen with
"touch to continue" -> main menu; Single Player -> campaign loading screen with tip + progress bar tied to real load stages
and a minimum display time. Startup mode option: default production boot; `--start-mode menu|swamp` test routes kept;
`--skip-boot` for tests.

## 2. Evidence (investigation)
- Original boot sequence (IDA GS states, durations, fades, title_intro.wav): TODO
- Reference video frames (logo / title screens): TODO
- MenuLoading / title SWF authored actions: TODO
- Loading progress reference (Android notes, v172 log): TODO

## 3. Design
- Intro movie: convert once offline (`tools/convert_intro_video.ps1`), decoder choice: TODO (pl_mpeg presence check: TODO)
- Abstract press/tap input event: TODO
- State machine: TODO

## 4. Changes
- TODO

## 5. Tests
- TODO

## 6. Verification (quiet batches)
- TODO

## 7. Package files required
- TODO

## 8. Open risks / not verified
- TODO
