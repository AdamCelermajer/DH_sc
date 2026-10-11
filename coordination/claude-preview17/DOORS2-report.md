# DOORS2 report (Preview 17)

Branch `p17/doors2` (worktree `DH_wt/p17doors2`), started from `p17/doors` (`c92eefc6`). Commits: `3598abcc` (save/reload checkpoint, mask lookup, channel filter and test), `bd3a2a27` (lever use path, QE_TriggerOn, sound uri logs, condition overrides), `6b92289a` (no bind banners on restore, registration condition), `72c7be46` (component rename). Not pushed. Build: `p14_build.ps1 -Name p17doors2 -Jobs 6`, EXE `DH_wt/build-p17doors2/dh-foundation.exe`. Quiet runs use the rc3 package assets (`.local-inputs/windows-source-clock-v19-preview-15-rc3/assets`), run folder `.local-inputs/claude-preview17/doors2-run/`.

## Result by item

| # | Item | State |
|---|------|-------|
| 1 | Door collision removed/restored, hero walks through open / blocked by closed | **Not done.** No door physical body or floor flag reaches movement (see Gaps 1). |
| 2 | Save/reload of gate and door state with campaign triggers on | **Done for in-process save (F5/frame 200) and reload (F9/frame 220).** Three refusing rules found and handled. Fresh-process restart does not restore door state (Gaps 2). |
| 3 | Lever (TriggerObject) use: QE_TriggerOn raised, unlock honoured, condition forced true | **Done** (event id mapping is a labelled placeholder). |
| 4 | Live spawns of gated characters on a flip | **Not implemented. Source finding:** characters are not spawned on a flip (see Investigation). |
| 5 | Door sounds through the audio host with cue logs | **Done** (submitted with uid and file uri; voices start). Ordinal mapping unverified (Gaps 5). |
| 6 | Missing `env_swamp_alpha.tga` mask | **Found and fixed.** The mask exists as `pvr2_env_swamp_alpha.tga` (BTEX PVRTC); the loader never tried the `pvr2_` name. |
| 7 | Waterfall animated until the witch dies | **Finding stands:** no witch-gated waterfall object in the Swamp. Frames checked; no state change seen at the kill (see Investigation). |
| 8 | Shared-code change in engine-animation (non-node channels) | **Done:** the predicate is in `engine-animation/channel_filter.cpp`, unit test `animation_channel_filter`, and each skipped (type, target) logs once. Verified in the EXE log (5 distinct lines in one run). |

ctest (`-Jobs 6`, direct run with the toolchain on PATH): 132 of 134 pass. Failing: `session_skill_binding` (allowed) and `character_menu` (pre-existing on base `261f2e49`, see the CONDITIONS report). `combat_session_source_departure` briefly regressed during this work and is fixed (6b92289a).

## Investigation (evidence)

**Save refusal (item 2).** The refusal text "Campaign lifecycle/controller providers are not persisted" comes from `CombatSession::validate_lifecycle_checkpoint`. The setter `set_actor_combat_permission_provider` sets `lifecycleRegistered`, and main.cpp installs that provider when `retainHiddenActors || lifecycleEnabled`. `lifecycleEnabled` is true here because the Swamp container OnOpen summon contracts reserve spawn-pool slots (P16 CONTAINERS2, main.cpp ~1282-1335). So the rule was triggered by the spawn-pool lifecycle set up at load, not by the campaign triggers themselves. I did not run a trigger-less control, so this attribution is by code path, not by experiment. Three refusing rules were met in order:
1. `validate_lifecycle_checkpoint`: lifecycle registration also set by the load-time init (freeze and `set state 17` on the two PreSpawn slots). Log: `P17PROBE` run showed `freeze` and `setstate actor=...769 state=17` at load only.
2. Body checkpoint: "lacks matching current source physical-presence facts": lifecycle init/remove_physical changed the body but not `actor.source_physical_present`.
3. Body checkpoint: "source flags mismatch ... state=17 flags=4864 expected=0": no expected flag for state 17. IDA `CSPreSpawn::OnFocus` (`pseudocode-all.c` 003c688c, line 143179) writes `*((_DWORD*)a3+328)=4864` (0x1300), which is the source flags word.

**Door state persistence.** Door state is written to a world-object component (1 byte, DoorStateV1) by `persist_door_world_state_v1` each frame and restored by `restore_door_world_state_v1` at quest bind. In-process F9 restore reads it from the GameSave snapshot (`load_game(liveSave)`). `load_game` is called only on F9 (main.cpp 3875). Fresh process startup does not read the GameSave.

**Door collision (item 1).** IDA `PFWorld::FlagFloorAsDeadEnd` (005252ec, `pseudocode-all.c` line ~378484) sets bit 1 of the floor word at `PFObject+16 -> floor+32` when the door is closed (called with 1 from Closed, 0 from Opened). In the port the field exists (`level-world/floors.hpp`, `source_flags32`) but nothing reads it. `PFObject::CanPathOn` (00524230) reads `floor+36` (type mask), not `+32`. `PFRoom::GetFloorHeightAt` (00520f98) reads `+36 & 0x3000000`. I did not find a reader of bit 1 at `+32` in the PF functions of this pass. The door's physical body (`physical_slot` 0x2dc, `physical_filter`) has no counterpart in the EXE (native bodies exist for actors only). Conclusion: the port has no door collision today; the hero passes closed doors. Not verified by walking.

**Lever (item 3).** `TriggerObject::Interact` (00399af0): OnTrigger script, `activate` clip, sound row +16 via `VoxSoundManager::Play3D`, `ScriptManager::StartScript` of the authored script (`open_SwampKingDoor`), then `EventManager::RaiseAsync` of `QE_TriggerOn` with objective type `PyDataConstants("v2QuestObjectiveType","TriggerOn")`, id `this+25`, row `this+460`. The port's quest runtime matches `object_interact` on `oid1 == object_id` (features/quest_runtime). The decode of `this+25` as an id in the level data is not done (the lever declaration has no `id` attribute).

**Live spawn on a flip (item 4).** Gated characters exist: `Swamp_MerchantCamp_NPC_A` in `merchantcamp_ruins_swe_00.mgp` has `deactivate_cond="IsBefore_Swamp_KillWitch"` (the witch condition). IDA: `Character::InitPost` (003b4d60, line 132036) is the only character-level `MeetCondition` check, and it runs at initialisation. `Level::_LoadBatchList` (003f2d38, line 176579) also evaluates conditions when a batch list is loaded. No runtime path re-evaluates a character condition on a flip, and no campaign script spawns this NPC (`original-campaign.xml` has no reference). So the source does not spawn or despawn a gated character when a condition flips during play. The port matches that: population is evaluated at level load and reload. The port's only in-place spawn is the scripted `--spawn-declared` path, which needs an admitted lifecycle record.

**Waterfall (item 7).** Re-checked against the reference videos (`Desktop/dh2_video_research/video/part1_...mp4`, 22:11; `part2_...mp4`, 15:16; 640x360):
- Witch arena (part 1): the Grievous Bogwitch fight runs to about 975-1003 s (`video-scan/p1kill-01.png`, 1 s steps, 975-1015 s). The boss falls near 1003-1006 s, followed by a cutscene (Celeste: "crisbz? Her heart... it turned into an orb... this is her crystallized power, we should break the seal with this"). The arena is a tiled crypt floor. No water feature is visible in these frames.
- After the kill (part 1, `video-scan/p1post-01.png`, 2 s steps from 1018 s): "Quest Completed: Defeat the Bogwitch in the Northern Caves", inventory and merchant screens, loading, then boardwalk and water scenes. No door or water change is seen at the kill in these samples.
- Waterfall in the reference: Part 2 at 909 s shows a grotto with a waterfall in tree roots, location "Wormroot Hollow" (`video-scan/wf-905-909.png`). It is in Darkwood, not the Swamp and not the witch cave. A single still cannot show animation.
- The Swamp outdoor level was not sampled in detail (time). The earlier finding stands: no Swamp declaration has a waterfall or a witch condition (see the CONDITIONS report).
- The "unexplained purple glow" (item 6) is visible in the closed-door frame and absent in the open-door frame (`doors2-run/cap/sbs-closed-open-240.png`). It belongs to the fire door (`_prim_SwampDoor_FIRE`): its visual has a particle emitter (type 28) and an FX-glow material (type 87), both skipped by the port, so the drawn glow is an inference, not a verified effect.

**Mask (item 6).** `loadDoorTexture` checks `loadImage("env_swamp_alpha.tga")`. `resolve_content_path` tries the bare name and the `data/3d/textures/` name, but the `pvr2_` variant is only tried when the request is already under `data/3d/textures/`. The package (`rc3/assets/data/3d/textures/pvr2_env_swamp_alpha.tga`, BTEX PVRTC2, 1024x1024) decodes through `texture_loader.cpp`, which handles BTEX. The earlier DOORS report saying the mask was missing everywhere is wrong.

## Implementation

- `main.cpp` `loadImage`: adds the candidate `data/3d/textures/pvr2_<name>` (item 6).
- `engine-animation/channel_filter.{hpp,cpp}`: `skip_channel_without_node(type,target,target_is_node,dynamic_channels)`. Skips non-node channels (types other than 1/5/10, not dynamic position/angle) and logs each distinct (type, target) once. `animation.cpp` calls it (item 8).
- `combat_session.cpp`: new `lifecycleActivity` flag, set only on lifecycle transitions (`set_actor_original_state`, non-generic `play_actor_state_sequence`, freeze). `validate_lifecycle_checkpoint` refuses on activity, controller-admission, or network-mode providers; a host permission provider alone no longer refuses. `mark_lifecycle_baseline()` (called in main after the load-time lifecycle records) clears the flag. **Shared-code change: review by the SPAWN/OPENING owners** (the rule was not relaxed for generic programs).
- `main.cpp` lifecycle `init_physical`/`remove_physical`: keep `source_physical_present` equal to the real body (checkpoint rule 2).
- `main.cpp` body checkpoint: `case 17: expected=0x1300` (IDA 003c688c) (rule 3).
- `main.cpp` `bindQuestRuntime(showBindBanners)`: F9 restore and R reload drop the bind-time banners (the NEW QUEST banner was replayed on every restore, visible in `audio-reload-260`).
- Door state component renamed `dh2.world-gate.door.v1` (`door_runtime_v1.hpp`); the brief requires the `dh2.world-gate.*` namespace.
- Lever (item 3): `useLeverObject` raises `QE_TriggerOn` (object_interact) on an accepted use, with a labelled placeholder id (`trigger.object_id = lever stable id`). Debug aid `--lever-use DECLARATION@FRAME` calls the same function as the context button.
- Condition overrides (`--condition-active/--condition-inactive`) also apply to the door/lever/decor refresh (test aid only; the watch is unchanged).
- Sound cue logs: door and lever cues print `uid` and `uri`.

## Isolated tests

- `animation_channel_filter` (`features/animation_channels/channel_filter_tests.cpp`): skip for material/emitter channels with no node; kept when a node exists; types 1/5/10 and dynamic channels untouched; null target untouched; a repeated (type, target) logs nothing, a new one logs exactly one line naming type and target. Passes.
- ctest: 132/134 (see Result).

## Integrated runtime verification (quiet hidden runs, rc3 assets, real EXE)

Run folder `.local-inputs/claude-preview17/doors2-run/`, logs in `out/`, frames in `cap/`.

- **Save and reload with campaign triggers** (`out/audio-reload.log`, `save-run.log`): `Door view _prim_SwampDoor state=0` at load, `[door] command kind=54 open ... state=0->3` at frame ~61 (script `open_SwampKingDoor`), `Saved live checkpoint frame=200`, `Door view _prim_SwampDoor state=1` and `_prim_SwampDoor_FIRE state=1` after `Restored live checkpoint frame=220`. `Swamp2Door_FIRE` stays state 0 (untouched). No replay: the harness start, trigger-zone feed, door command, and gate applies appear once each. `Quest banners suppressed at restore count=1` (no NEW QUEST replay).
- **Visual** (`cap/sbs-closed-open-240.png`, same position -12550): left closed (wooden door panel and purple glow), middle open (panel gone, glow gone). `cap/sbs-closed-open-reloaded2.png` right panel: after the F9 restore at 220, frame 260, no banner, door open as in the middle panel.
- **Lever** (`out/lever.log`, `locked.log`, `lever-audio.log`): with `--condition-active IsAfter_Swamp_KillWitch2`, `--lever-use _prim_TriggerObject@60` gives `status=0 (accepted) unlocked=1`, `script=open_SwampKingDoor status=started`, `QE_TriggerOn raised object_id=834855499` (placeholder), `Quest event kind=3 ... applied=0` (no objective has that id). Without the forced condition: `status=1 (locked) script=(none)`, no event. Audio run: `Lever sound ordinal=501 status=submitted uid=170 uri=data/sounds/sfx_orb_deactivate.wav`.
- **Door sounds** (`lever-audio.log`, `audio-reload.log`): `Door sound ordinal=427 status=submitted uid=158 uri=data/sounds/sfx_giant_wood_door_open.wav` (`_prim_SwampDoor`); `ordinal=487 status=submitted uid=161 uri=data/sounds/sfx_magical_wall_down.wav` (`_prim_SwampDoor_FIRE`). `Audio voice kind=0 token=1..3` start. Close is not exercised.
- **Fresh-process restart** (`out/restart-door.log`): door states return to 0 (`Door view ... state=0`). The GameSave is not loaded at startup (gap 2). Quest state at bind is `current=50`, the same as before the save.
- **R reload** (`out/witch-r.log`): `Foundation error: Spawn template table: Actual initialized CharacterTemplates table cannot be replaced under live actors`. Pre-existing (the Swamp container-summon spawn table is built again on reload). Blocks an in-process content reload on the Swamp; not caused by this work.

## Gaps (explicit)

1. **Door collision not implemented and not verified.** No door physical body in the EXE; the dead-end bit (`PFFloor+32` bit 1) is set in IDA but has no reader in the port, and I did not find an IDA reader of that bit in the PF functions checked. The hero's walk-through test was not run. Next step: find the readers of `PFFloor+32` (e.g. the path/route search) and bind `FlagFloorAsDeadEnd` to the port's `floors::Record::source_flags32`.
2. **Fresh-process restart does not restore door state.** The GameSave is read only by F9. A startup "continue" that loads the GameSave is a separate feature (level transition or save streams); I did not change the startup path.
3. **Campaign trigger and lifecycle state are not persisted as components.** The save is accepted because the idle lifecycle is reconstructible and the checkpoint rules were corrected, not because a lifecycle component was written. A lifecycle that has transitioned since load still refuses the save. Trigger edge state (`TriggerZoneSet::inside`, the campaign runtime's trigger edges) is not saved; no replay was observed in-process, but a fresh process would reset it (not verified).
4. **Live spawn on a flip not implemented.** Source does not do it (see Investigation). The port's population is evaluated at load and at R reload; R reload is blocked on the Swamp by the CharacterTemplates error (see Integrated runtime verification). The merchant-camp NPC check after the witch kill is inconclusive (restart population count 35 both before and after the flip; I did not add a per-actor log).
5. **Door and lever sound ordinals assumed identity.** The uri names look right (`sfx_giant_wood_door_open.wav` for the wooden door, `sfx_magical_wall_down.wav` for the fire door, `sfx_orb_deactivate.wav` for the lever), but the mapping from Arrays sound ids to the audio table index was not checked against IDA. Close cues are not exercised.
6. **Quest event id placeholder.** `QE_TriggerOn` carries the lever's stable id; the source id (`this+25`) is not decoded. The event is raised but matches no objective (`applied=0`).
7. **Glow**: the purple glow is attributed to the fire door by its disappearance on opening; the emitter and FX-glow channels are skipped, so the glow's source is not confirmed.
8. **Waterfall**: no Swamp waterfall; the Darkwood Wormroot Hollow waterfall (part 2, 909 s) is the only waterfall seen. No state change was observed around the witch kill in the sampled frames (1-4 s steps); animation cannot be judged from stills. The Swamp outdoor segments of part 1 were not sampled in detail.
9. **Not re-verified**: ctest after the final rename (the rename is covered by the EXE runs, not by a unit test).

## Placeholders

- `QE_TriggerOn` object id = lever stable id (`useLeverObject`, labelled PLACEHOLDER in code).
- Condition overrides in the door refresh (`--condition-active/--condition-inactive`): test aid, not gameplay.
- `--lever-use`: debug aid, default off.

## Package files required

- Everything used is in the rc3 package: `assets/original-cache/data/pydata/v2quests_pyarray.bin` (+ names), `assets/data/game_objects_pyarray.bin` and related tables, `assets/data/3d/gameobjects/go_swamp_door_ruins.bdae`, `go_swamp_door_ruins_fire.bdae`, `go_swamp_witch_lever_lockable.bdae`, `assets/data/3d/textures/pvr2_env_swamp_alpha.tga`, `audio-assets/sounds_pyarray.bin` and `audio-assets/data/sounds/*.wav`.
- The audio paths must be absolute (`--audio-assets`, `--audio-table`); relative paths resolve against the run folder and the audio start fails with `Cannot read original audio table`.

## Verifier script

1. Build and test: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p17doors2 -Jobs 6`; then run ctest with the toolchain on PATH (`llvm-mingw-20261006-ucrt-x86_64/bin` first in PATH): expect exactly `session_skill_binding` and `character_menu` to fail.
2. Save and reload with audio: `quiet_run.ps1 -JobsFile doors2-run/jobs-audio.json -Parallel 1`, then grep `Restored live checkpoint frame=220`, `Door view _prim_SwampDoor state=1`, `banners suppressed at restore`.
3. Visual: `quiet_run.ps1 -JobsFile doors2-run/jobs-visual.json -Parallel 2`; compare `cap/closed-240.png` and `cap/open-240.png` (door panel top centre).
4. Lever: `quiet_run.ps1 -JobsFile doors2-run/jobs-lever2.json -Parallel 2` (lever forced true and locked control); grep `Lever use declaration=`, `QE_TriggerOn raised`.
5. Unit: `channel_filter_tests.exe` from the build directory.
