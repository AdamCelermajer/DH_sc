# B039 - audio stutters / chopped output (Windows WinMM) - implementer report

Status: implemented (WinMM ring 4 x 512 -> 8 x 512), isolated harness and existing runners pass. NOT closed:
audible and integrated-EXE verification pending (verifier). Implementer evidence only.

## 1. Evidence

### Logic (code, verified by reading)
- Production path for the RuntimeSessionAudioV1 host (`--audio`, used by `package_shared_checkpoint.py`) is
  `RuntimeSessionAudioV1::after_update` -> `RuntimeAudioHostV1::update` -> `runtime->pump_receipts()` only
  (`features/audio/runtime_session_audio_v1.cpp:240-242`, `runtime_audio_host_v1.cpp:114-121`).
  It does NOT call `WinmmAudioOutput::update()`. The lead's claim that WinMM refill happens from `after_update`
  does not match the current code for this path.
- WinMM refill is owned by `WindowsSourceSessionControlV1::tick()` (`windows_source_session_control_v1.cpp:12-31`),
  which runs on the dedicated control thread of `AudioNativeSessionV42::control_thread()`
  (`integration-v42/audio_native_session_v42.cpp:148-193`). That loop calls `tick()` and then
  `changed_.wait_for(lock, 20 ms, stop-predicate)`, i.e. the output is pumped every ~20 ms, independent of game-frame time.
- The frontend menu session (`frontend_menu_audio_v1.cpp:100`) uses the same control factory, so it is also control-thread driven.
- Receipt `features/audio/windows-source-session-control-receipt.json` records the same ownership model
  ("Dedicated NativeSessionV42 control thread constructs WinmmAudioOutput over the same V42 mixer").
- Mixer: `AudioMixerV34` is documented as single-consumer, lock-free; commands/receipts go through SPSC rings
  (`audio_mixer_v34.hpp:23-30,41,53-56`). Only the control thread renders in production.
- Buffer arithmetic is as the lead says: 4 x 512 stereo frames at 48 kHz = 42.67 ms queued, so the control thread
  must refill within ~42 ms of starvation (less Windows timer slop) to avoid a gap.

### Measured baseline (harness `port/windows-foundation/features/audio/winmm_pump_underrun_tests.cpp`, unmodified ring 4 x 512)
Run: `run_winmm_pump_underrun_tests.ps1` (6 s per pattern, real AudioMixerV34 + real WinMM WAVE_MAPPER output on this machine).
Underrun = update() entered with every submitted header already WHDR_DONE.

| Scenario | Pump period | updates | refills | underruns | max update gap |
|---|---|---|---|---|---|
| A (old hypothesis: pump per game frame) 16 ms frames | sleep(16) | 201 | 438 | 40 | 32.3 ms |
| A 33 ms frames | sleep(33) | 130 | 344 | 47 | 48.4 ms |
| A 50 ms frames | sleep(50) | 97 | 382 | 93 | 64.3 ms |
| A 80 ms frames | sleep(80) | 65 | 260 | 64 | 95.2 ms |
| A 16 ms + 120 ms hitch every 2 s | | 193 | 406 | 46 | 126.7 ms |
| **B production control thread** + 16 ms game + 120 ms hitch/2 s | wait_for(20 ms) | 193 | 435 | **44** | 33.1 ms |
| **B production control thread** + 80 ms game frames | wait_for(20 ms) | 195 | 429 | **49** | 34.9 ms |

Finding: the production pump (scenario B) underruns about 7 times per second with NO game-frame involvement.
Mechanism (measured): the nominal 20 ms wait is ~31-35 ms on Windows default timer resolution (15.6 ms), and
after each refill only ~34 ms of audio remains queued (4 x 10.7 ms minus the partially-played buffers), so any
pump period above ~34 ms drains the device.

## 2. Expected behaviour and the fix (implemented)

Expected: the device queue never runs dry while the game runs (no gaps/chops at the 20 ms pump cadence or under
game-frame hitches), the mixer stays single-consumer (control thread renders; commands/receipts stay on the existing
SPSC rings), and the AudioClockV40 contract is unchanged (the clock still reports the WinMM played position, so
event-to-frame mapping stays correct; only the written-ahead distance grows).

Minimal change: enlarge the WinMM ring from 4 x 512 frames (42.7 ms) to 8 x 512 frames (85.3 ms) via
`kWinmmBufferCount=8` / `kWinmmFramesPerBuffer=512` in `winmm_output.hpp`; the buffers, prepare/unprepare, close and
refill loops now use those constants (same byte counts: dwBufferLength = 1024 samples x 2 bytes = 2048 bytes).
No threading/ownership change was needed: the dedicated control thread already exists (see Evidence).
Diagnostic counters (`winmm_pump_stats_v1`) were added so underruns are observable at runtime and in the harness.

Not done (deliberately): no `timeBeginPeriod(1)` (process-wide timer change would alter the game's own Sleep(1)
frame pacing; not verified safe); no change to `audio_native_session_v42.cpp` (control-thread wait is not my file).

## 3. Changes

- `port/windows-foundation/features/audio/winmm_output.hpp`: ring constants `kWinmmBufferCount=8`,
  `kWinmmFramesPerBuffer=512` (above `class WinmmAudioOutput`); `WinmmPumpStatsV1`, `winmm_pump_stats_v1()`,
  `winmm_pump_stats_reset_v1()` (after `winmm_monotonic_ns`).
- `port/windows-foundation/features/audio/winmm_output.cpp`: constants used for all header/PCM loops; underrun and
  refill counters and max-gap tracking in `update()` (Windows-only, atomics, process-wide); stats accessors (non-Windows stubs return zeros).
- `port/windows-foundation/features/audio/winmm_pump_underrun_tests.cpp` (new): the harness.
- `port/windows-foundation/features/audio/run_winmm_pump_underrun_tests.ps1` (new): runner, build dir
  `.local-inputs/b039-audio-pump-build/`. `-Stress` adds the starvation sweep.
- Not touched: `main.cpp`, `runtime_audio_host_v1.*`, Linux SDL2 files, `CMakeLists.txt`, engine-audio.
  NOTE: `runtime_audio_host_v1.cpp/.hpp` show uncommitted edits (`play_level_music`) that are NOT mine; they belong to
  the B040 worker and I left them alone.

## 4. Tests

Commands (run from the repo root, toolchain bin on PATH):
- `port/windows-foundation/features/audio/run_winmm_pump_underrun_tests.ps1 -Stress` (final config) ->
  exit 0, `PASS winmm pump underrun harness failures=0`. Output copied to
  `.local-inputs/claude-preview12/B039/harness-after-8x512-ring-stress.txt`.
- Baseline before the fix (4 x 512, same harness code path, counters present) ->
  `.local-inputs/claude-preview12/B039/harness-before-4x512-ring.txt`. Production scenario B with 4 buffers: 44 and 49
  underruns per 6 s, the harness failed (`FAILED ... failures=2`).

Results (6 s per pattern; underrun = update entered with all submitted headers WHDR_DONE):

| Scenario | 4 x 512 (before) | 8 x 512 (after) |
|---|---|---|
| A old hypothesis: pump per game frame, 16 ms | 40 | 0 |
| A, 33 ms | 47 | 0 |
| A, 50 ms | 93 | 0 |
| A, 80 ms | 64 | 33 |
| A, 16 ms + 120 ms hitch every 2 s | 46 | 2 |
| **B production control thread, 16 ms + 120 ms hitch/2 s** | **44** | **0** |
| **B production control thread, 80 ms game frames** | **49** | **0** |
| B stress: pump starved 20 ms every 2 s | not run | 2 |
| B stress: 30 / 40 / 60 ms | not run | 1 / 2 / 2 |

Scenario A rows are the lead's per-frame hypothesis and are NOT the production path. Scenario B is production.
The 6 x 512 alternative (64 ms) also gave 0 underruns on the B nominal patterns, but less headroom (see
`harness-alt-6x512-ring-stress.txt`), so 8 was kept.

Existing runners (all exit 0, run against the final 8 x 512 build):
- `run_windows_source_session_control_tests.ps1`: PASS (real WinMM open/focus/QPC, focus pause/resume, checked close/join/drain)
- `run_runtime_audio_host_v1_tests.ps1`: PASS (includes checked shutdown, focus pause/resume)
- `run_canonical_source_audio_adapter_tests.ps1`: PASS
- `run_frontend_menu_audio_v1_tests.ps1`: PASS
- `run_runtime_combat_audio_v1_tests.ps1`: PASS

Lifecycle (in the harness, all PASS): open/close/close-again x2; `shutdown while pumping` x3 (control thread running,
then `WindowsSourceSessionControlV1::shutdown` plus `close_succeeded`); focus loss then restore while pumping.

Duplicate-cue check: no live cue/voice log exists in the workspace (the only `Audio voice kind=` hit is the source line).
The code has two independent producers (combat hit/death observer in `runtime_combat_audio_v1.cpp`, and attack step-entry
observer in `runtime_attack_sound_v1.cpp`), and both log "Duplicate exact retained occurrence suppressed". Not verified
against a live run; cue restarts at state transitions not checked.

## 5. Uncertainties and what the verifier / root must check

- Not verified: audible output. The harness proves the pump refill gaps, not the sound. Listen for before/after:
  before, periodic short gaps or clicks (~2 per second) during play, most at the start of combat; after, none.
  Expect the hit/swing sounds to lag the visuals by about 40 ms more than before (latency doubled from ~43 to ~85 ms).
  Check music/ambience (B040) is not affected by the extra latency.
- Residual: with an artificial 20 ms+ starvation of the control thread every 2 s, 1-2 underruns per 6 s still occur
  even with 85 ms queued. My simple model (62 ms gap < ~75-85 ms queued) predicts none, so the cause is unexplained.
  The likely suspect is WinMM's WHDR_DONE reporting lag, but I did not verify it. Not a nominal-path failure.
- Root cause for the lead: the investigation doc says the refill comes from `after_update`. That is stale: production
  refill is on the NativeSessionV42 control thread (20 ms wait). The real mechanism is that the 20 ms wait is ~31-35 ms
  on the default 15.6 ms Windows timer, against ~34 ms of post-refill queue in the 4 x 512 ring.
- Root must integrate: no main.cpp change. Build the integrated EXE once, with the existing runners. Harness and
  runners are standalone; the new test is not added to CMake (same as the other audio runners).
- Not done: B039 is NOT closed. Verifier should launch an isolated `--audio` run with a fresh save and listen.
