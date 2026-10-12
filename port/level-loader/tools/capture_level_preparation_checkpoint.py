"""Freeze verified staged source preparation and current private Android evidence.

The bundle is an integration slice over its immutable prior reconstruction
baseline. Gameplay factories, actor rendering and restoration remain incomplete.
"""
import hashlib, json, pathlib, zipfile
root=pathlib.Path(__file__).resolve().parents[1];worktree=root.parents[1]
assert worktree==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
reports=root/'reports';build=worktree.parent/'build'
def sha(path):
    with path.open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
baseline=reports/'source-pipeline-handoff-bd48491b34de1b76.zip'
assert sha(baseline)=='684f2a0ace2ced5aa7400bffc5ae181f01f930a7e2990b01ba3faff30367c346'
with zipfile.ZipFile(baseline) as archive:
    prior=json.loads(archive.read('port/level-loader/reports/source-pipeline-checkpoint.json'))
sources={}
changed={'port/level-loader/CMakeLists.txt','port/level-loader/android/loader_preview.cpp',
         'port/level-loader/tools/audit_source_pipeline_preview.py'}
for relative,expected in prior['source_sha256'].items():
    path=worktree/relative
    if relative not in changed:assert sha(path)==expected,relative
    sources[relative]=sha(path)
extra=['level_preparation_v1.hpp','level_preparation_v1.cpp','tests/level_preparation_probe.cpp',
       'tests/level_preparation_host.py','tests/level_preparation_coverage.py',
       'tools/capture_level_preparation_checkpoint.py','LEVEL-PREPARATION-HANDOFF.md']
for relative in extra:
    path=root/relative;sources[path.relative_to(worktree).as_posix()]=sha(path)
receipts={};artifacts={};pair=[]
for label,folder in [('host','host-xml'),('sanitizers','host-sanitizers')]:
    binary=build/folder/'dh2_loader_level_preparation_probe';artifacts[str(binary)]=sha(binary)
    lifecycle_path=reports/f'level-preparation-{label}.json';lifecycle=json.loads(lifecycle_path.read_text())
    coverage_path=reports/f'level-preparation-coverage-{label}.json';coverage=json.loads(coverage_path.read_text())
    for path,report in [(lifecycle_path,lifecycle),(coverage_path,coverage)]:
        assert report['validation']=='PASS' and report['probe_sha256']==sha(binary)
        assert not report['full_loader_verified']
        for relative,digest in report['source_sha256'].items():assert sha(root/relative)==digest,relative
        receipts[path.name]=sha(path)
    assert lifecycle['actual']['lifecycle_checks']==14
    assert coverage['fixed_cases']==16 and coverage['procedural_cases']==70 and len(coverage['cases'])==86
    assert coverage['baseline_archive_sha256']==sha(baseline)
    pair.append((lifecycle['actual'],coverage['cases']))
assert pair[0]==pair[1]
assert 'CMAKE_CXX_FLAGS:STRING=-fsanitize=address,undefined' in (build/'host-sanitizers/CMakeCache.txt').read_text()
preview_path=reports/'level-preparation-preview.json';preview=json.loads(preview_path.read_text())
assert preview['validation']=='PASS' and preview['preparation_stages_verified']
assert preview['serial']=='emulator-5590' and preview['avd']=='DH2_Loader_API37'
assert preview['audit_source_sha256']==sha(root/'tools/audit_source_pipeline_preview.py')
assert preview['logcat_sha256']==sha(reports/'level-preparation-preview-logcat.txt')
assert preview['steps'][3]['previous_map_pixels_unchanged']
for name,digest in preview['screenshots'].items():assert sha(reports/name)==digest,name
apk=worktree/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
assert sha(apk)==preview['apk_sha256']==json.loads((reports/'preview-apk.json').read_text())['sha256']
packaged={}
with zipfile.ZipFile(apk) as archive:
    for abi in ('arm64-v8a','x86_64'):
        name=f'lib/{abi}/libdh2_loader_preview.so';packaged[name]=hashlib.sha256(archive.read(name)).hexdigest()
    catalog=json.loads(archive.read('assets/loader-map-catalog.json'))
    assert len(catalog['maps'])==51
    assert catalog['coverage_sha256']==sha(reports/'source-pipeline-fixed-maps-host.json')
    digest=hashlib.sha256()
    with archive.open('assets/dh2-original-cache.zip') as stream:
        while chunk:=stream.read(1024*1024):digest.update(chunk)
    assert digest.hexdigest()==prior['canonical_cache_sha256']
report={'validation':'PASS','scope':__doc__,'source_sha256':sources,'receipt_sha256':receipts,
        'baseline_archive_sha256':sha(baseline),'built_artifact_sha256':artifacts,
        'apk_sha256':sha(apk),'packaged_binary_sha256':packaged,
        'preview_receipt_sha256':sha(preview_path),'canonical_cache_sha256':digest.hexdigest(),
        'lifecycle_checks_per_build':14,'coverage_cases_per_build':86,
        'normal_sanitizer_semantics_match':True,'current_android_stages_verified':True,
        'retained_borrow_direction_accepted':True,'runtime_factory_provider_available':False,
        'runtime_objects_verified':False,'mob_and_chest_rendering_verified':False,
        'campaign_restoration_verified':False,'full_loader_verified':False}
checkpoint=reports/'level-preparation-checkpoint.json';checkpoint.write_text(json.dumps(report,indent=2)+'\n')
files={relative:worktree/relative for relative in sources}
for name in list(receipts)+['level-preparation-checkpoint.json','level-preparation-preview.json',
                         'level-preparation-preview-logcat.txt','preview-apk.json']+list(preview['screenshots']):
    path=reports/name;files[path.relative_to(worktree).as_posix()]=path
manifest={'scope':__doc__,'checkpoint_sha256':sha(checkpoint),'baseline_archive_sha256':sha(baseline),
          'files_sha256':{relative:sha(path) for relative,path in sorted(files.items())},'full_loader_verified':False}
target=reports/('level-preparation-handoff-'+sha(checkpoint)[:16]+'.zip')
assert not target.exists(),'Refusing to replace frozen handoff'
with zipfile.ZipFile(target,'x',compression=zipfile.ZIP_DEFLATED,compresslevel=6) as archive:
    for relative,path in sorted(files.items()):
        info=zipfile.ZipInfo(relative,date_time=(2026,10,5,0,0,0));info.compress_type=zipfile.ZIP_DEFLATED
        archive.writestr(info,path.read_bytes())
    archive.writestr('HANDOFF-MANIFEST.json',json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(target) as archive:
    for relative,digest in manifest['files_sha256'].items():assert hashlib.sha256(archive.read(relative)).hexdigest()==digest
result={'archive':str(target),'archive_sha256':sha(target),'checkpoint_sha256':sha(checkpoint),
        'apk_sha256':sha(apk),'files':len(files),'full_loader_verified':False}
(reports/(target.name+'.json')).write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
