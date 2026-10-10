# B040 - original map ambience / level music missing in gameplay

Status: implemented and isolated-tested. NOT verified in the integrated EXE, NOT visually compared with the reference video.
Stage reached: investigation (logic) + implementation + asset/decode tests. Integrated runtime and visual checks are still owed.

## 1. Evidence

### Visual (reference video)
- Not done. No reference frames were extracted or viewed for B040, so there are no video timestamps for area changes.
  The verifier/fidelity agent must check swamp hub start, merchant camp safe zone, and any witch-cave transition.

### Logic (IDA libDungeonHunter2.so, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
- `Level::Update` (0x3f82d8, pseudocode ~l.179930): while `Level+324` (level-sound-started byte) is 0 it calls
  `VoxSoundManager::PlayMusic(Level+284 /*music*/, 1, manager+50 /*stop*/, 2000)` (or `SetInSafeZoneMusic` when manager+50 is set),
  then `VoxSoundManager::Play(Level+292 /*ambient*/, 0,0,0,0)`, sets manager+49=1 and the started byte. This is the level-start path.
- `Script_PlayLevelMusic::Execute` (0x45fd20, l.248275): `PlayMusic(CurrentLevel+284, 1, 0, script fade 2000)` then
  `SetMusicState("ambient"|"combat")`. Script id 23 `ResumeLevelMusic` (encounter-source-scripts.json) is this command, fade 2000.
- `VoxSoundManager::PlayMusic(id,enabled,stop,fade)` (l.79801): gated by `GetSoundVolume(2)>=0.5`, `IsDisablingSounds` off and
  `byte_99F7E0`. `id==-1` stops only when stop is set. Same id as current (manager+9) with a live emitter: `Resume(0.05 s)`, no restart.
  Otherwise StopMusic(old, fade), then Play(id, group 2, loop from sounds.xml).
- `SetInSafeZoneMusic` (l.79883): safe zone uses Level+120 (safezone music), fade 2000. Called by Script_EnterSafeZone/LeaveSafeZone (l.248259/248271).
- Death/revive: `PlayerManager::ReviveLocalPlayers` (l.87012): `StopAllMusic(2)` then `PlayMusic(Level+284,1,0,1000)`.
- Menus: `MenuMainMenu::Hide` StopMusic(1000) (l.215164); NativeStopMusic 500 (l.225355).
- Focus: `Application::Pause` (l.19751) records music-was-playing (+166) and `PauseAllSounds` (0.2 s); `Application::Resume` (l.24825)
  `ResumeAllSounds` (or `ResumeMusic(500)` in the IGP/IGM branch). Music is paused and resumed, not restarted.
- Level config for the swamp hub (`port/windows-foundation/assets/original-cache/data/scene/001_swamp.mlx`, `type="level"`):
  `music="SwampHubAmbientMusic"`, `safezone_music="SwampMerchantCampMusic"`, no `ambiant_music` attribute.
  So the hub has one music layer and no ambience layer. Water (uid 470) and witch (uid 468) are not referenced by this level.
- Sound table (`features/audio/assets/data/sounds/sounds.xml`): uid 467 `SwampHubAmbientMusic`, `m_level_swamp_sfx_swamp.vxn`, vxn, group 2, loop yes;
  uid 468 `SwampWitchCaveAmbientMusic`, `m_level_swamp_sfx_witch.vxn`; uid 470 `WaterTempleCaveAmbientMusic`, `m_level_swamp_sfx_water.vxn`.
  Hashes match `port/level-world/reference/audio-source-v34/routing-census.json` (sha256 980137ac..., c3530cfe..., d386f78b...).
- VXN initial state: `AudioGameplayRuntimeV42::fresh_native_state_v68` zeroes state 0 (source NativeSubDecoder C1).

## 2. Expected behaviour (implemented)
- Entering the swamp level starts `SwampHubAmbientMusic` (uid 467) looping, group 2, fade-in 2000 ms, native state 0.
- Requested while the window is unfocused or minimised: not started; started on the first focused, non-minimised frame.
- Same-ordinal request while playing: resume, no restart (PlayMusic same-id branch). New ordinal: stop old with fade, start new.
- Focus loss or minimise pauses all voices (mixer `pause_all`, the PauseAllSounds equivalent); regain resumes (`resume_all`).
- Reload/restore (`unbind`/`bind`) keeps the music running (unbind does not stop it), matching same-id PlayMusic.
- Missing source-table row or start failure is logged once per distinct error and retried each frame (no substitute sample).

## 3. Changes
Owned / new:
- `port/windows-foundation/features/audio/level_music_v1.hpp` (new): `LevelMusicNamesV1`, `read_level_music_names_v1` (reads the
  `gametype="LevelConfig"` element's `music` / `safezone_music` from the level scene via `dh2::loader::XmlDocumentV1`).
- `port/windows-foundation/features/audio/level_music_v1.cpp` (new).
- `port/windows-foundation/features/audio/level_music_v1_tests.cpp` (new), `run_level_music_v1_tests.ps1` (new).
- `port/windows-foundation/features/audio/assets/data/sounds/m_level_swamp_sfx_swamp.vxn`, `m_level_swamp_sfx_water.vxn`,
  `m_level_swamp_sfx_witch.vxn` (new, byte copies of `.local-inputs/audio-v34/cache/`, 12278272 bytes each).
- `port/windows-foundation/features/audio/source-subset-manifest.json`: 3 entries added to `entries` after `sfx_cutscene_moth.wav`.

Shared, minimal:
- `port/windows-foundation/features/audio/runtime_audio_host_v1.hpp`: new public `play_level_music`, `stop_level_music`,
  `level_music_ordinal`, `set_output_paused`; private `level_music_ordinal_{-1}` (after `manager_general_initialized_`).
- `port/windows-foundation/features/audio/runtime_audio_host_v1.cpp`: bodies inserted after `RuntimeAudioHostV1::source_ordinal`.
  Uses existing AudioGameplayRuntimeV42 primitives (`submit_plain_source`, `source_ordinal_playing`, `resume_source_ordinal`,
  `stop_source_sound_v106`, `fresh_native_state_v68`, `mixer().post`). `winmm_output.*` is NOT edited (only `winmm_monotonic_ns` is called).
- `port/windows-foundation/features/audio/runtime_session_audio_v1.hpp`: `set_level_music`; private `level_music_name_`, `level_music_error_`.
- `port/windows-foundation/features/audio/runtime_session_audio_v1.cpp`: (a) `window_activity` calls `set_output_paused` when focus or
  minimise changes; (b) new `set_level_music`; (c) `after_update` start tick, after `host_->update(error)` succeeds.
- `port/windows-foundation/main.cpp` (2 hunks):
  1. include: after `#include "features/audio/runtime_audio_host_v1.hpp"` (line ~60) add `#include "features/audio/level_music_v1.hpp"`.
  2. After `runtimeAudio=std::move(next);` inside the `if(options.runtimeAudio)` try block (before the catch with
     "Audio initialization diagnostic"): read the `levelMusic` names from `options.level` and call `runtimeAudio->set_level_music(levelMusic.music,error)`.
- `port/windows-foundation/CMakeLists.txt` (1 line + comment): `features/audio/level_music_v1.cpp`, after `runtime_session_audio_v1.cpp`.
  Required because main.cpp now calls `read_level_music_names_v1`.

## 4. Tests
Commands run (real output):
- `powershell -File port/windows-foundation/features/audio/run_level_music_v1_tests.ps1` (run through the PowerShell tool):
  all 21 native checks `PASS` (sounds.xml rows for uid 467/468/470 with vxn, loop yes, group 2; staged size 12278272; manifest entry;
  VXN opens via `audio_sample_open_v34`; native states present; fresh state 0 resolves a playlist; first decoded IMA block non-silent),
  then `ALL PASS (0)`. Then `HASH-PASS` for the 3 files (sha256 980137ac..., d386f78b..., c3530cfe...) and `HASH ALL PASS`.
  The first run failed with exit -1073741515 (llvm-mingw DLL not on PATH); fixed in the runner by prepending the toolchain bin.
- Syntax checks: `level_music_v1.cpp`, `runtime_audio_host_v1.cpp`, `runtime_session_audio_v1.cpp` with
  `-std=gnu++17 -fsyntax-only -Wall -Wextra -Werror` exit 0. `main.cpp` with the flags from `ninja -t commands dh-foundation.exe`
  (`-fsyntax-only`, no `-o`) exit 0, with only pre-existing warnings. The EXE was not built.

Not run / missing:
- No test of start/stop/transition/focus/death behaviour. These need a live `AudioNativeSessionV42` (the
  `DH2_AUDIO_NATIVE_SESSION_FIXTURE` seam exists) and were not written in this pass. This is the largest gap.
- No failure or duplicate-case test for `play_level_music` (same-ordinal resume, different-ordinal stop, missing row).
- No test of `read_level_music_names_v1` (needs an AssetCatalog; expected values are music "SwampHubAmbientMusic", safezone "SwampMerchantCampMusic").

## 5. Uncertainties and what the root/verifier must check
- Lead correction: `features/audio/frontend_menu_audio_v1.*` does NOT play `m_title`. It only plays the authored `MenuConfirm` sound
  (uid 2). No title-music owner exists to copy, so title music is still missing. This is outside B040 but is a related gap.
- Not wired: the original `AudioLevelGameplayV67::play_music/start_level_sound` owner (engine-audio) needs many bindings the Windows
  production path does not provide. I used the same runtime primitives with PlayMusic semantics instead. This is a deliberate
  deviation for review.
- Safe zone (`SwampMerchantCampMusic`, `m_safe_zone_01_sfx_swamp.wav`) is NOT staged and NOT switched. Enter/LeaveSafeZone are not wired.
- Witch cave (uid 468) and water (uid 470) are not started. No witch-cave level file is in `assets/original-cache/data/scene`
  (only `001_swamp.mlx`), and level transitions are not implemented (one level per process). The VXN files are staged for that future path.
- Death/revive: `PlayerManager::ReviveLocalPlayers` (StopAllMusic(2), then PlayMusic fade 1000) is not hooked. The port's revive op in
  main.cpp (~l.1534, `OriginalLifecycleOperation::revive`) is an actor operation; I did not confirm it is the same path.
- Menu return: the frontend is not audio-driven, and in-game menu resume goes through Application::Pause/Resume. Only the mixer
  pause/resume is implemented. I did not read the lifecycle gate's own focus handling in full, so the verifier must check there is no
  double pause or mute.
- Unfocused start: `submit_plain_source` requires a focused output, so the request waits. Verify music starts within a frame of focus
  regain and does not start while minimised.
- Packaging (root): the preview package's `audio-assets/data/sounds` (preview-11 has 244 WAVs and NO .vxn) must include
  `m_level_swamp_sfx_swamp.vxn`, `m_level_swamp_sfx_water.vxn`, `m_level_swamp_sfx_witch.vxn` from
  `port/windows-foundation/features/audio/assets/data/sounds/`, plus the updated `source-subset-manifest.json`. `sounds.xml` is unchanged.
  `tools/prepare_assets.py` does not exist in this tree, so the packager is outside the repo. The 3 files are untracked (~36 MB total);
  `.gitignore` only excludes `/.local-inputs/`, so the root must decide whether to track them.
- Integrated check for the verifier: run the swamp with `--audio` and `--audio-assets` pointing at a package with the VXN files. Expect
  audible looping swamp music after the first focused frame, with no "Level music diagnostic" line. Alt-tab or minimise should pause
  and resume it. Also check the "Level music diagnostic" line is absent.
