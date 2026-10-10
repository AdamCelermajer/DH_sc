# LEVELUP report (I026: level-up feedback missing)

Branch p15/levelup, worktree DH_wt/levelup, build DH_wt/build-levelup.
Scratch, jobs and captures: `.local-inputs/claude-preview15/levelup/` (dump_fx.py, dump_dict.py, check_fx_files.py, patch_xp.py, mkjob.sh, jobs/, ref/, fixtures/).

Status in one line: the effect data is identified and the live path is wired and logs each step. The on-hero FX does NOT render, because `data/3D/interface/level_up.bdae` is not in any local package. The HUD text and the sound cue are not drawn or played (reasons below).

## 1. Investigation

### 1a. Logic (IDA, libDungeonHunter2.so pseudocode-all.c)
- `Character::_GiveXP` 0x3bf498: adds modified XP (`PROPS_Add 33`). When XP >= XP-to-next (prop 34) it calls `Character::LevelUp(this, carry)`, then clamps XP to prop 34. Gates: max level (`MaxLevelBNormal/CHard/DVeryHard`), not online, not in a level flag.
- `Character::LevelUp` 0x3beb88 (pseudocode line ~138700), only when level < cap:
  1. Level +1 (prop 19), XP = 0 (prop 33), `UpdateBaseProperties`, `RegenHP(-1)`, `RegenMP(-1)`, `SG_SetPlayerLevel`, `SG_Save`.
  2. StrID `MENU_LEVEL_UP` is pushed to the `StatusMsg` deque. If it is the first queued message, the HUD root AS callback `StatusMsg::s_StartFuncName[0]` is invoked (this is the HUD text).
  3. ScriptManager `PlayerLevelUp` is started (id -1 if absent).
  4. `VisualFXManager::PlayAnimFXSet(&Singleton<VisualFXManager>, 135, this, 0)` runs on the hero. Its result is not checked.
  5. Only when the new level is 2: non-online, normal difficulty, tutorial flag +46 starts `cinematic_Tuto_levelUp`, clears flag +46 and starts updateJob thread4. Also `cinematic_Tuto_menuCharacterSheet` (flag +41) and `cinematic_Tuto_menuSkillSheet` (flag +44).
  6. Only when the new level is 12: character-menu `IsSpecTime` callback. For the local player: trophies `epic_lvl10..epic_lvl100`.
- Sound: source sound "LevelUp" (sound id 127, label LevelUp, bank 6, group 4, priority 2, file `sfx_level_up.wav`; the package has that file). Its caller is NOT in `Character::LevelUp`, and I did not locate it. It is probably in the PlayerLevelUp script, which is binary pyscript data. Not wired.
- The Android port already has the same sequence in `android-native/app/src/main/cpp/source_campaign_death_rewards_v84.cpp` (`level_up_suffix`: PlayerLevelUp, play_set(0x87 = 135, zero, nullptr, identity), tutorials). The Windows runtime had none of it.

### 1b. Effect data (decoded from `windows-source-clock-v19-preview-14-rc1/assets/data/effects_*.bin`)
- Set 135 is named `level_up`: force_cache 0, loop 0, type 0, one step. The step has file index 63, play_time -1, loop 0, speed 1.0, no subobject.
- Dictionary index 63 `level_up` maps to `data/3D/interface/level_up.bdae`.
- That .bdae is NOT in the Preview 14 package (9 of 284 dictionary FX files exist there), NOT in the APK, and NOT anywhere in .local-inputs or the workspace (searched).
- The device cache list `C:/Users/adamc/Desktop/dh2_video_research/cache_list.txt` (line 29292) lists `com.gameloft.android.GAND.GloftD2SS/files/data/3d/interface/level_up.bdae` at 29292 bytes. That copy exists only on the device or in the original install.

### 1c. Reference video (Part 1, 720p; frames in `.local-inputs/claude-preview15/levelup/ref/`)
- t ~761.5 s: "QUEST COMPLETED - Kill 8 Bog Moths", reward "20 EXP / 150 GOLD", with a faint "LEVEL UP" line under the gold line in the quest panel (observed).
- t ~766-768 s: gold/green ring halo around the hero (10 fps, `dense_766_774.jpg`).
- t ~769.0-769.5 s (observed at 10 fps, `burst_768_8_770.jpg`): a white-blue burst at hero height at 769.0. Then a tall white light column with bloom at the hero's feet, from about 769.2 to 769.4. It fades by 769.5, and a ring/halo remains. This is direct observation of the on-hero effect. Its exact shape beyond these frames is not known.
- t ~770-774 s (2 fps, `status_770_774.jpg`): floating green "+N EXP" and gold pickup text. No "LEVEL UP!" text was seen on the HUD in any sampled frame (10 fps 768.8-770, 2 fps 770.5-774.5).
- t ~779-781 s: character sheet shows "Warrior Lvl 3" (observed). The pre-level number is not visible in the play HUD.
- Uncertain: whether the video's level-up is 1->2 or 2->3. The play HUD does not show the level.

### 1d. Text
- The string "LEVEL UP!" is in `original-cache/data/text/global.english`, next to "empty" in the string sequence. The string itself is verified; the MENU_LEVEL_UP StrID-to-index mapping is not.
- The Windows runtime has no StatusMsg or quest-banner owner (no "Quest Completed" banner code in the port). The reference shows no HUD "LEVEL UP!" at the level-up moment. So text is LOGGED ONLY (`drawn=0`). It is not drawn and not routed through combat text, because the reference's floating text at that moment is XP, not a level-up label.

## 2. Changes (commit 8e54497b on p15/levelup)
- NEW `features/effects/runtime_level_up_presentation_v1.{hpp,cpp}`: `play_level_up_presentation_v1(tables, play_seam, player, result, error)`. It validates that set 135 is named `level_up`, then calls the FX owner `play_set(135, ZERO, nullptr, player identity, nullptr)`. A failed play is logged (`fx=...failed(reason)`) and does NOT fail the award, because the source ignores the PlayAnimFXSet result. Constants cite the IDA addresses.
- NEW `features/effects/runtime_level_up_presentation_v1_tests.cpp`, with CMake target `runtime_level_up_presentation_v1_tests` and ctest `runtime_level_up_presentation_v1`. Its argument is the Preview 14 package `assets/data` directory.
- `features/loot/runtime_death_rewards_v1.{hpp,cpp}`: new `RuntimeDeathRewardServicesV1::on_level_up` callback. It fires in `award_player_xp` only after a real level gain (`stats.level > source_level`). The cap branch returns before it.
- `features/loot/runtime_session_death_rewards_v1.{hpp,cpp}`: new `RuntimeSessionDeathRewardBindingsV1::level_up_presentation` (std::function) and `level_up_thunk` forwarding. When unset there is no presentation, so headless tests are unaffected.
- `main.cpp`: include, plus a `rewardBindings.level_up_presentation` lambda. It uses `sourceEffectsTables.borrow()` and `sourceEffectsFactory->manager().play_set`, and prints the log line with the frame counter `drawn`.
- `CMakeLists.txt`: the presentation .cpp is added to `foundation_runtime_effects`, plus the new test target.

## 3. Tests
- `build-levelup/runtime_level_up_presentation_v1_tests.exe <package assets/data>` prints `passed`. It covers: the success call shape (set 135, ZERO position, NULL rotation, anchor = player, NULL created identity, log text); the FX-failure path, which does not fail the award; and invalid player, missing seam and missing tables, which all fail.
- Build `p14_build.ps1 -Name levelup` is green (exit 0).
- Full ctest (`ctest --test-dir build-levelup -j 6`, log `.local-inputs/claude-preview15/levelup/ctest2.log`): 111 of 112 passed, including `runtime_level_up_presentation_v1` (Passed). The only failure is `session_skill_binding`, the known worktree junction failure that the brief says to ignore.
- Not unit-tested: the cap branch and the loot-side callback ordering. These are covered only by the live run (section 4).

## 4. Live path (quiet batches, `jobs/batch1.json`, Parallel 9, all exitCode 0)
- Fixture `fixtures/character-xp-high.save` is the package character.save with experience set to 7,000,000, a value above the threshold so the first lizard kill crosses it. 900,000,000 overflowed the signed Q8 range and failed with "Profile progression exceeds source signed Q8 range".
- Kill sequence: the args of `claude-preview14/drops/b-a/s19f260` (space-key attacks, lizards and moths, fixed step .016, seed).
- The level-up fires at frame 143 (fixed-step timeline, deterministic). Exact log line from `jobs/b144/run.log`:
  `Level up presentation frame=143 actor=18446744073709551615 level=2 fx=135:level_up:failed(Exact authored FX resource not found (basename fallback disabled): data/3D/interface/level_up.bdae) text=LEVEL UP! drawn=0 (no HUD status-message owner)`
  - actor 18446744073709551615 is the normal in-session player ObjectId. The swing FX logs use the same value. invalid_object_id is 0.
  - level=2 is the reached level (1 -> 2 in this fixture; the save's stored level field is overwritten at load).
- Control `ctrl` (unmodified character.save, XP 18, same 260 frames): 0 presentation lines.
- Captures I LOOKED AT (`verify-sheet1.jpg`: frames 140, 143, 144, 146, 150, 160 in a 3x2 tile). Combat with Bogwomp continues, with "Miss" and "17" numbers, and a blue swoosh ring on the hero (combat effect, present before and after). There is no light column, no level-up ring on the hero, and no "LEVEL UP!" text. This is the expected result given the missing asset. It is NOT the reference effect.

## 5. Not verified / gaps
- On-hero FX visually: NOT verified. The FX path is correct to the data, but the asset is missing (section 6). Once `level_up.bdae` is provided at `assets/data/3d/interface/level_up.bdae` (or the exact URI the asset catalog expects), rerun batch1 and check b144 and b146 for the column.
- HUD "LEVEL UP!" text: not drawn. There is no status-message owner, and the reference shows no text at that moment in the 10 fps frames 768.8-774.5. A StatusMsg queue owner is needed only if the text turns out to appear later in the reference.
- Sound "LevelUp" (sfx id 127 -> sfx_level_up.wav): caller not found, not wired. The reference audio was not analysed.
- Level-2 tutorial cinematics (`cinematic_Tuto_levelUp`, `cinematic_Tuto_menuCharacterSheet`, `menuSkillSheet`): not implemented. The Windows runtime has no cinematic runner. Relevant only when the reached level is 2.
- Character-menu IsSpecTime (level 12) and trophies: not implemented.
- Possible version difference: the reference asset source is the Android device cache. The PC package may differ.

## 6. Package files required
- `assets/data/3d/interface/level_up.bdae` (the original 29292-byte file from `com.gameloft.android.GAND.GloftD2SS/files/data/3d/interface/`). It is not present in any local package. Without it, `fx=...failed(...)` is logged and nothing is drawn.
- `assets/data/sounds/sfx_level_up.wav` is already in the audio packages (not wired yet).

## 7. Verifier script
1. Build the EXE: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name levelup`.
2. Quiet batch: `quiet_run.ps1 -JobsFile .local-inputs/claude-preview15/levelup/jobs/batch1.json -Parallel 9 -Summary ...` (jobs b130, b140, b143, b144, b146, b150, b160, b200, ctrl; generated by `mkjob.sh`).
3. Expect: exitCode 0 for all jobs. `Level up presentation frame=143 ... fx=135:level_up:...` in the b144 to b200 logs. None in b130, b140 or b143 (captured before the event), and none in ctrl.
4. Expect, while the asset is missing: `fx=135:level_up:failed(...level_up.bdae...)`. Once the asset is present: `fx=135:level_up:played`, and a light column visible in the b144 and b146 captures.
5. Unit: `runtime_level_up_presentation_v1_tests.exe <.local-inputs/windows-source-clock-v19-preview-14-rc1/assets/data>` prints `passed`.

## 8. Open risks
- The fixture sets XP to 7,000,000. The source clamps XP to the threshold after one level, so this is a one-level test only.
- The FX owner's behaviour with a real asset is unverified. Only the missing-asset path ran.
- The order of the presentation relative to the loot drop and the XP share is not checked against the source by a test.
