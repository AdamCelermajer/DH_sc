# Preview 15.1 rc3 verification report (verifier, verify-151c)

Status: COMPLETE (audio-cadence runs skipped by user decision)

## EXE under test
- `.local-inputs/windows-source-clock-v19-preview-15-1-rc3/dh-foundation.exe`
- SHA256 95F209ABDEE594D5CC9DDBBC9DFE6F8FA50E6D99472D757C364F9842F5B0713B (confirmed match)
- Source: p15/patch e03fffea (confirmed in DH_wt/p15patch)
- Baseline for B056 rail: rc1 EXE (`.local-inputs/windows-source-clock-v19-preview-15-1-rc1/`)
- Jobs, args, logs, captures: `.local-inputs/claude-preview15/verify-151c/jobs/<name>/` (run.log, *.ppm, *.png). Generator: `verify-151c/gen.sh`. Summaries: `sum-*.json`.
- Packaging note: the rc3 package's `release-notes.json` and `package-receipt.json` list exe_sha256 `2B70D38C...` (a different build). The EXE in the package is 95F2..., so the receipt is stale.

## Results

| Item | Check | Result | Evidence |
|---|---|---|---|
| B064 | Title/menu music starts, resumes at menu, handed off at gameplay | PASS (log level; audible NOT verified) | `jobs/boot-N/run.log`: `Frontend music transition: kind=start track=TitleMusic fadeMs=2000 screen=title_splash`, then `kind=resume ... screen=menu_MainMenu`, then `kind=handoff-stop track=TitleMusic fadeMs=600` at level entry. `Frontend music state: ... playing=1` lines, none with `playing=0`. No stop/switch line between title and handoff, so the track continues through Enter Name/class select/loading by the log (those screens are not separately logged). |
| B065 | SKIP releases movie voice; title music after SKIP and after natural end | PASS | `jobs/b065-skip-N/run.log`: `Boot outcome=0 movie="skipped: user" ... handoff_voices=0 handoff_released=1 soundtrack_released=1`; `kind=start TitleMusic` at title. `jobs/b065-natural-N/run.log`: `movie="played" movie_frames=1191 soundtrack_seconds=50.29 handoff_voices=0 handoff_released=1`, then TitleMusic start, then resume at menu. |
| Boot | Production flow: logo, SKIP, title, menu, loading, swamp | PASS | `jobs/boot-N`: Boot outcome `movie="skipped: user"` with press at 6 s ignored (`presses_ignored=1`), SKIP at 9 s. `c12.png`: title splash with "Touch the screen to continue" (no atlas sprites). `load-040.png`: ornate LOADING frame, tip text ("The World Map allows you to travel..."), red bar at ~40%. `swamp-end.png`: swamp HUD and hero in game. Menu reached via `Frontend audio authored menu=menu_MainMenu button=btn_MENU_SINGLE_PLAYER`; no menu frame captured. Exit 0. |
| B056b | Equipment page: rail dim real art, no dark square plates | PASS (rail) | `jobs/b056-N-rh/rh.png` and crop `rail-N.png`: unselected slot icons are dim real art on the rail, no plates. Baseline (rc1) `rail-B.png` shows dark square plates behind several icons (the old look in `user-shots/b067-rail-shaded.png`). Reference `user-shots/b056-REFERENCE-original-equipment-video-0832.png` has the same dim silhouettes. Caveat: in `rh.png` the VALUE box and TRANSMUTE button are not visible (the bottom of the lower panel shows only REQ text), so that part is not confirmed here. Grey lip below Auto-equip is absent, as in the reference. Torso and feet captures not viewed. |
| B057 | Unmet requirement row (Imbued Armor REQ 7 ENG) | NOT-RUN | Reason: the Torso list shows only "Ceremonial Garb" (no Imbued/Mystic Armor rows) with all three bag fixtures (bagA/B/C, `jobs/b057-N-*-torso/torso.png`, `rh.png`), and with the fix057 game save (`jobs/b057q-N-torso/torso.png`). The same thing happened in the first 15.1 verification. The fix057 branch report shows these rows with the same inputs, so the harness or fixture differs. Not reproduced; no red X or dark EQUIP observed. |
| Combat | One combat run | PARTIAL | `jobs/combat-N` (with `--audio`) and `jobs/combat-N-noaudio`: `do_skill` events applied (frame 34), exit 0. Lizard stays at `HP=47.5/47.5`, no death or `Combat cue` line. The earlier verify-151 report's lizard kill (frame 148) is not in the current `b063a-N-idle` log either, so it is not reproduced here. |
| F5/F9 | Save/load (scripted hooks) | PASS (scripted) | `jobs/saveload-N/run.log`: `Saved live checkpoint frame=90 HP=165.098` and `Restored live checkpoint frame=150 HP=165.098`. Live F5/F9 keys cannot be injected on the hidden desktop. |
| Audio cadence | Real-time underrun checks | NOT-RUN (user decision) | Not run. |

## Verdict per item

| Item | Verdict |
|---|---|
| B056b equipment rail | APPROVE WITH CAVEATS (rail fixed; VALUE/Transmute and list details not confirmed in these frames) |
| B064 title/menu music | APPROVE WITH CAVEATS (log-level evidence only; not heard) |
| B065 SKIP releases movie audio | APPROVE |
| B057 unmet requirement | NOT VERIFIED (not reachable in this harness) |
| Boot flow | APPROVE |
| Combat | CAVEAT: skill runs, lizard kill not reproduced |
| F5/F9 | APPROVE (scripted only) |

Overall: APPROVE WITH CAVEATS. B057 and the combat kill are still open. The package receipt hash should be updated to the real EXE hash.

## Deviations
- B065 jobs timed out at first (the menu keeps running with no frame limit); re-run with `--menu-actions` and `--frames 420`.
- Harness scripts (`gen.sh`) changed on disk during the run; the jobs were generated from the version that ran.
