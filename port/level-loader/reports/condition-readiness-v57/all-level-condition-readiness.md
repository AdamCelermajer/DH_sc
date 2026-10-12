# All51 level condition readiness — source transport only

Checked all51 supplied level identities against actual ZIP XML attributes and the real118-row Conditions table. 873 authored activate/deactivate slots reference 93 distinct names; 20 explicit name/operator gaps. Original source parse/missing issues remain separate. This audit does not choose procedural objects, execute Eval, or prove rendering/playability.

| Level | Form | Slots | Resolved | Empty/Invalid | Missing | Source XML issues |
|---|---|---:|---:|---:|---:|---:|
| ABBEY_ENTRANCE_01 | procedural | 7 | 6 | 1 | 0 | 0 |
| ABBEY_RUINS_01 | fixed | 35 | 32 | 3 | 0 | 0 |
| APPLETEMPLE | procedural | 0 | 0 | 0 | 0 | 0 |
| APPLETEMPLEBACKUP | fixed | 10 | 0 | 10 | 0 | 0 |
| DARKWOOD | fixed | 120 | 92 | 24 | 4 | 0 |
| DARKWOOD_V2 | fixed | 120 | 92 | 24 | 4 | 0 |
| DARKWOOD_V3 | fixed | 120 | 92 | 24 | 4 | 0 |
| DARK_DUNGEON_TEMPLE_01 | fixed | 5 | 5 | 0 | 0 | 0 |
| DARK_DUNGEON_TEMPLE_BOSSROOM | fixed | 7 | 5 | 2 | 0 | 0 |
| DESERT_CAVE_01 | procedural | 8 | 5 | 3 | 0 | 0 |
| DESERT_CAVE_02 | procedural | 7 | 5 | 2 | 0 | 0 |
| DESERT_CAVE_03 | procedural | 3 | 1 | 2 | 0 | 0 |
| EARTH_TEMPLE_01 | procedural | 5 | 4 | 1 | 0 | 0 |
| EARTH_TEMPLE_02 | procedural | 0 | 0 | 0 | 0 | 0 |
| EARTH_TEMPLE_03 | procedural | 7 | 7 | 0 | 0 | 0 |
| ELEMENTAL_AIR | procedural | 1 | 1 | 0 | 0 | 1 |
| ELEMENTAL_EARTH | procedural | 1 | 1 | 0 | 0 | 1 |
| ELEMENTAL_FIRE | procedural | 4 | 1 | 3 | 0 | 1 |
| ELEMENTAL_LIGHT | procedural | 1 | 1 | 0 | 0 | 1 |
| ELEMENTAL_WATER | procedural | 1 | 1 | 0 | 0 | 1 |
| FIRE_TEMPLE_01 | fixed | 17 | 11 | 4 | 2 | 0 |
| GOTHICUS_CASTLE_01 | procedural | 5 | 4 | 1 | 0 | 0 |
| GOTHICUS_CASTLE_02 | procedural | 5 | 3 | 2 | 0 | 0 |
| GOTHICUS_CRYPT_01 | procedural | 28 | 23 | 5 | 0 | 1 |
| GOTHICUS_CRYPT_02 | procedural | 10 | 8 | 2 | 0 | 1 |
| ICY_CAVERN_01 | procedural | 11 | 2 | 9 | 0 | 0 |
| ICY_CAVERN_02 | procedural | 18 | 14 | 3 | 1 | 0 |
| ICY_CAVERN_03 | procedural | 5 | 4 | 0 | 1 | 0 |
| ICY_HUB | fixed | 32 | 20 | 12 | 0 | 0 |
| INFECTED_VILLAGE_01 | fixed | 3 | 2 | 1 | 0 | 0 |
| LIGHT_HOUSE_01 | procedural | 6 | 4 | 2 | 0 | 0 |
| LIGHT_HOUSE_02 | procedural | 0 | 0 | 0 | 0 | 1 |
| LIGHT_HOUSE_03 | procedural | 8 | 5 | 3 | 0 | 0 |
| MACHINE_ROOMS_01 | fixed | 4 | 1 | 3 | 0 | 0 |
| MULTIPLAYER_TEST | fixed | 0 | 0 | 0 | 0 | 0 |
| PORTAL_ROOM | fixed | 120 | 92 | 24 | 4 | 0 |
| QUESTTEST | procedural | 1 | 0 | 1 | 0 | 1 |
| RANDOMFORALEX | procedural | 1 | 1 | 0 | 0 | 1 |
| RED_DESERT_HUB | fixed | 12 | 10 | 2 | 0 | 0 |
| ROOT_DEN_01 | procedural | 6 | 2 | 4 | 0 | 1 |
| SECRET_BASEMENT_01 | procedural | 5 | 4 | 1 | 0 | 1 |
| SWAMP | fixed | 44 | 29 | 15 | 0 | 0 |
| SWAMP_02 | procedural | 2 | 2 | 0 | 0 | 0 |
| SWAMP_CAVE_TROLL_A | procedural | 1 | 1 | 0 | 0 | 0 |
| SWAMP_CAVE_WITCH_A | procedural | 7 | 6 | 1 | 0 | 0 |
| UNDERWORLD_HUB | fixed | 24 | 13 | 11 | 0 | 0 |
| VOID_MAZE_01 | procedural | 2 | 0 | 2 | 0 | 1 |
| VOID_MAZE_02 | procedural | 2 | 0 | 2 | 0 | 1 |
| VOID_MAZE_03 | procedural | 1 | 0 | 1 | 0 | 0 |
| WATER_TEMPLE_01 | procedural | 16 | 14 | 2 | 0 | 1 |
| WIND_TEMPLE_01 | procedural | 15 | 12 | 3 | 0 | 0 |

Explicit gaps:

- DARKWOOD / data/3d/modules/darkwood/mgp/darkwood_15_01.mgp / _prim_tmp_commontype130 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- DARKWOOD / data/3d/modules/darkwood/mgp/darkwood_20_01.mgp / _prim_DestructibleContainer03x / activate_cond = `IsDuring_Darkwood_Ring`: missing, ops[].
- DARKWOOD / data/3d/modules/darkwood/mgp/darkwood_14_01.mgp / _prim_tmp_commontype136 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- DARKWOOD / data/3d/modules/darkwood/mgp/darkwood_14_01.mgp / _prim_tmp_commontype134 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- DARKWOOD_V2 / data/3d/modules/darkwood/mgp/darkwood_15_01.mgp / _prim_tmp_commontype130 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- DARKWOOD_V2 / data/3d/modules/darkwood/mgp/darkwood_20_01.mgp / _prim_DestructibleContainer03x / activate_cond = `IsDuring_Darkwood_Ring`: missing, ops[].
- DARKWOOD_V2 / data/3d/modules/darkwood/mgp/darkwood_14_01.mgp / _prim_tmp_commontype136 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- DARKWOOD_V2 / data/3d/modules/darkwood/mgp/darkwood_14_01.mgp / _prim_tmp_commontype134 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- DARKWOOD_V3 / data/3d/modules/darkwood/mgp/darkwood_15_01.mgp / _prim_tmp_commontype130 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- DARKWOOD_V3 / data/3d/modules/darkwood/mgp/darkwood_20_01.mgp / _prim_DestructibleContainer03x / activate_cond = `IsDuring_Darkwood_Ring`: missing, ops[].
- DARKWOOD_V3 / data/3d/modules/darkwood/mgp/darkwood_14_01.mgp / _prim_tmp_commontype136 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- DARKWOOD_V3 / data/3d/modules/darkwood/mgp/darkwood_14_01.mgp / _prim_tmp_commontype134 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- FIRE_TEMPLE_01 / data/3d/modules/firetemple/mgp/firetemple_12.mgp / _prim_Door_NoEscape / activate_cond = `IsBefore_FireTemp_GoToHellgate`: missing, ops[].
- FIRE_TEMPLE_01 / data/3d/modules/firetemple/mgp/firetemple_09.mgp / _prim_TriggerZone_TemplarTalk / activate_cond = `IsBefore_FireTemp_GoToHellgate`: missing, ops[].
- ICY_CAVERN_02 / data/3d/modules/icy_cavern/mgp/ic02_monument_icycavern_we_01.mgp / _prim_NPC01_001 / activate_cond = `IsDuringIH_Howlingdeactivate_cond = Invalid`: missing, ops[].
- ICY_CAVERN_03 / data/3d/modules/icy_cavern/mgp/ic03_objend_icycavern_w_00_01.mgp / _prim_NPC01_001 / activate_cond = `IsDuringIH_Howlingdeactivate_cond = Invalid`: missing, ops[].
- PORTAL_ROOM / data/3d/modules/darkwood/mgp/darkwood_15_01.mgp / _prim_tmp_commontype130 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- PORTAL_ROOM / data/3d/modules/darkwood/mgp/darkwood_20_01.mgp / _prim_DestructibleContainer03x / activate_cond = `IsDuring_Darkwood_Ring`: missing, ops[].
- PORTAL_ROOM / data/3d/modules/darkwood/mgp/darkwood_14_01.mgp / _prim_tmp_commontype136 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].
- PORTAL_ROOM / data/3d/modules/darkwood/mgp/darkwood_14_01.mgp / _prim_tmp_commontype134 / activate_cond = `IsLowerThanDW_Phase_3`: missing, ops[].

All seven source constructors are adopted in selected766 and the actual50 SWAMP condition-field probe passed (parent receipt condition-list-v57-selected-verified.json). The actual cache uses quest discriminator types0/1/2. Empty/Invalid values follow the original early-return branch; missing names follow the original table-miss branch rather than creating a condition. Current ObjectBase TestEnableCondition only calls its evaluator when compiled_a8 is nonnull and tested_ac is false; a miss must preserve that original branch, not invent a condition or treat a source miss as a missing constructor. Actual gameplay effects remain main-owned. Conditional attributes outside activate_cond/deactivate_cond are retained as separate domains, without claiming they route through ConditionData. Per-level inherited source issues and module/declaration provenance remain in the machine-readable table.

Exact missing-name counts:

- `IsLowerThanDW_Phase_3`: 12 authored references.
- `IsDuring_Darkwood_Ring`: 4 authored references.
- `IsBefore_FireTemp_GoToHellgate`: 2 authored references.
- `IsDuringIH_Howlingdeactivate_cond = Invalid`: 2 authored references.

Original XML attribute verification: 873 slots checked against exact original XML; 0 slots remain inventory-only where strict source parsing fails.15 source parse/missing issue occurrences are retained; the inherited broader audit issues are not discarded. These do not automatically prove a runtime map failure because procedural selection and original XML parser behavior are separate prerequisites. SWAMP and SWAMP_02 have zero condition-name or constructor-op misses.
