# Act 3

> Act membership is INFERENCE. See `../data/act-membership.json`; owner confirmation is pending.

## Levels

| File | Format | Level script | Music | Scene objects | Procedural lists | Source status |
| --- | --- | --- | --- | --- | --- | --- |
| 015_red_desert_hub.mlx | campaign | data\PyData\scripts\015_RED_DESERT_HUB.pyscript | RedDesertHubAmbientMusic | 6 | 0 | DONE |
| 016_red_desert_cave_01.rule.xml | campaign | data\pydata\scripts\016_red_desert_cave_01.pyscript | RedDesertCaveOneAmbientMusic | 0 | 5 | DONE |
| 017_red_desert_cave_02.rule.xml | campaign | data\pydata\scripts\017_red_desert_cave_02.pyscript | RedDesertCaveTwoAmbientMusic | 0 | 6 | DONE |
| 018_red_desert_cave_03.rule.xml | campaign | data\PyData\scripts\018_RED_DESERT_CAVE_03.pyscript | RedDesertThreeAmbientMusic | 0 | 6 | DONE |
| 019_light_house_01.rule.xml | campaign | data\pydata\scripts\019_light_house_01.pyscript | LighthouseTempleAmbientMusic | 0 | 6 | DONE |
| 019_light_house_02.rule.xml | campaign |  |  | 0 | 0 | UNRESOLVED |
| 019_light_house_03.rule.xml | campaign | data\pydata\scripts\019_light_house_01.pyscript | LighthouseTempleAmbientMusic | 0 | 12 | DONE |


## Level detail

### `015_red_desert_hub.mlx`

Source: `.local-inputs/assets-extra/android/data/scene/015_red_desert_hub.mlx`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| type | level |
| name | _prim_LevelConfig |
| position | 0.0,0.0,0.0 |
| scale | 1.0,1.0,1.0 |
| debugcolor | 255.0,0.0,255.0 |
| rotation | 0.0,0.0,0.0 |
| gametype | LevelConfig |
| scriptFile | data\PyData\scripts\015_RED_DESERT_HUB.pyscript |
| camera_file |  |
| camera_name |  |
| camera_znear | 900 |
| camera_zfar | 10000 |
| music | RedDesertHubAmbientMusic |
| safezone_music | BloodSandsMusic |
| ambientColor | 0.5,0.5,0.5 |
| fog_start | 4900 |
| fog_end | 6000 |
| fog_color | 1.14,0.36,0.49 |
| _templateName | LevelConfig |
| skybox | data/3D/skybox/skybox_blood.bdae |
| light_set | data/3D/Light/reddeserthub.lightset_xml |
| fixed_light_set | data/3D/Light/reddeserthub_es1_1.lightset_xml |
| prog_vertex_buffer_size | 860288 |
| prog_index_buffer_size | 74028 |
| fix_vertex_buffer_size | 860288 |
| fix_index_buffer_size | 74028 |


Direct scene objects (not character spawns):

| Type | Name | Game type | Script |
| --- | --- | --- | --- |
| Block | _module_01_001 | Module |  |
| Block | _module_02_001 | Module |  |
| Block | _module_03_001 | Module |  |
| Block | _module_05_001 | Module |  |
| Block | _module_04_001 | Module |  |
| Block | _module_06_001 | Module |  |


### `016_red_desert_cave_01.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/016_red_desert_cave_01.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/desert_cave |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\016_red_desert_cave_01.pyscript |
| camera_znear | 900 |
| camera_zfar | 10000 |
| music | RedDesertCaveOneAmbientMusic |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/desertcave.lightset_xml |
| fixed_light_set | data/3D/Light/desertcave_es1_1.lightset_xml |
| prog_vertex_buffer_size | 524400 |
| prog_index_buffer_size | 97200 |
| fix_vertex_buffer_size | 1181880 |
| fix_index_buffer_size | 148224 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| objectives | true | 1 |
| path_bandits | true | 5 |
| path_spiders | true | 5 |
| start | true | 1 |
| end | true | 1 |


### `017_red_desert_cave_02.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/017_red_desert_cave_02.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/desert_cave |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\017_red_desert_cave_02.pyscript |
| camera_znear | 900 |
| camera_zfar | 10000 |
| music | RedDesertCaveTwoAmbientMusic |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/desertcave.lightset_xml |
| fixed_light_set | data/3D/Light/desertcave_es1_1.lightset_xml |
| prog_vertex_buffer_size | 229272 |
| prog_index_buffer_size | 69816 |
| fix_vertex_buffer_size | 910188 |
| fix_index_buffer_size | 122262 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| deadends | true | 4 |
| tmodules | true | 1 |
| path | true | 4 |
| wormbattle | true | 1 |
| start | true | 1 |
| end | true | 1 |


### `018_red_desert_cave_03.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/018_red_desert_cave_03.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/desert_cave |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\PyData\scripts\018_RED_DESERT_CAVE_03.pyscript |
| camera_znear | 900 |
| camera_zfar | 10000 |
| music | RedDesertThreeAmbientMusic |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/desertcave.lightset_xml |
| fixed_light_set | data/3D/Light/desertcave_es1_1.lightset_xml |
| prog_vertex_buffer_size | 59472 |
| prog_index_buffer_size | 24822 |
| fix_vertex_buffer_size | 896184 |
| fix_index_buffer_size | 123270 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| deadends | true | 4 |
| tmodules | true | 1 |
| objectives | true | 2 |
| path | true | 5 |
| start | true | 1 |
| end | true | 1 |


### `019_light_house_01.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/019_light_house_01.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/lighthousse |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\019_light_house_01.pyscript |
| camera_znear | 900 |
| camera_zfar | 1000000 |
| music | LighthouseTempleAmbientMusic |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/lighthousse.lightset_xml |
| fixed_light_set | data/3D/Light/lighthousse_es1_1.lightset_xml |
| prog_vertex_buffer_size | 157680 |
| prog_index_buffer_size | 45312 |
| fix_vertex_buffer_size | 2057420 |
| fix_index_buffer_size | 168034 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| start | true | 1 |
| level1 | true | 1 |
| level3 | true | 1 |
| level4 | true | 3 |
| level5 | true | 2 |
| level6 | true | 1 |


### `019_light_house_02.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/019_light_house_02.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |


### `019_light_house_03.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/019_light_house_03.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/lighthousse |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\019_light_house_01.pyscript |
| camera_znear | 500 |
| camera_zfar | 1000000 |
| music | LighthouseTempleAmbientMusic |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/lighthousse.lightset_xml |
| fixed_light_set | data/3D/Light/lighthousse_es1_1.lightset_xml |
| skybox | data/3D/skybox/skybox_wind.bdae |
| prog_vertex_buffer_size | 19764 |
| prog_index_buffer_size | 1488 |
| fix_vertex_buffer_size | 167652 |
| fix_index_buffer_size | 10230 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| start | true | 1 |
| level1 | true | 1 |
| level2 | true | 1 |
| level3 | true | 1 |
| level4 | true | 3 |
| level5 | true | 3 |
| level6 | true | 1 |
| level7 | true | 1 |
| level8 | true | 1 |
| level9 | true | 3 |
| level10 | true | 2 |
| end | true | 1 |

