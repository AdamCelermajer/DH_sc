import pathlib,subprocess,json,hashlib
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
reports=root/'port/level-loader/reports'
base=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5590']
def adb(args):
 name=subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0]
 assert name=='DH2_Loader_API37',name
 return subprocess.check_output(base+args,timeout=45).decode(errors='replace')
remote='/data/local/tmp/dh2-loader-map-recovery'
expected='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
assert adb(['shell','sha256sum',remote+'/cache.zip']).split()[0]==expected
fixtures=reports/'connected-source-fixtures.zip'
adb(['push',str(fixtures),remote+'/connected-source-fixtures.zip'])
host=json.loads((reports/'canonical-connected-source-host.json').read_text(encoding='utf-8'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
rows=[]
for name,relative,args in [
 ('dh2_loader_connected_source_probe','dh2_loader_connected_source_probe',[remote+'/cache.zip',remote+'/connected-source-fixtures.zip']),
 ('dh2_loader_canonical_source_adapter_probe','loader/dh2_loader_canonical_source_adapter_probe',[])]:
 p=root.parent/'build/connected-owner-android-x86_64'/relative
 adb(['push',str(p),remote+'/'+name]);adb(['shell','chmod','755',remote+'/'+name])
 result=json.loads(adb(['shell',remote+'/'+name,*args]))
 match=next(row['result'] for row in host['cases'] if row['build']=='connected-owner-host' and row['probe']==relative)
 assert result==match,(result,match)
 rows.append({'probe':name,'probe_sha256':sha(p),'result':result})
 print(json.dumps({'abi':'x86_64','probe':name,'checks':result.get('composition_checks',result.get('adapter_checks'))}),flush=True)
apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
assert sha(apk)=='ef7048fcdecf30e0ce2422551639f27d1d61938843ff2c1cc46c4da29789a7e2'
arm=root.parent/'build/connected-owner-android-arm64-v8a'
receipt={'validation':'PASS','scope':'Native executable source-composition checks; no APK installation or runtime world publication',
 'serial':'emulator-5590','avd':'DH2_Loader_API37','cases':rows,
 'arm64_compile_sha256':{relative:sha(arm/relative) for relative in ['dh2_loader_connected_source_probe','loader/dh2_loader_canonical_source_adapter_probe']},
 'unchanged_map_apk_sha256':sha(apk),'cache_sha256':expected,'host_receipt_sha256':sha(reports/'canonical-connected-source-host.json'),
 'full_loader_verified':False}
(reports/'canonical-connected-source-android.json').write_text(json.dumps(receipt,indent=2)+'\n',encoding='utf-8')
