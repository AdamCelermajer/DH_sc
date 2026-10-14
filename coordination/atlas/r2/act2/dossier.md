# Act 2 inferred readiness dossier

Static source inventory covers **39 reachable scenes**, **121 authored transition edges**, and **217 unique Character-table rows**. The narrative Act boundary remains INFERENCE.

This dossier is static-source evidence. It does not claim that any scene, actor, boss, quest, sound, or loot path runs in the Windows production game. Original-source semantics, resource staging, port logic, and runtime/visual verification are recorded separately.

## Scope expansion and parser repairs

Recovered **2 new scenes**, **35 new authored edges**, and **176 scene/platform module sources** beyond frozen Step 0. Eleven original malformed-rule frontiers now have narrow, in-memory repair receipts. The source bytes were not edited.

Remaining transition frontiers are kept explicit:

- **module_parse_failed** (`GUESS`): 019_light_house_03.rule.xml
- **module_source_missing** (`GUESS`): gc02_corner_ws_01.mgp
- **other_transition_mechanisms_not_closed** (`GUESS`): The scope follows authored TriggerZoneExitLevel.levelName values, direct and rule-candidate MGP includes, and serialized Script_ChangeLevel commands. Fast-travel menu membership, every Lua/native route, opaque rule conditions, and cross-platform differences are not yet proven closed.
- **possible_ios_only_source_candidate** (`GUESS`): No exact case-insensitive match was found in the explicit Android, Android original-cache, or extracted iOS data roots. The packed iOS logical-path index has not yet been joined for this filename; membership and any exit declarations remain unknown.

## Per-level inventory

Each linked JSON row carries the actor placements and Character rows, model/animation/sound/AI/loot source chains, campaign script and dialogue-command references, boss Lua evidence where present, B staging reference IDs, A native-contract source sites, and separate port/runtime status.

| Scene | Membership | Static placements | Linked actor rows | Scripts / commands | Boss candidates | Music | Loot | Port/runtime |
| --- | --- | ---: | ---: | ---: | ---: | --- | --- | --- |
| 001_swamp.mlx | act_1_backlink (LIKELY) | 50 | 18 | 55 / 789 | 1 | SwampHubAmbientMusic | 12 | UNVERIFIED / NOT RUN |
| 002_swamp_witchcave.rule.xml | act_1_backlink (LIKELY) | 39 | 7 | 24 / 183 | 0 | SwampWitchCaveAmbientMusic | 6 | UNVERIFIED / NOT RUN |
| 003_darkwood.mlx | act_2_inferred_start (LIKELY) | 90 | 30 | 105 / 1177 | 2 | DarkWoodHubAmbientMusic | 6 | UNVERIFIED / NOT RUN |
| 004_root_den.rule.xml | unknown_act_membership (GUESS) | 67 | 9 | 0 / 0 | 1 | unresolved | 8 | UNVERIFIED / NOT RUN |
| 005_infectedvillage.mlx | unknown_act_membership (GUESS) | 26 | 5 | 6 / 36 | 1 | InfectedVillageAmbientMusic | 4 | UNVERIFIED / NOT RUN |
| 006_basement.rule.xml | unknown_act_membership (GUESS) | 65 | 9 | 0 / 0 | 0 | unresolved | 7 | UNVERIFIED / NOT RUN |
| 007_crypt_01.rule.xml | unknown_act_membership (GUESS) | 108 | 10 | 0 / 0 | 4 | unresolved | 8 | UNVERIFIED / NOT RUN |
| 008_gothicus_castle_01.rule.xml | unknown_act_membership (GUESS) | 29 | 5 | 10 / 328 | 1 | GothicusCastleHubAmbientMusic | 2 | UNVERIFIED / NOT RUN |
| 009_abbey.rule.xml | unknown_act_membership (GUESS) | 90 | 16 | 37 / 424 | 3 | AbbeyRuinsOneAmbientMusic | 8 | UNVERIFIED / NOT RUN |
| 009a_abbey.mlx | unknown_act_membership (GUESS) | 14 | 10 | 37 / 424 | 0 | GatedCastleVillageMusic | 2 | UNVERIFIED / NOT RUN |
| 010_el_lightning.rule.xml | unknown_act_membership (GUESS) | 70 | 3 | 0 / 0 | 0 | unresolved | 2 | UNVERIFIED / NOT RUN |
| 011_earth_temple_01.rule.xml | unknown_act_membership (GUESS) | 136 | 5 | 5 / 23 | 0 | EarthTempleAmbientMusic | 4 | UNVERIFIED / NOT RUN |
| 012_earth_temple_02.rule.xml | unknown_act_membership (GUESS) | 148 | 11 | 8 / 60 | 2 | EarthTempleAmbientMusic | 10 | UNVERIFIED / NOT RUN |
| 013_earth_temple_03.rule.xml | unknown_act_membership (GUESS) | 110 | 14 | 15 / 274 | 1 | EarthTempleAmbientMusic | 12 | UNVERIFIED / NOT RUN |
| 014_el_earth.rule.xml | unknown_act_membership (GUESS) | 71 | 3 | 0 / 0 | 0 | unresolved | 2 | UNVERIFIED / NOT RUN |
| 015_red_desert_hub.mlx | unknown_act_membership (GUESS) | 40 | 14 | 23 / 158 | 0 | RedDesertHubAmbientMusic | 2 | UNVERIFIED / NOT RUN |
| 016_red_desert_cave_01.rule.xml | unknown_act_membership (GUESS) | 98 | 5 | 5 / 10 | 1 | RedDesertCaveOneAmbientMusic | 4 | UNVERIFIED / NOT RUN |
| 017_red_desert_cave_02.rule.xml | unknown_act_membership (GUESS) | 40 | 6 | 7 / 28 | 3 | RedDesertCaveTwoAmbientMusic | 5 | UNVERIFIED / NOT RUN |
| 018_red_desert_cave_03.rule.xml | unknown_act_membership (GUESS) | 64 | 4 | 5 / 10 | 1 | RedDesertThreeAmbientMusic | 3 | UNVERIFIED / NOT RUN |
| 019_light_house_01.rule.xml | unknown_act_membership (GUESS) | 106 | 6 | 55 / 617 | 7 | LighthouseTempleAmbientMusic | 3 | UNVERIFIED / NOT RUN |
| 019_light_house_02.rule.xml | unknown_act_membership (GUESS) | 92 | 6 | 0 / 0 | 7 | unresolved | 5 | UNVERIFIED / NOT RUN |
| 019_light_house_03.rule.xml | unknown_act_membership (GUESS) | 201 | 10 | 55 / 617 | 14 | LighthouseTempleAmbientMusic | 7 | UNVERIFIED / NOT RUN |
| 020_crypt_02.rule.xml | unknown_act_membership (GUESS) | 81 | 9 | 0 / 0 | 5 | unresolved | 7 | UNVERIFIED / NOT RUN |
| 021_gothicus_castle_02.rule.xml | unknown_act_membership (GUESS) | 47 | 10 | 22 / 225 | 0 | GothicusCastleHubAmbientMusic | 2 | UNVERIFIED / NOT RUN |
| 022_swamp2.rule.xml | unknown_act_membership (GUESS) | 50 | 9 | 6 / 38 | 0 | SwampHubAmbientMusic | 6 | UNVERIFIED / NOT RUN |
| 022_swamp2_trollcave.rule.xml | unknown_act_membership (GUESS) | 56 | 11 | 8 / 45 | 1 | SwampHubAmbientMusic | 10 | UNVERIFIED / NOT RUN |
| 023_watertemple.rule.xml | unknown_act_membership (GUESS) | 136 | 13 | 0 / 0 | 2 | unresolved | 8 | UNVERIFIED / NOT RUN |
| 024_el_water.rule.xml | unknown_act_membership (GUESS) | 66 | 3 | 0 / 0 | 2 | unresolved | 2 | UNVERIFIED / NOT RUN |
| 025_icy_hub.mlx | unknown_act_membership (GUESS) | 52 | 15 | 25 / 215 | 0 | IcyCliffHubAmbientMusic | 5 | UNVERIFIED / NOT RUN |
| 026_icy_cavern_01.rule.xml | unknown_act_membership (GUESS) | 54 | 6 | 7 / 36 | 1 | IcyCaveOneTwoAmbientMusic | 4 | UNVERIFIED / NOT RUN |
| 027_icy_cavern_02.rule.xml | unknown_act_membership (GUESS) | 71 | 8 | 10 / 68 | 2 | IcyCaveTwoAmbientMusic | 5 | UNVERIFIED / NOT RUN |
| 029_wind_temple.rule.xml | unknown_act_membership (GUESS) | 41 | 8 | 17 / 192 | 2 | WindTempleAmbientMusic | 6 | UNVERIFIED / NOT RUN |
| 030_el_air.rule.xml | unknown_act_membership (GUESS) | 72 | 3 | 0 / 0 | 1 | unresolved | 2 | UNVERIFIED / NOT RUN |
| 031_firetemple.mlx | unknown_act_membership (GUESS) | 58 | 7 | 20 / 366 | 1 | FireTempleAmbientMusic | 4 | UNVERIFIED / NOT RUN |
| 032_el_fire.rule.xml | unknown_act_membership (GUESS) | 72 | 3 | 0 / 0 | 0 | unresolved | 2 | UNVERIFIED / NOT RUN |
| 036_underworld_hub.mlx | unknown_act_membership (GUESS) | 51 | 13 | 27 / 282 | 4 | UnderworldHubAmbientMusic | 11 | UNVERIFIED / NOT RUN |
| 037_machine_rooms_01.mlx | unknown_act_membership (GUESS) | 58 | 5 | 67 / 195 | 1 | MachineRoomsAmbientMusic | 3 | UNVERIFIED / NOT RUN |
| 038_dark_temple.mlx | unknown_act_membership (GUESS) | 62 | 6 | 13 / 116 | 0 | DarkDungeonTempleAmbientMusic | 4 | UNVERIFIED / NOT RUN |
| 039_dark_temple_bossroom_01.mlx | unknown_act_membership (GUESS) | 19 | 10 | 13 / 254 | 13 | DarkDungeonTempleAmbientMusic | 5 | UNVERIFIED / NOT RUN |

## Readiness fractions

The conservative production-capability fraction counts a required per-level capability as supported only when source contract, package A/B cross-joins, frozen Windows production path, and integrated runtime/visual evidence are all present. This package is static-only and has not run those paths, so the verified-playable numerator is **0**. The explicit denominator is the count of required capability rows emitted in `coverage.json`; original asset presence does not increase it.

The separate static source-resolution fraction is reported in `coverage.json`. It measures source joins only and must not be read as playable coverage.

## Detailed artifacts

- `scope.json`: frozen Step 0 enumeration (preserved).
- `scope-expansion.json`: evidence-backed additions and source-repair receipts; new rows start TODO.
- `scope-current.json`: refreshed full transition closure for dossier joins.
- `dossier.json`: full per-level and per-entity evidence links.
- `levels/`: per-scene JSON and Markdown dossiers; actor details link to `entities/` shards.
- `entities/`: per-Character JSON and Markdown source-link records.
- `coverage.json` / `coverage.md`: explicit denominators, capability metric, and source-resolution counts.
- `blocker-reach.json` / `blocker-reach.md`: implementation order ranked by affected-scene reach.
