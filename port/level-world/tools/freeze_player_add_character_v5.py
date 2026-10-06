from pathlib import Path
import json,hashlib,zipfile
root=Path(__file__).resolve().parents[3];out=root/'port/level-world/reference/player-add-character-v5'
source=(root/'port/level-world/reference/player-manager-owner-v1/original-source.asm').read_text()
section=next(s for s in source.split('\n\n')if s.startswith('# 0x372220 '))
(out/'original-add-character.asm').write_text(section+'\n')
files=['port/level-world/player_add_character_owner_v5.hpp','port/level-world/player_add_character_owner_v5.cpp','port/level-world/tests/player_add_character_v5_native.cpp','port/level-world/tools/produce_player_add_character_v5.py']
files += [p.relative_to(root).as_posix()for p in out.iterdir()if p.suffix in ['.md','.json','.hpp','.asm']and p.name!='source-manifest.json']
files=sorted(set(files));manifest={'scope':'Whole AddCharacter control order and explicitly staged development post-publication continuation; actual positive lifecycle services still required.','files':[{'path':p,'sha256':hashlib.sha256((root/p).read_bytes()).hexdigest(),'bytes':(root/p).stat().st_size}for p in files]}
(out/'source-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(out/'handoff.zip','w',zipfile.ZIP_DEFLATED)as z:
 for p in files:z.write(root/p,p)
 z.write(out/'source-manifest.json','port/level-world/reference/player-add-character-v5/source-manifest.json')
print(json.dumps({'entries':len(files),'sha256':hashlib.sha256((out/'handoff.zip').read_bytes()).hexdigest()}))
