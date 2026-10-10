# B064 - title / main-menu music (fix064, branch fix/b064)

Bug: the user believes the original plays music/ambience in the title screen and main menu; our build was silent there except click sounds.

## Evidence (investigation)
Video (`Dungeon Hunter 2 (v1.0.3) Part 1`, frames 30-80 s, contact sheet in scratch): story cinematic -> "Dungeon Hunter II" logo -> title splash (loading spinner, (c) 2011 GAMELOFT) -> main menu (Start Game / Options / More Games, empty slot) -> Enter Name -> Choose a Class -> main menu with the created hero -> Loading (bar + tip, "Touch the screen to continue") -> in-game cinematic (SKIP). The mp4 has NO audio, so what is audible is NOT verified by ear; everything below is from data + IDA.

Logic (IDA `libDungeonHunter2.so` `pseudocode-all.c`, plus the constant pools/ActionScript of `dqmenus_droid.swf`):
- `NativePlayMusic` 0x43ad84: one string arg -> `Arrays::GetMemberIDByString<Sounds>` -> `VoxSoundManager::PlayMusic(id, loop=1, force=0, fade 2000)`. `NativeStopMusic` 0x43ad2c = StopMusic(500); no menu screen calls it.
- `VoxSoundManager::PlayMusic` 0x36bd78: same id already owned -> `Resume(emitter, 0.05 s)` (no restart); different id -> `StopMusic(fade)` then `Play(id, loop, fade)`. Gate: music volume >= 0.5.
- `dqmenus_droid.swf` actions calling `TitleMusic.NativePlayMusic`: `menu_MainMenu.onPush` ("IN PUSH MM", both movie builds, SWF offsets 272854 and 291257), the credits menus' `onPush`, and the `menu_splash` block ("touch the screen to continue", `MENU_LOADING_TITLE`, `ClickToContinue`). No other frontend screen (enter name, class select, options, start-game slot menu, loading) has a music call: the title track just keeps playing.
- Track: sounds.xml uid 464, label `TitleMusic`, `m_title.vxn` (group 1, loop=yes, generated ordinal 221). VXN = intro segment (5.8 s) + loop segment (113 s). Level start: `Level::Update` -> `PlayMusic(level music, fade 2000)` replaces it; return to menu: level `StopMusic(1000)` (existing B040), then main menu onPush plays TitleMusic again.
- Root cause of the silence: the frontend audio session (`frontend_menu_audio_v1`) only knew MenuConfirm; nothing called PlayMusic, and the title screen lives in the boot runner (own movie mixer). Second blocker found in the run: `m_title.vxn` has Afmt bits = 16 (level banks 4) and `audio_sample_v34` rejected it ("Required source IMA block layout"). The original reader overwrites that field with 16 (`DecoderNative::ParseFile`, case 0x746D6641, pseudocode-all.c ~1082027), so a VoxN IMA stream accepts 4 or 16. Sample counts match IMA 1024-byte stereo blocks exactly (segments 186635 / 3622847 frames), not PCM16.

## Change
- `features/audio/music_voice_v1.*` (new): PlayMusic/StopMusic voice semantics extracted from `RuntimeAudioHostV1` (level music) and shared; the host delegates (behaviour and logs unchanged).
- `features/audio/frontend_music_v1.hpp` (new): data-driven rule table screen -> track/fade (`title_splash`, `menu_MainMenu` -> TitleMusic, 2000) and a request director that keeps the request until the output is ready.
- `FrontendMenuAudioSessionV1::play_music/stop_music/music_playing` (same V42 runtime as the clicks); retries while the device clock is not ready.
- `flow::Services::menu_entered` + Navigator hook (initial main menu, every push/pop/back that changes the top menu).
- Boot runner `on_title_entered` hook (also releases the movie soundtrack so a skipped movie never overlaps the title track); main.cpp closes the boot output, starts the session and TitleMusic at the title screen, keeps the session through main menu, enter name, class select and loading; at the gameplay-audio handoff the title track fades out (600 ms) before the gameplay output opens, then the existing level music starts. Return to menu re-requests TitleMusic.
- `audio_sample_v34.cpp`: accepts VoxN IMA Afmt bits 16.
- Logs: `Frontend music transition: kind=start|resume|switch|handoff-stop ...` (same `music_transition_line_v1` as `Level music transition`), `Frontend music state: ordinal=221 playing=1` every 5 s (producer stats, looping proof).

## Tests
- ctest `frontend_music_v1`: rule table, director retain/consume, Navigator hook (initial/push/back/no duplicate), transition line format, VoxN Afmt bits 4/16 accepted, 8 and unknown codec rejected. Full ctest: only `session_skill_binding` fails (known worktree failure).
- Real EXE, quiet runs (`DH_AUDIO_SILENT=1`, hidden desktop), logs in `.local-inputs/claude-preview15/fix064/runs/`:
  - boot + title + menu (`menu-new`): `Frontend music transition: kind=start ... screen=title_splash ordinal=221 uid=464` at the title, `kind=resume ... screen=menu_MainMenu` (no restart), `playing=1` throughout. Baseline Preview 15 EXE (`menu-old`): no music lines.
  - 186 s run (`menu-long`, longer than the 119 s track): 36 state lines, all `playing=1`, none `playing=0` => loops.
  - empty slot -> Enter Name (`name-new`): no new transition, still `playing=1`.
  - menu -> Start -> gameplay (`full-new`): `handoff-stop fadeMs=600`, then `Level music transition: kind=start track=SwampHubAmbientMusic fadeMs=2000`.
  - gameplay -> pause -> main menu (`ret-new`): `Level ... kind=return-stop fadeMs=1000`, then `Frontend ... kind=start ... screen=menu_MainMenu`.
  - audio cadence, real time, `-Parallel 1`, 35 s gameplay after the menu (`cadsk-new`): `WinMM pump: underruns=0 refills=3291 maxGapMs=50.2 gapsOver40ms=1` (baseline `cadsk-old`: underruns=0, maxGapMs=3269.7 between menu and game). With boot + loading (`cad-new` vs `cad-old`): underruns=1 in BOTH (pre-existing, not from this change).
- NOT verified by ear (video has no audio, silent runs): that the audible track sounds right; only the submitted voice, ordinal/uid, looping and device receipts are proven.

## Remaining gaps / uncertainties
- The original crossfades title -> level music over 2000 ms at the first Level::Update; ours fades the title out over 600 ms, then the level music fades in over 2000 ms (separate output sessions). A short quiet gap while the level loads is possible.
- The credits/About menu also calls TitleMusic on onPush in the original; the desktop `menu_About` has no rule yet (SWF sprite to menu mapping not verified), so it keeps the current track.
- The music-volume gate (PlayMusic needs volume >= 0.5) and the Options music slider are not wired to this path (same gap as the level music).
- The first menu click sound at frame 0 is skipped because the device clock is not ready yet (pre-existing).
- Whether the original starts the title track at the very first splash frame or after the splash load completes cannot be told from data (the user video has audio; not checkable here).
- The frontend audio session is Windows-only (`_WIN32` in main.cpp); the voice, rule table and flow hook are portable.

## Package files required
- `audio-assets/data/sounds/m_title.vxn` (3,837,292 bytes): already in the Preview 15 package (manifest `audio-assets/data/sounds/m_title.vxn` and `assets/original-media/m_title.vxn`); nothing missing for TitleMusic.
- sounds.xml / sounds_pyarray row TitleMusic uid 464: present.
- Not needed / not present: `m_title_intro.wav`, `m_title_loop.wav` (uid 534/535) belong to the newer iOS sounds.xml only; `assets-extra/ios/data/sounds/m_title.vxn` is the same VXN. `m_world_map.wav` (WorldMapMusic) is in the package and unused until a world map screen exists.
