# TRANSITIONS report (Preview 17): generic level exits, checkpoints, session swap

Branch `p17/transitions` (worktree `DH_wt/p17transitions`, build `DH_wt/build-p17transitions`). Started from `p16/integrate` at 261f2e49. Not pushed. Commits: 7cf5f83f (module, tests), a74a7042 (main session loop), ebd9bc6f (arrival load and item adoption), 17df420d (banner gate, resource logs), cc4f8aa7 (cherry-pick of b7862d6f, character_menu test expectation).

## Result in one paragraph

A level transition now works in the real EXE, in one process, on any fixed map pair that loads through the existing loaders. Swamp's bossroom exit (`_prim_ExitLevelZone_001`, `levelName=DARKWOOD`) is entered, confirmed, the character is saved, the Swamp session is torn down, the Darkwood session loads through the same population/collision/camera/music path, the player arrives at the authored entry point (`entrypointID=0`), walks into Darkwood's exit back to Swamp (`entrypointID=13`), and returns to Swamp with the visited rooms, level row and quest state carried over. A four-session run (Swamp, Darkwood, Swamp, Darkwood) finishes with exit code 0 and bounded process handle and GUI object counts. The real player path is not complete: the exit prompt is not drawn and the confirm is a test aid, the condition evaluator is a stub, and per-level world state (LevelSavegame) is not carried. Witch cave and Swamp 2 exits are built, reported and refused (procedural levels have no EXE path). ctest: 131/132 pass; the one failure is the allowed `session_skill_binding`.

## Investigation (IDA, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)

Visual evidence: no reference frames were re-checked for this stream (time). The fast-travel prompt and the loading screen behaviour are from the IDA calls below; the on-screen prompt art and its button are NOT verified against footage.

Exit zones:
- `TriggerZoneExitLevel::Update` 0039c33c: while the local host touches the zone and `GameObject::MeetCondition` holds, it calls `DisplayFastTravel` (HUD flash) with the LevelList display name, the level name and the entry id, and sets the fast-travel unlock (`Character::SG_SetFastTravelIdUnlocked`, fast-travel id from `fasttravel`). Leaving hides it. The zone itself does not change level.
- Properties (DeclareProperties 0039c8f8): `levelID`, `levelName`, `entrypointID`, `maploc`, `question`, `fasttravel`.
- The travel is `NativeGoToZone(levelName, entryId)` 004422b8 (HUD side): saves all players (`QuickSave` if state 38, `SG_SaveAllPlayer`), reads the slot, then `Application::LoadLevel(file, entry, slot, 1, 1, difficulty, 0, 0, 0)`.
- `Script_ChangeLevel::Execute` 0045cba0 (no prompt): `Level::SG_SaveAllPlayer`, `Character::SG_SetLevelId`, `Character::SG_Save`, then `Application::LoadLevel`. Command fields: `int entry` (+8), `string levelName` (+16) (Structs::ChangeLevel::read 00500114).
- `CheckpointZone::OnCollisionBegins` 00395f04: begin edge by the local player → `Level::CheckpointSave` 003f04b4 (saves checkpoint pose, `SG_SaveCheckpoint`, `LevelSavegame::SaveCheckPoint`). The position-changed check is not replicated.
- `QuestMoveInZone::OnCollisionBegins` 00396120 raises the MoveInZone quest event (already built by `features/quest_runtime/quest_zones_v1`; delegated here).
- LevelList matching: `Level::Level` 003f3128 matches the lowercase level file against the LevelList names; `Script_ChangeLevel` and `NativeGoToZone` compare the name exactly (strcmp). The port uses exact matching.

Level load and unload:
- `Application::LoadLevel` 0032bdc8: if a level is active, `SG_SaveLocalPlayer(1)`, `QuickSave(1)`, `ResetIsLoaded`, then `GSLevel::LoadLevel` (00386818, parameter switch only).
- `GSLevel` state: Ctor 00386190 creates `Level`, pushes `menu_Loading` and calls `onProgress`. Update 00386630: stage 1 to 2 `Level::Load`, stage 3 `Level::Update` until state 38 (running), stage 4 Lua dump. Dtor 003860bc: `Level::Unload`, releases `menu_HUD_0`, deletes the level.
- `Level::Unload` 003f0690: `QuickSave(keep)`, push `menu_Loading`, `SG_SaveAllPlayer`, `VoxSoundManager::StopAllSounds(500)`, `NetworkUnInitLevel`, reseed the game RNG (`Random::s_seed = realtime`). Scope exit of the port's session objects matches this.
- Level music: `Level::Update` 003f82d8 on first update (level flag +324 clear): `VoxSoundManager::PlayMusic(CurrentLevel+284, loop, safe-zone, 2000 ms)` plus the ambient `Play(+292)`. Music id comes from the level's LevelConfig.

Entry point and arrival:
- `SpawnPoint` declarations (DeclareProperties 003ea554): `entrypointID` at +884, `script` (started on placement). `SpawnPoint::PlaceObject` 003ea22c sets position and rotation, then starts the script if set.
- `Level::_LoadPlayer` 003f0018: for each local player, the first SpawnPoint (object-list order) whose entry id equals the level's entry id gets `PlaceObject`, `SetInitialPosition`, and `Character::SG_SetLevelEntryPoint(entry)`. No match: no placement (saved position stays).
- `Level::SG_SavePlayer` 003efa54 saves the level, the entry id (`Level+272`) and the save date on each character.
- Invulnerability on arrival: no such flag or function found in the decoded paths (`vincib` matches only a trophy name). The port applies none; not verified against footage.

Save rows (schema v4): the level row (LevelList index) is stamped on the character before save (`stampSaveMetadata` in the port, equivalent to `SG_SetLevelId` + `SG_SavePlayer` date); difficulty stays in the character save (not separately verified across transitions).

Act rows: the LevelList rows carry no act field; act numbers are inferred from file ids (unverified, as in LEVELS-report).

## Level table facts (verified)

- `data/levels_pyarray.bin` (package assets) decodes to 51 rows (`level_names[i]` with `levels[i].file`). `DARKWOOD`, `DARKWOOD_V2`, `DARKWOOD_V3` and `PORTAL_ROOM` all name `003_darkwood.mlx` (the inventory script printed PORTAL_ROOM only, because it kept the last match). `SWAMP` names `001_swamp.mlx`, `INFECTED_VILLAGE_01` names `005_infectedvillage.mlx`.
- Swamp exits (declarations placed in 001): bossroom `_prim_ExitLevelZone_001` to DARKWOOD entry 0, gated by `activate_cond=IsAfter_Swamp_Escape`; `deadend_brdwalk_w_00` `_prim_ExitLevelZone` to SWAMP_CAVE_WITCH_A entry 0 (procedural); merchant camp `_prim_ExitLevelZone_toSwamp02` to SWAMP_02 entry 0, gated by `IsAfter_Gothicus2Survivors` (procedural). Swamp checkpoints: 4 zones. Quest move: `_prim_WitchQuestStart` (delegated).
- Darkwood builds 11 exits (log `Level exit zone` lines, run1). Fixed targets: SWAMP entry 13 (`_prim_ExitLevelZone_to_swamp`, loads, the return used in the proof), INFECTED_VILLAGE_01 entry 0 (loads), ICY_HUB entry 0 (loads), UNDERWORLD_HUB entry 1 and ABBEY_RUINS_01 entry 0 (loads; both gated), RED_DESERT_HUB entry 0 (loads; gated by `IsAfter_EDKillTroll`). Procedural: ROOT_DEN_01, EARTH_TEMPLE_01, GOTHICUS_CRYPT_01, GOTHICUS_CRYPT_02, GOTHICUS_CASTLE_02. The fixed targets are candidates for the next proof hop (not run here).
- Entry 13 exists in the Swamp bossroom module (`_prim_EntryPoint_from_darkwood02`, one candidate). Darkwood entry 0 has 3 candidates; the first in placement order is `_prim_EntryPoint_from_swamp02` (3661.49,-23455.9,0). IDA takes the first match in its object list; the port takes the first in declaration (placement) order. Equivalence unverified.

## Implementation

1. `port/windows-foundation/features/transitions/level_transitions_v1.{hpp,cpp}` (new, portable, no platform code):
   - `ZoneClassRegistry`: gametype to builder. Registered: `TriggerZoneExitLevel` (level_exit), `CheckpointZone` (checkpoint), `QuestMoveInZone` (delegated to quest_zones_v1). Unknown gametypes are ignored.
   - `level_rows()` from `dh2::data::LevelTables`; `find_level()` exact name; `level_uri()` = `data/scene/<file>`.
   - `ExitZoneSet::build()`: oriented Block boxes (campaign_host rule, `oriented_zone_box`); targets resolved through the level table; `target_procedural` flag for `.rule.xml`; unresolved or non-Block zones are kept in `notices()` with the reason.
   - `ExitZoneSet::update()`: begin edges. The first update only records state (arrival inside a zone does not fire a checkpoint). `level_exit.inside` stays true while inside (prompt semantics).
   - `resolve_entry_spawn()`: first SpawnPoint with `entrypointID`, position from the placement translation, heading from the X axis.
   - `ConditionSource` interface with `LoggedConditionStub` (true, logged once per name). CONDITIONS replaces the stub.
2. `port/windows-foundation/main.cpp` (P17 TRANSITIONS anchors, additive):
   - Session loop: the existing outer `for(;;)` (boot, frontend, level block) now also carries transitions. A confirmed transition breaks the frame loop, runs the existing cleanup (audio shutdown, texture release), saves the character (`stampSaveMetadata` for the target level, then `save_character`), sets `options.level`, `transitionEntryId`, `startMode=swamp`, and `continue`s. Level objects leave scope (the session "unload"). The next session loads the target through the same population, collision, camera, music and loading paths.
   - Arrival: `Transition character loaded` (explicit `load_character`, since non-fresh starts do not load otherwise), population built, then `resolve_entry_spawn` sets the start position and facing (motor `setPosition(pos, heading)`).
   - Starting items: the diagnostic startup-item block pushed the actor's starting equipment on every non-frontend session, which duplicated the instance on a continuing character and failed the save validation (`duplicate inventory instance: equipped/StartingSuit`). It now adopts owned instances. This also affects non-fresh loads.
   - Quest banner: no NEW QUEST banner on transition arrival (the quest was announced in the previous session; the persisted announcement flag is a gap).
   - Frame counter `drawn` continues across sessions (`--frames` is a total).
   - Loading screen shown for transition sessions (the existing `LoadingScreenV1`).
   - Test aids (labelled, not gameplay): `--player-to ZONE@FRAME` (places the player at the zone centre through the motor reset), `--transition-confirm FRAME` (confirm press for an armed exit), `--capture-at FRAME:PATH`.
   - Logs: `Level exit zone`, `Exit zone entered`, `Transition confirmed|refused|leave|character saved|character loaded|arrival`, `Checkpoint zone ... saved`, `Session start|end resources handles= gdi= user=`.
3. `port/windows-foundation/CMakeLists.txt`: `# P17 TRANSITIONS` block (library source in foundation_data, test `level_transitions`).

## Isolated tests

- `level_transitions` (ctest, PASS): registry bindings; builders for Block, non-Block, unknown level, procedural target, checkpoint, delegated quest; edge behaviour (arrival, staying, leaving and re-entry, dead player, checkpoint begin); spawn resolution (first match, candidates, absent entry); condition stub logs once; real data (Swamp: 3 built exits, 4 checkpoints; Darkwood: 11 exits incl. return to SWAMP entry 13; entry 0 and entry 13 resolve).
- Full ctest (`build-p17transitions`, `-j 6`, PATH with the toolchain): 131/132 pass. Failing: `session_skill_binding` (expected). Note: `character_menu` failed at the branch point because its expectation predates b7862d6f; cherry-picked as cc4f8aa7 (two-line test change). `schema_v4` failed once under `-j 6` and passed on rerun (not reproduced).

## Integrated runtime verification (real EXE, quiet hidden runs, `tools/quiet_run.ps1`, `DH_AUDIO_SILENT`)

Overlay: `.local-inputs/claude-preview16/levels-root` (package assets + Android data for 003 and the Darkwood/Witch modules). Args: `.local-inputs/claude-preview17/transitions/run{1..4}/`. Profile save in each run directory (`character.save`, not the package).

Run 1 (`run1/`, 170 frames, exit 0):
- Frames 0-60, Swamp: `Map room visited frame=55 ... total=2`. `Player-to zone=_prim_ExitLevelZone_001` (frame 40), `Exit zone entered=... target=DARKWOOD` (frame 41), `Transition condition stub: 'IsAfter_Swamp_Escape' treated as active`, `Transition confirmed ... entry=0` (frame 60). Frames 0 and 35 captured.
- Session end and save at frame 61: `Transition character saved level=data/scene/003_darkwood.mlx visited_records=2`. Character reloaded in the new session (`visited_records=2`).
- Frames 61-130, Darkwood: `Transition arrival entry=0 spawn=_prim_EntryPoint_from_swamp02 candidates=3 position=3661.49,-23455.9,0 heading=-2.97`. Music start `DarkWoodHubAmbientMusic` (run2 log). Frame 75 captured (arrival view, HUD visible).
- Frames 100-130: `Player-to zone=_prim_ExitLevelZone_to_swamp`, `Transition confirmed ... target=SWAMP entry=13` (frame 130). Frame 118 captured.
- Frames 131-170, Swamp: `Transition arrival entry=13 spawn=_prim_EntryPoint_from_darkwood02 position=-19363.9,11187.8,-109.7 heading=-1.57`. `visited_records=3` loaded. `Map room zones level=001 visited=2` (both Swamp rooms restored from the save), `Quest banner suppressed kind=NEW QUEST row=50 (transition arrival)`. `Actor final position=-19363.9,11187.8,-113.97 grounded=1`. Frame 150 captured.
- Captures (`run1/*.png`): `s1-swamp-before-f35`, `s1-swamp-armed-f58` (New Quest banner from the quest runtime, no prompt drawn), `s2-darkwood-arrival-f75`, `s2-darkwood-armed-f118`, `s3-swamp-return-f150` (Swamp bossroom, HUD visible).

Run 2 (`run2/`, 230 frames, `--audio`, exit 0): Swamp, Darkwood, Swamp, Darkwood in one process.
- Music per session: `Level music transition: kind=start track=SwampHubAmbientMusic fadeMs=2000` (sessions 1 and 3), `DarkWoodHubAmbientMusic` (sessions 2 and 4). Audible output not verified (silent run).
- Resource counts (handles / GDI / USER) at session boundaries: start S1 92/0/1; end S1 565/46/55; start S2 566/46/55; end S2 570/49/55; start S3 570/49/55; end S3 570/47/54; start S4 570/47/54. Handles flat from the second transition on; GDI and USER move both ways (no monotonic growth). Working set not measured.

Run 3 (`run3/`, negative and checkpoint, exit 0):
- `Checkpoint zone=_prim_CheckpointZone_01 level=data/scene/001_swamp.mlx saved frame=21` (begin edge on contact).
- `Exit zone entered=_prim_ExitLevelZone target=SWAMP_CAVE_WITCH_A entry=0`, then `Transition refused zone=_prim_ExitLevelZone target=SWAMP_CAVE_WITCH_A file=data/scene/002_swamp_witchcave.rule.xml reason=procedural level has no EXE path yet` (frame 40). The session continues to frame 60.

Run 4 (`run4/`, `--loading-capture`, exit 0): transition loading frame `load-tr-080.png` shows the existing `LoadingScreenV1` (LOADING heading, original tip text "Faeries are the source of magic in Gothicus...", red progress bar). The frame is fill-drawn, not the recovered ornate art.

## Gaps (explicit)

1. Exit prompt: not drawn. The original shows the fast-travel prompt through the HUD flash (`DisplayFastTravel`); its art and confirm button are not recovered or verified. The port's confirm is the `--transition-confirm` test aid only. A real player cannot transition yet (no key or button is bound to the confirm). Highest-priority gap.
2. Conditions: `LoggedConditionStub` (true, logged). `IsAfter_Swamp_Escape` (the bossroom exit) and `IsAfter_Gothicus2Survivors` (Swamp 2) are not evaluated.
3. Per-level world state is not carried. The original saves each level (`LevelSavegame`, `Level::QuickSave`) so returning to Swamp keeps killed monsters, opened containers and similar. The port re-creates the Swamp world on return; only the character is saved (GameSave is roster-locked to one level and cannot carry a transition). Needs a per-level world save; not done.
4. Entry point persistence: IDA stores `entrypointID` on the character (`SG_SetLevelEntryPoint`). `CharacterState` has no entry field; a process restart from a transition save starts at the default start. Needs a save-schema field (not changed here).
5. Loading screen art: `LoadingScreenV1` draws fills. The real art is staged as `assets/front-compat/loadanims_droid.swf` but not exported (same route as the character menu art). Shown for transitions, placeholder frame.
6. Arrival invulnerability and HUD flow: no invulnerability found in IDA; the port shows the HUD at the first arrival frame. The original's `menu_Loading` / `menu_HUD_0` sequence is not replicated beyond the loading screen. Not verified against footage.
7. Facing: the arrival heading is passed to the motor; the on-screen facing was not compared with footage.
8. Checkpoint: saves the profile on begin edge. The original's position-change check, the checkpoint file and the `LevelSavegame` checkpoint are not replicated.
9. Procedural targets (`.rule.xml`): refused at run time (Swamp witch cave, Swamp 2, troll cave, Darkwood-side Root Den, Earth Temple, Gothicus Crypt, Castle 2, etc.). The canonical port has the generator (LEVEL-COVERAGE gap 7); not wired here.
10. Rotated exit zones: built with the oriented box; the Darkwood `SWAMP` exit's rotation convention is not verified (campaign rule reused).
11. QuestMoveInZone: delegated to `quest_zones_v1` (rotated quest zones still not built there).
12. Quest banner: suppressed on transition arrival, not persisted. The original shows NEW QUEST on accept only; the announcement state is not in the save.
13. Act rows: not verified (LevelList has no act field).
14. Difficulty across a transition: carried by the character save only; not separately verified.
15. RNG: the arriving session starts from the launch seed (`diagnosticRngSeed`). IDA reseeds on unload (`Random::s_seed = realtime`); the port's seed is deterministic on purpose (proof reproducibility).
16. Audio: music changes are verified in logs only (silent quiet runs). Unload stops via the existing `runtimeAudio->shutdown`, not the 500 ms `StopAllSounds` fade.
17. Memory: handle and GUI object counts are bounded across four sessions; working set is not measured; GDI count is not monotonic. Not a full leak test.
18. Branch base: `p16/integrate` advanced 11 commits after 261f2e49 (a 287-line rewrite of `main.cpp` among them). This branch is not merged with it; the merge needs the integration owner (hunks in the frontend, quest banner, equipment and frame-loop areas).
19. Arrival entry choice: the port takes the first SpawnPoint in placement order; IDA takes the first in object-list order. Equivalence not verified (Darkwood entry 0 has 3 candidates).
20. Swamp-specific data: none in code. The Swamp and Darkwood names appear only in tests and as the existing `startMode=swamp` route name.

## Placeholders (labelled in code and above)

- Exit prompt (not drawn; `(prompt art not drawn)` in logs).
- Confirm input (`--transition-confirm` test aid).
- Condition evaluator (`LoggedConditionStub`, logs `treated as active`).
- Loading frame (fill-drawn `LoadingScreenV1`).

## Package files required

- Package assets: `.local-inputs/windows-source-clock-v19-preview-15-rc3/assets` (level table `data/levels_pyarray.bin`, `front-compat/loadanims_droid.swf` not yet used).
- Level data for the proof: Android copies in `.local-inputs/assets-extra/android/data/scene/{001_swamp,003_darkwood}.mlx` and their modules; overlay `.local-inputs/claude-preview16/levels-root` (existing, read-only).
- Nothing new added to the package.

## Verifier script

1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p17transitions -Jobs 6`.
2. ctest with the toolchain on PATH (`llvm-mingw-20261006-ucrt-x86_64/bin`): expect 131/132, only `session_skill_binding` failing. `level_transitions` must pass.
3. Quiet runs (`tools/quiet_run.ps1 -JobsFile <job> -Parallel 1`): `.local-inputs/claude-preview17/transitions/run1/jobs.json` (Swamp to Darkwood to Swamp), `run2/jobs.json` (four sessions, `--audio`), `run3/negative.args` (checkpoint and refusal), `run4/loading.args` (loading capture). Expected: exit 0 for each; the `Transition ...` lines listed above; `Map room zones level=data/scene/001_swamp.mlx ... visited=2` in run1 after the return.
4. Look at `run1/s*.png` and `run4/load-tr-080.png`.

## Notes for the integration owner

- The exit zones, checkpoint zones and the transition session are in `features/transitions`; `main.cpp` hunks all carry `P17 TRANSITIONS`.
- The levels coverage report (`LEVEL-COVERAGE.md`) lists TriggerZoneExitLevel as "ported_elsewhere" in `features/levels/level_class_registry.cpp`; it now has a live builder in `features/transitions`. The registry text was not updated.
