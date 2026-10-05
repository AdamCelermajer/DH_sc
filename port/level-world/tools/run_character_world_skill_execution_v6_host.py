from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
base=(root/'port/level-world/tools/run_character_world_skill_combat_v6_host.py').read_text()
base=base.replace("'tests/character_world_skill_combat_v6_host.cpp'", "'character_world_skill_execution_v6.cpp','trophy_manager_owner_v1.cpp','character_skill_native_v6.cpp','character_skill_readonly_v6.cpp','character_skill_combat_v6.cpp','tests/character_world_skill_execution_v6_host.cpp'")
base=base.replace('character_world_skill_combat_v6_'+"'", 'character_world_skill_execution_v6_'+"'").replace('character-world-skill-combat-v6-host-audit.json','character-world-skill-execution-v6-host-audit.json')
base=base.replace("'character_skill_combat_v6.cpp'", "'character_skill_combat_v6.cpp','character_skill_mana_v5.cpp','character_target_search_v5.cpp'")
base=base.replace("'character_world_skill_execution_v6.cpp'", "'character_world_skill_execution_v6.cpp','character_world_aggro_event_v1.cpp'")
exec(compile(base,str(__file__),'exec'))
