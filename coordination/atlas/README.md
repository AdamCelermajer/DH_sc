# Original Data Atlas

Generated from the supplied Android, iOS and Android original-cache corpus and the frozen port source. This is a static data and source atlas. Port status records source coverage; it does not certify gameplay or audibility.

Coverage check: **PASS**. Processed: **126,534/126,534 (100.00%)**.

Processed means every inventoried row has a terminal disposition. Explicitly unresolved references and native contracts remain follow-up work.

Commands (PowerShell, from this worktree):

```powershell
./tools/atlas/regen.ps1
. ./tools/atlas/use.ps1
atlas check
atlas serve
atlas show "Mud Lord"
atlas show skill:BashDown
atlas show item:Potion0
atlas mob Witch --json
atlas find sound:517
atlas cleancheck
```

[Open the searchable atlas](index.html). Run `atlas serve` and open `http://127.0.0.1:18789/` for source inspection and audio playback. Search by original name, English display name, URI or typed ID; Dependencies and Used by navigate in both directions.

[Mob, boss and actor profiles](general/mobs/index.md) join every CharacterProperties row to its available scripts, animation/effect/projectile dependencies, sound producers and loot graph. **Semantic coverage remains partial:** dynamic Lua bindings, native contracts, encounter placement and runtime conditions are explicit gaps. All inventoried Android table payload rows are decoded; packed iOS table payloads retain their own evidence limits. The processed percentage is not a claim that every mob is fully understood.

The wrapper stages an ignored portable interpreter from the Codex dependency bundle on first use. Use `--inputs <directory>` to select another read-only input corpus. No global Python or original data is committed.

## Coverage

| Category | Total | Statuses |
| --- | --- | --- |
| scripts | 219 | {"UNRESOLVED": 215, "DONE": 4} |
| functions | 288 | {"UNRESOLVED": 257, "DONE": 31} |
| calls | 7982 | {"DONE": 4170, "UNRESOLVED": 3812} |
| references | 95124 | {"UNRESOLVED": 42955, "RESOLVED": 49682, "AMBIGUOUS": 2, "NOT-NEEDED": 2485} |
| assets | 13355 | {"ORPHAN": 2455, "REFERENCED": 10900} |
| levels | 190 | {"DONE": 91, "UNRESOLVED": 15, "RESOLVED": 78, "INFERENCE": 6} |
| actors | 448 | {"RESOLVED": 448} |
| bosses | 65 | {"PROVEN": 59, "UNRESOLVED": 6} |
| tables | 8863 | {"RESOLVED": 8863} |

## Reference resolution and port status

| Kind | Total | Resolved | Ambiguous | Unresolved | Reasons | Port status |
| --- | --- | --- | --- | --- | --- | --- |
| actor_name_candidate | 46 | 32 | 0 | 14 | {"OTHER": 14} | {"UNKNOWN": 46} |
| anim | 4429 | 4429 | 0 | 0 | {} | {"PARTIAL": 1897, "UNKNOWN": 2532} |
| animated effect | 553 | 553 | 0 | 0 | {} | {"PARTIAL": 284, "UNKNOWN": 269} |
| animation event | 1042 | 0 | 0 | 1042 | {"OTHER": 1042} | {"UNKNOWN": 1042} |
| animation_dictionary | 854 | 854 | 0 | 0 | {} | {"PARTIAL": 854} |
| campaign_script_target_index | 1004 | 1004 | 0 | 0 | {} | {"UNKNOWN": 1004} |
| character | 25391 | 1105 | 0 | 24286 | {"OTHER": 24242, "NAME_NO_FILE": 44} | {"UNKNOWN": 24242, "PARTIAL": 1149} |
| class | 511 | 511 | 0 | 0 | {} | {"PARTIAL": 511} |
| data file | 698 | 225 | 0 | 473 | {"OTHER": 467, "FILE_MISSING": 2, "NAME_NO_FILE": 4} | {"UNKNOWN": 698} |
| dialog_style_id | 1812 | 0 | 0 | 1812 | {"OTHER": 1812} | {"UNKNOWN": 1812} |
| dialog_text_string_id | 1812 | 0 | 0 | 1812 | {"OTHER": 1812} | {"UNKNOWN": 1812} |
| effect set | 575 | 575 | 0 | 0 | {} | {"PARTIAL": 452, "UNKNOWN": 123} |
| effect_set | 370 | 370 | 0 | 0 | {} | {"PARTIAL": 370} |
| faction | 519 | 519 | 0 | 0 | {} | {"PARTIAL": 519} |
| faery | 29 | 29 | 0 | 0 | {} | {"PARTIAL": 29} |
| global_campaign_operand | 37 | 3 | 0 | 34 | {"OTHER": 34} | {"UNKNOWN": 37} |
| item | 2178 | 2178 | 0 | 0 | {} | {"PARTIAL": 2178} |
| item power | 3643 | 3643 | 0 | 0 | {} | {"PARTIAL": 3643} |
| level | 84 | 51 | 0 | 33 | {"OTHER": 33} | {"PARTIAL": 84} |
| level_name_candidate | 9 | 9 | 0 | 0 | {} | {"UNKNOWN": 9} |
| loot | 1141 | 1141 | 0 | 0 | {} | {"PARTIAL": 1141} |
| model | 13397 | 11097 | 0 | 2300 | {"NAME_NO_FILE": 1991, "FILE_MISSING": 280, "OTHER": 29} | {"UNKNOWN": 11388, "PARTIAL": 2009} |
| model component | 1245 | 0 | 0 | 1245 | {"OTHER": 1245} | {"PARTIAL": 1245} |
| named_reference | 3864 | 46 | 0 | 2032 | {"OTHER": 2029, "FILE_MISSING": 3} | {"UNKNOWN": 3864} |
| numeric_reference | 294 | 17 | 0 | 212 | {"OTHER": 198, "FILE_MISSING": 14} | {"UNKNOWN": 294} |
| projectile | 150 | 150 | 0 | 0 | {} | {"PARTIAL": 110, "UNKNOWN": 40} |
| quest | 154 | 154 | 0 | 0 | {} | {"PARTIAL": 154} |
| scene_object_condition | 472 | 462 | 0 | 10 | {"OTHER": 10} | {"UNKNOWN": 472} |
| script | 1628 | 681 | 0 | 947 | {"OTHER": 850, "FILE_MISSING": 97} | {"UNKNOWN": 1097, "PARTIAL": 531} |
| serialized_string | 7727 | 4133 | 0 | 3594 | {"OTHER": 3594} | {"UNKNOWN": 7727} |
| skill | 268 | 268 | 0 | 0 | {} | {"PARTIAL": 55, "UNKNOWN": 213} |
| sound | 4754 | 2988 | 2 | 1764 | {"OTHER": 391, "FILE_MISSING": 395, "NAME_NO_FILE": 978} | {"UNKNOWN": 4140, "PARTIAL": 614} |
| source_sound_ordinal | 202 | 202 | 0 | 0 | {} | {"PARTIAL": 202} |
| symbolic_reference | 2544 | 1067 | 0 | 843 | {"OTHER": 828, "NAME_NO_FILE": 4, "FILE_MISSING": 11} | {"UNKNOWN": 2544} |
| texture | 11688 | 11186 | 0 | 502 | {"FILE_MISSING": 492, "NAME_NO_FILE": 10} | {"UNKNOWN": 11688} |

## Asset consumers and orphans

| Corpus | Files | Referenced | ORPHAN |
| --- | --- | --- | --- |
| android | 6040 | 5170 | 870 |
| android-extra | 18 | 0 | 18 |
| audio-cache | 265 | 254 | 11 |
| ios | 256 | 254 | 2 |
| ios-app-resources | 28 | 0 | 28 |
| ios-glive | 50 | 0 | 50 |
| ios-packed | 6157 | 5107 | 1050 |
| original-cache | 152 | 112 | 40 |
| port-runtime-original-cache | 387 | 1 | 386 |
| sound-table | 2 | 2 | 0 |

ORPHAN means no extracted consumer in this supplied corpus. It does not prove an asset was unused by the original game.

## Engine functions to implement next

Ranked by distinct boss/custom scripts blocked, then all affected scripts, then call sites. PARTIAL remains a readiness gap.

| Function | Port status | Boss/custom scripts | All scripts | Call sites |
| --- | --- | --- | --- | --- |
| GetPyOID | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "darkqueen2_summons_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_core.luac", "madruk_dyingattack.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "madruktorso_dyingattack.luac", "miniboss_state.luac", "prince_assassin_assassinate.luac", "prince_assassin_assassination.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_contaminate.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "resurrectinggod.luac", "roottroll_core.luac", "sandworm01.luac", "sandworm02.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_queen.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_trap.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 160 | 416 |
| GetPyStruct | PARTIAL | ["assassin1_settings.luac", "assassin2_settings.luac", "assassin3_settings.luac", "assassindemon_settings.luac", "balrog_flames_skill.luac", "balrog_settings.luac", "darkqueen1_core.luac", "darkqueen1_settings.luac", "darkqueen2_ranged_00.luac", "darkqueen2_settings.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "edwardboss_settings.luac", "emissary_settings.luac", "flaminite_core.luac", "flaminite_settings.luac", "flaminiteed_settings.luac", "madruk_core.luac", "madruk_fastenergybolt.luac", "madruk_slowenergybolt.luac", "madruk_torso_core.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_drainlife.luac", "prince_assassin_assassinate.luac", "prince_assassin_assassination.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_contaminate.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "resurrectinggod.luac", "roottroll_core.luac", "roottroll_settings.luac", "sandworm01.luac", "sandworm02.luac", "sandworm_core.luac", "sandworm_queen.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_aoebombs.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "winddragon_settings.luac", "witch_ranged_00.luac"] | 139 | 431 |
| GetPyCst | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_core.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madruk_torso_core.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "miniboss_state.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_trap.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 115 | 598 |
| IsTargetListEmpty | MISSING | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 97 | 229 |
| GetTargetListTop | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 97 | 155 |
| TargetListSearch | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 97 | 138 |
| SetTargetListSorting | MISSING | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 97 | 128 |
| SetTargetListCharacterFilter | MISSING | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 97 | 126 |
| SetTargetListObjectFilter | MISSING | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 96 | 125 |
| LookAt | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_bite.luac", "witch_ranged_00.luac"] | 68 | 104 |
| Trace | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_summons_00.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_ranged_00.luac", "madruk_core.luac", "madruk_torso_core.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_blindness.luac", "prince_assassin_contaminate.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac", "resurrectinggod.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_trap.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 101 | 684 |
| ApplyPropClass | PARTIAL | ["balrog_flames_skill.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "sandworm_emerge.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 74 | 85 |
| PopTargetList | MISSING | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "madruk_aoeknockback.luac", "madruk_slowenergybolt.luac", "madruktorso_beamray.luac", "madruktorso_drainlife.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_aoebombs.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac"] | 66 | 81 |
| Include | PARTIAL | ["assassin1.luac", "assassin2.luac", "assassin3.luac", "assassindemon.luac", "balrog.luac", "darkqueen1.luac", "darkqueen2.luac", "edwardboss.luac", "emissary.luac", "flaminite.luac", "flaminiteed.luac", "madruk.luac", "madruk_torso.luac", "miniboss.luac", "roottroll.luac", "sandworm01.luac", "sandworm02.luac", "sandworm_queen.luac", "winddragon.luac"] | 28 | 49 |
| SetProp | PARTIAL | ["balrog_flames_skill.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "madruk_fastenergybolt.luac", "madruk_slowenergybolt.luac", "madruk_torso_core.luac", "madruktorso_drainlife.luac", "prince_assassin_assassinate.luac", "prince_assassin_assassination.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_contaminate.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "resurrectinggod.luac", "witch_ranged_00.luac"] | 90 | 137 |
| DoSkill | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_buffer.luac", "madruk_core.luac", "madruk_healer.luac", "madruk_torso_core.luac", "miniboss_state.luac", "resurrectinggod.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "winddragon_core.luac"] | 19 | 103 |
| GetRand | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "madruk_torso_core.luac", "miniboss_state.luac", "prince_assassin_contaminate.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "winddragon_aoebombs.luac", "winddragon_core.luac"] | 35 | 61 |
| MoveTo | PARTIAL | ["darkqueen1_core.luac", "darkqueen2_summons_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_buffer.luac", "madruk_core.luac", "madruk_healer.luac", "madruk_torso_core.luac", "miniboss_state.luac", "roottroll_core.luac", "winddragon_core.luac"] | 21 | 56 |
| Stop | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_buffer.luac", "madruk_core.luac", "madruk_healer.luac", "resurrectinggod.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small_core.luac", "winddragon_core.luac"] | 21 | 54 |
| AddAggro | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "madruk_torso_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "winddragon_core.luac"] | 13 | 27 |
| PlayFX | PARTIAL | ["darkqueen1_core.luac", "flaminite_slam.luac", "madruk_dyingattack.luac", "madruk_slam.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_drainlife.luac", "madruktorso_dyingattack.luac", "sandworm_emerge.luac"] | 28 | 34 |
| ClearAggro | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "winddragon_core.luac"] | 13 | 25 |
| GetHostPlayerDifficulty | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "winddragon_core.luac"] | 13 | 13 |
| CallPyScript | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "madruk_dyingattack.luac", "madruktorso_dyingattack.luac", "resurrectinggod.luac", "roottroll_core.luac", "winddragon_core.luac"] | 12 | 17 |
| ApplyBuff | PARTIAL | ["madruktorso_drainlife.luac", "prince_assassin_assassinate.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "sandworm_small.luac", "sandworm_small_core.luac"] | 41 | 54 |
| CreateBuff | PARTIAL | ["madruktorso_drainlife.luac", "prince_assassin_assassinate.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "sandworm_small.luac", "sandworm_small_core.luac"] | 41 | 54 |
| GetSelf | MISSING | ["madruk_core.luac", "madruk_torso_core.luac", "prince_assassin_assassination.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_core.luac"] | 24 | 36 |
| GetDistanceBetween | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_aoebombs.luac"] | 15 | 23 |
| GetHostPlayerLevel | PARTIAL | ["edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "winddragon_core.luac"] | 13 | 13 |
| GetCurrentLevelRange | PARTIAL | ["edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "winddragon_core.luac"] | 12 | 12 |
| PlayMusic | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "madruk_torso_core.luac", "resurrectinggod.luac", "roottroll_core.luac", "winddragon_core.luac"] | 9 | 16 |
| PlaySound | PARTIAL | ["darkqueen1_core.luac", "madruk_core.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "sandworm_emerge.luac", "winddragon_aoebombs.luac"] | 27 | 35 |
| GetPropHP | PARTIAL | ["madruk_core.luac", "madruk_torso_core.luac", "madruktorso_drainlife.luac", "resurrectinggod.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac"] | 10 | 15 |
| target:PlayFX | PARTIAL | ["flaminite_slam.luac", "flaminite_sp_attack.luac", "madruk_slam.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac"] | 26 | 31 |
| target:GetName | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac", "madruktorso_drainlife.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac"] | 19 | 26 |
| Attack | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "miniboss_state.luac", "roottroll_core.luac", "winddragon_core.luac"] | 11 | 30 |
| TargetListBackup | MISSING | ["darkqueen1_core.luac", "flaminite_core.luac", "prince_assassin_assassination.luac", "winddragon_breathsweep.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac"] | 11 | 14 |
| RemoveBuff | PARTIAL | ["prince_assassin_assassinate.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac"] | 42 | 64 |
| GetCurrentSpellInfo | MISSING | ["darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "madruk_fastenergybolt.luac", "madruk_slowenergybolt.luac", "witch_ranged_00.luac"] | 15 | 15 |
| PlayAnim | PARTIAL | ["flaminite_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac"] | 7 | 19 |
| MarkAsFlying | MISSING | ["sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "witch.luac"] | 5 | 5 |
| HasMana | PARTIAL | ["prince_assassin_assassination.luac", "prince_assassin_blindness.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac"] | 40 | 40 |
| UseMana | PARTIAL | ["prince_assassin_assassination.luac", "prince_assassin_blindness.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac"] | 40 | 40 |
| GetName | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac"] | 13 | 19 |
| HasPath | PARTIAL | ["darkqueen1_core.luac", "darkqueen2_summons_00.luac", "madruk_core.luac", "miniboss_state.luac"] | 11 | 27 |
| Kill | PARTIAL | ["madruk_buffer.luac", "madruk_dyingattack.luac", "madruk_healer.luac", "madruktorso_dyingattack.luac"] | 5 | 7 |
| LookAtTarget | PARTIAL | ["madruktorso_drainlife.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac"] | 4 | 4 |
| GetTargetListSize | MISSING | ["darkqueen1_core.luac", "flaminite_core.luac", "sandworm_small.luac"] | 9 | 9 |
| target:IsCharacter | PARTIAL | ["madruktorso_drainlife.luac", "prince_assassin_grapling_hook.luac", "witch_ranged_00.luac"] | 9 | 9 |
| target:IsPlayer | PARTIAL | ["edwardboss_core.luac", "roottroll_core.luac", "sandworm_core.luac"] | 8 | 8 |
| SetIsTargetable | PARTIAL | ["prince_assassin_blindness.luac", "sandworm_core.luac", "sandworm_small_core.luac"] | 3 | 7 |
| SetActorPosition | PARTIAL | ["sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac"] | 3 | 3 |
| target:ApplyBuff | PARTIAL | ["madrukminion_bufferspell.luac", "madruktorso_drainlife.luac"] | 15 | 15 |
| target:CreateBuff | PARTIAL | ["madrukminion_bufferspell.luac", "madruktorso_drainlife.luac"] | 15 | 15 |
| Summon | PARTIAL | ["darkqueen2_summons_00.luac", "madruk_core.luac"] | 13 | 29 |
| SetMaster | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 7 | 8 |
| HasMaster | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 6 | 9 |
| target:SetStunState | MISSING | ["madruktorso_drainlife.luac", "prince_assassin_grapling_hook.luac"] | 5 | 5 |
| WarpBehind | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 4 | 4 |
| SetIsActorTargetable | MISSING | ["sandworm_core.luac", "sandworm_small.luac"] | 3 | 4 |
| target:GetPropHP | PARTIAL | ["madrukminion_healerspell.luac", "madruktorso_drainlife.luac"] | 3 | 3 |
| SummonTimerTrap | PARTIAL | ["sandworm_spit.luac", "swampking_atk_spit.luac"] | 2 | 4 |
| RegisterSummon | PARTIAL | ["madruk_core.luac", "resurrectinggod.luac"] | 2 | 3 |
| friend:GetName | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 2 | 2 |
| g_master:GetName | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 2 | 2 |
| g_target:GetName | PARTIAL | ["sandworm_spit.luac", "swampking_atk_spit.luac"] | 2 | 2 |
| obj:GetPropHP | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac"] | 2 | 2 |
| target:GetMaster | MISSING | ["darkqueen1_core.luac", "flaminite_core.luac"] | 2 | 2 |
| target:HasMaster | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac"] | 2 | 2 |
| WarpTo | PARTIAL | ["darkqueen1_core.luac"] | 5 | 10 |
| attacker:TargetInMeleeRange | MISSING | ["prince_assassin_contaminate.luac"] | 5 | 5 |
| CanAttackFromRange | MISSING | ["miniboss_state.luac"] | 3 | 4 |
| CanAttackInMelee | MISSING | ["miniboss_state.luac"] | 3 | 4 |
| GetGameObjectsByType | PARTIAL | ["winddragon_aoebombs.luac"] | 3 | 3 |
| obj:GetName | PARTIAL | ["winddragon_aoebombs.luac"] | 3 | 3 |
| object:GetName | PARTIAL | ["winddragon_breathsweep.luac"] | 2 | 3 |
| DealDamages | PARTIAL | ["swampking_trap.luac"] | 2 | 2 |
| attacker:ApplyBuff | PARTIAL | ["prince_assassin_contaminate.luac"] | 2 | 2 |
| attacker:CreateBuff | PARTIAL | ["prince_assassin_contaminate.luac"] | 2 | 2 |
| myTarget:IsPlayer | PARTIAL | ["winddragon_core.luac"] | 2 | 2 |
| objspawns:GetName | PARTIAL | ["winddragon_aoebombs.luac"] | 2 | 2 |
| object:PlayFX | PARTIAL | ["winddragon_breathsweep.luac"] | 1 | 4 |
| attackedObject:GetName | PARTIAL | ["winddragon_breathsweep.luac"] | 1 | 2 |
| invisible_man:WarpTo | PARTIAL | ["madruktorso_drainlife.luac"] | 1 | 2 |
| ChangeAIState | PARTIAL | ["miniboss.luac"] | 1 | 1 |
| RegisterAIState | PARTIAL | ["miniboss.luac"] | 1 | 1 |
| RegisterActorAnim | PARTIAL | ["miniboss_state.luac"] | 1 | 1 |
| Remove | PARTIAL | ["swampking_trap.luac"] | 1 | 1 |
| invisible_man:PlayFX | PARTIAL | ["madruktorso_drainlife.luac"] | 1 | 1 |
| target:EnableCollisions | MISSING | ["madruktorso_drainlife.luac"] | 1 | 1 |
| target:HeadTo | MISSING | ["prince_assassin_grapling_hook.luac"] | 1 | 1 |
| target:Kill | PARTIAL | ["prince_assassin_assassination.luac"] | 1 | 1 |
| target:PlayActorAnim | PARTIAL | ["madruktorso_drainlife.luac"] | 1 | 1 |
| target:SetProp | PARTIAL | ["madrukminion_healerspell.luac"] | 1 | 1 |
| target:TargetListSearch | PARTIAL | ["sandworm_small.luac"] | 1 | 1 |
| target:WarpTo | PARTIAL | ["madruktorso_drainlife.luac"] | 1 | 1 |
| HasBow | MISSING | [] | 6 | 8 |
| PlaySound3D | MISSING | [] | 5 | 16 |
| SpellCombatRoll | PARTIAL | [] | 5 | 15 |
| SetBuffProp | MISSING | [] | 5 | 7 |
| GetInt | PARTIAL | [] | 5 | 5 |
| actor:PlayFX | PARTIAL | [] | 5 | 5 |
| SetInt | PARTIAL | [] | 4 | 18 |
| Flee | MISSING | [] | 4 | 7 |
| ClearProps | PARTIAL | [] | 4 | 6 |
| EnableCollisions | MISSING | [] | 4 | 5 |
| actor:ApplyBuff | PARTIAL | [] | 4 | 4 |
| actor:CreateBuff | PARTIAL | [] | 4 | 4 |
| GetHP | PARTIAL | [] | 3 | 4 |
| HeadTo | MISSING | [] | 3 | 4 |
| target:SetKnockBackState | MISSING | [] | 3 | 4 |
| defender:IsDead | PARTIAL | [] | 3 | 3 |
| monster:IsDead | PARTIAL | [] | 3 | 3 |
| monster:Kill | PARTIAL | [] | 3 | 3 |
| player:GetName | PARTIAL | [] | 3 | 3 |
| GetEquippedFaeryElement | MISSING | [] | 2 | 6 |
| GetHostPlayer | PARTIAL | [] | 2 | 3 |
| SetProjectileTarget | MISSING | [] | 2 | 3 |
| attacker:Stop | PARTIAL | [] | 2 | 3 |
| defender:GetName | PARTIAL | [] | 2 | 3 |
| target:IsDoor | MISSING | [] | 2 | 3 |
| attacker:SetKnockBackState | MISSING | [] | 2 | 2 |
| attacker:SetStunState | MISSING | [] | 2 | 2 |
| defender:ApplyBuff | PARTIAL | [] | 2 | 2 |
| defender:CreateBuff | PARTIAL | [] | 2 | 2 |
| object:IsDead | PARTIAL | [] | 2 | 2 |
| target:IsDead | PARTIAL | [] | 2 | 2 |
| wave.precb | MISSING | [] | 2 | 2 |
| GetCurrentEquippedFaeryLevel | MISSING | [] | 1 | 5 |
| GetMaster | MISSING | [] | 1 | 5 |
| Rand | PARTIAL | [] | 1 | 5 |
| KillRemainingMonsters | MISSING | [] | 1 | 4 |
| attacker:GetName | PARTIAL | [] | 1 | 4 |
| GetObjectByName | PARTIAL | [] | 1 | 3 |
| item:ApplyBuff | PARTIAL | [] | 1 | 3 |
| item:CreateBuff | PARTIAL | [] | 1 | 3 |
| item:GetName | PARTIAL | [] | 1 | 3 |
| target:GetCharAIFlags | MISSING | [] | 1 | 3 |
| target:SetScareState | MISSING | [] | 1 | 3 |
| GetNumPlayers | PARTIAL | [] | 1 | 2 |
| HasShield | PARTIAL | [] | 1 | 2 |
| SetSkillCooldownTimerId__ | MISSING | [] | 1 | 2 |
| SetSpellCooldownTimerId__ | MISSING | [] | 1 | 2 |
| attacker:SetProp | PARTIAL | [] | 1 | 2 |
| hasReachDestination | MISSING | [] | 1 | 2 |
| monster:MoveTo | PARTIAL | [] | 1 | 2 |
| monster:UNLOCK | PARTIAL | [] | 1 | 2 |
| mothSlot1:UNLOCK | PARTIAL | [] | 1 | 2 |
| mothSlot2:UNLOCK | PARTIAL | [] | 1 | 2 |
| mothSlot3:UNLOCK | PARTIAL | [] | 1 | 2 |
| mothSlot4:UNLOCK | PARTIAL | [] | 1 | 2 |
| oldTarget:GetName | PARTIAL | [] | 1 | 2 |
| player:WarpTo | PARTIAL | [] | 1 | 2 |
| refData.callback | PARTIAL | [] | 1 | 2 |
| target:SetBuffProp | MISSING | [] | 1 | 2 |
| victim:LOCK | MISSING | [] | 1 | 2 |
| AddPlayer | PARTIAL | [] | 1 | 1 |
| CF__CalcDodge | MISSING | [] | 1 | 1 |
| GetCurrentSkillInfo__ | MISSING | [] | 1 | 1 |
| InitPathingLocations | MISSING | [] | 1 | 1 |
| IsMasterHostPlayer | PARTIAL | [] | 1 | 1 |
| PlayActorAnim | PARTIAL | [] | 1 | 1 |
| RegenHP | PARTIAL | [] | 1 | 1 |
| SLOT1_ITEM:GetName | PARTIAL | [] | 1 | 1 |
| SLOT2_ITEM:GetName | PARTIAL | [] | 1 | 1 |
| SLOT3_ITEM:GetName | PARTIAL | [] | 1 | 1 |
| SetGameType | PARTIAL | [] | 1 | 1 |
| SetMaxPath | MISSING | [] | 1 | 1 |
| SkillCombatRoll__ | MISSING | [] | 1 | 1 |
| SpawnSkillProjectile__ | MISSING | [] | 1 | 1 |
| StopSkill__ | MISSING | [] | 1 | 1 |
| TargetInMeleeRange | MISSING | [] | 1 | 1 |
| UpdatePlayerPosition | MISSING | [] | 1 | 1 |
| UpdatePlayers | MISSING | [] | 1 | 1 |
| actor:RegenHP | PARTIAL | [] | 1 | 1 |
| actor:RegenMP | PARTIAL | [] | 1 | 1 |
| attackTarget:GetName | PARTIAL | [] | 1 | 1 |
| attackTarget:LOCK | MISSING | [] | 1 | 1 |
| attacker:ApplyPropClass | PARTIAL | [] | 1 | 1 |
| attacker:GetAttackRatingBonus | MISSING | [] | 1 | 1 |
| attacker:GetDamageBonus | MISSING | [] | 1 | 1 |
| attacker:GetHP | PARTIAL | [] | 1 | 1 |
| attacker:GetHitCount | MISSING | [] | 1 | 1 |
| attacker:Kill | PARTIAL | [] | 1 | 1 |
| cb_info.callback | PARTIAL | [] | 1 | 1 |
| defender:GetHP | PARTIAL | [] | 1 | 1 |
| defender:HasShield | PARTIAL | [] | 1 | 1 |
| defender:IsCharacter | PARTIAL | [] | 1 | 1 |
| defender:RemoveBuff | PARTIAL | [] | 1 | 1 |
| defender:SetIsTargetable | PARTIAL | [] | 1 | 1 |
| defender:Stop | PARTIAL | [] | 1 | 1 |
| fdata.func | PARTIAL | [] | 1 | 1 |
| firstTarget:GetName | PARTIAL | [] | 1 | 1 |
| firstTarget:PlayFX | PARTIAL | [] | 1 | 1 |
| goToNextDestination | MISSING | [] | 1 | 1 |
| hellhole:GetHP | PARTIAL | [] | 1 | 1 |
| hellhole:IsDead | PARTIAL | [] | 1 | 1 |
| monster:GetName | PARTIAL | [] | 1 | 1 |
| mothSlot1:IsDead | PARTIAL | [] | 1 | 1 |
| mothSlot1:Kill | PARTIAL | [] | 1 | 1 |
| mothSlot1:LOCK | MISSING | [] | 1 | 1 |
| mothSlot2:IsDead | PARTIAL | [] | 1 | 1 |
| mothSlot2:Kill | PARTIAL | [] | 1 | 1 |
| mothSlot2:LOCK | MISSING | [] | 1 | 1 |
| mothSlot3:IsDead | PARTIAL | [] | 1 | 1 |
| mothSlot3:Kill | PARTIAL | [] | 1 | 1 |
| mothSlot3:LOCK | MISSING | [] | 1 | 1 |
| mothSlot4:IsDead | PARTIAL | [] | 1 | 1 |
| mothSlot4:Kill | PARTIAL | [] | 1 | 1 |
| mothSlot4:LOCK | MISSING | [] | 1 | 1 |
| object:ApplyBuff | PARTIAL | [] | 1 | 1 |
| object:CreateBuff | PARTIAL | [] | 1 | 1 |
| object:IsConnected | MISSING | [] | 1 | 1 |
| objpath:GetName | PARTIAL | [] | 1 | 1 |
| playAlarmAnimation | MISSING | [] | 1 | 1 |
| player:GetHP | PARTIAL | [] | 1 | 1 |
| player:IsConnected | MISSING | [] | 1 | 1 |
| player:IsDead | PARTIAL | [] | 1 | 1 |
| player:Kill | PARTIAL | [] | 1 | 1 |
| player:RemoveBuff | PARTIAL | [] | 1 | 1 |
| secondTarget:GetName | PARTIAL | [] | 1 | 1 |
| startDebuffTimer | MISSING | [] | 1 | 1 |
| statue:SetMaster | PARTIAL | [] | 1 | 1 |
| statue:SetProp | PARTIAL | [] | 1 | 1 |
| statueDummy:GetHP | PARTIAL | [] | 1 | 1 |
| statueDummy:Kill | PARTIAL | [] | 1 | 1 |
| target:AddAggro | PARTIAL | [] | 1 | 1 |
| target:ClearAggro | PARTIAL | [] | 1 | 1 |
| target:IsDecor | MISSING | [] | 1 | 1 |
| target:RegenHP | PARTIAL | [] | 1 | 1 |
| target:RegenMP | PARTIAL | [] | 1 | 1 |
| target:SetIsTargetable | PARTIAL | [] | 1 | 1 |
| tcallback.mycallback | MISSING | [] | 1 | 1 |
| thisTarget:GetName | PARTIAL | [] | 1 | 1 |
| v:ApplyBuff | PARTIAL | [] | 1 | 1 |
| v:CreateBuff | PARTIAL | [] | 1 | 1 |
| victim:GetName | PARTIAL | [] | 1 | 1 |
| victim:IsCharacter | PARTIAL | [] | 1 | 1 |
| victim:PlayFX | PARTIAL | [] | 1 | 1 |
| wave.postcb | MISSING | [] | 1 | 1 |

## Known-answer checks

| Check | Passed | Evidence/result |
| --- | --- | --- |
| swampking_core.luac | True | Exact call-list fixture matches=True (46 names); code anchors verify source-specific skill slots, dive targetability, animation/state and timer parsing plus timer-trap parser. B159 supersedes the early B155 gaps for level scaling, damage calibration and AOE pushback: those are implemented and documented in mudlord3-report.md. Remaining documented gaps are MarkAsFlying, the camera-frustum awake latch, resetonhubchange, timer/culling differences, TimerTrap point-cloud bounds and projectile expiry; SetIsTargetable is a targeting gate, not a damage gate. Missing code anchors=[]. |
| witch.luac | True | Exact call-list fixture matches=True (44 names); DoSkill count=1, only slot 0=True, 10s teleport delay=True. Production code anchors cover generic DoTeleport/FX/WarpTo and timer delay handling; full user-session integration is not asserted. Missing code anchors=[]. |
| sound row index is the 638-row source table with 63 events | True | rows=638, names=638, isEvent=63, ledger=638 |
| 638 audio rows resolve from the union of supplied source corpora | True | {"rows": 638, "saved_rows": 638, "disagreement_count": 0, "disagreement_sample": [], "row11_balrog_killed": "RESOLVED", "row244_event": "RESOLVED"} |
| 45 distinct sounds.xml filenames absent from the combined original corpus | True | xml_names=498, combined_source_missing=45, ios_only_missing=245; combined_missing_names=['m_boss_hell_king.wav', 'm_combat_big_enemy.wav', 'm_cutscene_01.wav', 'm_level_cathedral.vxn', 'm_level_darkwood.vxn', 'm_level_gothicus.vxn', 'm_level_hell.vxn', 'm_level_icy_cliff.vxn', 'm_level_red_desert.vxn', 'm_level_swamp.vxn', 'm_title_intro.wav', 'm_title_loop.wav', 'sfx_assassin_bomb_throw.wav', 'sfx_assassin_knife_throw1.wav', 'sfx_assassin_laugh.wav', 'sfx_balrog_idle.wav', 'sfx_checkpoint_activated.wav', 'sfx_critical_hit.wav', 'sfx_cultitst_group_chant_loop', 'sfx_dark_queen_critical_hurt.wav', 'sfx_dark_queen_intimidate_1.wav', 'sfx_dark_queen_intimidate_2.wav', 'sfx_dark_queen_intimidate_3.wav', 'sfx_dark_queen_killed.wav', 'sfx_demon_attack.wav', 'sfx_demon_die.wav', 'sfx_demon_howl.wav', 'sfx_demon_hurt.wav', 'sfx_demon_rain_attack.wav', 'sfx_demon_spawn.wav', 'sfx_demon_steam_burst_attack', 'sfx_faerie_ice.wav', 'sfx_impact_metal_metal_2.wav', 'sfx_impactl_metal_1_22.wav', 'sfx_skill_mage_thunder_braid_add_2,wav', 'sfx_specter_attack_energyball_explosion', 'sfx_statue_attack.wav', 'sfx_statue_frost_attack.wav', 'sfx_statue_heat_attack.wav', 'sfx_trap_armed.wav', 'sfx_wolf_attack.wav', 'sfx_wolf_die.wav', 'sfx_wolf_howl.wav', 'sfx_wolf_hurt.wav', 'sfx_wolf_spawn.wav'] |
| iOS-only sound corpus gap retained as platform diagnostic | True | ios_only_missing_distinct=245; iOS corpus is one source platform, not the complete Android/recovered-cache authority. |
| sfx_faerie_ice.wav is absent from original data | True | sfx_faerie_ice.wav present=False |
| Knight BashDown, GroundSlam, and Charge source rows | True | {"expected": {"BashDown": [347, "skill_dh2_prince_warrior_bash_down.bdae", 164, 190, 101], "GroundSlam": [350, "skill_dh2_prince_warrior_crushing_blow.bdae", 173, 185, 103], "Charge": [349, "skill_dh2_prince_warrior_charge.bdae", 172, 48, 102]}, "actual": {"BashDown": {"skill_index": 7, "skill_anim": 347, "fx": 164, "sound": 190, "sound_uid": 101, "clip_present": true, "matrix_row_present": true}, "GroundSlam": {"skill_index": 51, "skill_anim": 350, "fx": 173, "sound": 185, "sound_uid": 103, "clip_present": true, "matrix_row_present": true}, "Charge": {"skill_index": 13, "skill_anim": 349, "fx": 172, "sound": 48, "sound_uid": 102, "clip_present": true, "matrix_row_present": true}}} |
| literal PyData aliases and gated Swamp charpropsname source joins | True | {"literal_calls": [{"site": "darkqueen1_core.luac:10", "status": "RESOLVED", "table": "effect_sets", "index": 229, "expected_table": "effect_sets", "expected_index": 229}, {"site": "flaminite_core.luac:67", "status": "RESOLVED", "table": "animations_dictionary#group0", "index": 1144, "expected_table": "animations_dictionary#group0", "expected_index": 1144}, {"site": "prince_mage_stone_escape.luac:30", "status": "RESOLVED", "table": "effect_sets", "index": 229, "expected_table": "effect_sets", "expected_index": 229}], "charpropsname": {"status": "RESOLVED", "table": "character_properties#group0", "index": 360, "port_status": "PARTIAL"}, "CharSoundsTable_group_scope": {"status": "RESOLVED", "table": "sounds#group0", "target_table": "sounds#group0", "index": 2, "source_row_table": "sounds#group0", "source_row_name": "Default"}} |
| itemdrop scene tokens and SkillTable script/icon source targets | True | {"itemdrop_visual_rows": 29, "itemdrop_unresolved_symbols": 29, "skill_script_rows": 124, "scripts_with_exact_uri_assets": 124, "skill_icon_rows": 74, "icons_with_sprite79_frames": 74} |

## Platform comparison

| Logical resource format | Platform availability |
| --- | --- |
| .bdae | {"BOTH": 2892, "IOS_ONLY": 9} |
| .bin | {"ANDROID_OR_RECOVERED_ONLY": 4, "BOTH": 215} |
| .english | {"BOTH": 37} |
| .fnt | {"BOTH": 1} |
| .french | {"BOTH": 37} |
| .german | {"BOTH": 37} |
| .italian | {"BOTH": 37} |
| .japanese | {"BOTH": 37} |
| .korean | {"BOTH": 37} |
| .lightset_xml | {"BOTH": 40} |
| .luac | {"BOTH": 219} |
| .mgp | {"BOTH": 629} |
| .mgx | {"BOTH": 250} |
| .mlx | {"BOTH": 400} |
| .mvp | {"BOTH": 598} |
| .mvx | {"BOTH": 253} |
| .pak | {"IOS_ONLY": 1} |
| .sc | {"BOTH": 37} |
| .spanish | {"BOTH": 37} |
| .state | {"BOTH": 1} |
| .swf | {"ANDROID_OR_RECOVERED_ONLY": 23, "BOTH": 7} |
| .symbols | {"BOTH": 37} |
| .tga | {"ANDROID_OR_RECOVERED_ONLY": 9, "BOTH": 233} |
| .ttf | {"ANDROID_OR_RECOVERED_ONLY": 7} |
| .tweaker_xml | {"IOS_ONLY": 2} |
| .txt | {"BOTH": 22} |
| .vxn | {"IOS_ONLY": 14} |
| .wav | {"IOS_ONLY": 239} |
| .xml | {"BOTH": 39} |
| .zip | {"IOS_ONLY": 1} |
| extensionless | {"ANDROID_OR_RECOVERED_ONLY": 237} |

Both supplied partial caches are used under the owner’s clarification. Each resolved target retains its source platform. The iOS-only audio subset has 245 distinct missing catalog filenames; combining it with the recovered Android audio leaves the required 45.

## Checker failure-branch validation

| Test | Passed |
| --- | --- |
| unmodified generated atlas passes | True |
| missing script row fails | True |
| missing mob profile fails | True |
| missing mob section fails | True |
| missing CharSounds reference fails | True |
| missing function row fails | True |
| missing XML/BRES reference site fails | True |
| missing row status fails | True |
| unresolved reference without reason fails | True |
| unresolved reference without candidates fails | True |
| unresolved function without candidates fails | True |
| nonexistent referenced id fails | True |
| nonexistent typed table target fails | True |
| nonexistent target path fails | True |
| extra function row fails | True |
| paired script reference deletion fails | True |
| duplicate table source key fails | True |
| stale table source key fails | True |
| unreferenced file without ORPHAN fails | True |
| fabricated referrer fails | True |
| unknown row status fails | True |
| unknown work status fails | True |

## Act gaps

Act allocation and boundaries are **INFERENCE pending owner confirmation**. See [generated act views](general/README.md), [resolution rules](RULES.md), [progress](PROGRESS.md) and [evidence reports](reports/).

```json
{
  "by_act": [
    {
      "act": "act_1",
      "boss_flag_proven_global": 59,
      "incomplete_campaign_engine_commands": 0,
      "rank_basis": "Source-joined incomplete campaign command port anchors and unresolved references; command addresses and ref resolver metadata retained.",
      "status": "DONE",
      "unmapped_actor_rows_global": 400,
      "unresolved_boss_candidates_global": 6,
      "unresolved_boss_candidates_status": "UNRESOLVED",
      "unresolved_source_references": 1736
    },
    {
      "act": "act_2",
      "boss_flag_proven_global": 59,
      "incomplete_campaign_engine_commands": 13,
      "rank_basis": "Source-joined incomplete campaign command port anchors and unresolved references; command addresses and ref resolver metadata retained.",
      "status": "DONE",
      "unmapped_actor_rows_global": 400,
      "unresolved_boss_candidates_global": 6,
      "unresolved_boss_candidates_status": "UNRESOLVED",
      "unresolved_source_references": 1043
    },
    {
      "act": "act_3",
      "boss_flag_proven_global": 59,
      "incomplete_campaign_engine_commands": 0,
      "rank_basis": "Source-joined incomplete campaign command port anchors and unresolved references; command addresses and ref resolver metadata retained.",
      "status": "DONE",
      "unmapped_actor_rows_global": 400,
      "unresolved_boss_candidates_global": 6,
      "unresolved_boss_candidates_status": "UNRESOLVED",
      "unresolved_source_references": 0
    },
    {
      "act": "act_4",
      "boss_flag_proven_global": 59,
      "incomplete_campaign_engine_commands": 0,
      "rank_basis": "Source-joined incomplete campaign command port anchors and unresolved references; command addresses and ref resolver metadata retained.",
      "status": "DONE",
      "unmapped_actor_rows_global": 400,
      "unresolved_boss_candidates_global": 6,
      "unresolved_boss_candidates_status": "UNRESOLVED",
      "unresolved_source_references": 0
    },
    {
      "act": "act_5",
      "boss_flag_proven_global": 59,
      "incomplete_campaign_engine_commands": 0,
      "rank_basis": "Source-joined incomplete campaign command port anchors and unresolved references; command addresses and ref resolver metadata retained.",
      "status": "DONE",
      "unmapped_actor_rows_global": 400,
      "unresolved_boss_candidates_global": 6,
      "unresolved_boss_candidates_status": "UNRESOLVED",
      "unresolved_source_references": 0
    }
  ],
  "ranking_scope": "Act 2+ only. Campaign engine command rows include native kind, factory/reader address, descriptor source, port status, and current anchors; unresolved source rows include resolver metadata.",
  "shared_asset_count": 85,
  "shared_rule": "A stable asset ID is shared only when its source-linked references occur in at least two explicitly assigned acts.",
  "status": "DONE",
  "top20_acts_2_plus": [
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 263,
      "kind": 10,
      "name": "Script_StartDialog",
      "native_factory_address": "0x458a70",
      "native_reader_address": "0x459034",
      "port_path_anchors": [],
      "port_status": "UNKNOWN",
      "priority": 0,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:13",
      "status": "UNRESOLVED",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 253,
      "kind": 12,
      "name": "Script_WaitDialog",
      "native_factory_address": "0x4589e0",
      "native_reader_address": "0x455628",
      "port_path_anchors": [],
      "port_status": "UNKNOWN",
      "priority": 0,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:14",
      "status": "UNRESOLVED",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 2,
      "kind": 64,
      "name": "Script_ChangeLevel",
      "native_factory_address": "0x457b4c",
      "native_reader_address": "0x455628",
      "port_path_anchors": [],
      "port_status": "UNKNOWN",
      "priority": 0,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:74",
      "status": "UNRESOLVED",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 60,
      "kind": 41,
      "name": "Script_LookActor",
      "native_factory_address": "0x45819c",
      "native_reader_address": "0x455628",
      "port_path_anchors": [
        "port/windows-foundation/features/campaign_host/campaign_host.cpp:637"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:35",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 29,
      "kind": 45,
      "name": "Script_PlayActorAnim",
      "native_factory_address": "0x4595c8",
      "native_reader_address": "0x45a2ec",
      "port_path_anchors": [
        "port/windows-foundation/features/campaign_host/campaign_host.cpp:595"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:39",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 29,
      "kind": 46,
      "name": "Script_SetActorPosition",
      "native_factory_address": "0x45807c",
      "native_reader_address": "0x455628",
      "port_path_anchors": [
        "port/windows-foundation/features/campaign_host/campaign_host.cpp:619"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:40",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 16,
      "kind": 43,
      "name": "Script_HideActor",
      "native_factory_address": "0x45810c",
      "native_reader_address": "0x455628",
      "port_path_anchors": [
        "port/windows-foundation/features/campaign_host/campaign_host.cpp:662"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:37",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 16,
      "kind": 42,
      "name": "Script_ShowActor",
      "native_factory_address": "0x458154",
      "native_reader_address": "0x455628",
      "port_path_anchors": [
        "port/windows-foundation/features/campaign_host/campaign_host.cpp:661"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:36",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 9,
      "kind": 13,
      "name": "Script_PlaySound",
      "native_factory_address": "0x458998",
      "native_reader_address": "0x455628",
      "port_path_anchors": [
        "port/windows-foundation/features/campaign_host/campaign_host.cpp:755"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:15",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 4,
      "kind": 40,
      "name": "Script_MoveActor",
      "native_factory_address": "0x4581e4",
      "native_reader_address": "0x455628",
      "port_path_anchors": [
        "port/windows-foundation/features/campaign_host/campaign_host.cpp:574"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:34",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 2,
      "kind": 30,
      "name": "Script_SpawnCharacter",
      "native_factory_address": "0x4584c0",
      "native_reader_address": "0x455628",
      "port_path_anchors": [
        "port/windows-foundation/original_campaign_world_adapter.cpp:25",
        "port/windows-foundation/original_campaign_world_adapter.cpp:37"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:30",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 2,
      "kind": 39,
      "name": "Script_StopActor",
      "native_factory_address": "0x458238",
      "native_reader_address": "0x455628",
      "port_path_anchors": [
        "port/windows-foundation/original_campaign_world_adapter.cpp:25",
        "port/windows-foundation/original_campaign_world_adapter.cpp:38"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:33",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "category": "campaign_engine_command_port_gap",
      "count": 1,
      "kind": 14,
      "name": "Script_StopSound",
      "native_factory_address": "0x458950",
      "native_reader_address": "0x455628",
      "port_path_anchors": [
        "port/windows-foundation/features/campaign_host/campaign_host.cpp:732"
      ],
      "port_status": "PARTIAL",
      "priority": 1,
      "scene": "003_darkwood.mlx",
      "source": "003_darkwood.mlx",
      "source_descriptor": "port/level-loader/command_c1_descriptors_v59.inc:16",
      "status": "PARTIAL",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "actor": "DW_Merchant",
      "actor_id": "89d6b10973db87e6eecd",
      "category": "unresolved_actor_table_dependency",
      "confidence": "PROVEN",
      "dependency": "CharacterProperties[56].AI -> ai#group0[39 ]",
      "field": "Script",
      "implementation": "port/game-data/ai.cpp:22",
      "index": 39,
      "port_status": "PARTIAL",
      "priority": 2,
      "reason": "OTHER",
      "reference_id": "a90ce796fa6cad82548e",
      "rule": "CharAI::SetScriptByName; scripts/ai/<name>.luac or native __policy__",
      "status": "UNRESOLVED",
      "table": "ai#group0",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "actor": "DW_Merchant",
      "actor_id": "89d6b10973db87e6eecd",
      "category": "unresolved_actor_table_dependency",
      "confidence": "PROVEN",
      "dependency": "CharacterProperties[56].Loot -> loot_table#group5[2 ] -> SubLoots[0] -> loot_table#group5[256 ] -> RandomLootEntries[0].ItemListID -> loot_table#group2[176 ] -> List[0].Item -> loot_table#group3[682 ] -> AudioVisualID -> loot_audiovisual#group0[18 ]",
      "field": "Visual",
      "implementation": "port/game-data/loot_audiovisual_v8.cpp:10",
      "index": 18,
      "port_status": "PARTIAL",
      "priority": 2,
      "reason": "OTHER",
      "reference_id": "1f55ab86a417fde515fc",
      "rule": "port/game-data/loot_audiovisual_v8.cpp:10; field Visual",
      "status": "UNRESOLVED",
      "table": "loot_audiovisual#group0",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "actor": "DW_Merchant",
      "actor_id": "89d6b10973db87e6eecd",
      "category": "unresolved_actor_table_dependency",
      "confidence": "PROVEN",
      "dependency": "CharacterProperties[56].Loot -> loot_table#group5[2 ] -> SubLoots[0] -> loot_table#group5[256 ] -> RandomLootEntries[0].ItemListID -> loot_table#group2[176 ] -> List[0].Item -> loot_table#group3[682 ]",
      "field": "ModularModule",
      "implementation": "port/game-data/items.cpp:43",
      "index": 682,
      "port_status": "PARTIAL",
      "priority": 2,
      "reason": "OTHER",
      "reference_id": "5706e25bea23d2442fc7",
      "rule": "Item metadata stores a named modular node/icon; a matching file name alone is insufficient to prove its consumer.",
      "status": "UNRESOLVED",
      "table": "loot_table#group3",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "actor": "DW_Merchant",
      "actor_id": "89d6b10973db87e6eecd",
      "category": "unresolved_actor_table_dependency",
      "confidence": "PROVEN",
      "dependency": "CharacterProperties[56].Loot -> loot_table#group5[2 ] -> SubLoots[0] -> loot_table#group5[256 ] -> RandomLootEntries[0].ItemListID -> loot_table#group2[176 ] -> List[1].Item -> loot_table#group3[49 ] -> AudioVisualID -> loot_audiovisual#group0[0 ]",
      "field": "Visual",
      "implementation": "port/game-data/loot_audiovisual_v8.cpp:10",
      "index": 0,
      "port_status": "PARTIAL",
      "priority": 2,
      "reason": "OTHER",
      "reference_id": "8b5c056b727bf2ddee6e",
      "rule": "port/game-data/loot_audiovisual_v8.cpp:10; field Visual",
      "status": "UNRESOLVED",
      "table": "loot_audiovisual#group0",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "actor": "DW_Merchant",
      "actor_id": "89d6b10973db87e6eecd",
      "category": "unresolved_actor_table_dependency",
      "confidence": "PROVEN",
      "dependency": "CharacterProperties[56].Loot -> loot_table#group5[2 ] -> SubLoots[0] -> loot_table#group5[256 ] -> RandomLootEntries[0].ItemListID -> loot_table#group2[176 ] -> List[1].Item -> loot_table#group3[49 ]",
      "field": "ModularModule",
      "implementation": "port/game-data/items.cpp:43",
      "index": 49,
      "port_status": "PARTIAL",
      "priority": 2,
      "reason": "OTHER",
      "reference_id": "101b67cb92d66b9e8c43",
      "rule": "Item metadata stores a named modular node/icon; a matching file name alone is insufficient to prove its consumer.",
      "status": "UNRESOLVED",
      "table": "loot_table#group3",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "actor": "DW_Merchant",
      "actor_id": "89d6b10973db87e6eecd",
      "category": "unresolved_actor_table_dependency",
      "confidence": "PROVEN",
      "dependency": "CharacterProperties[56].Loot -> loot_table#group5[2 ] -> SubLoots[0] -> loot_table#group5[256 ] -> RandomLootEntries[0].ItemListID -> loot_table#group2[176 ] -> List[2].Item -> loot_table#group3[1292 ] -> AudioVisualID -> loot_audiovisual#group0[28 ]",
      "field": "Visual",
      "implementation": "port/game-data/loot_audiovisual_v8.cpp:10",
      "index": 28,
      "port_status": "PARTIAL",
      "priority": 2,
      "reason": "OTHER",
      "reference_id": "db9a4024c467bafa637d",
      "rule": "port/game-data/loot_audiovisual_v8.cpp:10; field Visual",
      "status": "UNRESOLVED",
      "table": "loot_audiovisual#group0",
      "work_status": "DONE"
    },
    {
      "act": "act_2",
      "actor": "DW_Merchant",
      "actor_id": "89d6b10973db87e6eecd",
      "category": "unresolved_actor_table_dependency",
      "confidence": "PROVEN",
      "dependency": "CharacterProperties[56].Loot -> loot_table#group5[2 ] -> SubLoots[0] -> loot_table#group5[256 ] -> RandomLootEntries[0].ItemListID -> loot_table#group2[176 ] -> List[2].Item -> loot_table#group3[1292 ]",
      "field": "ModularModule",
      "implementation": "port/game-data/items.cpp:43",
      "index": 1292,
      "port_status": "PARTIAL",
      "priority": 2,
      "reason": "OTHER",
      "reference_id": "9593a42715c34b34682a",
      "rule": "Item metadata stores a named modular node/icon; a matching file name alone is insufficient to prove its consumer.",
      "status": "UNRESOLVED",
      "table": "loot_table#group3",
      "work_status": "DONE"
    }
  ]
}
```
