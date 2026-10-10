# STARTUP report (Preview 15, branch p15/startup)

Status: PARTIAL. Boot flow (logo, movie with skip, touch-to-continue title, main menu) and the campaign loading
bar are implemented and visually verified in quiet runs. Not done: loading tips and loading art, movie audio through the
mixer, and the original logo look. See section 8 (remaining gaps). Do not call the stream complete.

## 1. Goal
Original startup flow, portable by design: Gameloft logo -> intro movie (skip) -> title screen "touch to continue" -> main menu;
Single Player -> campaign loading screen with tip + progress bar tied to real load stages and a minimum display time.
Startup mode: default production boot; `--start-mode menu|swamp` kept; `--skip-boot` for tests.

## 2. Evidence
### 2a. Original boot logic (IDA, pseudocode-all.c)
- `GSInit::Update` (pseudocode line ~99355-99594) is the boot state machine, driven by `this+1` step index:
  0 VoxSoundManager::CreateInstance; 1 SavegameManager::loadSettings(1); 2 PyDataConstants::Load (polled, yields after 49 ticks);
  3 PyDataArrays::Load (polled); 4 loadSettings(0) if not already; 5 `nativeLoadMovie("intro_jp.mp4"|"intro_kr.mp4"|"intro.mp4")`
  by language (4=JP, 5=KR, else intro.mp4); 6 wait until `videoDone`; 7 LoadBackground: splash texture chosen by language and
  screen width: `splash_final_jp.tga` (lang 4), `splash_final_kor.tga` (lang 5), `splash_final_i9000.tga` (width 800),
  `splash_final_droid.tga` (width 854), else `splash_final.tga`; 8 done; 9 VoxSoundManager::Initialize; 0xA StringManager::switchPack;
  0xB TrophyManager; 0xC MenuManager::GetInstance; 0xD Random seed; 0xE MenuManager::Reset then GSFlashMenu with
  "menu_splash" (or "menu_language" when the flag at savegame+55 is set).
- `videoDone` is set to 1 only in `appInit` (reset to 0) and `appResume` (after device pause). So the original movie ends or is
  skipped by returning control to the game (Android player activity), not by a native timer. (Inference from the set/clear sites.)
- `menu_splash` push calls `GSInit::ClearLoadingScreen`, i.e. the title screen replaces the splash background.
- Title text: `MENU_TOUCH_TO_CONTINUE` = "Touch  the  screen  to  continue"; skip label `MENU_SKIP` = "SKIP"
  (features/frontend/art/original_art_data.cpp).

### 2b. Reference video (v1.0.3 Part 1, /c/Users/adamc/Desktop/dh2_video_research/video/part1_z_Zky7qQdYs.mp4)
- Strip 0-8 s (`.local-inputs/claude-preview15/startup/ref-boot-0-8s-tile.png`): streaks converge into a blue-glow "G" logo from
  about 0.25 to 2 s, glow/flash at 2-3 s, wordmark "GAMELOFT" appears around 3 s, full logo with light sweep at about 4 s.
  Observed directly from frames; exact durations are inference from 4 fps sampling.
- The reference logo is that animated blue logo. Our boot logo phase draws the static `gameloft.tga` (orange wordmark on a white
  box), which does NOT match the reference (see gaps).
- Loading screen reference, Part 1 at 70/73/76 s (`.local-inputs/claude-preview15/startup/final/png/compare-loading-ref-part1-vs-ours.png`,
  left three frames): parchment frame with ornate border, "LOADING" heading, a framed tip box ("It is an area blocked by traps,
  look around, you should find a way to open up the path."), a red progress bar with a spark, a small corner badge, and a
  SKIP control on the level-transition frame. Observed directly; tip text and art are not implemented in the port.

### 2c. Assets (package preview-13 assets)
- `assets/data/3d/textures/gameloft.tga`, `splash_final*.tga`, `assets/original-media/intro.mp4` (20.2 MB),
  `assets/original-media/title_intro.wav` (746,622 bytes), `front-compat/loadanims_droid.swf`, `original-cache/data/menus/loadanims*.swf`.
- pl_mpeg (pl_mpeg.h): NOT present anywhere on this machine (search of DH_sc and whole drive). Fallback chosen (section 3).

### 2d. Campaign loading (original tips)
- `NativeGetLoadingTipStrID` (IDA 0043cb24): picks a random row of `Arrays::HintPages` (12-byte rows, string id at +8) with an LCG
  (`Random::s_seed = (59051*s + 177149) % 0xDAF26B`), returns `HintStringID`. The data is `help_pages_pyarray.bin` +
  `help_pages_pyarraynames.bin` + `help_pages_pystructnames.bin` (schema `HintAvatarID`, `HintStringID`).
- `engine-ui/loading_menu_v1.cpp` already has `LoadingHintTableV1` (loads those three blobs, schema-checked, `next()` with the LCG)
  and `loading_menu_progress_v1`. It is NOT wired into the Windows boot or loading screen (the tip text needs the `HudTextV1`
  string table with a loaded pack: `HudTextV1::integer_string`).

## 3. Decisions
- Intro movie: pl_mpeg is not on this machine and cannot be downloaded here. Fallback: the movie is converted offline
  (`port/windows-foundation/tools/convert_intro_video.ps1`, ffmpeg asset-prep only) into our own container
  `DH2INTR1` (layout in `features/startup/intro_stream_v1.hpp`): RGB565 640x360 @ 24 fps, each frame XOR'd with the previous
  frame, deflate-compressed, decoded by our own `inflate_raw_v1.cpp`. Plain C++, no platform calls, no FFmpeg at runtime.
  Porting cost: the decoder is ~200 lines of portable C++, same on Linux/Android. Known cost: the file is large (see section 5).
- Boot state machine (`boot_flow_v1.cpp`) is pure; the runner (`boot_runner_v1.cpp`) owns window/renderer/overlay calls and
  the abstract press edge (mouse_left/touch, space, enter, escape). Scripted presses (`--boot-press`) use the same edge.
- Logo timing (fade 0.5 s, hold 2 s) is an implementation choice, not recovered.
- Splash variant follows GSInit case 7 with language 0 assumed (`splash_final.tga` at 1280 wide; droid/i9000 by width).

## 4. Implementation (commits on p15/startup)
- c80660f6: boot state machine, intro stream decoder + inflate, offline converter, tests.
- b6622325: boot runner (logo, intro movie, touch to continue), `--skip-boot`, `--intro-stream`, boot hook on first menu entry.
- efbbadab: title label, boot verification hooks (`--boot-press`, `--boot-capture <sec>=<file>`, `--boot-max-seconds`),
  campaign loading screen with real stage progress.
- This session:
  - CMakeLists.txt: the two startup test executables were added before `add_link_options(-static)`, so they linked the UCRT
    forwarder DLLs and failed with 0xc0000135 under ctest. Fixed: `-static` on both test targets (same as the other tests).
  - boot_runner_v1.cpp: label builder generalised (`build_text_label`, center or right-bottom anchor); skip overlay "SKIP"
    (MENU_SKIP text) drawn bottom-right during the movie. Placement is a port choice; the original skip is a native player control
    not visible in our assets, so the placement is NOT verified against the original.
  - loading_screen_v1.cpp/.hpp + main.cpp: `--loading-capture <prefix>` writes `<prefix>-NNN.ppm` once per distinct stage fraction
    (verification hook only).

## 5. Verification (quiet, hidden-desktop, silent runs; EXE = build-startup)
- Build: `p14_build.ps1 -Name startup -Test` build exit 0. ctest: startup_boot_flow_v1 and startup_intro_stream_v1 pass after the
  static fix (2/2). Full ctest (104): only `session_skill_binding` fails, the known worktree junction issue.
- Converter (ffmpeg 8.1.2, Gyan build), run took 40 s:
  - `converted-media/intro_v1.dhintro`: 177,486,569 bytes, sha256 F8D8F7F060A3D8F668CCD259928A69340DC8751FD039519EA262473347177B24,
    1207 frames, 640x360, 24 fps.
  - `converted-media/intro_v1.wav`: 9,633,896 bytes, sha256 23C389E5E304137D10CB47FDF8AC0882578D42C870560598F9954FFB9811E3B4,
    48 kHz 16-bit stereo PCM.
  - Output folder: `.local-inputs/claude-preview15/startup/converted-media/` (not in the Preview package).
  - Size risk: the stream is ~9x the 20 MB source. Open decision: lower resolution, better inter-frame coding, or a real MPEG-1 decoder.
- Movie vs reference: boot time T shows movie time T-3 (3 s logo). `final/png/compare-movie-ours-left-ref-right.png`: our frames at
  movie 12, 22, 37 s (left) against `ffmpeg` frames of intro.mp4 at the same times (right). Visually matching (same scene, same
  position, same colours). The SKIP overlay is visible bottom-right.
- Skip: `--boot-press 6` at boot time 6 s: log `movie="skipped: user" movie_frames=72 seconds=9.01`; `skip/s-6.5.ppm` shows the
  title. Title screen: `title/t-56.ppm` shows "Touch the screen to continue" over the splash.
- Touch-to-continue then main menu: job `menu` (presses at 6 and 9 s): boot `outcome=0` (complete) at 9.03 s, then the menu
  renders (`menu/menu-end.png`: Start game / Options / Info, class QA Warrior).
- Single Player -> loading (job `loading`, args `final/loading/startup.args`, presses 6 and 9, then `--menu-actions
  menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER`): captures `load-000/040/080/100.ppm`
  (stages: start 0, level loaded 0.4, actors 0.8, finish 1.0). Exit 0, log ends with `Population final enabled=12`.
  Observed: black screen with a thin amber bar at 0%, 40%, 80%, 100%. The bar fills with the real stages. The minimum display
  time (1.5 s) is applied in `finish()`.
- `--skip-boot`: job `skipboot` exit 0, menu verification runs and prints the profile line; no boot lines in the log.
- `--start-mode swamp`: job `swamp` (args from preview-12 `b004-P/run.args` with the save copy) exit 0, `Level triangles=24738`,
  no boot line, no loading screen. Job `swamp-legacy-noflags` (`--start-mode swamp --skip-boot --frames 5`) exit 0.
- Movie audio: NOT verified. The boot runner does not play `intro_v1.wav` at all (no mixer call). See gaps.

## 6. Verifier script (exact quiet jobs; run with `quiet_run.ps1 -JobsFile jobs.json -Parallel 8`)
Jobs (all `--start-mode menu --assets <preview13 assets> --intro-stream <converted-media/intro_v1.dhintro> --save <copy>`):
- logo: `--boot-capture 1.5=l-1.5.ppm --boot-capture 2.5=l-2.5.ppm --boot-max-seconds 3.2`. Expect a white box with the orange
  gameloft.tga wordmark (NOT the reference logo) and `Boot outcome=1 movie="stopped before movie end"`.
- movie: captures at 6, 15, 25, 40, 52 s, `--boot-max-seconds 54`. Expect `movie="played" movie_frames=1203`.
- skip: `--boot-press 6`, captures 5.5/6.5/8, `--boot-max-seconds 9`. Expect `movie="skipped: user"`, title at 6.5 s.
- title: capture 56 s. Expect touch-to-continue title.
- menu: `--boot-press 6 --boot-press 9 --menu-frames 40 --menu-capture-directory <d> --capture <d>/menu-end.ppm`. Expect `outcome=0`
  and the main menu.
- loading: `--startup-config startup.args` (contents in `final/loading/startup.args`). Expect 4 loading captures and exit 0.
- skipboot: `--skip-boot --menu-frames 5`. Expect no boot line, menu verification output.
- swamp: `--startup-config` with the preview-12 b004-P args. Expect `Level triangles=` and no boot line.

## 7. Package files required (runtime)
- `converted-media/intro_v1.dhintro` under the assets root (default path `<assets>/converted-media/intro_v1.dhintro`, or
  `--intro-stream <file>`). Without it the movie is skipped with the reason logged, and the boot continues to the title.
- `converted-media/intro_v1.wav`: needed once the movie audio is wired (currently unused by the EXE).
- Existing assets used: `data/3d/textures/gameloft.tga`, `splash_final*.tga`, `data/Fontin SmallCaps.ttf`.
- Not yet needed but required for the loading tips: `help_pages_pyarray.bin`, `help_pages_pyarraynames.bin`,
  `help_pages_pystructnames.bin`, and the string table pack used by `HudTextV1`.

## 8. Remaining gaps (honest list)
1. Movie audio: not played. Integration path: `audio_sample_open_v34` (engine-audio/audio_sample_v34.cpp) accepts a RIFF PCM16
   WAV (format 1), and `AudioMixerV34::post(AudioCommandV34{kind=play, sample=...})` plays it on the mixer output clock. The boot
   needs a mixer owner (the frontend audio session is source-cue based and WinMM/V42-bound, so a small owner is needed). Not done.
2. Loading tips and art: no tip text (use `LoadingHintTableV1` + `HudTextV1`, see 2d), no parchment frame, no SKIP control on the
   loading screen. The current screen is black with a bar only (documented in loading_screen_v1.hpp).
3. Logo: the boot shows `gameloft.tga` (orange on white). The reference shows the animated blue logo. Needs an evidence-based
   logo sequence (or the movie's own opening) before this is called fixed.
4. Skip button placement and look: port choice, unverified against the original.
5. Loading stages: only 4 real stages (0, 0.4, 0.8, 1.0); finer progress is not wired.
6. Stream size 177 MB (see section 5).
7. Converted stream is 640x360 (source 1280x720): fine for a 640-wide preview; not a 720p output.
8. The reference loading frames (70-76 s) do not give the exact original timing; the minimum display time 1.5 s is a choice.
