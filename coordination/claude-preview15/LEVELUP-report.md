# LEVELUP report (I026: level-up presentation missing)

Status: IN PROGRESS (investigation). Branch p15/levelup, worktree DH_wt/levelup.

## Evidence so far

### IDA (libDungeonHunter2.so pseudocode-all.c)
- Character::LevelUp @ 0x3beb88 (pseudocode line ~138699):
  - Cap = MaxLevelBNormal / MaxLevelCHard / MaxLevelDVeryHard by SG_GetGameDifficultyUnlocked.
  - If level < cap: PROPS_AddInt(19,1); PROPS_SetInt(33,0); UpdateBaseProperties; RegenHP(-1); RegenMP(-1); SG_SetPlayerLevel; SG_Save.
  - Then (after the vtable+40 gate): StringManager MENU_LEVEL_UP pushed to StatusMsg deque (`std::deque<StatusMsg>::push_back`), shown through the HUD root callback (StatusMsg::s_StartFuncName, DebugCachedCharacter) when HUD root exists.
  - ScriptManager "PlayerLevelUp" script started (id lookup, -1 if absent).
  - **VisualFXManager::PlayAnimFXSet(&VisualFXManager, 135, this, 0)**: the on-hero FX set 135.
  - Level 2 gate (`v11==2`): if not online and normal difficulty and tutorial flag +46: starts script "cinematic_Tuto_levelUp", clears flag +46, starts updateJob thread4. Also "cinematic_Tuto_menuCharacterSheet" (flag +41) and "cinematic_Tuto_menuSkillSheet" (flag +44).
  - Level 12 gate: MenuManager character menu callback IsSpecTime.
  - Local player: trophies epic_lvl10..epic_lvl100 at 10,20,...,100.
- Character::_GiveXP @ 0x3bf498: XP add; if XP >= XP-to-next (prop 34), calls LevelUp(carry), clamps XP.

### Effects data (Preview 14 package assets/data/effects_*.bin, decoded by .local-inputs/claude-preview15/levelup/dump_fx.py)
- Set 135 name = `level_up`, loop 0, type 0, one step: file index 63, play_time -1, loop 0, speed 1.0 (0x3f800000), no subobject.
- Dictionary index 63 `level_up` -> `data/3D/interface/level_up.bdae`.
- The .bdae is NOT in the Preview 14 package (only 9 of 284 dictionary FX files are present in that package).
- Original file is listed in the research cache list (`C:/Users/adamc/Desktop/dh2_video_research/cache_list.txt`, line 29292): `com.gameloft.android.GAND.GloftD2SS/files/data/3d/interface/level_up.bdae`, size 29292 bytes. Not extracted locally (the APK's assets/data.save does not contain it).
- Sound: `sfx_level_up.wav` exists in package audio-assets (`data/sounds/sfx_level_up.wav`). Not yet confirmed which IDA sound call plays it.

### Reference video (Part 1, 720p, 10 fps frames)
- t~761-762 s: "QUEST COMPLETED" banner (1 fps sheet).
- t~766-771 s (10 fps sheet, `ref/dense_766_774.jpg`): gold/green ring halo around the hero (~768 s), then a bright white light column with rings on the hero (~769-771 s), then the bright flash fades; HUD name label shows the new level number.
- t~779-781 s: stats page "Warrior Lvl 3" shown.
- Observation only; the banner text itself (LEVEL UP) not yet located frame by frame.

## Open items
- Find the level-up HUD text path (StatusMsg / MENU_LEVEL_UP string, combat_text owner).
- Find the sound call in IDA (grep sfx_level_up / PlayAnimFXSet 135 owner).
- Check whether level_up.bdae can be sourced for the package (it is not local).
