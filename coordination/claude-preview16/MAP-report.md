# P16 MAP report (stream map, branch p16/map, worktree DH_wt/p16map)

Scope delivered: the character-menu Map page (6th tab, menu_MapSheet sprite 655) for any level:
top-down visited-level render through the RenderMap rectangle, RoomZone visit tracker driven by the
current level, schema v4 visited persistence, drawn parchment and icons, legend, reset zoom, tests.
No HUD minimap (original has none in play). Not pushed.

## Commits (branch p16/map, on top of b05ae173)
- beee1bfa visit tracker (features/map_visit), module room bounds in the decoder, Map camera model, ctest map_visit
- 67b74bae Map tab: export_art (art4, art4_legend, btnMapTab/Show legend/Reset zoom hit zones), Tab::map, Action map*, composition slot 4
- f1949812 main: level loader feeds the tracker; per-frame visit update; Map draw; --map-page-frame
- c17e6bbf legend at settled frame, captions, Show legend caption; diag --map-legend, --map-zoom
- 5431db74 drawn parchment (menus/map_bottom.tga), per-type map and legend icons from the authored sheet

## What is implemented
- Visit rule (features/map_visit/room_zone_visit_v1.*): canonical RoomZone::Update mirror
  (canonical_room_zone_v3.cpp source_update_v104): six camera planes from the gameplay camera, most-inside
  corner test; active and not yet visited and player XY inside (inclusive) => visited, once.
- Zones of the CURRENT level (features/map_visit, assembled_level.hpp load_level_with_module_zones): one per
  visible unconditional manifest Module; id = placement index among all Module declarations; bounds = the
  module's `_module_` room box (new decoder output OriginalScene::moduleMinimum/Maximum), falling back to
  visible geometry; draw ranges recorded per module.
- Persistence: CharacterState::visited_modules via menu_metadata::set_visited_module (schema v4, keyed by
  level_uri = options.level). Old saves: no entries = unvisited.
- Map camera (map_page_v1): CreateMapCamera constants (FOV 0.42963 rad, aspect 1.5, up (-1,1,0)). Reset view
  fits the level footprint along the screen diagonals and centres on the level; zoom centres on the player.
- Drawing (main.cpp, character-menu draw block): parchment quad over the RenderMap rectangle; visited zones drawn
  with renderer.drawRange through renderer.withViewport (dark slate, vertex colour retained); map icons from the
  authored MapIconsDynamic frames (player = family 3 Character, icon 3); legend icons at their authored positions.
- Controls: Show legend toggles the authored LegendPopup (settled frame 13); Reset zoom request; both only on
  the Map tab (hit_test filters them elsewhere). Back arrow and tab icon 6 are the existing menu art.

## Rule compliance: missing art drawn from code, no placeholders
- Parchment: the sheet's bitmap fill (shape 600 style 0 = bitmap id 2) is menus/map_bottom.tga (ExportAssets tag 56
  maps id 2 to it; BTEX PVRTC4 1024x1024, decoded by texture_loader). Region = the bbox of that fill's edges
  (13.6,48.35 - 472.9,277.25 px); UV from the fill matrix 26.6095 twips/texel, offset (-3599,-3417) twips.
- Icons: DuplicateIcon (0045/00454504.c) sets frame = icon type on the MapIconsDynamic clip; labels
  (sprite 614): Objective 0, Entrance 1, Exit 2, Character 3, Enemies 4, Champion 5, Boss 6, Player2..4 7..9,
  QuestGiver 10, Merchant 11, Checkpoint 12, Arrow 13. ShowPlayersIcons (00454a08.c): local player = type 3.
- Legend icons: the twelve MapIconsDynamicK placements of the legend, each showing the type of its caption.
- Placeholders drawn: NONE. The first blue circle marker was removed when the icon was drawn.

## Evidence (quiet hidden-desktop batches, build DH_wt/build-p16map/dh-foundation.exe, Swamp rc3 assets)
Captures: `.local-inputs/claude-preview16/map/job-*/map.png` (copies in `.../map/evidence/`).
- job-fresh (rc3 profile, 0 visited): log `Map room visited frame=0 ... modules=0 total=1` (start module),
  parchment with one dark diamond and the Character icon. Matches the reference early state (t=60 s dark start blob).
- job-f5 / job-f9 (fixtures: 5 and 9 visited ids written into the v4 tail): log `visited=5` / `visited=9` at
  load (round trip). f9: all nine Swamp rooms dark slate, player icon. Reference later state: dark slate rooms
  with lighter outlines on parchment; qualitative match.
- job-f5-legend (--map-legend): legend with all authored icons (Checkpoint, Entrance, Exit, NPC, Merchant, Character,
  Enemy, Objective, arrow), captions, Legend title, Show legend / Reset zoom.
- job-f9-zoom (--map-zoom 2.5): zoomed on the player (diag shows eye at the player XY).
- Persistence: `swamp-fresh/run-save.log` (--save-frame 120): `Saved live checkpoint`; the saved character.save
  tail = visited_count 1, `data/scene/001_swamp.mlx`, module 0, visited 1.
- Reference frames extracted: `.local-inputs/claude-preview16/map/ref/` (Part 2 t=55-70, 150-170, 265-280,
  505-520, 607-622, 830-845 contact strips; states: parchment-with-dark-blob early, dark slate later, legend at t~160-170).

## Tests (ctest in DH_wt/build-p16map)
- map_visit (new): frustum planes (inside/outside/near/far/side/boundary), HasInside inclusive XY, tracker
  (visit once, re-entry, outside frustum, no player, duplicate/inverted rejects, saved ids restored, unknown
  ignored), persistence per level, map extent/zoom clamp/reset camera/projection. PASS.
- level_module_zones (new, args: Swamp rc3 assets + scratch Darkwood root): Swamp 9 zones / 392 ranges; Gothicus
  Darkwood (003_darkwood.mlx, 24 placements of darkwood2.bdae) 24 zones / 936 ranges; every zone inside the reset
  view; ranges contiguous and covering the scene. PASS.
- character_menu (extended): Map tab selection, Map hit contour, Show legend toggles and survives tab switches,
  Reset zoom request consumed once, controls inert outside Map. PASS.
- schema_v4, menu_metadata_v1, game_save*, combat_session*: PASS. Full ctest: 115/116 pass; the failing
  session_skill_binding is the known worktree junction issue (brief: ignore).

## Package files required
- Swamp: `.local-inputs/windows-source-clock-v19-preview-15-rc3/assets` (001_swamp.mlx, data/3D/Modules/swamp,
  data/3D/textures/map_bottom.tga, MenusGraphics_droid.tga, dqcharmenu_droid.swf).
- Level-2 unit test: `.local-inputs/claude-preview16/map/assets-darkwood/` = copy of 003_darkwood.mlx (from the Android
  device folder `files/data/scene/`) and `data/3D/Modules/darkwood/darkwood2.bdae` (copied from the rc3 package).
  Copies, not junctions: AssetCatalog rejects paths that resolve outside its root.
- The minimap camera asset `minimapcameras.bdae` is NOT read by this stream (see open risks).

## Verifier script
Jobs file: `.local-inputs/claude-preview16/map/jobs-batch1.json` (5 jobs, `quiet_run.ps1 -Parallel 5`), each job
`--startup-config <job>/run.args` with `--frames 170 --map-page-frame 150 --capture map.ppm`.
Expected: exit 0 for all five; fresh log `Map room visited frame=0 ... total=1`; f5 `visited=5`; f9 `visited=9`;
f5-legend shows the legend (log `Map legend trace` was removed; check the image); f9-zoom zoomed on the marker.

## Open risks and gaps (not done)
1. Level title (MapName, e.g. "Gothicus Darkwood") is not drawn: needs the LevelList/location name lookup.
2. "Unexplored area" legend caption: no symbol in the menu text table; the arrow icon has no caption.
3. Zoom input: no mouse-wheel or pinch in the port; zoom is only reachable through the Reset zoom button and the
   --map-zoom diagnostic. Zoomed view centres on the player; manual pan is not implemented (inference).
4. Markers: only family 3 (local player). Enemies (4), other players (7-9), NPCs (10/11), exits, objectives are not
   produced (the icon art exists; the producers are not wired to the live actor/quest owners).
5. Camera: the map camera uses the CreateMapCamera constants with a computed top-down pose; the authored pose in
   minimapcameras.bdae (anim Default, clip PlayerCamera_Default_minimap) is not loaded. Up vector (-1,1,0) is the
   source value; its orientation against the original Swamp map is unverified (the reference frames are Darkwood).
6. Visit bounds: module `_module_` room boxes from the module BDAE; the original's RoomZone box (BRES room bounds)
   is not cross-checked against an original capture. Conditional (quest) modules are not tracked (not loaded).
7. The parchment is one quad over the RenderMap rectangle with the fill-region bbox; the ornaments of shape 600
   (bitmap-1 styles) are not drawn, and no per-edge clipping is replicated. Slate colour (0.23,0.25,0.27) and icon
   scale 1.55 are estimates from the reference frames and the authored sheet, not measured.
8. Level 2 in the game: `003_darkwood.mlx` loads (40142 triangles, 936 ranges) but the EXE stops on the diagnostic
   source-constructor path (`Level::_LoadFromXML service: required original LoadTemplate assertion(9) provider`),
   so no in-game Darkwood Map capture was taken. Owned by the levels stream (p16levels).
9. Export tool drift: `tools/export_hud_geometry.py` changed after fb35cf1f; the current version rejects the
   character-menu shape 600 and changes the output (180 KB instead of 656 KB). `features/character_menu/export_art.py`
   therefore has to run against the historical tool (fb35cf1f copy of export_hud_geometry.py). Not fixed here (shared
   file). Owner should decide.
10. Visit timing uses the gameplay camera from the active frame; the frame ordering vs the original RoomZone::Update
    call site is inferred from the canonical port, not captured.
