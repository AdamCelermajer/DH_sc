# CONTAINERS report (Preview 16, branch p16/containers)

Status: IN PROGRESS. Sections are updated per task (T1..T6) and each task has its own commit.

## Evidence (investigation)
- Survey: coordination/claude-preview15/CONTAINERS-survey.md (IDA addresses: Container::Interact 0x3a0b38, DoOpen 0x3a0a98, DestructibleContainer::Interact 0x3a0da0, GetInteractionType 0x3a16cc (openable = 0) and 0x3a0d60 (destructible = 8)).
- Data: original grouped game_objects streams (game_objects_pyarray.bin + names, GameObjectDict dictionary) decoded by the existing GameObjectArraysOwnerV81 (level-loader). Visual id -> ColladaFile through GameObjectDict.

## T1 population + closed visuals (done)
- features/containers/container_class_registry_v1.{hpp,cpp}: class registry. Registered: OpenableContainer (interaction type 0), DestructibleContainer (interaction type 8). Other gametypes are counted per gametype as unsupported in one log line. Extension point: ContainerClassRegistryV1::add.
- features/containers/container_declarations_v1.{hpp,cpp}: decodes the original tables once, instantiates each authored declaration of a registered class (data_desc -> row -> visual id -> ColladaFile, loot/sound ids), and decodes each distinct visual through CharacterVisual::load_embedded_scene (the chest and urn BDAEs are skinned, so the static-scene path is rejected).
- main.cpp hooks (CRLF file, additive): include, state next to the population, loader after the population notices, texture binding before bindMaterials, draw after the actor loop.
- Log line: `Containers instantiated=N declarations=N visuals=N unsupported=...`.
- Swamp result: instantiated=14 (5 chests, 9 barrels), visuals=14, unsupported=AnimatedDecor:37,CheckpointZone:4,Decor:10,Door:4,Dummy:41,LevelConfig:1,QuestMoveInZone:1,SoundEmitter:1,TriggerObject:1,TriggerZone:18,TriggerZoneExitLevel:3.
- Build: CMake target foundation_containers (features/containers + level-loader/game_object_arrays_owner_v81.cpp, level-world destructible/openable tables, game-data game_object_dictionary_v11).

### Package files required (staged, not yet in the package)
The rc3 package lacks the game_objects tables and two gameobject BDAEs. Copied non-destructively from the Android device folder:
- assets/data/game_objects_pyarray.bin, game_objects_pyarraynames.bin, game_objects_dictionary_pyarray.bin, game_objects_dictionary_pyarraynames.bin
- assets/data/3d/gameobjects/go_chest_swamp.bdae, go_swamp_urn_breakable.bdae
Copies also in .local-inputs/assets-extra/android/data/ (mirror).
The package root must include these (rc3 directory was modified additively for local testing only).

### Verification so far
- Quiet runs (quiet_run.ps1, hidden, silent): job t2-batch1 and t2-chest in .local-inputs/p16-runs/. Captures: chestA (chest -2198.67,935.19 north of player), chest2 (1826.65,1796.04), barrel1 (-1229.64,-1224.21), barrel2/barrel3 rendered (not individually inspected). Visual check: chests and barrels draw at their authored places, closed pose.

## T2 interaction (done in isolated/quiet runs; not wired to the Space context button)
- Gate order (Container::Interact 0x3a0b38 plus the caller's OOI query): unknown declaration -> no visual -> range <= 200 (OOI_Distance, horizontal; the range lives in the caller, Container::Interact itself has none) -> state 3/4 rejected -> SetState(3) and play `activate`.
- Exposed interaction type per declaration: 0 for OpenableContainer, 8 for DestructibleContainer (ContainerRuntimeV1::interaction_type).
- Debug: `--interact-at DECLARATION@FRAME`, repeatable, default off. Log: `Container interact declaration=... status=... state=A->B distance=... frame=...`.
- Batch t3-a (quiet): accepted at 100 units (chest _prim_OpenableContainer_2 and _prim_OpenableContainer_1); out_of_range at 300; unknown_declaration; rejected_state while activating (state 3) and after opened (state 4); destructible barrel returns unsupported_family (see gaps).

## T3 chest open timing (done for chests; destructible path open)
- IDA: DoOpen runs from the `opened` event; Container::__Callback sets state 4 and plays `idleactive` when the `activate` clip completes. The clip keeps playing after `opened` (the event is not the end of the lid motion).
- Implementation: markers and clip end read from the visual (markers("activate"), animation_range); clip clock = visual animation_elapsed_seconds.
- Measured (quiet, 3 chests): `opened` at clipMs 240-246 (authored marker 233-266 ms, frame granularity ~15 ms); lid state idleactive after the clip ends (~1.9 s). Lid sequence in frames 18/24/30/60/140 captured (chest crop strip) and checked.
- Reference video (Part 1, v1.0.3): frames 209.3-210.0 s at 0.1 s steps: closed at 209.3-209.4; lid rising 209.5-209.9; fully open 210.0. Our lid is mostly raised by ~140 ms, so our rise may be faster than the original (~0.5 s). Not confirmed (coarse sampling, different camera/HUD); no change made.
- Logs: `Container opened declaration=... loot=227 clipMs=... frame=...`; a non-empty row script prints `Container OnOpen script=... not run: spawn/script provider not bound` (T6 hook; no chest on Swamp has a script, so this has not fired).

## Not done in this pass (honest status)
- T4 rewards via DROPS: not implemented. The drop owners (session_container_modern_drop_v1, RuntimeWorldItemAdapterV1 via death rewards) are bound to a CombatSession WorldObject; a session-free roll for an arbitrary declaration needs a new loot-roll API (LootTablesV2/LootEntryServicesV8 + RNG). Loot id is logged on open (loot=227 chest, 9 barrel).
- T5 persistence (OBJS 7-byte state): not implemented. Container state lives in ContainerRuntimeV1 and resets on level reload; no GameSave change.
- Destructible (barrel) hit/destroy: not implemented. The source hit count is `slot count - 3` (Destructible::InitPost, vtable+16 on the visual), and CharacterVisual does not expose the slot count yet. Barrels are instantiated and drawn; interaction returns unsupported_family.
- T6 Lua OnOpen summon: only the unbound-provider log line. Moth/plant/zombie summon is not implemented.
- Main Space-button dispatcher and trigger/trap/shrine/door families: not implemented (registry extension point exists).

## Generality proof (same loader, no code change)
Quiet runs, each level with the Swamp arg set (only `--level`/`--source-root-scopes` changed), population active:
| level | declarations | instantiated | visuals | unsupported gametypes (count) |
|---|---|---|---|---|
| 001_swamp | 14 | 14 | 14 | AnimatedDecor 37, CheckpointZone 4, Decor 10, Door 4, Dummy 41, LevelConfig 1, QuestMoveInZone 1, SoundEmitter 1, TriggerObject 1, TriggerZone 18, TriggerZoneExitLevel 3 |
| 003_darkwood | 20 | 20 | 20 | AnimatedDecor 13, CheckpointZone 1, Door 15, Dummy 24, LevelConfig 1, QuestMoveInZone 5, TriggerZone 16, TriggerZoneExitLevel 11 |
| 005_infectedvillage | 5 | 5 | 5 | AnimatedDecor 9, Door 3, LevelConfig 1, QuestMoveInZone 1, SoundEmitter 1, TriggerZone 2, TriggerZoneExitLevel 2 |
| 025_icy_hub | 25 | 25 | 25 | AnimatedDecor 40, CheckpointZone 3, Door 1, LevelConfig 1, QuestMoveInZone 1, TriggerZone 4, TriggerZoneExitLevel 4 |
| 036_underworld_hub | 19 | 19 | 19 | CheckpointZone 3, Door 3, Dummy 7, LevelConfig 1, QuestMoveInZone 4, TriggerObject 1, TriggerZone 5, TriggerZoneExitLevel 2 |
The witch cave in the brief is a .rule.xml (not .mlx) and was not run. Earlier, before the gameobject BDAEs were staged, visuals for the same levels were 0-4; that was asset staging, not the loader.

## Package files required (added to the rc3 package assets for local testing; root must package them)
I modified `.local-inputs/windows-source-clock-v19-preview-15-rc3/assets` additively (cp -n only; nothing overwritten). Copies also in `.local-inputs/assets-extra/android/`.
- assets/data/game_objects_pyarray.bin, game_objects_pyarraynames.bin, game_objects_dictionary_pyarray.bin, game_objects_dictionary_pyarraynames.bin
- assets/data/3d/gameobjects/* (85 files from the Android 3d/gameobjects folder, 4.3 MB)
- assets/data/scene/{003_darkwood,005_infectedvillage,025_icy_hub,036_underworld_hub}.mlx and the same under assets/original-cache/data/scene
- assets/data/3d/modules/{darkwood,infectedvillage,icy_hub,underworldhub} and the same under assets/original-cache/data/3d/modules

## Verifier script (quiet, hidden)
- Build: `powershell -NoProfile -File DH_wt/p14_build.ps1 -Name p16containers`
- Batch: `node .local-inputs/p16-gen-jobs.js <dir> "name=x,y,z@fx,fy,fz!--interact-at,DECL@20"` then `quiet_run.ps1 -JobsFile <dir>/jobs.json`.
- Expect: `Containers instantiated=14 ... visuals=14`; accepted -> `Container opened ... clipMs~240`; out_of_range at 300 units; rejected_state after opened.
- Captures: .local-inputs/p16-runs/t2-chest/chestA/chestA.png, t2-batch1/*.png, p16-runs/seq-strip2.png.

## Commits (branch p16/containers)
- dcc6de86 T1 loader and closed visuals
- 4e1a1157 T2/T3 interaction gate and chest open timing
