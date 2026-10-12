"""Freeze partial XML/canonical handle adapter; no live runtime or level claim."""
import pathlib,hashlib,json,zipfile
loader=pathlib.Path(__file__).resolve().parents[1];root=loader.parents[1];reports=loader/'reports'
assert root==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
base=reports/'map-recovery-handoff-5888339e4164baaa.zip'
assert sha(base)=='d83a37a11b3833f03d45cb89a63f18f2765f92b5cb973e4516facc6728075a87'
with zipfile.ZipFile(base) as z:
    old=json.loads(z.read('HANDOFF-MANIFEST.json'))
    for relative,digest in old['files_sha256'].items():
        if relative in ('port/level-loader/CMakeLists.txt','port/level-loader/README.md','port/level-loader/OWNER-CONTEXT-REQUIREMENTS.md'):continue
        assert sha(root/relative)==digest,relative
    assert (loader/'CMakeLists.txt').read_bytes().startswith(z.read('port/level-loader/CMakeLists.txt'))
vendor=loader/'vendor/canonical-owners-e8cc6acab5183a2b'
incoming=json.loads((vendor/'canonical-loader-owners-manifest.json').read_text(encoding='utf-8'))
for name in ('canonical_object_manager_v1.hpp','canonical_object_manager_v1.cpp','canonical_object_factory_v1.hpp','canonical_object_factory_v1.cpp'):
    assert sha(vendor/name)==incoming['entries']['port/level-world/'+name]['sha256'],name
checks=json.loads((reports/'canonical-source-adapter-checks.json').read_text(encoding='utf-8'));assert checks['validation']=='PASS'
assert len(checks['cases'])==3
for row in checks['cases']:
    assert row['result']['adapter_checks']==14 and not row['result']['runtime_objects_verified']
    probe=(root/'port/android-native/app/build/intermediates/cxx/Debug/5e223t2g/obj/x86_64/dh2_loader_canonical_source_adapter_probe') if row['build']=='android-x86_64' else root.parent/'build'/row['build']/'dh2_loader_canonical_source_adapter_probe'
    assert sha(probe)==row['probe_sha256']
assert all(row['result']==checks['cases'][0]['result'] for row in checks['cases'])
reuse=json.loads((reports/'canonical-owner-reuse-check.json').read_text(encoding='utf-8'))
assert reuse['validation']=='PASS' and reuse['external_owner_selected'] and not reuse['second_owner_target_created']
assert reuse['result']==checks['cases'][0]['result']
assert sha(root.parent/'build/canonical-owner-reuse/loader/dh2_loader_canonical_source_adapter_probe')==reuse['probe_sha256']
apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
assert sha(apk)=='ef7048fcdecf30e0ce2422551639f27d1d61938843ff2c1cc46c4da29789a7e2'
paths=['canonical_source_adapter_v1.hpp','canonical_source_adapter_v1.cpp','tests/canonical_source_adapter_probe.cpp','tests/cmake-canonical-owner/CMakeLists.txt','CMakeLists.txt','README.md','OWNER-CONTEXT-REQUIREMENTS.md','CANONICAL-SOURCE-ADAPTER-HANDOFF.md','tools/run_canonical_adapter_checks.py','tools/record_owner_reuse_result.py','tools/capture_canonical_source_adapter_checkpoint.py','reports/canonical-source-adapter-checks.json','reports/canonical-owner-reuse-check.json']
paths += [p.relative_to(loader).as_posix() for p in sorted(vendor.iterdir())]
files={p:loader/p for p in paths}
dependencies=['port/level-world/character_target_providers.hpp','port/level-world/character_combat_queries.hpp','port/game-data/animation_tables.hpp','port/game-data/data.hpp']
arm=root/'port/android-native/app/build/intermediates/cxx/Debug/5e223t2g/obj/arm64-v8a/dh2_loader_canonical_source_adapter_probe'
checkpoint={'validation':'PASS','scope':__doc__,'base_archive':base.name,'base_archive_sha256':sha(base),
 'canonical_owner_archive':'canonical-loader-owners-handoff-e8cc6acab5183a2b.zip','canonical_owner_archive_sha256':'6fef03533022762709695f2847d7877f22ff8c4471296dae83fa85ad704eab9e',
 'source_sha256':{p:sha(file) for p,file in files.items()},'existing_dependency_sha256':{p:sha(root/p) for p in dependencies},
 'arm64_probe_sha256':sha(arm),'map_apk_sha256':sha(apk),'adapter_checks_per_execution':14,'executed_configurations':['host-xml','host-sanitizers','android-x86_64','existing-owner-target'],
 'full_property_map_provider_available':False,'class_init_final_available':False,'canonical_cleanup_commit_available':False,'runtime_objects_verified':False,'full_loader_verified':False}
raw=json.dumps(checkpoint,indent=2)+'\n';digest=hashlib.sha256(raw.encode()).hexdigest()
cp=reports/'canonical-source-adapter-checkpoint.json';cp.write_text(raw);files['reports/'+cp.name]=cp
manifest={'scope':__doc__,'checkpoint_sha256':digest,'files_sha256':{'port/level-loader/'+p:sha(file) for p,file in files.items()},'runtime_objects_verified':False,'full_loader_verified':False}
archive=reports/('canonical-source-adapter-handoff-'+digest[:16]+'.zip');assert not archive.exists()
with zipfile.ZipFile(archive,'x',compression=zipfile.ZIP_DEFLATED) as z:
    for relative,file in sorted(files.items()):z.writestr('port/level-loader/'+relative,file.read_bytes())
    z.writestr('HANDOFF-MANIFEST.json',json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(archive) as z:
    for name,digest in manifest['files_sha256'].items():assert hashlib.sha256(z.read(name)).hexdigest()==digest,name
result={'archive':str(archive),'archive_sha256':sha(archive),'files':len(files),'checks_per_execution':14,'full_loader_verified':False}
(reports/(archive.name+'.json')).write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
