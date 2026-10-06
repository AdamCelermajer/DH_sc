import pathlib,subprocess,json,hashlib
root=pathlib.Path(__file__).resolve().parents[3];draft=pathlib.Path(__file__).parent
assets=draft/'scene-v3-chest-assets';manifest=json.loads((assets/'manifest.json').read_text(encoding='utf-8'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
expected='Actual chest scene-root/visibility/animator removal PASS models=3; assetread and PF are declared boundary providers'
rows=[]
for kind in ('host','sanitizers'):
 p=root.parent/'build'/('scene-v3-'+kind)/'dh2_loader_scene_v3_probe'
 run=subprocess.run(['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(p),linux(assets)],capture_output=True,text=True,timeout=45)
 assert run.returncode==0 and not run.stderr and run.stdout.strip()==expected,(run.returncode,run.stdout,run.stderr)
 rows.append({'build':kind,'binary_sha256':sha(p),'output':run.stdout.strip(),'models':3});print(json.dumps({'build':kind,'validation':'PASS','models':3}),flush=True)
adb_base=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5590']
def adb(args):
 assert subprocess.check_output(adb_base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0]=='DH2_Loader_API37'
 return subprocess.check_output(adb_base+args,timeout=45).decode(errors='replace')
remote='/data/local/tmp/dh2-loader-scene-v3';adb(['shell','mkdir','-p',remote])
for name,row in manifest['entries'].items():
 p=assets/name;assert sha(p)==row['sha256'];adb(['push',str(p),remote+'/'+name]);assert adb(['shell','sha256sum',remote+'/'+name]).split()[0]==row['sha256']
p=root.parent/'build/scene-v3-x86_64/dh2_loader_scene_v3_probe';adb(['push',str(p),remote+'/probe']);adb(['shell','chmod','755',remote+'/probe'])
result=adb(['shell',remote+'/probe',remote]);assert result.strip()==expected,result
rows.append({'build':'android-x86_64','binary_sha256':sha(p),'output':result.strip(),'models':3});print(json.dumps({'build':'android-x86_64','validation':'PASS','models':3}),flush=True)
arm=root.parent/'build/scene-v3-arm64-v8a/dh2_loader_scene_v3_probe';apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
assert sha(apk)=='ef7048fcdecf30e0ce2422551639f27d1d61938843ff2c1cc46c4da29789a7e2'
snapshot=root/'port/level-loader/vendor/scene-connected-ef63b7f2573b1c94'
frozen=json.loads((snapshot/'snapshot-manifest.json').read_text(encoding='utf-8'))
for name,row in frozen['entries'].items():assert sha(snapshot/name)==row['sha256'],name
receipt={'validation':'PASS','scope':'Exact retained scene V3 ownership/visibility/animation removal/PF failure cleanup with real original3 chest assets; not authored SWAMP objects or visible rendered level',
 'cases':rows,'arm64_compile_sha256':sha(arm),'snapshot_manifest_sha256':sha(snapshot/'snapshot-manifest.json'),'chest_asset_manifest':manifest,
 'lifecycle_header_sha256':sha(root/'port/level-loader/vendor/scene-lifecycle-header-85cd574bb8f03bcd/character_world_npc_object_v1.hpp'),
 'standard_header_compile_correction':'-include stdexcept on immutable retained_gameobject_visual_v1.cpp only',
 'cmake_sha256':sha(root/'port/level-loader/tests/cmake-scene-v3/CMakeLists.txt'),'unchanged_map_apk_sha256':sha(apk),
 'asset_read_fixture':True,'pf_fixture':True,'scene_registration_fixture':False,'animator_removal_fixture':False,
 'world_construction_verified':False,'visible_swamp_chests_verified':False,'full_loader_verified':False}
(root/'port/level-loader/reports/scene-v3-private-checks.json').write_text(json.dumps(receipt,indent=2)+'\n',encoding='utf-8')
