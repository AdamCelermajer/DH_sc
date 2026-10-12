from pathlib import Path
import subprocess,hashlib,json,argparse
p=argparse.ArgumentParser();p.add_argument('--native',action='store_true');p.add_argument('--coherent-ready',action='store_true');p.add_argument('--only-character',action='store_true');a=p.parse_args();assert not a.native or a.coherent_ready,'Parent751 READY required'
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');r=b/'reference/character-parent-kernel-v51';v=b/'vendor/character-rng-integration-v5-loading/port';build=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/build/receiver-transport-v5-host');reports=b/'reports';root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();w=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
dirs=[r,v/'level-world',v/'level-loader',b]+[x for x in v.iterdir() if x.is_dir() and x.name not in ['level-world','level-loader']];flags=['-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-UNDEBUG']+['-I'+w(x) for x in dirs]
tests=[r/'parent_kernel_control_v51.cpp',r/'parent_character_model_v51.cpp'];sources=[r/'game_object_initialization_owner_v1.cpp',r/'condition_data_init_v3.cpp',v/'level-world/character_set_position_v7.cpp',root/'port/level-world/game_object_spawn_probability_v1.cpp']
def run(cmd,log):
 q=subprocess.run(cmd,capture_output=True,text=True);log.write_text(q.stdout+q.stderr);print((q.stdout+q.stderr)[-10000:],flush=True);assert q.returncode==0,str(log);return q
if not a.native:
 run(['wsl.exe','-d','Ubuntu','--','c++']+flags+['-fsyntax-only']+[w(p) for p in sources+tests],reports/'character-parent-kernel-v51-tests-syntax.log');print('PASS V51 two actual-control/model continuation tests syntax; no archives');raise SystemExit
selected={str(v/'level-world'/n):sha(v/'level-world'/n) for n in ['retained_character_actor_v1.hpp','retained_character_actor_v1.cpp','character_parent_init_fields_v48.hpp','canonical_character_family_v4.cpp','game_object_initialization_owner_v1.hpp','game_object_initialization_owner_v1.cpp','condition_data_init_v3.hpp','condition_data_init_v3.cpp']};archives=sorted(build.rglob('*.a'));assert archives
snapshot=b/'reference/character-model-route-v45/genuine-save-snapshot'
assert sha(snapshot/'player_savegame_v1.hpp')==sha(v/'game-data/player_savegame_v1.hpp')=='014acd3b821b4ab99b7fbd2072a8df4132465d783917e3b9592e58e8aeda0f6f'
assert sha(snapshot/'player_savegame_v1.cpp')=='ca713e12bd25307bca7b24c95714cec45bda1e198fdbe70e952315659ef91d43'
assert sha(root/'port/game-data/faery_tables.hpp')==sha(v/'game-data/faery_tables.hpp')
results=[]
for test in tests:
 if a.only_character and test.stem=='parent_kernel_control_v51':
  log=reports/(test.stem+'-native.log');out=build.parent/test.stem;linkmap=reports/(test.stem+'.map');assert 'PASS' in log.read_text();results.append({'test':str(test),'sha256':sha(test),'stdout':log.read_text(),'binary_sha256':sha(out),'link_map_sha256':sha(linkmap)});continue
 extra=[] if test.stem=='parent_kernel_control_v51' else [snapshot/'player_savegame_v1.cpp',root/'port/game-data/faery_tables.cpp']
 out=build.parent/test.stem;linkmap=reports/(test.stem+'.map')
 run(['wsl.exe','-d','Ubuntu','--','c++']+flags+['-O1','-g','-ffunction-sections','-fdata-sections']+[w(p) for p in sources+extra+[test]]+['-Wl,--gc-sections','-Wl,-Map='+w(linkmap),'-lstdc++','-Wl,--start-group']+[w(p) for p in archives]+['-Wl,--end-group',w(build/'libdh2_script_runtime.so'),'-Wl,-rpath,'+w(build),'-lz','-o',w(out)],reports/(test.stem+'-build.log'))
 for bad in ['(game_object_initialization_owner_v1.cpp.o)','(condition_data_init_v3.cpp.o)']:assert bad not in linkmap.read_text(),bad
 args=[] if test.stem=='parent_kernel_control_v51' else ['/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip',w(b/'tests/native-ctor-v4/design.bin')]
 q=run(['wsl.exe','-d','Ubuntu','--',w(out)]+args,reports/(test.stem+'-native.log'));results.append({'test':str(test),'sha256':sha(test),'stdout':q.stdout,'binary_sha256':sha(out),'link_map_sha256':sha(linkmap)})
assert all(sha(Path(p))==h for p,h in selected.items()),'Selected source changed during candidate proof'
receipt={'schema':'dh2-character-parent-shared-kernel-v51-verified','validation':'PASS shared original control +actual ordered Character parent prefix','tests':results,'candidate_sources':{str(p):sha(p) for p in r.glob('*') if p.suffix in ['.hpp','.cpp']},'selected_sources_unchanged':selected,'no_old_generic_initialization_condition_objects':True,'no_second_base_pose_visual_save':True,'source_halfword_Character_storage_preserved':True,'actual_Character_prefix':'actual NPC ordered existing model then shared ConditionData/cache/SetPosition; required actual Device stop','whole_Character_InitPost_success':False,'compiled_condition_allocated':False,'successful_visual_callbacks':False,'controlled_NULLvisual_inputs_are_not_first_SWAMP':True,'emulator_launched':False}
(reports/'character-parent-kernel-v51-verified.json').write_text(json.dumps(receipt,indent=2)+'\n');(r/'build_parent_kernel_v51.py').write_bytes(Path(__file__).read_bytes());print('PASS V51 surgical shared control proposal; no selected writes')
