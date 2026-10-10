# B054 SKIP drawn on the Gameloft logo

## Evidence
- Video (Part 1, recorded with ~3 s lead-in): 0-2 s black, spark ~2 s, logo glow 3-9 s, logo cut to black at ~9.0 s, story parchment fades in 9.25 s (frames at 4 fps, sheet in .local-inputs/claude-preview15/fix054). Equivalent in intro_v1.mpg (luma trace): static logo to ~5.45 s, black to ~6.2 s, story fade-in from 6.2 s. No SKIP is visible anywhere in the video (recording shows none, or the tap-to-reveal button was never touched): direct observation only that the logo has none.
- IDA (classes.dex, MyVideoView; the movie is played by the Java activity, GSInit case 5 only calls nativeLoadMovie("intro.mp4") and waits for videoDone, case 6): SKIP is an ImageButton (field l) hidden by c() at start. MyVideoView.onTouchEvent (0x487fc) ignores every touch while VideoView.getCurrentPosition() <= MyVideoView.o = 0x1C84 = 7300 ms; after that a tap toggles the button visible (field q starts true); the button's onClick (bi.onClick 0x4dc8c) ends the movie and returns to the game. So the logo part is uninterruptible and SKIP exists only from 7.3 s on, after a tap (the user asked for SKIP visible in the story; the tap-to-reveal step is not reproduced: UNKNOWN whether users must tap first, we show it from 7.3 s).
## Change
- features/startup/intro_segments_v1.{hpp,cpp}: pure parser/query for a segment table. Sidecar `<movie>.segments.txt` (tools/intro_v1.segments.txt: `0.0 0 gameloft_logo`, `7.3 1 story`), copied next to the movie by tools/convert_intro_video.ps1.
- boot_runner_v1.cpp: SKIP drawn and presses accepted only when the table says skippable at the movie clock; otherwise the press is ignored (counted). Missing/invalid table: whole movie skippable (old behavior, logged as segments="none ...").
- Boot log line now has segments="..." presses_ignored=N.
## Tests
- ctest startup_intro_segments_v1 (parse, boundary 7.3, no table, bad input), startup_boot_flow_v1 unchanged; full ctest: only session_skill_binding (known) and winmm_pump_priority_v1 (0xc0000135, missing DLL in this env, unrelated) fail.
- Quiet runs (P15 assets, movie copy + sidecar in .local-inputs/claude-preview15/fix054/media, via --intro-movie): nopress (frames at 3 s logo no SKIP, 6 s black no SKIP, 9 s and 20 s story with SKIP); logopress (presses at 3 and 5 s: presses_ignored=2, movie still playing at 10 s); storyskip (press 3 s ignored, press 12 s: movie="skipped: user", title "Touch the screen to continue" at 13 s). Composite: fix054/after.png.
## Package files required
- `assets/converted-media/intro_v1.segments.txt` (copy of port/windows-foundation/tools/intro_v1.segments.txt). Without it the old behavior (SKIP always) remains.
## Gaps
- 7.3 s is the original activity's touch threshold on intro.mp4 time; our re-encode is the same timeline (same duration within 0.1 s). Tap-to-reveal and the original subtitles (timed TextView strings) are not implemented here.
