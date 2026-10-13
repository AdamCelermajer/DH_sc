# Act 1

> Act membership is INFERENCE. See `../data/act-membership.json`; owner confirmation is pending.

## Levels

| File | Format | Level script | Music | Scene objects | Procedural lists | Source status |
| --- | --- | --- | --- | --- | --- | --- |
| 001_swamp.mlx | campaign | data\PyData\scripts\001_SWAMP.pyscript | SwampHubAmbientMusic | 9 | 0 | DONE |
| 002_swamp_witchcave.rule.xml | campaign | data\pydata\scripts\002_swamp_cave_witch.pyscript | SwampWitchCaveAmbientMusic | 0 | 5 | DONE |
| 022_swamp2.rule.xml | campaign | data\pydata\scripts\022_swamp2.pyscript | SwampHubAmbientMusic | 0 | 5 | DONE |
| 022_swamp2_trollcave.rule.xml | campaign | data\pydata\scripts\022_SWAMP2_TROLL_CAVE.pyscript | SwampHubAmbientMusic | 0 | 6 | DONE |
| 023_watertemple.rule.xml | campaign |  |  | 0 | 0 | UNRESOLVED |


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


### `022_swamp2.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/022_swamp2.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/swamp |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\022_swamp2.pyscript |
| camera_znear | 900 |
| camera_zfar | 5000 |
| music | SwampHubAmbientMusic |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/swamp.lightset_xml |
| fixed_light_set | data/3D/Light/swamp_es_1_1.lightset_xml |
| prog_vertex_buffer_size | 430776 |
| prog_index_buffer_size | 77538 |
| fix_vertex_buffer_size | 877392 |
| fix_index_buffer_size | 91068 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| start | true | 1 |
| deadends2 | true | 1 |
| deadends3 | true | 1 |
| tmodules | true | 1 |
| path_road | true | 7 |


### `022_swamp2_trollcave.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/022_swamp2_trollcave.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/swamp |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\022_SWAMP2_TROLL_CAVE.pyscript |
| camera_znear | 900 |
| camera_zfar | 5000 |
| music | SwampHubAmbientMusic |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/swampcave.lightset_xml |
| fixed_light_set | data/3D/Light/swampcave_es1_1.lightset_xml |
| prog_vertex_buffer_size | 152488 |
| prog_index_buffer_size | 38416 |
| fix_vertex_buffer_size | 368308 |
| fix_index_buffer_size | 61570 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| start | true | 1 |
| deadends2 | true | 4 |
| deadends3 | true | 1 |
| bossroom | true | 1 |
| straight | true | 1 |
| path_road | true | 7 |


### `023_watertemple.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/023_watertemple.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |


## Actors and mobs

CharacterProperties rows are linked from exact source-script names, MGP `charpropsname`, or explicit B155/B156 encounter evidence. Other MGP Character objects and `Script_SpawnCharacter` arguments remain conditional candidates; command branches and runtime execution are not evaluated.

| Actor rows | Mapped to this act | Status |
| --- | --- | --- |
| 448 | 28 | INFERENCE: serialized spawn-command targets |

Serialized Script_SpawnCharacter authored targets: 41; modular XML Character candidates: 195. Both are conditional candidates, not a static runtime roster.

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
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_00.mgp | _prim_Monster_3_05_286 | Swamp2_LizadMan_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_00.mgp | _prim_NPC | Fallen_cultist_1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_00.mgp | _prim_NPC01_001 | Fallen_cultist_1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_00.mgp | _prim_NPC02 | Fallen_cultist_1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_00.mgp | _prim_Faery | DefaultFairy | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_00.mgp | _prim_Celeste | DummyLightningFaery | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_00.mgp | _prim_NPC_Eddie | WanderingEddie | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_deadend_brdwalk_w_00.mgp | _prim_Monster | Swamp_Moth_Cocoon | None | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_deadend_brdwalk_w_00.mgp | _prim_Monster01_001 | Swamp_Moth_Cocoon | None | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_01.mgp | _prim_Monster_05_173 | Swamp2_Moth_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_01.mgp | _prim_Monster_07_174 | Swamp2_Moth_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_01.mgp | _prim_Monster_05_173_001 | Swamp2_Moth_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_01.mgp | _prim_Monster_07_174_001 | Swamp2_Moth_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_01.mgp | _prim_Monster_08_175_04 | Troll2 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_obj_1of4_brdwalk_nse_00.mgp | _prim_Monster | Swamp2_LizadMan_Type2 | None | 100 | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_obj_1of4_brdwalk_nse_00.mgp | _prim_Monster01 | Swamp2_LizadMan_Type2 | None | 100 | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_obj_1of4_brdwalk_nse_00.mgp | _prim_tmp_commontype1 | None | Swamp2_CommonType1 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_obj_1of4_brdwalk_nse_00.mgp | _prim_tmp_commontype02 | None | Swamp2_CommonType1 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_obj_1of4_brdwalk_nse_00.mgp | _prim_tmp_commontype03 | None | Swamp2_CommonType1 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_obj_1of4_brdwalk_nse_00.mgp | _prim_tmp_commontype04 | None | Swamp2_CommonType1 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_we_00.mgp | _prim_MothTemplate_14 | None | Swamp2_CommonType2 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_we_00.mgp | _prim_MothTemplate_15 | None | Swamp2_CommonType2 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_we_00.mgp | _prim_LizTemplate_15 | None | Swamp2_CommonType1 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_we_00.mgp | _prim_LizTemplate_16 | None | Swamp2_CommonType1 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_we_00.mgp | _prim_LizTemplate_17 | None | Swamp2_CommonType1 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_we_00.mgp | _prim_LizTemplate_18 | None | Swamp2_CommonType1 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_we_00.mgp | _prim_LizTemplate_19 | None | Swamp2_CommonType1 | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_00.mgp | _prim_Monster_05_173 | Swamp2_Moth_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_00.mgp | _prim_Monster_07_174 | Swamp2_Moth_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_00.mgp | _prim_Monster_05_173_001 | Swamp2_Moth_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_00.mgp | _prim_Monster_07_174_001 | Swamp2_Moth_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_00.mgp | _prim_Monster_08_175_04 | Troll2 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_01.mgp | _prim_Monster_3_05_286 | Swamp2_LizadMan_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_02.mgp | _prim_Monster_3_05_286 | Swamp2_LizadMan_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_se_00.mgp | _prim_Monster_04_299 | Swamp2_LizadMan_Type2 | None | 100 | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_se_00.mgp | _prim_Monster_3_06_301 | Swamp2_LizadMan_Type1 | None | 50 | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_se_00.mgp | _prim_Monster_08_303 | Swamp2_LizadMan_Type2 | None | 80 | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_se_00.mgp | _prim_Monster_09_304 | Swamp2_LizadMan_Type1 | None | 80 | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_se_00.mgp | _prim_Monster_04_300 | Swamp2_LizadMan_Type2 | None | 100 | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_se_00.mgp | _prim_Monster_09_305 | Swamp2_LizadMan_Type1 | None | 80 | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_obj2of4_brdwalk_sw_00.mgp | _prim_Monster | Swamp_Moth_Cocoon | None | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_obj2of4_brdwalk_sw_00.mgp | _prim_Monster01 | Swamp_Moth_Cocoon | None | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_obj2of4_brdwalk_sw_00.mgp | _prim_Monster02 | Swamp_Moth_Cocoon | None | 100 | Idle | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_brdwalk_ne_00.mgp | _prim_Monster_3_06 | Swamp2_LizadMan_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_brdwalk_ne_00.mgp | _prim_Monster_07_169 | Swamp2_Moth_Type1 | None | None | None | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_brdwalk_ne_00.mgp | _prim_Monster_08 | Swamp2_LizadMan_Type1 | None | None | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_brdwalk_ne_00.mgp | _prim_Monster_09 | Swamp2_LizadMan_Type1 | None | 50 | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_brdwalk_ne_00.mgp | _prim_Monster_10 | Swamp2_LizadMan_Type1 | None | 50 | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_brdwalk_ne_00.mgp | _prim_Monster_11 | Swamp2_LizadMan_Type1 | None | 50 | Limbus | INFERENCE |
| 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_brdwalk_ne_00.mgp | _prim_Monster_12 | Swamp2_LizadMan_Type1 | None | None | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_w_00.mgp | _prim_Monster_05_17 | Swamp2_Slime | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_w_00.mgp | _prim_Monster_07_20 | Swamp2_Slime | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_w_00.mgp | _prim_Faery | DefaultFairy | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_w_00.mgp | _prim_NPC_Eddie | WanderingEddie | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_n_00.mgp | _prim_Monster_3_05 | Swamp2_Slime | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_n_00.mgp | _prim_Monster_04 | Swamp2_Slime | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_n_00.mgp | _prim_Monster_05 | Swamp2_Slime | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_n_00.mgp | _prim_Monster_06 | Swamp2_Slime | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_merchant_n_00.mgp | _prim_epic_merchant | Epic_merchant_notherlands | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_s_00.mgp | _prim_Monster | Swamp2_Slime | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_s_00.mgp | _prim_Monster01_001 | Swamp2_Slime | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_s_00.mgp | _prim_Monster02 | Swamp2_Slime | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_s_00.mgp | _prim_Monster03_001 | Swamp2_Slime | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_t_swpcave_nse_01.mgp | _prim_Monster_3_05_3 | Swamp_Moth_Cocoon_Queen | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_t_swpcave_nse_01.mgp | _prim_Monster_04_4 | Swamp2_Slime_Large | None | None | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_t_swpcave_nse_01.mgp | _prim_Monster_05_5 | Swamp2_Slime_Large | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_t_swpcave_nse_01.mgp | _prim_Monster_06_6 | Swamp2_LizadMan_Type1 | None | None | None | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_t_swpcave_nse_01.mgp | _prim_Monster_04_05 | Swamp2_Slime_Large | None | None | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_we_01.mgp | _prim_MedLizard_04 | Swamp_ActorTroll2 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_we_01.mgp | _prim_MedLizard_05 | Swamp_ActorTroll2 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_we_01.mgp | _prim_tmp_commontype1 | None | Swamp_CommonType3 | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_we_01.mgp | _prim_MedLizard_06 | Swamp2_LizadMan_Type1 | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_we_01.mgp | _prim_MedLizard_07 | Swamp2_LizadMan_Type1 | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_we_00.mgp | _prim_MedLizard_04 | Swamp2_LizadMan_Type2 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_we_00.mgp | _prim_MedLizard_05 | Swamp2_LizadMan_Type2 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_ns_00.mgp | _prim_LizardMan_11 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_ns_00.mgp | _prim_LizardMan_12 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_ns_00.mgp | _prim_LizardMan_13 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_ns_00.mgp | _prim_LizardMan_14 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_straight_swpcave_ns_00.mgp | _prim_LizardMan_15 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_nw_00.mgp | _prim_LizardMan_04 | Swamp2_LizadMan_Type2 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_nw_00.mgp | _prim_LizardMan_06 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_nw_00.mgp | _prim_LizardMan_07 | Swamp2_LizadMan_Type2 | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_nw_00.mgp | _prim_LizardMan_08 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_nw_00.mgp | _prim_LizardMan_09 | Swamp2_LizadMan_Type2 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_nw_00.mgp | _prim_LizardMan_10 | Swamp2_LizadMan_Type1 | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_se_00.mgp | _prim_LizardMan_16 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_se_00.mgp | _prim_LizardMan_17 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_se_00.mgp | _prim_LizardMan_21 | Swamp2_LizadMan_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_se_00.mgp | _prim_LizardMan_22 | Swamp2_LizadMan_Type2 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_00.mgp | _prim_LizardMan_24 | Swamp2_LizadMan_Type2 | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_00.mgp | _prim_LizardMan_25 | Swamp2_LizadMan_Type1_Respawn | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_00.mgp | _prim_LizardMan_26 | Swamp2_LizadMan_Type1_Respawn | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_00.mgp | _prim_LizardMan_27 | Swamp2_LizadMan_Type2 | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_00.mgp | _prim_LizardMan_28 | Swamp2_LizadMan_Type1_Respawn | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_00.mgp | _prim_LizardMan_29 | Swamp2_LizadMan_Type1_Respawn | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_01.mgp | _prim_LizardMan_24 | Swamp2_LizadMan_Type2 | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_01.mgp | _prim_LizardMan_25 | Swamp2_LizadMan_Type1_Respawn | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_01.mgp | _prim_LizardMan_26 | Swamp2_LizadMan_Type1_Respawn | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_01.mgp | _prim_LizardMan_27 | Swamp2_LizadMan_Type2 | None | 100 | Limbus | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_01.mgp | _prim_LizardMan_28 | Swamp2_LizadMan_Type1_Respawn | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_sw_01.mgp | _prim_LizardMan_29 | Swamp2_LizadMan_Type1_Respawn | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_ne_00.mgp | _prim_Moth_06 | Swamp2_Moth_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_ne_00.mgp | _prim_Moth_07 | Swamp2_Moth_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_ne_00.mgp | _prim_Moth_08 | Swamp2_Moth_Type1 | None | 100 | Idle | INFERENCE |
| 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_corner_swpcave_ne_00.mgp | _prim_Moth_09 | Swamp_Moth_Cocoon | None | 100 | Idle | INFERENCE |


### CharacterProperties profiles

This table joins listed actor rows to their original table references. AI script/type values come from the linked AI table row; loot, skills, model and animation remain source-table links. It does not prove the actor spawns in this level.

| Actor | Model | Animation table | AI row | AI facts | Skill tree | Loot | Sounds |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Epic_merchant_swamp | head_cultist [49] | thamos_merchant [79] | Merchant [39] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 7} | -1 | Barrel_Level_01 [9] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Fallen_cultist_1 | friendly_cultist [41] | dead_body [78] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | BalrogAttack (reference TODO; source row UNRESOLVED) |
| NPCmerchantCamp | prisoner [79] | thamos_merchant [79] | Merchant [39] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 7} | -1 | AssassinTreasure [8] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Prisonier | prisoner [79] | Prisonier [51] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Prisonier2 | prisoner [79] | Prisonier2 [52] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Slime | slime_green_v2 [89] | Slime [64] | Monster [40] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Weak_Loot [232] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp2_LizadMan_Type1 | lizardman [60] | LizardMan [34] | LizardMan [30] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | WT_Basic_Loot [329] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp2_LizadMan_Type1_Respawn | lizardman [60] | LizardMan [34] | LizardMan [30] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | WT_Basic_Loot [329] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp2_LizadMan_Type2 | lizardman_big [61] | BigLizardMan [7] | LizadManLarge [29] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | WT_Champion_Loot [331] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp2_Moth_Type1 | moth [66] | Moth [42] | Moth [41] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | WT_Weak_Loot [333] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp2_Slime | slime_green_v2 [89] | Slime [64] | Monster [40] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | WT_Weak_Loot [333] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp2_Slime_Large | slime_green_v2 [89] | Slime [64] | Monster [40] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | WT_Basic_Loot [329] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| SwampKing | swampking [99] | SwampKing [70] | SwampKing [63] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "swampking_core", "SelfFX": -1, "Trophy": -1, "Type": 4} | SwampKingSkills [33] | SwampKing_Loot [229] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_ActorTroll | troll_swamp [108] | Troll [73] | Troll [67] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Champion_Loot [231] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_LizadMan_Type1 | lizardman [60] | LizardMan [34] | LizardMan [30] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Basic_Loot [230] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_LizadMan_Type2 | lizardman_big [61] | BigLizardMan [7] | LizadManLarge [29] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Champion_Loot [231] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_LizadMan_Type3 | lizardman_big [61] | BigLizardMan [7] | LizadManLarge [29] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Champion_Loot [231] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_MerchantCamp_NPC_A | prisoner [79] | Prisonier3 [53] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_MerchantCamp_NPC_B | prisoner [79] | Prisonier3 [53] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_MerchantCamp_NPC_C | prisoner [79] | Prisonier6 [56] | Villager [70] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 6} | -1 | -1 | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_Moth_Cocoon | moth_cocoon [68] | MothCocoon [43] | MothCocoon [42] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "mothcocoon", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | WT_Basic_Loot [329] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_Moth_Cocoon_Queen | moth_cocoon [68] | MothCocoon [43] | QueenCocoon [45] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "mothcocoon", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | WT_Champion_Loot [331] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_MovieMoth | moth [66] | Moth [42] | Moth [41] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Weak_Loot [232] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Swamp_SnackMan | lizardman_big [61] | BigLizardMan [7] | LizadManLarge [29] | {"CombatMusic": -1, "Flags": 1, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Basic_Loot [230] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Troll | troll_swamp [108] | Troll [73] | Troll [67] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | Swamp_Champion_Loot [231] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Troll2 | troll_swamp [108] | Troll [73] | Troll [67] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "monster", "SelfFX": -1, "Trophy": -1, "Type": 4} | -1 | WT_Champion_Loot [331] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| Witch | witch [113] | Witch [76] | Witch [74] | {"CombatMusic": -1, "Flags": 5, "OnAggroSFX": -1, "Script": "witch", "SelfFX": -1, "Trophy": -1, "Type": 4} | WitchSkills [35] | Witch_Loot [338] | BalrogAttack (reference TODO; source row UNRESOLVED) |
| zKill5Lizman_Post | invisible_man [55] | NPC_Priest [47] | InvisibleMan [27] | {"CombatMusic": -1, "Flags": 0, "OnAggroSFX": -1, "Script": "__npc__", "SelfFX": -1, "Trophy": -1, "Type": 9} | -1 | AssassinTreasure [8] | BalrogAttack (reference TODO; source row UNRESOLVED) |


## Bosses

Boss candidates use explicit BOSS/MIDBOSS name markers or named encounter actors. Act 1 encounter rows are linked through B155/B156 handoff evidence; AI script/type is shown in the CharacterProperties profile table. Remaining boss classification and encounter semantics stay TODO.

| Candidate rows | Linked spawn targets | Status |
| --- | --- | --- |
| 26 | 2 | INFERENCE: see campaign references |


## Dialogues

The dialog table is inventoried globally. Serialized `Script_StartDialog` occurrences are listed as command rows; mapping them to dialog table IDs remains TODO.

Dialog table rows: 55; StartDialog commands: 136.

## Serialized level scripts

The configured `.pyscript` names resolve to original serialized `_pyscriptnames.bin` and `_pyscripts.bin` pairs. Atlas decodes script names, command kinds, and raw fields without executing callbacks.

| Scene | Script file | Scripts | Commands | Decode status |
| --- | --- | --- | --- | --- |
| 001_swamp.mlx | data\PyData\scripts\001_SWAMP.pyscript | 55 | 789 | DONE |
| 002_swamp_witchcave.rule.xml | data\pydata\scripts\002_swamp_cave_witch.pyscript | 24 | 183 | DONE |
| 022_swamp2.rule.xml | data\pydata\scripts\022_swamp2.pyscript | 6 | 38 | DONE |
| 022_swamp2_trollcave.rule.xml | data\pydata\scripts\022_SWAMP2_TROLL_CAVE.pyscript | 8 | 45 | DONE |


### Command readiness by native kind

The native factory/reader addresses come from the recovered C1 descriptor map. Port anchors identify current dispatch code only; PARTIAL/UNKNOWN does not establish complete source parity or runtime verification.

| Kind | Command | Count | Factory address | Reader address | Port status | Code anchors |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | Script_ExecScript | 47 | 0x458d40 | 0x455628 | UNKNOWN |  |
| 0 | Script_ExecScript | 8 | 0x458d40 | 0x455628 | UNKNOWN |  |
| 0 | Script_ExecScript | 4 | 0x458d40 | 0x455628 | UNKNOWN |  |
| 0 | Script_ExecScript | 8 | 0x458d40 | 0x455628 | UNKNOWN |  |
| 3 | Script_CONSOLE | 14 | 0x458c68 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:680, port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:66 |
| 3 | Script_CONSOLE | 4 | 0x458c68 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:680, port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:66 |
| 3 | Script_CONSOLE | 2 | 0x458c68 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:680, port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:66 |
| 3 | Script_CONSOLE | 2 | 0x458c68 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:680, port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:66 |
| 5 | Script_PlayCamera | 40 | 0x458bd8 | 0x455628 | UNKNOWN |  |
| 5 | Script_PlayCamera | 5 | 0x458bd8 | 0x455628 | UNKNOWN |  |
| 6 | Script_SetCameraClip | 5 | 0x458b90 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:681 |
| 6 | Script_SetCameraClip | 2 | 0x458b90 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:681 |
| 8 | Script_SetCameraTarget | 35 | 0x458b00 | 0x455628 | UNKNOWN |  |
| 8 | Script_SetCameraTarget | 9 | 0x458b00 | 0x455628 | UNKNOWN |  |
| 8 | Script_SetCameraTarget | 2 | 0x458b00 | 0x455628 | UNKNOWN |  |
| 8 | Script_SetCameraTarget | 2 | 0x458b00 | 0x455628 | UNKNOWN |  |
| 10 | Script_StartDialog | 112 | 0x458a70 | 0x459034 | UNKNOWN |  |
| 10 | Script_StartDialog | 11 | 0x458a70 | 0x459034 | UNKNOWN |  |
| 10 | Script_StartDialog | 3 | 0x458a70 | 0x459034 | UNKNOWN |  |
| 10 | Script_StartDialog | 10 | 0x458a70 | 0x459034 | UNKNOWN |  |
| 12 | Script_WaitDialog | 102 | 0x4589e0 | 0x455628 | UNKNOWN |  |
| 12 | Script_WaitDialog | 9 | 0x4589e0 | 0x455628 | UNKNOWN |  |
| 12 | Script_WaitDialog | 2 | 0x4589e0 | 0x455628 | UNKNOWN |  |
| 12 | Script_WaitDialog | 9 | 0x4589e0 | 0x455628 | UNKNOWN |  |
| 13 | Script_PlaySound | 10 | 0x458998 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:755 |
| 13 | Script_PlaySound | 6 | 0x458998 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:755 |
| 14 | Script_StopSound | 4 | 0x458950 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:732 |
| 15 | Script_PlayLevelMusic | 1 | 0x458908 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:769 |
| 15 | Script_PlayLevelMusic | 1 | 0x458908 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:769 |
| 15 | Script_PlayLevelMusic | 1 | 0x458908 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:769 |
| 15 | Script_PlayLevelMusic | 1 | 0x458908 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:769 |
| 16 | Script_EnterSafeZone | 1 | 0x4588c0 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:65 |
| 17 | Script_LeaveSafeZone | 1 | 0x458878 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:65 |
| 19 | Script_PlayAnimByName | 5 | 0x4587e8 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:692 |
| 20 | Script_PlayEffect | 16 | 0x4587a0 | 0x4598b4 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:702 |
| 20 | Script_PlayEffect | 6 | 0x4587a0 | 0x4598b4 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:702 |
| 20 | Script_PlayEffect | 2 | 0x4587a0 | 0x4598b4 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:702 |
| 21 | Script_StopEffect | 8 | 0x458758 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:726 |
| 21 | Script_StopEffect | 2 | 0x458758 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:726 |
| 24 | Script_LockCharacter | 26 | 0x458678 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 24 | Script_LockCharacter | 4 | 0x458678 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 24 | Script_LockCharacter | 2 | 0x458678 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 24 | Script_LockCharacter | 4 | 0x458678 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 25 | Script_UnlockCharacter | 27 | 0x458630 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 25 | Script_UnlockCharacter | 4 | 0x458630 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 25 | Script_UnlockCharacter | 2 | 0x458630 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 25 | Script_UnlockCharacter | 4 | 0x458630 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:30 |
| 26 | Script_Wait | 42 | 0x4585e0 | 0x455628 | UNKNOWN |  |
| 26 | Script_Wait | 22 | 0x4585e0 | 0x455628 | UNKNOWN |  |
| 26 | Script_Wait | 5 | 0x4585e0 | 0x455628 | UNKNOWN |  |
| 27 | Script_SetFaeryState | 1 | 0x458598 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:49 |
| 27 | Script_SetFaeryState | 1 | 0x458598 | 0x455628 | PARTIAL | port/windows-foundation/original_campaign_world_adapter.cpp:25, port/windows-foundation/original_campaign_world_adapter.cpp:49 |
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
| 41 | Script_LookActor | 3 | 0x45819c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:637 |
| 41 | Script_LookActor | 2 | 0x45819c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:637 |
| 42 | Script_ShowActor | 15 | 0x458154 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:661 |
| 42 | Script_ShowActor | 2 | 0x458154 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:661 |
| 42 | Script_ShowActor | 2 | 0x458154 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:661 |
| 43 | Script_HideActor | 10 | 0x45810c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:662 |
| 43 | Script_HideActor | 1 | 0x45810c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:662 |
| 43 | Script_HideActor | 2 | 0x45810c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:662 |
| 44 | Script_KillActor | 1 | 0x4580c4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:588 |
| 44 | Script_KillActor | 1 | 0x4580c4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:588 |
| 45 | Script_PlayActorAnim | 127 | 0x4595c8 | 0x45a2ec | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:595 |
| 45 | Script_PlayActorAnim | 23 | 0x4595c8 | 0x45a2ec | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:595 |
| 46 | Script_SetActorPosition | 47 | 0x45807c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:619 |
| 46 | Script_SetActorPosition | 15 | 0x45807c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:619 |
| 46 | Script_SetActorPosition | 3 | 0x45807c | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:619 |
| 51 | Script_UnEquipHands | 2 | 0x457f14 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:745 |
| 52 | Script_ReEquipHands | 2 | 0x457ecc | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:746 |
| 54 | Script_OpenDoor | 3 | 0x457e34 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:772 |
| 54 | Script_OpenDoor | 3 | 0x457e34 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:772 |
| 54 | Script_OpenDoor | 1 | 0x457e34 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:772 |
| 54 | Script_OpenDoor | 1 | 0x457e34 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:772 |
| 55 | Script_CloseDoor | 1 | 0x457de4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:773 |
| 55 | Script_CloseDoor | 1 | 0x457de4 | 0x455628 | PARTIAL | port/windows-foundation/features/campaign_host/campaign_host.cpp:773 |
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
| 022_swamp2.rule.xml | CloseDoor1 | Script_CloseDoor | _prim_Door_Swamp2 | 1 |
| 022_swamp2.rule.xml | OpenDoor1 | Script_OpenDoor | _prim_Door_Swamp2 | 1 |
| 022_swamp2.rule.xml | ResumeLevelMusic | Script_PlayLevelMusic |  | 1 |
| 022_swamp2.rule.xml | SwampB_intro | Script_HideActor | _prim_Celeste | 1 |
| 022_swamp2.rule.xml | SwampB_intro | Script_HideActor | _prim_Faery | 1 |
| 022_swamp2.rule.xml | SwampB_intro | Script_LookActor | LocalPlayer; _prim_Celeste | 1 |
| 022_swamp2.rule.xml | SwampB_intro | Script_LookActor | _prim_NPC01_001; _prim_Celeste | 1 |
| 022_swamp2.rule.xml | SwampB_intro | Script_LookActor | _prim_NPC02; LocalPlayer | 1 |
| 022_swamp2.rule.xml | SwampB_intro | Script_SetActorPosition | _prim_WaypointCelesteSpot; _prim_Celeste | 2 |
| 022_swamp2.rule.xml | SwampB_intro | Script_SetActorPosition | _prim_WaypointFaerySpoot; _prim_Faery | 1 |
| 022_swamp2.rule.xml | SwampB_intro | Script_ShowActor | _prim_Celeste | 1 |
| 022_swamp2.rule.xml | SwampB_intro | Script_ShowActor | _prim_Faery | 1 |
| 022_swamp2.rule.xml | SwampB_intro | Script_StartDialog |  | 2 |
| 022_swamp2.rule.xml | SwampB_intro | Script_WaitDialog |  | 2 |
| 022_swamp2.rule.xml | enterLocation_Swamp2 | Script_StartDialog |  | 1 |
| 022_swamp2_trollcave.rule.xml | CloseDoor1 | Script_CloseDoor | _prim_Door_TrollCave | 1 |
| 022_swamp2_trollcave.rule.xml | Epic_hello | Script_LookActor | LocalPlayer; _prim_epic_merchant | 1 |
| 022_swamp2_trollcave.rule.xml | Epic_hello | Script_LookActor | _prim_epic_merchant; LocalPlayer | 1 |
| 022_swamp2_trollcave.rule.xml | Epic_hello | Script_StartDialog |  | 3 |
| 022_swamp2_trollcave.rule.xml | Epic_hello | Script_WaitDialog |  | 3 |
| 022_swamp2_trollcave.rule.xml | OpenDoor1 | Script_OpenDoor | _prim_Door_TrollCave | 1 |
| 022_swamp2_trollcave.rule.xml | ResumeLevelMusic | Script_PlayLevelMusic |  | 1 |
| 022_swamp2_trollcave.rule.xml | TrollCave_complete | Script_StartDialog |  | 3 |
| 022_swamp2_trollcave.rule.xml | TrollCave_complete | Script_WaitDialog |  | 3 |
| 022_swamp2_trollcave.rule.xml | TrollCave_intro | Script_StartDialog |  | 3 |
| 022_swamp2_trollcave.rule.xml | TrollCave_intro | Script_WaitDialog |  | 3 |
| 022_swamp2_trollcave.rule.xml | enterLocation_Swamp2TrollCave | Script_StartDialog |  | 1 |


Raw strings are recorded in `../data/campaign-references.json`. Command labels and recovered field offsets identify spawn, transition, dialogue, and audio command records; runtime branch conditions and dialogue-table IDs remain TODO.
