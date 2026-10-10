# MAP survey (HUD minimap + character-menu Map page), Preview 14

Survey only. No file under port/, docs/ or tools/ was edited. Scratch lives in
`.local-inputs/claude-preview14/map/` (frames, strips, one quiet EXE run).

Bottom line:
- The original shipped v1.0.2/1.0.3 build does not show a HUD minimap during play. The HUD
  target movie `menu_miniMap` is absent from every shipped SWF, and no sampled frame of
  either reference video shows one. The HUD minimap is therefore a user-requested feature,
  not a faithful reproduction. This needs a user decision (section G).
- The character-menu Map page exists only as source adapters (features/map, features/map_ui).
  It is not integrated: the Character menu has no Map tab (`Tab {stats,equipment,skills,faery}`),
  main.cpp has no hook, and no live Level/RoomZone owner feeds it.
- Visited/fog state is not persisted anywhere in the live save. GameSave v1-3 has no room
  or module field. The original stores one visited byte per Module (Module::visited3fc).

---------------------------------------------------------------------------------------------
## A. EXISTING

| Item | Location | Purpose | Status |
|---|---|---|---|
| Map page presenter/provider | `port/windows-foundation/features/map/runtime_source_map_menu_provider_v1.*`, `runtime_source_map_page_v1.*` | Fail-closed presenter for MenuCharMenu_Map Show/RenderMap; requires live Level, RoomZone, camera, player, save, quest, event owners | Source-only plus component tests. Not in main.cpp (grep: no hit). |
| RoomZone adapter (canonical) | `features/map/runtime_source_map_room_zones_v1.*`, `features/map_ui/source_map_room_zone_v1.*` | Wraps canonical room factory records; calls source_has_been_visited_v104 and source_has_inside_v104 | Source/component tests. Not integrated. |
| Decoded SWAMP room zones | `features/map_ui/source_map_decoded_room_zone_v1.*` | Builds RoomZone candidate AABBs from decoded module BRES mesh bounds; nine Swamp modules; visitation = nullopt (no source reader) | Compile + linked decode test pass (B023 row). Not registered. |
| Map kernels | `features/map_ui/source_map_kernel_v1.*`, `source_map_camera_v1.*`, `map_ui.*` | Show bounds, camera XY clamp, CameraBase::GetScreenCoord projection, rect projection, SetData sequence | Standalone C++17 tests pass (per JSON). Not integrated. |
| Actor markers | `features/map/runtime_source_map_actor_markers_v1.*` | Marker producers (family 3 = local player positions) | Tests exist; family 3 only. |
| Integration JSON | `port/windows-foundation/reports/act1-map-native-integration.json` | Status: `source_adapter_partial_live_integration_blocked`, goal_complete=false, runtime_readiness.ready=false, original_runtime_capture=not_available, pixel_acceptance=pending | Authoritative status record. |
| Tracker rows | `docs/BUGS-AND-IMPLEMENTATION.md` line 96 (B023), line 139 (I005) | B023 = minimap/map page lacks markers; I005 = complete Map Sheet production page | Open. Next step per I005: main registration plus live Level+36 membership, visited byte, camera planes, remaining marker providers. |
| Character menu tabs | `features/character_menu/character_menu.hpp` (`enum class Tab {stats,equipment,skills,faery}`), `character_menu.cpp` line 19 (rejects other tabs), `Action` enum | Only four tabs | Map (and Quest) tab absent in the EXE. |
| HUD | `features/generic_skills/pc_gameplay_hud_*` (dqhud_droid art) | Portrait, bars, skill slots, potion, faery | Verified in EXE (quiet run, section B.5). No minimap anywhere in windows-foundation code (only `original_scene.cpp` role-name list containing "_minimap"). |
| Level/module floors | `port/windows-foundation/source_module_floors.*`, main.cpp ~1298-1362 | Navigation floor/room registry from decoded modules | Exists; carries no visited state. |
| Canonical room/module source | `port/level-world/canonical_room_zone_v3.*`, `canonical_module_graph_v3.cpp`, `canonical_level_config_module_v1.hpp` (`visited3fc_`) | Source-level RoomZone visit logic (mirrors RoomZone::Update), module visited byte | Source-only; not wired into the EXE. |
| Non-character save connection | `port/level-loader/noncharacter_save_connection_v89.hpp` (`module_save_fields_v91`: writes Module visited3fc + zone400) | Serializes module visited byte | Compiled into level-loader tests only; not used by GameSave. |

Verified in the EXE:
- Quiet run (section B.5): `.local-inputs/windows-source-clock-v19-preview-13-rc2/dh-foundation.exe`
  with the Preview 13 swamp args (assets pointed at rc2 assets, fresh save folder
  `.local-inputs/claude-preview14/map/run-swamp/`), 200 frames, exit 0, hidden desktop.
  Capture `run-swamp/swamp.png`: portrait, two bars, skill slots 1-5, "5 Potion: 0". No minimap.
  No map-related log lines.

---------------------------------------------------------------------------------------------
## B. ORIGINAL BEHAVIOUR

Evidence sources: IDA pseudocode `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`
(line numbers below), per-function files under `pseudocode/0041/` and `pseudocode/0045/`, and
`elf-symbols.jsonl`. Video: Part 1 (Swamp, 1331 s) and Part 2 (Gothicus Darkwood, 916 s) at
`C:/Users/adamc/Desktop/dh2_video_research/video/` (part1_z_Zky7qQdYs.mp4 = the reference-video
path in COMMON-BRIEF; part2_cIAW38IfLxY.mp4).

### B.1 HUD minimap (class HUDMinimap; MenuMinimap is its menu-manager wrapper)

- Menu name: `HUDMinimap::GetInstance` (0x41cce0) constructs `HUDMinimap("menu_miniMap")`.
  That target is not present in any shipped SWF. Searched all `original-cache/data/menus/*.swf`:
  `dqhud_droid.swf` and `dqcharmenu_droid.swf` contain no `miniMap` name. Only the char-menu
  SWF has `NativeShowMinimapLegend`. Both HUD and char-menu SWFs contain `RenderMap` strings,
  but `RenderMap` is in dqcharmenu only (not in dqhud_droid).
- Gate `HUDMinimap::Update` (0x41c7b0, pseudocode 205127): runs only if DebugSwitch
  `IsDisablingMinimap` is false AND `Application::GetCurrentLevel()+304 == 38` AND map camera
  `this+224` is non-null AND shown flag `this+196` is set. Level+304 == 38 is the same test used
  by `Application::IsLevelLoaded` (0x31f5b4, pseudocode 17084): it is a loaded/state value,
  not a level ID (inference; the Swamp's value is unverified).
- `HUDMinimap::Show` (0x41c9e4, pseudocode 205191): refreshes the cached SWF character "RenderMap"
  from the menu context, sets `this+196=1`, sets camera target to the local player (Character +408),
  then calls the virtuals that populate icons and visibility.
- Caller: `MenuMinimap::Show` (0x435308, pseudocode 220885) calls `HUDMinimap::Show`, then
  `ZoomHandler::setCamera(dword_99F77C, this+56)` and sets zoom flag +36=1. `MenuMinimap` is the
  registered menu. Who calls `MenuMinimap::Show` during play was not traced (open).
- Map camera `HUDMinimap::CreateMapCamera` (0x41be88, pseudocode 204737): CameraLevel loaded from
  `data/3D/camera/CameraTests.bdae`, anim set `"MiniMap"`, clip `"PlayerCamera_Default"`;
  byte133=1, word136=0, float140=1.0, damping off,
  `CameraBase::SetData(FOV 0.42963, aspect 1.0, near 0, far 100000)`, then PlayAnim.
  `CameraTests.bdae` is in preview-13 packages (`.local-inputs/claude-preview13/h/pkg/assets/data/3d/camera/`).
- Render: `HUDMinimap::RenderMap` (0x41c4a0, pseudocode 205027), registered as RenderFX display callback
  "RenderMap" (`RegisterDisplayCallback` 0x41c058). It reads the absolute bounding rect of the SWF
  character "RenderMap", converts it to pixels with the root's inverse pixel scale, sets the viewport
  to that rect, switches the scene camera to the map camera (`this+224`), calls the scene's
  render/draw virtuals and restores the game camera. So the HUD minimap is a top-down re-render of
  the live level geometry through a second camera. It is not a bitmap.
- Visibility: `HUDMinimap::SetRoomVisibility` (0x41d384, pseudocode 205558): rebuilds a list of
  visited RoomZones from the Level RoomZone list (`dword_99F764+36`), keeping `HasBeenVisited` ones
  (or all if DebugSwitch `isUsingMapHack`). For each scene child in list `a2+244`, it sets
  visibility from `IsInsideVisitedRooms(absolute position)`.
  `IsInsideVisitedRooms` (0x41b868) returns true if any visited RoomZone `HasInside` the point.
  `SetObjectsVisibility` (0x41b8b4) does the same for object lists at +988 (count +212) and +1020
  (count +208), writing object byte +155.
- Projection: `HUDMinimap::GetMapScreenCoord` (0x41b790): `CameraBase::GetScreenCoord(camera)` gives
  (x, y) in [-1,1]; result = (cx + x*w*0.5, cy - y*h*0.5) with w/h/center from the rect. The Y
  inversion is from the sign-flip in the decompile (inference from `COERCE_FLOAT(v10+0x80000000)`).
- Icons: `UpdateIconPositions` (0x41b9bc, pseudocode 204528). Player icons (count +200, from
  `PlayerManager::GetPlayer(i)`, Character +1632, position +88..+90), then four more families with
  count fields +216 / +208 / +212 / +204 (arrays at +211, +127, +151, +175). Their semantic names
  are not decoded (open). Only the player family is clearly identified.
- Not decoded: `SetIconFrames` (0x41cff4), `InitAllIcons` (0x41b568), `HideAllIcons` (0x41b628).

### B.2 Character-menu Map page (MenuCharMenu_Map)

- `MenuCharMenu_Map::Show` (0x455138, pseudocode 0045/00455138.c): registers RenderMap; calls
  ShowPlayersIcons, ShowNpcIcons, ShowMapExitsIcons, ShowRoomExitsIcons, ShowObjectivesIcons,
  ShowLevelName. Icon families (18 slots) and producer predicates are in
  `reports/act1-map-native-integration.json` (`source_marker_families`).
- `MenuCharMenu_Map::CreateMapCamera` (0x45351c, pseudocode 0045/0045351c.c): CameraLevel from
  `data/3D/camera/minimapcameras.bdae`, anim set `"Default"`, clip `"PlayerCamera_Default_minimap"`;
  byte133=1, float140=1, word136=0, damping off,
  `CameraBase::SetData(FOV 0.42963, aspect 1.5, near 0, far 100000)`, up vector via vtable+276
  set to (-1, 1, 0), SetActive, PlayAnim, SetTarget(local player).
  Note: the Map camera uses aspect 1.5 and the file `minimapcameras.bdae`. The HUD minimap uses
  aspect 1.0 and `CameraTests.bdae`. `minimapcameras.bdae` was NOT found anywhere in the tree.
- `MenuCharMenu_Map::GetMapScreenCoord` (0x45389c) and `RenderMap` (0x454108): projection from the
  map camera, as in the JSON.

### B.3 Visited / fog rule (RoomZone)

- `RoomZone::Update` (0x396e9c, pseudocode-all.c line 111784): each frame, the zone is tested
  against 6 camera planes (from the level camera's vtable+324). It is activated or deactivated
  based on that test. Only when every plane test passes (zone in frustum) does it reach the visit
  check. If the zone is not yet visited and the local player's Character position (+408 -> +352..+360)
  is inside (`RoomZone::HasInside`), it calls `RoomZone::SetVisited(1)`.
  Inference: the frustum gate is part of the visit condition. Verified by control-flow reading only.
- `RoomZone::HasBeenVisited` 0x3964a8, `SetVisited` 0x3964bc, `HasInside` 0x3964cc (inclusive XY per
  decoded note in the JSON). Visited byte is `Module::visited3fc` (per Module, not per map cell).
- Port: `canonical_room_zone_v3.cpp` lines 102-139 reproduce this logic and the Module byte.

### B.4 Map art / assets

- `menus/dqcharmenu_droid.swf`: `menu_MapSheet` sprite 655, `RenderMap` character 601 (authored
  matrix in JSON), `MapName` 617, icon containers 614/615, legend and reset-zoom buttons 623.
  This is the Map page art. Not a per-level bitmap.
- `menus/dqhud_droid.swf`: no minimap character. Contains `btnMapICon`, `MapButtons`, `WholeMap`,
  `ZoneName`, `LevelNameInMap`, `EntryPointIdInMap`, `NativeGetWorldMapLocations`, `menu_MapSheet`,
  and string references to `map_top.tga` / `map_bottom.tga`. These are world-map / travel-map
  pieces (inference).
- `data/3d/textures/map_top.tga` (131 KB) and `map_bottom.tga` (524 KB): BTEX-wrapped (`BTEX`/`pvr`
  header), not plain TGA. Dimensions were not decoded. Purpose unverified (likely WholeMap).
- No per-level minimap texture was found in the cache. HUD minimap geometry comes from live level
  modules. Scene nodes with role `_minimap` exist (original_scene.cpp helper role list). IDA strings
  `Darkwood_Minimap` and `Tut_Minimap` exist, but their use was not traced (open).
- Zone names: `text/locations.english` (e.g. "Gothicus Darkwood", "Bogwitch Cavern",
  "The Boglands - To Bogwitch Cavern").

### B.5 Video observations (what was actually seen)

Method: 10-second contact sheets over Part 1 (0-1331 s, all 5 sheets) and Part 2 (0-916 s,
all 4 sheets), plus 3-second and 2-second strips at Part 2 600-660 s and 150-165 s, plus
full-resolution frames. Timestamps below are sampled, so +/- 3-10 s. Sheet-based timestamps
proved slightly off in one case (270 s), so treat each as approximate. Files:
`.local-inputs/claude-preview14/map/p1/sheet_*.png`, `.../p2/sheet_*.png`,
`.../p2mapf/grid.png`, `.../p2mapf/strip600.png`, `.../full/*.png`.

Part 1 (Swamp / Boglands / Bogwitch Cavern, 22 min):
- HUD observed in every gameplay sample: portrait top-left, HP/MP bars, potion orb and a small
  round icon top-right, attack and skill buttons bottom. No minimap in any corner.
  Full-resolution check at 735 s (Bogwitch Cavern, "Bogwomp" fight): no minimap.
- No Map page seen in any Part 1 sample (no map at 0-1331 s at 10 s steps).
- A loading card at about 1290-1331 s reads (small text, low confidence on exact wording):
  "If you do not know where to go, open your Quest Log. You can also use returning to previously
  visited areas." This is the only in-video hint about visited areas.

Part 2 (Gothicus Darkwood, 15 min):
- Map page (title "Gothicus Darkwood", "MAP" tab, buttons "Show legend" and "Reset zoom") visible at
  about 60-65, 90-95, 160-164, 270, 510, 612, 633, 654 and 840 s. Each open lasted a few seconds
  (3 s strip at 600-660 s showed open/close cycles).
- Two visual states, observed:
  (a) about 60-91 s: pale sepia parchment with terrain drawing, no markers, title visible. At 61 s,
      a top overlay reads "UNEXPLORED AREA" with "Merchant" in the upper-left (overlay meaning unverified).
  (b) about 160 s onward: dark slate map with lighter room outlines and markers. Inference: revealed
      (visited) areas are drawn dark and unvisited stay parchment, but this was not confirmed.
- Markers seen (observation): blue pin/ring (likely local player, family 3), red dots (hostile
  monsters, likely family 4), green crosses (likely quest objectives or NPCs, families 0/10
  inferred), orange arch icon at bottom edge (likely exit/gate, families 1/2/14-17 inferred).
- Labels: the Part 2 HUD shows the top-centre place name during play (e.g. "Corrupted Woods").
- Part 2 gameplay HUD matches Part 1 (no minimap).

B.5 EXE capture (current build): see section A (`run-swamp/swamp.png`). The EXE HUD has the same
layout as the reference HUD, minus the top-right potion orb, and has no minimap.

---------------------------------------------------------------------------------------------
## C. GAPS (each with evidence)

1. HUD minimap absent from the EXE: no code in windows-foundation (grep "minimap": only
   `original_scene.cpp` role names); quiet capture `run-swamp/swamp.png` shows none.
2. Original HUD minimap target `menu_miniMap` absent from shipped SWFs. The port's runtime log
   shows "Original SWF core diagnostic: can't find target menu_miniMap"
   (`.local-inputs/dh2-after-singleplayer-logcat.txt`, 10-08 12:42:09, tag DH2Native).
   Consistent with the video showing no minimap. Not confirmed on the original device.
3. Character menu has no Map tab or action: `character_menu.hpp` `Tab`/`Action` enums; the menu
   rejects non-stats/equipment/skills/faery tabs (character_menu.cpp line 19).
4. Map page not integrated: no `map`/`MapSheet`/`source_map` reference in `main.cpp`; the
   integration JSON lists "Show/RenderMap not connected to live Level"
   (`act1-map-native-integration.json`, runtime_readiness.ready=false).
5. No live Level+36 RoomZone list or Level+244 object walk exposed to the Map code (JSON
   `still_requires_actual_owners`). The EXE does have a navigation room registry
   (`source_module_floors`, main.cpp ~1354) but it has no visited byte.
6. Visited state not persisted: `game_save.hpp` GameSave v1-3 has `version`, `character`,
   `level_uri`, `controlled_actor_id`, `random`, `actors`, `objects`, physical presence. No room,
   module or visited field. `campaign_save/FORMAT-MIGRATION.md` is a proposal only.
   Original: Module visited3fc is saved through the non-character save
   (`noncharacter_save_connection_v89.hpp`, module_save_fields_v91), which the EXE does not use.
7. Camera asset missing: `data/3D/camera/minimapcameras.bdae` not found anywhere in the tree.
   `CameraTests.bdae` is present in preview packages but the "MiniMap" anim-set and
   "PlayerCamera_Default" clip lookups were not verified (`Arrays::CamAnimSetTable`).
8. Marker families beyond the local player are unproduced in the EXE (families 0,1,2,4,7,10-17).
9. Level+304 == 38 gate: meaning of the value for the Swamp is unverified.
10. Map page verification: no original-runtime capture of the Map page on Swamp exists (JSON:
    `original_runtime_capture=not_available`). The only original Map frames are Part 2
    (Gothicus Darkwood), not the Swamp.

---------------------------------------------------------------------------------------------
## D. DESIGN (minimal reusable)

Shared data model (new, feature-owned, no map-specific code):
- `ModuleVisitState`: map from authored module id (the same id the decoded modules and
  `CanonicalModuleV1` use) to a 1-byte visited flag. Stored per level URI. Read/written by
  feature code, never by CharacterState.
- `RoomZoneRuntime` (per live Level): list of zones (AABB from module BRES bounds, inclusive-XY
  HasInside), each bound to a module id. Updated each frame with the frustum-gated visit rule
  (B.3), using the local player's Character position.
- `MapPageModel` (features/map_ui): consumes RoomZoneRuntime, camera matrices, marker producers;
  produces show bounds, projected markers and visibility. Reuse the existing kernels
  (source_map_kernel_v1, source_map_camera_v1) and runtime_source_map_page_v1 provider.

Owners read/written:
- Reads: CharacterState (local player position via PlayerManager/Character), GameSave (visited
  state, level_uri), PlayableActorWorld (actors for monster/NPC markers, later), quest owner
  (objective markers, later).
- Writes: visited bytes (on zone visit); GameSave via a new optional section.

New files (proposed):
- `port/windows-foundation/features/map_visit/visited_state_v1.{hpp,cpp}` (ModuleVisitState codec
  and per-frame update).
- Wiring for the Map page inside `features/character_menu` (new Tab/Action) and a presenter using
  `runtime_source_map_menu_provider_v1`.
- Optional HUD minimap second pass (only if the user confirms, section G).

Integration hooks (main.cpp; each hunk should be small):
- Character menu input/tab switch: near the `Action` dispatch in `character_menu.cpp` line ~66
  (`case Action::stats` ... `case Action::faery`). Add `Action::map` and `Tab::map`. Main-side
  hook is the char-menu draw call in main.cpp (search `character menu` / `CharacterMenu`).
- Visited update: once per frame in the level update, after player movement (main.cpp, near the
  SourceModuleFloors loop at ~1354). Use the same module ids.
- Save: `save_game`/`load_game` through game_save.cpp; `capture_game_save`/`restore_game_save`
  call sites in main.cpp (search `capture_game_save`, `restore_game_save`).
- CMakeLists.txt: add the new sources to the windows-foundation target (shared file; minimal diff).

Save-format implications:
- Add a visited section as an extension of GameSave (version 4 or an appended optional section,
  consistent with how Version2/Version3 were appended). Old saves (v1-3) load with all modules
  unvisited. This matches the original, where a fresh Module starts unvisited (inference:
  initial visited3fc value is 0 at construction; not verified in IDA).
- Never change the byte layout of existing fields. Reject duplicate or unknown module ids.
  Unknown trailing data fails before publication.

---------------------------------------------------------------------------------------------
## E. WORK BREAKDOWN (dependency order)

1. (S, under 2 h) Visited persistence codec. `visited_state_v1` with encode/decode, GameSave
   optional section, load of old saves as unvisited. Tests: round trip; v1-v3 load with empty visited;
   duplicate id rejected; unknown id rejected; trailing bytes rejected. Verifier: unit test runner
   plus quiet save/load of a copied swamp save.
2. (M, under half day) Visit update in the live level. RoomZone runtime from Swamp module bounds
   (nine decoded modules, already tested in `source_map_decoded_room_zone_v1`); frustum-gated visit
   rule; local player inside test. Tests: entering a zone sets its byte; a zone outside the frustum
   does not; re-entering keeps it set; saved and restored. Verifier: quiet run of Swamp with a
   scripted walk, then compare saves (`--game-save`, copied save folder).
3. (M) Map page in the character menu. New Tab/Action; presenter uses sprite 655 from
   dqcharmenu_droid; live projection with the Map camera (requires `minimapcameras.bdae`, see G);
   Show/Hide; legend and reset-zoom buttons. Family 3 (player) markers only, plus visited/unvisited
   reveal. Tests: presenter readiness fails closed without owners; projection of the player position
   inside the rect; reset-zoom restores. Verifier: quiet capture with the Map tab open on Swamp,
   compared with the Part 2 Map frames for layout and button placement.
4. (M/L) Remaining markers: monsters (4), NPC (10/11), objectives (0, from quest owner), exits
   (14-17 with visited test). Depends on quests and actor registry streams. Verifier: quiet capture
   with actors present.
5. (L, only if user confirms) HUD minimap second-pass render through the "MiniMap" camera and a
   viewport rect; player icon only. Verifier: quiet capture showing the minimap rect; compare with
   the absence of a minimap in the video, so this must be an explicit user-requested addition.

---------------------------------------------------------------------------------------------
## F. DEPENDENCIES / CONFLICTS

- Quests: objective markers (family 0 / 10) need the quest owner; Map page and quest log share
  `Quest Log` loading hint and quest state.
- Main-menu metadata: current map/act display can use the zone name from `text/locations.english`
  (no conflict, read-only).
- Stats/skills, faery, equipment: all touch the character menu Tab/Action enums and the draw path.
  Adding `Tab::map` is a shared-file edit in `character_menu.hpp/.cpp` and main.cpp. Serialize with
  those streams.
- Drops: no map dependency.
- Save format: visited section touches `game_save.hpp/.cpp`, shared with all other save extensions
  (skills, faery, quests, drops). Coordinate a single version bump or an appended optional section.
- main.cpp is shared with every stream; keep hunks small (per COMMON-BRIEF rules).

---------------------------------------------------------------------------------------------
## G. OPEN QUESTIONS FOR THE USER

1. HUD minimap: the original build shows none in play (video, and the target movie `menu_miniMap`
   is missing). Should Preview 14 add a HUD minimap as a new feature (section E.5), or keep the HUD
   faithful (no minimap) and deliver only the Map page?
2. Map page scope for the Swamp: confirm families for the first cut (player only, or player +
   monsters + exits).
3. Visited persistence: is an appended optional save section (old saves load as unvisited) acceptable?
4. Level target: the reference Map frames are from Gothicus Darkwood (Part 2). The Swamp (Part 1,
   Boglands/Bogwitch) shows no Map page in sampled frames. Is the Swamp (`data/scene/001_swamp.mlx`)
   the required acceptance level, or should Gothicus Darkwood be used for the Map visuals?
5. Asset: `data/3D/camera/minimapcameras.bdae` is missing from every tree. Should the implementer
   extract it from the original data archive (which owner/tool?), or is the Map camera to be replaced
   with a documented fallback?
6. Original-device confirmation: does the original Android build show a HUD minimap at all? The
   survey relies on the video and the missing SWF target, not a device capture.
