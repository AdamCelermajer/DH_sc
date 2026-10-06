from pathlib import Path
import subprocess,shutil,json,hashlib
base=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader').resolve()
build=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host')
temp=Path('C:/Users/adamc/AppData/Local/Temp');ref=base/'reference/character-loader-fields-v38';candidate=ref/'actor-candidate-v38';v39=base/'reference/character-template-binding-v39';v39.mkdir(exist_ok=True)
def w(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
vendor=base/'vendor/character-rng-integration-v5-loading/port'
includes=[candidate,ref,v39,base]+[p for p in vendor.iterdir() if p.is_dir()]+[vendor/'physics-backend/box2d-2.0.1/Include']
archives=list(build.rglob('*.a'));assert archives and all('v5' in str(p) for p in archives)
flags=['-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-UNDEBUG','-ffunction-sections','-fdata-sections']+['-I'+w(p) for p in includes]
receipt={'scope':'Independent FULL candidate actual actor implementation; V5 non-actor dependencies only. Not coherent factory rebuild.','cases':{}}
for name,test,args,extra in [('character-actor-fields-v38',ref/'character_actor_fields_v38_test.cpp',[],[]),('character-template-frame-v39',v39/'character_template_frame_v39_test.cpp',[base/'reference/character-templates-v35'],[base/'character_template_assets_v35.cpp'])]:
 out=build.parent/name;linkmap=base/'reports'/f'{name}.map'
 cmd=['wsl.exe','-d','Ubuntu','--','c++']+flags+[w(candidate/'retained_character_actor_v1.cpp'),w(test)]+[w(p) for p in extra]+['-Wl,--gc-sections','-Wl,-Map='+w(linkmap),'-lstdc++','-Wl,--start-group']+[w(p) for p in archives]+['-Wl,--end-group',w(build/'libdh2_script_runtime.so'),'-Wl,-rpath,'+w(build),'-lz','-o',w(out)]
 r=subprocess.run(cmd,capture_output=True,text=True);(base/'reports'/f'{name}-build.log').write_text(r.stdout+r.stderr);print(name,'BUILD',r.returncode,r.stdout+r.stderr,flush=True);assert r.returncode==0
 m=linkmap.read_text();forbidden=['(retained_character_actor_v1.cpp.o)','(canonical_character_family_v4.cpp.o)'];assert not any(s in m for s in forbidden),[s for s in forbidden if s in m]
 r=subprocess.run(['wsl.exe','-d','Ubuntu','--',w(out)]+[w(p) for p in args],capture_output=True,text=True);print(name,r.returncode,r.stdout+r.stderr);assert r.returncode==0
 receipt['cases'][name]={'pass':True,'output':r.stdout,'binary_sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'link_map_sha256':hashlib.sha256(linkmap.read_bytes()).hexdigest(),'old_actor_object_not_linked':True,'old_factory_object_not_linked':True}
receipt['files']={str(p.relative_to(base)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [candidate/'retained_character_actor_v1.hpp',candidate/'retained_character_actor_v1.cpp',ref/'character_actor_fields_v38_test.cpp',v39/'character_template_frame_v39.hpp',v39/'character_template_frame_v39_test.cpp']}
(base/'reports/character-actual-binding-v39-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
