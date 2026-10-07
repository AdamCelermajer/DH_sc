from pathlib import Path
import hashlib,json,zipfile
root=Path(__file__).resolve().parents[3];out=root/'port/level-world/reference/character-kill-live-v21'
files=[f'port/level-world/{name}{suffix}'for name in ['character_kill_fields_v21','character_kill_live_v21']for suffix in ['.hpp','.cpp']]
files+=['port/android-native/app/src/main/cpp/renderer_character_kill_live_v21.inc','port/level-world/tests/character_kill_live_v21_native.cpp','port/level-world/tools/produce_character_kill_live_v21.py']
files+=[p.relative_to(root).as_posix()for p in out.iterdir()if p.suffix in ['.md','.hpp','.json']and p.name!='source-manifest.json']
files=sorted(set(files))
manifest={'scope':'Source-only live Kill preparation; no installed Hit8 hook or complete Level/loot/XP/quest acceptance.','production_dependencies':['combat_ctrl_kill_owner_v1.cpp','character_kill.cpp','character_world_runtime_v1.cpp','player_manager_owner_v1.cpp','properties.cpp','character_target_providers.cpp'],'files':[{'path':p,'sha256':hashlib.sha256((root/p).read_bytes()).hexdigest(),'bytes':(root/p).stat().st_size}for p in files]}
(out/'source-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(out/'handoff.zip','w',zipfile.ZIP_DEFLATED)as z:
 for p in files:z.write(root/p,p)
 z.write(out/'source-manifest.json','port/level-world/reference/character-kill-live-v21/source-manifest.json')
print(json.dumps({'entries':len(files),'sha256':hashlib.sha256((out/'handoff.zip').read_bytes()).hexdigest()}))
