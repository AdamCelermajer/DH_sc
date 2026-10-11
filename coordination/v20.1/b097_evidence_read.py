from pathlib import Path
import json

repo = Path(__file__).resolve().parents[2]
text = (repo / '.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c').read_text(encoding='utf-8', errors='replace')
for token in ['/* FUNCTION 003b7eac ', '/* FUNCTION 003babdc ', '/* FUNCTION 003bdef4 ', '/* FUNCTION 003e2d4c ', '/* FUNCTION 00445a10 ']:
    start = text.index(token)
    end = text.find('/* FUNCTION ', start + 4)
    block = text[start:end]
    if '00445a10' in token:
        lines = block.splitlines()
        for i, line in enumerate(lines):
            if any(t in line for t in ['GetSkillLevel', 'AI_SkillInfo', 'CanIncrementSkill', 'GetCharSkill', 'MaxSkillLevel']):
                print('\n'.join(lines[max(0, i-3):i+8]))
    else:
        print(block)
data = json.loads((repo / 'port/level-world/reference/character-skill-state-v4/skill-lua-v46.json').read_text())
print('JSON ROOT', type(data).__name__)
if isinstance(data, dict):
    for key, value in data.items():
        if isinstance(value, dict) and 'bashdown' in str(value.get('script', key)).lower():
            print(key, value)
        elif 'bashdown' in key.lower():
            print(key, value)
elif isinstance(data, list):
    for value in data:
        if 'bashdown' in str(value).lower():
            print(value)
