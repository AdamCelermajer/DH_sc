import hashlib,json,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader';reports=loader/'reports';build=root.parent/'build'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip');assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
name='dh2_loader_module_graph_source_probe'
def parse(output):
    value=json.loads(output.strip().splitlines()[-1]);assert value['validation']=='PASS'
    for key,count in {'original_mlx_declarations':10,'actual_modules':9,'floor_clones':16,'exits':19,'generated_zones':9,'actual_init_final':9,'same_root_controllers':9,'controllers_released':9,'render_frame_modules':9,'actual_container_constructed':2,'actual_container_published':2}.items():assert value[key]==count
    assert value['render_frame_meshes']>0
    assert value['reserved_null_key0']
    assert value['first_mgp_missing_type']=='Character' and value['factory_failure_prefix']=='empty' and value['config_across_rooms_modern_repair'] and value['room_zone_across_rooms_modern_repair'] and value['authored_container_properties_verified'] and value['named_template_context_modern_repair'] and not value['template_context_problem'] and value['container_init_post_missing_service']=='OpenableContainer required source service: CheckSpawnProbability' and value['outer_provider_fixtures'] and not value['full_loader_verified']
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
overlay=loader/'vendor/openable-graph-integration-54a3fb6ce996459b';manifest=json.loads((overlay/'integration-manifest.json').read_text())
for relative,row in manifest['files'].items():assert sha(overlay/relative)==row['sha256'],relative
incoming=loader/'vendor/openable-graph-54a3fb6ce996459b';origin=json.loads((incoming/'MANIFEST.json').read_text())
for row in origin['entries']:assert sha(overlay/row['path'])==row['sha256'],row['path']
successor=loader/'vendor/config-across-a3df90efd191542c'
successor_manifest=json.loads((successor/'MANIFEST.json').read_text())
for relative,expected in successor_manifest.items():assert sha(successor/relative)==expected,relative
selected='port/level-world/canonical_level_config_module_v1.cpp'
cmake=(loader/'tests/cmake-receiver-transport/CMakeLists.txt').read_text()
assert '"${loader}/vendor/config-across-a3df90efd191542c/'+selected+'"' in cmake
regressions=[];regression_name='dh2_loader_config_across_regression'
for kind in ('host','sanitizers'):
    probe=build/('receiver-transport-'+kind)/regression_name
    run=subprocess.run(['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(probe)],capture_output=True,timeout=60)
    assert run.returncode==0 and not run.stderr and run.stdout.startswith(b'PASS '),(run.stdout,run.stderr)
    regressions.append({'build':kind,'probe_sha256':sha(probe),'result':'PASS'})
probe=build/'receiver-transport-x86_64'/regression_name
adb(['push',str(probe),remote+'/config-regression']);adb(['shell','chmod','755',remote+'/config-regression'])
output=adb(['shell',remote+'/config-regression']);assert output.startswith('PASS '),output
regressions.append({'build':'android-x86_64','probe_sha256':sha(probe),'result':'PASS'})
print(json.dumps({'config_across_regressions':regressions}),flush=True)
room=loader/'vendor/room-zone-byte87-v4';room_manifest=json.loads((room/'MANIFEST.json').read_text())
for row in room_manifest['files']:assert sha(room/row['path'])==row['sha256'],row['path']
room_leaf=loader/'vendor/room-zone-leaf-v4/canonical_room_zone_v3.cpp'
assert sha(room_leaf)==sha(room/'port/level-world/canonical_room_zone_v3.cpp')
property_leaf=loader/'vendor/named-template-context-v1/canonical_property_map_v1.cpp'
assert '"${loader}/vendor/named-template-context-v1/canonical_property_map_v1.cpp"' in cmake
room_regressions=[];regression_name='dh2_loader_room_zone_across_regression'
for kind in ('host','sanitizers'):
    probe=build/('receiver-transport-'+kind)/regression_name
    run=subprocess.run(['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(probe)],capture_output=True,timeout=60)
    assert run.returncode==0 and not run.stderr and b'PASS' in run.stdout,(run.stdout,run.stderr)
    room_regressions.append({'build':kind,'probe_sha256':sha(probe),'result':'PASS'})
probe=build/'receiver-transport-x86_64'/regression_name
adb(['push',str(probe),remote+'/room-regression']);adb(['shell','chmod','755',remote+'/room-regression'])
output=adb(['shell',remote+'/room-regression']);assert 'PASS' in output,output
room_regressions.append({'build':'android-x86_64','probe_sha256':sha(probe),'result':'PASS'})
receipt={'validation':'PASS' ,'scope':'Complete original SWAMP root XML -> actual config/nine-Module static graph, generated RoomZones, same-world floor/collision/post-load/InitFinal and same-root controller construction/release and cached static Module render submission; actual Container C1 then Add lookup failure on unproduced generated RoomZone byte87 after documented modern Config byte87 correction; no constructor replay; absent Container InitPost RNG continuation; outer providers fixtures',
    'cases':rows,'arm64_compile_sha256':sha(build/'receiver-transport-arm64-v8a'/name),'cache_sha256':sha(cache),
    'probe_source_sha256':sha(loader/'tests/canonical_module_graph_source_probe.cpp'),'render_source_sha256':{p:sha(loader/p) for p in ('module_draw_frame_v1.hpp','module_draw_frame_v1.cpp')},'coherent_manifest_sha256':sha(overlay/'integration-manifest.json'),
    'coherent_files':len(manifest['files']),'incoming_openable_entries_verified':165,'config_successor_entries_verified':12,'selected_config_cpp_sha256':sha(successor/selected),'room_successor_entries_verified':141,'selected_room_cpp_sha256':sha(room_leaf),'selected_property_map_cpp_sha256':sha(property_leaf),'room_regressions':room_regressions,'named_template_policy':'explicit modern registered _templateName setter uses actual SetTemplate clone; assertion9 delivery fixture declared','config_regressions':regressions,'compatibility_policy':'native Config byte87=0 is explicit modern legacy-UB repair, not original constructor initialization','copied_module_declarations':False,
    'whole_candidate_destruction_verified':False,'tiny_anim_controller_constructor_verified':True,'controller_scope':'actual nine static SWAMP roots, empty original animation libraries','visible_apk_updated':False,'full_loader_verified':False}
(reports/'canonical-named-template-source-checks.json').write_text(json.dumps(receipt,indent=2)+'\n')
