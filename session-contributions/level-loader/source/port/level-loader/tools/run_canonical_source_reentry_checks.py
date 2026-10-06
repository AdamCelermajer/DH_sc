import hashlib,json,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[3]
reports=root/'port/level-loader/reports';build=root.parent/'build'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
source_fixture=reports/'connected-source-fixtures.zip';module_fixture=build/'module-file-fixtures.zip'
probes=[('loader/dh2_loader_cached_file_reentry_probe','direct_file_reentry_checks',4,source_fixture),
        ('dh2_loader_connected_source_probe','composition_checks',12,source_fixture),
        ('loader/dh2_loader_canonical_source_adapter_probe','adapter_checks',14,None),
        ('loader/dh2_loader_canonical_module_files_probe','module_file_checks',14,module_fixture)]
rows=[]
def parse(output,key,count):
    value=json.loads(output.strip().splitlines()[-1])
    assert value['validation']=='PASS' and value[key]==count,value
    return value
for kind in ('host','sanitizers'):
    for relative,key,count,fixture in probes:
        probe=build/('connected-owner-'+kind)/relative
        args=[] if fixture is None else [linux(cache),linux(fixture)]
        run=subprocess.run(['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(probe),*args],capture_output=True,timeout=60)
        assert run.returncode==0 and not run.stderr,(relative,run.stdout,run.stderr)
        result=parse(run.stdout.decode(),key,count)
        rows.append({'build':kind,'probe':relative,'probe_sha256':sha(probe),'result':result})
        print(json.dumps({'build':kind,'probe':relative,'checks':count}),flush=True)
base=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5590']
def adb(args):
    name=subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0]
    assert name=='DH2_Loader_API37',name
    return subprocess.check_output(base+args,timeout=60).decode(errors='replace')
cache_remote='/data/local/tmp/dh2-loader-map-recovery/cache.zip'
assert adb(['shell','sha256sum',cache_remote]).split()[0]==sha(cache)
remote='/data/local/tmp/dh2-loader-source-reentry-v1'
adb(['shell','mkdir','-p',remote])
for fixture in (source_fixture,module_fixture):adb(['push',str(fixture),remote+'/'+fixture.name])
for relative,key,count,fixture in probes:
    probe=build/'connected-owner-android-x86_64'/relative;destination=remote+'/'+probe.name
    adb(['push',str(probe),destination]);adb(['shell','chmod','755',destination])
    args=[] if fixture is None else [cache_remote,remote+'/'+fixture.name]
    result=parse(adb(['shell',destination,*args]),key,count)
    assert result==next(row['result'] for row in rows if row['build']=='host' and row['probe']==relative)
    rows.append({'build':'android-x86_64','probe':relative,'probe_sha256':sha(probe),'result':result})
    print(json.dumps({'build':'android-x86_64','probe':relative,'checks':count}),flush=True)
paths=[root/'port/level-loader'/p for p in ('canonical_cached_file_v1.hpp','canonical_cached_file_v1.cpp','tests/canonical_cached_file_reentry_probe.cpp')]
receipt={'validation':'PASS','scope':'Direct canonical file callback guards and existing source/Module regression checks; no whole-world construction',
    'cases':rows,'baseline_reproduction':{'exit_code':1,'diagnostic':'recursive direct file delivery was accepted'},
    'source_sha256':{p.relative_to(root).as_posix():sha(p) for p in paths},
    'arm64_compile_sha256':{relative:sha(build/'connected-owner-android-arm64-v8a'/relative) for relative,_,_,_ in probes},
    'cache_sha256':sha(cache),'fixture_sha256':{p.name:sha(p) for p in (source_fixture,module_fixture)},'full_loader_verified':False}
(reports/'canonical-source-reentry-checks.json').write_text(json.dumps(receipt,indent=2)+'\n')
