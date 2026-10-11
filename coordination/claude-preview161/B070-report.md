# B070 Movement Tutorial appears again (fix/b070, build fix070)

## Evidence
- Screenshot: `.local-inputs/claude-preview161/user-shots/b070-movement-tutorial-shown-again.png` (the "Movement Tutorial" title box, after the tutorial was already seen).
- Video (z_Zky7qQdYs): prior survey `coordination/claude-preview15/CINE-survey.md` row 96 records the Movement Tutorial once, at local t~180-190 s, after the opening. The video is a single uninterrupted playthrough, so it shows "appears once" and cannot show non-repetition across a relaunch. I did not re-extract frames this session (unknown; nothing contradicts one-shot).
- IDA (libDungeonHunter2.so pseudocode-all.c):
  - `Script_DoTutorial::Execute` 0x460950: runs only if the local player exists, `SG_GetGameDifficulty()==0`, `SavegameManager[+41+index]!=0`, and not online; it starts the named script, writes `byte[+41+index]=0` and calls `SavegameManager::saveSettings` (or queues the update job thread when the settings menu is not open).
  - `Script_LockTutorial::Execute` 0x45925c: writes `byte[+41+index]=0` and saves settings the same way.
  - Storage: 14 bytes at SavegameManager+0x29, `__loadTutorials` 0x46c778 / `__saveTutorials` 0x46c798, called from `loadSettings` / `saveSettings`; file `dh2_settings.savegame` = the GLOBAL settings file, not a character save. `_initSettings` sets all 14 to 1 (enabled). Nothing resets them on New Game (only these writes to +41 found).
- Cause in our runner: `CampaignHost` kept `consumed_tutorials_` in memory only (stubs "tutorial flag persistence (session only)" / "settings save (nothing written)"). Movement_Tuto's DoTutorial (kind 78) runs at level start, so every relaunch/reload replayed it.

## Change
- `features/campaign_host/campaign_host.{hpp,cpp}`: `CampaignHostServices::tutorial_settings_path`; consumed set loaded lazily on the first gate check and written (tmp + rename) on consume / settings save. File `dh2_settings.tutorials` = `DH2TUT1\n` + 14 bytes (1 enabled, 0 consumed); missing or malformed file = defaults. Empty path keeps the old session-only stub (tests).
- `main.cpp` (one line after `CampaignHostServices hostServices;`): path = directory of `--save` / `dh2_settings.tutorials`. Platform neutral (std::filesystem).
- Test `tutorial_is_one_shot_across_sessions` in `campaign_host_tests.cpp`: first session plays, second does not, deleted file = plays again, garbage file = plays again.
- Panel layout untouched (B068).

## Verification
- ctest: 139/140, only the allowed `session_skill_binding` fails; `campaign_host` passes.
- Real EXE, quiet_run (scratch `.local-inputs/claude-preview161/fix070/`): run1 fresh save, full opening: `DoTutorial frame=3935`, caption "Movement Tutorial" frame=4249, `LockTutorial frame=4617`, file `dh2_settings.tutorials` created. run2 same save: `DoTutorial frame=2`, `tutorial settings loaded: 1 consumed`, no Movement Tutorial caption. Before (Preview 16 EXE, same save): "Movement Tutorial" caption at frame 313 on reload plus the stub notes.
- Frames were not viewed (log evidence only; caption drawing is B068's).

## Remaining / unknowns
- New game: the original does not reset these bytes (global settings), so a new character with the same settings file does not see the tutorial again. A per-character reset would be a deliberate deviation; not done.
- Save/load: the flag is independent of GameSave files, so loading an old save does not bring it back (matches the original).
- Linux/Android hosts must bind `tutorial_settings_path` to their settings directory; Android native code was not touched.

## Package files required
None (`dh2_settings.tutorials` is created at runtime next to the profile save).
