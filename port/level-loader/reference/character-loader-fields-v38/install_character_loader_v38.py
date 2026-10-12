from pathlib import Path
import json,shutil,subprocess,sys,hashlib,zipfile,struct
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader').resolve();reference=base/'reference/character-loader-fields-v38';temp=Path('C:/Users/adamc/AppData/Local/Temp');reference.mkdir(parents=True,exist_ok=True)
for name in ['prove_character_loader_fields_v38.py','character_model_name_v38.hpp','character_model_name_v38.cpp','character_model_name_v38_test.cpp','capture_character_loader_v38.py','capture_character_master_producers_v38.py']:
 p=reference/name;assert p.resolve().is_relative_to(base);shutil.copyfile(temp/name,p)
subprocess.run([sys.executable,str(reference/'prove_character_loader_fields_v38.py'),'--output',str(reference/'character-loader-fields-original-v38.json')],check=True)
oracle=json.loads((reference/'character-loader-fields-original-v38.json').read_text());lines=[]
for case in oracle['model_cases']:
 f=case['fixture'];lines.append('\t'.join(str(f[k]) for k in ['model','faery','player','high','local'])+'\t'+str(int(bool(f['owner'])))+'\t'+str(f['override'])+'\t'+str(f['class'])+'\t'+str(f['saved_current'])+'\t'+(case['result'] or '<NULL>'))
(reference/'model-name-original-gold-v38.tsv').write_bytes(('\n'.join(lines)+'\n').encode())
z=zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');raw=z.read('com.gameloft.android.GAND.GloftD2SS/files/data/pydata/character_models_dictionary_pyarray.bin');at=4;files=[]
for _ in range(struct.unpack_from('<i',raw,0)[0]):size=struct.unpack_from('<i',raw,at)[0];at+=4;files.append(raw[at:at+size].decode());at+=size
assert at==len(raw);(reference/'model-files-original-v38.txt').write_bytes(('\n'.join(files)+'\n').encode())
overlay=base/'vendor/character-rng-integration-v4/port/game-data';output=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/character-model-name-v38-probe')
def w(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
cmd=['wsl.exe','-d','Ubuntu','--','c++','-std=c++17','-O1','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-UNDEBUG','-I'+w(overlay),w(reference/'character_model_name_v38.cpp'),w(reference/'character_model_name_v38_test.cpp'),'-o',w(output)]
r=subprocess.run(cmd,capture_output=True,text=True);(base/'reports/character-model-name-v38-build.log').write_text(r.stdout+r.stderr);print(r.stdout+r.stderr);assert r.returncode==0
r=subprocess.run(['wsl.exe','-d','Ubuntu','--',w(output),w(reference/'model-files-original-v38.txt'),w(reference/'model-name-original-gold-v38.tsv')],capture_output=True,text=True);print(r.stdout+r.stderr);assert r.returncode==0
receipt={'validation':'PASS','scope':'New stateless model selection adapter; field/predicate/save providers are explicit source-domain fixtures. Actual retained actor field patch is proposed only, not integrated.','original_cases':len(oracle['model_cases']),'native_output':r.stdout,'binary_sha256':hashlib.sha256(output.read_bytes()).hexdigest(),'existing_actor_sources_changed':False,'active_loader_integrated':False,'emulator_launched':False,'files':{str(p.relative_to(base)):hashlib.sha256(p.read_bytes()).hexdigest() for p in reference.iterdir() if p.is_file()}}
(base/'reports/character-loader-fields-v38-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
