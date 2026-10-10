# P16 MAPFIX report (worktree DH_wt/p16mapfix, branch p16/mapfix, started from p16/integrate 2d703432)

Scope: the character-menu Map page after the merge. Goals: (1) the grey bar and the Quest/Map hit overlap,
(2) MapName and the "Unexplored area" caption, (3) zoom and pan input with Reset zoom, (4) markers beyond the
player, (5) camera pose from minimapcameras.bdae. Not pushed. Quest files untouched (QUESTUI2 owns them).

## Commits
- `a6b06e39` tab overlap, MapName, legend arrow caption, authored map camera, PC zoom/pan.
- `35c0dc66` enemy markers, legend hides the level name, tab overlap test.
- this report (follow-up commit).

## (1) Grey bar and tab overlap
- Grey bar = the authored MapName plate, not a stray fill. In `original_art.cpp` art4 the batch
  `menu_MapSheet/13` (sheet x 101.4..386.6, y 52.65..78.45, UV in the sheet atlas) is the plate; the text field
  `menu_MapSheet/MapName` (617, box x 105..381, y 55..75) sits on it. The port drew the plate but never the name,
  so the plate looked unexplained. With the name drawn (goal 2) the plate reads as the title band. The reference
  (Part 2, t=90 and 165, full-res frames) shows the same band behind "GOTHICUS DARKWOOD".
- Tab overlap: hit rectangles are 63.4 px wide on a 52 px pitch (Quest Log 369.1..432.4, Map 420.9..484.3), so
  the overlap is 11.4 px. Decision: where several tab zones contain the point, the tab whose centre is nearest
  answers (split at x 426.7). The shared table `original_menu_hit_zones()` is unchanged; only `hit_test` resolves
  the overlap. Both tabs stay reachable: unit test checks exclusive parts (380, 470) and both sides of the overlap
  (424 -> Quest, 430 -> Map). The IDA z-order of the two buttons was not decoded (open).

## (2) MapName and the legend caption
- IDA `MenuCharMenu_Map::ShowLevelName` (0x45335c): `Application::GetCurrentLevel()+60` = LevelList row, the
  name StrID is row word +36 (= word 9; the row's trailing words are LevelName, LevelState, MapName, monster levels
  in sorted schema order, `game-data/level_tables.cpp`), then `StringManager::getString(id)`. The port resolves the
  StrID with `MenuLocalization::string_id` (HudTextV1 integer lookup, same as the other numeric menu text).
  Result on Swamp: "The Boglands" (row 41; the same name as the fresh-profile label in menu_metadata_v1.hpp).
  The name is hidden while the legend popup is open (its own title takes the plate).
- "Unexplored area" caption: the menu text table has no `MENU_*` symbol for it. Source found in
  `gameplaymenus.english` (string 530 "Unexplored Area", after 529 "Zone") with symbol `GAMEPLAYMENUS_LEGEND_ARROW`
  (gameplaymenus.symbols entry 445, next to GAMEPLAYMENUS_QUEST_ZONE 444). Layout: the legend's Arrow icon (type
  13) is the right column row 4 (placed at y 196.75), which is text field iconText10. Mapped index 10 to that symbol.
  Verified by render: the arrow row reads "Unexplored Area" (out/legend.png). Other legend captions unchanged.

## (3) Zoom and pan (PC adaptation, labelled in code and here)
- The original is touch: `ZoomHandler::onEvent` (0x382040): pinch changes CameraLevel +136 (zoom), drag changes the
  pan offsets +152/+156 (x 50 per unit), and `ZoomHandler::ResetZoom` (0x38201c) resets +136/+140.
- PC: mouse wheel (new `Window::take_wheel_notches`, WM_MOUSEWHEEL accumulated, remainder kept) zooms one step per
  notch while the cursor is over the map; left-button drag inside the RenderMap rectangle pans by the cursor
  movement; keys: +/- (and numpad) zoom in steps, arrows pan, Home resets. Reset zoom button resets zoom and pan.
  Zoom range 1..4 is the PC adaptation (the original limits come from DesignSettings, not applied here).
- Pan is clamped to the visited extent as UpdateMapCamera does (eye XY inside the visited zones' bounds padded by
  their own width/height). Zoom scales the eye distance to the target.
- Verified: unit tests for pan direction, zoom distance, clamp, reset (map_visit). In the EXE: the `--map-zoom 2.5`
  frame (out/zoom.png) is zoomed on the player. The wheel/drag/key glue was not exercised in the EXE (quiet runs
  inject no input): open.

## (4) Markers
Implemented (general, from the live owners):
- Family 3, local player (Character icon). Existing.
- Family 4, enemies: `combatSession->world()->actors()`, live actors that the world's eligible-target rule marks
  hostile to the player, inside a visited zone (IDA ShowNpcIcons: `IsMonster` and `IsInsideRooms(pos, visited)`).
  Verified: the nine-room fixture (`out/f9.png`) shows one enemy marker; log `Map enemy markers frame=150 drawn=1`.
Not implemented (gaps, with the IDA rules for the next owner):
- NPC quest giver (10) and merchant (11): ShowNpcIcons: non-monster with +762 set and +763 clear -> 10; `Character::IsMerchant` -> 11.
  The port has no per-actor quest-giver or merchant flag in ActorState.
- Entrance (1), exit (2), checkpoint (12): ShowMapExitsIcons on level objects with type codes 13, 14, 12 (object +244),
  active byte +138, and for type 13 the object's level id must equal the current level (+884 vs Level+272).
  The port does not map those numeric codes to its class names (CheckpointZone / TriggerZoneExitLevel).
- Room exit arrows (type 2 -> icon 16 at +x, type 3 -> 17 at -x, type 1 -> 15 at -y, other -> 14 at +y; all drawn with
  frame 13 "Arrow" in DuplicateIcon): ShowRoomExitsIcons draws an exit point inside a
  visited room whose point 3000 units along its direction is inside an unvisited room. The producer of
  `dword_9A275C` is not decoded.
- Objectives (0): ShowObjectivesIcons uses the current quest's objective positions (Vector3DFList from each objective).
  The quest runtime tracks counters but exposes no positions.
- Family 7-9 (other players): single-player only; not produced.
- Placeholders drawn: none. Every drawn icon is the authored MapIconsDynamic frame.

## (5) Camera pose from minimapcameras.bdae
- `features/map_visit/map_camera_pose_v1.*`: loads the BRES with `dh2::camera::GameplayCameraSceneV3` (the same
  scene reader the gameplay camera uses), selects node `PlayerCamera_Default_minimap`, and stores eye and target as
  offsets from the scene root (the root is the player anchor). Authored values: eye offset (0, 0, 23100), target
  offset (0, 0, -3204.78): straight down, 26.3k units from the target.
- `map_camera_v1` places the offsets on the local player (CameraTarget follows the target), up (-1, 1, 0),
  FOV 0.42963 rad, aspect 1.5 (CreateMapCamera constants). The computed fit of the earlier stream is removed.
- Asset: `data/3D/camera/minimapcameras.bdae` is not in the rc3 package. The loader reads the asset root first and
  then `--map-camera-root` (here `.local-inputs/assets-extra/ios`). Log: `Map camera pose eye=... target=...`.
- Orientation: the up vector is the IDA constant. Screen right = (1,1,0)/sqrt2, screen up = (-1,1,0)/sqrt2; a square
  room therefore appears as a diamond (seen in out/fresh.png). Not verified against an original frame: the only
  reference map frames are Gothicus Darkwood (Part 2), and the in-game Darkwood level stops in the levels
  stream (`Level::_LoadFromXML` LoadTemplate assertion, see MAP-report gap 8). The Darkwood frame is not matched.
- Scale: at zoom 1 the authored view shows the one-room start (6000 units square, rotated 45 degrees) at about 420 of
  the 514 map pixels in height. The reference Darkwood frame (t=90) shows its visited area at about 40% of the map
  width, and the visited sets are different, so the scale is not confirmed.

## Evidence (quiet hidden runs, build DH_wt/build-p16mapfix)
- Jobs: `.local-inputs/claude-preview16/mapfix/` (`jobs.json`: fresh, legend, zoom; `jobs-f9.json`: nine rooms).
  All exit 0, about 6 s each.
- `out/fresh.png`: "The Boglands" on the plate, parchment, one visited room (dark diamond), player marker.
- `out/legend.png`: legend with all icons and captions incl. "Unexplored Area"; title "Legend" only.
- `out/zoom.png`: `--map-zoom 2.5`, zoomed on the player.
- `out/f9.png`: nine visited rooms, one enemy marker (red), player marker.
- Comparisons: `compare-map-ref90-vs-ours-f9.png` (reference Darkwood t=90 vs ours), `compare-legend-ref165-vs-ours.png`.
  Reference frames: `p16mapfix-scratch/ref_*.png`, `legend_t*.png`, contact sheet `sheet_150_174.png`.

## Tests
- ctest in DH_wt/build-p16mapfix: 129/130 pass; the only failure is `session_skill_binding` (known, ignored per brief).
- map_visit (camera pose math, visited extent and padding, pan, zoom, clamp, reset, projection, unloaded pose error),
  level_module_zones (overview frame now uses a synthetic pose capped below the 100000 far plane), character_menu
  (tab overlap, legend and reset controls). PASS.
- Not added: a test for `load_map_camera_pose_v1` on the real bdae (no test asset path in the suite).

## Placeholders
None.

## Package files required
- `data/3D/camera/minimapcameras.bdae` in the asset package (copy of `.local-inputs/assets-extra/ios/data/3d/camera/`
  or the android copy), or pass `--map-camera-root` to a root containing it.
- Swamp rc3 assets and the Darkwood test assets as in MAP-report.

## Verifier script
- Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16mapfix -Jobs 6 -Test`
  (the helper `p16mapfix-scratch/build.sh` writes the full log when the script hides errors).
- Runs: `port/windows-foundation/tools/quiet_run.ps1 -JobsFile .local-inputs/claude-preview16/mapfix/jobs.json -Parallel 3`
  and `-JobsFile .../mapfix/jobs-f9.json`.
- Expected: exit 0 for all; logs `Map camera pose eye=0,-0.000994,23100 target=0,0.00014,-3204.78`,
  `Map room visited frame=0 ... total=1`, `Map enemy markers frame=150 drawn=0` (fresh) and `drawn=1` (f9),
  `zoom=2.5` (zoom job).

## Open risks and gaps
1. Markers: NPC quest giver/merchant, entrance/exit/checkpoint, room exit arrows, objectives (see section 4).
2. Camera orientation and scale against an original frame: not verified (Darkwood not loadable in game).
3. Input glue (wheel, drag, keys) only unit-tested for the math; not driven in the EXE.
4. Tab overlap rule is a nearest-centre decision; IDA z-order of the two buttons not decoded.
5. Parchment colour: our sepia parchment is warmer than the reference frame (video grading or a different tint: unverified).
6. Visited geometry colour (0.23, 0.25, 0.27) is an estimate; per-edge clipping and the shape-600 ornaments are not replicated.
7. Clamp with nothing visited falls back to every zone (IDA would degenerate to zero).
8. Level name verified only for Swamp ("The Boglands"); Darkwood's name is unverified in game.
9. Shared tool drift (export_hud_geometry.py, export_art.py) from MAP-report gap 9 is unchanged.
