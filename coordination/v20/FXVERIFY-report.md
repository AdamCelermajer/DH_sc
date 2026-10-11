# I028 FX verification report (v20fxverify)

Branch `v20/fxverify`, worktree `DH_wt/v20fxverify`, base `v20/integrate` (107f2a86).
Commits: `25680042` (DH_FX_DEBUG per-cue logging, no behaviour change), `148c3b29` (branch tests).

## Frozen runtime build

- EXE: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v20/fxverify/dh-foundation-fxverify.exe`
- SHA256: `8FFB1F376BF114F3D08D1C46717610A516EE1B596DA62F0D4566F2B4F735B65F` (19,065,856 bytes)
- Built from the worktree with `p14_build.ps1 -Name v20fxverify -Jobs 3`: baseline build exit 0; incremental build after logging exit 0; incremental build after test-only commit exit 0. The test-only rebuild left the EXE hash unchanged.
- Asset root: `.local-inputs/v20/asset-stage/assets`.

## ctest (final build, `ctest --test-dir build-v20fxverify --timeout 120 -j 4`)

- 160 of 161 passed; 1 failed: `session_skill_binding` ("Asset path escapes asset root"). This is the known worktree-only failure from the brief: the test points at `${CMAKE_CURRENT_SOURCE_DIR}/../..`.
- Directly run: `source_script_fx_adapter_v1_tests` PASS (exit 0); `campaign_host_tests` "campaign_host tests passed" (exit 0, includes the new `play_effect_branches`).

## Integrated runtime runs (all via `port/windows-foundation/tools/quiet_run.ps1`, hidden desktop, DH_AUDIO_SILENT=1, fresh folders with no prior save, `DH_FX_DEBUG=1`)

### A. Normal opening: `.local-inputs/v20/fxverify/normal-opening-debug/`

- Compiled bank, `--start-mode swamp --level data/scene/001_swamp.mlx --campaign-triggers`, no XML campaign-command, no `--source-root-scopes`, no `--condition-active`. 4,900 frames. `--capture-frames` covers 60–4900 and the windows around the cues (2599–2602, 2603–2670 every 10, 2671–2680 every frame, then 4-frame steps to 2809, plus 2900, 3300, 3900, 4500, 4900).
- Exit code 0 (summary.json), "Rendered frames=4900; clean shutdown", `Source FX final packetFrames=4900 packets=21216 textureUploads=10 presentationFailed=0`. An earlier identical run without the debug variable (folder `normal-opening/`) also reported clean shutdown; its summary was not written because of a job-file formatting error on my side.
- Timing: `Swamp_Intro` index 101 (set 119) at frame 2601 and index 107 (set 120) at frame 2670. The earlier run had 2593 and 2661, so the cue frames vary by about 8 frames between runs. Index 112 (`idle_opened`, PlayAnimByName) at frame 2792 (earlier run: 2788).

Per-cue log (`[fx-debug]`, all `play=ok`, all `pool=cold`, `step=0`, zero local offset):

| source index | frame | set | target (module) | anchor handle | resource (dictionary path) |
|---|---|---|---|---|---|
| 28 | 3 | 116 | `_prim_Waypoint_5` (module -1 in the call; resolved module 0) | 0x8000000000000001 (level-object tag, generation 2) | `data/3D/interface/cs_swamp_intro_01_bloodsplat_prisoner.bdae` |
| 33 | 242 | 117 | `_prim_Waypoint_5` | same | `.../cs_swamp_intro_02_bloodsplat_prisoner.bdae` |
| 73 | 1701 | 118 | `_prim_Waypoint_5` | same | `.../cs_swamp_intro_09_rezeffect.bdae` |
| 101 | 2601 | **119** | `_prim_Waypoint_5` | same | `.../cs_swamp_intro_13_refletdelumiere.bdae` |
| 107 | 2670 | **120** | `_prim_Waypoint_5` | same | `.../cs_swamp_intro_14_explodedoor.bdae` |

The `_prim_Waypoint_5` declaration is a Dummy at position (0,0,0) in `obj_4of4_brdwalk_sw_00.mgp` (staged asset `data/3d/modules/swamp/mgp/`), and the module is at world origin, so the cue sits on the cage model's origin as the source requires. Set 132 is not played in the opening; it is covered by run B.

### B. Witch Door_Teleport: `.local-inputs/v20/fxverify/witch-door-debug/{open,close}/`

- Same compiled-bank arguments as `v20/integration/i028-compiled-bank-replay-6559/jobs.json`, re-pointed to fresh folders, frozen EXE above, 120 frames. Both exit 0.
- **open** (BlueDoor state 1): no `Door_Teleport` start, no set 132 call. Suppression confirmed.
- **close**: this uses the diagnostic setup `--campaign-command Intro_Witch:63:0`, which runs the actual compiled row 63 (kind 55) through the typed executor. It is not a normal Intro_Witch traversal. The zone then starts `Door_Teleport` through the ordinary updater.

Log order in close: `[door] ... state=1->2`, trigger started, index 0 kind 20 (set 132, target `Player`, actor handle `0x4000000000000025`, generation 2, `fx_airtemple_teleportshockwave.bdae`, pool=cold, play=ok), index 1 kind 13 PlaySound (unbound in this build, sound 604 not played), index 3 kind 46 SetActorPosition, index 5 kind 20 (set 132 again, same handle, play=ok), script finished. Both cues are in campaign frame 2 (Wait0 is non-blocking), so the log cannot show a temporal gap between them.

## Visual verification (frames inspected)

Reference frames were extracted with ffmpeg from `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`. P1 610–645 s at 1 fps (36 frames, `reference/p1-001..036.png`), with contact sheets `sheetA_612-623`, `sheetB_624-635`, `sheetC_636-645`; full frame `p1-031` (640 s). Entrance check `sheetD_entrance_718-729`.

Normal run frames inspected (PNG from captures): 2580, 2599–2602, 2610–2670 (sheet A), 2671–2680, 2683, 2687 (sheet B), 2691–2735 (sheet C), 2739–2783 (sheet D), 2787–2809, 2900, 3300, 3900, 4500, and full frame `png_f2699`. Witch frames: close f1, f2, f3, f5, f10, f20; open f2, f3, f10, f20, f60, f120.

Observations with honest confidence (direct observation vs inference):

- **Orange gate (P1 612–623, Celeste dialogue, orange fire wall in a corridor)**: direct observation in P1. Not present in our opening frames 2580–2670 (Rene close-up in a dark corridor with a chest). It is a different door family (fire door, documented known issue). Not compared further.
- **Blue-white effect (P1 624–645, Trembling Prisoner dialogue)**: direct observation in P1. A diffuse blue-white glow at the foot of a wooden-plank wall beside a green pedestal (floor glyph tiles), lasting continuously across 624–645. Our run's frames 2599–2809 do not show that composition. The run's cage is barred wooden cages over teal water, with a captive Prince, and the Trembling Prisoner captions do not appear in those frames (captions appear only at the 3300 "Rene" line). Conclusion: not reproduced in timing or composition; inference that the P1 scene is a different moment or scene. Not established.
- **Cage reflection (set 119, frame 2601)**: logged as played; visually not isolated. The frames 2599–2602 are a close-up of the hero in a dark corridor in which I see no clear new light. Frame 2699 shows a pale vertical shaft and a diffuse teal ray over the central cage, which could be this cue or scene lighting. Unresolved.
- **Explosion (set 120, frame 2670)**: at 2670 the camera cuts to the cage. Frames 2671–2680 show a white winged faery glow and a yellow wedge at the cage foot; a blue-white lightning bolt (Rene thunder, audio event at 2673) appears at 2683–2695. No clear burst at the cage door is visible in the captured frames. The cage lock is still visible at 2699. Unresolved whether the faery wedge is set 120 or another effect.
- **Set 132 (Witch Door_Teleport, close run)**: a cyan-white shaft appears at f3, becomes a large expanding shell at f10, and a larger shell at f20, centred on the player. The open run shows nothing. Suppression by door state 1 is confirmed visually.
- **Set 132 vs P1 entrance**: P1 at 718–729 s shows a gold/white light column around the player after the "Bogwitch Cavern - Entrance" loading transition. It is gold/white and tied to the level arrival; our cue is cyan-white and tied to the in-room relocation. The colour and moment do not match, so I do not claim the P1 entrance column is set 132. Possible causes: v1.0.3 vs v1.0.2 asset difference, a separate cue, or lighting tint. Unknown.

## Defect check and fixes

- No defect found in the effects layer for these cues: anchor (level-object and LocalPlayer actor handles, generation 2), resource (per source row 116/117/118/119/120/132), position (zero offset), and play result all match the source table and the authored commands. No fix was applied.
- Observation, not changed: `Impl::play_set_v118` (`character_mesh_fx_sets_v118.inc`) returns success silently for a set ID outside EffectsTables, and when the active pool holds more than 5 instances (`current.size()>5`). The adapter's out-of-range rejection test therefore describes a callback that the production manager does not implement. Changing this to a failure could break callers that pass no set on purpose (for example set -1 in combat hit paths), so it is flagged for the owner, not fixed here. The new DH_FX_DEBUG line reports both branches.

## Changes in this session

1. `features/campaign_host/campaign_host.cpp` case 20 (Script_PlayEffect): `[fx-debug] PlayEffect` line (set, target, module, position, anchor, skip, result, error) under DH_FX_DEBUG.
2. `features/effects/source_fx_anchor_registry_v1.cpp`: `[fx-debug] anchor resolve|actor` lines (name, module, kind, generation, handle).
3. `level-world/character_mesh_fx_sets_v118.inc` and `character_mesh_fx_owner_v4.cpp`: `[fx-debug]` lines for resolved resource, cold/warm, instance, play result, and the three silent branches. Includes added.
4. `features/campaign_host/campaign_host_tests.cpp`: `play_effect_branches` covers the placed target (two cues, no dedupe, forwarded set/anchor/offset), unplaced target (no play), resolver failure (error propagates, no play), play failure (listed, command continues), and unbound resolver (fails closed). `Rig` takes an optional resolver; default unchanged.
5. `features/effects/source_script_fx_adapter_v1_tests.cpp`: skip wins over an absent position.

main.cpp and CMakeLists were not edited.

## Remaining gaps

- Visual identity of sets 119 and 120 is not established. The cues play and reach the right anchor, but the frames do not isolate their appearance.
- The P1 blue-white prisoner effect (624–645 s) is not reproduced in the opening run. The P1 Trembling Prisoner scene and its timing are not located in our frames; a longer capture of the cage scene at 2900–3300 or a different route would be needed.
- The P1 gold/white entrance column is not matched to set 132.
- Set 132 timing: both cues fire in the same campaign frame, so the temporal separation (Wait0) is not observed in runtime.
- Normal Intro_Witch admission into `_prim_TriggerZone_doorport` (closed-door traversal from outside the room) is still not exercised; the close run uses the diagnostic command.
- The silent set/pool no-op branches are flagged above, not fixed.
- Sound 604 (Script_PlaySound) remains unbound in this build.
