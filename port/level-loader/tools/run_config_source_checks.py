import hashlib,json,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader';reports=loader/'reports';build=root.parent/'build'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
target='dh2_loader_config_source_probe'
def parse(output):
    result=json.loads(output.strip().splitlines()[-1]);assert result['validation']=='PASS' and result['config_source_checks']==8
    assert result['actual_level_config_constructor'] and result['actual_config_publication'] and result['first_missing_type_after_config']=='Module' and not result['full_loader_verified']
    return result
rows=[]
for kind in ('host','sanitizers'):
    probe=build/('receiver-transport-'+kind)/target
    run=subprocess.run(['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(probe),linux(cache)],capture_output=True,timeout=60)
    assert run.returncode==0 and not run.stderr,(run.stdout,run.stderr)
    rows.append({'build':kind,'probe_sha256':sha(probe),'result':parse(run.stdout.decode())});print(json.dumps({'build':kind,'checks':8}),flush=True)
base=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5590']
def adb(args):
    avd=subprocess.check_output(base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0]
    assert avd=='DH2_Loader_API37',avd
    return subprocess.check_output(base+args,timeout=60).decode(errors='replace')
cache_remote='/data/local/tmp/dh2-loader-map-recovery/cache.zip';assert adb(['shell','sha256sum',cache_remote]).split()[0]==sha(cache)
remote='/data/local/tmp/dh2-loader-config-source-v1';adb(['shell','mkdir','-p',remote])
probe=build/'receiver-transport-x86_64'/target;adb(['push',str(probe),remote+'/probe']);adb(['shell','chmod','755',remote+'/probe'])
result=parse(adb(['shell',remote+'/probe',cache_remote]));assert result==rows[0]['result']
rows.append({'build':'android-x86_64','probe_sha256':sha(probe),'result':result});print(json.dumps({'build':'android-x86_64','checks':8}),flush=True)
overlay=loader/'vendor/module-graph-integration-0598cd0ac3591e30';manifest=json.loads((overlay/'integration-manifest.json').read_text())
for relative,row in manifest['files'].items():assert sha(overlay/relative)==row['sha256'],relative
original=loader/'vendor/module-graph-0598cd0ac3591e30';graph=json.loads((original/'MANIFEST.json').read_text())
for row in graph['files']:assert sha(original/row['path'])==row['sha256'] and sha(overlay/row['path'])==row['sha256'],row['path']
receipt={'validation':'PASS','scope':'Actual original-SWAMP LevelConfig source constructor/defaults/overrides/early InitPost and publication into SAME Level fields; Debug/network/parser/current-slot/Array-name providers explicit fixtures',
    'cases':rows,'arm64_compile_sha256':sha(build/'receiver-transport-arm64-v8a'/target),'source_sha256':sha(loader/'tests/canonical_config_source_probe.cpp'),
    'module_graph_archive_sha256':'64cdf8c34aab885b3bc9b9e9a1f619056d1ad5d70e4057ac2545414f763c5ac2','verified_graph_entries':135,
    'coherent_manifest_sha256':sha(overlay/'integration-manifest.json'),'coherent_files':len(manifest['files']),
    'coherent_level_header_sha256':sha(overlay/'port/level-loader/canonical_level_context_v1.hpp'),'same_private_level_header_sha256':sha(loader/'canonical_level_context_v1.hpp'),
    'cache_sha256':sha(cache),'music_reference':'SwampHubAmbientMusic','safezone_reference':'SwampMerchantCampMusic','both_absent_from_supplied_183_name_fixture':True,
    'full_module_graph_verified':False,'full_loader_verified':False}
assert receipt['coherent_level_header_sha256']==receipt['same_private_level_header_sha256']
(reports/'canonical-config-source-checks.json').write_text(json.dumps(receipt,indent=2)+'\n')
