# P16 MAPMARKERS report (worktree DH_wt/p16mapmarkers, branch p16/mapmarkers, started from p16/integrate 261f2e49)

Scope: the Map page marker families MAPFIX left open: NPC quest giver (10), merchant (11), entrance (1), exit (2),
checkpoint (12), room exit arrows (13..17), quest objectives (0). Not pushed. Commits on p16/mapmarkers:
`67c41ca6` (module, rules, tests), `dfefedd8` (main.cpp integration), `7602dc97` (character_menu test), `a744ec8e`
(live activation refresh; entrance entry rule is in dfefedd8 and the entry diagnostics in the same branch).

## Status by family (what is verified, what is not)
| family | producer (IDA) | implemented | live verification in EXE (Swamp) |
|---|---|---|---|
| 4 Enemies | ShowNpcIcons monster branch | yes (combat owner, visited) | yes: f9 frame, one enemy at the projected pixel of its world position |
| 11 Merchant | ShowNpcIcons -> IsMerchant (AI type 7) | yes | yes: `cluster` frame, merchant icon drawn at its position (1 of 1) |
| 12 Checkpoint | ShowMapExitsIcons (type 12) | yes | yes: `cluster` frame, 3 ornate checkpoint frames drawn (4th outside the frame) |
| 2 Exit | ShowMapExitsIcons (type 14, active) | yes | yes: `exit` frame, exit arch drawn at its position (1 of 1) |
| 1 Entrance | ShowMapExitsIcons (type 13, entrypointID == Level+272, active) | yes (rule) | NO: the Swamp start spawn is `activate_cond="Invalid"`, so no entrance is produced in any fixture |
| 10 Quest giver | ShowNpcIcons (+762 && !+763) | yes (rule; +763 not modelled) | NO: no talk objective is active in the fixtures (see gaps) |
| 0 Objective | ShowObjectivesIcons -> Objective_MoveInZone::GetPositions | yes (current quest MoveInZone zones) | NO: the current quest (row 50) has only a kill objective |
| 13..17 Room exits | ShowRoomExitsIcons | NOT produced (no writer found, see investigation) | n/a |
| 3 Character, 5..9 | ShowPlayersIcons (local), other players | local player only (existing) | yes (existing) |

Unit tests cover every rule and provider (`map_markers` test), so the NO rows are verified at the rule level only.

## Investigation (IDA `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
Producers, called by `MenuCharMenu_Map::Show` (0x455138) after `ClearAllIcons`, in this order: ShowPlayersIcons
(0x454a08), ShowNpcIcons (0x454fb8), ShowMapExitsIcons (0x454d5c), ShowRoomExitsIcons (0x454b8c),
ShowObjectivesIcons (0x4548ac), ShowLevelName (0x45335c). Each marker = `DuplicateIcon(type, pos)` (frame = type).

- ShowNpcIcons: walks the live character list (`dword_99F764+96`). Requires visible (`VisualObject::IsVisible`), +128
  alive, and `IsInsideRooms(pos, visited=1)`. Then: `Character::IsMonster` -> 4; `+762 && !+763` -> 10 (QuestGiver);
  else `Character::IsMerchant` -> 11. `IsMerchant` = `GetCharType()==7`, and `GetCharType` = CharAI row `type`
  (the port's `ai_props(...)->type`, which `level-world` also reads as 7 = merchant, 8 = cleaner).
- `+762` is set to 1 by `Objective_TalkToNPC::Register` (0x47db34) and by `InstallObjectiveMarker` (0x47d8e8) for the
  character whose CharacterTable row equals the objective oid (`Character::SafeGetCharPropsId`); cleared by
  `Unregister` (0x47dbc4) and `RemoveObjectiveMarker` (0x47da6c). `InstallObjectiveMarker(a2,a3)` also sets `+763 = (a2 == 3)`.
- Installation: `Quest::Compile` (0x480178) calls `ObjectiveList::Register` and `InstallObjectiveMarkers(list,
  *(quest+26)+276, state)` only when the quest state is 6 = `active` (v2QuestState: 0 locked, 1 post_locked,
  2 pre_available, 3 available, 4 post_available, 5 pre_active, 6 active). The `a2` argument is `*(quest+26)+276`, not
  decoded.
- ShowMapExitsIcons: walks the level object list (`dword_99F764+20`); object type word = obj[61], and the GO_IDS come from
  the constructors (`CheckpointZone(v,12)`, `SpawnPoint(v,13)`, `TriggerZoneExitLevel(v,14)`, see GetNewInstance).
  - 13 (SpawnPoint, Entrance icon 1): `obj[221] == Level[68]` and active byte `obj[138]`, inside a visited room.
    `obj[221]` is the SpawnPoint property `entrypointID` (`SpawnPoint::DeclareProperties` 0x3ea554, offset 884), and
    `Level+272` is the current entry: `Level::SG_SavePlayer` and the spawn placement store it with
    `Character::SG_SetLevelEntryPoint` (pseudocode-all.c lines 174258 and 174630).
  - 14 (TriggerZoneExitLevel, Exit icon 2): active byte `obj[138]`, inside a visited room.
  - 12 (CheckpointZone, icon 12): inside a visited room (no active byte read).
- ShowObjectivesIcons: the CURRENT quest only (`SG_GetCurrentQuest`), each objective's `GetPositions` (vtable +32), and
  no visited check. Only three classes implement GetPositions: `Objective_MoveInZone` (0x47b850: the bound object's
  position, else the TriggerZoneExitLevel whose level matches), `Objective_Automatic` and `Objective_GatherLoot` (both
  empty). TalkToNPC has none; its marker is the NPC flag.
- ShowRoomExitsIcons: reads the global `dword_9A275C..dword_9A2760` (16-byte entries type,x,y,z). Type 2 -> icon 16
  (dir +x), 3 -> 17 (-x), 1 -> 15 (-y), other -> 14 (+y); drawn when the point is in a visited room and
  `point + 3000*dir` is in an unvisited room. The full IDA listing (`full-listing.asm`) has NO writer for that global
  (grep for `dword_9A275C`, `9A275C` and the `9A2760` offset: only the two loads inside ShowRoomExitsIcons). The global
  is in `.bss` (no initial data), so in this binary the list is never populated and ShowRoomExitsIcons draws nothing.
  This is an inference from the absence of writes in this IDA export, not from a runtime observation.
- Authored data: placed Character actors carry `charpropsname`; the actor profile `propertyRow` selects the
  CharacterProperties row, `build_original_combat_properties` resolves `sheets.resolved[1]` (the AI row id), and
  `ai_props(tables, id)->type` gives the AI type. Level objects keep every authored attribute (`activate_cond`,
  `deactivate_cond`, `entrypointID`, `levelName`, ...) in `ActorDefinition::properties`.

## Implementation (general, no map names)
- `features/map_visit/map_markers_v1.{hpp,cpp}` (new, in `foundation_map_visit`): the marker kinds (MapIconsDynamic frame
  labels), the input struct (visited test, level objects, characters, quest-talk rows, objective zones, enemies),
  the rules (`map_object_class_rule_v1` table of GO classes, `map_activation_gate_v1` with the population's
  activate/deactivate rule, `map_character_marker_kind_v1` with the IDA order quest giver before merchant), four
  providers (characters, level objects, objectives, enemies), and `MapMarkerRegistryV1` (named providers, duplicate
  names rejected). `standard_map_marker_registry_v1()` registers the providers in original Show order.
- `main.cpp` (small hunks, marked `P16 MAPMARKERS`):
  - level load (after the quest identity map): level objects with the activation gate and `entrypointID`; placed
    characters with the CharacterTable row and the merchant fact (AI type 7, cached per row); the current entry is the
    unique SpawnPoint within 1 unit of the start position (`options.actorPosition`).
  - each frame in the Map draw block: the quest-talk rows (quests in `active` state, TalkToNPC objectives not completed),
    the objective zones (current quest, MoveInZone, the zone box centre from `questZones`), the enemies (combat owner),
    activation refreshed from `population.actors()`, then `registry.collect` and one `drawMapIcon` per marker.
  - diagnostics (one time per map draw): `Map marker facts`, `Map markers frame=... <kind>=drawn/produced`, each
    produced marker with its world position and window pixel, and `Map quest current` (current quest objectives and the
    active / talk / zone rows).
- `features/character_menu/character_menu_tests.cpp`: the MAPFIX overlap expectation was stale. QUESTUI2 (`e91be20a`)
  narrowed the Quest Log contour to x 369.05..420.95, so the tabs no longer overlap. The test now expects the Map tab at
  x 424 (no overlap). `hit_test` is unchanged from integrate.

## Tests (ctest in DH_wt/build-p16mapmarkers)
- `map_markers` (new): kind table, activation gate (absent/present/empty/deactivate), character rule order (quest
  giver before merchant, unknown row), providers (visited and enabled filters, checkpoint without gate, exit and entrance
  with gate, entrance only for the current entry and not when the entry is unknown, objectives without visited check,
  enemies with visited check), registry (duplicate name rejected, order of collect). PASS.
- `character_menu`: PASS after the test update above.
- Full ctest: green except `session_skill_binding` (known, ignored per brief).

## Integrated runtime verification (quiet hidden runs, build DH_wt/build-p16mapmarkers, Swamp rc3 assets)
Jobs: `.local-inputs/claude-preview16/mapmarkers/jobs-markers.json` (fresh, f9), `jobs-cluster.json`, `jobs-exit.json`.
All exit 0, about 8 s each. Logs: `.local-inputs/claude-preview16/mapmarkers/<job>/run.log`.
- Facts (all runs): `level_objects=18 characters=35 merchants=1 entry=0` (start position), or `entry=unknown` when the
  start position is moved onto a non-spawn point.
- `fresh` (1 visited room, no markers produced): `Map markers frame=150 ... talk_rows=0 objective_zones=0`.
- `f9` (nine rooms visited, the start position): produced entrance 0, exit 1, checkpoint 4, enemy 22, merchant 1; drawn
  enemy 1 (the others are outside the zoom-1 frame, which is centred on the player). Image: `evidence/f9.png`.
- `cluster` (start position moved onto the merchant): drawn merchant 1/1, checkpoint 3/4, enemy 9/22.
  Image: `evidence/cluster.png` (ornate checkpoint frames, red enemy markers, merchant icon over the player marker).
- `exit` (start position moved onto the exit): drawn exit 1/1, enemy 8/22. Image: `evidence/exit.png` (red exit arch).
- The per-marker projection in the logs matches the drawn icon positions (the one enemy in `f9` at px 220.7,195.2 is the
  red dot at the same pixel in `evidence/f9.png`).
- Quest diagnostics (all runs): current quest row 50 is `active` with one objective of type 0 (kill); rows with
  TalkToNPC or MoveInZone objectives are all locked (state 0) in the fresh and f9 profiles.

## Reference comparison
- Part 1 (`video/part1_z_Zky7qQdYs.mp4`, 1331 s): scanned in contact sheets at 5 s (0-500 s) and 10 s (500-1332 s)
  and at 15 s over the whole video. No Map page (parchment, title, tab row) was found. Sheets in
  `.local-inputs/claude-preview16/mapmarkers/ref/`.
- Part 2 (`part2_cIAW38IfLxY.mp4`): the map pages are Gothicus Darkwood (level 2), which the port does not load in the EXE
  (`Level::_LoadFromXML` LoadTemplate assertion, see MAP-report gap 8). Frames already in `.local-inputs/claude-preview16/map/ref/`.
- So no original Swamp map frame exists in the supplied video. Marker icons are the authored MapIconsDynamic frames; their
  placement is the projection of the world positions (verified against the same camera as the visited rooms).

## Second level
- `005_infectedvillage.mlx` (in the rc3 package and `original-cache`) stops with the same
  `Level::_LoadFromXML ... LoadTemplate assertion(9) provider` error as Darkwood (levels stream, p16levels). Without
  `--source-root-scopes` it stops with `Source floor world requires --source-root-scopes`. No second-level marker run.
  Job: `.local-inputs/claude-preview16/mapmarkers/l005/`.

## Gaps (open, with the reason)
1. Room exit arrows (13..17): no producer found in the IDA export (no writer of `dword_9A275C`). The arrow art exists
   (frames 13..17). Owner should confirm with a runtime observation of a room-exit arrow in the original, or accept that
   this build never draws them.
2. Quest giver: the rule is implemented, but the fixtures have no active TalkToNPC objective, and the port cannot import
   the device saves (`dh2_000.savegame`, `dh2_000_0_000_041_level.savegame` are original-format; no importer in the port).
   `+763` (`InstallObjectiveMarker` a2 == 3) is not modelled: the a2 source (`*(quest+26)+276`) is not decoded.
3. Objectives: the rule is implemented (current quest MoveInZone, zone box centre). Not verified live: the current quest has
   no MoveInZone objective. Objective_MoveInZone's fallback (the TriggerZoneExitLevel whose level matches, when no bound
   zone object exists) is not implemented: the port binds by zone name only.
4. Entrance: the rule uses the entry of the spawn the player is placed on at load (inference from the IDA order of
   `PlaceObject` and `SG_SetLevelEntryPoint`). The Swamp start spawn is `activate_cond="Invalid"`, so no entrance is shown.
   The port keeps no level entry state; a real entry (loading from a door or a save) is not tracked yet.
5. Activation: `activate_cond`/`deactivate_cond` use the start-up active condition set (as the population does). A
   condition that changes during play does not update the level-object gate.
6. Second level: blocked by the levels stream (LoadTemplate), no runtime evidence on any other level.
7. Marker visibility depends on the Map camera framing (MAPFIX gap: the zoom-1 scale is not confirmed against an
   original frame). At zoom 1 the frame shows about the nearest 6-7k units, so most markers of a level are outside it.

## Placeholders
None. Every drawn icon is the authored MapIconsDynamic frame.

## Package files required
- Swamp rc3 assets (`.local-inputs/windows-source-clock-v19-preview-15-rc3/assets`, with `original-cache/data/pydata` for the
  AI tables and character properties), as in MAP-report. No new package files.
- Quiet-run fixtures: `.local-inputs/claude-preview16/mapmarkers/f9/` (copy of the MAPFIX nine-room save).

## Verifier script
- Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16mapmarkers -Jobs 6 -Test`
- Unit: `build-p16mapmarkers/map_markers_tests.exe` (prints `map_markers tests passed`).
- Runs: `port/windows-foundation/tools/quiet_run.ps1 -JobsFile .local-inputs/claude-preview16/mapmarkers/jobs-markers.json -Parallel 2`,
  then `jobs-cluster.json` and `jobs-exit.json` (Parallel 1).
- Expected logs: exit 0; `Map marker facts ... merchants=1`; `Map markers frame=150` with `merchant=1/1` (cluster),
  `checkpoint=3/4`, `exit=1/1` (exit job), `enemy=1/22` (f9).
