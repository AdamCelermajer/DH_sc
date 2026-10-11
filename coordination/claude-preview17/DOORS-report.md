# DOORS report (Preview 17)

Branch `p17/doors` (worktree `DH_wt/p17doors`), started from `p17/conditions` (`d8eff368`). Commits: `0fb4da28` (door, lever and decor runtimes, command handler), `a23417a5` (EXE wiring), `bdd9524c` (texture tolerance, engine channel filter), `7d688884` (diagnostics). Not pushed.

## Result

Delivered and seen in the real EXE (Swamp `001_swamp`, quiet hidden runs):
- Door visuals load (3 Swamp doors, embedded-scene models from the game_objects rows), show the idle clip of their state, and play the transition clip.
- `Script_OpenDoor` runs through the campaign host door route (`routes.doors`). `open_SwampKingDoor` started by `--campaign-start` drives `_prim_SwampDoor_FIRE` and `_prim_SwampDoor` from closed (state 0) to opening (state 3, busy). The after-frame shows the large door panel gone (see Visual evidence).
- Lever (`_prim_TriggerObject`, `unlock_cond=IsAfter_Swamp_KillWitch2`) is bound: its unlock gate is evaluated at bind and on flips, and it is a context-button candidate once unlocked. Its use path is implemented but was not exercised (the lever stays locked until the witch dies).
- Gated AnimatedDecor (`_anim_swamp_dustmotes_swamp2`, `_anim_destructible_wall_001`) is bound: present or absent by its condition, with `startanim` looping when present. Creation and removal happen on flips.
- Door presence (activate/deactivate), lever unlock and decor presence are re-derived from the condition watch at every quest bind (session start and reload). Gate flips reach these owners through the world-gate owner (the stub is replaced).

NOT delivered or NOT verified (details in Gaps): live save/reload of door state (checkpoint rejected in this config), the witch kill in play (the witch is in another level), spawns of gated characters on flips, collision removal, door and lever sounds (submitted, but `asset_missing` in the sound layer is not checked), and the reference-video comparison.

## Investigation (evidence)

Logic (IDA `libDungeonHunter2.so/pseudocode-all.c`):
- `Door::Open` (003e786c) requires state 0 and not busy. `Door::Close` (003e763c) requires state 1 and not busy. States: 0 closed, 1 opened, 2 closing, 3 opening. With no visual or skip, the end state is applied at once; otherwise the transition clip "Open"/"Close" plays and the state is set to 3/2 with busy.
- `Door::__Callback` (003e8568): at clip end, busy clears and the end state applies (`Opened(0)` plays "IdleOpened", `Closed(0)` plays "IdleClosed" looping).
- `Door::__EventCallback` (003e8450): the authored markers "opened"/"closed" apply the end state early (no idle replay).
- `Opened`/`Closed` (003e7788/003e7598): Opened disables the physical filter and clears the floor dead-end flag; Closed does the reverse.
- Sound: `Close` plays Arrays::Doors row +12 (sound_c), `Open` plays row +16 (sound10), via `VoxSoundManager::Play3D` at the door position, only when the id is not -1. Both branches (direct and clip) play it.
- Serialize writes the 4-byte state; Deserialize maps 1 and 3 to opened, others closed.
- `TriggerObject::TestInteractiveCond` (00399 3bc): unlock_cond via `ConditionList::Eval`; unlocked plays "unlock". `TriggerObject::Interact` (00399af0): OnTrigger script, "activate" clip, row +16 sound, `ScriptManager::StartScript` of the authored script, then `QE_TriggerOn` raised. `GetInteractionType` = row +4.
- `AnimatedDecor::InitPost` (00389128): MeetCondition false removes the decor; true plays startanim (default "idle") looping. "randomall" is handled by the source but not here (no such decor in the Swamp).

Data (scan of all Android module files, `claude-preview17/scan/`):
- Doors in Swamp: `_prim_SwampDoor` (data Swamp_Door_Ruins), `_prim_SwampDoor_FIRE` (Swamp_Door_Ruins_FIRE, `idle_sound` sfx_magical_wall_loop), `_prim_Swamp2Door_FIRE` (Red_Door, act IsBefore_Gothicus2Survivors). Lever `_prim_TriggerObject` (Lever_Swamp, unlock_cond IsAfter_Swamp_KillWitch2, script open_SwampKingDoor). The script opens `_prim_SwampDoor_FIRE`, waits, opens `_prim_SwampDoor` (script 53 in `original-campaign.xml`).
- Gated AnimatedDecor in Swamp: `_anim_swamp_dustmotes_swamp2` (swamp_caveentrance_effect.bdae, act IsAfter_Gothicus2Survivors) and `_anim_destructible_wall_001` (CS_wallbreak_death_cutscene.bdae, act IsBefore_Swamp_Escape).
- Clip names in the models: doors have `open`, `close`, `idleopened`, `idleclosed` and the markers are in the clip library. Fire doors add a particle emitter (`GL_PCloud01-emitter`, type 28) and a material channel (`Standard_8zxcv`); the lever model has a material channel (`Glo...`).

Waterfall (question 3): NOT FOUND as a witch-gated object.
- Scan of 2984 declarations over every Android module (`3d/modules`, all levels): 1438 AnimatedDecor, 114 Door, 43 TriggerObject, 34 SoundEmitter. Every `_anim_animwater_*` / `_anim_water*` decor (125 entries, swamp_watercaustics, cryptwater, bossroom_effect, water_temple) has no activate/deactivate condition. None references a Kill-witch condition.
- The only "waterfall" names are `_prim_SoundEmitter_waterfall*` (witch_a_wpcave_nw_00_01.mgp, trollcave_corner_swpcave_nw_00.mgp): sound emitters with `BanditLairRiverLoop`, no conditions, no visual.
- Closest witch-gated visual change: `bossroom_witch_.mgp` `_prim_portal_witch` (Door, data Swamp_exit_door_witch) opened by `_prim_TriggerZone_12` (act IsAfter_Swamp_KillWitch, script `Open_portal_witch`). That is in the witch cave level (`SWAMP_CAVE_WITCH_A`, `002_swamp_cave_witch`), not in the Swamp level. Its script body is not in the Swamp campaign XML.
- Visual evidence: the reference video was NOT checked for a waterfall in this stream (time). Nothing about the waterfall is observed.

## Implementation

New feature `port/windows-foundation/features/doors/`:
- `door_runtime_v1` (Door state machine from the IDA above; visuals from the Arrays::Doors row via `ContainerTablesV1`; transition clip timing and markers per direction; sound requests; `refresh_conditions`; `restore`; `take_changed`).
- `world_objects_v1` (LeverRuntimeV1: row type/sound/visual, unlock via `refresh_conditions`, `use`; DecorRuntimeV1: gated decor only, create/remove on flip).
- `door_world_v1` (one WorldObject per door with its stable ID; component `dh2.door.state.v1`, 1 byte; reuses objects bound by others).
- `door_commands_v1` (Script_OpenDoor/CloseDoor: name at string 12, wait byte at scalar 20 (absent = 0); absent or gated-out doors are no-ops; blocking while busy with wait=1).

Changes elsewhere (small, anchored with `P17 DOORS`):
- `campaign_host` (`CampaignHostServices::doors`, routed through the existing `routes.doors` slot, which was pass-through).
- `main.cpp`: declarations; population load and visuals after the containers block; door world-object bind; `refreshDoorWorld` (condition refresh + door restore) from the quest bind; gate owner (door/lever/decor); lever interactable candidates and context-button use; per-frame update, sound requests and persistence; draw; textures.
- `engine-animation/animation.cpp`: channels whose target is not a registered scene node (material/FX parameter, particle emitter) are ignored, like the existing unregistered-URI filter. Errors now print the type and target. This is the one shared-engine change; it affects any model with such channels (previously an error), and no test failed.
- Textures of door/lever/decor: a missing alpha map is logged and drawn without a mask (was a fatal error).

## Isolated tests

- ctest: 131 of 133 pass with `-Jobs 6` and the toolchain on PATH. Failing: `session_skill_binding` (allowed) and `character_menu` (pre-existing on the base, see the CONDITIONS report). No door-specific unit tests were added (see Gaps).

## Integrated runtime verification (quiet hidden runs, real EXE)

Assets: `.local-inputs/claude-preview17/doors-run/assets` (hardlink copy of the quests-run assets plus the overlays listed below). Logs in `doors-run/out/`, frames in `doors-run/cap/`.
- Bind (`doors-run1.log`): `Doors declarations=3 levers=1 gated_decor=2`, `Doors world objects declarations=3 notices=0`, `World gates bound=12 notices=0`, `Doors refreshed present=3/3 levers=1 decor=2`. No visual notices.
- Door placement check: `Door view` lines show `_prim_SwampDoor` at (-12588.5, 11512, 73.6). The population frame adds the module offset (-12000, +12000) to the mgp coordinates (-588, -488). The camera tests use this frame.
- Open script (`open-240.log`): `[door] command kind=54 open declaration=_prim_SwampDoor_FIRE skip=0 wait=0 state=0->3 busy=1` and the same for `_prim_SwampDoor`. Sound ordinals 487 and 427 submitted; no audio runtime in these runs (`status=no_audio_runtime`).
- Visual evidence (AGENTS item 1): frames from the player standing at the door (`cam2-D.png` closed; `open-30.png` 30 frames after the open; `open-240.png` 240 frames after): `sbs-closed-mid-open.png`. Observed: the large wooden door panel at the top centre is present in the closed and mid frames and gone in the open frame, where stone pillars are visible. Mid-transition (open-30) looks like closed. Not verified: the exact clip timing, which door leaf moved, and whether the frame difference is entirely door motion. The purple glow in the closed frame is not explained (it is absent in the mid frame, probably lighting at startup).
- Save and reload: NOT verified. `Save checkpoint rejected frame=200: Campaign lifecycle/controller providers are not persisted` is the existing P16 checkpoint policy: `--campaign-triggers` leaves lifecycle providers active. The persistence code (bind, persist, restore) is compiled but not exercised by a live save.
- Lever: not exercised (locked until the witch dies). The witch is in the cave level, which this EXE does not load in the Swamp level.
- Decor: created at load (no visual notices). Not visually checked in a frame.

## Gaps (explicit)

1. Live save/reload of door state: blocked by the existing checkpoint rule with campaign triggers (see above). Needs the P16 checkpoint policy to accept campaign providers, or a run without triggers that still binds the door route.
2. Witch kill in play: the Bogwitch is in `002_swamp_cave_witch` (`bossroom_witch_.mgp`), not in the Swamp level. No live kill of the witch is possible in this level. The kill path (`raiseQuestKills` on combat death, in `main.cpp`) exists but was not exercised with the witch. The conditions run used the property-435 test aid. Needs the level transition stream (`p17transitions`, not merged here) or a run that starts the witch cave level.
3. Gated characters do not spawn or despawn on a flip (the spawn owner, P16 SPAWN, is not bound to the condition watch). Population is still at load for characters.
4. Door collision: not removed or restored. The floor owner (`FlagFloorAsDeadEnd`, physical filter) is not bound. `collision_closed()` reports the state only.
5. Door and lever sounds: requests are submitted through `runtimeAudio->submit_world_item_sound` with the raw Arrays ordinals (487, 427, ...). Not verified in an audio run (no `--audio`), and the mapping from Arrays sound ids to the SoundAutoGen ordinal is assumed to be identity. Check `asset_missing` in an audio run.
6. Lever quest event: `QE_TriggerOn` is not raised. Its object id mapping (this+25 / table row) is not decoded.
7. `Door::Open/Close` extra direct branch (`vtable+196` with bytes 750/752) is not reproduced: treated as false (transition plays the clip whenever a visual exists and skip is off).
8. `SourceDoorCommands` (progression_barriers) is not used. Its canonical ObjectManager/CanonicalDoorV27 receiver is not part of the Windows EXE. `DoorCommandsV1` reproduces its command contract (name, wait, skip, blocking) on `DoorRuntimeV1`.
9. Ungated AnimatedDecor (torches, waterfalls, dust motes without conditions) is not drawn by this owner, as before. Whether it is drawn elsewhere was not checked.
10. The door's particle fire glow (emitter channel) and the material channels are ignored (no particle runtime, no material animation).
11. Alpha mask `env_swamp_alpha.tga` is missing in every package and staging folder checked. The door materials draw without that mask.
12. Scale of decor: `_anim_*` placements are used as authored; no visual check against the reference video.
13. No door-specific unit tests were added.
14. The witch and waterfall questions: see Investigation. No reference-video check was done in this stream.

## Placeholders

- Lever `activate` clip plays the authored clip name only; no separate unlock/idle sequence is verified.
- `door_commands_v1`: an absent door is a silent no-op with one log line (source: the cached object is cleared).

## Package files required

- game_objects tables: `data/game_objects_pyarray.bin`, `data/game_objects_pyarraynames.bin`, `data/game_objects_dictionary_pyarray.bin`, `data/game_objects_dictionary_pyarraynames.bin`. Already staged (identical to the Android device files) in `.local-inputs/assets-extra/android/data/` (non-destructive, no copy was needed).
- Door and lever models: `data/3d/gameobjects/go_swamp_door_ruins.bdae`, `go_swamp_door_ruins_fire.bdae`, `go_door_fire_red.bdae`, `go_swamp_witch_lever_lockable.bdae`. Staged in `assets-extra/android/data/3d/gameobjects/`.
- Decor models: `data/3d/animateddecors/swamp_caveentrance_effect.bdae`, `CS_wallbreak_death_cutscene.bdae`, and the other Swamp decor (`swamp_*.bdae`). Staged in `assets-extra/android/data/3d/animateddecors/`.
- Missing everywhere checked: `env_swamp_alpha.tga` (door material alpha mask).
- The run overlay for the verifier: `doors-run/assets/data/3d/animateddecors` and `gameobjects` were hardlinked from `assets-extra`; the four game_objects files were copied into `doors-run/assets/data/`. Nothing in `assets-extra` or the quests-run assets was changed.
- Campaign: `original-campaign.xml` (the door script) must be passed with `--campaign-commands` and `--campaign-triggers`.

## Verifier script

1. Build and test: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p17doors -Test -Jobs 6` (toolchain on PATH). Expect exactly `character_menu` and `session_skill_binding` to fail.
2. Door bind and presence: `quiet_run.ps1 -JobsFile doors-run/jobs-door.json -Parallel 1`, then grep `Doors declarations=3`, `Doors refreshed present=3/3`, `World gates bound=12 notices=0`.
3. Door open and visual: `quiet_run.ps1 -JobsFile doors-run/jobs-open.json -Parallel 2` (`door-cam-run` args plus `--campaign-start open_SwampKingDoor`). Expect `[door] command kind=54 open ... state=0->3`. Compare `doors-run/cap/cam2-D.png` (closed) with `open-240.png` (open) at the top centre.
4. Not runnable as specified: the save and reload check (blocked, gap 1) and the witch-kill check (gap 2).
