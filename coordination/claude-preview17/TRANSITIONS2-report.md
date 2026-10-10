# TRANSITIONS2 report (Preview 17): exit prompt, per-level world, entry record, loading art, checkpoint

Branch `p17/transitions2` (worktree `DH_wt/p17transitions2`, build `DH_wt/build-p17transitions2`), started from `p17/transitions` 6647a745. Not pushed. Commits: aaa5f6b1, fee21269, 1db151a1 (B055 loading art cherry-picked from `fix/b055`), b31b004f (prompt, entry record, per-level snapshot), 18af0933 (restore sequence, equipment alignment, localized prompt text), 7ce10209 (checkpoint snapshot log).

## Result in one paragraph

A level exit now shows the travel prompt in the EXE (`Travel to:` + the localized level name, placeholder panel) while the player stands in an armed exit, and E confirms it. `--transition-confirm` stays as the test hook and `--transition-key FRAME` is the scripted E press. On a transition the level's world (its `GameSave` snapshot) is written next to the profile and restored on the next arrival at that level, with the player placed at the arrival entry. The entry id is recorded on the character (a named component in the schema v4 tail; no bump). A process restart from a transition save arrives at the recorded entry. The original B055 loading art (ornate frame, LOADING plate, red bar) is now on transitions. Verified in quiet runs: Swamp to Darkwood to Swamp round trip with E confirms, restart from the save, the procedural-exit negative, and the loading frame. ctest: 132/133, the one failure is the allowed `session_skill_binding`. Still open: the prompt art is a placeholder (HUDART owns the travel panel), monsters and containers are not in the combat world on this branch so world-state carry is only proven for the player, checkpoints are saved but not resumed on restart, and the condition evaluator is still the stub.

## Investigation (IDA `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`, HUD `dqhud_droid.swf`, text tables)

- `TriggerZoneExitLevel::Update` 0039c33c: while the local host touches the zone and `GameObject::MeetCondition` holds, it calls the HUD root's `DisplayFastTravel(true, displayName, levelName, entryId)`: `displayName` is the LevelList display text (`LevelList+36` string id through StringManager), `levelName` the authored `levelName`, `entryId` the authored `entrypointID`. Leaving calls `DisplayFastTravel(false, ...)`. The zone itself does not change level.
- HUD movie (`dqhud_droid.swf`, strings): `DisplayFastTravel`, `FastTravelVisibility`, `btn_FastTravel`, `LevelName`, `EntryPointId`, `GAMEPLAYMENUS_FASTTRAVEL`. The button calls the native `NativeGoToZone(levelName, entryId)` (IDA 004422b8), which saves all players and calls `Application::LoadLevel`.
- Text (`ingame.english`): INGAME_GO_TO_LEVEL = `Travel to:`; INGAME_GO_TO_WORLDMAP = `Exit level and go to world map?` (map exit, not used by the zone prompt).
- Display names resolved through the menu text owner (`MenuLocalization::string_id`) from `LevelList+36`: DARKWOOD shows `Gothicus Darkwood`, SWAMP shows `The Boglands `.
- Authored zone (`bossroom_ruins_ns_.mgp`): `_prim_ExitLevelZone_001`, `levelName=DARKWOOD`, `entrypointID=0`, `activate_cond=IsAfter_Swamp_Escape`, `fasttravel=a01_SWAMP_CAMP`. No `question` property on this zone.
- Video (Part 1 `dh2_video_research/video/part1_z_Zky7qQdYs.mp4`): black-frame detection finds four short fades (0-2 s, 8.9 s, 37.7 s, 75.2 s); the LOADING screen appears at about 66-80 s (B055) and again near 1045-1050 s. The prompt itself was not located in footage (frames at 1043 s and the 10 s and 1 s contact sheets show no prompt). The prompt art and the exact confirm button are therefore NOT verified against footage; the text, show/hide rule and confirm are from IDA.

## Implementation

1. `features/transitions/transition_prompt_v1.{hpp,cpp}` (new): `find_armed_exit` (built level_exit, player inside, condition true, not procedural) and `build_prompt_frame` (placeholder panel, `Travel to:`, display name, `[E]` hint). Stage units 480x320, drawn like the cinematic boxes through the frontend text owner.
2. `main.cpp` (P17 TRANSITIONS2 anchors):
   - Prompt show/hide edges (`Transition prompt shown|hidden`), E edge confirm (`Transition confirmed key=E`), guarded by pause and character menu. The E edge state starts from the held state at session start so a key held across a load does not confirm.
   - Draw: after the cinematic layer, before markers.
   - Level world: `saveLevelWorld` (`GameSave` snapshot to `<save stem>.world-<level stem>.gsave`) on transition leave and checkpoint. On arrival (after `initializeCombat`): the controlled actor is placed at the arrival entry, its equipment slot names are aligned to the live actor by instance id, the session is detached (`detach_for_restore`), the snapshot is restored, the session rebound (`rebind_after_restore`), and the faery cooldown lease refreshed.
   - Entry: the character records `(level, entry)` on transition (`set_entry_component`); a checkpoint clears it; a process restart whose save records the current level arrives at that entry (`Entry restored from save`).
   - Refusal diagnostics print both equipment lists when a restore is refused.
3. `character_state.{hpp,cpp}`, `save_store.cpp`: `CharacterState::components` (id + bytes, limits 64 and 4 KiB), written only when non-empty as an append-only tail of schema v4 (`w.integer` count, string id, size, bytes). A v4 save without components has the exact previous layout (`schema_v4` test passes unchanged). Reader accepts the tail when bytes remain.
4. `features/transitions/transition_persistence_v1.{hpp,cpp}` (new): `entry_component` (id `dh2.transition.entry.v1`, u32 entry + level URI), `level_world_path`.
5. `level_transitions_v1.{hpp,cpp}`: `LevelRow::display_string_id` (`scalar.words[9]` = LevelList+36).
6. B055 loading art: `fix/b055` commits 78a5830c, 0b9087ca, a7835c03, 510d4cc9 cherry-picked (clean; `merge-tree` and cherry-pick both without conflict). The generated `loading_art_data.cpp` is committed, so no Python exporter at build time.
7. CMake: `# P17 TRANSITIONS2` block (two sources in foundation_data). Tests: `level_transitions` extended (prompt selection, display id, gated and procedural exits, entry record round trip through `save_character`/`load_character`, per-level path).
8. Test hooks: `--transition-key FRAME` (scripted E press, labelled test aid); `--transition-confirm` unchanged.

## Isolated tests

- `level_transitions` PASS (prompt, entry record, save round trip, path).
- Full ctest (`build-p17transitions2`, `-j 6`): 132/133. Only `session_skill_binding` fails (expected). `schema_v4` passes unchanged.

## Integrated runtime verification (quiet hidden runs through `port/windows-foundation/tools/quiet_run.ps1`, `-Parallel 1`, exit codes in each `summary.json`)

Run dirs: `.local-inputs/claude-preview17/transitions2/run{1..5}`.

Run 1 (`run1/round.log`, exit 0): Swamp to Darkwood to Swamp.
- `Transition prompt shown zone=_prim_ExitLevelZone_001 target=DARKWOOD display="Gothicus Darkwood" entry=0 frame=41`; `Transition confirmed key=E ... frame=60`.
- `Level world saved reason=transition level=data/scene/001_swamp.mlx`; `Transition arrival entry=0 spawn=_prim_EntryPoint_from_swamp02 ... heading=-2.96706`; `Level world restore status=none` (first visit).
- Darkwood: `Transition prompt shown zone=_prim_ExitLevelZone_to_swamp ... display="The Boglands " entry=13 frame=61`. This shows on the arrival frame because the arrival point is already inside that zone (touch rule); not verified against footage.
- `Transition confirmed key=E ... entry=13 frame=130`; `Transition arrival entry=13 spawn=_prim_EntryPoint_from_darkwood02 ... position=-19363.9,11187.8,-109.679`; `Level world restored level=data/scene/001_swamp.mlx`; final position -19363.9,11187.8,-113.968 (the entry).
- Captures (PNG in `run1/`): `s1-swamp-armed-f58.png` (the New Quest banner is on screen at the start of the Swamp session; the prompt is below it), `s2-darkwood-armed-f118.png` (`Travel to: The Boglands [E]`, legible, clear of the HUD row), `s2-darkwood-arrival-f75.png`, `s3-swamp-return-f150.png`.

Run 2 (`run2/restart.log`, exit 0): process restart from the Swamp save written by run 1, without `--fresh-player`.
- `Entry restored from save level=data/scene/001_swamp.mlx entry=13`; `Transition arrival entry=13 ...`; `Level world restored level=data/scene/001_swamp.mlx`; final position is the entry. `Rendered frames=60; clean shutdown`. (Before the detach/rebind fix this run exited with an access violation at shutdown; fixed in 18af0933.)

Run 3 (`run3/negative.log`, exit 0): checkpoint and procedural negative.
- `Checkpoint zone=_prim_CheckpointZone_01 ... saved frame=22`; `Level world saved reason=checkpoint`; the snapshot holds the checkpoint position (x=-6059.93896 at byte 2739, y=3206.26, z=255, found by a byte scan; aligned 4-byte scans miss it because the record is unaligned).
- The witch-cave exit (`SWAMP_CAVE_WITCH_A`, procedural) shows no prompt and ignores the scripted E press at 45; `--transition-confirm 50` gives `Transition refused ... reason=procedural level has no EXE path yet`.

Run 4 (`run4/loading.log`, exit 0): `load-tr-040.png` shows the B055 loading screen (ornate frame, LOADING plate, tip text, red bar) during the Swamp to Darkwood transition.

Run 5 (`run5/lag.log`, exit 0): F5 live save at frame 40 after the same test-aid placement: `live.gsave` holds the exact position (-6059.94, 3206.26, 255) at offset 2772; the checkpoint snapshot holds the same position (above).

## Gaps (explicit)

1. Prompt art: placeholder panel (`PLACEHOLDER` in `transition_prompt_v1.cpp`). The original travel panel is in `dqhud_droid.swf` (`FastTravelVisibility`, `btn_FastTravel`); not exported (HUDART owns the HUD panels; that branch is not in this worktree). The prompt position is a guess (lower centre, clear of the banner); not verified against footage. The `Exit level and go to world map?` map prompt is not built.
2. Confirm key: E is a PC adaptation (the original is a touch button). The Space context button (P16) is not used here; the two must be reconciled at integration.
3. Per-level world: the snapshot is written on leave and checkpoint and restored on arrival (verified: the Swamp return restores; the controlled actor is at the entry). What it carries is limited on this branch: the combat world holds only the player (`actors=1`, `objects=0`), because the population's 33 visuals are not combat actors and the container declarations are 0 (P16 containers not merged). Killed monsters, opened containers and picked drops are therefore NOT proven to carry. Ground drops (`WorldDropRuntime`) are not in the snapshot.
4. Entry record: written on transition, cleared on checkpoint, applied on process restart. Whether the original restarts at the entry or at the saved position on a plain continue is not verified; the record only changes a restart from a transition save.
5. Checkpoint restore: the checkpoint snapshot is written and its position verified, but a process restart does not resume at the checkpoint (the restart path only uses the entry record; F9 uses the live save). The original resumes at `SG_SaveCheckpoint` position; not wired.
6. Arrival facing: the heading from the SpawnPoint rotation is passed to the motor (`heading=-2.96706` Darkwood entry 0, `-1.5708` Swamp entry 13). On-screen facing not compared with footage.
7. Arrival invulnerability: none found in the IDA paths (`vincib` matches a trophy name only). The port applies none. Not verified against footage.
8. Arrival HUD flow: the HUD is visible at the arrival frame (captures). The original `menu_Loading`/`menu_HUD_0` sequence beyond the loading screen is not replicated. The loading screen shows only for transitions.
9. Prompt on arrival: the prompt shows at the arrival frame when the arrival point is inside the return exit (Darkwood entry 0 vs `_prim_ExitLevelZone_to_swamp`). Matches the touch rule in IDA; footage not checked.
10. Equipment slot names: the fresh start and a loaded character name equipment slots differently (`slot-N` vs `source-type/N`). The arrival aligns the snapshot's slots to the live actor by instance id. Root cause (two producers) not fixed.
11. Conditions: `LoggedConditionStub` (`IsAfter_Swamp_Escape` treated active). CONDITIONS replaces it.
12. Procedural targets: refused (no EXE path). Unchanged.
13. Quest banner, act rows, difficulty across transitions, RNG reseed on unload: unchanged from TRANSITIONS (see its gaps 12-17).

## Placeholders (labelled in code)

- Prompt panel and text layout (`transition_prompt_v1.cpp`, `PLACEHOLDER` comment; font face 5 as the cinematic caption).
- E confirm (PC adaptation; `--transition-key` test hook).
- Condition stub (unchanged).

## Package files required

- None new. Level data from the existing overlay `.local-inputs/claude-preview16/levels-root`; the Android scene files in `.local-inputs/assets-extra/android/data/scene/`.
- The B055 art data is committed in `loading_art_data.cpp`.

## Verifier script

1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p17transitions2 -Jobs 6`.
2. ctest (toolchain on PATH): 132/133, only `session_skill_binding` failing.
3. Quiet runs (`tools/quiet_run.ps1 -JobsFile <job> -Parallel 1`): `run1/jobs.json` (round trip), then `run2/jobs.json` (restart), `run3/jobs.json` (checkpoint and negative), `run4/jobs.json` (loading frame). All exit 0.
4. Look at `run1/s2-darkwood-armed-f118.png` (prompt text) and `run4/load-tr-040.png` (loading art).

## Tracker (docs/BUGS-AND-IMPLEMENTATION.md, not edited here)

- I021: the confirm path is now the E key (normal path) with the scripted hook for tests; world state and entry record are persisted; still open: the travel panel art (HUDART), monsters/containers in the combat world (P16 merge), checkpoint restart, conditions.
- I022: not addressed by this stream.
