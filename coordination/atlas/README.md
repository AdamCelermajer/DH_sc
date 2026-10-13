# Original Data Atlas

Generated from the supplied Android, iOS and Android original-cache corpus and the frozen port source. This is a static data and source atlas. Port status records source coverage; it does not certify gameplay or audibility.

Coverage check: **PASS**. Processed: **84,855/84,855 (100.00%)**.

Commands (PowerShell, from this worktree):

```powershell
./tools/atlas/regen.ps1
./tools/atlas/atlas.ps1 check
./tools/atlas/atlas.ps1 find swampking
./tools/atlas/atlas.ps1 find sound:517
```

The wrapper stages an ignored portable interpreter from the Codex dependency bundle on first use. Use `--inputs <directory>` to select another read-only input corpus. No global Python or original data is committed.

## Coverage

| Category | Total | Statuses |
| --- | --- | --- |
| scripts | 219 | {"UNRESOLVED": 215, "DONE": 4} |
| functions | 288 | {"UNRESOLVED": 257, "DONE": 31} |
| calls | 7982 | {"DONE": 4170, "UNRESOLVED": 3812} |
| references | 60106 | {"UNRESOLVED": 31769, "RESOLVED": 28337} |
| assets | 6733 | {"ORPHAN": 964, "REFERENCED": 5769} |
| levels | 190 | {"DONE": 91, "UNRESOLVED": 99} |
| actors | 448 | {"RESOLVED": 448} |
| bosses | 26 | {"UNRESOLVED": 26} |
| tables | 8863 | {"RESOLVED": 7463, "UNRESOLVED": 1400} |

## Reference resolution and port status

| Kind | Total | Resolved | Ambiguous | Unresolved | Reasons | Port status |
| --- | --- | --- | --- | --- | --- | --- |
| actor_name_candidate | 46 | 6 | 0 | 40 | {"OTHER": 40} | {"UNKNOWN": 46} |
| anim | 4429 | 4429 | 0 | 0 | {} | {"PARTIAL": 1897, "UNKNOWN": 2532} |
| animated effect | 553 | 553 | 0 | 0 | {} | {"PARTIAL": 284, "UNKNOWN": 269} |
| animation event | 521 | 0 | 0 | 521 | {"OTHER": 521} | {"UNKNOWN": 521} |
| animation_dictionary | 854 | 854 | 0 | 0 | {} | {"PARTIAL": 854} |
| character | 12050 | 0 | 0 | 12050 | {"OTHER": 12050} | {"UNKNOWN": 12050} |
| class | 298 | 298 | 0 | 0 | {} | {"PARTIAL": 298} |
| data file | 230 | 225 | 0 | 5 | {"FILE_MISSING": 1, "NAME_NO_FILE": 4} | {"UNKNOWN": 230} |
| effect set | 575 | 575 | 0 | 0 | {} | {"PARTIAL": 452, "UNKNOWN": 123} |
| effect_set | 370 | 370 | 0 | 0 | {} | {"PARTIAL": 370} |
| faction | 519 | 519 | 0 | 0 | {} | {"PARTIAL": 519} |
| faery | 29 | 29 | 0 | 0 | {} | {"PARTIAL": 29} |
| item | 2178 | 2178 | 0 | 0 | {} | {"PARTIAL": 2178} |
| item power | 3643 | 3643 | 0 | 0 | {} | {"PARTIAL": 3643} |
| level_name_candidate | 9 | 9 | 0 | 0 | {} | {"UNKNOWN": 9} |
| loot | 1141 | 1141 | 0 | 0 | {} | {"PARTIAL": 1141} |
| model | 8022 | 6683 | 0 | 1339 | {"FILE_MISSING": 375, "NAME_NO_FILE": 964} | {"UNKNOWN": 6013, "PARTIAL": 2009} |
| model component | 1245 | 0 | 0 | 1245 | {"OTHER": 1245} | {"PARTIAL": 1245} |
| named_reference | 1939 | 0 | 0 | 1939 | {"OTHER": 1936, "FILE_MISSING": 3} | {"UNKNOWN": 1939} |
| numeric_reference | 294 | 0 | 0 | 294 | {"OTHER": 279, "FILE_MISSING": 15} | {"UNKNOWN": 294} |
| projectile | 150 | 150 | 0 | 0 | {} | {"PARTIAL": 110, "UNKNOWN": 40} |
| script | 1164 | 576 | 0 | 588 | {"OTHER": 494, "FILE_MISSING": 94} | {"UNKNOWN": 633, "PARTIAL": 531} |
| serialized_string | 7727 | 0 | 0 | 7727 | {"OTHER": 7727} | {"UNKNOWN": 7727} |
| skill | 268 | 268 | 0 | 0 | {} | {"PARTIAL": 55, "UNKNOWN": 213} |
| sound | 3257 | 2194 | 0 | 1063 | {"FILE_MISSING": 245, "OTHER": 55, "NAME_NO_FILE": 763} | {"UNKNOWN": 2653, "PARTIAL": 604} |
| source_sound_ordinal | 202 | 202 | 0 | 0 | {} | {"PARTIAL": 202} |
| symbolic_reference | 2544 | 0 | 0 | 2544 | {"OTHER": 2533, "FILE_MISSING": 11} | {"UNKNOWN": 2544} |
| texture | 5849 | 3435 | 0 | 2414 | {"FILE_MISSING": 244, "NAME_NO_FILE": 2170} | {"UNKNOWN": 5849} |

## Asset consumers and orphans

| Corpus | Files | Referenced | ORPHAN |
| --- | --- | --- | --- |
| android | 6040 | 5148 | 892 |
| android-extra | 18 | 0 | 18 |
| audio-cache | 265 | 254 | 11 |
| ios | 256 | 254 | 2 |
| original-cache | 152 | 112 | 40 |
| sound-table | 2 | 1 | 1 |

ORPHAN means no extracted consumer in this supplied corpus. It does not prove an asset was unused by the original game.

## Engine functions to implement next

Ranked by distinct boss/custom scripts blocked, then all affected scripts, then call sites. PARTIAL remains a readiness gap.

| Function | Port status | Boss/custom scripts | All scripts | Call sites |
| --- | --- | --- | --- | --- |
| GetPyOID | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "darkqueen2_summons_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_core.luac", "madruk_dyingattack.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "madruktorso_dyingattack.luac", "miniboss_state.luac", "prince_assassin_assassinate.luac", "prince_assassin_assassination.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_contaminate.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "resurrectinggod.luac", "roottroll_core.luac", "sandworm01.luac", "sandworm02.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_queen.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "swampking_trap.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac", "witch_ranged_00.luac"] | 162 | 428 |
| None | UNKNOWN | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "darkqueen2_summons_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_buffer.luac", "madruk_core.luac", "madruk_dyingattack.luac", "madruk_fastenergybolt.luac", "madruk_healer.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madruk_torso_core.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "madruktorso_dyingattack.luac", "miniboss.luac", "prince_assassin_assassinate.luac", "prince_assassin_assassination.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_contaminate.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "resurrectinggod.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac", "witch_ranged_00.luac"] | 157 | 1310 |
| GetPyStruct | PARTIAL | ["assassin1_settings.luac", "assassin2_settings.luac", "assassin3_settings.luac", "assassindemon_settings.luac", "balrog_flames_skill.luac", "balrog_settings.luac", "darkqueen1_core.luac", "darkqueen1_settings.luac", "darkqueen2_ranged_00.luac", "darkqueen2_settings.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "edwardboss_settings.luac", "emissary_settings.luac", "flaminite_core.luac", "flaminite_settings.luac", "flaminiteed_settings.luac", "madruk_core.luac", "madruk_fastenergybolt.luac", "madruk_slowenergybolt.luac", "madruk_torso_core.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_drainlife.luac", "prince_assassin_assassinate.luac", "prince_assassin_assassination.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_contaminate.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "resurrectinggod.luac", "roottroll_core.luac", "roottroll_settings.luac", "sandworm01.luac", "sandworm02.luac", "sandworm_core.luac", "sandworm_queen.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "winddragon_settings.luac", "witch.luac", "witch_ranged_00.luac"] | 141 | 442 |
| GetPyCst | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_core.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madruk_torso_core.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "miniboss_state.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "swampking_trap.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac", "witch_ranged_00.luac"] | 117 | 608 |
| IsTargetListEmpty | MISSING | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac", "witch_ranged_00.luac"] | 99 | 233 |
| GetTargetListTop | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac", "witch_ranged_00.luac"] | 99 | 157 |
| TargetListSearch | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac", "witch_ranged_00.luac"] | 99 | 140 |
| SetTargetListSorting | MISSING | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac", "witch_ranged_00.luac"] | 99 | 130 |
| SetTargetListCharacterFilter | MISSING | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac", "witch_ranged_00.luac"] | 99 | 128 |
| SetTargetListObjectFilter | MISSING | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac", "witch_ranged_00.luac"] | 98 | 127 |
| LookAt | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_bite.luac", "witch.luac", "witch_ranged_00.luac"] | 70 | 106 |
| Trace | PARTIAL | ["balrog_flames_skill.luac", "darkqueen1_core.luac", "darkqueen2_melee_00.luac", "darkqueen2_summons_00.luac", "flaminite_core.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_ranged_00.luac", "madruk_core.luac", "madruk_torso_core.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_blindness.luac", "prince_assassin_contaminate.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac", "resurrectinggod.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "swampking_trap.luac", "winddragon_aoebombs.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 102 | 725 |
| ApplyPropClass | PARTIAL | ["balrog_flames_skill.luac", "darkqueen2_melee_00.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "flaminite_slam.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "lighthouseemissary_ranged_00.luac", "madruk_aoeknockback.luac", "madruk_fastenergybolt.luac", "madruk_slam.luac", "madruk_slowenergybolt.luac", "madruktorso_bite.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "sandworm_emerge.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_bite.luac", "winddragon_breathsweep.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch_ranged_00.luac"] | 74 | 85 |
| PopTargetList | MISSING | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "flaminite_sp_attack.luac", "lighthouseemissary_melee_00.luac", "madruk_aoeknockback.luac", "madruk_slowenergybolt.luac", "madruktorso_beamray.luac", "madruktorso_drainlife.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_emerge.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_breathsweep.luac", "winddragon_core.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac", "witch.luac"] | 68 | 83 |
| Include | PARTIAL | ["assassin1.luac", "assassin2.luac", "assassin3.luac", "assassindemon.luac", "balrog.luac", "darkqueen1.luac", "darkqueen2.luac", "edwardboss.luac", "emissary.luac", "flaminite.luac", "flaminiteed.luac", "madruk.luac", "madruk_torso.luac", "miniboss.luac", "roottroll.luac", "sandworm01.luac", "sandworm02.luac", "sandworm_queen.luac", "winddragon.luac"] | 28 | 49 |
| SetProp | PARTIAL | ["balrog_flames_skill.luac", "darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "madruk_fastenergybolt.luac", "madruk_slowenergybolt.luac", "madruk_torso_core.luac", "madruktorso_drainlife.luac", "prince_assassin_assassinate.luac", "prince_assassin_assassination.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_contaminate.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "resurrectinggod.luac", "witch_ranged_00.luac"] | 90 | 137 |
| DoSkill | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_buffer.luac", "madruk_core.luac", "madruk_healer.luac", "madruk_torso_core.luac", "miniboss_state.luac", "resurrectinggod.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "winddragon_core.luac", "witch.luac"] | 21 | 116 |
| GetRand | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "madruk_torso_core.luac", "miniboss_state.luac", "prince_assassin_contaminate.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "winddragon_core.luac"] | 36 | 68 |
| Stop | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_buffer.luac", "madruk_core.luac", "madruk_healer.luac", "resurrectinggod.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small_core.luac", "swampking_core.luac", "winddragon_core.luac", "witch.luac"] | 23 | 59 |
| MoveTo | PARTIAL | ["darkqueen1_core.luac", "darkqueen2_summons_00.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_buffer.luac", "madruk_core.luac", "madruk_healer.luac", "madruk_torso_core.luac", "miniboss_state.luac", "roottroll_core.luac", "winddragon_core.luac", "witch.luac"] | 22 | 61 |
| AddAggro | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "madruk_torso_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "winddragon_core.luac", "witch.luac"] | 15 | 30 |
| ClearAggro | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "winddragon_core.luac", "witch.luac"] | 15 | 30 |
| GetHostPlayerDifficulty | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "winddragon_core.luac", "witch.luac"] | 15 | 15 |
| PlayFX | PARTIAL | ["darkqueen1_core.luac", "flaminite_slam.luac", "madruk_dyingattack.luac", "madruk_slam.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_drainlife.luac", "madruktorso_dyingattack.luac", "sandworm_emerge.luac", "witch.luac"] | 29 | 36 |
| GetHostPlayerLevel | PARTIAL | ["edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "winddragon_core.luac", "witch.luac"] | 15 | 15 |
| GetCurrentLevelRange | PARTIAL | ["edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "winddragon_core.luac", "witch.luac"] | 14 | 14 |
| PlayMusic | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "madruk_torso_core.luac", "resurrectinggod.luac", "roottroll_core.luac", "swampking_core.luac", "winddragon_core.luac", "witch.luac"] | 11 | 20 |
| PlaySound | PARTIAL | ["darkqueen1_core.luac", "madruk_core.luac", "madruktorso_drainlife.luac", "prince_assassin_assassination.luac", "prince_assassin_grapling_hook.luac", "sandworm_emerge.luac", "swampking_core.luac", "winddragon_aoebombs.luac", "witch.luac"] | 29 | 41 |
| GetSelf | MISSING | ["madruk_core.luac", "madruk_torso_core.luac", "prince_assassin_assassination.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "winddragon_core.luac", "witch.luac"] | 25 | 37 |
| GetDistanceBetween | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac", "swampking_core.luac", "winddragon_aoebombs.luac"] | 16 | 25 |
| CallPyScript | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "madruk_core.luac", "madruk_dyingattack.luac", "madruktorso_dyingattack.luac", "resurrectinggod.luac", "roottroll_core.luac", "winddragon_core.luac"] | 12 | 17 |
| ApplyBuff | PARTIAL | ["madruktorso_drainlife.luac", "prince_assassin_assassinate.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "sandworm_small.luac", "sandworm_small_core.luac"] | 41 | 54 |
| CreateBuff | PARTIAL | ["madruktorso_drainlife.luac", "prince_assassin_assassinate.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac", "sandworm_small.luac", "sandworm_small_core.luac"] | 41 | 54 |
| GetPropHP | PARTIAL | ["madruk_core.luac", "madruk_torso_core.luac", "madruktorso_drainlife.luac", "resurrectinggod.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac"] | 11 | 16 |
| target:GetName | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac", "madruktorso_drainlife.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac"] | 20 | 28 |
| target:PlayFX | PARTIAL | ["flaminite_slam.luac", "flaminite_sp_attack.luac", "madruk_slam.luac", "madrukminion_bufferspell.luac", "madrukminion_healerspell.luac", "madruktorso_beamray.luac"] | 26 | 31 |
| Attack | PARTIAL | ["darkqueen1_core.luac", "edwardboss_core.luac", "flaminite_core.luac", "miniboss_state.luac", "roottroll_core.luac", "winddragon_core.luac"] | 11 | 30 |
| TargetListBackup | MISSING | ["darkqueen1_core.luac", "flaminite_core.luac", "prince_assassin_assassination.luac", "winddragon_breathsweep.luac", "winddragon_insidesweep.luac", "winddragon_outsidesweep.luac"] | 11 | 14 |
| PlayAnim | PARTIAL | ["flaminite_core.luac", "roottroll_core.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac"] | 8 | 21 |
| LookAtTarget | PARTIAL | ["madruktorso_drainlife.luac", "sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "witch.luac"] | 6 | 6 |
| RemoveBuff | PARTIAL | ["prince_assassin_assassinate.luac", "prince_assassin_blade_deflect.luac", "prince_assassin_blindness.luac", "prince_assassin_poison_sting.luac", "prince_assassin_treasure_hunter.luac"] | 42 | 64 |
| GetCurrentSpellInfo | MISSING | ["darkqueen2_ranged_00.luac", "darkqueen2_slave_ranged_00.luac", "madruk_fastenergybolt.luac", "madruk_slowenergybolt.luac", "witch_ranged_00.luac"] | 15 | 15 |
| HasPath | PARTIAL | ["darkqueen1_core.luac", "darkqueen2_summons_00.luac", "madruk_core.luac", "miniboss_state.luac", "witch.luac"] | 12 | 32 |
| MarkAsFlying | MISSING | ["sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac", "witch.luac"] | 5 | 5 |
| HasMana | PARTIAL | ["prince_assassin_assassination.luac", "prince_assassin_blindness.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac"] | 40 | 40 |
| UseMana | PARTIAL | ["prince_assassin_assassination.luac", "prince_assassin_blindness.luac", "prince_assassin_grapling_hook.luac", "prince_assassin_poison_sting.luac"] | 40 | 40 |
| GetName | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac", "sandworm_spit.luac", "swampking_atk_spit.luac"] | 13 | 19 |
| target:IsPlayer | PARTIAL | ["edwardboss_core.luac", "roottroll_core.luac", "sandworm_core.luac", "witch.luac"] | 9 | 9 |
| Kill | PARTIAL | ["madruk_buffer.luac", "madruk_dyingattack.luac", "madruk_healer.luac", "madruktorso_dyingattack.luac"] | 5 | 7 |
| SetIsTargetable | PARTIAL | ["prince_assassin_blindness.luac", "sandworm_core.luac", "sandworm_small_core.luac", "swampking_core.luac"] | 4 | 10 |
| SetActorPosition | PARTIAL | ["sandworm_core.luac", "sandworm_small.luac", "sandworm_small_core.luac", "swampking_core.luac"] | 4 | 5 |
| GetTargetListSize | MISSING | ["darkqueen1_core.luac", "flaminite_core.luac", "sandworm_small.luac"] | 9 | 9 |
| target:IsCharacter | PARTIAL | ["madruktorso_drainlife.luac", "prince_assassin_grapling_hook.luac", "witch_ranged_00.luac"] | 9 | 9 |
| target:ApplyBuff | PARTIAL | ["madrukminion_bufferspell.luac", "madruktorso_drainlife.luac"] | 15 | 15 |
| target:CreateBuff | PARTIAL | ["madrukminion_bufferspell.luac", "madruktorso_drainlife.luac"] | 15 | 15 |
| Summon | PARTIAL | ["darkqueen2_summons_00.luac", "madruk_core.luac"] | 13 | 29 |
| SetMaster | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 7 | 8 |
| WarpTo | PARTIAL | ["darkqueen1_core.luac", "witch.luac"] | 6 | 14 |
| HasMaster | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 6 | 9 |
| target:SetStunState | MISSING | ["madruktorso_drainlife.luac", "prince_assassin_grapling_hook.luac"] | 5 | 5 |
| WarpBehind | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 4 | 4 |
| SetIsActorTargetable | MISSING | ["sandworm_core.luac", "sandworm_small.luac"] | 3 | 4 |
| myTarget:IsPlayer | PARTIAL | ["swampking_core.luac", "winddragon_core.luac"] | 3 | 3 |
| target:GetPropHP | PARTIAL | ["madrukminion_healerspell.luac", "madruktorso_drainlife.luac"] | 3 | 3 |
| SummonTimerTrap | PARTIAL | ["sandworm_spit.luac", "swampking_atk_spit.luac"] | 2 | 4 |
| RegisterSummon | PARTIAL | ["madruk_core.luac", "resurrectinggod.luac"] | 2 | 3 |
| friend:GetName | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 2 | 2 |
| g_master:GetName | PARTIAL | ["madruk_buffer.luac", "madruk_healer.luac"] | 2 | 2 |
| g_target:GetName | PARTIAL | ["sandworm_spit.luac", "swampking_atk_spit.luac"] | 2 | 2 |
| obj:GetPropHP | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac"] | 2 | 2 |
| target:GetMaster | MISSING | ["darkqueen1_core.luac", "flaminite_core.luac"] | 2 | 2 |
| target:HasMaster | PARTIAL | ["darkqueen1_core.luac", "flaminite_core.luac"] | 2 | 2 |
| attacker:TargetInMeleeRange | MISSING | ["prince_assassin_contaminate.luac"] | 5 | 5 |
| HeadTo | MISSING | ["witch.luac"] | 4 | 5 |
| CanAttackFromRange | MISSING | ["miniboss_state.luac"] | 3 | 4 |
| CanAttackInMelee | MISSING | ["miniboss_state.luac"] | 3 | 4 |
| GetGameObjectsByType | PARTIAL | ["winddragon_aoebombs.luac"] | 3 | 3 |
| obj:GetName | PARTIAL | ["winddragon_aoebombs.luac"] | 3 | 3 |
| object:GetName | PARTIAL | ["winddragon_breathsweep.luac"] | 2 | 3 |
| DealDamages | PARTIAL | ["swampking_trap.luac"] | 2 | 2 |
| attacker:ApplyBuff | PARTIAL | ["prince_assassin_contaminate.luac"] | 2 | 2 |
| attacker:CreateBuff | PARTIAL | ["prince_assassin_contaminate.luac"] | 2 | 2 |
| objspawns:GetName | PARTIAL | ["winddragon_aoebombs.luac"] | 2 | 2 |
| StopSound | PARTIAL | ["swampking_core.luac"] | 1 | 4 |
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
| 45 distinct sounds.xml filenames absent from the combined original corpus | True | xml_names=498, combined_source_missing=45, ios_only_missing=245; combined_missing_names=['m_boss_hell_king.wav', 'm_combat_big_enemy.wav', 'm_cutscene_01.wav', 'm_level_cathedral.vxn', 'm_level_darkwood.vxn', 'm_level_gothicus.vxn', 'm_level_hell.vxn', 'm_level_icy_cliff.vxn', 'm_level_red_desert.vxn', 'm_level_swamp.vxn', 'm_title_intro.wav', 'm_title_loop.wav', 'sfx_assassin_bomb_throw.wav', 'sfx_assassin_knife_throw1.wav', 'sfx_assassin_laugh.wav', 'sfx_balrog_idle.wav', 'sfx_checkpoint_activated.wav', 'sfx_critical_hit.wav', 'sfx_cultitst_group_chant_loop', 'sfx_dark_queen_critical_hurt.wav', 'sfx_dark_queen_intimidate_1.wav', 'sfx_dark_queen_intimidate_2.wav', 'sfx_dark_queen_intimidate_3.wav', 'sfx_dark_queen_killed.wav', 'sfx_demon_attack.wav', 'sfx_demon_die.wav', 'sfx_demon_howl.wav', 'sfx_demon_hurt.wav', 'sfx_demon_rain_attack.wav', 'sfx_demon_spawn.wav', 'sfx_demon_steam_burst_attack', 'sfx_faerie_ice.wav', 'sfx_impact_metal_metal_2.wav', 'sfx_impactl_metal_1_22.wav', 'sfx_skill_mage_thunder_braid_add_2,wav', 'sfx_specter_attack_energyball_explosion', 'sfx_statue_attack.wav', 'sfx_statue_frost_attack.wav', 'sfx_statue_heat_attack.wav', 'sfx_trap_armed.wav', 'sfx_wolf_attack.wav', 'sfx_wolf_die.wav', 'sfx_wolf_howl.wav', 'sfx_wolf_hurt.wav', 'sfx_wolf_spawn.wav'] |
| iOS-only sound corpus gap retained as platform diagnostic | True | ios_only_missing_distinct=245; iOS corpus is one source platform, not the complete Android/recovered-cache authority. |
| sfx_faerie_ice.wav is absent from original data | True | sfx_faerie_ice.wav present=False |
| Knight BashDown, GroundSlam, and Charge source rows | True | {"expected": {"BashDown": [347, "skill_dh2_prince_warrior_bash_down.bdae", 164, 190, 101], "GroundSlam": [350, "skill_dh2_prince_warrior_crushing_blow.bdae", 173, 185, 103], "Charge": [349, "skill_dh2_prince_warrior_charge.bdae", 172, 48, 102]}, "actual": {"BashDown": {"skill_index": 7, "skill_anim": 347, "fx": 164, "sound": 190, "sound_uid": 101, "clip_present": true, "matrix_row_present": true}, "GroundSlam": {"skill_index": 51, "skill_anim": 350, "fx": 173, "sound": 185, "sound_uid": 103, "clip_present": true, "matrix_row_present": true}, "Charge": {"skill_index": 13, "skill_anim": 349, "fx": 172, "sound": 48, "sound_uid": 102, "clip_present": true, "matrix_row_present": true}}} |

## Act gaps

Act allocation and boundaries are **INFERENCE pending owner confirmation**. See [generated act views](general/README.md), [resolution rules](RULES.md), [progress](PROGRESS.md) and [evidence reports](reports/).

```json
{
  "by_act": [
    {
      "act": "act_2",
      "rank_basis": "source-scene references with TODO/UNRESOLVED/AMBIGUOUS status; global actor/boss/dialogue gaps shown separately",
      "status": "TODO",
      "unmapped_actor_rows": 448,
      "unmapped_boss_candidates": 26,
      "unmapped_dialogue_rows": 55,
      "unresolved_scene_references": 274
    },
    {
      "act": "act_3",
      "rank_basis": "source-scene references with TODO/UNRESOLVED/AMBIGUOUS status; global actor/boss/dialogue gaps shown separately",
      "status": "TODO",
      "unmapped_actor_rows": 448,
      "unmapped_boss_candidates": 26,
      "unmapped_dialogue_rows": 55,
      "unresolved_scene_references": 158
    },
    {
      "act": "act_1",
      "rank_basis": "source-scene references with TODO/UNRESOLVED/AMBIGUOUS status; global actor/boss/dialogue gaps shown separately",
      "status": "TODO",
      "unmapped_actor_rows": 448,
      "unmapped_boss_candidates": 26,
      "unmapped_dialogue_rows": 55,
      "unresolved_scene_references": 153
    },
    {
      "act": "act_4",
      "rank_basis": "source-scene references with TODO/UNRESOLVED/AMBIGUOUS status; global actor/boss/dialogue gaps shown separately",
      "status": "TODO",
      "unmapped_actor_rows": 448,
      "unmapped_boss_candidates": 26,
      "unmapped_dialogue_rows": 55,
      "unresolved_scene_references": 77
    },
    {
      "act": "act_5",
      "rank_basis": "source-scene references with TODO/UNRESOLVED/AMBIGUOUS status; global actor/boss/dialogue gaps shown separately",
      "status": "TODO",
      "unmapped_actor_rows": 448,
      "unmapped_boss_candidates": 26,
      "unmapped_dialogue_rows": 55,
      "unresolved_scene_references": 5
    }
  ],
  "shared_rule": "A target is shared only when resolved references from source scene files in at least two distinct inferred acts point to the same target.",
  "status": "TODO",
  "top20_acts_2_plus": []
}
```
