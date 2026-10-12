# All-map authored script resource coverage

This is a pure stored-byte inspection using the retained 80 Data schemas. It did not run the native parser, original code, Init/Execute/IsBlocking, tests or compiler jobs. Raw file and operand bytes are retained in source-script-coverage.json; integer values are unsigned bit displays, CString text is display-only, and no opaque meaning is inferred. No new hash or frozen packet was produced.

Common C00 is the application-owned common script resource. Local L01..L38 are authored pair resources associated with distinct level identities; sharing a pair does not merge those level owners. U01 remains authored but unassociated with the retained 51 identity scriptFile mapping. MULTIPLAYER_TEST has no authored local reference in that metadata; common ownership remains separate.

## Source inventory

| Pair / owner scope | Authored command URI | Associated identities | Command / name bytes | Scripts / names / complete records | Exact reached kinds | Source status |
|---|---|---|---:|---:|---|---|
|C00 /common_application|data/pydata/scripts_pyscripts.bin|none mapped|1277 /321|15 /15 /81|0,1,2,3,10,12,13,22,23,24,25,26,31,32,39,63,68,69,70,79|SOURCE_PAIR_DECODED|
|L01 /local_level|data/pydata/scripts/001_swamp_pyscripts.bin|SWAMP|18117 /1155|55 /55 /789|0,3,5,6,8,10,12,13,14,15,16,17,19,20,21,24,25,26,27,29,30,31,39,40,41,42,43,44,45,46,51,52,54,55,77,78,79|SOURCE_PAIR_DECODED|
|L02 /local_level|data/pydata/scripts/002_swamp_cave_witch_pyscripts.bin|SWAMP_CAVE_WITCH_A|4638 /465|24 /24 /183|0,3,5,6,8,10,12,13,15,20,21,24,25,26,29,30,31,41,42,43,44,45,46,54,55,79|SOURCE_PAIR_DECODED|
|L03 /local_level|data/pydata/scripts/003_darkwood_pyscripts.bin|DARKWOOD, DARKWOOD_V2, DARKWOOD_V3, PORTAL_ROOM|19505 /2251|105 /105 /1177|0,3,5,6,8,10,12,13,14,15,20,21,22,23,24,25,26,29,30,31,39,40,41,42,43,45,46,54,55,64,74,77,78,79|SOURCE_PAIR_DECODED|
|L04 /local_level|data/pydata/scripts/004_root_den_pyscripts.bin|ROOT_DEN_01|811 /202|9 /9 /44|0,3,5,8,10,12,15,24,25,26,27,41,46,79|SOURCE_PAIR_DECODED|
|L05 /local_level|data/pydata/scripts/005_infectedvillage_pyscripts.bin|INFECTED_VILLAGE_01|612 /106|6 /6 /36|0,5,8,10,12,15,24,25,26,30|SOURCE_PAIR_DECODED|
|L06 /local_level|data/pydata/scripts/006_driedwell_pyscripts.bin|SECRET_BASEMENT_01|802 /143|8 /8 /55|0,3,8,10,12,15,24,25,26,30,41,43,45|SOURCE_PAIR_DECODED|
|L07 /local_level|data/pydata/scripts/007_crypt_01_pyscripts.bin|GOTHICUS_CRYPT_01, GOTHICUS_CRYPT_02, QUESTTEST|2129 /507|25 /25 /116|0,3,8,10,12,15,24,25,26,30,41,54,55,77,78|SOURCE_PAIR_DECODED|
|L08 /local_level|data/pydata/scripts/008_gothicus_castle_01_pyscripts.bin|GOTHICUS_CASTLE_01|7738 /181|10 /10 /328|0,3,5,6,8,10,12,13,14,15,19,20,24,25,26,30,31,40,41,43,44,45,46,51,52,55,64|SOURCE_PAIR_DECODED|
|L09 /local_level|data/pydata/scripts/009_abbey_pyscripts.bin|ABBEY_ENTRANCE_01, ABBEY_RUINS_01|8388 /696|37 /37 /424|0,3,5,8,10,12,13,15,16,20,24,25,26,27,30,39,40,41,42,43,45,46,54,55,78,79|SOURCE_PAIR_DECODED|
|L10 /local_level|data/pydata/scripts/010_elemental_lightning_room_pyscripts.bin|ELEMENTAL_LIGHT, RANDOMFORALEX|438 /138|6 /6 /37|0,10,12,15,24,25,27,28|SOURCE_PAIR_DECODED|
|L11 /local_level|data/pydata/scripts/011_earth_temple_01_pyscripts.bin|EARTH_TEMPLE_01|465 /105|5 /5 /23|0,8,10,12,15,24,25,26,27,41,46,54,69|SOURCE_PAIR_DECODED|
|L12 /local_level|data/pydata/scripts/011_earth_temple_demo_pyscripts.bin|APPLETEMPLE, APPLETEMPLEBACKUP|1171 /170|10 /10 /57|0,5,8,10,13,15,19,20,22,23,24,25,26,30,54,55,69|SOURCE_PAIR_DECODED|
|L13 /local_level|data/pydata/scripts/012_earth_temple_02_pyscripts.bin|EARTH_TEMPLE_02|1077 /194|8 /8 /60|0,5,8,10,12,15,20,24,25,26,27,30,41,42,54,55|SOURCE_PAIR_DECODED|
|L14 /local_level|data/pydata/scripts/013_earth_temple_03_pyscripts.bin|EARTH_TEMPLE_03|6547 /307|15 /15 /274|0,3,5,6,8,10,12,13,14,15,20,21,24,25,26,27,29,30,31,40,41,42,43,44,45,46,54,55,66,69,77,78|SOURCE_PAIR_DECODED|
|L15 /local_level|data/pydata/scripts/014_elemental_earth_room_pyscripts.bin|ELEMENTAL_EARTH|438 /130|6 /6 /37|0,10,12,15,24,25,27,28|SOURCE_PAIR_DECODED|
|L16 /local_level|data/pydata/scripts/015_red_desert_hub_pyscripts.bin|RED_DESERT_HUB|2687 /481|23 /23 /158|0,3,8,10,12,15,16,17,24,25,26,39,40,41,43,45,79|SOURCE_PAIR_DECODED|
|L17 /local_level|data/pydata/scripts/016_red_desert_cave_01_pyscripts.bin|DESERT_CAVE_01|226 /125|5 /5 /10|0,3,10,15,24,25,27,79|SOURCE_PAIR_DECODED|
|L18 /local_level|data/pydata/scripts/017_red_desert_cave_02_pyscripts.bin|DESERT_CAVE_02|514 /152|7 /7 /28|0,3,5,8,10,12,15,24,25,26,27,41,79|SOURCE_PAIR_DECODED|
|L19 /local_level|data/pydata/scripts/018_red_desert_cave_03_pyscripts.bin|DESERT_CAVE_03|226 /125|5 /5 /10|0,3,10,15,24,25,27,79|SOURCE_PAIR_DECODED|
|L20 /local_level|data/pydata/scripts/019_light_house_01_pyscripts.bin|LIGHT_HOUSE_01, LIGHT_HOUSE_02, LIGHT_HOUSE_03|16203 /1421|55 /55 /617|0,3,5,8,10,12,13,14,15,20,22,23,24,25,26,29,30,40,41,42,43,45,46,54,55,79|SOURCE_PAIR_DECODED|
|L21 /local_level|data/pydata/scripts/021_gothicus_castle_02_pyscripts.bin|GOTHICUS_CASTLE_02|4448 /427|22 /22 /225|0,3,8,10,12,13,15,20,21,24,25,26,40,41,42,43,44,45,46,54,55,69|SOURCE_PAIR_DECODED|
|L22 /local_level|data/pydata/scripts/022_swamp2_pyscripts.bin|SWAMP_02|831 /114|6 /6 /38|0,3,8,10,12,15,20,24,25,26,27,41,42,43,46,54,55|SOURCE_PAIR_DECODED|
|L23 /local_level|data/pydata/scripts/022_swamp2_troll_cave_pyscripts.bin|SWAMP_CAVE_TROLL_A|703 /162|8 /8 /45|0,3,8,10,12,15,24,25,27,41,54,55|SOURCE_PAIR_DECODED|
|L24 /local_level|data/pydata/scripts/023_watertemple_01_pyscripts.bin|WATER_TEMPLE_01|6062 /403|18 /18 /329|0,5,8,10,12,13,15,20,24,25,26,27,30,31,39,40,41,42,43,44,45,46,54,55,66|SOURCE_PAIR_DECODED|
|L25 /local_level|data/pydata/scripts/024_elemental_water_room_pyscripts.bin|ELEMENTAL_WATER|438 /130|6 /6 /37|0,10,12,15,24,25,27,28|SOURCE_PAIR_DECODED|
|L26 /local_level|data/pydata/scripts/025_icy_hub_pyscripts.bin|ICY_HUB|2761 /510|25 /25 /215|0,5,8,10,12,15,16,17,24,25,26,27,41,54,79|SOURCE_PAIR_DECODED|
|L27 /local_level|data/pydata/scripts/026_icy_caverns_01_pyscripts.bin|ICY_CAVERN_01|617 /143|7 /7 /36|0,3,8,10,12,15,24,25,41,79|SOURCE_PAIR_DECODED|
|L28 /local_level|data/pydata/scripts/027_icy_caverns_02_pyscripts.bin|ICY_CAVERN_02|1051 /195|10 /10 /68|0,3,5,8,10,12,13,15,24,25,26,31,45,79|SOURCE_PAIR_DECODED|
|L29 /local_level|data/pydata/scripts/028_icy_caverns_03_pyscripts.bin|ICY_CAVERN_03|85 /95|4 /4 /4|3,10,15|SOURCE_PAIR_DECODED|
|L30 /local_level|data/pydata/scripts/029_wind_temple_pyscripts.bin|WIND_TEMPLE_01|3638 /329|17 /17 /192|0,3,5,8,10,12,13,15,20,24,25,26,27,30,40,41,42,43,44,45,46,54,66|SOURCE_PAIR_DECODED|
|L31 /local_level|data/pydata/scripts/030_elemental_air_room_pyscripts.bin|ELEMENTAL_AIR|438 /124|6 /6 /37|0,10,12,15,24,25,27,28|SOURCE_PAIR_DECODED|
|L32 /local_level|data/pydata/scripts/031_firetemple_pyscripts.bin|FIRE_TEMPLE_01|7107 /389|20 /20 /366|0,3,5,8,10,12,13,14,15,20,21,22,23,24,25,26,27,30,31,39,40,41,42,43,45,46,54,55,66|SOURCE_PAIR_DECODED|
|L33 /local_level|data/pydata/scripts/032_elemental_fire_room_pyscripts.bin|ELEMENTAL_FIRE|438 /126|6 /6 /37|0,10,12,15,24,25,27,28|SOURCE_PAIR_DECODED|
|L34 /local_level|data/pydata/scripts/033_voidmaze_pyscripts.bin|VOID_MAZE_01, VOID_MAZE_02, VOID_MAZE_03|2801 /482|25 /25 /174|0,8,10,12,15,22,23,24,25,26,39,41,46,54,64|SOURCE_PAIR_DECODED|
|L35 /local_level|data/pydata/scripts/036_underworld_hub_pyscripts.bin|UNDERWORLD_HUB|4495 /634|27 /27 /282|0,3,5,8,10,12,13,15,16,17,20,22,23,24,25,26,30,31,40,41,42,43,45,46,54,64,79|SOURCE_PAIR_DECODED|
|L36 /local_level|data/pydata/scripts/037_machine_rooms_01_pyscripts.bin|MACHINE_ROOMS_01|3534 /1050|67 /67 /195|0,3,8,10,12,13,15,20,23,24,25,26,27,30,42,43,44,46,64|SOURCE_PAIR_DECODED|
|L37 /local_level|data/pydata/scripts/039_darktemple_pyscripts.bin|DARK_DUNGEON_TEMPLE_01|2574 /288|13 /13 /116|0,3,8,10,12,13,15,20,22,24,25,26,30,31,40,41,42,43,44,46,54,55|SOURCE_PAIR_DECODED|
|L38 /local_level|data/pydata/scripts/040_darktemple_bossroom_pyscripts.bin|DARK_DUNGEON_TEMPLE_BOSSROOM|5790 /284|13 /13 /254|0,3,5,6,8,10,12,13,14,15,20,21,22,23,24,25,26,30,31,41,42,43,44,45,46,51,52,65,67,79|SOURCE_PAIR_DECODED|
|U01 /unmapped_authored_pair|data/pydata/scripts/102_swamp_cave_troll_pyscripts.bin|none mapped|1404 /177|9 /9 /59|0,8,24,25,26,30,31,40,41,43,45,46|SOURCE_PAIR_DECODED|

## All 51 retained level identities

| Level identity | Common owner resource | Local owner resources | Exact local reached kinds | Source status |
|---|---|---|---|
|ABBEY_ENTRANCE_01|C00; application scope|L09|0,3,5,8,10,12,13,15,16,20,24,25,26,27,30,39,40,41,42,43,45,46,54,55,78,79|SOURCE_LOCAL_PAIRS_DECODED|
|ABBEY_RUINS_01|C00; application scope|L09|0,3,5,8,10,12,13,15,16,20,24,25,26,27,30,39,40,41,42,43,45,46,54,55,78,79|SOURCE_LOCAL_PAIRS_DECODED|
|APPLETEMPLE|C00; application scope|L12|0,5,8,10,13,15,19,20,22,23,24,25,26,30,54,55,69|SOURCE_LOCAL_PAIRS_DECODED|
|APPLETEMPLEBACKUP|C00; application scope|L12|0,5,8,10,13,15,19,20,22,23,24,25,26,30,54,55,69|SOURCE_LOCAL_PAIRS_DECODED|
|DARKWOOD|C00; application scope|L03|0,3,5,6,8,10,12,13,14,15,20,21,22,23,24,25,26,29,30,31,39,40,41,42,43,45,46,54,55,64,74,77,78,79|SOURCE_LOCAL_PAIRS_DECODED|
|DARKWOOD_V2|C00; application scope|L03|0,3,5,6,8,10,12,13,14,15,20,21,22,23,24,25,26,29,30,31,39,40,41,42,43,45,46,54,55,64,74,77,78,79|SOURCE_LOCAL_PAIRS_DECODED|
|DARKWOOD_V3|C00; application scope|L03|0,3,5,6,8,10,12,13,14,15,20,21,22,23,24,25,26,29,30,31,39,40,41,42,43,45,46,54,55,64,74,77,78,79|SOURCE_LOCAL_PAIRS_DECODED|
|DARK_DUNGEON_TEMPLE_01|C00; application scope|L37|0,3,8,10,12,13,15,20,22,24,25,26,30,31,40,41,42,43,44,46,54,55|SOURCE_LOCAL_PAIRS_DECODED|
|DARK_DUNGEON_TEMPLE_BOSSROOM|C00; application scope|L38|0,3,5,6,8,10,12,13,14,15,20,21,22,23,24,25,26,30,31,41,42,43,44,45,46,51,52,65,67,79|SOURCE_LOCAL_PAIRS_DECODED|
|DESERT_CAVE_01|C00; application scope|L17|0,3,10,15,24,25,27,79|SOURCE_LOCAL_PAIRS_DECODED|
|DESERT_CAVE_02|C00; application scope|L18|0,3,5,8,10,12,15,24,25,26,27,41,79|SOURCE_LOCAL_PAIRS_DECODED|
|DESERT_CAVE_03|C00; application scope|L19|0,3,10,15,24,25,27,79|SOURCE_LOCAL_PAIRS_DECODED|
|EARTH_TEMPLE_01|C00; application scope|L11|0,8,10,12,15,24,25,26,27,41,46,54,69|SOURCE_LOCAL_PAIRS_DECODED|
|EARTH_TEMPLE_02|C00; application scope|L13|0,5,8,10,12,15,20,24,25,26,27,30,41,42,54,55|SOURCE_LOCAL_PAIRS_DECODED|
|EARTH_TEMPLE_03|C00; application scope|L14|0,3,5,6,8,10,12,13,14,15,20,21,24,25,26,27,29,30,31,40,41,42,43,44,45,46,54,55,66,69,77,78|SOURCE_LOCAL_PAIRS_DECODED|
|ELEMENTAL_AIR|C00; application scope|L31|0,10,12,15,24,25,27,28|SOURCE_LOCAL_PAIRS_DECODED|
|ELEMENTAL_EARTH|C00; application scope|L15|0,10,12,15,24,25,27,28|SOURCE_LOCAL_PAIRS_DECODED|
|ELEMENTAL_FIRE|C00; application scope|L33|0,10,12,15,24,25,27,28|SOURCE_LOCAL_PAIRS_DECODED|
|ELEMENTAL_LIGHT|C00; application scope|L10|0,10,12,15,24,25,27,28|SOURCE_LOCAL_PAIRS_DECODED|
|ELEMENTAL_WATER|C00; application scope|L25|0,10,12,15,24,25,27,28|SOURCE_LOCAL_PAIRS_DECODED|
|FIRE_TEMPLE_01|C00; application scope|L32|0,3,5,8,10,12,13,14,15,20,21,22,23,24,25,26,27,30,31,39,40,41,42,43,45,46,54,55,66|SOURCE_LOCAL_PAIRS_DECODED|
|GOTHICUS_CASTLE_01|C00; application scope|L08|0,3,5,6,8,10,12,13,14,15,19,20,24,25,26,30,31,40,41,43,44,45,46,51,52,55,64|SOURCE_LOCAL_PAIRS_DECODED|
|GOTHICUS_CASTLE_02|C00; application scope|L21|0,3,8,10,12,13,15,20,21,24,25,26,40,41,42,43,44,45,46,54,55,69|SOURCE_LOCAL_PAIRS_DECODED|
|GOTHICUS_CRYPT_01|C00; application scope|L07|0,3,8,10,12,15,24,25,26,30,41,54,55,77,78|SOURCE_LOCAL_PAIRS_DECODED|
|GOTHICUS_CRYPT_02|C00; application scope|L07|0,3,8,10,12,15,24,25,26,30,41,54,55,77,78|SOURCE_LOCAL_PAIRS_DECODED|
|ICY_CAVERN_01|C00; application scope|L27|0,3,8,10,12,15,24,25,41,79|SOURCE_LOCAL_PAIRS_DECODED|
|ICY_CAVERN_02|C00; application scope|L28|0,3,5,8,10,12,13,15,24,25,26,31,45,79|SOURCE_LOCAL_PAIRS_DECODED|
|ICY_CAVERN_03|C00; application scope|L29|3,10,15|SOURCE_LOCAL_PAIRS_DECODED|
|ICY_HUB|C00; application scope|L26|0,5,8,10,12,15,16,17,24,25,26,27,41,54,79|SOURCE_LOCAL_PAIRS_DECODED|
|INFECTED_VILLAGE_01|C00; application scope|L05|0,5,8,10,12,15,24,25,26,30|SOURCE_LOCAL_PAIRS_DECODED|
|LIGHT_HOUSE_01|C00; application scope|L20|0,3,5,8,10,12,13,14,15,20,22,23,24,25,26,29,30,40,41,42,43,45,46,54,55,79|SOURCE_LOCAL_PAIRS_DECODED|
|LIGHT_HOUSE_02|C00; application scope|L20|0,3,5,8,10,12,13,14,15,20,22,23,24,25,26,29,30,40,41,42,43,45,46,54,55,79|SOURCE_LOCAL_PAIRS_DECODED|
|LIGHT_HOUSE_03|C00; application scope|L20|0,3,5,8,10,12,13,14,15,20,22,23,24,25,26,29,30,40,41,42,43,45,46,54,55,79|SOURCE_LOCAL_PAIRS_DECODED|
|MACHINE_ROOMS_01|C00; application scope|L36|0,3,8,10,12,13,15,20,23,24,25,26,27,30,42,43,44,46,64|SOURCE_LOCAL_PAIRS_DECODED|
|MULTIPLAYER_TEST|C00; application scope|no authored local scriptFile||NO_AUTHORED_LOCAL_SCRIPTFILE_IN_RETAINED_METADATA|
|PORTAL_ROOM|C00; application scope|L03|0,3,5,6,8,10,12,13,14,15,20,21,22,23,24,25,26,29,30,31,39,40,41,42,43,45,46,54,55,64,74,77,78,79|SOURCE_LOCAL_PAIRS_DECODED|
|QUESTTEST|C00; application scope|L07|0,3,8,10,12,15,24,25,26,30,41,54,55,77,78|SOURCE_LOCAL_PAIRS_DECODED|
|RANDOMFORALEX|C00; application scope|L10|0,10,12,15,24,25,27,28|SOURCE_LOCAL_PAIRS_DECODED|
|RED_DESERT_HUB|C00; application scope|L16|0,3,8,10,12,15,16,17,24,25,26,39,40,41,43,45,79|SOURCE_LOCAL_PAIRS_DECODED|
|ROOT_DEN_01|C00; application scope|L04|0,3,5,8,10,12,15,24,25,26,27,41,46,79|SOURCE_LOCAL_PAIRS_DECODED|
|SECRET_BASEMENT_01|C00; application scope|L06|0,3,8,10,12,15,24,25,26,30,41,43,45|SOURCE_LOCAL_PAIRS_DECODED|
|SWAMP|C00; application scope|L01|0,3,5,6,8,10,12,13,14,15,16,17,19,20,21,24,25,26,27,29,30,31,39,40,41,42,43,44,45,46,51,52,54,55,77,78,79|SOURCE_LOCAL_PAIRS_DECODED|
|SWAMP_02|C00; application scope|L22|0,3,8,10,12,15,20,24,25,26,27,41,42,43,46,54,55|SOURCE_LOCAL_PAIRS_DECODED|
|SWAMP_CAVE_TROLL_A|C00; application scope|L23|0,3,8,10,12,15,24,25,27,41,54,55|SOURCE_LOCAL_PAIRS_DECODED|
|SWAMP_CAVE_WITCH_A|C00; application scope|L02|0,3,5,6,8,10,12,13,15,20,21,24,25,26,29,30,31,41,42,43,44,45,46,54,55,79|SOURCE_LOCAL_PAIRS_DECODED|
|UNDERWORLD_HUB|C00; application scope|L35|0,3,5,8,10,12,13,15,16,17,20,22,23,24,25,26,30,31,40,41,42,43,45,46,54,64,79|SOURCE_LOCAL_PAIRS_DECODED|
|VOID_MAZE_01|C00; application scope|L34|0,8,10,12,15,22,23,24,25,26,39,41,46,54,64|SOURCE_LOCAL_PAIRS_DECODED|
|VOID_MAZE_02|C00; application scope|L34|0,8,10,12,15,22,23,24,25,26,39,41,46,54,64|SOURCE_LOCAL_PAIRS_DECODED|
|VOID_MAZE_03|C00; application scope|L34|0,8,10,12,15,22,23,24,25,26,39,41,46,54,64|SOURCE_LOCAL_PAIRS_DECODED|
|WATER_TEMPLE_01|C00; application scope|L24|0,5,8,10,12,13,15,20,24,25,26,27,30,31,39,40,41,42,43,44,45,46,54,55,66|SOURCE_LOCAL_PAIRS_DECODED|
|WIND_TEMPLE_01|C00; application scope|L30|0,3,5,8,10,12,13,15,20,24,25,26,27,30,40,41,42,43,44,45,46,54,66|SOURCE_LOCAL_PAIRS_DECODED|

## Additional authored kinds

| Kind | Actual source class | Local occurrences / exact pairs |
|---:|---|---|
|28|Script_IncFaeryLevel|5 /L10,L15,L25,L31,L33|
|64|Script_ChangeLevel|8 /L03,L08,L34,L35,L36|
|65|Script_EndGame|1 /L38|
|66|Script_AwardTrophy|4 /L14,L24,L30,L32|
|67|Script_AwardEndGameTrophies|1 /L38|
|74|Script_EnqueueCharMenuTutorialMessage|4 /L03|

Common plus mapped local resources reach 52 distinct kinds; the mapped local resources alone reach 46. The unassociated U01 pair contains 9 scripts/59 records and 1404 command bytes/177 name bytes; its 12 kinds are already present in mapped local resources. U01 is not assigned to any of the 51 identities.

## Exact resource and decoding boundaries

Every pair JSON row identifies both requested command/name URI, exact ZIP entry, uncompressed source length and stored ZIP length. Each script/name index, count/header byte span, complete command byte span and ordered field source cursor is retained. CString raw bytes exclude the native sentinel that storage adds; raw arrays preserve every four-byte word. Constructor defaults are not fabricated into authored bytes.

The selected mapped pairs contain 724 authored local scripts and 7113 complete command records; common adds 15 scripts and 81 records. Additional authored kinds beyond the historical common/SWAMP 46 set: 28,64,65,66,67,74. Static table slots not reached in these selected resources: 4,7,9,11,18,33,34,35,36,37,38,47,48,49,50,53,56,57,58,59,60,61,62,71,72,73,75,76.

No missing file is replaced with empty data. A source-decode success establishes record boundaries under retained schemas, not independent native parser agreement, correct actual Init effects, script scheduling, authored progression, resource readiness or gameplay. The original common/SWAMP receipts remain the historical native/original evidence; the other pairs now have source-data inspection only.

The same inventory also contains SWAMP-OBJECT-SCRIPTS.md and swamp-object-script-summary.json for the exact five chest/ten NPC-template non-map resource roots, with separate object/character Lua ownership.
