"""Freeze V4 additions without modifying the existing V3 implementation."""
import hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
FILES=[
 'port/level-world/character_skill_state_v4.hpp',
 'port/level-world/character_skill_state_v4.cpp',
 'port/level-world/character_skill_context_v4.hpp',
 'port/level-world/character_skill_context_v4.cpp',
 'port/level-world/tests/character_skill_state_v4_differential.py',
 'port/level-world/tests/character_skill_anim_event_v4_differential.py',
 'port/level-world/tests/character_skill_state_v4_host.cpp',
 'port/level-world/reference/character-skill-state-v4/gold-v4.json',
 'port/level-world/reference/character-skill-state-v4/anim-event-gold-v4.json',
 'port/level-world/reference/character-skill-state-v4/NOTES.md',
 'port/level-world/reports/character-skill-state-v4-arm64-differential.json',
 'port/level-world/reports/character-skill-anim-event-v4-arm64-differential.json',
 'port/level-world/tools/build_character_skill_state_v4_oracle.ps1',
 'port/level-world/tools/freeze_character_skill_state_v4.py',
 'port/level-world/tools/run_character_skill_state_v4_host.py',
 'port/level-world/reports/character-skill-state-v4-host-audit.json',
]
if __name__=='__main__':
 records={name:dict(bytes=(ROOT/name).stat().st_size,sha256=hashlib.sha256((ROOT/name).read_bytes()).hexdigest())for name in FILES}
 out=ROOT/'port/level-world/reference/character-skill-state-v4/freeze-manifest-v4.json'
 out.write_text(json.dumps(dict(version=4,scope='Borrowed V3 owner adapter; source CSSkill kernel and state6 animation use only. Required live gameplay providers remain external.',files=records),indent=2)+'\n')
 print(f'Frozen {len(records)} V4 files')
