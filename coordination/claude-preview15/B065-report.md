# B065 report: movie sound after SKIP, silent logo, title music

## Findings (evidence)
- (b) Logo has no sound in the SOURCE. `original-media/intro.mp4` (AAC) 0-5 s: peak -40.0 dB, RMS -41.5 dB, constant (DC/hum floor, not audio). Per-second RMS of the converted `intro_v1.mpg` (ffmpeg astats, 1 s windows): s0-5 = -42 dB, s6 = -16, then -8..-25 dB until s48, tail to -59. So the conversion (tools/convert_intro_video.ps1 maps 0:a:0 untouched, 48 kHz MP2) loses nothing; the recording has no logo jingle. Whether the original game plays a jingle there is unknown (the reference mp4 has no audio); nothing was invented. The audio clock starts at once (movie_clock="audio", first frame held until audible, no frames dropped).
- (a) On Preview 15 the movie voice was only released at the END of the boot (after the title press): SKIP at 9 s, sound kept running to the second press (A/B on the P15 EXE: soundtrack_released only reported after the 14 s press). B064 (already in this branch) releases the voice on entry to the title.
- (c) P15 EXE log has no TitleMusic transition; this branch does.

## Verification (quiet runs, DH_AUDIO_SILENT=1, mixer counters)
- SKIP at 9 s: `Boot outcome movie="skipped: user" soundtrack_seconds=8.928 handoff_voices=0 handoff_released=1`, then `Frontend music transition: kind=start track=TitleMusic screen=title_splash` (after the stop).
- Natural end (press at 58 s): `movie="played" soundtrack_seconds=50.304 handoff_voices=0 handoff_released=1`, TitleMusic start follows.
- New diagnostic: `BootRunResult::handoff_voices/handoff_released` = mixer voices active when on_title_entered ran (must be 0), printed in the Boot outcome line.
- WinMM close() calls waveOutReset, so the ~85 ms ring is dropped at the title entry.

## Change
- boot_runner_v1: handoff evidence fields (hpp/cpp, main.cpp log line).
- New test `startup_intro_soundtrack_v2` on the real mixer: silent-head (logo) voice is active, clock runs; SKIP silences the next ring block and the whole 8x512 ring, voice released; duplicate stop and late stop after natural end harmless; second start refused.
- ctest: all green except session_skill_binding.

## Not verified / gaps
- Not verified by ear (silent mode; real WinMM output not listened to).
- Whether the original has a logo sound is unknown.
- Package gap (B054, not this bug): `assets/converted-media/intro_v1.segments.txt` is absent in the Preview 15 package, so the log says `segments="none (whole movie skippable)"` and the logo is skippable. Package files required: `converted-media/intro_v1.segments.txt` next to intro_v1.mpg.
