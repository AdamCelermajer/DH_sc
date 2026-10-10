# STARTUP report (Preview 15, branch p15/startup, round 2)

Status: boot flow (intro movie with SKIP -> "Touch the screen to continue" -> main menu) and the campaign
loading screen (tip box from the original hint table, red bar on the real stages, 1.5 s minimum) are implemented
and verified in quiet batches. The movie picture and soundtrack are both driven by the mixer clock. Not done:
original loading art (ornate frame, avatar badge), finer load stages, Linux/Android audio output for the boot.
Do not call the startup stream complete.

## 1. Changes in round 2
- Intro movie replaced: the 177 MB custom zlib stream and its inflate decoder are removed. The movie is now
  `intro_v1.mpg` (MPEG-1 video + MP2 audio), decoded by pl_mpeg (`third_party/pl_mpeg/pl_mpeg.h`, MIT).
- Soundtrack plays on the shared audio mixer (`AudioMixerV34`) as one voice; the movie clock is the mixer's
  output frame counter minus the platform queue (`intro_soundtrack_v2`). Video follows that clock.
- Separate static Gameloft logo stage removed: the logo is the first part of intro.mp4 (checked: frame at
  0.5 s shows the silver Gameloft logo with SKIP bottom-right). Order: movie -> title -> menu.
- Loading screen: tip text from the original hint table (LCG + common_text), shared text labels, tip box,
  red progress bar with a spark.

## 2. Evidence
### 2a. Original order (IDA, from round 1, unchanged)
`GSInit::Update` (pseudocode-all.c ~99355-99594): step 5 `nativeLoadMovie("intro.mp4")` (JP/KR variants by
language), step 6 waits for `videoDone` (set by appInit/appResume, i.e. the player ends or is skipped), step 7
splash by language/width, then `menu_splash` (title, "Touch the screen to continue", `MENU_SKIP` = "SKIP").
### 2b. Movie (direct observation, this round)
- `r2/movie/m-0.5.png`: silver Gameloft logo on black, SKIP bottom-right. The reference video's logo at 0-4 s
  looked blue and animated (round 1 notes). Difference NOT resolved (see risks).
- Picture vs ffmpeg frames of the same .mpg (`tools/verify_intro_movie.py`): 6 captures at 0.5-50 s, best PSNR
  35.5-37.8 dB (40 s: 37.4 dB; 50 s end card: 77.8 dB), 0-3 frames behind nominal time. Side-by-side check at 25 s:
  same scene, same position (`r2/verify-movie/side-25.png`, scratch).
- Unit test: decoded frame 24 vs ffmpeg rgb24, PSNR 55.9 dB.
### 2c. Loading (reference Part 1, 70-76 s, round 1 + this round)
Dark ornate frame, "LOADING" heading, framed tip box with the text, red bar with a bright spark. Tip logic:
`NativeGetLoadingTipStrID` (IDA 0043cb24): help-page row picked by the LCG `(59051*s + 177149) % 14348907`,
string id from the row (`engine-ui/loading_menu_v1.cpp`, `LoadingHintTableV1::next`).
Observed tips in our runs: "If you do not know where to go, open your Quest Log..." and "The World Map allows
you to travel..." (different per run; seed = tick count, logged as `Loading tip string_id=`).
### 2d. Not directly verified
Exact tip box geometry, fonts, and the avatar badge of the original; the original skip control on the campaign
loading screen (the SKIP frame in the reference is the level-transition screen, not the campaign loading).

## 3. Decisions
- Movie: ffmpeg `mpeg1video 1024x576 24 fps -b:v 3000k` (maxrate 3500k, bufsize 1835k, GOP 24, no B-frames) +
  `mp2 192k 48 kHz stereo`. 1024x576 is 16:9 and macroblock aligned in height (576 = 36 x 16). Size 19.9 MB.
- Start lead 0; the platform queue latency is subtracted from the clock as `kWinmmBufferCount *
  kWinmmFramesPerBuffer` = 4096 frames (~85 ms). Measured picture offset: 2-3 frames (~100 ms) behind nominal
  time, consistent with the queue being full. Estimate, not a measured latency.
- Soundtrack is decoded in one pass at movie start (2.41 M stereo frames, ~9.6 MB PCM, a WAV copy of the same
  size is made for the mixer sample). Chosen because the mixer plays whole samples; a streaming owner is a later
  change if memory or start time matters.
- Movie ends at its last frame time (1207 / 24 = 50.29 s on the audio clock). Skip ends the voice (`stop_all`).
- The boot owns the WinMM output only while the boot runs (closed before the menu). Without an audio output the
  movie falls back to the wall clock and logs `movie_clock="wall"`.
- Loading tip uses `MenuLocalization` (common_text pack 0) and `LoadingHintTableV1`; the same text stack as the
  character menu.

## 4. Implementation (commits on p15/startup)
- `052e77b8` pl_mpeg.h added (MIT).
- `88c0bee5` intro movie (pl_mpeg), soundtrack owner, boot flow without the logo, old stream removed, converter
  rewritten (ffmpeg only), `--intro-movie` replaces `--intro-stream`. Tests: boot flow, intro movie.
- `04da4a29` soundtrack clock at movie end/skip, zero lead, `soundtrack_released` logged, verifier.
- `cf73c0ee` loading screen with tip (loading_tip_v1), shared text labels (text_label_v1), tip box, red bar.
- `3241ddb1` third-party notice (`third_party/THIRD_PARTY_NOTICES.md`) and this report.

Files: `features/startup/{boot_flow_v1,boot_runner_v1,intro_movie_v2,intro_soundtrack_v2,pcm_wav_v2,text_label_v1,
loading_tip_v1,loading_screen_v1}.{hpp,cpp}`, `intro_movie_v2_tests.cpp`, `boot_flow_v1_tests.cpp`,
`tools/convert_intro_video.ps1`, `tools/verify_intro_movie.py`, `main.cpp` (boot block, loading screen), CMake.

## 5. Verification (quiet batches, hidden desktop, DH_AUDIO_SILENT=1; EXE = build-startup)
Build: `p14_build.ps1 -Name startup -Test` builds clean. Unit tests pass: `startup_boot_flow_v1`,
`startup_intro_movie_v2` (with the converted .mpg and the ffmpeg frame 24 reference; PSNR 55.9 dB, 1207 frames,
soundtrack 50.184 s).
Jobs: generator `.local-inputs/claude-preview15/startup/r2/make-jobs.ps1` -> `r2/jobs.json`
(`quiet_run.ps1 -JobsFile jobs.json -Parallel 6 -Summary summary.json`). All 6 exit 0 (last batch):
- `movie`: `Boot ... movie="played" movie_frames=1206 movie_clock="audio" soundtrack_seconds=50.2933
  soundtrack_duration=50.184 soundtrack_released=1`. Captures 0.5/6/15/25/40/50 s checked by the verifier (PASS),
  52 s = title (`movie/t-52.png`: splash with "Touch the screen to continue").
- `skip`: press at 6 s -> `movie="skipped: user" movie_frames=144 soundtrack_seconds=5.97`; the clock is the
  audio clock (6 s x 24 = 144 frames). Title at 6.5 s.
- `menu`: press 6 and 9 -> menu captures (`menu/`), exit 0, boot outcome 0.
- `loading`: presses 6 and 9, Single Player -> loading captures `load-000/040/080/100.ppm` (bar 0/40/80/100 %),
  tip logged, `loading/load-040.png` shows the tip box and red bar. Exit 0.
- `skipboot`: exit 0, no boot line.
- `missing`: movie file absent -> `movie="skipped: intro movie not found: ..." movie_clock="wall"`, boot continues.
- Verifier: `py -3 tools/verify_intro_movie.py --ffmpeg <ffmpeg.exe> --movie <intro_v1.mpg> --capture 6=<m-6.ppm>
  ... --log <movie/log.txt>` -> `intro movie verification: PASS` (audio 50.1840 s = ffmpeg 50.1840 s).

## 6. Package files required (runtime)
- `converted-media/intro_v1.mpg` under the assets root (default path; or `--intro-movie <file>`).
  19,894,272 bytes, SHA256 `B3DCC8A10445C09D5A1AE2543D12D1BF32949C54759DEDFEE8CCE2728C3B6DD8`
  (scratch: `.local-inputs/claude-preview15/startup/converted-media-v2/`). Without it the movie is skipped with the
  reason logged and the boot continues to the title.
- Existing assets used: `data/3d/textures/splash_final*.tga`, `data/Fontin SmallCaps.ttf`,
  `data/help_pages_pyarray.bin`, `data/help_pages_pyarraynames.bin`, `data/help_pages_pystructnames.bin`,
  `data/pydata/common_text_pyarray.bin`, `common_text_pyarraynames.bin`, `common_text_pystructnames.bin`,
  `common_text_pycst.bin`, `data/fonts_pycst.bin`. `gameloft.tga` is no longer used by the boot.
- Notices: `port/windows-foundation/third_party/THIRD_PARTY_NOTICES.md` (pl_mpeg MIT text, flagged for a check against
  the upstream LICENSE file, which was not in the downloaded header).
- Old `converted-media/intro_v1.dhintro` and `.wav` (177 MB + 9.6 MB) deleted from scratch; not needed.

## 7. Verifier script
`port/windows-foundation/tools/verify_intro_movie.py` (stdlib Python 3, `py -3`): picture check (aspect-fit
window sampling vs ffmpeg frames, PASS needs PSNR >= 30 dB within 4 frames of time x 24) and soundtrack length
check against `soundtrack_duration=` in the boot log (20 ms tolerance). Logos/loading images are checked by eye.

## 8. Open risks and gaps (honest list)
1. Loading art: the ornate frame, corner ornaments and the avatar badge (loadanims SWF / dqmenus bitmaps) are not
   rendered; the frame is drawn with fills. Colours are sampled by eye from the reference frames.
2. Loading stages: still 4 real stages (0, 0.4, 0.8, 1.0). Finer progress is not wired.
3. Fresh saves: the loading job only reached Single Player with a seeded save (`r2/loading/p.savegame`, copied
   from an earlier run). With a fresh save the menu action failed ("Source button is not active"). Not investigated;
   may be a pre-existing menu-state issue, not caused by the boot.
4. Logo: the movie shows the silver logo (intro.mp4). The round-1 reference video showed a blue, animated logo at
   0-4 s. Either a different encode/version of the reference or a misread; unresolved, not claimed as matched.
5. Audio output: the boot audio uses WinMM only (`WinmmAudioOutput`). On Linux/Android the boot falls back to the
   wall clock until an output that pumps this mixer is wired (LinuxSdl2AudioOutput exists; Android AAudio not wired).
6. Latency compensation is an estimate (ring size, ~85 ms); measured picture offset 2-3 frames. Per-platform
   latency values are needed.
7. Memory/time: the whole soundtrack is decoded at movie start (~9.6 MB PCM + 9.6 MB WAV copy). Decode time not
   measured here.
8. Title: the "Touch the screen to continue" label placement and the splash placement are port choices; the splash
   art occupies the top-left of the window in our capture (`movie/t-52.png`), not verified against the original.
9. Skip button placement and look: port choice, not verified against the original (the original skip is a native
   player control).
10. `movie_frames` counts frame updates per loop iteration (1206 for a 1207-frame movie); the verifier uses
    pixels, not this count.
11. Full ctest: see section 9.

## 9. Test status
Full `ctest` after the last commit: 103/104 pass. The only failure is `session_skill_binding` (known: the .local-inputs junction in worktrees, ignored per the brief). `startup_boot_flow_v1` and `startup_intro_movie_v2` pass.
