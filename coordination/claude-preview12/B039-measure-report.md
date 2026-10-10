# B039 in-game measurement: is WinMM audio smooth in the real game?

Short answer: **yes, in the real game with the 8 x 512 ring, the pump never underran** (0 underruns over
about 97 s of focused real-time play across runs 2-4, including 61.7 s in run 3). The same real game with the
old 4 x 512 ring underruns 478 times in 61.7 s and renders only 76% of real time (run 5). The verifier's
"7.7 s rendered after 34 s of game time" anomaly is explained by the game's focus gate: audio is paused by
design while the window is not focused, and the mixer frame counter only advances while audio renders.
This is a misreading of the anomaly, not an underrun. Not verified: audibility (a human must listen).

## 1. Build and tests

- Toolchain bin on PATH, `cmake --build .local-inputs/windows-foundation-build --parallel 14`: exit 0, no warnings.
  Log: `.local-inputs/claude-preview12/audio-measure/build.log` (then `build2.log`, `build-final.log`).
- `ctest --test-dir .local-inputs/windows-foundation-build -j 6`: **101/101 passed** (`ctest-final.log`, after the final rebuild).
- Standalone harness `port/windows-foundation/features/audio/run_winmm_pump_underrun_tests.ps1` (8 x 512, final code):
  `PASS winmm pump underrun harness failures=0` (`harness-final.txt`). Production scenario B: underruns 0.

## 2. Diagnostic added (no behaviour change)

The shutdown summary now prints, for example:

`WinMM pump: underruns=0 refills=5786 renderedSeconds=61.7173 wallSeconds=61.6574 maxGapMs=33.1536 gapsOver40ms=0`

- `renderedSeconds` = mixer `output_frame()` / 48000 (frames the WinMM pump actually rendered).
- `wallSeconds` = steady-clock time from the first to the latest pump update (the pump's own window; it stops when
  the pump stops, which is what makes focus pauses visible).
- `maxGapMs` = largest gap between pump updates; `gapsOver40ms` = count of gaps above 40 ms.
- Also printed under `--frames` (per second): `Frame rate second=N frames=F worstFrameMs=X`.
- Also printed on each focus/minimise change: `Audio window activity focused=F minimized=M`.

## 3. Real-game runs

Scenario (`.local-inputs/claude-preview12/audio-measure/rt.args`): the candidate's `combat.args` with `--audio`,
**no `--fixed-step`** (real time), `--frames 3900` (about 62 s at ~15.9 ms/frame), attack at frame 300 plus
24 Space-hold intervals (`--space-key-interval`, every 150 frames, 20 frames each) for a fight with the lizard.
Same saves copied into the folder; EXE copied into the folder. `run-focused.ps1` starts the game and tries to bring
its own window to the foreground; it also logs (without stealing focus) which process owns the foreground.

| Run | Build | Audio | Focus during run | underruns | refills | renderedS | wallS | maxGapMs | gaps>40ms |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 8 x 512 | on | launched from bash; focus lost ~1 s, regained; lost ~6 s, regained | 0 | 5635 | 60.11 | 62.93 | 2040.8 | 2 (the two focus gaps) |
| 2 | 8 x 512 | on | acquired; focus lost at ~9 s, never regained | 0 | 875 | 9.33 | 9.27 | 32.6 | 0 |
| 3 | 8 x 512 | on | launcher could not foreground; game focus flag stayed set all run | **0** | 5786 | **61.72** | **61.66** | 33.2 | 0 |
| 4 | 8 x 512 | on | acquired; T3 Code took foreground at ~26 s | 0 | 2433 | 25.95 | 25.89 | 32.9 | 0 |
| 5 | **4 x 512** (temporary, same counters) | on | acquired; no focus change | **478** | 4392 | **46.85** | **61.72** | 33.2 | 0 |
| 6 | 8 x 512 | **off** | acquired; T3 Code took foreground at ~17 s | n/a | n/a | n/a | n/a | n/a | n/a |

Logs: `run1-out.txt` ... `run6-noaudio-out.txt`, `run5-4ring-out.txt`; foreground owner log `foreground-watch.txt`
(last run); launcher timings `run*-time.txt`.

Reading the table:
- **Before/after in the real game:** same scenario, same control-thread path, same focus (run 5 vs run 3):
  4 x 512 gives 478 underruns and 15 s of audio missing from 61.7 s; 8 x 512 gives 0 underruns and 61.7 s rendered.
  The fix is confirmed in-game, not only in the harness (the harness predicted ~9 per second for 4 x 512; in-game it is ~7.7 per second).
- **Focus gap explained:** in runs 1, 2 and 4 the pump's gap or wall time stops at the focus loss, and
  `renderedSeconds` tracks `wallSeconds` (run 2, 9.33 vs 9.27; run 4, 25.95 vs 25.89). Run 1's 2040 ms max gap and its
  two `gaps>40ms` are exactly its two focus losses, not starvation. Focus-off is by design (the gate requires
  focus + window + resumed, and `window_activity` posts `pause_all` and `waveOutPause`), so the pump and the mixer
  freeze until focus returns. Game frames keep running at ~65 fps while unfocused.
- **Anomaly:** the verifier's candidate run printed 369152 mixer frames (7.7 s) at ~34 s of game time. That is the
  same signature as run 2 (focus lost after ~9 s, rendered time stops there). The verifier's EXE had no focus log,
  so this is a strong inference, not proven from their logs. It was not an underrun: the 8 x 512 build shows 0
  underruns in every focused run.
- **Frame time (per second):** run 3 (audio on, focus held, 61 s): mean 63.90 fps, 61 per-second samples, one second
  below 60 fps (second 30: 54 frames, worst clamped frame 100 ms). Run 6 (audio off, 64.9 s wall, focus lost at 17 s
  but frames still ran): mean 63.92 fps, one second below 60 fps (worst 100 ms). Audio does not measurably change
  game frame time. Run 1 had 8 seconds of 33-58 fps during the fight, which runs 3 and 6 did not reproduce; cause
  not determined (run 1 was the only run launched from bash without foreground acquisition, and its focus changes
  came early).

## 4. Fix decision

No further fix needed. The residual theory from the previous report (control-thread wait_for(20 ms) at ~33 ms on the
15.6 ms timer, against ~85 ms queued) gives a maximum observed gap of 33 ms in game and zero underruns. I did NOT add
`timeBeginPeriod(1)`, thread priority, or a ring change, because nothing failed in game.

## 5. Files changed (my hunks; others' uncommitted hunks in the same files were left alone)

- `port/windows-foundation/features/audio/winmm_output.hpp`: `WinmmPumpStatsV1` gained `first_ns`, `last_ns`,
  `gaps_over_40ms` (+ comment line above it; anchor "B039 pump diagnostics").
- `port/windows-foundation/features/audio/winmm_output.cpp`: anonymous-namespace atomics `g_pump_first_ns`,
  `g_pump_gaps_over_40ms`; `update()` first-update store and >40 ms gap counter (anchor
  "if(last&&now>last&&now-last>g_pump_max_gap_ns"); `winmm_pump_stats_v1()` and `winmm_pump_stats_reset_v1()` now
  fill/reset the new fields (anchor "return {g_pump_updates.load()").
- `port/windows-foundation/features/audio/runtime_audio_host_v1.hpp`: declaration `rendered_frames()` (after
  `set_output_paused`).
- `port/windows-foundation/features/audio/runtime_audio_host_v1.cpp`: definition of `rendered_frames()` (before
  `RuntimeAudioHostV1::ready`).
- `port/windows-foundation/features/audio/runtime_session_audio_v1.cpp`: `#include "winmm_output.hpp"`; focus/minimise
  log line in `window_activity()` (after the publish check); WinMM pump summary line in `summary()` under `#ifdef _WIN32`
  (Linux has no `winmm_output.cpp` in its build, so the guard is required).
- `port/windows-foundation/main.cpp`: one hunk, the per-second `Frame rate second=` line placed just before
  `if(runtimeAudio) {std::string audioError;if(!runtimeAudio->window_activity(`, gated on `options.frames>0`.
- Temporary edit reverted: `winmm_output.hpp` `kWinmmBufferCount` set to 4 for the run-5 build, restored to 8; final
  EXE is the 8-buffer build (same size as the measured run-3 build; differs only in PE timestamp bytes).

No gameplay code, no `CMakeLists.txt`, no docs/trackers touched. Not committed.

## 6. Not verified / for the root and verifier

- Audibility: still not listened to. Numbers say no gaps; a human should confirm the sound.
- `window.focused()` is `GetFocus()==hwnd` (`platform_win32.cpp:221`), which is not the same as the foreground window
  (run 3: foreground owned by another process for the whole run, yet the game kept its focus flag). When checking
  pauses, read the `Audio window activity` lines, not the foreground.
- Harness counters are process-wide; the summary line only prints when the pump has run.
- The EXE includes the working tree as it stands, including other workers' uncommitted changes in the same files
  (e.g. B040 `play_level_music` in `runtime_audio_host_v1.*`).
- To re-measure with a clean signal: start the game and keep its window focused (do not switch apps), then read the
  `WinMM pump:` line at exit.
