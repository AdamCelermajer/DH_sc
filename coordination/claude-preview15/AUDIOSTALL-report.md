# AUDIOSTALL report (Preview 15 blocker B039: rc4 audio cadence)

Status: DONE (fix verified in real EXE under CPU saturation; see Open risks). Branch p15/audiostall, worktree DH_wt/audiostall, build DH_wt/build-audiostall.
Scratch: `.local-inputs/claude-preview15/audiostall/` (probe EXEs, run dirs, stress tool).

## Question
rc4 assets (rc2 + 256 iPad audio/3d files, level_up.bdae, minimap cameras, intro_v1.mpg) with the same EXE stall the
WinMM pump 1.8-3.2 s and underrun 6-115 times in the 35 s real-time swamp run (`-Parallel 1`). Which call stalls, and when?

## Evidence from reading code and logs (before measuring)
- B039 logs: `.local-inputs/claude-preview15/verify-rc4/B039/rt35*/run.log`. rc4 world-item drop cue resolves to
  `uid=151 status=submitted` (rc2: `uid=148 status=asset_missing`), so a real WAV is now loaded on the frame thread.
- Frame log `worstFrameMs` is clamped to 100 ms in main.cpp, so a multi-second main-thread stall is invisible there.
- Producer load path: `RuntimeAudioHostV1::submit_world_item_sound` / `submit_source_sound` ->
  `AudioGameplayRuntimeV42::load_sample_actual_xml_uid` -> `AudioSampleBankV34::load` (synchronous, producer thread;
  cache by filename, LRU 64 MB) -> `AudioFilesystem::read` (`std::istreambuf_iterator` byte loop into a vector, no reserve)
  -> `audio_sample_open_v34` (header parse only; VXN 12 MB files are not decoded at open).
- Pump: `WinmmAudioOutput::update` is driven by the control thread (`audio_native_session_v42.cpp` loop, tick every 20 ms,
  wait with lock released). No mutex is shared with the producer except the session `mutex_`, held only briefly.
  The ring is 8 x 512 frames = 85 ms at 48 kHz, so any control-thread gap > ~85 ms underruns.
- quiet_run.ps1 starts every job at BELOW_NORMAL priority class. No thread in the game sets a thread priority
  (grep `SetThreadPriority|AvSetMm|THREAD_PRIORITY` in port/: none). So the WinMM pump thread runs at base priority 6,
  the same as every other quiet-run job thread and lower than any normal-priority process on the machine.
  The Android (AAudio) and Linux (SDL2) back ends use platform callback threads that the platform already runs at
  elevated priority; only the WinMM back end polls from an ordinary std::thread.

## Measurements
All runs: rt35 swamp args from `verify-rc4/B039/rt35-rerun2/run.args` (rc4) or `rt35-rc2/run.args` (rc2), 2100 frames,
quiet_run hidden/silent (`DH_AUDIO_SILENT=1`), `-Parallel 1` for the measured batch. Scratch root
`.local-inputs/claude-preview15/audiostall/`. Probe EXEs (`probe2/3.exe`, `base4.exe`) = rc4 source + temporary
steady-clock probes (load/read/submit/render/tick/control-loop/wait; removed again in commit 028f08f4).

1. Load path is NOT the stall (probe3, p3-rc4-1). Every `AudioFilesystem::read` in a whole rc4 run (15 files):
   12,278,272-byte `m_level_swamp_sfx_swamp.vxn` 28 ms (once, level start, producer thread); all WAVs <= 0.5 ms
   (rc4 drop cue `sfx_drop_gold.wav` 126,536 bytes 0.5 ms). `AudioSampleBankV34::load` max 39 ms (uid=467, the VXN)
   across 10 runs. rc4 vs rc2 differ only by 4-5 small WAVs (rc2 lacks lizardman attack/die, drop gold, hurt_1):
   < 2 ms total. Loads run on the producer thread and share no lock with the pump.
2. Without added load the rc4 assets do not stall: rc4 0 underruns / maxGap 32-39 ms in 10/10 runs (probe2 x5,
   probe3 x2, package EXE 90F724FC x3: `pkg-rc4-*`); rc2 assets underran in 3/9 runs (1, 4, 1; maxGap 108-203 ms)
   in the same period. The rc4 asset set is not the cause; the original B039 rc2 control run also underran
   (`rt35-rc2`: 4 underruns, 518 ms) and `rt35-rerun` shows wall 89.5 s vs rendered 64.6 s: a heavily loaded machine.
3. Where the time goes when it does stall (probes, rc2-3 / rc2-1 / s2): `tick` 87-165 ms and a single 512-frame
   `render` of ONE voice 17.6 ms (normally << 1 ms), i.e. the control thread was runnable but not scheduled.
   Under CPU saturation (s2, 56 spinning BELOW_NORMAL threads, same class as quiet_run jobs) the baseline
   control loop's `wait_for(20 ms)` took median 77 ms (max 143 ms); 2523 loop gaps > 60 ms (p50 78, p90 95,
   max 262 ms) in base-rc4-1; base-rc4-2: underruns=735, gapsOver40ms=1700, maxGap 195 ms. Same load with the
   fix: 1 gap > 60 ms in the whole run (109 ms, the first tick that opens the WinMM device).
4. Root cause: the WinMM ring (8 x 512 frames = 85 ms) is refilled only by AudioNativeSessionV42's control thread,
   which runs at default priority inside a BELOW_NORMAL process (quiet_run) and so loses the CPU to any
   competing job; a > 85 ms scheduling gap empties the ring. Asset content only shifts the run's CPU profile
   slightly. The 1.8-3.2 s gaps in the B039 logs are the same mechanism under heavier load than reproduced here
   (not reproduced at that magnitude; see Open risks).

### Before/after (real EXE, rc4 assets, real-time 35 s swamp run, -Parallel 1)
| condition | EXE | runs | underruns | maxGapMs | gapsOver40ms |
|---|---|---|---|---|---|
| CPU saturated (28 BELOW_NORMAL spinners, s3) | package rc4 90F724FC | 3 | 1, 6, 12 | 81.6, 158.5, 99.6 | 898, 1203, 1534 |
| CPU saturated (28 spinners, s3) | final 5B609B2F (fix) | 3 | 0, 0, 0 | 32.2, 34.4, 34.2 | 0, 0, 0 |
| moderate load (14 game jobs, ~50% CPU, s1) | base4 (probes, no fix) | 2 rc4 + 2 rc2 | 0 | 32-34 | 0 |
| moderate load (s1) | fix4 | 2 rc4 + 2 rc2 | 0 | 32-33 | 0 |
| no added load (s4) | final 5B609B2F | 3 | 0, 0, 0 | 32.4, 32.4, 32.3 | 0 |
| no added load (prev. worker) | package rc4 | 3 | 0, 0, 0 | 32.3, 32.2, 33.2 | 0 |
All fixed runs log `pumpThreadPriority=15`. Audio content unchanged: `Audio final ... diagnostics=0; original assets`
with matching voice counts (s3 pkg 141/152/157 vs final 144/147/151; s4 final 74/74/77 vs pkg 72/75/75), rc4 drop
cue `World item sound uid=151 ... status=submitted` in every final run.

## Changes
- `features/audio/winmm_output.{hpp,cpp}`: `winmm_promote_pump_thread_v1()` sets the calling thread to
  `THREAD_PRIORITY_TIME_CRITICAL` (Windows-only file; non-Windows stub returns false) and records the actual priority in
  `WinmmPumpStatsV1::pump_thread_priority`.
- `features/audio/windows_source_session_control_v1.cpp`: the production WinMM control factory (constructed on
  AudioNativeSessionV42's dedicated control thread) calls it. Non-fatal on failure. No shared engine-audio code changed.
- `features/audio/runtime_session_audio_v1.cpp`: `WinMM pump:` summary line appends `pumpThreadPriority=N`.
- `CMakeLists.txt` (anchor `P15 AUDIOSTALL (B039)`): new Windows test `winmm_pump_priority_v1`.
- Not changed (measured, not justified): AudioFilesystem byte-iterator read (28 ms for 12 MB, once), sample bank,
  mixer, ring size, cue routing. No cue is dropped or silenced.
- Original-behaviour note (AGENTS.md): internal infrastructure with no visual counterpart; the invariant is continuous,
  gap-free playback of the same original cues. Android AAudio / Linux SDL2 already run their audio callbacks on
  platform-elevated threads; this gives the Windows polling pump the equivalent scheduling class.

## Tests
- `winmm_pump_priority_v1` (new): helper promotes only the calling thread to 15 and records it; production factory
  promotes its constructing thread and not the caller. PASS.
- `p14_build.ps1 -Name audiostall -Test`: 117/118 pass; only `session_skill_binding` fails (worktree junction, known).

## Verifier script
Package files required: none new (EXE only).
1. Clean: `quiet_run.ps1 -Parallel 1` with 3 jobs, EXE under test, args = `verify-rc4/B039/rt35-rerun2/run.args`
   (rc4 assets), timeout 240 s. Expect per log `WinMM pump: underruns=0 ... maxGapMs<40 gapsOver40ms=0 pumpThreadPriority=15`.
2. Stress (reproduces B039 on the old EXE): start `.local-inputs/claude-preview15/audiostall/tools/cpu_burn.exe 28 3000 <stop.txt>`
   (28 BELOW_NORMAL spinning threads; source `tools/cpu_burn.cpp`), then the same measured batch with timeout 480 s
   alternating old/new EXE (`audiostall/s3/jobs-meas.json`, runner `s3/meas.ps1` writes the stop file at the end).
   Expect old EXE: underruns > 0 and gapsOver40ms in the hundreds; new EXE: underruns=0, gapsOver40ms=0.

## Open risks
- The multi-second (1.8-3.2 s) gaps of the original B039 runs were not reproduced at that magnitude; the conditions
  (what else ran then) are unknown. The mechanism (unscheduled pump thread) is measured; a 3 s gap could also come
  from a whole-process suspension or hard paging (memory use was 77-80%), which thread priority cannot fix.
- Time-critical priority applies to a thread that sleeps 20 ms per tick and renders well under 1 ms; if a future
  mixer change makes render expensive, it would take CPU from the game at high priority.
- MMCSS ("Pro Audio") was not used (needs avrt); THREAD_PRIORITY_TIME_CRITICAL (15) is sufficient against
  normal/below-normal competition but not against REALTIME-class processes.
- Real audible output was not listened to (all runs silent per QUIET-RULES); counters and logs only.
