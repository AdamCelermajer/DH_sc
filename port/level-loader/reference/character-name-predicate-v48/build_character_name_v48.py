from pathlib import Path
import json,subprocess,hashlib,argparse
p=argparse.ArgumentParser();p.add_argument('--native',action='store_true');p.add_argument('--coherent-ready',action='store_true');a=p.parse_args();assert not a.native or a.coherent_ready,'Parent coherent READY required; no dirty archive linkage'
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/character-name-predicate-v48';v=b/'vendor/character-rng-integration-v5-loading/port';build=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host');work=build.parent/'character-name-predicate-v48';work.mkdir(exist_ok=True);root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');reports=b/'reports'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def w(p):return '/mnt/c/'+str(p).replace('\\','/')[3:]
def run(cmd,log):
 q=subprocess.run(cmd,capture_output=True,text=True);log.write_text(q.stdout+q.stderr);print((q.stdout+q.stderr)[-9000:],flush=True);assert q.returncode==0,str(log);return q
dirs=[r,v/'level-world',v/'level-loader',b]+[p for p in v.iterdir() if p.is_dir() and p.name not in ['level-world','level-loader']]
flags=['-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-UNDEBUG']+['-I'+w(p) for p in dirs]
tests=[r/'character_name_predicate_v48_test.cpp',r/'character_spawn_name_v48_regression.cpp',r/'character_model_name_v48_regression.cpp']
if not a.native:
 run(['wsl.exe','-d','Ubuntu','--','c++']+flags+['-fsyntax-only',w(r/'retained_character_actor_v1.cpp'),w(r/'character_is_player_v41.cpp')]+[w(p) for p in tests],reports/'character-name-predicate-v48-syntax.log');print('PASS V48 NAME-only candidate +actualfactory20 +spawn8 +model regression syntax; no archive link');raise SystemExit
snapshot=b/'reference/character-model-route-v45/genuine-save-snapshot'
assert sha(snapshot/'player_savegame_v1.hpp')==sha(v/'game-data/player_savegame_v1.hpp')=='014acd3b821b4ab99b7fbd2072a8df4132465d783917e3b9592e58e8aeda0f6f'
assert sha(snapshot/'player_savegame_v1.cpp')=='ca713e12bd25307bca7b24c95714cec45bda1e198fdbe70e952315659ef91d43'
assert sha(root/'port/game-data/faery_tables.hpp')==sha(v/'game-data/faery_tables.hpp')
selected={str(v/'level-world'/n):sha(v/'level-world'/n) for n in ['retained_character_actor_v1.hpp','retained_character_actor_v1.cpp','character_is_player_v41.hpp','character_is_player_v41.cpp','canonical_character_family_v4.hpp','canonical_character_family_v4.cpp','character_model_route_v45.hpp','character_model_route_v45.cpp']}
sources=[r/'retained_character_actor_v1.cpp',r/'canonical_character_family_v4.cpp',r/'character_is_player_v41.cpp',r/'character_model_route_v45.cpp',v/'level-world/character_model_name_v38.cpp',root/'port/level-world/game_object_spawn_probability_v1.cpp',snapshot/'player_savegame_v1.cpp',root/'port/game-data/faery_tables.cpp']
archives=sorted(build.rglob('*.a'));assert archives
fingerprint=hashlib.sha256(json.dumps({'candidate':{str(p):sha(p) for p in r.glob('*.hpp')},'selected':selected,'selected_headers':{str(p):sha(p) for p in v.rglob('*.hpp')},'flags':flags},sort_keys=True).encode()).hexdigest()
def object_for(source):
 digest=hashlib.sha256((fingerprint+sha(source)).encode()).hexdigest();out=work/(source.stem+'-'+digest[:16]+'.o')
 if not out.exists():run(['wsl.exe','-d','Ubuntu','--','c++']+flags+['-O1','-g','-ffunction-sections','-fdata-sections','-c',w(source),'-o',w(out)],reports/(source.stem+'-v48-build.log'))
 return out
objects=[object_for(p) for p in sources];output=[];linkmaps={}
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');design=b/'tests/native-ctor-v4/design.bin'
for test in tests:
 own=object_for(test);binary=work/test.stem;linkmap=reports/(test.stem+'.map')
 run(['wsl.exe','-d','Ubuntu','--','c++']+[w(p) for p in objects+[own]]+['-Wl,--gc-sections','-Wl,-Map='+w(linkmap),'-lstdc++','-Wl,--start-group']+[w(p) for p in archives]+['-Wl,--end-group',w(build/'libdh2_script_runtime.so'),'-Wl,-rpath,'+w(build),'-lz','-o',w(binary)],reports/(test.stem+'-link.log'))
 for bad in ['(retained_character_actor_v1.cpp.o)','(canonical_character_family_v4.cpp.o)','(character_is_player_v41.cpp.o)']:assert bad not in linkmap.read_text(),bad
 q=run(['wsl.exe','-d','Ubuntu','--',w(binary),w(cache),w(design)],reports/(test.stem+'-native.log'));output.append({'test':str(test),'test_sha256':sha(test),'native_output':q.stdout,'binary_sha256':sha(binary),'link_map_sha256':sha(linkmap)})
assert all(sha(Path(p))==h for p,h in selected.items()),'Selected graph changed during candidate proof'
receipt={'schema':'dh2-character-name-predicate-v48-verified','status':'PASS actual-factory20 name-vs-gametype +livechanges20; spawn8 and ordered model regressions','actual_name30_buffer44':True,'actual_archetype48_buffer5c_excluded':True,'native_tests':output,'sources':{str(p):sha(p) for p in sources},'selected_sources_unchanged':selected,'no_archived_actor_factory_predicate':True,'explicit_fixtures':'Predicate inputs select existing actual AI rows in the same resolved-sheet field. Spawn offline/lifecycle observer and model Debug/difficultyDEFAULT/master-save test-domain fixtures remain explicit. No whole activation/player-save/equipment claim.','whole_InitPost_success':False,'selected_writes':False,'emulator_launched':False,'native_runner_sha256':sha(Path(__file__))}
(reports/'character-name-predicate-v48-verified.json').write_text(json.dumps(receipt,indent=2)+'\n');print('PASS V48 three native tests; selected source unchanged; original NAME field correction proven')
