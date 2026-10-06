import hashlib,json,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[3];reports=root/'port/level-loader/reports';build=root.parent/'build'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
fixture=reports/'receiver-transport-fixtures.zip';name='dh2_loader_receiver_transport_probe'
def parse(output):
    result=json.loads(output.strip().splitlines()[-1])
    assert result['validation']=='PASS' and result['receiver_transport_checks']==15 and result['catalog_entries_forwarded']==33
    assert result['class_construction_fixtures'] and not result['full_loader_verified']
    return result
rows=[]
for kind in ('host','sanitizers'):
    probe=build/('receiver-transport-'+kind)/'loader'/name
    run=subprocess.run(['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(probe),linux(cache),linux(fixture)],capture_output=True,timeout=60)
    assert run.returncode==0 and not run.stderr,(run.stdout,run.stderr)
    result=parse(run.stdout.decode());rows.append({'build':kind,'probe_sha256':sha(probe),'result':result})
    print(json.dumps({'build':kind,'checks':15}),flush=True)
base=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5590']
def adb(args):
    avd=subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0]
    assert avd=='DH2_Loader_API37',avd
    return subprocess.check_output(base+args,timeout=60).decode(errors='replace')
cache_remote='/data/local/tmp/dh2-loader-map-recovery/cache.zip'
assert adb(['shell','sha256sum',cache_remote]).split()[0]==sha(cache)
remote='/data/local/tmp/dh2-loader-receiver-transport-v1';adb(['shell','mkdir','-p',remote])
probe=build/'receiver-transport-x86_64/loader'/name
adb(['push',str(probe),remote+'/probe']);adb(['push',str(fixture),remote+'/fixtures.zip']);adb(['shell','chmod','755',remote+'/probe'])
result=parse(adb(['shell',remote+'/probe',cache_remote,remote+'/fixtures.zip']));assert result==rows[0]['result']
rows.append({'build':'android-x86_64','probe_sha256':sha(probe),'result':result});print(json.dumps({'build':'android-x86_64','checks':15}),flush=True)
snapshot=root/'port/level-loader/vendor/receiver-transport-owners-d4ff4a071e22a6db';manifest=json.loads((snapshot/'manifest.json').read_text())
for file,row in manifest['files'].items():assert sha(snapshot/file)==row['sha256'],file
paths=[root/'port/level-loader'/p for p in ('canonical_receiver_transport_v1.hpp','canonical_receiver_transport_v1.cpp','tests/canonical_receiver_transport_probe.cpp')]
receipt={'validation':'PASS','scope':'Generic catalog constructor/retained receiver transport against exact current root canonical owners; explicit receiver/field/continuation/network/delete fixtures',
    'cases':rows,'arm64_compile_sha256':sha(build/'receiver-transport-arm64-v8a/loader'/name),'source_sha256':{p.relative_to(root).as_posix():sha(p) for p in paths},
    'owner_snapshot':snapshot.relative_to(root).as_posix(),'owner_manifest_sha256':sha(snapshot/'manifest.json'),
    'cache_sha256':sha(cache),'fixture_sha256':sha(fixture),'unfiltered_swamp_first_type':'LevelConfig','unfiltered_swamp_factory_prefix':'empty',
    'class_construction_verified':False,'full_loader_verified':False}
(reports/'canonical-receiver-transport-checks.json').write_text(json.dumps(receipt,indent=2)+'\n')
