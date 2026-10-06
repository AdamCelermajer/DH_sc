import pathlib,subprocess,json,hashlib
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc');reports=root/'port/level-loader/reports';rows=[]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for label in ('host-xml','host-sanitizers'):
    p=root.parent/'build'/label/'dh2_loader_canonical_source_adapter_probe'
    linux='/mnt/c/'+str(p).replace('\\','/')[3:]
    cmd=['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux]
    completed=subprocess.run(cmd,capture_output=True,timeout=30);assert completed.returncode==0 and not completed.stderr,completed.stderr
    result=json.loads(completed.stdout);assert result['validation']=='PASS' and result['adapter_checks']==14
    rows.append({'build':label,'probe_sha256':sha(p),'result':result});print(json.dumps(rows[-1]),flush=True)
base=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5590']
def adb(args):
    assert subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0]=='DH2_Loader_API37'
    return subprocess.check_output(base+args,timeout=30)
remote='/data/local/tmp/dh2-loader-map-recovery/dh2_loader_canonical_source_adapter_probe'
p=root/'port/android-native/app/build/intermediates/cxx/Debug/5e223t2g/obj/x86_64/dh2_loader_canonical_source_adapter_probe'
adb(['push',str(p),remote]);adb(['shell','chmod','755',remote])
result=json.loads(adb(['shell',remote]));assert result['validation']=='PASS' and result['adapter_checks']==14
rows.append({'build':'android-x86_64','serial':'emulator-5590','avd':'DH2_Loader_API37','probe_sha256':sha(p),'result':result})
assert all(row['result']==rows[0]['result'] for row in rows)
report={'validation':'PASS','scope':'Retained XML/canonical source-prefix integration fixtures, not production classes or a live world','cases':rows,'runtime_objects_verified':False,'full_loader_verified':False}
(reports/'canonical-source-adapter-checks.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(rows[-1]),flush=True)
