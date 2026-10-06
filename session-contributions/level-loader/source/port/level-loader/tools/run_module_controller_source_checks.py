import hashlib,json,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader';reports=loader/'reports';build=root.parent/'build'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip');assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
name='dh2_loader_module_graph_source_probe'
def parse(output):
    value=json.loads(output.strip().splitlines()[-1]);assert value['validation']=='PASS'
    for key,count in {'original_mlx_declarations':10,'actual_modules':9,'floor_clones':16,'exits':19,'generated_zones':9,'actual_init_final':9,'same_root_controllers':9,'controllers_released':9}.items():assert value[key]==count
    assert value['reserved_null_key0']
    assert value['first_mgp_missing_type']=='OpenableContainer' and value['outer_provider_fixtures'] and not value['full_loader_verified']
    return value
rows=[]
for kind in ('host','sanitizers'):
    probe=build/('receiver-transport-'+kind)/name
    run=subprocess.run(['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(probe),linux(cache)],capture_output=True,timeout=60)
    assert run.returncode==0 and not run.stderr,(run.stdout,run.stderr)
    result=parse(run.stdout.decode());rows.append({'build':kind,'probe_sha256':sha(probe),'result':result});print(json.dumps({'build':kind,**result}),flush=True)
base=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5590']
def adb(args):
    avd=subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0];assert avd=='DH2_Loader_API37',avd
    return subprocess.check_output(base+args,timeout=60).decode(errors='replace')
remote_cache='/data/local/tmp/dh2-loader-map-recovery/cache.zip';assert adb(['shell','sha256sum',remote_cache]).split()[0]==sha(cache)
remote='/data/local/tmp/dh2-loader-module-graph-source-v1';adb(['shell','mkdir','-p',remote])
probe=build/'receiver-transport-x86_64'/name;adb(['push',str(probe),remote+'/probe']);adb(['shell','chmod','755',remote+'/probe'])
result=parse(adb(['shell',remote+'/probe',remote_cache]));assert result==rows[0]['result'];rows.append({'build':'android-x86_64','probe_sha256':sha(probe),'result':result});print(json.dumps({'build':'android-x86_64',**result}),flush=True)
overlay=loader/'vendor/module-graph-integration-b3ceccd6833a5ca6';manifest=json.loads((overlay/'integration-manifest.json').read_text())
for relative,row in manifest['files'].items():assert sha(overlay/relative)==row['sha256'],relative
incoming=loader/'vendor/module-controller-b3ceccd6833a5ca6';origin=json.loads((incoming/'MANIFEST.json').read_text())
for row in origin['files']:assert sha(overlay/row['path'])==row['sha256'],row['path']
receipt={'validation':'PASS','scope':'Complete original SWAMP root XML -> actual config/nine-Module static graph, generated RoomZones, same-world floor/collision/post-load/InitFinal and same-root controller construction/release; actual first MGP constructor failure; outer providers fixtures',
    'cases':rows,'arm64_compile_sha256':sha(build/'receiver-transport-arm64-v8a'/name),'cache_sha256':sha(cache),
    'probe_source_sha256':sha(loader/'tests/canonical_module_graph_source_probe.cpp'),'coherent_manifest_sha256':sha(overlay/'integration-manifest.json'),
    'coherent_files':len(manifest['files']),'incoming_controller_entries_verified':17,'copied_module_declarations':False,
    'whole_candidate_destruction_verified':False,'tiny_anim_controller_constructor_verified':True,'controller_scope':'actual nine static SWAMP roots, empty original animation libraries','full_loader_verified':False}
(reports/'canonical-module-controller-source-checks.json').write_text(json.dumps(receipt,indent=2)+'\n')
