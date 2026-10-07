from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
base=(root/'port/level-world/tools/run_character_world_skill_combat_v6_host.py').read_text()
base=base.replace("'tests/character_world_skill_combat_v6_host.cpp'", "'character_world_skill_execution_v6.cpp','character_melee_animation_event_v1.cpp','character_world_ai_can_attack_v1.cpp','trophy_manager_owner_v1.cpp','character_skill_native_v6.cpp','character_skill_readonly_v6.cpp','character_skill_combat_v6.cpp','tests/character_melee_animation_event_v1_host.cpp'")
base=base.replace('character_world_skill_combat_v6_'+"'", 'character_melee_animation_event_v1_'+"'").replace('character-world-skill-combat-v6-host-audit.json','character-melee-animation-event-v1-host-audit.json')
base=base.replace("'character_skill_combat_v6.cpp'", "'character_skill_combat_v6.cpp','character_skill_mana_v5.cpp','character_target_search_v5.cpp'")
base=base.replace("'character_world_skill_execution_v6.cpp','character_melee_animation_event_v1.cpp','character_world_ai_can_attack_v1.cpp'", "'character_world_skill_execution_v6.cpp','character_melee_animation_event_v1.cpp','character_world_ai_can_attack_v1.cpp','character_world_aggro_event_v1.cpp'")
base=base.replace("'full_skill_application':False", "'scope':'Whole authored event dispatch prefix/state guards; actual cache and same Gear/CF/RNG normal/offhand melee masks; existing registered HP/aggro/Trophy prefix; positive HitFX and complete presentation tails remain required. Named dispatcher services are fixtures, not production acceptance.', 'full_skill_application':False")
exec(compile(base,str(__file__),'exec'))
