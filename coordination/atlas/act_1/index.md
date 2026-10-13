# Act 1

> Act membership is INFERENCE. See `../data/act-membership.json`; owner confirmation is pending.

## Levels

| File | Format | Level script | Music | Scene objects | Procedural lists | Source status |
| --- | --- | --- | --- | --- | --- | --- |
| 001_swamp.mlx | campaign | data\PyData\scripts\001_SWAMP.pyscript | SwampHubAmbientMusic | 9 | 0 | DONE |
| 002_swamp_witchcave.rule.xml | campaign | data\pydata\scripts\002_swamp_cave_witch.pyscript | SwampWitchCaveAmbientMusic | 0 | 5 | DONE |


## Level detail

### `001_swamp.mlx`

Source: `.local-inputs/assets-extra/android/data/scene/001_swamp.mlx`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| type | level |
| name | level_config |
| position | 0.0,0.0,0.0 |
| scale | 1.0,1.0,1.0 |
| debugcolor | 27.0,177.0,27.0 |
| rotation | 0.0,0.0,0.0 |
| ambientColor | 0.5,0.5,0.5 |
| camera_file |  |
| camera_name |  |
| camera_zfar | 4200 |
| camera_znear | 900 |
| clear_color | 0,0,0 |
| fog_color | 1,14,0 |
| fog_end | 5800 |
| fog_start | 4000 |
| gametype | LevelConfig |
| skybox | data/3D/skybox/skybox_swamp.bdae |
| music | SwampHubAmbientMusic |
| safezone_music | SwampMerchantCampMusic |
| scriptFile | data\PyData\scripts\001_SWAMP.pyscript |
| light_set | data/3D/Light/swamp.lightset_xml |
| fixed_light_set | data/3D/Light/swamp_es_1_1.lightset_xml |
| prog_vertex_buffer_size | 351600 |
| prog_index_buffer_size | 86604 |
| fix_vertex_buffer_size | 1301796 |
| fix_index_buffer_size | 113910 |


Direct scene objects (not character spawns):

| Type | Name | Game type | Script |
| --- | --- | --- | --- |
| obj | obj_4of4_brdwalk_sw_00_0 | Module |  |
| obj | obj_3of4_brdwalk_sw_00_1 | Module |  |
| obj | obj_1of4_brdwalk_nse_00_2 | Module |  |
| corner | corner_ruin_ws_00_3 | Module |  |
| merchantcamp | merchantcamp_ruins_swe_00_4 | Module |  |
| corner | corner_brdwalk_se_00_5 | Module |  |
| deadend | deadend_brdwalk_w_00_7 | Module |  |
| bossroom | bossroom_ruins_ns__8 | Module |  |
| obj | obj_2of4_brdwalk_sw_00_9 | Module |  |


### `002_swamp_witchcave.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/002_swamp_witchcave.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/swamp |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\002_swamp_cave_witch.pyscript |
| camera_znear | 900 |
| camera_zfar | 5000 |
| music | SwampWitchCaveAmbientMusic |
| ambiant_music | sfx_witch_cave_ambiance |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/swampcave.lightset_xml |
| fixed_light_set | data/3D/Light/swampcave_es1_1.lightset_xml |
| prog_vertex_buffer_size | 319104 |
| prog_index_buffer_size | 58248 |
| fix_vertex_buffer_size | 319104 |
| fix_index_buffer_size | 58248 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| deadends | true | 1 |
| deadends2 | true | 7 |
| tmodules | true | 1 |
| bossroom | true | 1 |
| path_cave | true | 3 |


## Actors and mobs

CharacterProperties rows are linked from exact source-script names, MGP `charpropsname`, or explicit B155/B156 encounter evidence. Other MGP Character objects and `Script_SpawnCharacter` arguments remain conditional candidates; command branches and runtime execution are not evaluated.

| Actor rows | Mapped to this act | Status |
| --- | --- | --- |
| 448 | 18 | INFERENCE: serialized spawn-command targets |

Serialized Script_SpawnCharacter authored targets: 41; modular XML Character candidates: 89. Both are conditional candidates, not a static runtime roster.

### Modular XML Character candidates

Rows come from MGP files referenced directly by scene modules or by procedural-rule gameplay choices. `charpropsname` is an exact CharacterProperties link where present; `char_template` is kept as a template label when the CharacterProperties link is not decoded. Random module selection and `spawn_prob` are preserved as conditions.

| Scene | Module source | Object | CharacterProperties | Template | Spawn probability | AI state | Status |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp | _prim_NPC_PriestGood | WanderingPriest | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp | _prim_NPC | Prisonier2 | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp | _prim_NPC01_001 | Prisonier | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp | _prim_Faery | DefaultFairy | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp | _prim_ActorTroll | Swamp_ActorTroll | None | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp | _prim_ActorPrisoner | Prisonier2 | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp | _prim_MothTemplate_13 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_Monster_3_05_313_356_14_33_13_179_13_38 | Swamp_LizadMan_Type1 | None | None | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_Monster_LizManIntro1 | Swamp_LizadMan_Type1 | None | None | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_Monster_LizManIntro2 | Swamp_LizadMan_Type1 | None | None | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_Monster_FakeMoth_01 | Swamp_MovieMoth | None | None | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_Monster_FakeMoth_02 | Swamp_MovieMoth | None | None | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_LizTemplate_01 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_LizTemplate_02 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_LizTemplate_03 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_MothTemplate_01 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | _prim_MothTemplate_02 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_1of4_brdwalk_nse_00.mgp | _prim_Monster_54_chest2 | Swamp_LizadMan_Type1 | None | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_1of4_brdwalk_nse_00.mgp | _prim_Monster_53_03_001 | Troll | None | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_1of4_brdwalk_nse_00.mgp | _prim_Monster_54_chest1 | Swamp_LizadMan_Type1 | None | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_1of4_brdwalk_nse_00.mgp | _prim_MothTemplate_05 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_1of4_brdwalk_nse_00.mgp | _prim_LizTemplate_05 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_1of4_brdwalk_nse_00.mgp | _prim_LizTemplate_06 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_ruin_ws_00.mgp | _prim_Monster_04_324 | Swamp_LizadMan_Type2 | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_ruin_ws_00.mgp | _prim_Monster_05_325 | Swamp_LizadMan_Type3 | None | None | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_ruin_ws_00.mgp | _prim_Monster_08_329 | Swamp_LizadMan_Type1 | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_ruin_ws_00.mgp | _prim_LizTemplate_07 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_ruin_ws_00.mgp | _prim_LizTemplate_08 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_ruin_ws_00.mgp | _prim_LizTemplate_09 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp | _prim_NPC_04 | NPCmerchantCamp | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp | _prim_NPC_05 | Swamp_MerchantCamp_NPC_A | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp | _prim_NPC_LizBlood | Swamp_MerchantCamp_NPC_C | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp | _prim_NPC_LizBloodPOST | zKill5Lizman_Post | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_brdwalk_se_00.mgp | _prim_Monster_3_276 | Troll | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_brdwalk_se_00.mgp | _prim_Monster_3 | Swamp_LizadMan_Type2 | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_brdwalk_se_00.mgp | _prim_LizTemplate_10 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_brdwalk_se_00.mgp | _prim_LizTemplate_11 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_brdwalk_se_00.mgp | _prim_LizTemplate_13 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/corner_brdwalk_se_00.mgp | _prim_LizTemplate_14 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_brdwalk_w_00.mgp | _prim_LizTemplate_21 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_brdwalk_w_00.mgp | _prim_LizTemplate_22 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/bossroom_ruins_ns_.mgp | _prim_Monster_LizardFan01 | Swamp_SnackMan | None | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/bossroom_ruins_ns_.mgp | _prim_Monster_MisterSnack | Swamp_SnackMan | None | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/bossroom_ruins_ns_.mgp | _prim_Monster_LizardFan03 | Swamp_SnackMan | None | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/bossroom_ruins_ns_.mgp | _prim_Boss_SwampKing_01 | SwampKing | None | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/bossroom_ruins_ns_.mgp | _prim_NPC_Eddie | WanderingEddie | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_2of4_brdwalk_sw_00.mgp | _prim_NPC_Mothgiver | Swamp_MerchantCamp_NPC_B | None | None | None | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_2of4_brdwalk_sw_00.mgp | _prim_MothTemplate_06 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_2of4_brdwalk_sw_00.mgp | _prim_MothTemplate_08 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/obj_2of4_brdwalk_sw_00.mgp | _prim_MothTemplate_10 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_s_00.mgp | _prim_Faery | DefaultFairy | None | None | None | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_s_00.mgp | _prim_NPC_PriestGood | WanderingPriest | None | None | None | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_s_00.mgp | _prim_tmp_commontype1 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_s_00.mgp | _prim_tmp_commontype02 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_s_00.mgp | _prim_NPC_Eddie | WanderingPriest | None | None | None | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_n_00.mgp | _prim_Monster_3_05 | Swamp_LizadMan_Type1 | None | None | None | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_n_00.mgp | _prim_Monster_04 | Swamp_LizadMan_Type2 | None | None | None | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_n_00.mgp | _prim_Monster_05 | Swamp_LizadMan_Type1 | None | None | None | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_epic_n_01.mgp | _prim_Monster_3_05 | Swamp_LizadMan_Type1 | None | None | None | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_epic_n_01.mgp | _prim_Monster_3_06 | Swamp_LizadMan_Type1 | None | None | None | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_epic_n_01.mgp | _prim_epic_merchant | Epic_merchant_swamp | None | None | None | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_e_nse_00_01.mgp | _prim_Monster_09 | Swamp_LizadMan_Type1 | None | None | Limbus | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_e_nse_00_01.mgp | _prim_Monster_10 | Swamp_LizadMan_Type1 | None | None | Limbus | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_e_nse_00_01.mgp | _prim_Monster_11 | Swamp_LizadMan_Type1 | None | None | Limbus | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_e_nse_00_01.mgp | _prim_Monster_22 | Swamp_LizadMan_Type1 | None | None | Limbus | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_e_nse_00_01.mgp | _prim_tmp_commontype1_01 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_e_nse_00_01.mgp | _prim_tmp_commontype1_02 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_e_nse_00_01.mgp | _prim_tmp_commontype1_03 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_e_nse_00_01.mgp | _prim_tmp_commontype1_04 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/bossroom_witch_.mgp | _prim_Monster_04_56 | Witch | None | 100 | Limbus | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a__swpcave_we_00_01.mgp | _prim_Slime_08 | Slime | None | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a__swpcave_we_00_01.mgp | _prim_Slime_09 | Slime | None | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a__swpcave_we_00_01.mgp | _prim_Slime_45 | Slime | None | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a__swpcave_we_00_01.mgp | _prim_Slime_46 | Slime | None | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_nw_00_01.mgp | _prim_tmp_commontype1_moths | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_nw_00_01.mgp | _prim_tmp_commontype1_moths01 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_nw_00_01.mgp | _prim_tmp_commontype1_liz01 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_nw_00_01.mgp | _prim_tmp_commontype1_moths02 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_nw_00_01.mgp | _prim_tmp_commontype1_moths03 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_nw_00_01.mgp | _prim_tmp_commontype1_moths04 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_nw_00_01.mgp | _prim_tmp_commontype1_liz02 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_nw_00_01.mgp | _prim_tmp_commontype1_liz03 | None | Swamp_CommonType1 | 100 | Limbus | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_nw_00_01.mgp | _prim_tmp_commontype1_liz04 | None | Swamp_CommonType1 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_se_00_01.mgp | _prim_tmp_commontype1 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_se_00_01.mgp | _prim_tmp_commontype02 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_se_00_01.mgp | _prim_tmp_commontype03 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_se_00_01.mgp | _prim_tmp_commontype04 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_se_00_01.mgp | _prim_tmp_commontype05 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |
| 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/witch_a_wpcave_se_00_01.mgp | _prim_tmp_commontype06 | None | Swamp_CommonType2 | 100 | Idle | INFERENCE |


### CharacterProperties profiles

This table joins listed actor rows to their original table references. AI script/type values come from the linked AI table row; loot, skills, model and animation remain source-table links. It does not prove the actor spawns in this level.

| Actor | Model | Animation table | AI row | AI facts | Skill tree | Loot | Sounds |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Epic_merchant_swamp | head_cultist [49] | thamos_merchant [79] | Merchant [39] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 7} | -1 | Barrel_Level_01 [9] | Default (reference RESOLVED; source row RESOLVED) |
| NPCmerchantCamp | prisoner [79] | thamos_merchant [79] | Merchant [39] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 7} | -1 | AssassinTreasure [8] | Default (reference RESOLVED; source row RESOLVED) |
| Prisonier | prisoner [79] | Prisonier [51] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| Prisonier2 | prisoner [79] | Prisonier2 [52] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| Slime | slime_green_v2 [89] | Slime [64] | Monster [40] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Weak_Loot [232] | Default (reference RESOLVED; source row RESOLVED) |
| SwampKing | swampking [99] | SwampKing [70] | SwampKing [63] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "swampking_core", "SelfFX": -1, "Trophy": -1, "Type": 4} | SwampKingSkills [33] | SwampKing_Loot [229] | Default (reference RESOLVED; source row RESOLVED) |
| Swamp_ActorTroll | troll_swamp [108] | Troll [73] | Troll [67] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Champion_Loot [231] | Default (reference RESOLVED; source row RESOLVED) |
| Swamp_LizadMan_Type1 | lizardman [60] | LizardMan [34] | LizardMan [30] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Basic_Loot [230] | Default (reference RESOLVED; source row RESOLVED) |
| Swamp_LizadMan_Type2 | lizardman_big [61] | BigLizardMan [7] | LizadManLarge [29] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Champion_Loot [231] | Default (reference RESOLVED; source row RESOLVED) |
| Swamp_LizadMan_Type3 | lizardman_big [61] | BigLizardMan [7] | LizadManLarge [29] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Champion_Loot [231] | Default (reference RESOLVED; source row RESOLVED) |
| Swamp_MerchantCamp_NPC_A | prisoner [79] | Prisonier3 [53] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| Swamp_MerchantCamp_NPC_B | prisoner [79] | Prisonier3 [53] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| Swamp_MerchantCamp_NPC_C | prisoner [79] | Prisonier6 [56] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | Default (reference RESOLVED; source row RESOLVED) |
| Swamp_MovieMoth | moth [66] | Moth [42] | Moth [41] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Weak_Loot [232] | Default (reference RESOLVED; source row RESOLVED) |
| Swamp_SnackMan | lizardman_big [61] | BigLizardMan [7] | LizadManLarge [29] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Basic_Loot [230] | Default (reference RESOLVED; source row RESOLVED) |
| Troll | troll_swamp [108] | Troll [73] | Troll [67] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Champion_Loot [231] | Default (reference RESOLVED; source row RESOLVED) |
| Witch | witch [113] | Witch [76] | Witch [74] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "witch", "SelfFX": -1, "Trophy": -1, "Type": 4} | WitchSkills [35] | Witch_Loot [338] | Default (reference RESOLVED; source row RESOLVED) |
| zKill5Lizman_Post | invisible_man [55] | NPC_Priest [47] | InvisibleMan [27] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 9} | -1 | AssassinTreasure [8] | Default (reference RESOLVED; source row RESOLVED) |


## Bosses

The source HUD predicate `(AI.Flags & 4) != 0` is proven for 59 CharacterProperties-linked AI rows. 6 name-only markers remain supplemental unresolved candidates. This source marker does not establish encounter placement or runtime spawn conditions.

| Boss flag rows | Name-only supplements | Act-linked candidates | Placement status |
| --- | --- | --- | --- |
| 59 | 6 | 4 | INFERENCE / unresolved runtime placement |


## Dialogues

The dialog table is inventoried globally. Serialized `Script_StartDialog` occurrences are listed as command rows; mapping them to dialog table IDs remains unresolved.

Dialog table rows: 55; StartDialog commands: 123.

## Serialized level scripts

The configured `.pyscript` names resolve to original serialized `_pyscriptnames.bin` and `_pyscripts.bin` pairs. Atlas decodes script names, command kinds, and raw fields without executing callbacks.

| Scene | Script file | Scripts | Commands | Decode status |
| --- | --- | --- | --- | --- |
| 001_swamp.mlx | data\PyData\scripts\001_SWAMP.pyscript | 55 | 789 | DONE |
| 002_swamp_witchcave.rule.xml | data\pydata\scripts\002_swamp_cave_witch.pyscript | 24 | 183 | DONE |


### Command readiness by native kind

The native factory/reader addresses come from the recovered C1 descriptor map. Port anchors identify current dispatch code only; PARTIAL/UNKNOWN does not establish complete source parity or runtime verification.

| Kind | Command | Count | Factory address | Reader address | Port status | Code anchors |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | Script_ExecScript | 47 | 0x458d40 | 0x455628 | UNKNOWN |  |
| 0 | Script_ExecScript | 8 | 0x458d40 | 0x455628 | UNKNOWN |  |
| 3 | Script_CONSOLE | 14 | 0x458c68 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:680, port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:66 |
| 3 | Script_CONSOLE | 4 | 0x458c68 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:680, port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:66 |
| 5 | Script_PlayCamera | 40 | 0x458bd8 | 0x455628 | UNKNOWN |  |
| 5 | Script_PlayCamera | 5 | 0x458bd8 | 0x455628 | UNKNOWN |  |
| 6 | Script_SetCameraClip | 5 | 0x458b90 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:681 |
| 6 | Script_SetCameraClip | 2 | 0x458b90 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:681 |
| 8 | Script_SetCameraTarget | 35 | 0x458b00 | 0x455628 | UNKNOWN |  |
| 8 | Script_SetCameraTarget | 9 | 0x458b00 | 0x455628 | UNKNOWN |  |
| 10 | Script_StartDialog | 112 | 0x458a70 | 0x459034 | UNKNOWN |  |
| 10 | Script_StartDialog | 11 | 0x458a70 | 0x459034 | UNKNOWN |  |
| 12 | Script_WaitDialog | 102 | 0x4589e0 | 0x455628 | UNKNOWN |  |
| 12 | Script_WaitDialog | 9 | 0x4589e0 | 0x455628 | UNKNOWN |  |
| 13 | Script_PlaySound | 10 | 0x458998 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:755 |
| 13 | Script_PlaySound | 6 | 0x458998 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:755 |
| 14 | Script_StopSound | 4 | 0x458950 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:732 |
| 15 | Script_PlayLevelMusic | 1 | 0x458908 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:769 |
| 15 | Script_PlayLevelMusic | 1 | 0x458908 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:769 |
| 16 | Script_EnterSafeZone | 1 | 0x4588c0 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:65 |
| 17 | Script_LeaveSafeZone | 1 | 0x458878 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:65 |
| 19 | Script_PlayAnimByName | 5 | 0x4587e8 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:692 |
| 20 | Script_PlayEffect | 16 | 0x4587a0 | 0x4598b4 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:702 |
| 20 | Script_PlayEffect | 6 | 0x4587a0 | 0x4598b4 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:702 |
| 21 | Script_StopEffect | 8 | 0x458758 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:726 |
| 21 | Script_StopEffect | 2 | 0x458758 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:726 |
| 24 | Script_LockCharacter | 26 | 0x458678 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 24 | Script_LockCharacter | 4 | 0x458678 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 25 | Script_UnlockCharacter | 27 | 0x458630 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 25 | Script_UnlockCharacter | 4 | 0x458630 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 26 | Script_Wait | 42 | 0x4585e0 | 0x455628 | UNKNOWN |  |
| 26 | Script_Wait | 22 | 0x4585e0 | 0x455628 | UNKNOWN |  |
| 27 | Script_SetFaeryState | 1 | 0x458598 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:49 |
| 29 | Script_PutCharacterInLimbus | 4 | 0x458508 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:669 |
| 29 | Script_PutCharacterInLimbus | 1 | 0x458508 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:669 |
| 30 | Script_SpawnCharacter | 12 | 0x4584c0 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:37 |
| 30 | Script_SpawnCharacter | 29 | 0x4584c0 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:37 |
| 31 | Script_PutCharacterInIdle | 10 | 0x458478 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:38 |
| 31 | Script_PutCharacterInIdle | 1 | 0x458478 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:38 |
| 39 | Script_StopActor | 1 | 0x458238 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:38 |
| 40 | Script_MoveActor | 3 | 0x4581e4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:574 |
| 41 | Script_LookActor | 45 | 0x45819c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:637 |
| 41 | Script_LookActor | 12 | 0x45819c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:637 |
| 42 | Script_ShowActor | 15 | 0x458154 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:661 |
| 42 | Script_ShowActor | 2 | 0x458154 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:661 |
| 43 | Script_HideActor | 10 | 0x45810c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:662 |
| 43 | Script_HideActor | 1 | 0x45810c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:662 |
| 44 | Script_KillActor | 1 | 0x4580c4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:588 |
| 44 | Script_KillActor | 1 | 0x4580c4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:588 |
| 45 | Script_PlayActorAnim | 127 | 0x4595c8 | 0x45a2ec | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:595 |
| 45 | Script_PlayActorAnim | 23 | 0x4595c8 | 0x45a2ec | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:595 |
| 46 | Script_SetActorPosition | 47 | 0x45807c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:619 |
| 46 | Script_SetActorPosition | 15 | 0x45807c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:619 |
| 51 | Script_UnEquipHands | 2 | 0x457f14 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:745 |
| 52 | Script_ReEquipHands | 2 | 0x457ecc | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:746 |
| 54 | Script_OpenDoor | 3 | 0x457e34 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:772 |
| 54 | Script_OpenDoor | 3 | 0x457e34 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:772 |
| 55 | Script_CloseDoor | 1 | 0x457de4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:773 |
| 55 | Script_CloseDoor | 1 | 0x457de4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:773 |
| 77 | Script_LockTutorial | 3 | 0x4577a4 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:53 |
| 78 | Script_DoTutorial | 3 | 0x45775c | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:54 |
| 79 | Script_FlushMessages | 3 | 0x457714 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:63 |
| 79 | Script_FlushMessages | 1 | 0x457714 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:63 |


### Actor, dialogue, transition, and audio command details

The table below preserves exact command labels and decoded string operands. A `Script_SpawnCharacter` target is an authored object-name argument; it does not prove which conditional branch ran in gameplay.

| Scene | Script name | Command | String operands | Count |
| --- | --- | --- | --- | --- |
| 001_swamp.mlx | Camp_Intro | Script_LookActor | LocalPlayer; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Camp_Intro | Script_LookActor | _prim_NPC_PriestGood; LocalPlayer | 1 |
| 001_swamp.mlx | Camp_Intro | Script_LookActor | _prim_Waypoint_CampCam; LocalPlayer | 1 |
| 001_swamp.mlx | Camp_Intro | Script_LookActor | _prim_Waypoint_CampCam; _prim_Faery | 1 |
| 001_swamp.mlx | Camp_Intro | Script_LookActor | _prim_Waypoint_CampCam; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Camp_Intro | Script_SetActorPosition | _prim_Waypoint_CampIntro_FAERY; _prim_Faery | 1 |
| 001_swamp.mlx | Camp_Intro | Script_SetActorPosition | _prim_Waypoint_CampIntro_PRINCE; LocalPlayer | 1 |
| 001_swamp.mlx | Camp_Intro | Script_SetActorPosition | _prim_Waypoint_CampIntro_RENE; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Camp_Intro | Script_StartDialog |  | 5 |
| 001_swamp.mlx | Camp_Intro | Script_WaitDialog |  | 5 |
| 001_swamp.mlx | CombatTuto | Script_StartDialog |  | 9 |
| 001_swamp.mlx | CombatTuto | Script_WaitDialog |  | 9 |
| 001_swamp.mlx | Door_Inspection_South | Script_StartDialog |  | 3 |
| 001_swamp.mlx | Door_Inspection_South | Script_WaitDialog |  | 3 |
| 001_swamp.mlx | FaeryBackFlip | Script_PlayActorAnim | DefaultFairy | 1 |
| 001_swamp.mlx | FaeryIdle | Script_PlayActorAnim | DefaultFairy | 1 |
| 001_swamp.mlx | FaeryLoop | Script_PlayActorAnim | DefaultFairy | 1 |
| 001_swamp.mlx | FaeryTwist | Script_PlayActorAnim | DefaultFairy | 1 |
| 001_swamp.mlx | Faerycrazy | Script_PlayActorAnim | DefaultFairy | 1 |
| 001_swamp.mlx | HackSave | Script_SetActorPosition | _prim_Waypoint_BlueDoor; LocalPlayer | 1 |
| 001_swamp.mlx | LizBlood_Activate | Script_LookActor | LocalPlayer; _prim_NPC_LizBlood | 1 |
| 001_swamp.mlx | LizBlood_Activate | Script_LookActor | _prim_NPC_LizBlood; LocalPlayer | 1 |
| 001_swamp.mlx | LizBlood_Activate | Script_StartDialog |  | 4 |
| 001_swamp.mlx | LizBlood_Activate | Script_WaitDialog |  | 4 |
| 001_swamp.mlx | LizBlood_PostAccept | Script_StartDialog |  | 1 |
| 001_swamp.mlx | LizBlood_PostAccept | Script_WaitDialog |  | 1 |
| 001_swamp.mlx | LizCaptain_Intro | Script_KillActor | _prim_Monster_08_329 | 1 |
| 001_swamp.mlx | LizCaptain_Intro | Script_LookActor | LocalPlayer; _prim_Monster_05_325 | 1 |
| 001_swamp.mlx | LizCaptain_Intro | Script_PlayActorAnim | _prim_Monster_05_325 | 4 |
| 001_swamp.mlx | LizCaptain_Intro | Script_PlaySound |  | 3 |
| 001_swamp.mlx | LizCaptain_Intro | Script_SpawnCharacter | _prim_Monster_05_325 | 1 |
| 001_swamp.mlx | LizardMan_Ambush1 | Script_SpawnCharacter | _prim_Monster_54_chest1 | 1 |
| 001_swamp.mlx | LizardMan_Ambush1 | Script_SpawnCharacter | _prim_Monster_54_chest2 | 1 |
| 001_swamp.mlx | LizardMan_Ambush2 | Script_SpawnCharacter | _prim_Monster_3_312_355_13_32_12_178_12_37_58 | 1 |
| 001_swamp.mlx | LizardMan_Intro | Script_SpawnCharacter | _prim_Monster_LizManIntro1 | 1 |
| 001_swamp.mlx | LizardMan_Intro | Script_SpawnCharacter | _prim_Monster_LizManIntro2 | 1 |
| 001_swamp.mlx | MothIntro | Script_LookActor | LocalPlayer; _prim_Monster_FakeMoth_01 | 1 |
| 001_swamp.mlx | MothIntro | Script_LookActor | LocalPlayer; _prim_Monster_FakeMoth_02 | 1 |
| 001_swamp.mlx | MothIntro | Script_LookActor | _prim_Waypoint_MothSpotLookTo; _prim_Monster_FakeMoth_01 | 1 |
| 001_swamp.mlx | MothIntro | Script_LookActor | _prim_Waypoint_MothSpotLookTo; _prim_Monster_FakeMoth_02 | 1 |
| 001_swamp.mlx | MothIntro | Script_PlayActorAnim | _prim_Monster_FakeMoth_01 | 1 |
| 001_swamp.mlx | MothIntro | Script_PlayActorAnim | _prim_Monster_FakeMoth_02 | 1 |
| 001_swamp.mlx | MothIntro | Script_PlaySound |  | 1 |
| 001_swamp.mlx | MothIntro | Script_PutCharacterInIdle | _prim_Monster_FakeMoth_01 | 1 |
| 001_swamp.mlx | MothIntro | Script_PutCharacterInIdle | _prim_Monster_FakeMoth_02 | 1 |
| 001_swamp.mlx | MothIntro | Script_SetActorPosition | _prim_Waypoint_MothSpot01; _prim_Monster_FakeMoth_01 | 1 |
| 001_swamp.mlx | MothIntro | Script_SetActorPosition | _prim_Waypoint_MothSpot02; _prim_Monster_FakeMoth_02 | 1 |
| 001_swamp.mlx | MothIntro | Script_SetActorPosition | _prim_Waypoint_MothSpot_XYZ; _prim_Monster_FakeMoth_01 | 1 |
| 001_swamp.mlx | MothIntro | Script_SetActorPosition | _prim_Waypoint_MothSpot_XYZ; _prim_Monster_FakeMoth_02 | 1 |
| 001_swamp.mlx | MothIntro | Script_SpawnCharacter | _prim_Monster_FakeMoth_01 | 1 |
| 001_swamp.mlx | MothIntro | Script_SpawnCharacter | _prim_Monster_FakeMoth_02 | 1 |
| 001_swamp.mlx | Movement_Tuto2 | Script_StartDialog |  | 4 |
| 001_swamp.mlx | Movement_Tuto2 | Script_WaitDialog |  | 4 |
| 001_swamp.mlx | PlayerDialog | Script_PlayActorAnim | LocalPlayer | 1 |
| 001_swamp.mlx | PlayerIdle | Script_PlayActorAnim | LocalPlayer | 1 |
| 001_swamp.mlx | ResumeLevelMusic | Script_PlayLevelMusic |  | 1 |
| 001_swamp.mlx | Swamp2_Intro | Script_StartDialog |  | 5 |
| 001_swamp.mlx | Swamp2_Intro | Script_WaitDialog |  | 5 |
| 001_swamp.mlx | Swamp_Camp_PrisonerB_a | Script_LookActor | LocalPlayer; _prim_NPC_LizBlood | 1 |
| 001_swamp.mlx | Swamp_Camp_PrisonerB_a | Script_LookActor | _prim_NPC_LizBlood; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Camp_PrisonerB_a | Script_StartDialog |  | 3 |
| 001_swamp.mlx | Swamp_Camp_PrisonerB_a | Script_WaitDialog |  | 3 |
| 001_swamp.mlx | Swamp_Camp_PrisonerB_b | Script_LookActor | LocalPlayer; _prim_NPC_LizBlood | 1 |
| 001_swamp.mlx | Swamp_Camp_PrisonerB_b | Script_LookActor | _prim_NPC_LizBlood; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Camp_PrisonerB_b | Script_StartDialog |  | 1 |
| 001_swamp.mlx | Swamp_Camp_PrisonerB_b | Script_WaitDialog |  | 1 |
| 001_swamp.mlx | Swamp_Camp_PrisonerC_a | Script_StartDialog |  | 2 |
| 001_swamp.mlx | Swamp_Camp_PrisonerC_a | Script_WaitDialog |  | 2 |
| 001_swamp.mlx | Swamp_Camp_PrisonerC_b | Script_LookActor | _prim_NPC_Mothgiver; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Camp_PrisonerC_b | Script_LookActor | _prim_Waypoint_postmothlookto; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Camp_PrisonerC_b | Script_StartDialog |  | 2 |
| 001_swamp.mlx | Swamp_Camp_PrisonerC_b | Script_WaitDialog |  | 2 |
| 001_swamp.mlx | Swamp_Camp_PrisonerE_a | Script_StartDialog |  | 3 |
| 001_swamp.mlx | Swamp_Camp_PrisonerE_a | Script_WaitDialog |  | 3 |
| 001_swamp.mlx | Swamp_Intro | Script_HideActor | _prim_ActorPrisoner | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_HideActor | _prim_ActorTroll | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_HideActor | _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_HideActor | _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_LookActor | _prim_NPC_PriestGood; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_LookActor | _prim_Waypoint_LookSpot; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_LookActor | _prim_Waypoint_LookSpot; _prim_ActorPrisoner | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_LookActor | _prim_Waypoint_LookSpot; _prim_ActorTroll | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_LookActor | _prim_Waypoint_LookSpot; _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_LookActor | _prim_Waypoint_LookSpot; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_PlayActorAnim | Player | 18 |
| 001_swamp.mlx | Swamp_Intro | Script_PlayActorAnim | _prim_ActorPrisoner | 3 |
| 001_swamp.mlx | Swamp_Intro | Script_PlayActorAnim | _prim_ActorTroll | 5 |
| 001_swamp.mlx | Swamp_Intro | Script_PlayActorAnim | _prim_Faery | 11 |
| 001_swamp.mlx | Swamp_Intro | Script_PlayActorAnim | _prim_NPC_PriestGood | 11 |
| 001_swamp.mlx | Swamp_Intro | Script_PutCharacterInIdle | LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_PutCharacterInIdle | _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_PutCharacterInIdle | _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_5; LocalPlayer | 2 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_5; _prim_ActorPrisoner | 2 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_5; _prim_ActorTroll | 2 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_5; _prim_Faery | 3 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_5; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_CamTroll; _prim_ActorTroll | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_Faery_03; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_Faery_04; _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_TrollStack; _prim_ActorTroll | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_SetActorPosition | _prim_Waypoint_test; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_ShowActor | _prim_ActorTroll | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_ShowActor | _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_ShowActor | _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_ShowActor | _prim_Zone_EnterLoc_Prison | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_SpawnCharacter | _prim_ActorTroll | 1 |
| 001_swamp.mlx | Swamp_Intro | Script_StartDialog |  | 14 |
| 001_swamp.mlx | Swamp_Intro | Script_StopSound |  | 2 |
| 001_swamp.mlx | Swamp_Intro | Script_WaitDialog |  | 14 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_HideActor | _prim_Boss_SwampKing_01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_LookActor | _prim_Waypoint_FaerySpot01; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_LookActor | _prim_Waypoint_SKLookTo; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_LookActor | _prim_Waypoint_SKLookTo; _prim_Boss_SwampKing_01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_LookActor | _prim_Waypoint_SKLookTo; _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_LookActor | _prim_Waypoint_SKLookTo; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_OpenDoor | _prim_Door_exit | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_PlayActorAnim | LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_PlayActorAnim | Player | 11 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_PlayActorAnim | _prim_Boss_SwampKing_01 | 3 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_PlayActorAnim | _prim_Faery | 7 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_PlayActorAnim | _prim_NPC_PriestGood | 9 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_PlaySound |  | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_PutCharacterInIdle | LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_PutCharacterInIdle | _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_PutCharacterInIdle | _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_SetActorPosition | _prim_Waypoint_Center; LocalPlayer | 2 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_SetActorPosition | _prim_Waypoint_Center; _prim_Boss_SwampKing_01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_SetActorPosition | _prim_Waypoint_Center; _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_SetActorPosition | _prim_Waypoint_Center; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_SetActorPosition | _prim_Waypoint_PrinceEndingSpot; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_SetActorPosition | _prim_Waypoint_kinghider; _prim_Boss_SwampKing_01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_StartDialog |  | 12 |
| 001_swamp.mlx | Swamp_Serpent_Defeated | Script_WaitDialog |  | 12 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_CloseDoor | _prim_WallBreak | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_HideActor | _prim_Boss_SwampKing_01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_HideActor | _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_HideActor | _prim_Monster_LizardFan01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_HideActor | _prim_Monster_LizardFan03 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_HideActor | _prim_Monster_MisterSnack | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | LocalPlayer; _prim_Boss_SwampKing_01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | _prim_Boss_SwampKing_01; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | _prim_Boss_SwampKing_01; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | _prim_Waypoint_SKLookTo; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | _prim_Waypoint_SKLookTo; _prim_Boss_SwampKing_01 | 4 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | _prim_Waypoint_SKLookTo; _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | _prim_Waypoint_SKLookTo; _prim_Monster_LizardFan01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | _prim_Waypoint_SKLookTo; _prim_Monster_LizardFan03 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | _prim_Waypoint_SKLookTo; _prim_Monster_MisterSnack | 2 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_LookActor | _prim_Waypoint_SKLookTo; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_PlayActorAnim | Player | 7 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_PlayActorAnim | _prim_Boss_SwampKing_01 | 4 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_PlayActorAnim | _prim_Faery | 6 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_PlayActorAnim | _prim_Monster_LizardFan01 | 3 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_PlayActorAnim | _prim_Monster_LizardFan03 | 3 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_PlayActorAnim | _prim_Monster_MisterSnack | 4 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_PlayActorAnim | _prim_NPC_PriestGood | 5 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_PlaySound |  | 4 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_PutCharacterInIdle | _prim_Boss_SwampKing_01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_BossSpawnPoint_03; _prim_Boss_SwampKing_01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_Center; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_Center; _prim_Boss_SwampKing_01 | 4 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_Center; _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_Center; _prim_Monster_LizardFan01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_Center; _prim_Monster_LizardFan03 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_Center; _prim_Monster_MisterSnack | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_Center; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_FaerySpot01; _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_PriestSpot01; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_SetActorPosition | _prim_Waypoint_PrinceSpot; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_ShowActor | _prim_Boss_SwampKing_01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_ShowActor | _prim_Faery | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_ShowActor | _prim_Monster_LizardFan01 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_ShowActor | _prim_Monster_LizardFan03 | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_ShowActor | _prim_Monster_MisterSnack | 1 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_StartDialog |  | 3 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_StopSound |  | 2 |
| 001_swamp.mlx | Swamp_Serpent_Intro | Script_WaitDialog |  | 3 |
| 001_swamp.mlx | Swamp_Witch_Activate | Script_LookActor | _prim_Faery; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Witch_Activate | Script_LookActor | _prim_NPC_05; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Witch_Activate | Script_MoveActor | _prim_Waypoint_PrinceStand; LocalPlayer | 1 |
| 001_swamp.mlx | Swamp_Witch_Activate | Script_StartDialog |  | 8 |
| 001_swamp.mlx | Swamp_Witch_Activate | Script_WaitDialog |  | 8 |
| 001_swamp.mlx | SwimKing_01 | Script_SpawnCharacter | _prim_SwimKing_01 | 1 |
| 001_swamp.mlx | SwimKing_02 | Script_SpawnCharacter | _prim_SwimKing_02 | 1 |
| 001_swamp.mlx | SwimKing_03 | Script_SpawnCharacter | _prim_SwimKing_03 | 1 |
| 001_swamp.mlx | TrollReturn | Script_MoveActor | _prim_Waypoint_TrollWalkTo; _prim_Monster_53_03_001 | 2 |
| 001_swamp.mlx | TrollReturn | Script_PlayActorAnim | _prim_Monster_53_03_001 | 3 |
| 001_swamp.mlx | TrollReturn | Script_PlaySound |  | 1 |
| 001_swamp.mlx | TrollReturn | Script_PutCharacterInIdle | _prim_Monster_53_03_001 | 1 |
| 001_swamp.mlx | Tutorial_Merchant | Script_StartDialog |  | 8 |
| 001_swamp.mlx | Tutorial_Merchant | Script_WaitDialog |  | 8 |
| 001_swamp.mlx | chest_tuto | Script_StartDialog |  | 4 |
| 001_swamp.mlx | chest_tuto | Script_WaitDialog |  | 4 |
| 001_swamp.mlx | cinematic_Tuto_levelUp | Script_StartDialog |  | 5 |
| 001_swamp.mlx | cinematic_Tuto_levelUp | Script_WaitDialog |  | 5 |
| 001_swamp.mlx | cinematic_Tuto_potionUse | Script_StartDialog |  | 6 |
| 001_swamp.mlx | cinematic_Tuto_potionUse | Script_WaitDialog |  | 6 |
| 001_swamp.mlx | enterLocation_MerchantCamp | Script_StartDialog |  | 1 |
| 001_swamp.mlx | enterLocation_North_Boardwalk | Script_StartDialog |  | 1 |
| 001_swamp.mlx | enterLocation_South_Boardwalk | Script_StartDialog |  | 1 |
| 001_swamp.mlx | enterLocation_StartRoom | Script_StartDialog |  | 1 |
| 001_swamp.mlx | enterLocation_Swamp | Script_StartDialog |  | 1 |
| 001_swamp.mlx | enterLocation_Swamp_EDWARD | Script_SetActorPosition | LocalPlayer; _prim_NPC_Eddie | 1 |
| 001_swamp.mlx | enterLocation_Swamp_EDWARD | Script_ShowActor | _prim_NPC_Eddie | 2 |
| 001_swamp.mlx | enterLocation_Swamp_EDWARD | Script_StartDialog |  | 1 |
| 001_swamp.mlx | enterLocation_Swamp_King | Script_StartDialog |  | 1 |
| 001_swamp.mlx | enterLocation_Swamp_RENE | Script_SetActorPosition | LocalPlayer; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | enterLocation_Swamp_RENE | Script_ShowActor | _prim_NPC_PriestGood | 2 |
| 001_swamp.mlx | enterLocation_Swamp_RENE | Script_StartDialog |  | 1 |
| 001_swamp.mlx | enterLocation_Swamp_RENE2 | Script_SetActorPosition | LocalPlayer; _prim_NPC_PriestGood | 1 |
| 001_swamp.mlx | enterLocation_Swamp_RENE2 | Script_ShowActor | _prim_NPC_PriestGood | 2 |
| 001_swamp.mlx | enterLocation_To_Witch_Cave | Script_StartDialog |  | 1 |
| 001_swamp.mlx | exitLocation_MerchantCamp | Script_StartDialog |  | 1 |
| 001_swamp.mlx | open_SwampKingDoor | Script_OpenDoor | _prim_SwampDoor | 1 |
| 001_swamp.mlx | open_SwampKingDoor | Script_OpenDoor | _prim_SwampDoor_FIRE | 1 |
| 002_swamp_witchcave.rule.xml | Door_Teleport | Script_PlaySound |  | 1 |
| 002_swamp_witchcave.rule.xml | Door_Teleport | Script_SetActorPosition | _prim_Waypoint_doorport; LocalPlayer | 1 |
| 002_swamp_witchcave.rule.xml | Epic_hello | Script_LookActor | LocalPlayer; _prim_epic_merchant | 1 |
| 002_swamp_witchcave.rule.xml | Epic_hello | Script_LookActor | _prim_epic_merchant; LocalPlayer | 1 |
| 002_swamp_witchcave.rule.xml | Epic_hello | Script_ShowActor | _prim_epic_merchant | 1 |
| 002_swamp_witchcave.rule.xml | Epic_hello | Script_StartDialog |  | 3 |
| 002_swamp_witchcave.rule.xml | Epic_hello | Script_WaitDialog |  | 3 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_CloseDoor | _prim_BlueDoor | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_LookActor | LocalPlayer; _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_LookActor | _prim_Monster_04_56; LocalPlayer | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_LookActor | _prim_Monster_04_56; _prim_Faery | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_LookActor | _prim_Monster_04_56; _prim_NPC_PriestGood | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_LookActor | _prim_Waypoint_CenterLookTo; LocalPlayer | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_LookActor | _prim_Waypoint_CenterLookTo; _prim_Faery | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_LookActor | _prim_Waypoint_CenterLookTo; _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_LookActor | _prim_Waypoint_CenterLookTo; _prim_NPC_PriestGood | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_PlayActorAnim | Player | 6 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_PlayActorAnim | _prim_Faery | 7 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_PlayActorAnim | _prim_Monster_04_56 | 2 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_PlayActorAnim | _prim_NPC_PriestGood | 8 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_PlaySound |  | 4 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_SetActorPosition | _prim_Waypoint_Center; LocalPlayer | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_SetActorPosition | _prim_Waypoint_Center; _prim_Faery | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_SetActorPosition | _prim_Waypoint_Center; _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_SetActorPosition | _prim_Waypoint_Center; _prim_NPC_PriestGood | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_SetActorPosition | _prim_Waypoint_FaerySpot; _prim_Faery | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_SetActorPosition | _prim_Waypoint_PlayerCorner_02; LocalPlayer | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_SetActorPosition | _prim_Waypoint_PriestSpot; _prim_NPC_PriestGood | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_SetActorPosition | _prim_Waypoint_WitchSpotter; _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_SpawnCharacter | _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_StartDialog |  | 3 |
| 002_swamp_witchcave.rule.xml | Intro_Witch | Script_WaitDialog |  | 3 |
| 002_swamp_witchcave.rule.xml | LizardManJump1 | Script_SpawnCharacter | _prim_LizardMan_01 | 1 |
| 002_swamp_witchcave.rule.xml | LizardManJump1 | Script_SpawnCharacter | _prim_LizardMan_02 | 1 |
| 002_swamp_witchcave.rule.xml | LizardManJump1 | Script_SpawnCharacter | _prim_LizardMan_03 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn1 | Script_SpawnCharacter | _prim_Skeleton_27 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn2 | Script_SpawnCharacter | _prim_Skeleton_28 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn3 | Script_SpawnCharacter | _prim_Skeleton_29 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn4 | Script_SpawnCharacter | _prim_Skeleton_30 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn4 | Script_SpawnCharacter | _prim_Skeleton_31 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn5 | Script_SpawnCharacter | _prim_Skeleton_38 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn6 | Script_SpawnCharacter | _prim_Skeleton_32 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn7 | Script_SpawnCharacter | _prim_Skeleton_37 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn8 | Script_SpawnCharacter | _prim_Skeleton_33 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn9 | Script_SpawnCharacter | _prim_Skeleton_35 | 1 |
| 002_swamp_witchcave.rule.xml | MiniSkelSpawn9 | Script_SpawnCharacter | _prim_Skeleton_36 | 1 |
| 002_swamp_witchcave.rule.xml | Open_portal_witch | Script_OpenDoor | _prim_portal_witch | 1 |
| 002_swamp_witchcave.rule.xml | ResumeLevelMusic | Script_PlayLevelMusic |  | 1 |
| 002_swamp_witchcave.rule.xml | Return_Cinematic | Script_KillActor | _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | Return_Cinematic | Script_LookActor | LocalPlayer; _prim_Faery | 1 |
| 002_swamp_witchcave.rule.xml | Return_Cinematic | Script_OpenDoor | _prim_BlueDoor | 1 |
| 002_swamp_witchcave.rule.xml | Return_Cinematic | Script_OpenDoor | _prim_portal_witch | 1 |
| 002_swamp_witchcave.rule.xml | Return_Cinematic | Script_PlaySound |  | 1 |
| 002_swamp_witchcave.rule.xml | Return_Cinematic | Script_StartDialog |  | 3 |
| 002_swamp_witchcave.rule.xml | Return_Cinematic | Script_WaitDialog |  | 3 |
| 002_swamp_witchcave.rule.xml | SkeletonSpawn1 | Script_SpawnCharacter | _prim_Skeleton_13 | 1 |
| 002_swamp_witchcave.rule.xml | SkeletonSpawn1 | Script_SpawnCharacter | _prim_Skeleton_14 | 1 |
| 002_swamp_witchcave.rule.xml | SkeletonSpawn1 | Script_SpawnCharacter | _prim_Skeleton_15 | 1 |
| 002_swamp_witchcave.rule.xml | SkeletonSpawn1 | Script_SpawnCharacter | _prim_Skeleton_16 | 1 |
| 002_swamp_witchcave.rule.xml | SkeletonSpawn1 | Script_SpawnCharacter | _prim_Skeleton_17 | 1 |
| 002_swamp_witchcave.rule.xml | SkipCutscene_Bugfix | Script_SetActorPosition | _prim_Waypoint_FaerySpot; _prim_Faery | 1 |
| 002_swamp_witchcave.rule.xml | SkipCutscene_Bugfix | Script_SetActorPosition | _prim_Waypoint_PlayerCorner_02; LocalPlayer | 1 |
| 002_swamp_witchcave.rule.xml | SkipCutscene_Bugfix | Script_SetActorPosition | _prim_Waypoint_PriestSpot; _prim_NPC_PriestGood | 1 |
| 002_swamp_witchcave.rule.xml | SkipCutscene_Bugfix | Script_SetActorPosition | _prim_Waypoint_WitchSpotter; _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | SlimeSpawn1 | Script_SpawnCharacter | _prim_Slime_19 | 1 |
| 002_swamp_witchcave.rule.xml | SlimeSpawn1 | Script_SpawnCharacter | _prim_Slime_20 | 1 |
| 002_swamp_witchcave.rule.xml | SlimeSpawn1 | Script_SpawnCharacter | _prim_Slime_21 | 1 |
| 002_swamp_witchcave.rule.xml | SlimeSpawn1 | Script_SpawnCharacter | _prim_Slime_22 | 1 |
| 002_swamp_witchcave.rule.xml | SlimeSpawn1 | Script_SpawnCharacter | _prim_Slime_23 | 1 |
| 002_swamp_witchcave.rule.xml | WitchTeleport | Script_HideActor | _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | WitchTeleport | Script_LookActor | _prim_Waypoint_PlayerCorner_02; _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | WitchTeleport | Script_PutCharacterInIdle | _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | WitchTeleport | Script_SetActorPosition | _prim_Waypoint_WitchSpotter; _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | WitchTeleport | Script_ShowActor | _prim_Monster_04_56 | 1 |
| 002_swamp_witchcave.rule.xml | enterLocation_SwampCave | Script_SetActorPosition | LocalPlayer; _prim_NPC_PriestGood | 1 |
| 002_swamp_witchcave.rule.xml | enterLocation_SwampCave | Script_StartDialog |  | 1 |
| 002_swamp_witchcave.rule.xml | enterLocation_SwampCave_Inner_Den | Script_StartDialog |  | 1 |
| 002_swamp_witchcave.rule.xml | lezardman_chest_ambush | Script_SpawnCharacter | _prim_Monster_09 | 1 |
| 002_swamp_witchcave.rule.xml | lezardman_chest_ambush | Script_SpawnCharacter | _prim_Monster_10 | 1 |
| 002_swamp_witchcave.rule.xml | lezardman_chest_ambush | Script_SpawnCharacter | _prim_Monster_11 | 1 |
| 002_swamp_witchcave.rule.xml | lezardman_chest_ambush | Script_SpawnCharacter | _prim_Monster_22 | 1 |


Raw strings are recorded in `../data/campaign-references.json`. Command labels and recovered field offsets identify spawn, transition, dialogue, and audio command records; runtime branch conditions and dialogue-table IDs remain unresolved.

## Source-linked asset detail pages

Each page joins the source asset inventory to scene/MGP reference rows and carries the resolver status, confidence, rule, port metadata, and source path.

| Asset | Stable ID | Source status | Page |
| --- | --- | --- | --- |
| 3d/characters/troll/animations/troll_swamp_great_pushback_dying.bdae | 0053b2318c4dc72fd323 | ORPHAN | [details](assets/0053b2318c4dc72fd323.md) |
| 3d/characters/witch/animations/witch_spawn.bdae | 016b8d79bedf01da4754 | ORPHAN | [details](assets/016b8d79bedf01da4754.md) |
| file001716.dat | 01e2dae8d9f07237fceb | ORPHAN | [details](assets/01e2dae8d9f07237fceb.md) |
| 3d/characters/troll/animations/troll_swamp_great_pushback.bdae | 020a2cc741b9c3e3cbf7 | ORPHAN | [details](assets/020a2cc741b9c3e3cbf7.md) |
| 3d/characters/slime/animations/slime_idle.bdae | 0248243efe5c5471bb24 | ORPHAN | [details](assets/0248243efe5c5471bb24.md) |
| sfx_cutscene_witch_spawn.wav | 0493ad8197bace40e26c | ORPHAN | [details](assets/0493ad8197bace40e26c.md) |
| sounds/sfx_lizardman_hurt.wav | 04c923b8516a8a392847 | ORPHAN | [details](assets/04c923b8516a8a392847.md) |
| 3d/characters/lizardman/animations/big_lizardman_great_pushback_dying.bdae | 05c3a6ea265f29de472a | ORPHAN | [details](assets/05c3a6ea265f29de472a.md) |
| 3d/characters/lizardman/animations/lizardman_spawn_jump.bdae | 06c9ea2b89ed2b68137c | ORPHAN | [details](assets/06c9ea2b89ed2b68137c.md) |
| sounds/sfx_lizardman_attack_2.wav | 09206132f7e8d661e690 | ORPHAN | [details](assets/09206132f7e8d661e690.md) |
| 3d/characters/prince/animations/prince_1hand_post_combo_03_moving.bdae | 095e5e36cfd5ef97a2fd | ORPHAN | [details](assets/095e5e36cfd5ef97a2fd.md) |
| file006061.dat | 0ac454bc3a3ccca147cd | ORPHAN | [details](assets/0ac454bc3a3ccca147cd.md) |
| 3d/characters/lizardman/animations/lizardman_post_attack_01.bdae | 0ac64ff0ff22b748f66e | ORPHAN | [details](assets/0ac64ff0ff22b748f66e.md) |
| 3d/characters/faeries/animations/faeries_celeste_walk.bdae | 0e678c6cc573c72b0aa3 | ORPHAN | [details](assets/0e678c6cc573c72b0aa3.md) |
| sounds/sfx_moth_hurt_2.wav | 0ea085063c551f53c987 | ORPHAN | [details](assets/0ea085063c551f53c987.md) |
| 3d/characters/troll/animations/troll_swamp_attack_01.bdae | 0efd9dfa986c347d91f3 | ORPHAN | [details](assets/0efd9dfa986c347d91f3.md) |
| m_safe_zone_01_sfx_swamp.wav | 10a7f608d2d51777221f | ORPHAN | [details](assets/10a7f608d2d51777221f.md) |
| 3d/characters/swampking/animations/cs_swampking_death_cutscene_despawn.bdae | 113579012a13a43a5ad7 | ORPHAN | [details](assets/113579012a13a43a5ad7.md) |
| 3d/characters/troll/animations/troll_swamp_post_stunned.bdae | 121657178733fe5110f9 | ORPHAN | [details](assets/121657178733fe5110f9.md) |
| sounds/sfx_cave_troll_hurt_1.wav | 12812f19739640ab220b | ORPHAN | [details](assets/12812f19739640ab220b.md) |
| 3d/characters/troll/animations/troll_swamp_pre_attack_02.bdae | 13d76f12f0cf6869778e | ORPHAN | [details](assets/13d76f12f0cf6869778e.md) |
| file006069.dat | 142d476efb5ea7a5208b | ORPHAN | [details](assets/142d476efb5ea7a5208b.md) |
| 3d/light/swamp.lightset_xml | 156acd3b4f47a6b75b0b | ORPHAN | [details](assets/156acd3b4f47a6b75b0b.md) |
| 3d/characters/troll/animations/troll_swamp_hurt_02.bdae | 1681cd79b6ce71024413 | ORPHAN | [details](assets/1681cd79b6ce71024413.md) |
| 3d/light/swamp_es_1_1.lightset_xml | 17633716271761562097 | ORPHAN | [details](assets/17633716271761562097.md) |
| 3d/characters/faeries/animations/faeries_template_anim.bdae | 178c6c68d9774e8c39b8 | ORPHAN | [details](assets/178c6c68d9774e8c39b8.md) |
| file002400.dat | 1812a197cec76bac5275 | ORPHAN | [details](assets/1812a197cec76bac5275.md) |
| sounds/sfx_serpent_hurt.wav | 182e24a7925d7192abd3 | ORPHAN | [details](assets/182e24a7925d7192abd3.md) |
| 3d/characters/lizardman/animations/big_lizardman_post_stunned.bdae | 18df1469651583377015 | ORPHAN | [details](assets/18df1469651583377015.md) |
| 3d/characters/faeries/animations/faeries_celeste_spell_cast.bdae | 1966428b9a95dc571e28 | ORPHAN | [details](assets/1966428b9a95dc571e28.md) |
| scripts/ai/follower.luac | 19921b1cfbabfd09916c | ORPHAN | [details](assets/19921b1cfbabfd09916c.md) |
| 3d/interface/spell_dh2_hurt_lightning.bdae | 1a5b26e9aed8da1ec2a0 | ORPHAN | [details](assets/1a5b26e9aed8da1ec2a0.md) |
| scripts/skills/swampking_atk_spit.luac | 1b1c3debd5faabe8dcc8 | ORPHAN | [details](assets/1b1c3debd5faabe8dcc8.md) |
| file002716.dat | 1b45e20f160a1dce6d99 | ORPHAN | [details](assets/1b45e20f160a1dce6d99.md) |
| 3d/characters/lizardman/animations/lizardman_dying_01.bdae | 1b63f593a52c0e155d97 | ORPHAN | [details](assets/1b63f593a52c0e155d97.md) |
| 3d/characters/swampking/animations/swampking_dive.bdae | 1ba1d73c241f32b74de9 | ORPHAN | [details](assets/1ba1d73c241f32b74de9.md) |
| file001412.dat | 1eaf26a63ee706e1bfb2 | ORPHAN | [details](assets/1eaf26a63ee706e1bfb2.md) |
| sounds/sfx_cave_troll_attack.wav | 1ec62afb1145b05d317f | ORPHAN | [details](assets/1ec62afb1145b05d317f.md) |
| 3d/characters/troll/animations/troll_swamp_pre_stunned.bdae | 1f23bfa176200df62c52 | ORPHAN | [details](assets/1f23bfa176200df62c52.md) |
| sounds/sfx_moth_attack_2.wav | 1f67429a472b24cab328 | ORPHAN | [details](assets/1f67429a472b24cab328.md) |
| 3d/characters/moth/animations/moth_attack_01.bdae | 1f90b30eb00fb595ba81 | ORPHAN | [details](assets/1f90b30eb00fb595ba81.md) |
| file004264.dat | 1f96e51c93da562545d8 | ORPHAN | [details](assets/1f96e51c93da562545d8.md) |
| file002282.dat | 1fbec131047d1f33a22e | ORPHAN | [details](assets/1fbec131047d1f33a22e.md) |
| 3d/characters/troll/animations/troll_swamp_post_attack_03.bdae | 1ff3ea88d80700b85f07 | ORPHAN | [details](assets/1ff3ea88d80700b85f07.md) |
| 3d/characters/slime/animations/slime_attack_01.bdae | 22a640d0f26b58518ed4 | ORPHAN | [details](assets/22a640d0f26b58518ed4.md) |
| 3d/characters/prince/animations/prince_dying_01.bdae | 22f707e9c4b8bd6a1b24 | ORPHAN | [details](assets/22f707e9c4b8bd6a1b24.md) |
| 3d/characters/lizardman/animations/big_lizardman_despawn_01.bdae | 23224c7d08d892e0ac37 | ORPHAN | [details](assets/23224c7d08d892e0ac37.md) |
| 3d/characters/troll/animations/troll_swamp_idle.bdae | 2359a1389a786c6a77c9 | ORPHAN | [details](assets/2359a1389a786c6a77c9.md) |
| 3d/modules/swamp/mgp/bossroom_ruins_ns_.mgp | 2439fafb7f0ee290287d | ORPHAN | [details](assets/2439fafb7f0ee290287d.md) |
| scripts/skills/swamp_king_attack_2.luac | 24ab7387f0d9dabc70cf | ORPHAN | [details](assets/24ab7387f0d9dabc70cf.md) |
| file001909.dat | 2607e3134447707a67be | ORPHAN | [details](assets/2607e3134447707a67be.md) |
| 3d/characters/lizardman/animations/big_lizardman_dying_01.bdae | 289e07a8f7aa2c2cd099 | ORPHAN | [details](assets/289e07a8f7aa2c2cd099.md) |
| sfx_swoosh_sword_1_22.wav | 29528629ca15069ea2b7 | ORPHAN | [details](assets/29528629ca15069ea2b7.md) |
| 3d/characters/swampking/animations/swampking_attack_03.bdae | 29a2dfd4e13099df3fce | ORPHAN | [details](assets/29a2dfd4e13099df3fce.md) |
| 3d/modules/swamp/mvp/obj_2of4_brdwalk_sw_00.mvp | 29e7722a60236e8fde75 | ORPHAN | [details](assets/29e7722a60236e8fde75.md) |
| file003743.dat | 2abefc705008553cd6c6 | ORPHAN | [details](assets/2abefc705008553cd6c6.md) |
| 3d/characters/troll/animations/troll_swamp_great_pushback_despawn.bdae | 2b6cd35f1aee193d4720 | ORPHAN | [details](assets/2b6cd35f1aee193d4720.md) |
| 3d/light/swampcave_es1_1.lightset_xml | 2c02c61bd721d8b7f0bc | ORPHAN | [details](assets/2c02c61bd721d8b7f0bc.md) |
| file000232.dat | 2cc1f3240644d3037689 | ORPHAN | [details](assets/2cc1f3240644d3037689.md) |
| file004144.dat | 2d80e9ab78e9c3f17cf8 | ORPHAN | [details](assets/2d80e9ab78e9c3f17cf8.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_02_moving.bdae | 2d908c89cd2065be994e | ORPHAN | [details](assets/2d908c89cd2065be994e.md) |
| file001104.dat | 2d97d8f0bd0b0755bc13 | ORPHAN | [details](assets/2d97d8f0bd0b0755bc13.md) |
| 3d/characters/slime/animations/slime_pre_stunned.bdae | 2dc64d9f739004098220 | ORPHAN | [details](assets/2dc64d9f739004098220.md) |
| 3d/modules/swamp/mvp/corner_ruin_ws_00.mvp | 2e44382f0bb9dd158802 | ORPHAN | [details](assets/2e44382f0bb9dd158802.md) |
| scripts/ai/rene.luac | 2fbb332d04092f3fe22e | ORPHAN | [details](assets/2fbb332d04092f3fe22e.md) |
| 3d/characters/lizardman/animations/lizardman_great_pushback_despawn.bdae | 30cd50ae97fae9a644b5 | ORPHAN | [details](assets/30cd50ae97fae9a644b5.md) |
| 3d/characters/prince/animations/prince_1hand_combo_01.bdae | 30f51edd3c58f864b6e6 | ORPHAN | [details](assets/30f51edd3c58f864b6e6.md) |
| 3d/characters/npcs/head_cultist.bdae | 33d08ad9d8e8ee25dcd4 | ORPHAN | [details](assets/33d08ad9d8e8ee25dcd4.md) |
| sounds/sfx_slime_killed.wav | 371ac12049b615c29bc6 | ORPHAN | [details](assets/371ac12049b615c29bc6.md) |
| 3d/characters/troll/animations/troll_swamp_attack_03.bdae | 380969cff04d5cf6ee5f | ORPHAN | [details](assets/380969cff04d5cf6ee5f.md) |
| 3d/characters/moth/animations/moth_post_stunned.bdae | 38c6d9a65505803c96e2 | ORPHAN | [details](assets/38c6d9a65505803c96e2.md) |
| 3d/characters/witch/animations/witch_hurt.bdae | 390ae3d9cb93fcfd5c6b | ORPHAN | [details](assets/390ae3d9cb93fcfd5c6b.md) |
| sounds/sfx_moth_attack_1.wav | 395ed171c40972b0dcb4 | ORPHAN | [details](assets/395ed171c40972b0dcb4.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_01_moving.bdae | 39c00cf2fb0a0d7ec0ed | ORPHAN | [details](assets/39c00cf2fb0a0d7ec0ed.md) |
| 3d/characters/slime/animations/slime_hurt_01.bdae | 3a4ff7ce04b154abc14c | ORPHAN | [details](assets/3a4ff7ce04b154abc14c.md) |
| 3d/characters/lizardman/lizardman.bdae | 3bd7f740f0a9db59797b | ORPHAN | [details](assets/3bd7f740f0a9db59797b.md) |
| 3d/characters/lizardman/animations/lizardman_pre_stunned.bdae | 3ce61d62e28d4eb45d41 | ORPHAN | [details](assets/3ce61d62e28d4eb45d41.md) |
| 3d/characters/witch/animations/witch_great_pushback.bdae | 3d4047b5cd104204b718 | ORPHAN | [details](assets/3d4047b5cd104204b718.md) |
| 3d/characters/swampking/animations/swampking_hurt_01.bdae | 3d7c447681538f12a3f5 | ORPHAN | [details](assets/3d7c447681538f12a3f5.md) |
| 3d/characters/slime/animations/slime_pre_attack_01.bdae | 3d7f6856ca39c81e03c3 | ORPHAN | [details](assets/3d7f6856ca39c81e03c3.md) |
| 3d/characters/moth/animations/moth_great_pushback_despawn.bdae | 3db3259e251a9858d3a8 | ORPHAN | [details](assets/3db3259e251a9858d3a8.md) |
| sounds/sfx_witch_magic_explosion.wav | 40418e424aaeee129552 | ORPHAN | [details](assets/40418e424aaeee129552.md) |
| file001480.dat | 40ddfa57e3e7872660d7 | ORPHAN | [details](assets/40ddfa57e3e7872660d7.md) |
| 3d/characters/prince/animations/prince_1hand_combo_02.bdae | 4272f3fbd92520558594 | ORPHAN | [details](assets/4272f3fbd92520558594.md) |
| 3d/characters/prince/animations/prince_stunned.bdae | 444721ae1b4f1f410751 | ORPHAN | [details](assets/444721ae1b4f1f410751.md) |
| file004099.dat | 445e23a525acd3d1bced | ORPHAN | [details](assets/445e23a525acd3d1bced.md) |
| sounds/sfx_serpent_spit_water.wav | 44c9d544caf157a46862 | ORPHAN | [details](assets/44c9d544caf157a46862.md) |
| 3d/fx/bloodsplat.bdae | 45d6384ef2052d8258cd | ORPHAN | [details](assets/45d6384ef2052d8258cd.md) |
| file000941.dat | 48d9edd15b5ff1a850e5 | ORPHAN | [details](assets/48d9edd15b5ff1a850e5.md) |
| file001267.dat | 49394bc1a260dfe0c3d1 | ORPHAN | [details](assets/49394bc1a260dfe0c3d1.md) |
| 3d/interface/spell_dh2_hurt_steam.bdae | 49b91f83fce9f3f99be5 | ORPHAN | [details](assets/49b91f83fce9f3f99be5.md) |
| 3d/characters/slime/animations/slime_attack_02_to_idle.bdae | 49bc8afbd85333fb81a3 | ORPHAN | [details](assets/49bc8afbd85333fb81a3.md) |
| 3d/characters/swampking/swampking.bdae | 49e5ceba91d6a98084e0 | ORPHAN | [details](assets/49e5ceba91d6a98084e0.md) |
| sfx_skill_rogue_ass_concealing_cloud.wav | 4a523f5384ec82b3d637 | ORPHAN | [details](assets/4a523f5384ec82b3d637.md) |
| 3d/characters/witch/animations/witch_idle.bdae | 4adf89a3a67eb7929334 | ORPHAN | [details](assets/4adf89a3a67eb7929334.md) |
| file004392.dat | 4b249a2c76940b19cf35 | ORPHAN | [details](assets/4b249a2c76940b19cf35.md) |
| file001747.dat | 4ce91551479ea9130057 | ORPHAN | [details](assets/4ce91551479ea9130057.md) |
| sfx_swoosh_sword_2_22.wav | 4f1d3f8cf46f3f869bc3 | ORPHAN | [details](assets/4f1d3f8cf46f3f869bc3.md) |
| sfx_npc_priest_hurt_2_22.wav | 4fcc3581ac2c20509a6a | ORPHAN | [details](assets/4fcc3581ac2c20509a6a.md) |
| sounds/sfx_serpent_intimidate_2.wav | 51c1c1612d64cc3f1a6c | ORPHAN | [details](assets/51c1c1612d64cc3f1a6c.md) |
| 3d/characters/npcs/priest_good.bdae | 51cbb6394b39f62c502c | ORPHAN | [details](assets/51cbb6394b39f62c502c.md) |
| file005035.dat | 51fdb038ca6a1c08b883 | ORPHAN | [details](assets/51fdb038ca6a1c08b883.md) |
| 3d/interface/swoosh_prince_1hand_combo_01.bdae | 528b769d3c39254976b3 | ORPHAN | [details](assets/528b769d3c39254976b3.md) |
| sounds/sfx_cave_troll_death.wav | 552ce01e2cd96bc01674 | ORPHAN | [details](assets/552ce01e2cd96bc01674.md) |
| 3d/characters/troll/animations/troll_swamp_despawn.bdae | 56b8bb0dc816303e9ab0 | ORPHAN | [details](assets/56b8bb0dc816303e9ab0.md) |
| file005530.dat | 571e001396e04246a308 | ORPHAN | [details](assets/571e001396e04246a308.md) |
| file005793.dat | 575437701bb20a32bbba | ORPHAN | [details](assets/575437701bb20a32bbba.md) |
| 3d/characters/prince/animations/prince_post_stunned.bdae | 58b660d7b57d290d104f | ORPHAN | [details](assets/58b660d7b57d290d104f.md) |
| 3d/characters/prince/animations/prince_idle_shield_02.bdae | 58eb6d473413ed17e95e | ORPHAN | [details](assets/58eb6d473413ed17e95e.md) |
| file001533.dat | 5905717b48448d90bb98 | ORPHAN | [details](assets/5905717b48448d90bb98.md) |
| 3d/characters/lizardman/animations/big_lizardman_attack_01.bdae | 593299c3d43cf6361a40 | ORPHAN | [details](assets/593299c3d43cf6361a40.md) |
| 3d/characters/witch/witch.bdae | 5a17f0ecfed54afe8ebf | ORPHAN | [details](assets/5a17f0ecfed54afe8ebf.md) |
| sounds/sfx_slime_spawn.wav | 5a5016724bcc7a1ec106 | ORPHAN | [details](assets/5a5016724bcc7a1ec106.md) |
| 3d/characters/npcs/animations/npc_m_waiting.bdae | 5ac57748df5a39da600d | ORPHAN | [details](assets/5ac57748df5a39da600d.md) |
| 3d/characters/prince/animations/prince_walk_slow.bdae | 5c3aae8395bd920f526a | ORPHAN | [details](assets/5c3aae8395bd920f526a.md) |
| 3d/characters/prince/animations/prince_idle_shield.bdae | 5cf90dd273ef9ac8370a | ORPHAN | [details](assets/5cf90dd273ef9ac8370a.md) |
| file000441.dat | 5d239d5338a5d3f660e3 | ORPHAN | [details](assets/5d239d5338a5d3f660e3.md) |
| sounds/sfx_witch_hurt_2.wav | 5d79b93c723ae0ef6913 | ORPHAN | [details](assets/5d79b93c723ae0ef6913.md) |
| sounds/sfx_serpent_submerge.wav | 5e8a8b88b71800e62fbf | ORPHAN | [details](assets/5e8a8b88b71800e62fbf.md) |
| 3d/characters/lizardman/animations/big_lizardman_post_attack_01.bdae | 60695a7f2bafb0e5ddc0 | ORPHAN | [details](assets/60695a7f2bafb0e5ddc0.md) |
| 3d/characters/npcs/animations/npc_m_idle_standing.bdae | 61668df0fa9329cffec4 | ORPHAN | [details](assets/61668df0fa9329cffec4.md) |
| file004360.dat | 63a503469a5c5abaee87 | ORPHAN | [details](assets/63a503469a5c5abaee87.md) |
| file003037.dat | 640db4543185ec21352f | ORPHAN | [details](assets/640db4543185ec21352f.md) |
| file005452.dat | 64e77a682f57654f0579 | ORPHAN | [details](assets/64e77a682f57654f0579.md) |
| 3d/characters/lizardman/animations/lizardman_hurt_01.bdae | 660c75120dfd3c615fc3 | ORPHAN | [details](assets/660c75120dfd3c615fc3.md) |
| file001911.dat | 6c77684facbc6122dc14 | ORPHAN | [details](assets/6c77684facbc6122dc14.md) |
| 3d/characters/prince/animations/prince_standup.bdae | 6dead83a06a8a4f0c18c | ORPHAN | [details](assets/6dead83a06a8a4f0c18c.md) |
| 3d/characters/swampking/animations/swampking_swim_big.bdae | 6e438a9ce4a749dd051a | ORPHAN | [details](assets/6e438a9ce4a749dd051a.md) |
| 3d/interface/swoosh_prince_1hand_combo_03.bdae | 6ef63481382913ac92ba | ORPHAN | [details](assets/6ef63481382913ac92ba.md) |
| file001851.dat | 6f138be191445f485ab3 | ORPHAN | [details](assets/6f138be191445f485ab3.md) |
| sounds/sfx_serpent_spawn.wav | 709d73c5f4bc0c4cd19c | ORPHAN | [details](assets/709d73c5f4bc0c4cd19c.md) |
| file005239.dat | 720bbd0fc724b76928b7 | ORPHAN | [details](assets/720bbd0fc724b76928b7.md) |
| 3d/characters/troll/animations/troll_swamp_post_attack_02.bdae | 725992836c7ab4ebef33 | ORPHAN | [details](assets/725992836c7ab4ebef33.md) |
| file005633.dat | 72f8a11ae7dd39339e8f | ORPHAN | [details](assets/72f8a11ae7dd39339e8f.md) |
| 3d/characters/troll/animations/troll_swamp_pushback.bdae | 73bf790aabb9db245e04 | ORPHAN | [details](assets/73bf790aabb9db245e04.md) |
| 3d/characters/lizardman/animations/lizardman_pushback.bdae | 74d997644c56fde2f624 | ORPHAN | [details](assets/74d997644c56fde2f624.md) |
| 3d/characters/slime/animations/slime_pre_attack_02.bdae | 754a3c54c4c9a52a2833 | ORPHAN | [details](assets/754a3c54c4c9a52a2833.md) |
| 3d/projectiles/elemental_bolt_fire.bdae | 75eb749e6ecd2fc0f475 | ORPHAN | [details](assets/75eb749e6ecd2fc0f475.md) |
| 3d/characters/lizardman/animations/lizardman_great_pushback.bdae | 76ad94498523cf71f81c | ORPHAN | [details](assets/76ad94498523cf71f81c.md) |
| sounds/sfx_bandit_attack.wav | 76d1cba0f0b6a01714a9 | ORPHAN | [details](assets/76d1cba0f0b6a01714a9.md) |
| 3d/modules/swamp/mgp/corner_brdwalk_se_00.mgp | 775cf46f5ef4043f72ba | ORPHAN | [details](assets/775cf46f5ef4043f72ba.md) |
| 3d/characters/invisible_man/invisible_man.bdae | 776c8aa134973124d681 | ORPHAN | [details](assets/776c8aa134973124d681.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_02.bdae | 780f5b393dab705ea937 | ORPHAN | [details](assets/780f5b393dab705ea937.md) |
| 3d/characters/slime/animations/slime_idle_02.bdae | 7a242c05f3fcfacbda6b | ORPHAN | [details](assets/7a242c05f3fcfacbda6b.md) |
| 3d/modules/swamp/mgp/obj_2of4_brdwalk_sw_00.mgp | 7a652108fc6b33157fc8 | ORPHAN | [details](assets/7a652108fc6b33157fc8.md) |
| file001276.dat | 7b2f094286c6f1e105d1 | ORPHAN | [details](assets/7b2f094286c6f1e105d1.md) |
| 3d/interface/skill_dh2_monster_spawn.bdae | 7bd88c97508f9ffe9193 | ORPHAN | [details](assets/7bd88c97508f9ffe9193.md) |
| 3d/characters/lizardman/animations/lizardman_despawn_01.bdae | 7c447457fcf38119959b | ORPHAN | [details](assets/7c447457fcf38119959b.md) |
| 3d/characters/witch/animations/witch_despawn.bdae | 7c8023e3eb990ab32a3f | ORPHAN | [details](assets/7c8023e3eb990ab32a3f.md) |
| file000656.dat | 7d0d87a0489a6a8014ac | ORPHAN | [details](assets/7d0d87a0489a6a8014ac.md) |
| 3d/characters/lizardman/animations/big_lizardman_hurt_01.bdae | 7d12a4492f0c6b3959e2 | ORPHAN | [details](assets/7d12a4492f0c6b3959e2.md) |
| 3d/characters/witch/animations/witch_template_anim.bdae | 7e0c5bd07122f9186d40 | ORPHAN | [details](assets/7e0c5bd07122f9186d40.md) |
| 3d/characters/lizardman/animations/big_lizardman_pre_attack_02.bdae | 7e0e21e0a2d4b9e2cb7d | ORPHAN | [details](assets/7e0e21e0a2d4b9e2cb7d.md) |
| 3d/fx/bloodsplat_hero.bdae | 7e3c52b52f92471e8a7c | ORPHAN | [details](assets/7e3c52b52f92471e8a7c.md) |
| 3d/characters/slime/animations/slime_post_stunned.bdae | 7e7e329b1b1e11582407 | ORPHAN | [details](assets/7e7e329b1b1e11582407.md) |
| file004197.dat | 7f193be689d9f012d5b4 | ORPHAN | [details](assets/7f193be689d9f012d5b4.md) |
| 3d/modules/swamp/mvp/merchantcamp_ruins_swe_00.mvp | 7f2a56d27c4e334ccd22 | ORPHAN | [details](assets/7f2a56d27c4e334ccd22.md) |
| 3d/characters/prince/animations/prince_1hand_combo_01_moving.bdae | 7f5ba7bb6c3cc1dba7da | ORPHAN | [details](assets/7f5ba7bb6c3cc1dba7da.md) |
| 3d/characters/slime/animations/slime_attack_02.bdae | 80d76c8ac6fe853774e0 | ORPHAN | [details](assets/80d76c8ac6fe853774e0.md) |
| 3d/characters/swampking/animations/swampking_emerge.bdae | 80d8cc927cfc26d95efe | ORPHAN | [details](assets/80d8cc927cfc26d95efe.md) |
| file003443.dat | 812e7670eb8de71fbece | ORPHAN | [details](assets/812e7670eb8de71fbece.md) |
| file000889.dat | 842041f2a46d8cf4eb2c | ORPHAN | [details](assets/842041f2a46d8cf4eb2c.md) |
| file000649.dat | 84b03f153a5c5471f957 | ORPHAN | [details](assets/84b03f153a5c5471f957.md) |
| 3d/characters/swampking/animations/swampking_post_attack_01.bdae | 84c009094f20bade47b6 | ORPHAN | [details](assets/84c009094f20bade47b6.md) |
| 3d/characters/troll/animations/troll_swamp_water_spawn.bdae | 84e7a5ed20eb13ed4747 | ORPHAN | [details](assets/84e7a5ed20eb13ed4747.md) |
| 3d/light/swampcave.lightset_xml | 85ce1b02a2ee441bfffd | ORPHAN | [details](assets/85ce1b02a2ee441bfffd.md) |
| 3d/characters/moth/animations/moth_dying_01.bdae | 863ff54bd6d6cd31457b | ORPHAN | [details](assets/863ff54bd6d6cd31457b.md) |
| 3d/modules/swamp/mgp/deadend_brdwalk_w_00.mgp | 86a0342708c319ce58b4 | ORPHAN | [details](assets/86a0342708c319ce58b4.md) |
| file000984.dat | 890709df3ec87bb657cd | ORPHAN | [details](assets/890709df3ec87bb657cd.md) |
| sounds/sfx_witch_hurt_1.wav | 890970b7e410a2e3b38d | ORPHAN | [details](assets/890970b7e410a2e3b38d.md) |
| file004483.dat | 893d0f6bafd863bfe775 | ORPHAN | [details](assets/893d0f6bafd863bfe775.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_01.bdae | 8ac719dcc31a99b4c960 | ORPHAN | [details](assets/8ac719dcc31a99b4c960.md) |
| scripts/skills/swamp_king_attack_4.luac | 8b1bb575f95b9b800d82 | ORPHAN | [details](assets/8b1bb575f95b9b800d82.md) |
| file003646.dat | 8bab32a5d838672bf26a | ORPHAN | [details](assets/8bab32a5d838672bf26a.md) |
| sounds/sfx_serpent_intimidate_3.wav | 8c0bbf905eb6430903e3 | ORPHAN | [details](assets/8c0bbf905eb6430903e3.md) |
| 3d/characters/prince/animations/prince_hurt_01.bdae | 8c922b02e66e80603f0c | ORPHAN | [details](assets/8c922b02e66e80603f0c.md) |
| sounds/sfx_witch_attack.wav | 8d764229cb2134e830cb | ORPHAN | [details](assets/8d764229cb2134e830cb.md) |
| 3d/projectiles/projectile_dh2_bogwitch.bdae | 8ee593ac95689a44ab1f | ORPHAN | [details](assets/8ee593ac95689a44ab1f.md) |
| 3d/characters/prince/animations/prince_1hand_combo_03_moving.bdae | 8f1d0186c820d603179d | ORPHAN | [details](assets/8f1d0186c820d603179d.md) |
| 3d/characters/moth/moth.bdae | 9005adb36b8ed8d556ee | ORPHAN | [details](assets/9005adb36b8ed8d556ee.md) |
| 3d/characters/swampking/animations/swampking_idle.bdae | 9070c76ce6835865afb9 | ORPHAN | [details](assets/9070c76ce6835865afb9.md) |
| 3d/characters/troll/animations/troll_swamp_template_anim.bdae | 90fd3c16b15964ef11b7 | ORPHAN | [details](assets/90fd3c16b15964ef11b7.md) |
| file002691.dat | 91824150b7be186627b8 | ORPHAN | [details](assets/91824150b7be186627b8.md) |
| 3d/characters/moth/animations/moth_post_attack_01.bdae | 9288d8710d74325f0a19 | ORPHAN | [details](assets/9288d8710d74325f0a19.md) |
| 3d/characters/moth/animations/moth_great_pushback.bdae | 9439d2145a460dfab556 | ORPHAN | [details](assets/9439d2145a460dfab556.md) |
| 3d/characters/moth/animations/moth_idle_01.bdae | 96caf804e09ea44c74bd | ORPHAN | [details](assets/96caf804e09ea44c74bd.md) |
| 3d/characters/swampking/animations/swampking_attack_01.bdae | 96d69c58fe5749e5cae2 | ORPHAN | [details](assets/96d69c58fe5749e5cae2.md) |
| 3d/characters/moth/animations/moth_pre_attack_01.bdae | 9731018b6e541baa62ed | ORPHAN | [details](assets/9731018b6e541baa62ed.md) |
| 3d/characters/moth/animations/moth_despawn_01.bdae | 974ffdbc7696ff9d4dc6 | ORPHAN | [details](assets/974ffdbc7696ff9d4dc6.md) |
| 3d/modules/swamp/mvp/obj_4of4_brdwalk_sw_00.mvp | 98911750febac0d828d3 | ORPHAN | [details](assets/98911750febac0d828d3.md) |
| 3d/modules/swamp/mvp/obj_1of4_brdwalk_nse_00.mvp | 996134efbb58bd5370c5 | ORPHAN | [details](assets/996134efbb58bd5370c5.md) |
| 3d/characters/swampking/animations/swampking_template_anim.bdae | 996c65133e8ca944f705 | ORPHAN | [details](assets/996c65133e8ca944f705.md) |
| 3d/interface/spell_dh2_hurt_bogwitch.bdae | 9ad5474a9c07ac633491 | ORPHAN | [details](assets/9ad5474a9c07ac633491.md) |
| sounds/sfx_slime_hurt.wav | 9b0bd01770e39dffd8c9 | ORPHAN | [details](assets/9b0bd01770e39dffd8c9.md) |
| scripts/ai/monster.luac | 9b1caeb2ccf9b9424e45 | ORPHAN | [details](assets/9b1caeb2ccf9b9424e45.md) |
| scripts/skills/swamp_king_attack_1.luac | 9b572b5d71a19cacf3f2 | ORPHAN | [details](assets/9b572b5d71a19cacf3f2.md) |
| 3d/characters/lizardman/animations/lizardman_attack_01.bdae | 9bd3886e9db67c233368 | ORPHAN | [details](assets/9bd3886e9db67c233368.md) |
| 3d/characters/witch/animations/witch_walk.bdae | 9d29c3b341d73b04ab90 | ORPHAN | [details](assets/9d29c3b341d73b04ab90.md) |
| 3d/characters/slime/animations/slime_despawn.bdae | 9d6a1d4757aca6e94a69 | ORPHAN | [details](assets/9d6a1d4757aca6e94a69.md) |
| 3d/characters/slime/animations/slime_spawn.bdae | 9e71b890b449c22e680d | ORPHAN | [details](assets/9e71b890b449c22e680d.md) |
| 3d/characters/moth/animations/moth_spawn.bdae | 9e9987beb6853951853e | ORPHAN | [details](assets/9e9987beb6853951853e.md) |
| 3d/characters/swampking/animations/swampking_post_attack_02.bdae | 9ee89124cdb8f11b50b4 | ORPHAN | [details](assets/9ee89124cdb8f11b50b4.md) |
| sounds/sfx_cave_troll_mace_attack.wav | 9faee3cfedb4a86a4b7e | ORPHAN | [details](assets/9faee3cfedb4a86a4b7e.md) |
| file004834.dat | a0bb7b1ba7ce1f91dbb0 | ORPHAN | [details](assets/a0bb7b1ba7ce1f91dbb0.md) |
| sounds/sfx_bandit_killed.wav | a0e4195d35f6cbbf63ec | ORPHAN | [details](assets/a0e4195d35f6cbbf63ec.md) |
| file003550.dat | a1336fc596a1c8ddf24f | ORPHAN | [details](assets/a1336fc596a1c8ddf24f.md) |
| 3d/characters/swampking/animations/swampking_swim_small.bdae | a1ad1ef4ac1439e4c9af | ORPHAN | [details](assets/a1ad1ef4ac1439e4c9af.md) |
| 3d/characters/witch/animations/witch_attack_01.bdae | a27a9b0676542c9d7c5c | ORPHAN | [details](assets/a27a9b0676542c9d7c5c.md) |
| 3d/gameobjects/itemdrops.bdae | a2acd30044d5739365dd | ORPHAN | [details](assets/a2acd30044d5739365dd.md) |
| 3d/characters/troll/animations/troll_swamp_walk.bdae | a3e0c3d7168d6f76acb6 | ORPHAN | [details](assets/a3e0c3d7168d6f76acb6.md) |
| sounds/sfx_moth_hurt_1.wav | a4797c8df1a4c3e520ae | ORPHAN | [details](assets/a4797c8df1a4c3e520ae.md) |
| 3d/characters/slime/animations/slime_great_pushback_standup.bdae | a483a18ce3da5d3b8808 | ORPHAN | [details](assets/a483a18ce3da5d3b8808.md) |
| 3d/characters/slime/animations/slime_stunned.bdae | a4b744316811bbba7a5e | ORPHAN | [details](assets/a4b744316811bbba7a5e.md) |
| 3d/characters/moth/animations/moth_idle_02.bdae | a4c2357a1bb2a4397076 | ORPHAN | [details](assets/a4c2357a1bb2a4397076.md) |
| 3d/characters/slime/animations/slime_great_pushback_despawn.bdae | a540cc8e3d8a4530f998 | ORPHAN | [details](assets/a540cc8e3d8a4530f998.md) |
| 3d/characters/troll/animations/troll_swamp_attack_02.bdae | a548031c080ec31f218d | ORPHAN | [details](assets/a548031c080ec31f218d.md) |
| sounds/sfx_serpent_intimidate_1.wav | a74bc0491f6f498ab1e2 | ORPHAN | [details](assets/a74bc0491f6f498ab1e2.md) |
| 3d/characters/witch/animations/witch_dying.bdae | a766e3a8950628aeb2dd | ORPHAN | [details](assets/a766e3a8950628aeb2dd.md) |
| 3d/characters/troll/animations/troll_swamp_stunned.bdae | a7c98a04003da6464d48 | ORPHAN | [details](assets/a7c98a04003da6464d48.md) |
| 3d/characters/witch/animations/witch_post_attack_02.bdae | a8a3909dcd1670d2442f | ORPHAN | [details](assets/a8a3909dcd1670d2442f.md) |
| 3d/characters/witch/animations/witch_pre_attack_01.bdae | a8bcb70cd6c362c0edfc | ORPHAN | [details](assets/a8bcb70cd6c362c0edfc.md) |
| 3d/characters/npcs/edward_with_weapon.bdae | a8be5f2c03f6d4ef980f | ORPHAN | [details](assets/a8be5f2c03f6d4ef980f.md) |
| 3d/characters/lizardman/animations/big_lizardman_pre_attack_01.bdae | a953b51d08afc208249f | ORPHAN | [details](assets/a953b51d08afc208249f.md) |
| 3d/characters/lizardman/animations/lizardman_post_stunned.bdae | aaa5b7a51766cfae0c10 | ORPHAN | [details](assets/aaa5b7a51766cfae0c10.md) |
| 3d/characters/lizardman/animations/big_lizardman_spawn_jump.bdae | ab19b5e71ae4da9e59ce | ORPHAN | [details](assets/ab19b5e71ae4da9e59ce.md) |
| 3d/characters/slime/animations/slime_template_anim.bdae | ac4bbef57fe70c914c5e | ORPHAN | [details](assets/ac4bbef57fe70c914c5e.md) |
| 3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp | ac704423a4c072055ef0 | ORPHAN | [details](assets/ac704423a4c072055ef0.md) |
| file002959.dat | acdc642eb11f61db584e | ORPHAN | [details](assets/acdc642eb11f61db584e.md) |
| 3d/characters/prince/animations/prince_great_pushback_dying.bdae | ad12754ada26af8ef929 | ORPHAN | [details](assets/ad12754ada26af8ef929.md) |
| 3d/characters/npcs/animations/prisoner_idle_01.bdae | ad99ec7351938ff36e35 | ORPHAN | [details](assets/ad99ec7351938ff36e35.md) |
| 3d/characters/troll/animations/troll_swamp_great_pushback_standup.bdae | aeb2f06462a24c61e398 | ORPHAN | [details](assets/aeb2f06462a24c61e398.md) |
| 3d/characters/prince/animations/prince_hurt_03.bdae | af6b6b996ce7e855fdf2 | ORPHAN | [details](assets/af6b6b996ce7e855fdf2.md) |
| 3d/characters/witch/animations/witch_attack_02.bdae | b0bdfa8977ef3c3a08b9 | ORPHAN | [details](assets/b0bdfa8977ef3c3a08b9.md) |
| 3d/characters/troll/animations/troll_swamp_pre_attack_03.bdae | b0d8f38c46f30156c0f6 | ORPHAN | [details](assets/b0d8f38c46f30156c0f6.md) |
| 3d/characters/witch/animations/witch_pushback.bdae | b21ca0a0776073900e91 | ORPHAN | [details](assets/b21ca0a0776073900e91.md) |
| file000417.dat | b2ca67ecfbcff2263315 | ORPHAN | [details](assets/b2ca67ecfbcff2263315.md) |
| 3d/characters/troll/troll_swamp.bdae | b2d9b0f5d5008dda2e30 | ORPHAN | [details](assets/b2d9b0f5d5008dda2e30.md) |
| file004789.dat | b313ea7f97440192784b | ORPHAN | [details](assets/b313ea7f97440192784b.md) |
| 3d/characters/moth/animations/moth_walk_01.bdae | b34d7616910ffaeb1346 | ORPHAN | [details](assets/b34d7616910ffaeb1346.md) |
| 3d/characters/swampking/animations/swampking_pre_attack_01.bdae | b3f3fb02a5d23db55950 | ORPHAN | [details](assets/b3f3fb02a5d23db55950.md) |
| sounds/sfx_lizardman_attack_1.wav | b464d8da276e88642300 | ORPHAN | [details](assets/b464d8da276e88642300.md) |
| 3d/characters/lizardman/animations/big_lizardman_pushback.bdae | b54626656f04e2959e76 | ORPHAN | [details](assets/b54626656f04e2959e76.md) |
| file004753.dat | b5648730bb3b338e2d75 | ORPHAN | [details](assets/b5648730bb3b338e2d75.md) |
| file003495.dat | b61dc501a3a418598f8a | ORPHAN | [details](assets/b61dc501a3a418598f8a.md) |
| 3d/characters/prince/animations/prince_1hand_pre_combo_03_moving.bdae | b6378533362abfcd5828 | ORPHAN | [details](assets/b6378533362abfcd5828.md) |
| 3d/interface/spell_dh2_hurt_fire.bdae | b67905bc599dd0e6502a | ORPHAN | [details](assets/b67905bc599dd0e6502a.md) |
| 3d/characters/troll/animations/troll_swamp_hurt_01.bdae | b68ea25eef16296b556b | ORPHAN | [details](assets/b68ea25eef16296b556b.md) |
| 3d/characters/swampking/animations/swampking_attack_02.bdae | b6c71a28fdf145911f6d | ORPHAN | [details](assets/b6c71a28fdf145911f6d.md) |
| 3d/characters/faeries/faeries_02_celeste.bdae | b7dd84865e6930587069 | ORPHAN | [details](assets/b7dd84865e6930587069.md) |
| 3d/characters/lizardman/animations/lizardman_walk_01.bdae | b8a4bd91bfd3f01912e4 | ORPHAN | [details](assets/b8a4bd91bfd3f01912e4.md) |
| 3d/characters/prince/animations/prince_1hand_combo_02_to_idle.bdae | b90435446904c3bbcc58 | ORPHAN | [details](assets/b90435446904c3bbcc58.md) |
| 3d/characters/lizardman/animations/big_lizardman_walk_01.bdae | ba85e7aa0df93fbed781 | ORPHAN | [details](assets/ba85e7aa0df93fbed781.md) |
| sfx_npc_priest_hurt_3_22.wav | bbe2f9f70db6061bea9a | ORPHAN | [details](assets/bbe2f9f70db6061bea9a.md) |
| 3d/characters/lizardman/animations/lizardman_great_pushback_dying.bdae | bc212f1bf42cc97e4dfb | ORPHAN | [details](assets/bc212f1bf42cc97e4dfb.md) |
| file000998.dat | bc5dd8af0d0678614dc9 | ORPHAN | [details](assets/bc5dd8af0d0678614dc9.md) |
| 3d/modules/swamp/mgp/corner_ruin_ws_00.mgp | bc8acddf304772cb3f0c | ORPHAN | [details](assets/bc8acddf304772cb3f0c.md) |
| file004087.dat | bc8ce1eccab54349ecad | ORPHAN | [details](assets/bc8ce1eccab54349ecad.md) |
| sounds/sfx_slime_attack.wav | bcec9f084e0974235754 | ORPHAN | [details](assets/bcec9f084e0974235754.md) |
| 3d/characters/slime/animations/slime_dying.bdae | be4f487843a3d46aaf59 | ORPHAN | [details](assets/be4f487843a3d46aaf59.md) |
| 3d/characters/slime/animations/slime_great_pushback.bdae | bfb3250bd1862cdc40ed | ORPHAN | [details](assets/bfb3250bd1862cdc40ed.md) |
| file001990.dat | bfd518cd8ea4547e4601 | ORPHAN | [details](assets/bfd518cd8ea4547e4601.md) |
| 3d/characters/slime/animations/slime_walk.bdae | c03fef5990cf388249ba | ORPHAN | [details](assets/c03fef5990cf388249ba.md) |
| 3d/modules/swamp/mvp/corner_brdwalk_se_00.mvp | c05bab7fe6f4120116d0 | ORPHAN | [details](assets/c05bab7fe6f4120116d0.md) |
| 3d/characters/prince/animations/prince_pre_stunned.bdae | c0ce33ffa98e46e62dab | ORPHAN | [details](assets/c0ce33ffa98e46e62dab.md) |
| 3d/characters/troll/animations/troll_swamp_post_attack_01.bdae | c125de555d22a3152095 | ORPHAN | [details](assets/c125de555d22a3152095.md) |
| 3d/characters/slime/animations/slime_pushback.bdae | c205ad7f8deec7613c0e | ORPHAN | [details](assets/c205ad7f8deec7613c0e.md) |
| sounds/sfx_serpent_attack.wav | c316ba737ab237ddba99 | ORPHAN | [details](assets/c316ba737ab237ddba99.md) |
| sounds/sfx_witch_death.wav | c32ab1ae40364c0783ab | ORPHAN | [details](assets/c32ab1ae40364c0783ab.md) |
| 3d/characters/moth/animations/moth_hurt_01.bdae | c56aa4413331905fe39e | ORPHAN | [details](assets/c56aa4413331905fe39e.md) |
| file005435.dat | c578b782373820905b8f | ORPHAN | [details](assets/c578b782373820905b8f.md) |
| sounds/sfx_moth_die.wav | c5ad85f4db0d428e3f85 | ORPHAN | [details](assets/c5ad85f4db0d428e3f85.md) |
| 3d/characters/moth/animations/moth_template_anim.bdae | c5fbbdce83ac909de869 | ORPHAN | [details](assets/c5fbbdce83ac909de869.md) |
| 3d/characters/prince/animations/prince_1hand_post_combo_01_moving.bdae | c602fba90fcb7ec73688 | ORPHAN | [details](assets/c602fba90fcb7ec73688.md) |
| sounds/sfx_cave_troll_hurt_2.wav | c7c4c10d121bde8081c8 | ORPHAN | [details](assets/c7c4c10d121bde8081c8.md) |
| 3d/characters/prince/animations/prince_pushback.bdae | c8c50720e5612dd72f36 | ORPHAN | [details](assets/c8c50720e5612dd72f36.md) |
| file001510.dat | c8e533fdf06a94e76492 | ORPHAN | [details](assets/c8e533fdf06a94e76492.md) |
| 3d/modules/swamp/swamp.bdae | c90ca70c1b8f654c7734 | ORPHAN | [details](assets/c90ca70c1b8f654c7734.md) |
| 3d/characters/lizardman/animations/lizardman_stunned.bdae | c9913d7697ebd5497ac6 | ORPHAN | [details](assets/c9913d7697ebd5497ac6.md) |
| 3d/characters/witch/animations/witch_post_attack_01.bdae | ca73c4005de7b5c2c4fc | ORPHAN | [details](assets/ca73c4005de7b5c2c4fc.md) |
| 3d/characters/slime/slime_green_v2.bdae | cbee49e5fcd8910709f5 | ORPHAN | [details](assets/cbee49e5fcd8910709f5.md) |
| 3d/characters/prince/animations/prince_1hand_post_combo_02_moving.bdae | ccb91556684386cce06d | ORPHAN | [details](assets/ccb91556684386cce06d.md) |
| 3d/characters/prince/animations/prince_hurt_02.bdae | cdc4d0bfe5899dacfc54 | ORPHAN | [details](assets/cdc4d0bfe5899dacfc54.md) |
| 3d/characters/lizardman/animations/big_lizardman_idle_01.bdae | ce16991ae49b64b7124b | ORPHAN | [details](assets/ce16991ae49b64b7124b.md) |
| 3d/characters/faeries/animations/faeries_celeste_idle.bdae | d19f2af2ea4e9fa8e5f5 | ORPHAN | [details](assets/d19f2af2ea4e9fa8e5f5.md) |
| 3d/characters/npcs/animations/prisoner_idle_03.bdae | d2a4be87526eecd17038 | ORPHAN | [details](assets/d2a4be87526eecd17038.md) |
| 3d/characters/moth/animations/moth_walk_02.bdae | d2e137fc3c9452892c5f | ORPHAN | [details](assets/d2e137fc3c9452892c5f.md) |
| 3d/characters/witch/animations/witch_great_pushback_dying.bdae | d2faf488b23e797b91ba | ORPHAN | [details](assets/d2faf488b23e797b91ba.md) |
| sounds/sfx_bandit_hurt.wav | d64322964cbde70645e8 | ORPHAN | [details](assets/d64322964cbde70645e8.md) |
| 3d/characters/npcs/animations/npc_m_bored.bdae | d6dd246264b7e8a866b1 | ORPHAN | [details](assets/d6dd246264b7e8a866b1.md) |
| 3d/characters/prince/animations/prince_great_pushback_standup.bdae | d6fe30c6aa3735eea12c | ORPHAN | [details](assets/d6fe30c6aa3735eea12c.md) |
| scripts/skills/witch_ranged_00.luac | d92cdc0b7d50b7b65cfc | ORPHAN | [details](assets/d92cdc0b7d50b7b65cfc.md) |
| 3d/characters/moth/animations/moth_stunned.bdae | d94252b5bec71bf6f22d | ORPHAN | [details](assets/d94252b5bec71bf6f22d.md) |
| 3d/characters/moth/animations/moth_pushback.bdae | d9a60d029a55bab7f555 | ORPHAN | [details](assets/d9a60d029a55bab7f555.md) |
| 3d/characters/moth/animations/moth_pre_stunned.bdae | daaa9d28f430d3c5d541 | ORPHAN | [details](assets/daaa9d28f430d3c5d541.md) |
| file000912.dat | db7aceb2774360d1c2a3 | ORPHAN | [details](assets/db7aceb2774360d1c2a3.md) |
| 3d/characters/troll/animations/troll_swamp_pre_attack_01.bdae | dbd0f70844fa244e785d | ORPHAN | [details](assets/dbd0f70844fa244e785d.md) |
| 3d/characters/swampking/animations/swampking_pre_attack_02.bdae | dc7da675aca210285cf1 | ORPHAN | [details](assets/dc7da675aca210285cf1.md) |
| 3d/characters/npcs/animations/npc_m_lookingaround.bdae | ddb636673d3c96ebcad3 | ORPHAN | [details](assets/ddb636673d3c96ebcad3.md) |
| scripts/ai/swampking_core.luac | de30ef870beeb632a45b | ORPHAN | [details](assets/de30ef870beeb632a45b.md) |
| file002335.dat | de8671bc4c492f371a91 | ORPHAN | [details](assets/de8671bc4c492f371a91.md) |
| sounds/sfx_serpent_spit_water_explosion.wav | e07fd7adcbbf7f8dade8 | ORPHAN | [details](assets/e07fd7adcbbf7f8dade8.md) |
| file005918.dat | e0b2cdcf1a24e5973283 | ORPHAN | [details](assets/e0b2cdcf1a24e5973283.md) |
| file006014.dat | e16fb25700c442b000ff | ORPHAN | [details](assets/e16fb25700c442b000ff.md) |
| 3d/characters/swampking/animations/swampking_post_attack_03.bdae | e27d88dd078fa813a432 | ORPHAN | [details](assets/e27d88dd078fa813a432.md) |
| 3d/skybox/skybox_swamp.bdae | e30a712062e0af569976 | ORPHAN | [details](assets/e30a712062e0af569976.md) |
| 3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp | e33cb4d919201076bed6 | ORPHAN | [details](assets/e33cb4d919201076bed6.md) |
| 3d/characters/npcs/animations/prisoner_idle_02.bdae | e41b14ad440c04daf0da | ORPHAN | [details](assets/e41b14ad440c04daf0da.md) |
| 3d/modules/swamp/mgp/obj_3of4_brdwalk_sw_00.mgp | e559c28e6eadb40508b2 | ORPHAN | [details](assets/e559c28e6eadb40508b2.md) |
| file001413.dat | e5c8032b6ee55a7c8836 | ORPHAN | [details](assets/e5c8032b6ee55a7c8836.md) |
| 3d/characters/moth/animations/moth_great_pushback_standup.bdae | e613cfeb7e6982a37e8b | ORPHAN | [details](assets/e613cfeb7e6982a37e8b.md) |
| 3d/characters/prince/animations/prince_template_anim.bdae | e767e29aeb9e148aac3c | ORPHAN | [details](assets/e767e29aeb9e148aac3c.md) |
| 3d/characters/lizardman/animations/big_lizardman_post_attack_02.bdae | e8007869db252da0a3de | ORPHAN | [details](assets/e8007869db252da0a3de.md) |
| 3d/characters/lizardman/animations/big_lizardman_attack_02.bdae | e801c1c9dd8e645de265 | ORPHAN | [details](assets/e801c1c9dd8e645de265.md) |
| 3d/characters/witch/animations/witch_pre_attack_02.bdae | e91ae4041e352fa746d2 | ORPHAN | [details](assets/e91ae4041e352fa746d2.md) |
| 3d/characters/slime/animations/slime_hurt_02.bdae | ea14ea08f55e1951d8a2 | ORPHAN | [details](assets/ea14ea08f55e1951d8a2.md) |
| 3d/characters/moth/animations/moth_great_pushback_dying.bdae | ea3977a35e6b15bb20f0 | ORPHAN | [details](assets/ea3977a35e6b15bb20f0.md) |
| 3d/characters/lizardman/animations/big_lizardman_stunned.bdae | ea8d80704362765abd2f | ORPHAN | [details](assets/ea8d80704362765abd2f.md) |
| 3d/characters/lizardman/animations/lizardman_idle_01.bdae | eb22a2d08df4f9e3d3a8 | ORPHAN | [details](assets/eb22a2d08df4f9e3d3a8.md) |
| 3d/characters/lizardman/animations/lizardman_great_pushback_standup.bdae | eb9beb8fec53771bc7e3 | ORPHAN | [details](assets/eb9beb8fec53771bc7e3.md) |
| 3d/characters/lizardman/animations/big_lizardman_great_pushback_despawn.bdae | ebc6842b73e598ca4b8a | ORPHAN | [details](assets/ebc6842b73e598ca4b8a.md) |
| 3d/characters/swampking/animations/swampking_pre_attack_03.bdae | ecffe4f3e427270faef1 | ORPHAN | [details](assets/ecffe4f3e427270faef1.md) |
| 3d/characters/prince/animations/prince_walk_1hand.bdae | ee0cdf8b7caf073c8eab | ORPHAN | [details](assets/ee0cdf8b7caf073c8eab.md) |
| file000229.dat | ee6f6fad83e444bb30b4 | ORPHAN | [details](assets/ee6f6fad83e444bb30b4.md) |
| 3d/characters/prince/animations/prince_1hand_combo_01_to_idle.bdae | ee73294e45a9cab86c61 | ORPHAN | [details](assets/ee73294e45a9cab86c61.md) |
| 3d/characters/prince/animations/prince_1hand_combo_02_moving.bdae | eee5dc9e88bc94b45e4e | ORPHAN | [details](assets/eee5dc9e88bc94b45e4e.md) |
| 3d/interface/swoosh_prince_1hand_combo_02.bdae | f0d9aa09edfe4e00e6b6 | ORPHAN | [details](assets/f0d9aa09edfe4e00e6b6.md) |
| 3d/characters/lizardman/lizardman_big.bdae | f155f023cbcab0d26b9d | ORPHAN | [details](assets/f155f023cbcab0d26b9d.md) |
| 3d/characters/prince/animations/prince_great_pushback.bdae | f188b3f83d185c73cbdd | ORPHAN | [details](assets/f188b3f83d185c73cbdd.md) |
| 3d/characters/slime/animations/slime_attack_01_to_idle.bdae | f1f505543cd4db3f864d | ORPHAN | [details](assets/f1f505543cd4db3f864d.md) |
| 3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp | f22793b3ff2cff1c2186 | ORPHAN | [details](assets/f22793b3ff2cff1c2186.md) |
| 3d/characters/npcs/prisoner.bdae | f395725aef4d4d1f8315 | ORPHAN | [details](assets/f395725aef4d4d1f8315.md) |
| file000790.dat | f651fb87ba3536061617 | ORPHAN | [details](assets/f651fb87ba3536061617.md) |
| scripts/ai/witch.luac | f706ca6e29e70c3491c2 | ORPHAN | [details](assets/f706ca6e29e70c3491c2.md) |
| 3d/characters/witch/animations/witch_great_pushback_despawn.bdae | f95adf5cd318efd5bb55 | ORPHAN | [details](assets/f95adf5cd318efd5bb55.md) |
| 3d/characters/slime/animations/slime_great_pushback_dying.bdae | f9a1bfb14f0c79ca4f15 | ORPHAN | [details](assets/f9a1bfb14f0c79ca4f15.md) |
| 3d/characters/troll/animations/troll_swamp_dying.bdae | fa0e4801705d8813a874 | ORPHAN | [details](assets/fa0e4801705d8813a874.md) |
| file000164.dat | faa3efea812cd75a179c | ORPHAN | [details](assets/faa3efea812cd75a179c.md) |
| 3d/modules/swamp/mgp/obj_1of4_brdwalk_nse_00.mgp | fabdd1c61198236a383e | ORPHAN | [details](assets/fabdd1c61198236a383e.md) |
| sounds/sfx_lizardman_die.wav | fae8a4bdb397ddf0ee09 | ORPHAN | [details](assets/fae8a4bdb397ddf0ee09.md) |
| 3d/projectiles/elemental_bolt_lightning.bdae | fb70df094ad0a9c05898 | ORPHAN | [details](assets/fb70df094ad0a9c05898.md) |
| 3d/characters/lizardman/animations/lizardman_pre_attack_01.bdae | fbed3d4e318ff8f1fddc | ORPHAN | [details](assets/fbed3d4e318ff8f1fddc.md) |
| file004763.dat | fd6d6b3573aff98f84de | ORPHAN | [details](assets/fd6d6b3573aff98f84de.md) |
| 3d/characters/lizardman/animations/lizardman_template_anim.bdae | fddb5cfb6b454c4af561 | ORPHAN | [details](assets/fddb5cfb6b454c4af561.md) |
| 3d/characters/lizardman/animations/big_lizardman_pre_stunned.bdae | fddffcbe16a885f5e0dc | ORPHAN | [details](assets/fddffcbe16a885f5e0dc.md) |
| 3d/projectiles/elemental_bolt_water.bdae | fe63b7413559061dd3d1 | ORPHAN | [details](assets/fe63b7413559061dd3d1.md) |
| 3d/modules/swamp/mvp/bossroom_ruins_ns_.mvp | fe9f9dc3e3d055c5c670 | ORPHAN | [details](assets/fe9f9dc3e3d055c5c670.md) |
| 3d/modules/swamp/mvp/deadend_brdwalk_w_00.mvp | ff89080ed51fd1847eaa | ORPHAN | [details](assets/ff89080ed51fd1847eaa.md) |
