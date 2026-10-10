# Verify final report (Preview 12, final verifier)

## Verdict: APPROVE WITH CAVEATS

All five checks pass in the frozen candidate EXE. The caveats are: the audio path is confirmed by logs only (no one listened), and the candidate package metadata is stale and must be regenerated before packaging.

## Identity and setup

- Candidate EXE: `.local-inputs/windows-source-clock-v19-preview-12-candidate/dh-foundation.exe`
- SHA256 `1D43942ECD52DF7B86BDF77BA2E5CB306AFC76944813F38CA7CE9E6CCB252672`. This matches the hash in the task. The brief's `4E872FE0...` hash is outdated, and the older verify-* reports were run against that older build, so this verdict does not rely on them.
- The EXE and saves were copied to `.local-inputs/claude-preview12/verify-final/run2/` (`pkg/`, `saves/`). Copies match the candidate by SHA256. The candidate folder was not written to. `--assets` and `--audio-assets` point at the candidate read-only.
- Runner: `run2/scripts/run.sh <name> <frames|-> <yes|no> [args]`. It uses `swamp.args` with the save and output paths rewritten. Each run appends `--fixed-step .016 --frames N --capture`, except the real-time run.
- Combat scenario: `POS=-6752.641,938.285,250`, `--target-frame 5`, Bogwomp (id 7118915781085668844) selected. Runs are deterministic.
- Preview 11 was not touched. Another worker's `claude-preview13` `dh-foundation.exe` (PID 13836) was running and was not touched. My one game process (PID 268796) exited on its own. No process of mine is left running.

## Results

| # | Check | Result | Evidence |
|---|---|---|---|
| 1 | Smoke: Swamp 120+ frames, exit 0, HUD | PASS | `run2/runs/smoke`: `Rendered frames=130; clean shutdown`, `exit=0`, no error lines. HUD crop (`img/smoke-hud3x.png`): portrait inside the round frame, red HP bar, blue MP bar, and the XP strip above HP (empty at start). |
| 2 | B038: XP bar grows after 1 and 2 kills | PASS | Kill log: `Source death reward frame=146 ... xp=8` and `frame=249 ... xp=16`. XP fill in row 50 (`scripts/xpwidth.py`): base (40 f) 0 px; after kill 1 (160 f) x=119..122 (4 px); after kill 2 (260 f) x=119..132 (14 px). Visually confirmed in `img/xp-stack.png`. |
| 3 | B041: Space held, 3 combo swings, step FX, BashDown key 2 | PASS | `run2/runs/b41-200` (`--space-key-interval 60:130`): `Source player step FX frame=73 ... sequence=470 ... dispatched=1`, `frame=104 ... sequence=471 ... dispatched=1`, `frame=136 ... sequence=472 ... dispatched=1`. 0 `resource not found` lines. BashDown: `Source skill key=2 slot=0 frame=50 ... skill=BashDown MP=21.25 diagnostic=` (empty), step FX `sequence=347 dispatched=1`. Blue trail (`img/b41-trail-sheet.png`, blue px full-res): swing 1 at f80 = 9004, f86 = 307; swing 2 at f110 = 7397, f116 = 3777; swing 3 at f142 = 7409, f148 = 1984. Arcs are visible in each swing. |
| 4 | B037 + B004/B029: skill mid-swing cuts swing; marker persists | PASS | `run2/runs/b37-held` (Space 30:400, skill 50:2): key 2 accepted at frame 50 with `phase=1`, empty diagnostic. Swing is cut: no combo boundary between frames 50 and 131 (the next boundary is at 132, generation 2, with Space still held). `Damage ... marker=do_skill` at 84. `Source skill lifecycle frame=131 phase=3 ... Use/Post completed and authored ClearTarget was applied`. `run2/runs/b37-rel` (Space 30:60, released at 90): at frame 220, after Post (131) and release (90), the red target ring and HP bar are still on Bogwomp (`img/b37-rel-full.png`). Matches the no-skill baseline `img/b4-tgt-only-full.png`. |
| 5 | B039/B040: 40 s real-time, audio, focus held, level music | PASS (logs); audibility NOT-RUN | `run2/runs/rt40` (no `--fixed-step`, `--frames 2400`, audio on): `Audio final dispatched=0 startedVoices=1`, `WinMM pump: underruns=0 refills=3525 renderedSeconds=37.6 wallSeconds=37.5382 maxGapMs=32.8 gapsOver40ms=0`, `Rendered frames=2400; clean shutdown`, exit 0. No `Level music diagnostic` line. Focus: `GetForegroundWindow` returned my PID 268796 at launch and again mid-run (~20 s in). The check set focus only on my own PID's window. |

## Caveats

1. **Level music identity (B040).** The log shows one started voice (`startedVoices=1`) and no `Level music diagnostic` error. The voice is not named in the log. Attributing it to the level music is inference from the code path (`set_level_music` → `play_level_music`, `features/audio/runtime_session_audio_v1.cpp`). Loop, pause and resume were not tested.
2. **Audibility not verified.** Nobody listened. Sample-level audibility and latency sync are NOT-RUN.
3. **XP bar scale (B038).** The fill is small (4 px after one kill, 14 px after two). Growth is monotonic, and the logs show XP 8 then 16. Whether the fill is proportional to XP and where level-up falls was not checked. The `Character name=... xp=0` line is printed once at startup from the loaded save (`main.cpp:1605`), so it is not evidence of the kill XP.
4. **Exact skill-Post frame.** The logs show Post completing at frame 131 (`Retained state6 ... Use/Post completed`). The marker check was done at frame 220 after that point.
5. **Packaging metadata is stale.** `manifest.json` and `package-receipt.json` in the candidate folder both say `"preview": 11`, and `README.txt` names `FD850478...` (the Preview-11 hash). These must be regenerated for Preview 12 with hash `1D43942E...` before packaging. This is not a gameplay defect.
6. **Preview-11 A/B not re-run.** Only the candidate was tested in this pass.

## Screenshots looked at (`.local-inputs/claude-preview12/verify-final/run2/img/`)

`smoke-full.png`, `smoke-hud3x.png`, `xp-stack.png`, `b41-f80-full.png`, `b41-trail-sheet.png`, `b37-rel-full.png`, `b4-tgt-only-full.png`.
