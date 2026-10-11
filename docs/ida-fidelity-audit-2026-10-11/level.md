# level batch fidelity audit (IDA -> Windows reconstruction)

## Progress
- last_address_done: 0x4115d8 (CameraTarget::HandleGhostCam). Deep reads done: PlayerManager death/revive (0x375af0, 0x375eb4, 0x375fe8, 0x376100, 0x37565c, 0x370bf4, 0x370c9c), PlayerManager::_ManageCharacters level-up branch (0x37280c), Level::_LoadPlayer/LoadCheckpoint/CheckpointSave/QuickSave/Update/UpdateCameraZoom/_LoadCamera, GSLevel::Update/LoadLevel, GSEndGame::Update, CameraLevel::Load/Update/HandleZoom/HandleCentering/CalculateDefaultTargetDistance, CameraTarget::HandleDamping/HandleOffset/HandleTransition/SetTarget/ctor, CameraBase::SetData, PlayerSavegame::SG_Save/SG_SaveCheckpoint/SG_LoadCheckpoint/__SaveProperties/_SetupSavedSections, LevelSavegame::ValidateCheckpoint.
- groups_done: camera chain, death/revive, checkpoint save/load, spawn placement, save sections, level-up hook.
- next (not done; a resumed agent should continue here): Level::_LoadProcess (0x3f6990), Level::LoadFile, PlayerManager::_AddCharacter/PostInitCharacters/InitialSetup/RemoveCharacter, PlayerInfo ctor/Reset, LevelSavegame::__SaveObjects/__LoadObjects (what object state persists), PlayerSavegame fast-travel/level-state accessors (SG_SetFastTravelIdUnlocked etc.), MenuWorldMap/MenuCharMenu_Map, Module::_ChooseXmls, SceneManager (render-only, expected n/a).

## Summary
Scope note: the batch is 611 functions; this pass covered the player-visible gameplay core in depth and classified the rest by group. Rendering (SceneManager::*), multiplayer/net (PlayerManager online paths, PlayerInfo net structs) and debug paths are n/a.

Counts of audited items (findings plus mapping rows): faithful 6 (gameplay camera Load/Update/damping/transition/SetData/znear-zfar), divergent 2, partial 5, missing 7, unclear 4, n/a groups 6, intentional-adaptation 1.

Top risks:
1. Natural player death -> fade -> checkpoint reload -> revive at last checkpoint with at least 3 potions is NOT wired in the Windows exe (known: I024). A dead player cannot continue.
2. Level transitions, world map, fast travel and spawn-point-by-entry-point selection do not exist in the Windows exe (fixed start coordinate; I021 known). FTVL, LVLS, LEPT, LUSP save sections are not persisted by the Windows SaveStore.
3. Automatic checkpoint (Level::CheckpointSave from checkpoint zones, stores revive position and world objects) is absent; only manual F5/F9 snapshot exists, with a different data model.
4. Level-up presentation: original starts script "PlayerLevelUp" and plays FX set 135 when level rises by exactly 1; port plays FX only, "LEVEL UP!" text is not drawn and the script is not started.
5. Camera zoom: applied zoom is locked at 0 (authored default); wheel/pinch zoom and Level::UpdateCameraZoom auto-zoom are not wired. Default framing matches.

## Findings

### F1. Natural death / revive / checkpoint continuation not wired (high, known I024)
- IDA: PlayerManager::_CheckLocalDeaths 0x376100 (level type 38, local char dead) -> _HandleLocalDeaths 0x375fe8: offline pushes menu_FadeToBlackScreen and mutes FX volume; the HUD SWF later calls NativeReviveAllPlayers -> PlayerManager::ReviveLocalPlayers 0x37565c. Offline it calls Level::LoadCheckpoint (0x3f0428: LevelSavegame::LoadCheckPoint, GameEventManager::ReInit, Character::SG_LoadCheckpoint(16 = QEST section only), SG_Update(1)); restores FX volume; per local player: PROPS_RemoveAllBuffs; SetPosition to the stored revive position (character +5236..5244, only if not origin); Character::Revive(NULL,1); SM_SetIdleState; if potions < CharacterDesign.MinimumPotionsOnDeath (=3, from cache constants) set potions to 3; stop music and replay level music (fade 1000). There is no gold/XP/item death penalty in this path.
- Online only: MultiplayerDeathTimer = 15000 ms countdown, 2000 ms global-death message timer (n/a for PC offline).
- Port: features/persistence/death_restart_v1.{hpp,cpp} models gates and ordering (ticket, buffs, position, Revive, idle, min potions, music) with provider callbacks; death_restart_v1_evidence.md matches the IDA ordering. No production caller: main.cpp:1767 is an unrelated lifecycle `revive` op (HP/MP to max, idle); docs/RESOLVED-BUGS.md B006 says "No automatic revive is claimed"; BUGS-AND-IMPLEMENTATION.md I024 open.
- Consequence: after dying the player cannot continue; only Pause -> main menu; profile saves at HP 0.
- Confidence: high. Fix: wire fade-black -> revive action -> LoadCheckpoint provider -> death_restart_v1 execute with a real checkpoint; potion floor 3; revive position from last checkpoint.

### F2. Level transitions, spawn-point selection, world map, fast travel absent in Windows exe (high, known I021)
- IDA: Level::_LoadPlayer 0x3f0018 iterates objects for SpawnPoint (type 13) that are enabled (+138) and whose entry id (+221*4) equals Level entry point (Level+68*4), then SpawnPoint::PlaceObject(player), Character::SetInitialPosition, SG_SetLevelEntryPoint(entry,-1). GSLevel::LoadLevel 0x386818 stores level/entry/difficulty and switches state; GSLevel::Update 0x386630 state machine 1 -> 2 (Level::Load) -> 3 (Level::Update until type==38) -> 4. PlayerSavegame FTVL (SG_SetFastTravelIdUnlocked 0x4663f8, UnlockAll 0x466384), LVLS (SG_SetLevelState 0x466e48, SG_SetMapLocState 0x466b18), LEPT/LUSP entry/spawn ids, MenuWorldMap.
- Port: start position is command-line `--position 1090.75,-212.202,258` in swamp.args (main.cpp:431); actor_population.cpp:140 explicitly skips SpawnPoint ("requires caller-owned spawning policy"). level-world/canonical_spawn_point_v15.cpp and character_menu_campaign_save_v50.cpp (LEPT/LUSP/LVLS/FTVL writers) exist but serve the android-native target; Windows save_store.cpp / GameSave / CharacterState have none of LEPT/LUSP/LVLS/FTVL (only menu_metadata level_row/current_act and visited_modules). runtime_source_checkpoint_v1.hpp: source_entrypoint_persisted=false.
- Consequence: fixed start coordinate (not verified equal to the authored entry SpawnPoint: unclear); doors/exits do not load the next level; fast travel/level unlock state never persists.
- Confidence: high for absence; medium on start coordinate. Fix: select SpawnPoint by entry id from level XML; persist LEPT/LUSP/LVLS/FTVL equivalents; level load by LevelList row.

### F3. Automatic checkpoint save and world-state persistence differ (medium-high)
- IDA: Level::CheckpointSave 0x3f04b4 (level type > 33 and player alive): stores revive position into Character +5224.. and +5236.., Character::SG_SaveCheckpoint, LevelSavegame::SaveCheckPoint. Level::QuickSave 0x3f059c: only type 38, player alive, offline or host; LevelSavegame::Save. LevelSavegame::ValidateCheckpoint 0x463288 checks checkpoint file vs level/difficulty.
- Port: manual F5 (main.cpp:3011) / F9 (3024) snapshot via game_save.cpp (character + actors + objects + RNG) and pause-menu profile-only save (main.cpp:2945 "worldCheckpointOmitted"). No checkpoint-zone triggered CheckpointSave and no stored revive position.
- Consequence: no automatic checkpointing; after quit/reload profile (HP/MP/XP/gear) is restored but world (chests, kills, triggers) only if F5 was used.
- Confidence: medium-high. Fix: bind checkpoint zones to a CheckpointSave recording revive position and world objects (feeds F1).

### F4. Persisted property set (medium, partial)
- IDA: PlayerSavegame::__SaveProperties 0x4689d8 writes all 224 CharacterProperties (those with type flag 0x20 are restored on load); sections PNAM, PLVL, PCLS, PDFL, LNAM, LEPT, LUSP, LVLS, SKIL, FAES, CFEE, QEST, PROP, GEAR, FTVL.
- Port: CharacterState/SaveStore (character_state.hpp) persists level, HP/MP, XP, gold, stats, stat/skill points, inventory/equipment, skills+slots, faery per difficulty, quest CQPG blob, difficulty, menu metadata, visited modules. Missing: LEPT, LUSP, LVLS, FTVL; unclear which of the 224 properties are runtime-only vs saved.
- Confidence: medium. Needs: list of property indices with flag 0x20 compared against save_store fields.

### F5. PlayerLevelUp script and LEVEL UP text missing (medium)
- IDA: PlayerManager::_ManageCharacters 0x37280c (pseudocode ~lines 362-380): when level changes by exactly +1 (and Player+1253 set) it runs ScriptManager::StartScript(GetIDFromName("PlayerLevelUp")) if it exists and VisualFXManager::PlayAnimFXSet(135, character); level property 19 rewritten (8.8 fixed).
- Port: features/effects/runtime_level_up_presentation_v1.cpp plays FX 135 and records text "LEVEL UP!" as "drawn=0 (no HUD status-message owner)"; engine-ui/reports/authored-status-timeline-v27/NOTES.md lists the script/status/tutorial branches as outstanding; loot/runtime_death_rewards_v1.cpp:223 calls on_level_up.
- Consequence: FX but no message; any PlayerLevelUp script effects (e.g. tutorial prompts) skipped. Confidence: medium (script contents and Act 1 existence not inspected).

### F6. Gameplay camera zoom: wheel and auto-zoom not wired (low)
- IDA: CameraLevel::HandleZoom 0x410218: requested(+0x88) and automatic(+0x8c) clamped to DesignSettings ZoomMin(0xac)=0 and ZoomMax(0xa8)=0.35 (minimap mode uses 0x4c/0x48); applied(+0x90)=min(requested,automatic); camera child z = -applied * defaultDistance(+0x94). Level::UpdateCameraZoom 0x3f9870 (offline) moves automatic zoom by (AutoZoomRefPoint 0.2 - screen margin) * AutoZoomStep 0.1 using CoopCamLimits (Bottom 1, Sides 0.9, Top 0.4). Values read from .local-inputs/assets-extra/android/data/pydata/design_pyarray.bin.
- Port: original_camera_config.cpp sample(): zoom = 0 after intro idle clip (clamp(0,zoomMin,zoomMax)); gameplay_camera_level_v4.cpp / gameplay_camera_zoom_v17.cpp reproduce HandleZoom and Update exactly but are not linked into the exe (CMakeLists lists only target_v2, scene_v3, damping_v1).
- Consequence: default framing identical (requested zoom 0); only optional wheel/pinch zoom is absent. Confidence: high.

### F7. Camera items verified faithful (info)
- CameraLevel::Update 0x410390 order (transition, anchor, offset, centering, +offset98, zoom, child position, ghost, damping, root position) matches gameplay_camera_level_v4.cpp; HandleDamping 0x41165c (velocity=(desired-root)*0.7, pos=root+dt*0.001*vel) matches gameplay_camera_damping_v1.cpp and original_camera_config.cpp update_anchor; HandleTransition 0x411830 matches campaign_camera_adapter.cpp / gameplay_camera_target_v2.cpp; CameraBase::SetData FOV 0.42963 rad and aspect 1.6678 match (0x3edbf877, 0x3fd578e9); LevelConfig camera_znear 600 / camera_zfar 10000 defaults match OriginalCameraConfig. Unclear: CameraTarget::EnableDamping(false)+velocity reset on teleport/SetPosition (pseudocode-all.c ~110120) is not visibly reproduced in main.cpp (low risk).

### F8. Not compared (unclear)
- Level::Update 0x3f82d8 per-frame ordering (level script start, Character::SG_Update, PlayerManager::Update, ExecuteAllScripts, GameEventManager, physics, SpawnGroupManager, ObjectManager, camera, fog, music) versus port frame loop main.cpp ~3320-3500.
- Level::UpdateDynamicFog/UpdateFog, Level::UpdateListener, Level::CanMoveTowards 0x3f94f4.

## Mapping table
| IDA address | IDA function (or group) | port location(s) | status | note |
|---|---|---|---|---|
| 0x376100/0x375fe8 | PlayerManager::_CheckLocalDeaths/_HandleLocalDeaths | features/persistence/death_restart_v1.cpp (model only) | missing | no production caller (I024) |
| 0x375af0/0x375eb4/0x376380 | _HandleGlobalDeaths/_CheckGlobalDeaths/OnHostChanged | none | n/a | online co-op only |
| 0x37565c | PlayerManager::ReviveLocalPlayers | death_restart_v1.cpp:43-76 (model) | partial | order modelled, not wired |
| 0x370bf4/0x370c9c | SetCharacterDeathTimer/ResetCharacterDeathTimer | none | n/a | MultiplayerDeathTimer online only |
| 0x3f0428 | Level::LoadCheckpoint | runtime_source_checkpoint_v1.cpp, main.cpp F9 | partial | manual only |
| 0x3f04b4 | Level::CheckpointSave | none | missing | no checkpoint-zone autosave |
| 0x3f059c | Level::QuickSave | main.cpp:3011 (F5) / 2945 | divergent | different data model, manual |
| 0x463288 | LevelSavegame::ValidateCheckpoint | game_save.cpp restore policy | partial | different validation |
| 0x464b2c/0x466184/0x465450 | PlayerSavegame::SG_Save/SaveCheckpoint/LoadCheckpoint | save_store.cpp, game_save.cpp | intentional-adaptation | own SaveStore format (features/campaign_save/FORMAT-MIGRATION.md) |
| 0x4689d8/0x46932c | __Save/LoadProperties | save_store.cpp | unclear | 224-property set not enumerated |
| 0x4663f8/0x466384/0x469b18 | FastTravel unlock/list | none (android-native only) | missing | |
| 0x466e48/0x466b18/0x46a7a0 | LevelState/MapLocState | visited_modules only | missing | LVLS absent |
| 0x3f0018 | Level::_LoadPlayer (SpawnPoint by entry id) | main.cpp:431 --position | divergent | fixed coordinate; actor_population.cpp:140 skips SpawnPoint |
| 0x37280c | _ManageCharacters level-up branch | runtime_level_up_presentation_v1.cpp, runtime_death_rewards_v1.cpp:223 | partial | no script/text |
| 0x37280c | _ManageCharacters net sync | none | n/a | replication |
| 0x36fd58 | PlayerInfo::SetCharacterXP | none | n/a | net struct setter |
| 0x386630/0x386818 | GSLevel::Update/LoadLevel | main.cpp session setup | partial | no level-by-id load |
| 0x383fbc | GSEndGame::Update | none | missing | end-game/menu_About flow not audited further |
| 0x3f82d8 | Level::Update | main.cpp frame loop | unclear | ordering not compared |
| 0x3f1008 | Level::_LoadCamera | original_camera_config.cpp load | faithful | FOV/aspect/znear/zfar/CamAnimSet |
| 0x3f9870 | Level::UpdateCameraZoom | not linked | missing | affects zoom>0 only |
| 0x41068c | CameraLevel::Load | original_camera_config.cpp load | faithful | distance = parent-target length |
| 0x410390 | CameraLevel::Update | gameplay_camera_level_v4.cpp, original_camera_config.cpp | faithful | |
| 0x410218 | CameraLevel::HandleZoom | gameplay_camera_level_v4.cpp (not linked) | partial | exe zoom fixed 0 |
| 0x40fa68 | CameraLevel::HandleCentering | none | n/a | 2+ local players |
| 0x41165c/0x4117bc/0x411830/0x4115d8/0x4119c4 | CameraTarget damping/offset/transition/ghost/SetTarget | gameplay_camera_damping_v1.cpp, gameplay_camera_target_v2.cpp, campaign_camera_adapter.cpp | faithful | |
| 0x410930.. | CameraOverview | gameplay_camera_overview_v18 (not linked) | unclear | map overview camera |
| SceneManager::* | render/technique/rimlight/xray | renderer | n/a | rendering |
| 0x3f2.. | Level::_Load* batching/lightset/meshes/script | level-loader, level_manifest | unclear | not audited |
| Arrays::*::readNames, LevelConfig props | data readers | game-data | n/a | camera defaults verified |
| ctors/dtors/thunks | PlayerManager/PlayerInfo/Level/Savegame/Module | - | n/a | boilerplate |
| 0x36e*,0x378* | PlayerManager controller/player lookup | - | n/a | multiplayer plumbing |
| MenuWorldMap / MenuCharMenu_Map | world map UI | features/map_ui (minimap) | missing | no world map/fast travel UI |

## Note for coordinator
The message adding quests-batch functions (ObjectiveTemplate_*, GetRewardImplInstance, etc.) was addressed to the quests batch; I am the level batch and did not audit those.
