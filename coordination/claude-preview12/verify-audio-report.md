# Preview 12 audio verification (verifier `audio`): B039 and B040

Verdicts: **B040 INCONCLUSIVE** (level music start proven by logs; loop, pause/resume and audibility not observable; death, pause menu and return-to-menu not implemented). **B039 INCONCLUSIVE** (fix proven in the standalone WinMM harness only; the EXE exposes no underrun counter; one unexplained in-game timing anomaly). **Crash on shutdown: PASS.** **Latency/sync: INCONCLUSIVE.** Sample-level audibility: NOT-RUN (a human must listen).

Working folder: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview12/verify-audio/` (`cand/`, `p11/`, `harness/`). No file in either package folder was modified. All processes I started have exited. The `dh-foundation.exe` PID 186612 running from `verify-visual\cand-pkg` belongs to another worker and was not touched.

## 0. Setup and identity checks

- Candidate EXE SHA256 `4E872FE0BC27112B45AA84A7F5FE3196235942A534820C808E8DCD3F9913DABE`: matches the brief.
- Preview 11 EXE SHA256 `FD850478D8ADC2711F47CD3E0EE5A3897EA6FCC9C8C9BFC40E144227CD270A2F`.
- `swamp.args` is identical in both packages. Only `--assets`, `--audio-assets` and `--audio-table` were rewritten to absolute paths in my copies. Copies of `gameplay.save` and `character.save` sit in my folders.
- `Play-swamp.cmd` runs `dh-foundation.exe --startup-config swamp.args` from the package folder.
- Candidate `audio-assets/data/sounds` contains `m_level_swamp_sfx_swamp.vxn`, `..._water.vxn` and `..._witch.vxn`. Preview 11 contains 0 VXN files.
- Packaging note for the root: the candidate folder's `manifest.json` and `package-receipt.json` are byte-identical to Preview 11's (`"preview": 11`, Preview 11 EXE hash). They are stale and must be regenerated before the candidate is packaged.
- Strings (`grep -c`): `Level music diagnostic` matches in the candidate EXE (4 matching lines) and 0 in the Preview 11 EXE.

Commands (run through PowerShell with `Start-Process -WorkingDirectory <cand|p11> -RedirectStandardOutput ...`):
- `dh-foundation.exe --startup-config swamp.args` (600 frames), candidate and Preview 11.
- `dh-foundation.exe --startup-config combat.args` (2400 frames, `--attack-start-frame 300 --attack-frames 20 --target-frame 250`), candidate and Preview 11, run twice for the candidate and twice for Preview 11.
- `dh-foundation.exe --startup-config minimise.args` (1200 frames). The game window was minimised after 12 s and restored after 18 s with `ShowWindow` on its own PID's main window.

## 1. B040 level music (SwampHubAmbientMusic, uid 467)

Relevant log lines, quoted from the candidate run (`cand/run1-out.txt`, `cand/cand-combat-out.txt`):
```
Audio initialized listener=1 anchor=2 reference=1000 maximum=1800; one V42 WinMM output, source metadata
Level music diagnostic: Required actual output rate for level music fade (retrying)
Audio voice kind=0 token=1 outputFrame=4096
Audio final dispatched=0 startedVoices=1 diagnostics=0; original assets, no substituted samples
Rendered frames=600; clean shutdown
```
Preview 11, same args (`p11/run1-out.txt`):
```
Audio initialized listener=1 anchor=2 reference=1000 maximum=1800; one V42 WinMM output, source metadata
Audio final dispatched=0 startedVoices=0 diagnostics=0; original assets, no substituted samples
Rendered frames=600; clean shutdown
```
Results:
- **Start after load: PASS by log A/B.** Preview 11 shows zero voice receipts (`startedVoices=0`, no `Audio voice` line) in every run. The candidate shows exactly one started voice (token 1, `kind=0` = started, outputFrame 4096, which is 8 x 512 blocks, i.e. the first pump fill). The candidate-only difference is this one voice, consistent with the level music start. The mapping voice to uid 467 is inferred, not printed by the log. The `Level music diagnostic` line is a transient retry message (rate not ready on the first frame), not an error, and it is printed once per distinct text.
- Alternative start-time messages seen: `Required actual authored Play time and focused device clock (retrying)` (minimise run, start at outputFrame 5120). Both transient.
- **Loop: NOT PROVEN.** Token 1 never received a `completed` (kind 1) or `stopped` (kind 2) receipt in any candidate run (about 34 s of game time, 2400 frames). That is consistent with continuous playback (or a file longer than the run), but nothing in the log shows a loop point.
- **Pause and resume on focus loss / minimise: code path present, NOT OBSERVABLE.** `RuntimeSessionAudioV1::window_activity` posts `pause_all`/`resume_all` on focus or minimise change (`runtime_session_audio_v1.cpp:215`). No log line is printed for it, and the mixer emits no receipt for a pause. The minimise run (`cand/min-out.txt`) completed 1200 frames with `clean shutdown`, so the path does not crash. Whether the voice actually paused cannot be determined from logs. Human check needed.
- **Death, pause menu, return to menu: NOT IMPLEMENTED.** `set_level_music` is called only once at startup (`main.cpp:1761`). `play_level_music`/`stop_level_music` are called only from `after_update` and internally. There is no stop, pause or restart on death (`ReviveLocalPlayers` equivalent), in-game menu, or return to frontend. The implementer's report says the same (B040 report section 5). These are not fixed by this candidate.
- **Audibility: NOT-RUN.** No loopback capture device exists on this machine (ffmpeg dshow lists only mics and a webcam), and the EXE has no peak/RMS diagnostic. A human must listen for: swamp music fading in over about 2 s at load; a seamless loop with no click or gap at the loop point; music paused, not restarted, on alt-tab or minimise and resumed on return; silence on return to menu or death (expected to fail, see above).

## 2. B039 WinMM stutter

- **Counter accessor in the EXE: NOT PRESENT.** `winmm_pump_stats_v1()` and `winmm_pump_stats_reset_v1()` are called only by `winmm_pump_underrun_tests.cpp`. Neither `main.cpp` nor the runtime audio host prints them, and no summary line exists. Preview 11 has no counter at all. Underruns in the integrated EXE therefore cannot be counted or read, on either build. This is a finding, not a skipped test.
- **Standalone harness (not the EXE): reproduced.** I compiled the harness from the same sources into my own folder (`harness/`) and ran it (6 s per pattern, real WinMM on this machine):
  - Candidate, 8 x 512 ring (`harness/harness-out.txt`): production control-thread pattern (B) gives `underruns=0`, `refills=566`, `max_update_gap_ms=32.6`. Refills of 566 x 512 frames over 6 s is about 48.3 k frames/s, i.e. real time.
  - Preview 11 ring, 4 x 512 (`harness/harness4.exe`, built from a copy with `kWinmmBufferCount=4`; `harness/harness4-out.txt`): pattern B gives `underruns=53` (16 ms + 120 ms hitch every 2 s) and `underruns=47` (80 ms frames), with `refills=429` and `434`, i.e. about 72% of real time. The harness fails the 4-buffer config, which is the expected baseline.
  - Scenario A (per-game-frame pump, the superseded hypothesis): 4 x 512 gives 35/50/96/64/51 underruns; 8 x 512 gives 0/0/1/31/2 (16, 33, 50, 80 ms, and 16 ms + hitch). The 80 ms per-frame case still underruns on 8 x 512, which is expected because that pump is not the production path.
  - This confirms the B039 mechanism and the fix in isolation. It does not confirm the integrated EXE.
- **Observed frame time (game FPS) from the EXE logs:**
  - Candidate combat run: `Source FX frame=2190 ... gameplayMs=34050`, `frame=2188 gameplayMs=33973`, which is about 15.5 ms/frame (about 64 fps).
  - Wall-clock slope from the 600-frame and 2400-frame runs: candidate 12.82 s and 41.56 s gives 15.97 ms/frame (62.6 fps). Preview 11 12.42 s and 40.95 s gives 15.85 ms/frame (63.1 fps). No frame-time regression of the candidate is visible (about 0.7%, within noise).
- **Anomaly, unresolved (important for B039):** the candidate's receipt `Audio voice kind=0 token=2 outputFrame=369152` (combat run 1, the step/skill sound at `Source skill key ... frame=2188`, `gameplayMs=33973`). `outputFrame` is the mixer's rendered frame count (`audio_mixer_v34.cpp:109`, `frame_` advances per rendered frame, and the WinMM pump renders only as headers complete). 369152 frames at 48 kHz is 7.7 s of audio, but about 34 s of game time (and about 31 s after audio init) had elapsed. Real-time output would give about 1.5 M frames. The harness (same code, real WinMM) runs at real time, so either the EXE's pump is starved far more than the harness predicts (which would be a B039-class failure not fixed by the ring change), or the receipt frame is not what I think. This cannot be resolved from logs; it needs an instrumented run (a `winmm_pump_stats_v1()` print or a mixer frame print at shutdown) that is outside the frozen candidate. I did not repeat it because the combat event is not reproducible (see Section 4).
- **Verdict B039: INCONCLUSIVE.** Fix is supported in isolation (4 x 512: 53/47 underruns per 6 s; 8 x 512: 0). Not verified in the integrated EXE, and the EXE's own timing datum is inconsistent with real-time output.

## 3. Shutdown and sync

- **Crash on shutdown while audio pumps: PASS.** Candidate exit codes: the repeat combat run returned `exitcode=0` (`cand/rep-cand-out.txt`: `Audio final dispatched=0 startedVoices=1`, `Rendered frames=2400; clean shutdown`), with the music voice still active at shutdown. The 600-frame, 1200-frame minimise, and first combat runs also printed `clean shutdown`. No `Audio shutdown diagnostic` line appeared in any run. Exit code for the minimise run was not captured (PowerShell returned blank), but the process had exited and printed `clean shutdown`.
- **Latency and sync: INCONCLUSIVE.** Audio receipts carry only the mixer frame, and game events carry only the game frame. The logs have no shared clock, so the extra latency (about 43 to 85 ms, implementer's figures) cannot be measured from them. There was no visible sync break in the text logs (every started voice matches a `Source skill`/`Audio step` event at the right place in the sequence), but that is not a latency measurement. Human check: the hit/swing sound should lag its visual by roughly 40 ms more than Preview 11; a lag that is noticeable as "late" would be a regression.
- Combat run A/B (not like-for-like): the first candidate combat run produced a skill event (`Source skill key=2 ... skill=BashDown ... frame=2188`), but the repeat candidate run produced none, and neither Preview 11 run produced one. Gameplay is therefore not deterministic enough across runs for a sound-count A/B. Candidate: 2 started voices (music plus BashDown), 1 completed. Preview 11: 0.

## 4. Not run / limits

- Sample-level audibility (loudness, clicks, gaps) and music-vs-combat mix: NOT-RUN. No loopback device; no EXE diagnostic.
- Pause/resume effect on the voice: unobservable in logs.
- In-game underrun counts: not obtainable (no counter output in the EXE).
- Latency: not measurable.
- Loop point: not observable.

## 5. What only a human listening can confirm

1. Swamp music: starts on load, fades in over about 2 s, loops seamlessly (no click or gap at the loop point).
2. Alt-tab or minimise: music pauses (not restarts) and resumes on return.
3. Death, pause menu, return to menu: expected to fail (not implemented). Confirm the scope the root wants.
4. Combat: no periodic gaps or clicks about twice per second (the B039 symptom); compare with Preview 11 in the same scene.
5. Swing/hit sounds: no perceptible delay against the visuals beyond about 40 ms.
