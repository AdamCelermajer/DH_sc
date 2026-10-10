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
