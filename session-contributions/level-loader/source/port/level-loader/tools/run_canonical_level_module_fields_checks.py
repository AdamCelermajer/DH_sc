import pathlib,subprocess,json,hashlib
root=pathlib.Path(__file__).resolve().parents[3]
reports=root/'port/level-loader/reports';draft=pathlib.Path(__file__).parent
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
oracle_path=draft/'level-constructor-fields-original.json'
oracle=json.loads(oracle_path.read_text(encoding='utf-8'));assert oracle['validation']=='PASS' and len(oracle['cases'])==8
getter_path=draft/'gslevel-current-original.json'
getter=json.loads(getter_path.read_text(encoding='utf-8'));assert getter['validation']=='PASS' and len(getter['cases'])==6
module_oracle_path=draft/'level-module-fields-original.json'
module_oracle=json.loads(module_oracle_path.read_text(encoding='utf-8'));assert module_oracle['validation']=='PASS' and len(module_oracle['cases'])==8
rows=[]
for label in ('connected-owner-host','connected-owner-sanitizers'):
 p=root.parent/'build'/label/'loader/dh2_loader_canonical_level_context_probe'
 run=subprocess.run(['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(p),linux(cache)],capture_output=True,timeout=45)
 assert run.returncode==0 and not run.stderr,(run.stdout,run.stderr)
 result=json.loads(run.stdout);assert result['validation']=='PASS' and result['level_context_checks']==18
 assert all(row['fields']==result['constructor_fields'] for row in oracle['cases'])
 rows.append({'build':label,'probe_sha256':sha(p),'result':result});print(json.dumps({'build':label,'checks':result['level_context_checks']}),flush=True)
base=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5590']
def adb(args):
 assert subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0]=='DH2_Loader_API37'
 return subprocess.check_output(base+args,timeout=45).decode(errors='replace')
remote='/data/local/tmp/dh2-loader-map-recovery'
assert adb(['shell','sha256sum',remote+'/cache.zip']).split()[0]==sha(cache)
p=root.parent/'build/connected-owner-android-x86_64/loader/dh2_loader_canonical_level_context_probe'
adb(['push',str(p),remote+'/dh2_loader_canonical_level_context_probe']);adb(['shell','chmod','755',remote+'/dh2_loader_canonical_level_context_probe'])
result=json.loads(adb(['shell',remote+'/dh2_loader_canonical_level_context_probe',remote+'/cache.zip']))
assert result==rows[0]['result'];rows.append({'build':'android-x86_64','probe_sha256':sha(p),'result':result})
print(json.dumps({'build':'android-x86_64','checks':result['level_context_checks']}),flush=True)
for name in ('level-constructor-fields-original.json','level-constructor-3f3128.asm','gslevel-current-original.json','gslevel-current-original.asm','level-module-fields-original.json'):(reports/name).write_bytes((draft/name).read_bytes())
arm=root.parent/'build/connected-owner-android-arm64-v8a/loader/dh2_loader_canonical_level_context_probe'
apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
assert sha(apk)=='ef7048fcdecf30e0ce2422551639f27d1d61938843ff2c1cc46c4da29789a7e2'
paths=[root/'port/level-loader'/n for n in ('canonical_level_context_v1.hpp','canonical_level_context_v1.cpp','tests/canonical_level_context_probe.cpp')]
receipt={'validation':'PASS','scope':'One canonical Level identity/field authority and borrowed GSLevel-global/Module-load-field fixtures; exact root Module load TU, not whole Level constructor or actual Module construction' ,
 'cases':rows,'arm64_compile_sha256':sha(arm),'source_sha256':{p.relative_to(root).as_posix():sha(p) for p in paths},
 'original_constructor_prefix_cases':8,'original_module_field_prefix_cases':8,'module_field_oracle_sha256':sha(reports/'level-module-fields-original.json'),'original_current_level_getter_cases':6,'gslevel_getter_oracle_sha256':sha(reports/'gslevel-current-original.json'),'oracle_sha256':sha(reports/'level-constructor-fields-original.json'),
 'kill_contract_sha256':sha(root/'port/level-world/character_kill.hpp'),
 'cache_sha256':sha(cache),'unchanged_map_apk_sha256':sha(apk),'full_level_constructor_verified':False,'full_loader_verified':False}
(reports/'canonical-level-module-fields-checks.json').write_text(json.dumps(receipt,indent=2)+'\n',encoding='utf-8')
