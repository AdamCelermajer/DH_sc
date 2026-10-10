# Group D - B040 level music follow-ups (Preview 13)

Status: investigation (IDA + SWF strings) and a partial implementation with isolated tests. NOT verified in the integrated EXE, NOT listened to, NOT compared with the reference video (no frames were extracted in this pass). Bug stays OPEN.
Repo HEAD at start: `f6b7f134`. No commit made.

## 1. Evidence

### Visual (reference video)
- Not done in this pass. No timestamps observed. Verifier/fidelity must check pause menu, death/revive, and return-to-menu against the video.

### Logic (IDA `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
- `VoxSoundManager::PlayMusic(id,enabled,stop,fade)` (l.79801): gated by `IsDisablingSounds`. Same id with a live emitter: `Resume(emitter,0.05 s)`, no restart. Same id without a live emitter: replay. New id: `StopMusic(old,fade)` then `Play(new, group 2)`. Sets `manager+9`=id, `manager+10`=previous id.
- `StopAllMusic(2)` (l.78557): `StopAllEmitters("MUSIC", 2)`, sets `manager+9=-1`, `manager+10=old`.
- `Level::Update` start (l.179930-179960): once per level (`Level+324==0`): `PlayMusic(Level+284, 1, stop=manager+50, 2000)` (or `SetInSafeZoneMusic` if `manager+50`), then `Play(ambient)`, `manager+49=1`.
- Safe zone `SetInSafeZoneMusic` (l.79883): enter (a2=1): `manager+50=1`; if `manager+49` (level started), `PlayMusic(Level+288 /*safezone id*/,1,0,2000)`. Leave: `manager+50=0`, `PlayMusic(Level+284, 1, 0, 2000)`. NOTE: the B040 report said Level+120; IDA shows Level+288. Corrected here.
- Script commands: `Script_EnterSafeZone` (l.248271) and `Script_LeaveSafeZone` (l.248259) call `SetInSafeZoneMusic(1/0)`. `Script_PlayLevelMusic` (l.248275, script id 23 `ResumeLevelMusic`) calls `PlayMusic(Level+284,1,0,script fade)` then `SetMusicState` ambient/combat.
- Death: `PlayerManager::_HandleLocalDeaths` (l.87231) and `_HandleGlobalDeaths` (state machine, timer 2000 ms, fade to black) contain NO music call. The only music calls on this path are in `PlayerManager::ReviveLocalPlayers` (l.86817): `StopAllMusic(2)` then `PlayMusic(Level+284,1,0,1000)`. Revive is called from `_HandleGlobalDeaths` (l.87165), `_HandleLocalDeaths` (l.87260) and `NativeReviveAllPlayers` (l.225551). So the original music stops and restarts at REVIVE (fade-in 1000 ms), not at death.
- Menus: `MenuBase::Show` sets `MenuBase::s_igmOpened=1` only for `menu_Ingame`, `menu_playlist`, `menu_Merchant` (l.210985); Hide clears it (l.210601). `Application::Resume` (l.24825): if IGP/IGM/death-screen flags and app+166 (music was playing) then `ResumeMusic(500)`, else `ResumeAllSounds`. `Application::Pause` (l.19751): records music-playing, `PauseAllSounds`.
- `MenuMainMenu::Hide` `StopMusic(1000)` (l.215164). `NativeStopMusic` 500 (l.225355). `NativePlayPreviousMusic` (l.225360) = `RestartMusic(0)` (guarded by `manager+10 != -1`).
- `NativePauseMusic` (l.225331) = `VoxSoundManager::PauseMusic` (l.79153): pauses only the music emitter at `manager+36` (not all sounds).

### Logic (menu SWFs, extracted from `.local-inputs/connected-player-hud-final.apk`)
- `assets/original-cache/data/menus/dqhud_droid.swf` (IGM/HUD): string order near `onHide.onShow.InIGM.NativePauseMusic.NativeShowStatusBar` (inference: IGM onShow pauses music). `NativePlayPreviousMusic` appears in the fast-travel back button (`NativePopAllMenus.NativePlayPreviousMusic`), not in an IGM close path. The IGM close resume call was NOT identified (no `NativeResumeMusic` string in this SWF). Open.
- `dqmenus_droid.swf` (frontend): main menu `onPush` -> `TitleMusic.NativePlayMusic` ("IN PUSH MM"). Title music is uid 464 `m_title.vxn` (group 1). The port's `frontend_menu_audio_v1` does not play it (confirmed by grep: only MenuConfirm).

### Level config and reachability
- `001_swamp.mlx` LevelConfig: `music=SwampHubAmbientMusic` (uid 467), `safezone_music=SwampMerchantCampMusic` (uid 505, `m_safe_zone_01_sfx_swamp.wav`). No witch/water in this level.
- Merchant camp trigger: `_prim_Zone_EnterLoc_MerchantCamp` -> `enterLocation_MerchantCamp` (script 42, `Script_EnterSafeZone`) in `merchantcamp_ruins_swe_00.mgp` (swamp module, same scene). Exit: script 52 `Script_LeaveSafeZone`. Source: `port/windows-foundation/reports/encounter-source-scripts.json` (trigger list).
- Script id 23 (`ResumeLevelMusic`) has no `Script_ExecScript` caller in `encounter-source-scripts.json` (48 ExecScript commands checked). Its caller is unresolved.
- Witch cave (uid 468) and water (uid 470): only reachable through level transitions, which the port does not implement (one level per process; only `001_swamp.mlx` in `assets/original-cache/data/scene`).
- Combat/ambient interactive state (`SetMusicState` from AISPlayer aggro, l.160277/160337/AISPlayer::OnDeAggro): not implemented. The port has no aggro-count music owner.

## 2. Expected behaviour and what was implemented

| Original behaviour | Source | Status in this change |
|---|---|---|
| Level start: swamp hub music once, fade-in 2000 ms | Level::Update | Kept (existing); now logs `kind=start`. Start gate extracted and tested. |
| Same track while playing: resume, no restart | PlayMusic same-id | Kept (existing); host reports `resumed`. |
| Focus loss / minimise: pause all, regain resume | Application::Pause/Resume | Kept (existing mixer pause_all/resume_all); now logs `output-pause` / `output-resume` with focus state. |
| Pause menu (menu_Ingame): pause the music emitter, resume on close | PauseMusic + SWF strings | NOT implemented. Needs a per-voice pause primitive for the music ordinal. `port/engine-audio/audio_gameplay_runtime_v42.*` has resume_source_ordinal but no pause_source_ordinal; the mixer has `pause_voice`. Outside this group's file scope. The IGM close path is not identified. Do not use pause_all here: it would also pause SFX. |
| Death: no music change | _HandleLocalDeaths / _HandleGlobalDeaths | Nothing to implement (no music call on the death path). |
| Revive: stop (2 ms), restart the level music with fade 1000 | ReviveLocalPlayers | API added (`on_local_players_revived`), with deferred restart when unfocused. NOT wired: no global-death / revive flow is called from the normal EXE (`death_restart_v1` is not referenced by main.cpp). |
| Return to menu: stop music (fade 1000) | MenuMainMenu::Hide StopMusic(1000) | Implemented: `on_return_to_menu` called in main.cpp before the returnToFrontend `continue`. Caveat: the session is torn down in the same iteration, so the 1000 ms fade is not rendered. The stop is effectively immediate. Logged as `kind=return-stop`. |
| Return to menu: title music (uid 464) | frontend MM onPush TitleMusic | NOT implemented: owner is the frontend (`frontend_menu_audio_v1`, not in this group's scope). |
| Safe zone enter/leave: switch to uid 505 / back, fade 2000 | Script_Enter/LeaveSafeZone + SetInSafeZoneMusic | NOT implemented. Reachable in the swamp scene, file is staged, but there is no Windows owner for the Enter/LeaveSafeZone command. Would need a trigger hook plus a `set_safe_zone` API (PlayMusic switch semantics map to `play_level_music` with another ordinal). |
| Witch cave (468) / water (470) | Level config of their levels | NOT reachable in the current build (no level transitions). VXN files are staged. |

Implementation summary:
- `level_music_v1.hpp`: original fade constants (2000 start, 2 ms revive stop, 1000 revive restart, 1000 return stop, 50 ms same-id resume); `level_music_start_due_v1` (start gate); `level_music_transition_line_v1` (one log line format for every transition). Both helpers are inline so the standalone test compiles without the asset catalog.
- `runtime_audio_host_v1.*`: `play_level_music` now reports `LevelMusicActionV1 {unchanged, resumed, started, switched}`. Same-id, voice already ended, is reported as `started`.
- `runtime_session_audio_v1.*`: `after_update` start uses the gate and logs `start` or `switch`; new `on_local_players_revived` (stop 2 ms, deferred restart with fade 1000 when focused) and `on_return_to_menu` (stop 1000, clear track). `window_activity` logs `output-pause` / `output-resume` when a track is configured. Revive-pending state is cleared by `set_level_music` and return.

Diagnostic log lines (exact prefixes):
- `Level music transition: kind=start track=SwampHubAmbientMusic fadeMs=2000 ordinal=N`
- `Level music transition: kind=switch ...` (different track; old one stopped with fade)
- `Level music transition: kind=output-pause track=... fadeMs=0 focused=0 minimized=1` and `kind=output-resume ... focused=1 minimized=0`
- `Level music transition: kind=revive-stop ... fadeMs=2` and `kind=revive-restart ... fadeMs=1000` (not reachable yet)
- `Level music transition: kind=return-stop track=... fadeMs=1000` (on returnToFrontend)
- Existing: `Level music diagnostic: <reason> (retrying)` printed once per distinct reason.

## 3. Changes (every file touched)
- `port/windows-foundation/features/audio/level_music_v1.hpp`: constants, `level_music_start_due_v1`, `level_music_transition_line_v1` (inline); `<cstdint>` include.
- `port/windows-foundation/features/audio/level_music_v1.cpp`: no change in the end (helpers moved to header). Still contains `read_level_music_names_v1` only.
- `port/windows-foundation/features/audio/level_music_v1_tests.cpp`: include `level_music_v1.hpp`; 11 new gate/constant/log-line checks before the summary line.
- `port/windows-foundation/features/audio/runtime_audio_host_v1.hpp`: `enum class LevelMusicActionV1`; `play_level_music` signature gains `LevelMusicActionV1& action`.
- `port/windows-foundation/features/audio/runtime_audio_host_v1.cpp`: `#include "level_music_v1.hpp"` (line 2); `play_level_music` sets `action` in each branch and uses `kLevelMusicResumeMs`.
- `port/windows-foundation/features/audio/runtime_session_audio_v1.hpp`: `on_local_players_revived`, `on_return_to_menu`, private `level_music_revive_pending_`.
- `port/windows-foundation/features/audio/runtime_session_audio_v1.cpp`: `#include "level_music_v1.hpp"` (line 2); `window_activity` log lines; `set_level_music` clears pending; new `on_local_players_revived` / `on_return_to_menu`; `after_update` start block rewritten (anchor: `// Level::Update-equivalent start`).
- `port/windows-foundation/main.cpp`: ONE hunk, anchor `if(returnToFrontend) {` followed by `pcHudText.clear(renderer);pauseText.clear(renderer);` (around line 3252): adds `if(runtimeAudio) {... on_return_to_menu(audioError) ...}`. Note: `git diff` of main.cpp also shows another worker's faery hunk (`B002/B024` comment near `layout.active_faery_id`). That is NOT mine.
- Not touched: `winmm_output.*`, `CMakeLists.txt`, `port/engine-audio/*`, tracker, RESOLVED-BUGS.
- Untracked scratch only under `.local-inputs/claude-preview13/audio-lifecycle/` (SWF/APK extraction, ninja command extract, syntax script).

## 4. Tests

Commands and real output (PowerShell):
- `powershell -NoProfile -File port/windows-foundation/features/audio/run_level_music_v1_tests.ps1`
  - Asset checks: all PASS for uid 467/468/470 (sounds.xml row, staged size 12278272, manifest entry, VXN open, native states, fresh state 0 playlist, non-silent first block).
  - New transition checks, all PASS: no configured track (no start); unfocused or minimised (no start, retry later); missing source row (no start); focused, row 3, nothing owned (start); same track already owned (no restart); different owned track (switch); after revive-stop owned=-1 (restart due); fades 2000/2/1000/1000/50; start log line exact text; output-pause log names focus state; empty track printed as `none`.
  - Output: `ALL PASS (0)`, then `HASH-PASS` x3 and `HASH ALL PASS` (sha256 980137ac..., d386f78b..., c3530cfe...).
- Syntax, real compile lines from `ninja -C .local-inputs/windows-foundation-build -t commands dh-foundation.exe` with `-fsyntax-only` (no `-o`, no `-MD`), toolchain bin on PATH, script `.local-inputs/claude-preview13/audio-lifecycle/syntax.py`:
  - `runtime_session_audio_v1.cpp` exit 0 (after fixing `RuntimeAudioHostV1::LevelMusicActionV1` qualification; first run had 3 errors).
  - `runtime_audio_host_v1.cpp` exit 0.
  - `level_music_v1.cpp` exit 0.
  - `main.cpp` exit 0.
- Existing standalone runners that compile these audio sources: none found (`grep` of `features/*/run_*.ps1`). Integrated `ctest` is the root's job.

Not run:
- No live-session test of `play_level_music` / `stop` / `on_local_players_revived` / `on_return_to_menu` / output pause (the `DH2_AUDIO_NATIVE_SESSION_FIXTURE` seam was not used). The gate and log formats are tested; the live effects are not.
- No EXE build, no integrated run, no audibility check.

Failure and edge cases covered by the gate tests: unfocused/minimised, missing row, no track, owned track (no restart), owned=-1 after revive-stop (restart due), different owned track (switch).

## 5. Track reachability in the current build

| uid | Track | Reachable? | Notes |
|---|---|---|---|
| 467 | SwampHubAmbientMusic (`m_level_swamp_sfx_swamp.vxn`) | Yes | Starts once after load and focus. Start, resume, focus pause/resume, return-stop covered by logs. Pause menu not implemented; revive not wired. |
| 505 | SwampMerchantCampMusic (`m_safe_zone_01_sfx_swamp.wav`) | Scene reachable, NOT switched | Needs a safe-zone trigger hook and an owner. |
| 468 | SwampWitchCaveAmbientMusic (`m_level_swamp_sfx_witch.vxn`) | No | No level transition. |
| 470 | WaterTempleCaveAmbientMusic (`m_level_swamp_sfx_water.vxn`) | No | No level transition. |
| 464 | TitleMusic (`m_title.vxn`, group 1) | Frontend only, NOT played | Frontend owner outside this group. |
| 541 | SwampWitchCaveComabatMusic (`m_level_swamp.vxn`) | No | Not referenced by the swamp level config. Not checked for staging. |

## 6. Package files required

Checked against `.local-inputs/windows-source-clock-v19-preview-12` (Preview 12 candidate package; the folder named `...-preview-12-candidate` in the brief does not exist, the package folder is `windows-source-clock-v19-preview-12`):

| Path (relative to package) | Needed for | SHA256 (package copy) | Present |
|---|---|---|---|
| `audio-assets/data/sounds/m_level_swamp_sfx_swamp.vxn` | uid 467 (runtime, required now) | 980137ac5b032e5cee50e3f8e6940d31dbd5e0fd0d70346a5167d3c00567e0a8 | Yes (matches source) |
| `audio-assets/data/sounds/sounds.xml` | uid 467/468/470/505/464 rows | 4bc1de654d65470d49e48cee354d5d5cdd45006c18d65420ddc07acf5b400a6e | Yes |
| `audio-assets/data/sounds/m_level_swamp_sfx_witch.vxn` | uid 468 (future) | c3530cfea409d0cd855ffa070a49e9496bcb175dfd4a1c400f8437d6c43a0c0f | Yes |
| `audio-assets/data/sounds/m_level_swamp_sfx_water.vxn` | uid 470 (future) | d386f78bbf031117d2b99e458ca5b2ff88e6252b1b4ea349883b3159587b7172 | Yes |
| `audio-assets/data/sounds/m_safe_zone_01_sfx_swamp.wav` | uid 505 (future safe zone) | 9f2dc2ae2df224b4f40434a468f189b12e4cc22da146912f332a5483afdc75fb | Yes |
| `assets/original-media/m_title.vxn` | uid 464 title music (future frontend owner; note: under `assets/`, not `audio-assets/`) | 6dcd53d955ab1c923ea0bb951ce9c5d21dbf086d64d43314002ebfe2c2cd6f4f | Yes |

No file is missing from the Preview 12 candidate for the current build. The B040 report's packaging note (3 VXN files missing) is resolved in this package. The package `manifest.json` lists 5 matching entries.

## 7. Uncertainties and open risks
- Pause menu: music pause on IGM open is inferred from SWF string order, not decompiled ActionScript. The IGM close resume call is not identified. Needs a per-voice pause primitive (outside scope). The character menu's music behaviour is not checked.
- Return-to-menu fade: the session is torn down in the same iteration, so the 1000 ms fade-out is not rendered. Only a stop request is made. The verifier should confirm no music continues into the frontend.
- Return-to-menu title music: not started. Original plays TitleMusic on main-menu push.
- Revive: the API exists, but nothing calls it, so death -> revive restart cannot be observed in the EXE. Revive restart uses fade 1000 per IDA. The `on_local_players_revived` restart is deferred until focused output (the original's PlayMusic would play while app-paused; app pause already stops output).
- StopAllMusic argument unit: IDA passes `2` as a float to `StopAllEmitters`. Treated as ms (same unit as the other fades). Not verified against the vox engine source.
- Level+288 vs Level+120: the safe-zone id offset was corrected from the B040 report to Level+288 by reading `SetInSafeZoneMusic`. Verify once the safe-zone hook exists.
- Combat/ambient interactive state (SetMusicState on aggro) is not implemented.
- Diagnostic lines are logs only; no audibility proof.

## 8. Verifier script
- Root builds the integrated EXE (after this group). Package with `audio-assets/data/sounds/m_level_swamp_sfx_swamp.vxn` present (see section 6).
- Launch (own folder, own copies of saves, never the user's live game):
  `dh-foundation.exe --startup-config swamp.args` from the package folder. `swamp.args` already contains `--start-mode swamp --level data/scene/001_swamp.mlx` and the audio options used by the verify-audio run (`--audio`, `--audio-assets <pkg>/audio-assets`, `--audio-table <pkg>/audio-assets/data/sounds/sounds.xml` if needed). Confirm with `grep -n "audio" swamp.args` first.
- Expected log, in order:
  1. `Audio initialized ...`
  2. `Level music transition: kind=start track=SwampHubAmbientMusic fadeMs=2000 ordinal=<n>` once, only after a focused, non-minimised frame. Not before.
  3. No `Level music diagnostic` line (absent in a normal run; if present, it is a retry reason).
  4. Minimise the window: `Level music transition: kind=output-pause track=SwampHubAmbientMusic fadeMs=0 focused=... minimized=1`. Restore: `kind=output-resume ... focused=1 minimized=0`. No second `kind=start`.
  5. Pause menu return to main menu route (`Pause menu confirmed main menu`): `Level music transition: kind=return-stop track=SwampHubAmbientMusic fadeMs=1000` before the frontend starts.
- Expected NOT in logs: `revive-restart` (not wired), `kind=switch` in a swamp-only run.
- What to listen for (human): swamp music fades in over ~2 s at load; pauses (not restarts) on minimise; resumes on restore; stops on return to menu. Pause-menu pause and death are expected to FAIL (not implemented).
- Build check: `Audio final dispatched=... startedVoices=...` at shutdown; no crash.
