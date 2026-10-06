from pathlib import Path
import hashlib,json,zipfile
root=Path(__file__).resolve().parents[3]
dest=root/'port/level-world/reference/object-save-restore-v3'
files=[f'port/level-world/{name}{ext}' for name in ['object_save_restore_v3','character_save_restore_v3','retained_character_save_connection_v3'] for ext in ['.hpp','.cpp']]
files += ['port/level-world/tests/object_save_restore_v3_native.cpp','port/level-world/tools/produce_object_save_restore_v3.py','port/level-world/tools/produce_character_save_metadata_v3.py']
files += [p.relative_to(root).as_posix() for p in dest.iterdir() if p.suffix in ['.json','.hpp','.asm','.md'] and p.name!='source-manifest.json']
files=sorted(set(files))
manifest={'scope':'Source virtual Save10/Load14 and retained connection; typed lifecycle services remain required. No live restore acceptance.','files':[{'path':p,'sha256':hashlib.sha256((root/p).read_bytes()).hexdigest(),'bytes':(root/p).stat().st_size}for p in files]}
(dest/'source-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(dest/'handoff.zip','w',zipfile.ZIP_DEFLATED)as z:
 for p in files:z.write(root/p,p)
 z.write(dest/'source-manifest.json','port/level-world/reference/object-save-restore-v3/source-manifest.json')
print(json.dumps({'entries':len(files),'zip_sha256':hashlib.sha256((dest/'handoff.zip').read_bytes()).hexdigest()}))
