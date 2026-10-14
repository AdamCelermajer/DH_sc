# Act 2 inferred source-transition scope

Initial inventory: **39 scene files**, **121 source edges**, **4 TODO frontier rows**. Every row starts TODO.

## Evidence and limits

Visual evidence: this is static source inventory with no direct visual counterpart; no gameplay animation or transition timing is claimed.

Logic evidence: IDA pseudocode-all.c TriggerZoneExitLevel::DeclareProperties 0x0039c8f8 binds levelName; TriggerZoneExitLevel::Update 0x0039c33c gates touch/condition/local-host and exposes the travel prompt. Script_ChangeLevel::Execute is 0x0045cba0.

All source edges record authored targets only. Conditional module alternatives and command branches are retained without claiming execution. Known Act 1 backlinks are identified separately; the remaining destination act memberships are unknown. The closure does not claim that all reachable scenes belong to Act 2.

## Level rows

| Scene | Canonical scene ID | Membership candidate | Existing Atlas bucket | State |
| --- | --- | --- | --- | --- |
| 001_swamp.mlx | 6b2f662bc32e54797884 | act_1_backlink | act_1 | TODO |
| 002_swamp_witchcave.rule.xml | f6ed60ebabd20b89aaf1 | act_1_backlink | act_1 | TODO |
| 003_darkwood.mlx | 2e1304ff2c8ed98caa94 | act_2_inferred_start | act_2 | TODO |
| 004_root_den.rule.xml | bf6ca7beefbed5a32c86 | unknown_act_membership | unplaced | TODO |
| 005_infectedvillage.mlx | 1ee09bcff06fac62bcc3 | unknown_act_membership | unplaced | TODO |
| 006_basement.rule.xml | b392eb001343a839de27 | unknown_act_membership | unplaced | TODO |
| 007_crypt_01.rule.xml | 00f4beec573704987fff | unknown_act_membership | unplaced | TODO |
| 008_gothicus_castle_01.rule.xml | 9e69404bebff0e9a7211 | unknown_act_membership | unplaced | TODO |
| 009_abbey.rule.xml | 00d6b04d855fccf54bfd | unknown_act_membership | unplaced | TODO |
| 009a_abbey.mlx | b5c805e540ccbf63b432 | unknown_act_membership | unplaced | TODO |
| 010_el_lightning.rule.xml | fa629399ded8e23c4fb7 | unknown_act_membership | unplaced | TODO |
| 011_earth_temple_01.rule.xml | dedfd7d9431f2172275e | unknown_act_membership | unplaced | TODO |
| 012_earth_temple_02.rule.xml | 3ccb758ba39eee28aa87 | unknown_act_membership | unplaced | TODO |
| 013_earth_temple_03.rule.xml | 45fe61c5ea04fd4f4dea | unknown_act_membership | unplaced | TODO |
| 014_el_earth.rule.xml | 87474d560b2e8c1f85b5 | unknown_act_membership | unplaced | TODO |
| 015_red_desert_hub.mlx | 145c9b75ecc0bcae66e0 | unknown_act_membership | unplaced | TODO |
| 016_red_desert_cave_01.rule.xml | 5d5bf670c8e428777dbe | unknown_act_membership | unplaced | TODO |
| 017_red_desert_cave_02.rule.xml | 1670cdc190d298717e82 | unknown_act_membership | unplaced | TODO |
| 018_red_desert_cave_03.rule.xml | 7cb216ae991b0f5391f8 | unknown_act_membership | unplaced | TODO |
| 019_light_house_01.rule.xml | b2b7ec3332bd71af7ff0 | unknown_act_membership | unplaced | TODO |
| 019_light_house_02.rule.xml | 247b44fa30e0237397a6 | unknown_act_membership | unplaced | TODO |
| 019_light_house_03.rule.xml | 7939d43105cf511cd73e | unknown_act_membership | unplaced | TODO |
| 020_crypt_02.rule.xml | a763713c29120b2d03f7 | unknown_act_membership | unplaced | TODO |
| 021_gothicus_castle_02.rule.xml | 2adb7adbe4e5653621e3 | unknown_act_membership | unplaced | TODO |
| 022_swamp2.rule.xml | 6ee863765454d115924d | unknown_act_membership | unplaced | TODO |
| 022_swamp2_trollcave.rule.xml | a548290f261f5521bfe7 | unknown_act_membership | unplaced | TODO |
| 023_watertemple.rule.xml | 6add1b5bcc34b6249574 | unknown_act_membership | unplaced | TODO |
| 024_el_water.rule.xml | fc8f217aa6c8f4b41e89 | unknown_act_membership | unplaced | TODO |
| 025_icy_hub.mlx | 4a600b19f18959411fdf | unknown_act_membership | unplaced | TODO |
| 026_icy_cavern_01.rule.xml | 283f20b8e682c7f9b20a | unknown_act_membership | unplaced | TODO |
| 027_icy_cavern_02.rule.xml | 9e6cb2d21b3bb5326d90 | unknown_act_membership | unplaced | TODO |
| 029_wind_temple.rule.xml | 7bac2e2741f6bd3044a0 | unknown_act_membership | unplaced | TODO |
| 030_el_air.rule.xml | 04afe51fc8b013539656 | unknown_act_membership | unplaced | TODO |
| 031_firetemple.mlx | bf01948063f214749992 | unknown_act_membership | unplaced | TODO |
| 032_el_fire.rule.xml | 1803d31bc622e52e4ca3 | unknown_act_membership | unplaced | TODO |
| 036_underworld_hub.mlx | a9e0f80e1d8689ad5593 | unknown_act_membership | unplaced | TODO |
| 037_machine_rooms_01.mlx | 9fc3c1a42c6b469bbf26 | unknown_act_membership | unplaced | TODO |
| 038_dark_temple.mlx | f5c436de2b255f87b02f | unknown_act_membership | unplaced | TODO |
| 039_dark_temple_bossroom_01.mlx | 0995df95dc10822639cb | unknown_act_membership | unplaced | TODO |

## Transition edges

| Source scene | Source kind | Target LevelList name | Resolved scene | Source site | State |
| --- | --- | --- | --- | --- | --- |
| 001_swamp.mlx | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/bossroom_ruins_ns_.mgp | TODO |
| 001_swamp.mlx | trigger_zone_exit_level | SWAMP_CAVE_WITCH_A | 002_swamp_witchcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_brdwalk_w_00.mgp | TODO |
| 001_swamp.mlx | trigger_zone_exit_level | SWAMP_02 | 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp | TODO |
| 002_swamp_witchcave.rule.xml | trigger_zone_exit_level | SWAMP | 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/bossroom_witch_.mgp | TODO |
| 002_swamp_witchcave.rule.xml | trigger_zone_exit_level | SWAMP | 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/deadend_swpcave_s_00.mgp | TODO |
| 003_darkwood.mlx | script_change_level | UNDERWORLD_HUB | 036_underworld_hub.mlx | campaign-references.json:60fdd3e8223d6f4d2cb8 | TODO |
| 003_darkwood.mlx | script_change_level | UNDERWORLD_HUB | 036_underworld_hub.mlx | campaign-references.json:b3c39e41660691941615 | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | SWAMP | 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_01_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | INFECTED_VILLAGE_01 | 005_infectedvillage.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_03_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | GOTHICUS_CRYPT_01 | 007_crypt_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_07_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | GOTHICUS_CRYPT_02 | 020_crypt_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_07_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | EARTH_TEMPLE_01 | 011_earth_temple_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_12_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | UNDERWORLD_HUB | 036_underworld_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_14_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | GOTHICUS_CASTLE_02 | 021_gothicus_castle_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_17_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | ROOT_DEN_01 | 004_root_den.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_19_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_27_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | ICY_HUB | 025_icy_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_28_01.mgp | TODO |
| 003_darkwood.mlx | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/darkwood/mgp/darkwood_29_01.mgp | TODO |
| 004_root_den.rule.xml | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/root_den/mgp/objboss_rootden_w__01.mgp | TODO |
| 004_root_den.rule.xml | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/root_den/mgp/objentry_rootden_e__01.mgp | TODO |
| 005_infectedvillage.mlx | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/infectedvillage/mgp/infected01.mgp | TODO |
| 005_infectedvillage.mlx | trigger_zone_exit_level | SECRET_BASEMENT_01 | 006_basement.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/infectedvillage/mgp/infected01.mgp | TODO |
| 006_basement.rule.xml | trigger_zone_exit_level | INFECTED_VILLAGE_01 | 005_infectedvillage.mlx | .local-inputs/assets-extra/android/data/3d/modules/root_den/mgp/basement_deadend_s_01.mgp | TODO |
| 006_basement.rule.xml | trigger_zone_exit_level | INFECTED_VILLAGE_01 | 005_infectedvillage.mlx | .local-inputs/assets-extra/android/data/3d/modules/root_den/mgp/basement_deadend_w_01.mgp | TODO |
| 007_crypt_01.rule.xml | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt_cemetery_entrance_01.mgp | TODO |
| 007_crypt_01.rule.xml | trigger_zone_exit_level | GOTHICUS_CASTLE_01 | 008_gothicus_castle_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt_entrance_s_01.mgp | TODO |
| 007_crypt_01.rule.xml | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt_kings_chamber_s_01.mgp | TODO |
| 007_crypt_01.rule.xml | trigger_zone_exit_level | GOTHICUS_CASTLE_01 | 008_gothicus_castle_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt_straight_c_ns_01.mgp | TODO |
| 007_crypt_01.rule.xml | trigger_zone_exit_level | GOTHICUS_CASTLE_02 | 021_gothicus_castle_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt_straight_c_ns_01.mgp | TODO |
| 007_crypt_01.rule.xml | trigger_zone_exit_level | GOTHICUS_CASTLE_01 | 008_gothicus_castle_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt_straight_c_ns_02.mgp | TODO |
| 007_crypt_01.rule.xml | trigger_zone_exit_level | GOTHICUS_CASTLE_02 | 021_gothicus_castle_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt_straight_c_ns_02.mgp | TODO |
| 007_crypt_01.rule.xml | trigger_zone_exit_level | GOTHICUS_CASTLE_01 | 008_gothicus_castle_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt_straight_c_ns_03.mgp | TODO |
| 007_crypt_01.rule.xml | trigger_zone_exit_level | GOTHICUS_CASTLE_02 | 021_gothicus_castle_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt_straight_c_ns_03.mgp | TODO |
| 008_gothicus_castle_01.rule.xml | script_change_level | DARKWOOD | 003_darkwood.mlx | campaign-references.json:94d9749814d563ff0ca4 | TODO |
| 009_abbey.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/abbey_deadend_w_01.mgp | TODO |
| 009_abbey.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/abbey_entrance_n_01.mgp | TODO |
| 009a_abbey.mlx | trigger_zone_exit_level | ABBEY_ENTRANCE_01 | 009_abbey.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/abbey_entrance_ns_01.mgp | TODO |
| 009a_abbey.mlx | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/abbey_entrance_ns_01.mgp | TODO |
| 009a_abbey.mlx | trigger_zone_exit_level | ELEMENTAL_AIR | 030_el_air.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/abbey_entrance_ns_01.mgp | TODO |
| 009a_abbey.mlx | trigger_zone_exit_level | ELEMENTAL_EARTH | 014_el_earth.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/abbey_entrance_ns_01.mgp | TODO |
| 009a_abbey.mlx | trigger_zone_exit_level | ELEMENTAL_FIRE | 032_el_fire.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/abbey_entrance_ns_01.mgp | TODO |
| 009a_abbey.mlx | trigger_zone_exit_level | ELEMENTAL_LIGHT | 010_el_lightning.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/abbey_entrance_ns_01.mgp | TODO |
| 009a_abbey.mlx | trigger_zone_exit_level | ELEMENTAL_WATER | 024_el_water.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/abbey_entrance_ns_01.mgp | TODO |
| 010_el_lightning.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/boss_lightning.mgp | TODO |
| 010_el_lightning.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/el_lightning_deadend_n_01.mgp | TODO |
| 011_earth_temple_01.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_02 | 012_earth_temple_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et01_deadend_toet02_e__01.mgp | TODO |
| 011_earth_temple_01.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_02 | 012_earth_temple_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et01_deadend_toet02_n__01.mgp | TODO |
| 011_earth_temple_01.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_02 | 012_earth_temple_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et01_deadend_toet02_s__01.mgp | TODO |
| 011_earth_temple_01.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_02 | 012_earth_temple_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et01_deadend_toet02_w__01.mgp | TODO |
| 011_earth_temple_01.rule.xml | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et01_entry_earthtemp_w__01.mgp | TODO |
| 012_earth_temple_02.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_01 | 011_earth_temple_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et02_deadend_toet01_e__01.mgp | TODO |
| 012_earth_temple_02.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_01 | 011_earth_temple_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et02_deadend_toet01_n__01.mgp | TODO |
| 012_earth_temple_02.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_01 | 011_earth_temple_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et02_deadend_toet01_s__01.mgp | TODO |
| 012_earth_temple_02.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_01 | 011_earth_temple_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et02_deadend_toet01_w__01.mgp | TODO |
| 012_earth_temple_02.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_03 | 013_earth_temple_03.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et02_deadend_toet03_n__01.mgp | TODO |
| 013_earth_temple_03.rule.xml | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et03_objboss_earthtemp_s__01.mgp | TODO |
| 013_earth_temple_03.rule.xml | trigger_zone_exit_level | EARTH_TEMPLE_02 | 012_earth_temple_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/earth_temple/mgp/et03_tmodule_toet02_nsw__01.mgp | TODO |
| 014_el_earth.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/boss_earth.mgp | TODO |
| 014_el_earth.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/el_earth_deadend_n_01.mgp | TODO |
| 015_red_desert_hub.mlx | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/red_desert/mgp/rd01.mgp | TODO |
| 015_red_desert_hub.mlx | trigger_zone_exit_level | FIRE_TEMPLE_01 | 031_firetemple.mlx | .local-inputs/assets-extra/android/data/3d/modules/red_desert/mgp/rd02.mgp | TODO |
| 015_red_desert_hub.mlx | trigger_zone_exit_level | DESERT_CAVE_01 | 016_red_desert_cave_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/red_desert/mgp/rd03.mgp | TODO |
| 015_red_desert_hub.mlx | trigger_zone_exit_level | DESERT_CAVE_02 | 017_red_desert_cave_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/red_desert/mgp/rd03.mgp | TODO |
| 015_red_desert_hub.mlx | trigger_zone_exit_level | DESERT_CAVE_03 | 018_red_desert_cave_03.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/red_desert/mgp/rd04.mgp | TODO |
| 015_red_desert_hub.mlx | trigger_zone_exit_level | LIGHT_HOUSE_01 | 019_light_house_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/red_desert/mgp/rd05.mgp | TODO |
| 016_red_desert_cave_01.rule.xml | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/desert_cave/mgp/rdc01_obj01_dsrtcave_e_00_01.mgp | TODO |
| 016_red_desert_cave_01.rule.xml | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/desert_cave/mgp/rdc01_obj05_dsrtcave_n_00_01.mgp | TODO |
| 017_red_desert_cave_02.rule.xml | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/desert_cave/mgp/rdc02_obj05_dsrtcave_n_00_01.mgp | TODO |
| 017_red_desert_cave_02.rule.xml | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/desert_cave/mgp/rdc02_objentrance_dsrtcave_n_00_01.mgp | TODO |
| 018_red_desert_cave_03.rule.xml | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/desert_cave/mgp/rdc03_objentrance_dsrtcave_n_00_01.mgp | TODO |
| 018_red_desert_cave_03.rule.xml | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/desert_cave/mgp/rdc03_objexit_dsrtcave_s_00_01.mgp | TODO |
| 019_light_house_01.rule.xml | trigger_zone_exit_level | LIGHT_HOUSE_02 | 019_light_house_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/lighthousse/mgp/lh01_lv06_close_02_01.mgp | TODO |
| 019_light_house_01.rule.xml | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/lighthousse/mgp/lh01_startroom_01.mgp | TODO |
| 019_light_house_02.rule.xml | trigger_zone_exit_level | LIGHT_HOUSE_03 | 019_light_house_03.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/lighthousse/mgp/lh01_lv02_close_01_01.mgp | TODO |
| 019_light_house_02.rule.xml | trigger_zone_exit_level | LIGHT_HOUSE_01 | 019_light_house_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/lighthousse/mgp/lh01_lv07_open_02_01.mgp | TODO |
| 019_light_house_03.rule.xml | trigger_zone_exit_level | LIGHT_HOUSE_02 | 019_light_house_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/lighthousse/mgp/lh01_bossroom_01.mgp | TODO |
| 019_light_house_03.rule.xml | trigger_zone_exit_level | LIGHT_HOUSE_03 | 019_light_house_03.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/lighthousse/mgp/lh01_lv02_close_01_01.mgp | TODO |
| 019_light_house_03.rule.xml | trigger_zone_exit_level | LIGHT_HOUSE_02 | 019_light_house_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/lighthousse/mgp/lh01_lv06_close_02_01.mgp | TODO |
| 019_light_house_03.rule.xml | trigger_zone_exit_level | LIGHT_HOUSE_01 | 019_light_house_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/lighthousse/mgp/lh01_lv07_open_02_01.mgp | TODO |
| 019_light_house_03.rule.xml | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/lighthousse/mgp/lh01_startroom_01.mgp | TODO |
| 020_crypt_02.rule.xml | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt2_deadend_w_01.mgp | TODO |
| 020_crypt_02.rule.xml | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/crypt/mgp/crypt2_kingsroom_s_01.mgp | TODO |
| 021_gothicus_castle_02.rule.xml | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/gc02_deadend_n_01.mgp | TODO |
| 021_gothicus_castle_02.rule.xml | trigger_zone_exit_level | GOTHICUS_CRYPT_01 | 007_crypt_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/gc02_kingroom_s_01.mgp | TODO |
| 022_swamp2.rule.xml | trigger_zone_exit_level | SWAMP | 001_swamp.mlx | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_corner_ruin_wn_00.mgp | TODO |
| 022_swamp2.rule.xml | trigger_zone_exit_level | SWAMP_CAVE_TROLL_A | 022_swamp2_trollcave.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_deadend_brdwalk_w_00.mgp | TODO |
| 022_swamp2.rule.xml | trigger_zone_exit_level | WATER_TEMPLE_01 | 023_watertemple.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/swamp2_straight_brdwalk_ns_01.mgp | TODO |
| 022_swamp2_trollcave.rule.xml | trigger_zone_exit_level | SWAMP_02 | 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_merchant_n_00.mgp | TODO |
| 022_swamp2_trollcave.rule.xml | trigger_zone_exit_level | SWAMP_02 | 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_n_00.mgp | TODO |
| 022_swamp2_trollcave.rule.xml | trigger_zone_exit_level | SWAMP_02 | 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/swamp/mgp/trollcave_deadend_swpcave_w_00.mgp | TODO |
| 023_watertemple.rule.xml | trigger_zone_exit_level | SWAMP_02 | 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/water_temple/mgp/watertemple_deadend_objend_s_01.mgp | TODO |
| 023_watertemple.rule.xml | trigger_zone_exit_level | SWAMP_02 | 022_swamp2.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/water_temple/mgp/watertemple_deadend_objstart_w_01.mgp | TODO |
| 024_el_water.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/boss_water.mgp | TODO |
| 024_el_water.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/el_water_deadend_n_01.mgp | TODO |
| 025_icy_hub.mlx | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/icy_hub/mgp/icyhub_01.mgp | TODO |
| 025_icy_hub.mlx | trigger_zone_exit_level | ICY_CAVERN_01 | 026_icy_cavern_01.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/icy_hub/mgp/icyhub_03.mgp | TODO |
| 025_icy_hub.mlx | trigger_zone_exit_level | ICY_CAVERN_02 | 027_icy_cavern_02.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/icy_hub/mgp/icyhub_05.mgp | TODO |
| 025_icy_hub.mlx | trigger_zone_exit_level | WIND_TEMPLE_01 | 029_wind_temple.rule.xml | .local-inputs/assets-extra/android/data/3d/modules/icy_hub/mgp/icyhub_06.mgp | TODO |
| 026_icy_cavern_01.rule.xml | trigger_zone_exit_level | ICY_HUB | 025_icy_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/icy_cavern/mgp/ic01_bossroom_icycavern_w_00_01.mgp | TODO |
| 026_icy_cavern_01.rule.xml | trigger_zone_exit_level | ICY_HUB | 025_icy_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/icy_cavern/mgp/ic01_entrance_icycavern_w_00_01.mgp | TODO |
| 027_icy_cavern_02.rule.xml | trigger_zone_exit_level | ICY_HUB | 025_icy_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/icy_cavern/mgp/ic02_bossroom_icycavern_w_00_01.mgp | TODO |
| 027_icy_cavern_02.rule.xml | trigger_zone_exit_level | ICY_HUB | 025_icy_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/icy_cavern/mgp/ic02_entrance_icycavern_e_00_01.mgp | TODO |
| 029_wind_temple.rule.xml | trigger_zone_exit_level | ICY_HUB | 025_icy_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/wind_temple/mgp/entry_windtemp_n_01.mgp | TODO |
| 029_wind_temple.rule.xml | trigger_zone_exit_level | ICY_HUB | 025_icy_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/wind_temple/mgp/objboss_windtemp_e_01.mgp | TODO |
| 030_el_air.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/boss_air.mgp | TODO |
| 030_el_air.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/el_air_deadend_n_01.mgp | TODO |
| 031_firetemple.mlx | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/firetemple/mgp/firetemple_01.mgp | TODO |
| 031_firetemple.mlx | trigger_zone_exit_level | RED_DESERT_HUB | 015_red_desert_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/firetemple/mgp/firetemple_12.mgp | TODO |
| 032_el_fire.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/boss_fire.mgp | TODO |
| 032_el_fire.rule.xml | trigger_zone_exit_level | ABBEY_RUINS_01 | 009a_abbey.mlx | .local-inputs/assets-extra/android/data/3d/modules/castle/mgp/el_fire_deadend_n.mgp | TODO |
| 036_underworld_hub.mlx | script_change_level | MACHINE_ROOMS_01 | 037_machine_rooms_01.mlx | campaign-references.json:4ab4349b602de01af564 | TODO |
| 036_underworld_hub.mlx | script_change_level | MACHINE_ROOMS_01 | 037_machine_rooms_01.mlx | campaign-references.json:bcd656b5cd03f718952f | TODO |
| 036_underworld_hub.mlx | trigger_zone_exit_level | DARKWOOD | 003_darkwood.mlx | .local-inputs/assets-extra/android/data/3d/modules/underworldhub/mgp/underworld_01.mgp | TODO |
| 036_underworld_hub.mlx | trigger_zone_exit_level | DARK_DUNGEON_TEMPLE_01 | 038_dark_temple.mlx | .local-inputs/assets-extra/android/data/3d/modules/underworldhub/mgp/underworld_08.mgp | TODO |
| 037_machine_rooms_01.mlx | script_change_level | UNDERWORLD_HUB | 036_underworld_hub.mlx | campaign-references.json:70fc213bec399059ad1e | TODO |
| 037_machine_rooms_01.mlx | script_change_level | UNDERWORLD_HUB | 036_underworld_hub.mlx | campaign-references.json:e2ca883cfc0f5b3a8db4 | TODO |
| 037_machine_rooms_01.mlx | trigger_zone_exit_level | UNDERWORLD_HUB | 036_underworld_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/machineroom/mgp/mr01_plane_machineroom_02_01.mgp | TODO |
| 038_dark_temple.mlx | trigger_zone_exit_level | UNDERWORLD_HUB | 036_underworld_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/darktemple/mgp/dt_01.mgp | TODO |
| 038_dark_temple.mlx | trigger_zone_exit_level | DARK_DUNGEON_TEMPLE_BOSSROOM | 039_dark_temple_bossroom_01.mlx | .local-inputs/assets-extra/android/data/3d/modules/darktemple/mgp/dt_08.mgp | TODO |
| 038_dark_temple.mlx | trigger_zone_exit_level | UNDERWORLD_HUB | 036_underworld_hub.mlx | .local-inputs/assets-extra/android/data/3d/modules/darktemple/mgp/dt_08.mgp | TODO |
| 039_dark_temple_bossroom_01.mlx | trigger_zone_exit_level | DARK_DUNGEON_TEMPLE_01 | 038_dark_temple.mlx | .local-inputs/assets-extra/android/data/3d/modules/darktemple/mgp/dt_darktemple_bossroom_01.mgp | TODO |

## Stopping rule

Traverse every statically authored TriggerZoneExitLevel.levelName in direct MGP modules and every procedural <elem gameplay> module candidate, recurse nested Module.mgp includes, and include decoded Script_ChangeLevel targets. Keep branch, conditional, duplicate, and return edges. Stop when no newly resolved scene remains; unresolved modules/targets and other transition mechanisms remain explicit TODO frontier rows.

## Frontier

- **TODO — module_parse_failed**: 019_light_house_03.rule.xml
- **TODO — module_source_missing**: gc02_corner_ws_01.mgp
- **TODO — other_transition_mechanisms_not_closed**: The scope follows authored TriggerZoneExitLevel.levelName values, direct and rule-candidate MGP includes, and serialized Script_ChangeLevel commands. Fast-travel menu membership, every Lua/native route, opaque rule conditions, and cross-platform differences are not yet proven closed.
- **TODO — possible_ios_only_source_candidate**: No exact case-insensitive match was found in the explicit Android, Android original-cache, or extracted iOS data roots. The packed iOS logical-path index has not yet been joined for this filename; membership and any exit declarations remain unknown.
