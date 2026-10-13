# Act 4

> Act membership is INFERENCE. See `../data/act-membership.json`; owner confirmation is pending.

## Levels

| File | Format | Level script | Music | Scene objects | Procedural lists | Source status |
| --- | --- | --- | --- | --- | --- | --- |
| 025_icy_hub.mlx | campaign | data\PyData\scripts\025_ICY_HUB.pyscript | IcyCliffHubAmbientMusic | 8 | 0 | DONE |
| 026_icy_cavern_01.rule.xml | campaign | data\pydata\scripts\026_icy_caverns_01.pyscript | IcyCaveOneTwoAmbientMusic | 0 | 5 | DONE |
| 027_icy_cavern_02.rule.xml | campaign | data\pydata\scripts\027_icy_caverns_02.pyscript | IcyCaveTwoAmbientMusic | 0 | 6 | DONE |
| 028_icy_cavern_03.rule.xml | campaign | data\pydata\scripts\028_icy_caverns_03.pyscript | IcyCaveThreeAmbientMusic | 0 | 5 | DONE |


## Level detail

### `025_icy_hub.mlx`

Source: `.local-inputs/assets-extra/android/data/scene/025_icy_hub.mlx`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| type | Block |
| name | _prim_LevelConfig |
| position | -15209.1,-38521.1,-1829.36 |
| scale | 1.0,1.0,1.0 |
| debugcolor | 255.0,0.0,255.0 |
| rotation | 0.0,0.0,0.0 |
| dimensions | 200.0,200.0,200.0 |
| gametype | LevelConfig |
| camera_file |  |
| camera_name |  |
| camera_znear | 600 |
| camera_zfar | 50000 |
| music | IcyCliffHubAmbientMusic |
| safezone_music | NortherlandMusic |
| ambientColor | 1, 1, 1 |
| fog_start | 4800 |
| fog_end | 6500 |
| fog_color | 0.14, 0.36, 0.41 |
| clear_color | 0.14, 0.36, 0.41 |
| _templateName | LevelConfig |
| light_set_str | :data/3D/Light/default.lightset_xml |
| scriptFile | data\PyData\scripts\025_ICY_HUB.pyscript |
| light_set | data/3D/Light/icyhub.lightset_xml |
| fixed_light_set | data/3D/Light/icyhub_es1_1.lightset_xml |
| prog_vertex_buffer_size | 1053448 |
| prog_index_buffer_size | 100158 |
| fix_vertex_buffer_size | 1053448 |
| fix_index_buffer_size | 100158 |


Direct scene objects (not character spawns):

| Type | Name | Game type | Script |
| --- | --- | --- | --- |
| Block | _module__001 | Module |  |
| Block | _prim_LevelConfig | LevelConfig | data\PyData\scripts\025_ICY_HUB.pyscript |
| Block | _module_01_001 | Module |  |
| Block | _module_02_001 | Module |  |
| Block | _module_03_001 | Module |  |
| Block | _module_04_001 | Module |  |
| Block | _module_05_001 | Module |  |
| Block | _module_06_001 | Module |  |


### `026_icy_cavern_01.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/026_icy_cavern_01.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/icy_cavern |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\026_icy_caverns_01.pyscript |
| camera_znear | 900 |
| camera_zfar | 5000 |
| music | IcyCaveOneTwoAmbientMusic |
| ambiant_music | sfx_ice_cavern_ambiance |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/icycave.lightset_xml |
| fixed_light_set | data/3D/Light/icycave_es1_1.lightset_xml |
| prog_vertex_buffer_size | 92016 |
| prog_index_buffer_size | 24288 |
| fix_vertex_buffer_size | 1803136 |
| fix_index_buffer_size | 207986 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| start | true | 1 |
| deadends | true | 1 |
| tmodules | true | 4 |
| path | true | 6 |
| end | true | 1 |


### `027_icy_cavern_02.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/027_icy_cavern_02.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/icy_cavern |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\027_icy_caverns_02.pyscript |
| camera_znear | 900 |
| camera_zfar | 5000 |
| music | IcyCaveTwoAmbientMusic |
| ambiant_music | sfx_ice_cavern_ambiance |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/icycave.lightset_xml |
| fixed_light_set | data/3D/Light/icycave_es1_1.lightset_xml |
| prog_vertex_buffer_size | 396576 |
| prog_index_buffer_size | 150144 |
| fix_vertex_buffer_size | 1753740 |
| fix_index_buffer_size | 214812 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| deadends | true | 1 |
| tmodules | true | 2 |
| objectives | true | 1 |
| path | true | 6 |
| start | true | 1 |
| end | true | 1 |


### `028_icy_cavern_03.rule.xml`

Source: `.local-inputs/assets-extra/android/data/scene/028_icy_cavern_03.rule.xml`. Parsed level configuration: direct XML evidence.

| Field | Value |
| --- | --- |
| target | iphone |
| folder | data/3d/modules/icy_cavern |
| ambientColor | 0.5,0.5,0.5 |
| scriptFile | data\pydata\scripts\028_icy_caverns_03.pyscript |
| camera_znear | 900 |
| camera_zfar | 5000 |
| music | IcyCaveThreeAmbientMusic |
| ambiant_music | sfx_ice_cavern_ambiance |
| fog_start | 4000 |
| fog_end | 5800 |
| fog_color | 1.14,0.36,0.49 |
| light_set | data/3D/Light/icycave.lightset_xml |
| fixed_light_set | data/3D/Light/icycave_es1_1.lightset_xml |
| prog_vertex_buffer_size | 395184 |
| prog_index_buffer_size | 144978 |


Procedural rule lists (module choices; does not imply loaded actor roster):

| List | Random | Choices |
| --- | --- | --- |
| deadends | true | 1 |
| tmodules | true | 1 |
| path | true | 5 |
| start | true | 1 |
| end | true | 1 |

