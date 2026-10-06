import pathlib,subprocess,hashlib,json,zipfile,sys,os
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc');loader=root/'port/level-loader';overlay=loader/'vendor/character-rng-integration-v4'
sha=lambda b:hashlib.sha256(b).hexdigest()
manifest=json.loads((overlay/'integration-manifest.json').read_text())
for name,entry in manifest['files'].items():assert sha((overlay/name).read_bytes())==entry['sha256'],name
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache.read_bytes())=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
fixtures=json.loads((overlay/'port/level-world/reference/canonical-character-family-v4/cache-fixture-manifest.json').read_text())
with zipfile.ZipFile(cache) as z:
 for row in fixtures:
  name=row['path'].replace('\\','/');raw=(overlay/name).read_bytes();assert sha(raw)==row['sha256'],name
  assert raw==z.read('com.gameloft.android.GAND.GloftD2SS/files/data/pydata/'+pathlib.PurePosixPath(name).name),name
native_inputs=loader/'tests/native-ctor-v4'
native_manifest=json.loads((native_inputs/'input-manifest.json').read_text())
for name,row in native_manifest['files'].items():assert sha((loader/name).read_bytes())==row['sha256'],name
targets=['dh2_loader_module_graph_source_probe','dh2_loader_config_across_regression','dh2_loader_room_zone_across_regression','dh2_loader_character_family_prefix_probe','dh2_loader_application_rng_probe','dh2_loader_chest_data_probe','dh2_loader_character_position_kernel_probe','dh2_loader_character_position_owner_probe','dh2_loader_character_position_prefix_probe','dh2_loader_dummy_source_probe','dh2_loader_same_level_ctor_probe','dh2_loader_swamp_families_source_probe','dh2_loader_destructible_source_probe','dh2_loader_native_ctor_bindings_probe','dh2_loader_container_callbacks_v21_probe','dh2_loader_trigger_zone_v22_probe']
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
chest_assets=loader/'tests/chest-data-v11'
position_gold=loader/'tests/position-v7/original-gold.bin'
asset_dir=overlay/'port/android-native/app/src/main/assets/data'
receipt={'scope':'Whole native Level C1 on SAME Swamp context plus retained module/object preparation and source-positive callback capability with explicit external fixtures; full Init/currentLevel/gameplay unverified','coherent_files_verified':len(manifest['files']),'original_character_table_assets_verified':len(fixtures),'cases':[], 'full_loader_verified':False,'visible_apk_updated':False}
receipt_path=loader/'reports/retained-level-module-graph-exit-zone-v29-checks.json'
if receipt_path.exists():
 old=json.loads(receipt_path.read_text())
 for name,digest in old['source_sha256'].items():assert sha((loader/name).read_bytes())==digest,name
 receipt['cases']=[case for case in old['cases'] if case['build'] not in sys.argv[1:]]
callback_manifest=json.loads((loader/'reports/container-v21-received.json').read_text())
for row in callback_manifest['original_assets']:assert sha((loader/'tests/callback-v21-assets'/row['path']).read_bytes())==row['sha256'],row['path']
for kind in sys.argv[1:]:
 build=root.parent/'build'/('receiver-transport-'+kind)
 rows=[]
 if kind=='x86_64':
  adb_base=[r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe','-s','emulator-5590']
  if os.environ.get('DH2_LOADER_ADB_PORT'):
   port=int(os.environ['DH2_LOADER_ADB_PORT']);assert port==5039,'Only isolated loader ADB port5039 supported'
   adb_base[1:1]=['-P',str(port)]
  def adb(args):
   avd=subprocess.check_output(adb_base+['emu','avd','name'],timeout=15).decode().replace('\r','').splitlines()[0];assert avd=='DH2_Loader_API37',avd
   r=subprocess.run(adb_base+args,capture_output=True,timeout=60);assert r.returncode==0,(r.stdout,r.stderr);return r.stdout.decode(errors='replace')
  remote='/data/local/tmp/dh2-loader-retained-graph-v1';adb(['shell','mkdir','-p',remote+'/data',remote+'/chest-data'])
  adb(['shell','mkdir','-p',remote+'/native-ctor-scripts',remote+'/native-ctor-missing-saves',remote+'/callback-v21-assets'])
  adb(['push',str(build/'libdh2_script_runtime.so'),remote+'/libdh2_script_runtime.so'])
  for directory,destination in [(asset_dir,remote+'/data'),(chest_assets,remote+'/chest-data'),(native_inputs/'scripts',remote+'/native-ctor-scripts'),(loader/'tests/callback-v21-assets',remote+'/callback-v21-assets')]:
   for file in directory.iterdir():
    if file.is_file():adb(['push',str(file),destination+'/'+file.name])
 if kind=='x86_64':
  adb(['push',str(position_gold),remote+'/original-position-gold.bin'])
  adb(['push',str(native_inputs/'design.bin'),remote+'/native-ctor-design.bin'])
 for target in targets:
  binary=build/target
  if kind=='x86_64':
   adb(['push',str(binary),remote+'/'+target]);adb(['shell','chmod','755',remote+'/'+target])
   args=[remote+'/'+target]
   if target==targets[0]:args+=['/data/local/tmp/dh2-loader-map-recovery/cache.zip',remote+'/native-ctor-design.bin',remote+'/native-ctor-missing-saves']
   elif target==targets[3]:args+=[remote+'/data']
   elif target==targets[5]:args+=[remote+'/chest-data']
   elif target==targets[6]:args+=[remote+'/original-position-gold.bin']
   elif target==targets[8]:args+=[remote+'/data']
   elif target=='dh2_loader_native_ctor_bindings_probe':args+=[remote+'/native-ctor-design.bin',remote+'/native-ctor-scripts',remote+'/native-ctor-missing-saves']
   elif target=='dh2_loader_container_callbacks_v21_probe':args+=[remote+'/callback-v21-assets']
   out=adb(['shell','env','LD_LIBRARY_PATH='+remote]+args)
  else:
   args=['wsl.exe','-d','Ubuntu','--',linux(binary)]
   if target==targets[0]:
    missing_saves=build/'native-ctor-missing-saves';missing_saves.mkdir(exist_ok=True);args+=[linux(cache),linux(native_inputs/'design.bin'),linux(missing_saves)]
   elif target==targets[3]:args+=[linux(asset_dir)]
   elif target==targets[5]:args+=[linux(chest_assets)]
   elif target==targets[6]:args+=[linux(position_gold)]
   elif target==targets[8]:args+=[linux(asset_dir)]
   elif target=='dh2_loader_native_ctor_bindings_probe':
    missing_saves=build/'native-ctor-missing-saves';missing_saves.mkdir(exist_ok=True);args+=[linux(native_inputs/'design.bin'),linux(native_inputs/'scripts'),linux(missing_saves)]
   elif target=='dh2_loader_container_callbacks_v21_probe':args+=[linux(loader/'tests/callback-v21-assets')]
   r=subprocess.run(args,capture_output=True,timeout=60);assert r.returncode==0,(target,r.stdout,r.stderr);out=r.stdout.decode(errors='replace')
  if target==targets[0]:
   result=json.loads(out.strip().splitlines()[-1]);assert result['validation']=='PASS' and result['retained_application_API_verified'] and result['actual_character_constructed']>=1 and result['shared_application_RNG_connected'] and result['actual_chest_table_dictionary_connected'] and result['chest_condition_source_kernel_connected'] and result['whole_character_factory_position_verified'] and result['source_device_predicate_connected'] and result['whole_source_level_ctor_verified'] and not result['whole_level_init_verified'] and not result['current_GSLevel_published'] and not result['full_loader_verified']
  else:
   assert 'PASS' in out or not out.strip(),(target,out);result=out.strip()
   if target==targets[3]:assert '433 actual NPC rows' in out
   if target==targets[5]:assert 'checks=105' in out
   if target=='dh2_loader_native_ctor_bindings_probe':
    assert 'checks=459' in out and 'worlds/001_swamp.mlx' in out
    print(out.strip(),flush=True)
   if target=='dh2_loader_container_callbacks_v21_probe':assert 'cases=3' in out and 'authored_drops=6' in out
  rows.append({'target':target,'binary_sha256':sha(binary.read_bytes()),'result':result})
 receipt['cases'].append({'build':kind,'runtime_library_sha256':sha((build/'libdh2_script_runtime.so').read_bytes()),'probes':rows})
 print(kind,'PASS',flush=True)
receipt['validation']='PASS';receipt['arm64_binaries']={target:sha((root.parent/'build/receiver-transport-arm64-v8a'/target).read_bytes()) for target in targets}
receipt['arm64_runtime_library_sha256']=sha((root.parent/'build/receiver-transport-arm64-v8a/libdh2_script_runtime.so').read_bytes())
receipt['source_sha256']={name:sha((loader/name).read_bytes()) for name in ['retained_level_module_graph_v1.hpp','retained_level_module_graph_v1.cpp','canonical_level_class_dispatch_v1.hpp','canonical_level_class_dispatch_v1.cpp','module_draw_frame_v1.hpp','module_draw_frame_v1.cpp','tests/canonical_module_graph_source_probe.cpp','device_pipeline_borrow_v1.hpp','device_pipeline_borrow_v1.cpp','canonical_level_context_v1.hpp','canonical_level_context_v1.cpp','level_constructor_v3.hpp','level_constructor_v3.cpp','canonical_auxiliary_families_v16.hpp','canonical_auxiliary_families_v16.cpp']}
receipt_path.write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps({'validation':'PASS','builds':sys.argv[1:],'coherent_files':len(manifest['files']),'full_loader_verified':False}))
