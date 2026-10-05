"""Isolated private snapshot of current SAN DSOs, with new V5 source only.
No rebuild or source attribution for historical central dependencies.
"""
import hashlib,json,os,shutil,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
OUT=ROOT/'.local-inputs/character-skill-native-v5-host';OUT.mkdir(parents=True,exist_ok=True)
SNAP=OUT/'snapshot';SNAP.mkdir(exist_ok=True)
WP='/mnt/'+ROOT.drive[0].lower()+ROOT.as_posix()[2:]
WO=WP+'/.local-inputs/character-skill-native-v5-host';WS=WO+'/snapshot'
BUILD='/home/adampalace/dh2-world-build'
LIBS=['libdh2_level_world.so','game-data/libdh2_game_data.so','script-runtime/libdh2_script_runtime.so','engine-ui/libdh2_engine_ui.so','engine-skinning/libdh2_engine_skinning.so','engine-skinning/engine-animation/libdh2_engine_animation.so','engine-skinning/engine-animation/scene-materials/libdh2_scene_materials.so','libdh2_zip_asset_pack_v1.a']
commands=[]
def run(args):
 result=subprocess.run(['wsl.exe','--exec',*args],capture_output=True,text=True);commands.append(dict(args=args,exit_code=result.returncode,stdout=result.stdout,stderr=result.stderr))
 if result.returncode:raise RuntimeError(result.stderr or result.stdout)
 return result.stdout
before={name:run(['sha256sum',BUILD+'/'+name]).split()[0]for name in LIBS}
for name in LIBS:run(['cp',BUILD+'/'+name,WS+'/'+Path(name).name])
after={name:run(['sha256sum',BUILD+'/'+name]).split()[0]for name in LIBS};assert before==after,'Central closure moved while snapshotting'
snapshot={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in SNAP.iterdir()if p.is_file()}
assert all(snapshot[Path(n).name]==h for n,h in before.items())
sources=['port/level-world/character_skill_mana_v5.cpp','port/level-world/character_skill_native_v5.cpp','port/level-world/character_target_search_v5.cpp','port/level-world/tests/character_skill_native_v5_host.cpp','port/game-data/fresh_inventory_owned_v4.cpp']
args=['g++','-std=c++17','-O2','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*[WP+'/'+s for s in sources],'-L'+WS,'-Wl,-rpath,'+WS,'-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_zip_asset_pack_v1','-lz','-o',WO+'/audit']
run(args)
env=['env','LD_LIBRARY_PATH='+WS,'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
ldd=run([*env,'ldd',WO+'/audit']);assert all(WS+'/'+Path(n).name in ldd for n in LIBS if n.endswith('.so')),'Private DSO closure not selected'
gold=WP+'/port/level-world/reference/character-game-design/real-cache-inputs.bin'
assets=WP+'/port/android-native/app/src/main/assets';cache=WP+'/.local-inputs/character-skill-session-v2/cache'
zipfile='/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'
text=run([*env,WO+'/audit',gold,assets,cache,zipfile,WP+'/.local-inputs/items-discovery'])
receipt=json.loads(text.strip().splitlines()[-1]);assert receipt['validation']=='PASS'
search_cpp=WP+'/port/level-world/character_target_search_v5.cpp'
search_results={}
for name in ['character_target_search_v5_corpus','character_target_search_v5_host']:
 run(['g++','-std=c++17','-O2','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',search_cpp,WP+'/port/level-world/tests/'+name+'.cpp','-I'+WP+'/port/level-world','-L'+WS,'-Wl,-rpath,'+WS,'-ldh2_level_world','-o',WO+'/'+name])
 argv=[*env,WO+'/'+name]
 if name.endswith('corpus'):argv.append(WP+'/port/level-world/reference/character-target-search/search-fixtures.bin')
 search_results[name]=run(argv).strip()
after_snapshot={p.name:hashlib.sha256(p.read_bytes()).hexdigest()for p in SNAP.iterdir()if p.is_file()};assert snapshot==after_snapshot
report=dict(validation='PASS',scope=__doc__,sanitizers=['address','undefined'],leak_detection=True,results=receipt,search_results=search_results,central_binary_snapshot=before,private_snapshot=snapshot,ldd=ldd,commands=commands,source_sha256={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest()for s in sources},dependency_source_attribution=False)
(ROOT/'port/level-world/reports/character-skill-native-v5-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(receipt))
