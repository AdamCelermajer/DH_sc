# Act 2

> Act membership is INFERENCE. See `../data/act-membership.json`; owner confirmation is pending.

## Levels

| File | Format | Level script | Music | Scene objects | Procedural lists | Source status |
| --- | --- | --- | --- | --- | --- | --- |
| 003_darkwood.mlx | campaign | data\PyData\scripts\003_DARKWOOD.pyscript | DarkWoodHubAmbientMusic | 24 | 0 | DONE |


## Level detail

### `003_darkwood.mlx`

Source: `.local-inputs/assets-extra/android/data/scene/003_darkwood.mlx`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| type | level |
| name | _prim_LevelConfig |
| position | 2986.32,-21328.6,137.398 |
| scale | 1.0,1.0,1.0 |
| debugcolor | 255.0,0.0,255.0 |
| rotation | 0.0,0.0,0.0 |
| gametype | LevelConfig |
| scriptFile | data\PyData\scripts\003_DARKWOOD.pyscript |
| camera_file |  |
| camera_name |  |
| camera_znear | 600 |
| camera_zfar | 30000 |
| music | DarkWoodHubAmbientMusic |
| ambientColor | 0.5,0.5,0.5 |
| fog_start | 3000 |
| fog_end | 5800 |
| fog_color | 1,14,0 |
| clear_color | 0,0,0 |
| _templateName | LevelConfig |
| light_set | data/3D/Light/darkwood.lightset_xml |
| fixed_light_set | data/3D/Light/darkwood_es1_1.lightset_xml |
| prog_vertex_buffer_size | 390672 |
| prog_index_buffer_size | 79824 |
| fix_vertex_buffer_size | 900724 |
| fix_index_buffer_size | 79764 |
| fix_quantize | 1 |


Direct scene objects (not character spawns):

| Type | Name | Game type | Script |
| --- | --- | --- | --- |
| Block | _module_01_001 | Module |  |
| Block | _module_02_001 | Module |  |
| Block | _module_03_001 | Module |  |
| Block | _module_04_001 | Module |  |
| Block | _module_08_001 | Module |  |
| Block | _module_13_001 | Module |  |
| Block | _module_19_001 | Module |  |
| Block | _module_12_001 | Module |  |
| Block | _module_23_001 | Module |  |
| Block | _module_09_001 | Module |  |
| Block | _module_05_001 | Module |  |
| Block | _module_06_001 | Module |  |
| Block | _module_10_001 | Module |  |
| Block | _module_15_001 | Module |  |
| Block | _module_21_001 | Module |  |
| Block | _module_20_001 | Module |  |
| Block | _module_14_001 | Module |  |
| Block | _module_11_001 | Module |  |
| Block | _module_16_001 | Module |  |
| Block | _module_27_001 | Module |  |
| Block | _module_07_001 | Module |  |
| Block | _module_17_001 | Module |  |
| Block | _module_28_001 | Module |  |
| Block | _module_29_001 | Module |  |


## Actors and mobs

CharacterProperties rows are linked from exact source-script names, MGP `charpropsname`, or explicit B155/B156 encounter evidence. Other MGP Character objects and `Script_SpawnCharacter` arguments remain conditional candidates; command branches and runtime execution are not evaluated.

| Actor rows | Mapped to this act | Status |
| --- | --- | --- |
| 448 | 27 | INFERENCE: serialized spawn-command targets |

Serialized Script_SpawnCharacter authored targets: 2; modular XML Character candidates: 90. Both are conditional candidates, not a static runtime roster.

### Modular XML Character candidates

Rows come from MGP files referenced directly by scene modules or by procedural-rule gameplay choices. `charpropsname` is an exact CharacterProperties link where present; `char_template` is kept as a template label when the CharacterProperties link is not decoded. Random module selection and `spawn_prob` are preserved as conditions.

| Scene | Module source | Object | CharacterProperties | Template | Spawn probability | AI state | Status |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_01_01.mgp | _prim_NPC_PriestGood | WanderingPriest | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_01_01.mgp | _prim_Faery | DefaultFairy | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_01_01.mgp | _prim_tmp_commontype123 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_01_01.mgp | _prim_tmp_commontype124 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_01_01.mgp | _prim_NPC_StillStatueRed | IcyHub_StillStatue | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_02_01.mgp | _prim_tmp_commontype100 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_03_01.mgp | _prim_tmp_commontype167 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_03_01.mgp | _prim_tmp_commontype169 | None | Darkwood_bandits_lvl_5_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_04_01.mgp | _prim_tmp_commontype161 | None | Darkwood_bandits_lvl_9_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_04_01.mgp | _prim_tmp_commontype162 | None | Darkwood_bandits_lvl_9_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_04_01.mgp | _prim_tmp_commontype163 | None | Darkwood_bandits_lvl_9_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_04_01.mgp | _prim_tmp_commontype185 | None | Darkwood_bandits_lvl_9_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_13_01.mgp | _prim_tmp_commontype155 | None | Darkwood_bandits_lvl_7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_13_01.mgp | _prim_tmp_commontype156 | None | Darkwood_bandits_lvl_7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_13_01.mgp | _prim_tmp_commontype176 | None | Darkwood_rathound_lv_l7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_19_01.mgp | _prim_tmp_commontype151 | None | Darkwood_bandits_lvl_6_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_19_01.mgp | _prim_tmp_commontype152 | None | Darkwood_bandits_lvl_6_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_19_01.mgp | _prim_tmp_commontype153 | None | Darkwood_bandits_lvl_7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_19_01.mgp | _prim_tmp_commontype154 | None | Darkwood_bandits_lvl_7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_12_01.mgp | _prim_tmp_commontype20 | None | Darkwood_rathound_lv_l7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_12_01.mgp | _prim_tmp_commontype10 | None | Darkwood_rathound_lv_l7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_12_01.mgp | _prim_tmp_commontype13 | None | Darkwood_rathound_lv_l7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_12_01.mgp | _prim_tmp_commontype19 | None | Darkwood_rathound_lv_l7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_12_01.mgp | _prim_tmp_commontype23 | None | Darkwood_rathound_lv_l7_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_23_01.mgp | _prim_tmp_commontype192 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_23_01.mgp | _prim_tmp_commontype193 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_23_01.mgp | _prim_tmp_commontype194 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_09_01.mgp | _prim_tmp_commontype104 | None | Darkwood_bandits_lvl_4_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_09_01.mgp | _prim_tmp_commontype106 | None | Darkwood_bandits_lvl_4_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_09_01.mgp | _prim_tmp_commontype107 | None | Darkwood_bandits_lvl_4_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_09_01.mgp | _prim_tmp_commontype119 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_09_01.mgp | _prim_tmp_commontype120 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_09_01.mgp | _prim_tmp_commontype125 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_09_01.mgp | _prim_tmp_commontype126 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_05_01.mgp | _prim_tmp_commontype159 | None | Darkwood_bandits_lvl_9_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_05_01.mgp | _prim_tmp_commontype160 | None | Darkwood_bandits_lvl_9_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_05_01.mgp | _prim_tmp_commontype187 | None | Darkwood_bandits_lvl_9_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_10_01.mgp | _prim_tmp_commontype109 | None | Darkwood_bandits_lvl_4_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_10_01.mgp | _prim_tmp_commontype110 | None | Darkwood_bandits_lvl_4_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_10_01.mgp | _prim_tmp_commontype112 | None | Darkwood_bandits_lvl_4_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_15_01.mgp | _prim_tmp_commontype130 | None | Darkwood_rathound_lvl_25_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_21_01.mgp | _prim_tmp_commontype190 | None | Darkwood_rathound_lvl_25_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_21_01.mgp | _prim_tmp_commontype191 | None | Darkwood_rathound_lvl_25_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_14_01.mgp | _prim_DarkQueen | DarkQueenForm1 | None | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_14_01.mgp | _prim_ReneActor | ReneHG | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_14_01.mgp | _prim_tmp_commontype136 | None | Darkwood_rathound_lvl_25_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_14_01.mgp | _prim_KingActor01 | King | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_14_01.mgp | _prim_EdwardActor01 | Edward | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_14_01.mgp | _prim_tmp_commontype134 | None | Darkwood_rathound_lvl_25_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_14_01.mgp | _prim_NPC_hellman04 | HellMan_DW_final | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_14_01.mgp | _prim_NPC_hellman05 | HellMan_quest_1 | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_11_01.mgp | _prim_tmp_commontype1_001 | None | Darkwood_white_ghosts_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_16_01.mgp | _prim_tmp_commontype115 | None | Darkwood_bandits_lvl_4_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_16_01.mgp | _prim_tmp_commontype117 | None | Darkwood_bandits_lvl_4_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_16_01.mgp | _prim_tmp_commontype195 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_16_01.mgp | _prim_tmp_commontype196 | None | Darkwood_bandits_lvl_3_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_27_01.mgp | _prim_tmp_commontype138 | None | Darkwood_cultists_lvl_6_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_27_01.mgp | _prim_tmp_commontype139 | None | Darkwood_cultists_lvl_6_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_07_01.mgp | _prim_tmp_commontype03 | None | Darkwood_white_ghosts_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_07_01.mgp | _prim_tmp_commontype06 | None | Darkwood_white_ghosts_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_07_01.mgp | _prim_tmp_commontype07 | None | Darkwood_white_ghosts_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_07_01.mgp | _prim_tmp_commontype15 | None | Darkwood_white_ghosts_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_07_01.mgp | _prim_tmp_commontype16 | None | Darkwood_white_ghosts_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_07_01.mgp | _prim_tmp_commontype09 | None | Darkwood_white_ghosts_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_GCV_quest | GCV_DemonChild | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_Bandit_Hunter | GCV_BanditHunt_a | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor | GCV_GUARD_P1_001 | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor01 | GCV_GUARD_P2_001 | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor02 | GCV_GUARD_P4_001 | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor03 | GCV_GUARD_P1_002 | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor07 | GCV_P1_004 | None | 100 | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor08 | GCV_P1_005 | None | 100 | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor11 | GCV_P1_008 | None | 100 | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor17 | GCV_P2_005 | None | 100 | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor18 | GCV_P3_001 | None | 50 | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor20 | GCV_P3_003 | None | 50 | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor21 | GCV_P3_004 | None | 50 | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor24 | GCV_P4_002 | None | 50 | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor27 | GCV_GUARD_P2_002 | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_CastleGuard01 | DoorCultists2 | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_CastleGuard06 | DoorCultists2 | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | _prim_NPC_flavor28 | DW_Merchant | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_28_01.mgp | _prim_tmp_commontype143 | None | Darkwood_cultists_lvl_12_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_28_01.mgp | _prim_tmp_commontype144 | None | Darkwood_cultists_lvl_12_CommonType1 | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_28_01.mgp | _prim_NPC_Eddie | WanderingEddie | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_28_01.mgp | _prim_Celeste | DummyLightningFaery | None | None | None | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_29_01.mgp | _prim_Monster_StatueRed | DW_Statue | None | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_29_01.mgp | _prim_Monster_Child01 | DW_Cherub_RE | None | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_29_01.mgp | _prim_Monster_Child03 | DW_Cherub_RE | None | 100 | Idle | INFERENCE |
| 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_29_01.mgp | _prim_Monster_Child02 | DW_Cherub_RE | None | 100 | Idle | INFERENCE |


### CharacterProperties profiles

This table joins listed actor rows to their original table references. AI script/type values come from the linked AI table row; loot, skills, model and animation remain source-table links. It does not prove the actor spawns in this level.

| Actor | Model | Animation table | AI row | AI facts | Skill tree | Loot | Sounds |
| --- | --- | --- | --- | --- | --- | --- | --- |
| DW_Cherub_RE | child [12] | ChildStatue [8] | Bandit [7] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | NoLoot [188] | Default (reference RESOLVED; source row RESOLVED) |
| DW_Merchant | thamos_merchant [104] | thamos_merchant [79] | Merchant [39] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 7} | -1 | Abbey_Boss_Loot [2] | Default (reference RESOLVED; source row RESOLVED) |
| DW_Statue | statue [98] | Statue [68] | DW_Statue [14] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "DW_Statue", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | DWv1_Champion_Loot [60] | Default (reference RESOLVED; source row RESOLVED) |
| DarkQueenForm1 | dark_queen1stform [21] | DarkQueenForm1 [14] | DarkQueen1 [15] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "DarkQueen1", "SelfFX": -1, "Trophy": -1, "Type": 4} | DarkQueen1 [4] | NoLoot [188] | Default (reference RESOLVED; source row RESOLVED) |
| DoorCultists2 | cultist [14] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| DummyLightningFaery | faeries_02_celeste [34] | Faery [23] | Celest [11] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| Edward | edward [26] | Edward [20] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_BanditHunt_a | f_merchant [30] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_DemonChild | old_peasant [73] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_GUARD_P1_001 | jeremo_guard [56] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_GUARD_P1_002 | jeremo_guard [56] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_GUARD_P2_001 | jeremo_guard [56] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_GUARD_P2_002 | jeremo_guard [56] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_GUARD_P4_001 | jeremo_guard [56] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_P1_004 | young_peasant [115] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_P1_005 | old_peasant [73] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_P1_008 | old_peasant [73] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_P2_005 | old_peasant [73] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_P3_001 | young_peasant [115] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_P3_003 | castle_maid [10] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_P3_004 | young_peasant [115] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| GCV_P4_002 | castle_maid [10] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| HellMan_DW_final | old_man [72] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| HellMan_quest_1 | old_man [72] | NPC_Friendly [46] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| IcyHub_StillStatue | statue [98] | StillStatue [69] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| King | king [57] | King [30] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| ReneHG | priest_good [74] | NPC_Priest [47] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |


## Bosses

The source HUD predicate `(AI.Flags & 4) != 0` is proven for 59 CharacterProperties-linked AI rows. 6 name-only markers remain supplemental unresolved candidates. This source marker does not establish encounter placement or runtime spawn conditions.

| Boss flag rows | Name-only supplements | Act-linked candidates | Placement status |
| --- | --- | --- | --- |
| 59 | 6 | 2 | INFERENCE / unresolved runtime placement |


## Dialogues

The dialog table is inventoried globally. Serialized `Script_StartDialog` occurrences are listed as command rows; mapping them to dialog table IDs remains unresolved.

Dialog table rows: 55; StartDialog commands: 263.

## Serialized level scripts

The configured `.pyscript` names resolve to original serialized `_pyscriptnames.bin` and `_pyscripts.bin` pairs. Atlas decodes script names, command kinds, and raw fields without executing callbacks.

| Scene | Script file | Scripts | Commands | Decode status |
| --- | --- | --- | --- | --- |
| 003_darkwood.mlx | data\PyData\scripts\003_DARKWOOD.pyscript | 105 | 1177 | DONE |


### Command readiness by native kind

The native factory/reader addresses come from the recovered C1 descriptor map. Port anchors identify current dispatch code only; PARTIAL/UNKNOWN does not establish complete source parity or runtime verification.

| Kind | Command | Count | Factory address | Reader address | Port status | Code anchors |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | Script_ExecScript | 162 | 0x458d40 | 0x455628 | UNKNOWN |  |
| 3 | Script_CONSOLE | 20 | 0x458c68 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:680, port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:66 |
| 5 | Script_PlayCamera | 19 | 0x458bd8 | 0x455628 | UNKNOWN |  |
| 6 | Script_SetCameraClip | 3 | 0x458b90 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:681 |
| 8 | Script_SetCameraTarget | 32 | 0x458b00 | 0x455628 | UNKNOWN |  |
| 10 | Script_StartDialog | 263 | 0x458a70 | 0x459034 | UNKNOWN |  |
| 12 | Script_WaitDialog | 253 | 0x4589e0 | 0x455628 | UNKNOWN |  |
| 13 | Script_PlaySound | 9 | 0x458998 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:755 |
| 14 | Script_StopSound | 1 | 0x458950 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:732 |
| 15 | Script_PlayLevelMusic | 2 | 0x458908 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:769 |
| 20 | Script_PlayEffect | 13 | 0x4587a0 | 0x4598b4 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:702 |
| 21 | Script_StopEffect | 8 | 0x458758 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:726 |
| 22 | Script_ShowFlash | 1 | 0x45870c | 0x45983c | UNKNOWN |  |
| 23 | Script_HideFlash | 1 | 0x4586c0 | 0x4597e8 | UNKNOWN |  |
| 24 | Script_LockCharacter | 81 | 0x458678 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 25 | Script_UnlockCharacter | 81 | 0x458630 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 26 | Script_Wait | 36 | 0x4585e0 | 0x455628 | UNKNOWN |  |
| 29 | Script_PutCharacterInLimbus | 2 | 0x458508 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:669 |
| 30 | Script_SpawnCharacter | 2 | 0x4584c0 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:37 |
| 31 | Script_PutCharacterInIdle | 1 | 0x458478 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:38 |
| 39 | Script_StopActor | 2 | 0x458238 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:38 |
| 40 | Script_MoveActor | 4 | 0x4581e4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:574 |
| 41 | Script_LookActor | 60 | 0x45819c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:637 |
| 42 | Script_ShowActor | 16 | 0x458154 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:661 |
| 43 | Script_HideActor | 16 | 0x45810c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:662 |
| 45 | Script_PlayActorAnim | 29 | 0x4595c8 | 0x45a2ec | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:595 |
| 46 | Script_SetActorPosition | 29 | 0x45807c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:619 |
| 54 | Script_OpenDoor | 12 | 0x457e34 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:772 |
| 55 | Script_CloseDoor | 4 | 0x457de4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:773 |
| 64 | Script_ChangeLevel | 2 | 0x457b4c | 0x455628 | UNKNOWN |  |
| 74 | Script_EnqueueCharMenuTutorialMessage | 4 | 0x45787c | 0x455628 | UNKNOWN |  |
| 77 | Script_LockTutorial | 2 | 0x4577a4 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:53 |
| 78 | Script_DoTutorial | 2 | 0x45775c | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:54 |
| 79 | Script_FlushMessages | 5 | 0x457714 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:63 |


### Actor, dialogue, transition, and audio command details

The table below preserves exact command labels and decoded string operands. A `Script_SpawnCharacter` target is an authored object-name argument; it does not prove which conditional branch ran in gameplay.

| Scene | Script name | Command | String operands | Count |
| --- | --- | --- | --- | --- |
| 003_darkwood.mlx | BanditHunt_Start | Script_StartDialog |  | 5 |
| 003_darkwood.mlx | BanditHunt_Start | Script_WaitDialog |  | 5 |
| 003_darkwood.mlx | BanditHunt_Success | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | BanditHunt_Success | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | Cathedral_Locked | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | Cathedral_Locked | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | Darkwood_Gated | Script_LookActor | LocalPlayer; _prim_Faery | 1 |
| 003_darkwood.mlx | Darkwood_Gated | Script_LookActor | LocalPlayer; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Darkwood_Gated | Script_LookActor | _prim_CultistCam; LocalPlayer | 1 |
| 003_darkwood.mlx | Darkwood_Gated | Script_LookActor | _prim_CultistCam; _prim_Faery | 1 |
| 003_darkwood.mlx | Darkwood_Gated | Script_LookActor | _prim_CultistCam; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Darkwood_Gated | Script_LookActor | _prim_Faery; LocalPlayer | 1 |
| 003_darkwood.mlx | Darkwood_Gated | Script_SetActorPosition | _prim_CastleFaerySpot; _prim_Faery | 1 |
| 003_darkwood.mlx | Darkwood_Gated | Script_SetActorPosition | _prim_CastlePriestSpot; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Darkwood_Gated | Script_SetActorPosition | _prim_CastlePrinceSpot; LocalPlayer | 1 |
| 003_darkwood.mlx | Darkwood_Gated | Script_StartDialog |  | 6 |
| 003_darkwood.mlx | Darkwood_Gated | Script_WaitDialog |  | 6 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_LookActor | LocalPlayer; _prim_Faery | 1 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_LookActor | LocalPlayer; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_LookActor | _prim_Faery; LocalPlayer | 1 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_LookActor | _prim_NPC_PriestGood; LocalPlayer | 1 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_MoveActor | _prim_Waypoint_Enterwalkto; LocalPlayer | 1 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_SetActorPosition | _prim_Waypoint_EnterFaerySpot; _prim_Faery | 1 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_SetActorPosition | _prim_Waypoint_EnterPriestSpot; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_ShowActor | _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_StartDialog |  | 9 |
| 003_darkwood.mlx | Darkwood_IntroCinematic | Script_WaitDialog |  | 9 |
| 003_darkwood.mlx | Demon_Child_QuestStart | Script_StartDialog |  | 4 |
| 003_darkwood.mlx | Demon_Child_QuestStart | Script_WaitDialog |  | 4 |
| 003_darkwood.mlx | DesertGuardian_Locked | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | DesertGuardian_Locked | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | Door_Teleport | Script_PlaySound |  | 1 |
| 003_darkwood.mlx | Door_Teleport | Script_SetActorPosition | _prim_Waypoint_HGCenter; LocalPlayer | 1 |
| 003_darkwood.mlx | Door_Teleport | Script_SetActorPosition | _prim_Waypoint_HGCenter_LookTo; LocalPlayer | 1 |
| 003_darkwood.mlx | Entrypoint_fromSwamp | Script_MoveActor | _prim_Waypoint_Enterwalkto; LocalPlayer | 1 |
| 003_darkwood.mlx | Entrypoint_fromSwamp | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | FindAxe_PostAccept | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | FindAxe_PostAccept | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | FindAxe_QuestStart | Script_LookActor | LocalPlayer; _prim_NPC_WoodCutterC | 1 |
| 003_darkwood.mlx | FindAxe_QuestStart | Script_LookActor | _prim_NPC_WoodCutterC; LocalPlayer | 1 |
| 003_darkwood.mlx | FindAxe_QuestStart | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | FindAxe_QuestStart | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | FindAxe_Success | Script_StartDialog |  | 4 |
| 003_darkwood.mlx | FindAxe_Success | Script_WaitDialog |  | 4 |
| 003_darkwood.mlx | GCV1_guard_a | Script_LookActor | LocalPlayer; _prim_NPC_flavor | 1 |
| 003_darkwood.mlx | GCV1_guard_a | Script_PlayActorAnim | _prim_NPC_flavor | 1 |
| 003_darkwood.mlx | GCV1_guard_a | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV1_guard_a | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV1_guard_b | Script_LookActor | LocalPlayer; _prim_NPC_flavor03 | 1 |
| 003_darkwood.mlx | GCV1_guard_b | Script_PlayActorAnim | _prim_NPC_flavor03 | 1 |
| 003_darkwood.mlx | GCV1_guard_b | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV1_guard_b | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GCV1_villager_a | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV1_villager_a | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV1_villager_b | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV1_villager_b | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV1_villager_c | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | GCV1_villager_c | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | GCV1_villager_d | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV1_villager_d | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV1_villager_e | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV1_villager_e | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV1_villager_f | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV1_villager_f | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GCV1_villager_g | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV1_villager_g | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GCV2_guard_a | Script_LookActor | LocalPlayer; _prim_NPC_flavor01 | 1 |
| 003_darkwood.mlx | GCV2_guard_a | Script_PlayActorAnim | _prim_NPC_flavor01 | 1 |
| 003_darkwood.mlx | GCV2_guard_a | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | GCV2_guard_a | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | GCV2_guard_b | Script_LookActor | LocalPlayer; _prim_NPC_flavor27 | 1 |
| 003_darkwood.mlx | GCV2_guard_b | Script_PlayActorAnim | _prim_NPC_flavor27 | 1 |
| 003_darkwood.mlx | GCV2_guard_b | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV2_guard_b | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV2_villager_a | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV2_villager_a | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV2_villager_b | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV2_villager_b | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV2_villager_c | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV2_villager_c | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GCV2_villager_d | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV2_villager_d | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV2_villager_e | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV2_villager_e | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV3_villager_a | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV3_villager_a | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GCV3_villager_b | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV3_villager_b | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GCV3_villager_c | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV3_villager_c | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GCV3_villager_d | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV3_villager_d | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GCV3_villager_e | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV3_villager_e | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV4_gaurd_a | Script_LookActor | LocalPlayer; _prim_NPC_flavor02 | 1 |
| 003_darkwood.mlx | GCV4_gaurd_a | Script_PlayActorAnim | _prim_NPC_flavor02 | 1 |
| 003_darkwood.mlx | GCV4_gaurd_a | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV4_gaurd_a | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GCV4_villager_a | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV4_villager_a | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV4_villager_b | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GCV4_villager_b | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GCV4_villager_c | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | GCV4_villager_c | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | GhostHunt_PostAccept | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GhostHunt_PostAccept | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GhostHunt_QuestStart | Script_LookActor | LocalPlayer; _prim_NPC_WoodCutterE | 1 |
| 003_darkwood.mlx | GhostHunt_QuestStart | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | GhostHunt_QuestStart | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | GoToRedDesert | Script_LookActor | LocalPlayer; _prim_Faery | 1 |
| 003_darkwood.mlx | GoToRedDesert | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | GoToRedDesert | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | Goto_Underworld | Script_ChangeLevel | UNDERWORLD_HUB | 1 |
| 003_darkwood.mlx | HG2_NoEscape | Script_LookActor | LocalPlayer; _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_NoEscape | Script_SetActorPosition | _prim_Waypoint_HGCenter; _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_NoEscape | Script_SetActorPosition | _prim_Waypoint_PrinceSpot; LocalPlayer | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_HideActor | LocalPlayer | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_HideActor | _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_HideActor | _prim_Faery | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_LookActor | _prim_Waypoint_HGCenter_LookTo; LocalPlayer | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_LookActor | _prim_Waypoint_ReneSpot2; _prim_ReneActor | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_OpenDoor | _prim_QueenDoor_01 | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_OpenDoor | _prim_QueenDoor_02 | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_OpenDoor | _prim_QueenDoor_03 | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_OpenDoor | _prim_QueenDoor_04 | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_PlaySound |  | 1 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_StartDialog |  | 16 |
| 003_darkwood.mlx | HG2_boss_defeated | Script_WaitDialog |  | 16 |
| 003_darkwood.mlx | HG2_boss_intro | Script_CloseDoor | _prim_QueenDoor_01 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_CloseDoor | _prim_QueenDoor_02 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_CloseDoor | _prim_QueenDoor_03 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_CloseDoor | _prim_QueenDoor_04 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_HideActor | LocalPlayer | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_HideActor | _prim_Celeste | 2 |
| 003_darkwood.mlx | HG2_boss_intro | Script_HideActor | _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_HideActor | _prim_EdwardActor01 | 2 |
| 003_darkwood.mlx | HG2_boss_intro | Script_HideActor | _prim_Faery | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_HideActor | _prim_KingActor01 | 2 |
| 003_darkwood.mlx | HG2_boss_intro | Script_HideActor | _prim_ReneActor | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_LookActor | LocalPlayer; _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_LookActor | _prim_DarkQueen; LocalPlayer | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_LookActor | _prim_Waypoint_HGCenter_LookTo; LocalPlayer | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_LookActor | _prim_Waypoint_HGCenter_LookTo; _prim_Celeste | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_LookActor | _prim_Waypoint_HGCenter_LookTo; _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_LookActor | _prim_Waypoint_HGCenter_LookTo; _prim_EdwardActor01 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_LookActor | _prim_Waypoint_HGCenter_LookTo; _prim_KingActor01 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_LookActor | _prim_Waypoint_HGCenter_LookTo; _prim_ReneActor | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_PlayActorAnim | LocalPlayer | 5 |
| 003_darkwood.mlx | HG2_boss_intro | Script_PlayActorAnim | _prim_Celeste | 5 |
| 003_darkwood.mlx | HG2_boss_intro | Script_PlayActorAnim | _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_PlayActorAnim | _prim_EdwardActor01 | 2 |
| 003_darkwood.mlx | HG2_boss_intro | Script_PlayActorAnim | _prim_KingActor01 | 2 |
| 003_darkwood.mlx | HG2_boss_intro | Script_PlayActorAnim | _prim_ReneActor | 9 |
| 003_darkwood.mlx | HG2_boss_intro | Script_PlaySound |  | 5 |
| 003_darkwood.mlx | HG2_boss_intro | Script_PutCharacterInIdle | _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_SetActorPosition | _prim_Waypoint_CelesteHide; _prim_Celeste | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_SetActorPosition | _prim_Waypoint_HGCenter; LocalPlayer | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_SetActorPosition | _prim_Waypoint_HGCenter; _prim_Celeste | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_SetActorPosition | _prim_Waypoint_HGCenter; _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_SetActorPosition | _prim_Waypoint_HGCenter; _prim_EdwardActor01 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_SetActorPosition | _prim_Waypoint_HGCenter; _prim_KingActor01 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_SetActorPosition | _prim_Waypoint_HGCenter; _prim_ReneActor | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_SetActorPosition | _prim_Waypoint_PrinceSpot; LocalPlayer | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_SetActorPosition | _prim_Waypoint_Queenspot; _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_ShowActor | LocalPlayer | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_ShowActor | _prim_Celeste | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_ShowActor | _prim_DarkQueen | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_ShowActor | _prim_EdwardActor01 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_ShowActor | _prim_Faery | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_ShowActor | _prim_KingActor01 | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_ShowActor | _prim_ReneActor | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_StartDialog |  | 15 |
| 003_darkwood.mlx | HG2_boss_intro | Script_StopSound |  | 1 |
| 003_darkwood.mlx | HG2_boss_intro | Script_WaitDialog |  | 15 |
| 003_darkwood.mlx | HG2_boss_levelchange | Script_ChangeLevel | UNDERWORLD_HUB | 1 |
| 003_darkwood.mlx | HarvestMerchant_Activate | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | HarvestMerchant_Activate | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | HarvestMerchant_Found | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | HarvestMerchant_Found | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | HarvestMerchant_Start | Script_StartDialog |  | 4 |
| 003_darkwood.mlx | HarvestMerchant_Start | Script_WaitDialog |  | 4 |
| 003_darkwood.mlx | HarvestMerchant_Success | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | HarvestMerchant_Success | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_LookActor | LocalPlayer; _prim_Faery | 2 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_LookActor | LocalPlayer; _prim_NPC_PriestGood | 2 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_LookActor | _prim_Faery; LocalPlayer | 1 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_LookActor | _prim_NPC_PriestGood; LocalPlayer | 1 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_LookActor | _prim_Waypoit_HGLookat; LocalPlayer | 1 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_LookActor | _prim_Waypoit_HGLookat; _prim_NPC_PriestGood | 2 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_SetActorPosition | _prim_FairyMark_01; _prim_Faery | 1 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_SetActorPosition | _prim_PlayerMark_01; LocalPlayer | 1 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_SetActorPosition | _prim_PriestMark_01; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_StartDialog |  | 7 |
| 003_darkwood.mlx | Hellgate_Dialog | Script_WaitDialog |  | 7 |
| 003_darkwood.mlx | Hellman_aftAbbey | Script_StartDialog |  | 4 |
| 003_darkwood.mlx | Hellman_aftAbbey | Script_WaitDialog |  | 4 |
| 003_darkwood.mlx | Hellman_aftAir | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | Hellman_aftAir | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | Hellman_aftEarth | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | Hellman_aftEarth | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | Hellman_aftIcy | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | Hellman_aftIcy | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | Hellman_aftInit | Script_StartDialog |  | 6 |
| 003_darkwood.mlx | Hellman_aftInit | Script_WaitDialog |  | 6 |
| 003_darkwood.mlx | Infected_Guard_Start | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | Infected_Guard_Start | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | Infected_Guard_Success | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | Infected_Guard_Success | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | LostMerchant_Intro | Script_LookActor | LocalPlayer; _prim_tmp_questtype1_LostMerch1 | 1 |
| 003_darkwood.mlx | LostMerchant_Intro | Script_StartDialog |  | 5 |
| 003_darkwood.mlx | LostMerchant_Intro | Script_WaitDialog |  | 5 |
| 003_darkwood.mlx | LostMerchant_PostAccept | Script_HideActor | _prim_tmp_questtype1_LostMerch1 | 1 |
| 003_darkwood.mlx | LostMerchant_PostAccept | Script_SetActorPosition | _prim_Waypoint_merch1; _prim_tmp_questtype1_LostMerch1 | 1 |
| 003_darkwood.mlx | LostMerchant_PostAccept | Script_SetActorPosition | _prim_Waypoint_merch2; _prim_tmp_questtype1_LostMerch2 | 1 |
| 003_darkwood.mlx | LostMerchant_PostAccept | Script_SpawnCharacter | _prim_tmp_questtype1_LostMerch2 | 1 |
| 003_darkwood.mlx | LostMerchant_PostAccept | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | LostMerchant_PostAccept | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | LostMerchant_Success | Script_HideActor | _prim_tmp_questtype1_LostMerch2 | 1 |
| 003_darkwood.mlx | LostMerchant_Success | Script_LookActor | LocalPlayer; _prim_tmp_questtype1_LostMerch2 | 1 |
| 003_darkwood.mlx | LostMerchant_Success | Script_LookActor | _prim_tmp_questtype1_LostMerch2; LocalPlayer | 1 |
| 003_darkwood.mlx | LostMerchant_Success | Script_MoveActor | _prim_Waypoint_MerchSpot01; _prim_tmp_questtype1_LostMerch2 | 1 |
| 003_darkwood.mlx | LostMerchant_Success | Script_MoveActor | _prim_Waypoint_PlayerSpot03; LocalPlayer | 1 |
| 003_darkwood.mlx | LostMerchant_Success | Script_SetActorPosition | _prim_Waypoint_MerchSpot01; _prim_tmp_questtype1_MerchSuccess | 1 |
| 003_darkwood.mlx | LostMerchant_Success | Script_SetActorPosition | _prim_Waypoint_MerchSpot02; _prim_tmp_questtype1_LostMerch2 | 1 |
| 003_darkwood.mlx | LostMerchant_Success | Script_SpawnCharacter | _prim_tmp_questtype1_MerchSuccess | 1 |
| 003_darkwood.mlx | LostMerchant_Success | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | LostMerchant_Success | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | MonkEscort_Activate | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | MonkEscort_Activate | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | MonkEscort_Start | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | MonkEscort_Start | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | MonkEscort_Success | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | MonkEscort_Success | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | MoveEddie | Script_SetActorPosition | LocalPlayer; _prim_NPC_Eddie | 1 |
| 003_darkwood.mlx | MoveEddie | Script_ShowActor | _prim_NPC_Eddie | 2 |
| 003_darkwood.mlx | MoveRene | Script_SetActorPosition | LocalPlayer; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | MoveRene | Script_ShowActor | _prim_NPC_PriestGood | 2 |
| 003_darkwood.mlx | OpenRedDoorsToCemetary | Script_OpenDoor | _prim_Door_cemetary_01 | 1 |
| 003_darkwood.mlx | OpenRedDoorsToCemetary | Script_OpenDoor | _prim_Door_cemetary_02 | 1 |
| 003_darkwood.mlx | OpenRedDoorsToRedDesert | Script_OpenDoor | _prim_RedDoor_RedDesert | 1 |
| 003_darkwood.mlx | OpenRedDoorsToRedDesert | Script_OpenDoor | _prim_RedDoor_RedDesert01 | 1 |
| 003_darkwood.mlx | OpenRedDoorsToRedDesert | Script_OpenDoor | _prim_RedDoor_RedDesert02 | 1 |
| 003_darkwood.mlx | OpenRedDoorsToRedDesert | Script_OpenDoor | _prim_RedDoor_RedDesert03 | 1 |
| 003_darkwood.mlx | OpenRedDoorsToRedDesert | Script_OpenDoor | _prim_RedDoor_RedDesert04 | 1 |
| 003_darkwood.mlx | OpenRedDoorsToRedDesert | Script_OpenDoor | _prim_RedDoor_RedDesert05 | 1 |
| 003_darkwood.mlx | P1_merchant_b_initial | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | P1_merchant_b_initial | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | P1_merchant_f | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | P1_merchant_f | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | POK_preintro | Script_LookActor | LocalPlayer; _prim_Faery | 1 |
| 003_darkwood.mlx | POK_preintro | Script_LookActor | _prim_Faery; LocalPlayer | 1 |
| 003_darkwood.mlx | POK_preintro | Script_LookActor | _prim_TombLookTo; LocalPlayer | 1 |
| 003_darkwood.mlx | POK_preintro | Script_StartDialog |  | 4 |
| 003_darkwood.mlx | POK_preintro | Script_WaitDialog |  | 4 |
| 003_darkwood.mlx | PlayerUnhide | Script_ShowActor | LocalPlayer | 1 |
| 003_darkwood.mlx | PlayerUnhide | Script_ShowActor | _prim_Faery | 1 |
| 003_darkwood.mlx | Post_Cathedral | Script_StartDialog |  | 6 |
| 003_darkwood.mlx | Post_Cathedral | Script_WaitDialog |  | 6 |
| 003_darkwood.mlx | Post_Escape | Script_HideActor | LocalPlayer | 1 |
| 003_darkwood.mlx | Post_Escape | Script_LookActor | LocalPlayer; _prim_Faery | 1 |
| 003_darkwood.mlx | Post_Escape | Script_LookActor | LocalPlayer; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Post_Escape | Script_LookActor | _prim_Faery; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Post_Escape | Script_LookActor | _prim_NPC_PriestGood; LocalPlayer | 1 |
| 003_darkwood.mlx | Post_Escape | Script_LookActor | _prim_NPC_PriestGood; _prim_Faery | 1 |
| 003_darkwood.mlx | Post_Escape | Script_PlaySound |  | 1 |
| 003_darkwood.mlx | Post_Escape | Script_SetActorPosition | _prim_FaeryEscape01\|; _prim_Faery | 1 |
| 003_darkwood.mlx | Post_Escape | Script_SetActorPosition | _prim_PriestEscape01; _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Post_Escape | Script_ShowActor | LocalPlayer | 1 |
| 003_darkwood.mlx | Post_Escape | Script_ShowActor | _prim_NPC_PriestGood | 1 |
| 003_darkwood.mlx | Post_Escape | Script_StartDialog |  | 10 |
| 003_darkwood.mlx | Post_Escape | Script_WaitDialog |  | 10 |
| 003_darkwood.mlx | ResumeLevelMusic | Script_PlayLevelMusic |  | 1 |
| 003_darkwood.mlx | RingMerchant_Activated | Script_LookActor | LocalPlayer; _prim_NPC_LostRing | 1 |
| 003_darkwood.mlx | RingMerchant_Activated | Script_LookActor | _prim_NPC_LostRing; LocalPlayer | 1 |
| 003_darkwood.mlx | RingMerchant_Activated | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | RingMerchant_Activated | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | RingMerchant_Found | Script_LookActor | LocalPlayer; prim_Faery | 1 |
| 003_darkwood.mlx | RingMerchant_Found | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | RingMerchant_Found | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | RingMerchant_Start | Script_LookActor | LocalPlayer; _prim_NPC_LostRing | 1 |
| 003_darkwood.mlx | RingMerchant_Start | Script_LookActor | _prim_NPC_LostRing; LocalPlayer | 1 |
| 003_darkwood.mlx | RingMerchant_Start | Script_StartDialog |  | 4 |
| 003_darkwood.mlx | RingMerchant_Start | Script_WaitDialog |  | 4 |
| 003_darkwood.mlx | RingMerchant_Success | Script_LookActor | LocalPlayer; _prim_NPC_LostRing | 1 |
| 003_darkwood.mlx | RingMerchant_Success | Script_LookActor | _prim_NPC_LostRing; LocalPlayer | 1 |
| 003_darkwood.mlx | RingMerchant_Success | Script_StartDialog |  | 5 |
| 003_darkwood.mlx | RingMerchant_Success | Script_WaitDialog |  | 5 |
| 003_darkwood.mlx | TUT_COMBO | Script_StartDialog |  | 4 |
| 003_darkwood.mlx | TUT_COMBO | Script_WaitDialog |  | 4 |
| 003_darkwood.mlx | Tomb_Dialog | Script_LookActor | _prim_TombLookTo; LocalPlayer | 1 |
| 003_darkwood.mlx | Tomb_Dialog | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | Tomb_Dialog | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | Tomb_Intro | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | Tomb_Intro | Script_WaitDialog |  | 1 |
| 003_darkwood.mlx | Tut_Minimap | Script_LookActor | LocalPlayer; _prim_Faery | 2 |
| 003_darkwood.mlx | Tut_Minimap | Script_StartDialog |  | 6 |
| 003_darkwood.mlx | Tut_Minimap | Script_WaitDialog |  | 6 |
| 003_darkwood.mlx | enterLocation_Darkwood | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | enterLocation_Darkwood_central | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | enterLocation_Darkwood_east | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | enterLocation_Darkwood_hell_gate | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | enterLocation_Darkwood_north | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | enterLocation_Darkwood_south | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | enterLocation_Darkwood_west | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | enterLocation_Gated | Script_PlaySound |  | 1 |
| 003_darkwood.mlx | enterLocation_Gated | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | enterLocation_Graveyard | Script_StartDialog |  | 1 |
| 003_darkwood.mlx | exitLocation_Gated | Script_PlayLevelMusic |  | 1 |
| 003_darkwood.mlx | inspect_hell_aftAbbeyA | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | inspect_hell_aftAbbeyA | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | inspect_hell_aftAbbeyB | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftAbbeyB | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftAir | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftAir | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftEarth | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftEarth | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftGothicusB | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftGothicusB | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftInit | Script_StartDialog |  | 2 |
| 003_darkwood.mlx | inspect_hell_aftInit | Script_WaitDialog |  | 2 |
| 003_darkwood.mlx | inspect_hell_aftLighthouse | Script_StartDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftLighthouse | Script_WaitDialog |  | 3 |
| 003_darkwood.mlx | inspect_hell_aftUnderworld | Script_StartDialog |  | 6 |
| 003_darkwood.mlx | inspect_hell_aftUnderworld | Script_WaitDialog |  | 6 |
| 003_darkwood.mlx | inspect_hell_aftWater | Script_StartDialog |  | 4 |
| 003_darkwood.mlx | inspect_hell_aftWater | Script_WaitDialog |  | 4 |


Raw strings are recorded in `../data/campaign-references.json`. Command labels and recovered field offsets identify spawn, transition, dialogue, and audio command records; runtime branch conditions and dialogue-table IDs remain unresolved.

## Source-linked asset detail pages

Each page joins the source asset inventory to scene/MGP reference rows and carries the resolver status, confidence, rule, port metadata, and source path.

| Asset | Stable ID | Source status | Page |
| --- | --- | --- | --- |
| 3d/modules/darkwood/mgp/darkwood_08_01.mgp | 0025b3f00801cc848c34 | ORPHAN | [details](assets/0025b3f00801cc848c34.md) |
| 3d/modules/darkwood/mvp/darkwood_27_01.mvp | 02e48d4675fcb65b15b2 | ORPHAN | [details](assets/02e48d4675fcb65b15b2.md) |
| sounds/sfx_edward_hurt_2.wav | 03dc239998e958b1a16f | ORPHAN | [details](assets/03dc239998e958b1a16f.md) |
| file003895.dat | 03e3a7270b8c0b8df5e9 | ORPHAN | [details](assets/03e3a7270b8c0b8df5e9.md) |
| 3d/modules/darkwood/mgp/darkwood_07_01.mgp | 040ecdb97909363f6da9 | ORPHAN | [details](assets/040ecdb97909363f6da9.md) |
| file001549.dat | 04d36158e3691775581d | ORPHAN | [details](assets/04d36158e3691775581d.md) |
| sounds/sfx_dark_queen_die.wav | 05beb05dadb4fef0420e | ORPHAN | [details](assets/05beb05dadb4fef0420e.md) |
| sounds/sfx_edward_attack.wav | 0718bbb36603ba620f57 | ORPHAN | [details](assets/0718bbb36603ba620f57.md) |
| 3d/modules/darkwood/mgp/darkwood_09_01.mgp | 077617a853c9944dfcab | ORPHAN | [details](assets/077617a853c9944dfcab.md) |
| 3d/modules/darkwood/mvp/darkwood_27_01.mvp | 093873740b64ea17120f | ORPHAN | [details](assets/093873740b64ea17120f.md) |
| 3d/characters/prince/animations/prince_1hand_post_combo_03_moving.bdae | 095e5e36cfd5ef97a2fd | ORPHAN | [details](assets/095e5e36cfd5ef97a2fd.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_pushback.bdae | 0a0215e15ba2ef940576 | ORPHAN | [details](assets/0a0215e15ba2ef940576.md) |
| 3d/modules/darkwood/mgp/darkwood_13_01.mgp | 0abd8000c3489220627a | ORPHAN | [details](assets/0abd8000c3489220627a.md) |
| file004926.dat | 0acedb48f8f25124b5dc | ORPHAN | [details](assets/0acedb48f8f25124b5dc.md) |
| file002915.dat | 0c2a466ca08b1507c798 | ORPHAN | [details](assets/0c2a466ca08b1507c798.md) |
| 3d/modules/darkwood/mvp/darkwood_14_01.mvp | 0c633cee9e17480b95b8 | ORPHAN | [details](assets/0c633cee9e17480b95b8.md) |
| 3d/characters/faeries/animations/faeries_celeste_walk.bdae | 0e678c6cc573c72b0aa3 | ORPHAN | [details](assets/0e678c6cc573c72b0aa3.md) |
| 3d/modules/darkwood/mgp/darkwood_27_01.mgp | 0eed987fbb192ccf7691 | ORPHAN | [details](assets/0eed987fbb192ccf7691.md) |
| file005397.dat | 0f7fb016000001ccd3e6 | ORPHAN | [details](assets/0f7fb016000001ccd3e6.md) |
| 3d/characters/statue/animations/child_post_attack_02.bdae | 10e5d884d986189463b5 | ORPHAN | [details](assets/10e5d884d986189463b5.md) |
| 3d/characters/statue/animations/child_walk_01.bdae | 110ac2b7504afd993fac | ORPHAN | [details](assets/110ac2b7504afd993fac.md) |
| file004339.dat | 114b6c3aab22b988dc60 | ORPHAN | [details](assets/114b6c3aab22b988dc60.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_despawn.bdae | 120b2025815daf146888 | ORPHAN | [details](assets/120b2025815daf146888.md) |
| 3d/modules/darkwood/mgp/darkwood_13_01.mgp | 126f8899b0d4b6b1e01a | ORPHAN | [details](assets/126f8899b0d4b6b1e01a.md) |
| file003387.dat | 12a15ddee965cedfc1fe | ORPHAN | [details](assets/12a15ddee965cedfc1fe.md) |
| 3d/modules/darkwood/mgp/darkwood_20_01.mgp | 1632646db1bf5ebe0e68 | ORPHAN | [details](assets/1632646db1bf5ebe0e68.md) |
| 3d/interface/skill_dh2_monster_dark_queen1stform_projectile.bdae | 163edde558196306397f | ORPHAN | [details](assets/163edde558196306397f.md) |
| file000189.dat | 1673b6ad2abab647da56 | ORPHAN | [details](assets/1673b6ad2abab647da56.md) |
| 3d/characters/faeries/animations/faeries_template_anim.bdae | 178c6c68d9774e8c39b8 | ORPHAN | [details](assets/178c6c68d9774e8c39b8.md) |
| file002453.dat | 17f139b28207b64ebded | ORPHAN | [details](assets/17f139b28207b64ebded.md) |
| 3d/interface/spell_dh2_hurt_monster_madruk01_fast_bolt.bdae | 18e1f46488c24115abb1 | ORPHAN | [details](assets/18e1f46488c24115abb1.md) |
| 3d/characters/faeries/animations/faeries_celeste_spell_cast.bdae | 1966428b9a95dc571e28 | ORPHAN | [details](assets/1966428b9a95dc571e28.md) |
| scripts/ai/follower.luac | 19921b1cfbabfd09916c | ORPHAN | [details](assets/19921b1cfbabfd09916c.md) |
| 3d/modules/darkwood/mvp/darkwood_12_01.mvp | 19e71572b876e8225a25 | ORPHAN | [details](assets/19e71572b876e8225a25.md) |
| 3d/interface/spell_dh2_hurt_lightning.bdae | 1a5b26e9aed8da1ec2a0 | ORPHAN | [details](assets/1a5b26e9aed8da1ec2a0.md) |
| 3d/modules/darkwood/mvp/darkwood_02_01.mvp | 1b050f7bdc29f095f857 | ORPHAN | [details](assets/1b050f7bdc29f095f857.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_attack_melee.bdae | 1b3f97503ce9eeba3079 | ORPHAN | [details](assets/1b3f97503ce9eeba3079.md) |
| file001102.dat | 1bbec02bc4079b4cdbd1 | ORPHAN | [details](assets/1bbec02bc4079b4cdbd1.md) |
| file005308.dat | 1c0d401604a8bfd4efec | ORPHAN | [details](assets/1c0d401604a8bfd4efec.md) |
| 3d/characters/statue/animations/statue_despawn.bdae | 1f31a28f22cea2a3a57e | ORPHAN | [details](assets/1f31a28f22cea2a3a57e.md) |
| file005945.dat | 1f5804ee201ed7d0bbc4 | ORPHAN | [details](assets/1f5804ee201ed7d0bbc4.md) |
| file004264.dat | 1f96e51c93da562545d8 | ORPHAN | [details](assets/1f96e51c93da562545d8.md) |
| file000290.dat | 1fc20d26f016e81002e5 | ORPHAN | [details](assets/1fc20d26f016e81002e5.md) |
| 3d/modules/darkwood/mgp/darkwood_04_01.mgp | 20eae4a8a21440a2a1b2 | ORPHAN | [details](assets/20eae4a8a21440a2a1b2.md) |
| 3d/modules/darkwood/mvp/darkwood_20_01.mvp | 21a272f8cdff52dd5623 | ORPHAN | [details](assets/21a272f8cdff52dd5623.md) |
| 3d/characters/statue/animations/child_pushback.bdae | 21cb05b990c442a3021e | ORPHAN | [details](assets/21cb05b990c442a3021e.md) |
| 3d/modules/darkwood/mgp/darkwood_17_01.mgp | 2259a9e94a342dfaa64d | ORPHAN | [details](assets/2259a9e94a342dfaa64d.md) |
| 3d/characters/prince/animations/prince_dying_01.bdae | 22f707e9c4b8bd6a1b24 | ORPHAN | [details](assets/22f707e9c4b8bd6a1b24.md) |
| 3d/characters/statue/animations/child_pre_attack_01.bdae | 247f5ff80d1189dca516 | ORPHAN | [details](assets/247f5ff80d1189dca516.md) |
| file004243.dat | 2494119febbf53e7c980 | ORPHAN | [details](assets/2494119febbf53e7c980.md) |
| 3d/characters/statue/animations/statue_beam.bdae | 250f75c2131e0867b356 | ORPHAN | [details](assets/250f75c2131e0867b356.md) |
| sounds/sfx_statue_cherub_hurt_2.wav | 25460bf7788c4e8fe3ca | ORPHAN | [details](assets/25460bf7788c4e8fe3ca.md) |
| file002674.dat | 25f189b022e4b4c14db1 | ORPHAN | [details](assets/25f189b022e4b4c14db1.md) |
| file001059.dat | 275d8210b6aec23011cf | ORPHAN | [details](assets/275d8210b6aec23011cf.md) |
| 3d/modules/darkwood/mgp/darkwood_14_01.mgp | 27cddc4343bfaf26403f | ORPHAN | [details](assets/27cddc4343bfaf26403f.md) |
| 3d/modules/darkwood/mgp/darkwood_06_01.mgp | 27ce080c17f04f42a2e7 | ORPHAN | [details](assets/27ce080c17f04f42a2e7.md) |
| 3d/characters/npcs/f_merchant.bdae | 292ea0ee6c42f9a13ead | ORPHAN | [details](assets/292ea0ee6c42f9a13ead.md) |
| sfx_swoosh_sword_1_22.wav | 29528629ca15069ea2b7 | ORPHAN | [details](assets/29528629ca15069ea2b7.md) |
| 3d/modules/darkwood/mgp/darkwood_07_01.mgp | 2a48b850f145cc334a22 | ORPHAN | [details](assets/2a48b850f145cc334a22.md) |
| 3d/modules/darkwood/mgp/darkwood_01_01.mgp | 2d10c83fcc03c4d96e81 | ORPHAN | [details](assets/2d10c83fcc03c4d96e81.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_02_moving.bdae | 2d908c89cd2065be994e | ORPHAN | [details](assets/2d908c89cd2065be994e.md) |
| 3d/modules/darkwood/mvp/darkwood_16_01.mvp | 2dd11f3b9fac91b9b9c3 | ORPHAN | [details](assets/2dd11f3b9fac91b9b9c3.md) |
| 3d/modules/darkwood/mgp/darkwood_23_01.mgp | 2ecd5491b4429cc005c8 | ORPHAN | [details](assets/2ecd5491b4429cc005c8.md) |
| scripts/ai/rene.luac | 2fbb332d04092f3fe22e | ORPHAN | [details](assets/2fbb332d04092f3fe22e.md) |
| 3d/characters/npcs/cultist.bdae | 2fe7552e40bc0e4732bd | ORPHAN | [details](assets/2fe7552e40bc0e4732bd.md) |
| 3d/characters/prince/animations/prince_1hand_combo_01.bdae | 30f51edd3c58f864b6e6 | ORPHAN | [details](assets/30f51edd3c58f864b6e6.md) |
| 3d/modules/darkwood/mgp/darkwood_08_01.mgp | 32b1bf80e65532ec4aee | ORPHAN | [details](assets/32b1bf80e65532ec4aee.md) |
| 3d/modules/darkwood/mvp/darkwood_13_01.mvp | 32bf291e92e646de4000 | ORPHAN | [details](assets/32bf291e92e646de4000.md) |
| 3d/modules/darkwood/mgp/darkwood_10_01.mgp | 359e98ae8dd6db902950 | ORPHAN | [details](assets/359e98ae8dd6db902950.md) |
| file004550.dat | 36a779b4c6753ee83168 | ORPHAN | [details](assets/36a779b4c6753ee83168.md) |
| 3d/characters/statue/animations/child_post_stunned.bdae | 36a9df7a23e10b03b4e2 | ORPHAN | [details](assets/36a9df7a23e10b03b4e2.md) |
| file001053.dat | 37e0f64371a27f81b0a3 | ORPHAN | [details](assets/37e0f64371a27f81b0a3.md) |
| 3d/modules/darkwood/mvp/darkwood_13_01.mvp | 37eda52bd631e4378098 | ORPHAN | [details](assets/37eda52bd631e4378098.md) |
| file002998.dat | 385bc5da1d4b2f118712 | ORPHAN | [details](assets/385bc5da1d4b2f118712.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_01_moving.bdae | 39c00cf2fb0a0d7ec0ed | ORPHAN | [details](assets/39c00cf2fb0a0d7ec0ed.md) |
| file003653.dat | 39dcde38573c4f0aca9d | ORPHAN | [details](assets/39dcde38573c4f0aca9d.md) |
| 3d/interface/skill_dh2_monster_dark_queen1stform_attack_melee.bdae | 3a72b29e6bbdbc64068b | ORPHAN | [details](assets/3a72b29e6bbdbc64068b.md) |
| file001947.dat | 3b3b62ed9c6ca7f3b758 | ORPHAN | [details](assets/3b3b62ed9c6ca7f3b758.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_walk.bdae | 3bc6064d28a76cfd1990 | ORPHAN | [details](assets/3bc6064d28a76cfd1990.md) |
| 3d/characters/npcs/king.bdae | 3c3629271a8a80fef8ae | ORPHAN | [details](assets/3c3629271a8a80fef8ae.md) |
| file001603.dat | 3f0e9247b18b51eae564 | ORPHAN | [details](assets/3f0e9247b18b51eae564.md) |
| sounds/sfx_witch_magic_explosion.wav | 40418e424aaeee129552 | ORPHAN | [details](assets/40418e424aaeee129552.md) |
| file005293.dat | 4048bb0cb2c7812ba450 | ORPHAN | [details](assets/4048bb0cb2c7812ba450.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_hurt.bdae | 40d6091ce6a980fd3648 | ORPHAN | [details](assets/40d6091ce6a980fd3648.md) |
| file004662.dat | 4106322c8e21193a5912 | ORPHAN | [details](assets/4106322c8e21193a5912.md) |
| 3d/characters/npcs/thamos_merchant.bdae | 41fedada55ffb8759356 | ORPHAN | [details](assets/41fedada55ffb8759356.md) |
| 3d/characters/prince/animations/prince_1hand_combo_02.bdae | 4272f3fbd92520558594 | ORPHAN | [details](assets/4272f3fbd92520558594.md) |
| 3d/characters/prince/animations/prince_stunned.bdae | 444721ae1b4f1f410751 | ORPHAN | [details](assets/444721ae1b4f1f410751.md) |
| file004099.dat | 445e23a525acd3d1bced | ORPHAN | [details](assets/445e23a525acd3d1bced.md) |
| 3d/fx/bloodsplat.bdae | 45d6384ef2052d8258cd | ORPHAN | [details](assets/45d6384ef2052d8258cd.md) |
| 3d/modules/darkwood/mvp/darkwood_17_01.mvp | 467d590daa8ddb929574 | ORPHAN | [details](assets/467d590daa8ddb929574.md) |
| file000404.dat | 46bef23aaffc6673714d | ORPHAN | [details](assets/46bef23aaffc6673714d.md) |
| file001267.dat | 49394bc1a260dfe0c3d1 | ORPHAN | [details](assets/49394bc1a260dfe0c3d1.md) |
| sfx_skill_rogue_ass_concealing_cloud.wav | 4a523f5384ec82b3d637 | ORPHAN | [details](assets/4a523f5384ec82b3d637.md) |
| 3d/modules/darkwood/mgp/darkwood_11_01.mgp | 4aea46f9b42558281ce0 | ORPHAN | [details](assets/4aea46f9b42558281ce0.md) |
| 3d/modules/darkwood/mgp/darkwood_09_01.mgp | 4afea2ced1802e76698c | ORPHAN | [details](assets/4afea2ced1802e76698c.md) |
| 3d/modules/darkwood/mvp/darkwood_10_01.mvp | 4b5157bcf691b444667e | ORPHAN | [details](assets/4b5157bcf691b444667e.md) |
| 3d/characters/dark_queen/dark_queen1stform.bdae | 4e17a20b97ae3030c9c3 | ORPHAN | [details](assets/4e17a20b97ae3030c9c3.md) |
| sfx_swoosh_sword_2_22.wav | 4f1d3f8cf46f3f869bc3 | ORPHAN | [details](assets/4f1d3f8cf46f3f869bc3.md) |
| file003645.dat | 4f92c39c1faf0922ec4a | ORPHAN | [details](assets/4f92c39c1faf0922ec4a.md) |
| sfx_npc_priest_hurt_2_22.wav | 4fcc3581ac2c20509a6a | ORPHAN | [details](assets/4fcc3581ac2c20509a6a.md) |
| file000142.dat | 515bfc04c7d49c59f55c | ORPHAN | [details](assets/515bfc04c7d49c59f55c.md) |
| 3d/characters/npcs/priest_good.bdae | 51cbb6394b39f62c502c | ORPHAN | [details](assets/51cbb6394b39f62c502c.md) |
| 3d/modules/darkwood/mgp/darkwood_14_01.mgp | 5238552deed912d9400c | ORPHAN | [details](assets/5238552deed912d9400c.md) |
| 3d/interface/swoosh_prince_1hand_combo_01.bdae | 528b769d3c39254976b3 | ORPHAN | [details](assets/528b769d3c39254976b3.md) |
| sounds/sfx_statue_cherub_attack.wav | 53c6cd127b273fb81768 | ORPHAN | [details](assets/53c6cd127b273fb81768.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_template_anim.bdae | 54917e02e265f6f55bea | ORPHAN | [details](assets/54917e02e265f6f55bea.md) |
| file003044.dat | 556d3721f7192d9d275f | ORPHAN | [details](assets/556d3721f7192d9d275f.md) |
| 3d/modules/darkwood/mgp/darkwood_15_01.mgp | 573b17426b436108427b | ORPHAN | [details](assets/573b17426b436108427b.md) |
| file005793.dat | 575437701bb20a32bbba | ORPHAN | [details](assets/575437701bb20a32bbba.md) |
| 3d/modules/darkwood/mvp/darkwood_01_01.mvp | 5755e286cee4839b0706 | ORPHAN | [details](assets/5755e286cee4839b0706.md) |
| file002997.dat | 5788954ed373883ed0c8 | ORPHAN | [details](assets/5788954ed373883ed0c8.md) |
| 3d/characters/prince/animations/prince_post_stunned.bdae | 58b660d7b57d290d104f | ORPHAN | [details](assets/58b660d7b57d290d104f.md) |
| 3d/characters/prince/animations/prince_idle_shield_02.bdae | 58eb6d473413ed17e95e | ORPHAN | [details](assets/58eb6d473413ed17e95e.md) |
| 3d/characters/statue/animations/child_idle_01.bdae | 59c5ff245f97f61a2d28 | ORPHAN | [details](assets/59c5ff245f97f61a2d28.md) |
| 3d/characters/npcs/animations/npc_m_waiting.bdae | 5ac57748df5a39da600d | ORPHAN | [details](assets/5ac57748df5a39da600d.md) |
| 3d/characters/npcs/old_peasant.bdae | 5b11542df8cbea6817a6 | ORPHAN | [details](assets/5b11542df8cbea6817a6.md) |
| 3d/characters/prince/animations/prince_walk_slow.bdae | 5c3aae8395bd920f526a | ORPHAN | [details](assets/5c3aae8395bd920f526a.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_post_projectile.bdae | 5c540f3c92a90657ab28 | ORPHAN | [details](assets/5c540f3c92a90657ab28.md) |
| 3d/characters/npcs/animations/edward_1hand_combo_01.bdae | 5c5ee1e21bf184ed883d | ORPHAN | [details](assets/5c5ee1e21bf184ed883d.md) |
| 3d/characters/prince/animations/prince_idle_shield.bdae | 5cf90dd273ef9ac8370a | ORPHAN | [details](assets/5cf90dd273ef9ac8370a.md) |
| file001640.dat | 5e0cdaaebc394f3493e9 | ORPHAN | [details](assets/5e0cdaaebc394f3493e9.md) |
| 3d/characters/npcs/animations/npc_m_idle_standing.bdae | 61668df0fa9329cffec4 | ORPHAN | [details](assets/61668df0fa9329cffec4.md) |
| 3d/modules/darkwood/mvp/darkwood_07_01.mvp | 61878c6dad84950c4919 | ORPHAN | [details](assets/61878c6dad84950c4919.md) |
| 3d/modules/darkwood/mvp/darkwood_14_01.mvp | 62c12582b47b4904a121 | ORPHAN | [details](assets/62c12582b47b4904a121.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_die.bdae | 63eacd5b3b49276444f9 | ORPHAN | [details](assets/63eacd5b3b49276444f9.md) |
| file003886.dat | 683cb3d4d27aa8d73c38 | ORPHAN | [details](assets/683cb3d4d27aa8d73c38.md) |
| 3d/characters/statue/animations/statue_beam_to_cooldown.bdae | 69f5cc18eaea0ec78720 | ORPHAN | [details](assets/69f5cc18eaea0ec78720.md) |
| 3d/characters/statue/child.bdae | 6a8c6a86e4661987ac31 | ORPHAN | [details](assets/6a8c6a86e4661987ac31.md) |
| 3d/modules/darkwood/darkwood2.bdae | 6aa199271035e3937f84 | ORPHAN | [details](assets/6aa199271035e3937f84.md) |
| 3d/modules/darkwood/mvp/darkwood_28_01.mvp | 6ad7d215ea2da6f987ba | ORPHAN | [details](assets/6ad7d215ea2da6f987ba.md) |
| 3d/modules/darkwood/mgp/darkwood_28_01.mgp | 6b02307477be080b43e5 | ORPHAN | [details](assets/6b02307477be080b43e5.md) |
| file000461.dat | 6b6bd98bde4e5c9f1dff | ORPHAN | [details](assets/6b6bd98bde4e5c9f1dff.md) |
| scripts/ai/dw_statue.luac | 6b7c1d7270ab7b8a2716 | ORPHAN | [details](assets/6b7c1d7270ab7b8a2716.md) |
| file000151.dat | 6bb634b933a10dafa46e | ORPHAN | [details](assets/6bb634b933a10dafa46e.md) |
| file000637.dat | 6da1b834f79485d189b5 | ORPHAN | [details](assets/6da1b834f79485d189b5.md) |
| 3d/characters/prince/animations/prince_standup.bdae | 6dead83a06a8a4f0c18c | ORPHAN | [details](assets/6dead83a06a8a4f0c18c.md) |
| file006098.dat | 6ee58d0961156f1fd020 | ORPHAN | [details](assets/6ee58d0961156f1fd020.md) |
| 3d/interface/swoosh_prince_1hand_combo_03.bdae | 6ef63481382913ac92ba | ORPHAN | [details](assets/6ef63481382913ac92ba.md) |
| sounds/sfx_dark_queen_attack.wav | 6f2d8d7c1da025e362a5 | ORPHAN | [details](assets/6f2d8d7c1da025e362a5.md) |
| 3d/modules/darkwood/mvp/darkwood_05_01.mvp | 707aa5f6706d8fec655e | ORPHAN | [details](assets/707aa5f6706d8fec655e.md) |
| sfx_impact_arrow_object_2_22.wav | 724da0c3eff74749b247 | ORPHAN | [details](assets/724da0c3eff74749b247.md) |
| file005633.dat | 72f8a11ae7dd39339e8f | ORPHAN | [details](assets/72f8a11ae7dd39339e8f.md) |
| 3d/projectiles/elemental_bolt_fire.bdae | 75eb749e6ecd2fc0f475 | ORPHAN | [details](assets/75eb749e6ecd2fc0f475.md) |
| 3d/modules/darkwood/mvp/darkwood_21_01.mvp | 7697c6d672e16cb49d7b | ORPHAN | [details](assets/7697c6d672e16cb49d7b.md) |
| sounds/sfx_bandit_attack.wav | 76d1cba0f0b6a01714a9 | ORPHAN | [details](assets/76d1cba0f0b6a01714a9.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_02.bdae | 780f5b393dab705ea937 | ORPHAN | [details](assets/780f5b393dab705ea937.md) |
| 3d/modules/darkwood/mvp/darkwood_02_01.mvp | 78ae0efbb1f02b2d442c | ORPHAN | [details](assets/78ae0efbb1f02b2d442c.md) |
| 3d/modules/darkwood/mgp/darkwood_15_01.mgp | 791d02a720acbae7ca1f | ORPHAN | [details](assets/791d02a720acbae7ca1f.md) |
| 3d/modules/darkwood/mvp/darkwood_04_01.mvp | 79c18fa286e9585855b0 | ORPHAN | [details](assets/79c18fa286e9585855b0.md) |
| file004607.dat | 7aa65e9e6df06256ea43 | ORPHAN | [details](assets/7aa65e9e6df06256ea43.md) |
| file001276.dat | 7b2f094286c6f1e105d1 | ORPHAN | [details](assets/7b2f094286c6f1e105d1.md) |
| 3d/interface/skill_dh2_monster_spawn.bdae | 7bd88c97508f9ffe9193 | ORPHAN | [details](assets/7bd88c97508f9ffe9193.md) |
| 3d/characters/statue/animations/statue_dying.bdae | 7c43632b1aa238fa04aa | ORPHAN | [details](assets/7c43632b1aa238fa04aa.md) |
| 3d/characters/statue/statue.bdae | 7c9eec25eee78d16ee65 | ORPHAN | [details](assets/7c9eec25eee78d16ee65.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_projectile.bdae | 7cf7daf8a00b31252d74 | ORPHAN | [details](assets/7cf7daf8a00b31252d74.md) |
| scripts/skills/darkqueen2_melee_00.luac | 7d1c9e5e5d89c23162f5 | ORPHAN | [details](assets/7d1c9e5e5d89c23162f5.md) |
| 3d/modules/darkwood/mvp/darkwood_08_01.mvp | 7d2e4002ccaf5454cac8 | ORPHAN | [details](assets/7d2e4002ccaf5454cac8.md) |
| 3d/fx/bloodsplat_hero.bdae | 7e3c52b52f92471e8a7c | ORPHAN | [details](assets/7e3c52b52f92471e8a7c.md) |
| file004197.dat | 7f193be689d9f012d5b4 | ORPHAN | [details](assets/7f193be689d9f012d5b4.md) |
| 3d/characters/prince/animations/prince_1hand_combo_01_moving.bdae | 7f5ba7bb6c3cc1dba7da | ORPHAN | [details](assets/7f5ba7bb6c3cc1dba7da.md) |
| file003579.dat | 805f62e84ed976051b89 | ORPHAN | [details](assets/805f62e84ed976051b89.md) |
| file003443.dat | 812e7670eb8de71fbece | ORPHAN | [details](assets/812e7670eb8de71fbece.md) |
| 3d/modules/darkwood/mvp/darkwood_21_01.mvp | 81d2c8527b725590fd12 | ORPHAN | [details](assets/81d2c8527b725590fd12.md) |
| 3d/characters/statue/animations/child_attack_01.bdae | 82355ec07c5ebbcdc724 | ORPHAN | [details](assets/82355ec07c5ebbcdc724.md) |
| 3d/modules/darkwood/mvp/darkwood_05_01.mvp | 82b8ea386e4ab535a0cc | ORPHAN | [details](assets/82b8ea386e4ab535a0cc.md) |
| file000649.dat | 84b03f153a5c5471f957 | ORPHAN | [details](assets/84b03f153a5c5471f957.md) |
| 3d/characters/statue/animations/statue_idle_to_charge.bdae | 84c99d3ab94e0fa5fa7f | ORPHAN | [details](assets/84c99d3ab94e0fa5fa7f.md) |
| 3d/interface/skill_dh2_monster_dark_queen1stform_die.bdae | 872fee4d1ffae7a9dee4 | ORPHAN | [details](assets/872fee4d1ffae7a9dee4.md) |
| 3d/modules/darkwood/mvp/darkwood_06_01.mvp | 88d13a1add0c452757d9 | ORPHAN | [details](assets/88d13a1add0c452757d9.md) |
| 3d/modules/darkwood/mgp/darkwood_21_01.mgp | 892eaf6b8a3b1cff0a8e | ORPHAN | [details](assets/892eaf6b8a3b1cff0a8e.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_01.bdae | 8ac719dcc31a99b4c960 | ORPHAN | [details](assets/8ac719dcc31a99b4c960.md) |
| 3d/modules/darkwood/mvp/darkwood_29_01.mvp | 8b1faa093da383a3f8d7 | ORPHAN | [details](assets/8b1faa093da383a3f8d7.md) |
| file003646.dat | 8bab32a5d838672bf26a | ORPHAN | [details](assets/8bab32a5d838672bf26a.md) |
| 3d/characters/statue/animations/child_dying.bdae | 8be65fdaf3c4c1868dfe | ORPHAN | [details](assets/8be65fdaf3c4c1868dfe.md) |
| 3d/modules/darkwood/mvp/darkwood_09_01.mvp | 8c91e583bc8ef243b4ab | ORPHAN | [details](assets/8c91e583bc8ef243b4ab.md) |
| 3d/characters/prince/animations/prince_hurt_01.bdae | 8c922b02e66e80603f0c | ORPHAN | [details](assets/8c922b02e66e80603f0c.md) |
| 3d/modules/darkwood/mgp/darkwood_01_01.mgp | 8e1e4a33514d7d29b148 | ORPHAN | [details](assets/8e1e4a33514d7d29b148.md) |
| sounds/sfx_dark_queen_hurt.wav | 8f0d21e24fe28b13d97c | ORPHAN | [details](assets/8f0d21e24fe28b13d97c.md) |
| 3d/characters/prince/animations/prince_1hand_combo_03_moving.bdae | 8f1d0186c820d603179d | ORPHAN | [details](assets/8f1d0186c820d603179d.md) |
| 3d/modules/darkwood/mgp/darkwood_12_01.mgp | 907d0def09ff2861e6f3 | ORPHAN | [details](assets/907d0def09ff2861e6f3.md) |
| 3d/modules/darkwood/mgp/darkwood_29_01.mgp | 907e3c928c1ce05dad51 | ORPHAN | [details](assets/907e3c928c1ce05dad51.md) |
| sfx_impact_arrow_object_1_22.wav | 908a219c0b7b7c441ac1 | ORPHAN | [details](assets/908a219c0b7b7c441ac1.md) |
| file002691.dat | 91824150b7be186627b8 | ORPHAN | [details](assets/91824150b7be186627b8.md) |
| file004684.dat | 9367ae9c3168f0a5d7dd | ORPHAN | [details](assets/9367ae9c3168f0a5d7dd.md) |
| 3d/characters/statue/animations/child_hurt.bdae | 937a48e189fcc4242dd7 | ORPHAN | [details](assets/937a48e189fcc4242dd7.md) |
| 3d/modules/darkwood/mvp/darkwood_28_01.mvp | 941f0c407aa5214553f5 | ORPHAN | [details](assets/941f0c407aa5214553f5.md) |
| 3d/characters/statue/animations/child_template_anim.bdae | 9512fba2f7c0d13d93f0 | ORPHAN | [details](assets/9512fba2f7c0d13d93f0.md) |
| 3d/characters/statue/animations/child_pre_stunned.bdae | 967c6e253dfefaebdba6 | ORPHAN | [details](assets/967c6e253dfefaebdba6.md) |
| 3d/modules/darkwood/mgp/darkwood_03_01.mgp | 96caca8fc842899c3231 | ORPHAN | [details](assets/96caca8fc842899c3231.md) |
| 3d/characters/npcs/edward.bdae | 97a7f82fba592384dabc | ORPHAN | [details](assets/97a7f82fba592384dabc.md) |
| 3d/characters/npcs/castle_maid.bdae | 9a962e2c98c8b4286054 | ORPHAN | [details](assets/9a962e2c98c8b4286054.md) |
| 3d/modules/darkwood/mgp/darkwood_04_01.mgp | 9b0040c1ef34e240bcdd | ORPHAN | [details](assets/9b0040c1ef34e240bcdd.md) |
| scripts/ai/monster.luac | 9b1caeb2ccf9b9424e45 | ORPHAN | [details](assets/9b1caeb2ccf9b9424e45.md) |
| file003785.dat | 9b6851d633012b0bf520 | ORPHAN | [details](assets/9b6851d633012b0bf520.md) |
| 3d/characters/npcs/animations/edward_1hand_combo_01_to_idle.bdae | 9bd97d1567b229bff061 | ORPHAN | [details](assets/9bd97d1567b229bff061.md) |
| 3d/modules/darkwood/mvp/darkwood_12_01.mvp | 9cd61daad94bd13af7fc | ORPHAN | [details](assets/9cd61daad94bd13af7fc.md) |
| 3d/modules/darkwood/mgp/darkwood_29_01.mgp | 9d61413a9743a0b91ada | ORPHAN | [details](assets/9d61413a9743a0b91ada.md) |
| 3d/modules/darkwood/mgp/darkwood_06_01.mgp | 9e90c04b074e77569ba1 | ORPHAN | [details](assets/9e90c04b074e77569ba1.md) |
| 3d/modules/darkwood/mvp/darkwood_10_01.mvp | 9eebc2522fc8c71e8f1e | ORPHAN | [details](assets/9eebc2522fc8c71e8f1e.md) |
| file004783.dat | 9ef54539dcbab477949d | ORPHAN | [details](assets/9ef54539dcbab477949d.md) |
| file001952.dat | 9f315489390a7a48856f | ORPHAN | [details](assets/9f315489390a7a48856f.md) |
| file004733.dat | a0a7598bd1da15fe73fe | ORPHAN | [details](assets/a0a7598bd1da15fe73fe.md) |
| sounds/sfx_bandit_killed.wav | a0e4195d35f6cbbf63ec | ORPHAN | [details](assets/a0e4195d35f6cbbf63ec.md) |
| 3d/characters/statue/animations/child_great_pushback_dying_despawn.bdae | a1182ed3425886433752 | ORPHAN | [details](assets/a1182ed3425886433752.md) |
| file003550.dat | a1336fc596a1c8ddf24f | ORPHAN | [details](assets/a1336fc596a1c8ddf24f.md) |
| 3d/modules/darkwood/mgp/darkwood_12_01.mgp | a1667fcc49fa0c21cf9f | ORPHAN | [details](assets/a1667fcc49fa0c21cf9f.md) |
| file001316.dat | a166eb66d3c5194b23fa | ORPHAN | [details](assets/a166eb66d3c5194b23fa.md) |
| 3d/gameobjects/itemdrops.bdae | a2acd30044d5739365dd | ORPHAN | [details](assets/a2acd30044d5739365dd.md) |
| scripts/ai/darkqueen1.luac | a2e3866360ed767d346a | ORPHAN | [details](assets/a2e3866360ed767d346a.md) |
| 3d/modules/darkwood/mvp/darkwood_11_01.mvp | a3f9296e8a8ce480324d | ORPHAN | [details](assets/a3f9296e8a8ce480324d.md) |
| 3d/modules/darkwood/mvp/darkwood_07_01.mvp | a4c13d7e57658238fbf3 | ORPHAN | [details](assets/a4c13d7e57658238fbf3.md) |
| file004296.dat | a65482b40853d1217538 | ORPHAN | [details](assets/a65482b40853d1217538.md) |
| 3d/modules/darkwood/mgp/darkwood_17_01.mgp | a8484fadc2230eaa9d74 | ORPHAN | [details](assets/a8484fadc2230eaa9d74.md) |
| 3d/modules/darkwood/mvp/darkwood_16_01.mvp | a8b488420a75be3f49a3 | ORPHAN | [details](assets/a8b488420a75be3f49a3.md) |
| 3d/characters/npcs/edward_with_weapon.bdae | a8be5f2c03f6d4ef980f | ORPHAN | [details](assets/a8be5f2c03f6d4ef980f.md) |
| sounds/sfx_statue_cherub_hurt_1.wav | a900f80c028306f1ac12 | ORPHAN | [details](assets/a900f80c028306f1ac12.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_pre_projectile.bdae | a98a35a15668682c45c3 | ORPHAN | [details](assets/a98a35a15668682c45c3.md) |
| sounds/sfx_statue_cherub_die.wav | a98e0a93b75722e369da | ORPHAN | [details](assets/a98e0a93b75722e369da.md) |
| 3d/characters/npcs/animations/cin_king_sitting_idle.bdae | a9c0939478ab11ec3cdd | ORPHAN | [details](assets/a9c0939478ab11ec3cdd.md) |
| 3d/characters/lizardman/old_man.bdae | aaee6bdec81db5ce502d | ORPHAN | [details](assets/aaee6bdec81db5ce502d.md) |
| 3d/modules/darkwood/mgp/darkwood_05_01.mgp | ab8c46e3f8f38065ed27 | ORPHAN | [details](assets/ab8c46e3f8f38065ed27.md) |
| file002959.dat | acdc642eb11f61db584e | ORPHAN | [details](assets/acdc642eb11f61db584e.md) |
| 3d/characters/statue/animations/child_stunned.bdae | ad031eb87789eb581170 | ORPHAN | [details](assets/ad031eb87789eb581170.md) |
| 3d/characters/prince/animations/prince_great_pushback_dying.bdae | ad12754ada26af8ef929 | ORPHAN | [details](assets/ad12754ada26af8ef929.md) |
| 3d/modules/darkwood/darkwood2.bdae | ad3252c64b2070049ff5 | ORPHAN | [details](assets/ad3252c64b2070049ff5.md) |
| 3d/modules/darkwood/mvp/darkwood_04_01.mvp | ae1801e94c1b13da0c00 | ORPHAN | [details](assets/ae1801e94c1b13da0c00.md) |
| file001042.dat | ae41ad039919ccc7cd9e | ORPHAN | [details](assets/ae41ad039919ccc7cd9e.md) |
| 3d/modules/darkwood/mvp/darkwood_03_01.mvp | aea46573aa977beb6a2b | ORPHAN | [details](assets/aea46573aa977beb6a2b.md) |
| file001583.dat | af150eb11e6ebe4a49d0 | ORPHAN | [details](assets/af150eb11e6ebe4a49d0.md) |
| 3d/characters/prince/animations/prince_hurt_03.bdae | af6b6b996ce7e855fdf2 | ORPHAN | [details](assets/af6b6b996ce7e855fdf2.md) |
| file005273.dat | afa3ba3a89850391e8c3 | ORPHAN | [details](assets/afa3ba3a89850391e8c3.md) |
| file005262.dat | aff3713023ee3f281fda | ORPHAN | [details](assets/aff3713023ee3f281fda.md) |
| file004462.dat | b0d02850a2219d366359 | ORPHAN | [details](assets/b0d02850a2219d366359.md) |
| sounds/sfx_edward_die.wav | b2209efca60ecec67058 | ORPHAN | [details](assets/b2209efca60ecec67058.md) |
| file004789.dat | b313ea7f97440192784b | ORPHAN | [details](assets/b313ea7f97440192784b.md) |
| 3d/characters/statue/animations/statue_crying.bdae | b4b8e56f6c00ff50d30e | ORPHAN | [details](assets/b4b8e56f6c00ff50d30e.md) |
| file005260.dat | b611afbbcd219d65a994 | ORPHAN | [details](assets/b611afbbcd219d65a994.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_03_moving.bdae | b6378533362abfcd5828 | ORPHAN | [details](assets/b6378533362abfcd5828.md) |
| 3d/interface/spell_dh2_hurt_fire.bdae | b67905bc599dd0e6502a | ORPHAN | [details](assets/b67905bc599dd0e6502a.md) |
| 3d/modules/darkwood/mgp/darkwood_16_01.mgp | b6ded0452990906c7751 | ORPHAN | [details](assets/b6ded0452990906c7751.md) |
| 3d/modules/darkwood/mgp/darkwood_28_01.mgp | b77c09f711bf29003a35 | ORPHAN | [details](assets/b77c09f711bf29003a35.md) |
| 3d/characters/faeries/faeries_02_celeste.bdae | b7dd84865e6930587069 | ORPHAN | [details](assets/b7dd84865e6930587069.md) |
| 3d/characters/prince/animations/prince_1hand_combo_02_to_idle.bdae | b90435446904c3bbcc58 | ORPHAN | [details](assets/b90435446904c3bbcc58.md) |
| file000681.dat | bab65be0767d491ce767 | ORPHAN | [details](assets/bab65be0767d491ce767.md) |
| 3d/modules/darkwood/mvp/darkwood_01_01.mvp | bb715c70cb3d2ac187f5 | ORPHAN | [details](assets/bb715c70cb3d2ac187f5.md) |
| sfx_npc_priest_hurt_3_22.wav | bbe2f9f70db6061bea9a | ORPHAN | [details](assets/bbe2f9f70db6061bea9a.md) |
| file000998.dat | bc5dd8af0d0678614dc9 | ORPHAN | [details](assets/bc5dd8af0d0678614dc9.md) |
| 3d/modules/darkwood/mvp/darkwood_20_01.mvp | be62cb4d1602f53528cd | ORPHAN | [details](assets/be62cb4d1602f53528cd.md) |
| file003245.dat | be72e7deb330cf538d48 | ORPHAN | [details](assets/be72e7deb330cf538d48.md) |
| file002320.dat | bf0d279e91e097d1b06d | ORPHAN | [details](assets/bf0d279e91e097d1b06d.md) |
| file002146.dat | bf2086cca180d9942699 | ORPHAN | [details](assets/bf2086cca180d9942699.md) |
| 3d/projectiles/projectile_dh2_monster_darkqueen_form01.bdae | bf573fa0a59efdb0d9b8 | ORPHAN | [details](assets/bf573fa0a59efdb0d9b8.md) |
| 3d/characters/prince/animations/prince_pre_stunned.bdae | c0ce33ffa98e46e62dab | ORPHAN | [details](assets/c0ce33ffa98e46e62dab.md) |
| scripts/skills/darkqueen2_ranged_00.luac | c3e0a86855a7b81fa360 | ORPHAN | [details](assets/c3e0a86855a7b81fa360.md) |
| 3d/light/darkwood_es1_1.lightset_xml | c4858ece8e0edd9a6f39 | ORPHAN | [details](assets/c4858ece8e0edd9a6f39.md) |
| file001685.dat | c5408fea0a02ddf6628a | ORPHAN | [details](assets/c5408fea0a02ddf6628a.md) |
| 3d/characters/prince/animations/prince_1hand_post_combo_01_moving.bdae | c602fba90fcb7ec73688 | ORPHAN | [details](assets/c602fba90fcb7ec73688.md) |
| 3d/modules/darkwood/mgp/darkwood_16_01.mgp | c60f5c766855232ad2db | ORPHAN | [details](assets/c60f5c766855232ad2db.md) |
| file002619.dat | c6916891ef41c27a5efd | ORPHAN | [details](assets/c6916891ef41c27a5efd.md) |
| file000201.dat | c7114f79ece51130c300 | ORPHAN | [details](assets/c7114f79ece51130c300.md) |
| 3d/modules/darkwood/mvp/darkwood_08_01.mvp | c731f21f7e51f78b7765 | ORPHAN | [details](assets/c731f21f7e51f78b7765.md) |
| 3d/characters/prince/animations/prince_pushback.bdae | c8c50720e5612dd72f36 | ORPHAN | [details](assets/c8c50720e5612dd72f36.md) |
| 3d/modules/darkwood/mvp/darkwood_19_01.mvp | c963464d4c89f4c97a36 | ORPHAN | [details](assets/c963464d4c89f4c97a36.md) |
| 3d/characters/statue/animations/child_post_attack_01.bdae | c977c24ee1f3dbc37760 | ORPHAN | [details](assets/c977c24ee1f3dbc37760.md) |
| 3d/modules/darkwood/mgp/darkwood_19_01.mgp | cb5d5de052f8c3cf813d | ORPHAN | [details](assets/cb5d5de052f8c3cf813d.md) |
| 3d/modules/darkwood/mvp/darkwood_23_01.mvp | cbe9c099c40af4864ab3 | ORPHAN | [details](assets/cbe9c099c40af4864ab3.md) |
| 3d/characters/prince/animations/prince_1hand_post_combo_02_moving.bdae | ccb91556684386cce06d | ORPHAN | [details](assets/ccb91556684386cce06d.md) |
| 3d/modules/darkwood/mvp/darkwood_15_01.mvp | cd015639e125e978e910 | ORPHAN | [details](assets/cd015639e125e978e910.md) |
| 3d/characters/prince/animations/prince_hurt_02.bdae | cdc4d0bfe5899dacfc54 | ORPHAN | [details](assets/cdc4d0bfe5899dacfc54.md) |
| file003709.dat | cf93c4039a6e16975f83 | ORPHAN | [details](assets/cf93c4039a6e16975f83.md) |
| 3d/characters/statue/animations/statue_idle.bdae | cfad1c6be53d1a6b9ff3 | ORPHAN | [details](assets/cfad1c6be53d1a6b9ff3.md) |
| 3d/modules/darkwood/mgp/darkwood_02_01.mgp | d1682db83a4eec3679b0 | ORPHAN | [details](assets/d1682db83a4eec3679b0.md) |
| 3d/characters/faeries/animations/faeries_celeste_idle.bdae | d19f2af2ea4e9fa8e5f5 | ORPHAN | [details](assets/d19f2af2ea4e9fa8e5f5.md) |
| 3d/modules/darkwood/mgp/darkwood_11_01.mgp | d1ba73fed5b3929a0142 | ORPHAN | [details](assets/d1ba73fed5b3929a0142.md) |
| 3d/modules/darkwood/mvp/darkwood_09_01.mvp | d20e517eef55890bc46f | ORPHAN | [details](assets/d20e517eef55890bc46f.md) |
| file004255.dat | d247966d5d764d252919 | ORPHAN | [details](assets/d247966d5d764d252919.md) |
| 3d/modules/darkwood/mvp/darkwood_23_01.mvp | d2eb2205b9c7c88836cf | ORPHAN | [details](assets/d2eb2205b9c7c88836cf.md) |
| 3d/characters/statue/animations/statue_hurt.bdae | d35792209db13e50fec1 | ORPHAN | [details](assets/d35792209db13e50fec1.md) |
| file001157.dat | d53219191b1c6f49fafc | ORPHAN | [details](assets/d53219191b1c6f49fafc.md) |
| 3d/characters/npcs/animations/head_cultist_anim_template.bdae | d5786a58539300963b02 | ORPHAN | [details](assets/d5786a58539300963b02.md) |
| sounds/sfx_bandit_hurt.wav | d64322964cbde70645e8 | ORPHAN | [details](assets/d64322964cbde70645e8.md) |
| 3d/modules/darkwood/mgp/darkwood_21_01.mgp | d688e8b6d222672eb26d | ORPHAN | [details](assets/d688e8b6d222672eb26d.md) |
| file003121.dat | d689081de64282733892 | ORPHAN | [details](assets/d689081de64282733892.md) |
| file002381.dat | d68d9ea0435c8779b0ff | ORPHAN | [details](assets/d68d9ea0435c8779b0ff.md) |
| 3d/characters/npcs/animations/npc_m_bored.bdae | d6dd246264b7e8a866b1 | ORPHAN | [details](assets/d6dd246264b7e8a866b1.md) |
| 3d/characters/prince/animations/prince_great_pushback_standup.bdae | d6fe30c6aa3735eea12c | ORPHAN | [details](assets/d6fe30c6aa3735eea12c.md) |
| 3d/characters/npcs/jeremo_guard.bdae | d7036b61bf700a634409 | ORPHAN | [details](assets/d7036b61bf700a634409.md) |
| file004183.dat | d7659cae46bce4b60f78 | ORPHAN | [details](assets/d7659cae46bce4b60f78.md) |
| sounds/sfx_edward_hurt_1.wav | da016e6392547d5b1c3e | ORPHAN | [details](assets/da016e6392547d5b1c3e.md) |
| file002579.dat | dae1996478ea3b343c49 | ORPHAN | [details](assets/dae1996478ea3b343c49.md) |
| file001013.dat | dc99c85aa67d490ff9aa | ORPHAN | [details](assets/dc99c85aa67d490ff9aa.md) |
| 3d/characters/npcs/animations/npc_m_lookingaround.bdae | ddb636673d3c96ebcad3 | ORPHAN | [details](assets/ddb636673d3c96ebcad3.md) |
| file001045.dat | de1c76e62193bbc7decf | ORPHAN | [details](assets/de1c76e62193bbc7decf.md) |
| 3d/modules/darkwood/mvp/darkwood_17_01.mvp | def3f456b01177b4146f | ORPHAN | [details](assets/def3f456b01177b4146f.md) |
| file001825.dat | dfc252b69afe43c27325 | ORPHAN | [details](assets/dfc252b69afe43c27325.md) |
| 3d/characters/statue/animations/child_despawn.bdae | e1df971042315bb3b913 | ORPHAN | [details](assets/e1df971042315bb3b913.md) |
| 3d/modules/darkwood/mgp/darkwood_05_01.mgp | e1e36bc730ab6af35729 | ORPHAN | [details](assets/e1e36bc730ab6af35729.md) |
| file003617.dat | e4356569166aad518eec | ORPHAN | [details](assets/e4356569166aad518eec.md) |
| 3d/characters/statue/animations/child_great_pushback.bdae | e49bd7503a2b7cd5d70e | ORPHAN | [details](assets/e49bd7503a2b7cd5d70e.md) |
| 3d/modules/darkwood/mgp/darkwood_03_01.mgp | e51c51169f2719a9abc5 | ORPHAN | [details](assets/e51c51169f2719a9abc5.md) |
| 3d/modules/darkwood/mvp/darkwood_03_01.mvp | e5e3ae3c7c1b4e67f127 | ORPHAN | [details](assets/e5e3ae3c7c1b4e67f127.md) |
| 3d/characters/statue/animations/child_great_pushback_standup.bdae | e68073405b5f4d93f4b7 | ORPHAN | [details](assets/e68073405b5f4d93f4b7.md) |
| 3d/modules/darkwood/mgp/darkwood_27_01.mgp | e6f3eb70f83b6be2929f | ORPHAN | [details](assets/e6f3eb70f83b6be2929f.md) |
| 3d/characters/prince/animations/prince_template_anim.bdae | e767e29aeb9e148aac3c | ORPHAN | [details](assets/e767e29aeb9e148aac3c.md) |
| 3d/modules/darkwood/mvp/darkwood_15_01.mvp | e76b77209f8f8a4e5d29 | ORPHAN | [details](assets/e76b77209f8f8a4e5d29.md) |
| 3d/modules/darkwood/mgp/darkwood_02_01.mgp | e7927bcdae13dbeabc30 | ORPHAN | [details](assets/e7927bcdae13dbeabc30.md) |
| 3d/modules/darkwood/mgp/darkwood_20_01.mgp | e832d052220729876ba5 | ORPHAN | [details](assets/e832d052220729876ba5.md) |
| file002855.dat | e9f36b4478d01e700e6f | ORPHAN | [details](assets/e9f36b4478d01e700e6f.md) |
| 3d/characters/npcs/animations/npc_m_dialog.bdae | ea9d94d2372c72445294 | ORPHAN | [details](assets/ea9d94d2372c72445294.md) |
| file005581.dat | ec41e3a0642a0e413427 | ORPHAN | [details](assets/ec41e3a0642a0e413427.md) |
| 3d/characters/prince/animations/prince_walk_1hand.bdae | ee0cdf8b7caf073c8eab | ORPHAN | [details](assets/ee0cdf8b7caf073c8eab.md) |
| 3d/characters/prince/animations/prince_1hand_combo_01_to_idle.bdae | ee73294e45a9cab86c61 | ORPHAN | [details](assets/ee73294e45a9cab86c61.md) |
| 3d/characters/prince/animations/prince_1hand_combo_02_moving.bdae | eee5dc9e88bc94b45e4e | ORPHAN | [details](assets/eee5dc9e88bc94b45e4e.md) |
| 3d/interface/swoosh_prince_1hand_combo_02.bdae | f0d9aa09edfe4e00e6b6 | ORPHAN | [details](assets/f0d9aa09edfe4e00e6b6.md) |
| 3d/characters/prince/animations/prince_great_pushback.bdae | f188b3f83d185c73cbdd | ORPHAN | [details](assets/f188b3f83d185c73cbdd.md) |
| file001322.dat | f1efbd5cada4eb93f3e3 | ORPHAN | [details](assets/f1efbd5cada4eb93f3e3.md) |
| 3d/characters/npcs/animations/edward_1hand_pre_combo_01.bdae | f244b7398e23c3c74123 | ORPHAN | [details](assets/f244b7398e23c3c74123.md) |
| 3d/modules/darkwood/mvp/darkwood_11_01.mvp | f2ff036d7737b99ede62 | ORPHAN | [details](assets/f2ff036d7737b99ede62.md) |
| 3d/characters/npcs/young_peasant.bdae | f4e669fc3d5df8d720bd | ORPHAN | [details](assets/f4e669fc3d5df8d720bd.md) |
| 3d/characters/npcs/animations/king_anim_template.bdae | f562a8c54f6eb640f30b | ORPHAN | [details](assets/f562a8c54f6eb640f30b.md) |
| 3d/characters/statue/animations/statue_template_anim.bdae | f5672b42e6c80ad6738c | ORPHAN | [details](assets/f5672b42e6c80ad6738c.md) |
| file001420.dat | f6a5c2b3d8e662e195ed | ORPHAN | [details](assets/f6a5c2b3d8e662e195ed.md) |
| 3d/modules/darkwood/mvp/darkwood_19_01.mvp | f72e9ad796c65265039f | ORPHAN | [details](assets/f72e9ad796c65265039f.md) |
| 3d/light/darkwood.lightset_xml | f845de7060330d9f6f91 | ORPHAN | [details](assets/f845de7060330d9f6f91.md) |
| 3d/modules/darkwood/mgp/darkwood_23_01.mgp | fa060e7d8454fb66b265 | ORPHAN | [details](assets/fa060e7d8454fb66b265.md) |
| file001789.dat | fa0cc068dce79bf0a6e2 | ORPHAN | [details](assets/fa0cc068dce79bf0a6e2.md) |
| 3d/characters/dark_queen/animations/dark_queen1stform_idle.bdae | fa14f24f06827c174759 | ORPHAN | [details](assets/fa14f24f06827c174759.md) |
| 3d/modules/darkwood/mvp/darkwood_06_01.mvp | fa661e68395e64f5f2c8 | ORPHAN | [details](assets/fa661e68395e64f5f2c8.md) |
| 3d/characters/statue/animations/child_great_pushback_dying.bdae | fa8966be49829764d862 | ORPHAN | [details](assets/fa8966be49829764d862.md) |
| file000164.dat | faa3efea812cd75a179c | ORPHAN | [details](assets/faa3efea812cd75a179c.md) |
| 3d/projectiles/elemental_bolt_lightning.bdae | fb70df094ad0a9c05898 | ORPHAN | [details](assets/fb70df094ad0a9c05898.md) |
| 3d/modules/darkwood/mgp/darkwood_19_01.mgp | fc82e572022ba09ff5f7 | ORPHAN | [details](assets/fc82e572022ba09ff5f7.md) |
| file004763.dat | fd6d6b3573aff98f84de | ORPHAN | [details](assets/fd6d6b3573aff98f84de.md) |
| 3d/characters/statue/animations/child_attack_02.bdae | fe426ca313cc1522d3e4 | ORPHAN | [details](assets/fe426ca313cc1522d3e4.md) |
| 3d/characters/statue/animations/child_pre_attack_02.bdae | fe8a7e1b22057a3f0876 | ORPHAN | [details](assets/fe8a7e1b22057a3f0876.md) |
| 3d/modules/darkwood/mgp/darkwood_10_01.mgp | ff2e455187276eb34055 | ORPHAN | [details](assets/ff2e455187276eb34055.md) |
| 3d/modules/darkwood/mvp/darkwood_29_01.mvp | ff7218972239b429f133 | ORPHAN | [details](assets/ff7218972239b429f133.md) |
