# General authored FX resource V5 integration

The renderer now owns `CharacterAuthoredResourceFactoryV5` through its existing `combat_fx_particles` member. The manager, shared force world, source material metadata, GPU queue, anchors, and teardown order remain the existing V4/V5 graph. New production TUs: `character_authored_resource_v5.cpp`, `authored_fx_transform_v5.cpp`, and `authored_fx_mesh_graph_v5.cpp` in level-world.

Actual-cache linked CPU census accepts 33/45 resources over 643 source frames. All three new TUs pass strict ARM64 and x86_64 compilation. The census uses fixture camera/outer transform and does not establish live authored Use timing, damage, or GPU acceptance. All 140 parsed material passes in this inventory are transparent; supported mesh records have one primitive. No opaque or multiple-buffer behavior is claimed.

## Supported player resource families

- skill_dh2_prince_mage_cold_ray.bdae (source skill IDs 14)
- skill_dh2_prince_mage_cyclone.bdae (source skill IDs 19)
- skill_dh2_prince_mage_illusionist_eclipse.bdae (source skill IDs 30)
- skill_dh2_prince_mage_illusionist_figments.bdae (source skill IDs 45)
- skill_dh2_prince_mage_illusionist_terror.bdae (source skill IDs 110)
- skill_dh2_prince_mage_illusionist_time_control.bdae (source skill IDs 111)
- skill_dh2_prince_mage_necromancer_fire_skull_rain.bdae (source skill IDs 47)
- skill_dh2_prince_mage_necromancer_pestilence.bdae (source skill IDs 81)
- skill_dh2_prince_mage_necromancer_rise.bdae (source skill IDs 25,34,93)
- skill_dh2_prince_rogue_assassin_assassination.bdae (source skill IDs 5)
- skill_dh2_prince_rogue_assassin_blindness.bdae (source skill IDs 9)
- skill_dh2_prince_rogue_assassin_poison_sting.bdae (source skill IDs 85)
- skill_dh2_prince_rogue_jump_kick.bdae (source skill IDs 56)
- skill_dh2_prince_rogue_quickness.bdae (source skill IDs 90)
- skill_dh2_prince_rogue_roundhouse.bdae (source skill IDs 97)
- skill_dh2_prince_rogue_vicious_strike.bdae (source skill IDs 114)
- skill_dh2_prince_warrior_bash_down.bdae (source skill IDs 7)
- skill_dh2_prince_warrior_berseker_berserk.bdae (source skill IDs 8)
- skill_dh2_prince_warrior_berseker_intimidate.bdae (source skill IDs 55)
- skill_dh2_prince_warrior_berseker_whirlwind.bdae (source skill IDs 116)
- skill_dh2_prince_warrior_charge.bdae (source skill IDs 13)
- skill_dh2_prince_warrior_ground_slam.bdae (source skill IDs 33,51)
- skill_dh2_prince_warrior_paladin_divine_judgement.bdae (source skill IDs 28)

## Required resource families

- skill_dh2_monster_dark_queen2ndform_projectile.bdae: Required blood resource initialization
- skill_dh2_monster_dragon_attack_01.bdae: Blood source cloud update -1
- skill_dh2_monster_dragon_attack_02.bdae: Required authored FX mesh payload
- skill_dh2_monster_dragon_attack_03.bdae: Required authored FX mesh payload
- skill_dh2_monster_dragon_intimidate_01.bdae: Required authored FX mesh payload
- skill_dh2_monster_dragon_intimidate_02.bdae: Required authored FX mesh payload
- skill_dh2_monster_dragon_intimidate_03.bdae: Required authored FX mesh payload
- skill_dh2_monster_root_troll_charge.bdae: Required authored skinned FX mesh owner
- skill_dh2_monster_root_troll_dome_idle.bdae: Required authored skinned FX mesh owner
- skill_dh2_monster_root_troll_dome_warning.bdae: Required authored skinned FX mesh owner
- skill_dh2_monster_root_troll_idle_to_dome.bdae: Required authored skinned FX mesh owner
- skill_dh2_prince_mage_necromancer_death_claw.bdae: Required authored other animation continuation

The accepted path samples source component/default transforms, angle and quaternion tracks, material alpha/color and grouped texture transforms on one retained scene. Original factory null-track cases are skipped only by the proven source dispatch whitelist. Static birth parameters remain genuine constructor inputs when no BirthRate animation targets that emitter. Unsupported positive tracks/models remain failures; resource selection does not fall back to another effect.
