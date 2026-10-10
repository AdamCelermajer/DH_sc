# P16 MAPMARKERS report (worktree DH_wt/p16mapmarkers, branch p16/mapmarkers, started from p16/integrate 261f2e49)

Status: IN PROGRESS (skeleton). Scope: the Map page marker families not produced by MAPFIX: NPC quest giver (10),
merchant (11), entrance (1), exit (2), checkpoint (12), room exit arrows (13), quest objectives (0).

## Investigation (IDA, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
Producers, all called from `MenuCharMenu_Map::Show` (0x455138), after ClearAllIcons and ShowPlayersIcons:
- `ShowNpcIcons` (0x454fb8): walks the live character list (dword_99F764+96). Visible, alive, inside a visited room
  (`IsInsideRooms(pos, visited)`): `Character::IsMonster` -> 4 (Enemies); `+762 && !+763` -> 10 (QuestGiver);
  `Character::IsMerchant` -> 11 (Merchant). IsMerchant = `GetCharType()==7` (CharAI row type 7).
- `+762` is set by `Objective_TalkToNPC::Register` / `InstallObjectiveMarker` (0x47d8e8, 0x47d8e8 family) for the NPC
  whose CharacterTable row equals the objective oid; cleared by `Unregister` / `RemoveObjectiveMarker`. `+763` is set
  by `InstallObjectiveMarker` to `(a2 == 3)`; the a2 meaning is not decoded (open).
- Install is driven by `Quest::Compile` (0x480178): objectives are registered and markers installed when the quest
  state is 6 = `active` (v2QuestState numbering: 0 locked, 2 pre_available, 3 available, 5 pre_active, 6 active).
- `ShowMapExitsIcons` (0x454d5c): walks the level object list (dword_99F764+20). Object type word = obj[61]:
  13 = SpawnPoint (Entrance, icon 1; also needs obj[221] == Level[68] and active byte obj[138]); 14 =
  TriggerZoneExitLevel (Exit, icon 2; active byte obj[138]); 12 = CheckpointZone (icon 12). All inside visited rooms.
  GO_IDS are the constructor arguments (`CheckpointZone(v,12)`, `TriggerZoneExitLevel(v,14)`, `SpawnPoint(v,13)`).
- `ShowObjectivesIcons` (0x4548ac): the CURRENT quest only (`SG_GetCurrentQuest`), every objective's `GetPositions`.
  No visited check. GetPositions is implemented by `Objective_MoveInZone` (0x47b850: the bound object's position, or the
  TriggerZoneExitLevel whose level matches), and NOT by `Objective_Automatic` / `Objective_GatherLoot` (empty).
  TalkToNPC has no GetPositions; its marker is the NPC flag above.
- `ShowRoomExitsIcons` (0x454b8c): iterates the global list `dword_9A275C..dword_9A2760` (16-byte entries: type,x,y,z).
  Entry type 2 -> icon 16 (dir +x), 3 -> 17 (dir -x), 1 -> 15 (dir -y), other -> 14 (dir +y); drawn when the point is in
  a visited room and `point + 3000*dir` is in an unvisited room. NO WRITER of `dword_9A275C` exists in the decoded
  listing (only the two reads in ShowRoomExitsIcons), so the producer is not decoded: see gaps.
- Authored data: placed Character actors carry `charpropsname` (actor profile `propertyRow` -> CharacterProperties);
  AI row `type` (7 merchant) via `build_original_combat_properties` -> `sheets.resolved[1]` -> `ai_props`.
  Level objects keep every authored attribute in `ActorDefinition::properties` (activate_cond, deactivate_cond,
  levelName, entrypointID, ...).

## Gaps (to be filled in this report as work proceeds)
- Room exit arrows: producer not decoded (no writer of the global exit list in the decoded IDA listing).
- Entrance: the IDA level-row equality (obj[221] vs Level[68]) is not mapped to an authored field.
