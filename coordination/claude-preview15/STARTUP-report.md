# STARTUP report (Preview 15, branch p15/startup)

Status: IN PROGRESS. Investigation done (sections 2); implementation in progress (sections 4-7).

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
- Title text: `MENU_TOUCH_TO_CONTINUE` = "Touch  the  screen  to  continue" (features/frontend/art/original_art_data.cpp).

### 2b. Reference video (v1.0.3 Part 1, /c/Users/adamc/Desktop/dh2_video_research/video/part1_z_Zky7qQdYs.mp4)
- Contact sheet s001 (0-20 s): black at 0 s, Gameloft logo at 4 s, parchment intro cinematic from about 8 s.
- Strip 0-8 s at 4 fps (`.local-inputs/claude-preview15/startup/ref-boot-0-8s-tile.png`): streaks converge into the logo from
  about 0.25 to 2 s, glow/flash at 2-3 s, wordmark "GAMELOFT" appears around 3 s, full logo with light sweep at about 4 s.
  Observed directly from frames; exact durations are inference from 4 fps sampling.

### 2c. Assets (package preview-13 assets)
- `assets/data/3d/textures/gameloft.tga`, `splash_final*.tga`, `assets/original-media/intro.mp4` (20.2 MB),
  `assets/original-media/title_intro.wav` (746,622 bytes), `front-compat/loadanims_droid.swf`, `original-cache/data/menus/loadanims*.swf`.
- pl_mpeg (pl_mpeg.h): NOT present anywhere on this machine (search of DH_sc and whole drive). Fallback chosen (see section 3).

### 2d. Campaign loading (current port)
- No loading screen exists in main.cpp; the Single Player handoff loads synchronously. Real stages in main.cpp: `load_level`
  (~line 824), `population.load` (~915), collision setup (~1014), texture loads (lazy `loadTexture`).

