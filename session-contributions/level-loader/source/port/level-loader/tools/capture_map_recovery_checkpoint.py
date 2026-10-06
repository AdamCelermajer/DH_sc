"""Freeze verified map recovery as an incremental handoff over immutable sources.

Does not merge, publish, include the cache/ELF/APK, or instantiate gameplay objects.
"""
import pathlib,json,hashlib,zipfile
loader=pathlib.Path(__file__).resolve().parents[1];root=loader.parents[1]
assert root==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
reports=loader/'reports'
def sha(path):
    with path.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()
baselines={'source-pipeline-handoff-bd48491b34de1b76.zip':'684f2a0ace2ced5aa7400bffc5ae181f01f930a7e2990b01ba3faff30367c346',
           'level-preparation-handoff-8e58f2de9e440b61.zip':'29088dba2f0bf6377f7e1be93cd5e32c4454b05487afa5cd623f338c35240a99'}
for name,digest in baselines.items():assert sha(reports/name)==digest,name
with zipfile.ZipFile(reports/'level-preparation-handoff-8e58f2de9e440b61.zip') as z:
    prior=json.loads(z.read('port/level-loader/reports/level-preparation-checkpoint.json'))
changed={'port/level-loader/'+p for p in ('CMakeLists.txt','fixed_map_v1.cpp','fixed_map_v1.hpp','level_preparation_v1.cpp','level_preparation_v1.hpp','procedural_modules_v1.hpp','android/loader_preview.cpp','tests/level_preparation_probe.cpp','tests/level_preparation_coverage.py','tools/build_preview.py','README.md')}
sources={}
for relative,digest in prior['source_sha256'].items():
    p=root/relative
    if relative not in changed:assert sha(p)==digest,relative
    sources[relative]=sha(p)
for relative in ('README.md','MAP-RECOVERY-HANDOFF.md','CMakeLists.txt','source_reference_repairs_v1.cpp','source_reference_repairs_v1.hpp','static_decor_inspection_v1.cpp','static_decor_inspection_v1.hpp','fixed_map_v1.cpp','fixed_map_v1.hpp','tests/static_decor_inspection_probe.cpp','procedural_modules_v1.hpp','level_preparation_v1.cpp','level_preparation_v1.hpp','android/loader_preview.cpp','tests/loader_recovery_probe.cpp','tests/level_preparation_probe.cpp','tests/level_preparation_coverage.py','tools/build_preview.py','tools/audit_map_recovery_preview.py','tools/android_native_recovery_checks.py','tools/capture_map_recovery_checkpoint.py'):
    p=loader/relative;sources[p.relative_to(root).as_posix()]=sha(p)
names=('loader-recovery-host.json','loader-recovery-sanitizers.json','loader-recovery-coverage.json','loader-original-mode-coverage.json','loader-recovery-preview.json','map-recovery-preview.json','map-recovery-native-android.json','loader-current-host.json','loader-current-sanitizers.json','loader-current-coverage.json','loader-current-original-mode-coverage.json','decor-inspection-coverage.json','decor-inspection-sanitizers.json')
receipts={name:json.loads((reports/name).read_text()) for name in names}
for name,receipt in receipts.items():assert receipt['validation']=='PASS',name
host=receipts['loader-recovery-host.json'];san=receipts['loader-recovery-sanitizers.json']
assert [r['result'] for r in host['cases']]==[r['result'] for r in san['cases']]
assert host['cases'][0]['result']['recovery_checks']==20 and host['cases'][1]['result']['lifecycle_checks']==14
# Historical native receipts retain their probe hashes. They are not falsely
# bound to binaries/source changed by the later static-decor inspection route.
coverage=receipts['loader-recovery-coverage.json'];assert coverage['summary']=={'assembled':344}
assert coverage['backup_cases']==53 and coverage['repaired_cases']==2
original=receipts['loader-original-mode-coverage.json'];assert len(original['cases'])==86
for relative,digest in original['source_sha256'].items():
    if relative!='CMakeLists.txt':assert sha(loader/relative)==digest,relative
status=json.loads((reports/'map-recovery-verification-status.json').read_text(encoding='utf-8'))
assert status['final_android_build'].startswith('PASS') and status['final_source_host_sanitizer_retest'].startswith('PASS')
current=receipts['loader-current-coverage.json'];assert current['summary']=={'assembled':344}
assert current['backup_cases']==53 and current['repaired_cases']==2
assert current['probe_sha256']==sha(root.parent/'build/host-xml/dh2_loader_level_preparation_probe')
assert [(r['identity'],r['seed'],r['status'],r['result']) for r in current['cases']]==[(r['identity'],r['seed'],r['status'],r['result']) for r in coverage['cases']]
raw=receipts['loader-current-original-mode-coverage.json'];assert len(raw['cases'])==86
for relative,digest in raw['source_sha256'].items():assert sha(loader/relative)==digest,relative
for label,name in [('host-xml','loader-current-host.json'),('host-sanitizers','loader-current-sanitizers.json')]:
    for row in receipts[name]['cases']:
        filename='dh2_loader_recovery_probe' if row['name']=='recovery' else 'dh2_loader_level_preparation_probe'
        assert sha(root.parent/'build'/label/filename)==row['probe_sha256']
assert [r['result'] for r in receipts['loader-current-host.json']['cases']]==[r['result'] for r in receipts['loader-current-sanitizers.json']['cases']]
decor=receipts['decor-inspection-coverage.json'];dsan=receipts['decor-inspection-sanitizers.json']
assert len(decor['cases'])==86 and len(dsan['cases'])==13
assert decor['probe_sha256']==sha(root.parent/'build/host-xml/dh2_loader_static_decor_inspection_probe')
assert dsan['probe_sha256']==sha(root.parent/'build/host-sanitizers/dh2_loader_static_decor_inspection_probe')
lookup={(r['identity'],r['seed']):r['result'] for r in decor['cases']}
for row in dsan['cases']:assert lookup[(row['identity'],row['seed'])]==row['result']
native=receipts['map-recovery-native-android.json'];assert len(native['decor_cases'])==86
for row in native['decor_cases']:assert lookup[(row['identity'],row['seed'])]==row['result']
assert native['checks'][0]['result']['recovery_checks']==20 and native['checks'][1]['result']['lifecycle_checks']==14
assert native['abi']=='x86_64' and not native['sanitizers'] and native['serial']=='emulator-5590'
for relative,digest in native['source_sha256'].items():assert sha(root/relative)==digest,relative
for name,digest in native['probe_sha256'].items():
    assert sha(root/'port/android-native/app/build/intermediates/cxx/Debug/5e223t2g/obj/x86_64'/name)==digest,name
preview=receipts['map-recovery-preview.json'];assert len(preview['cases'])==86
assert preview['native_frames_verified']==preview['nonblank_map_viewports_verified']==86
assert preview['coverage_sha256']==sha(reports/'loader-recovery-coverage.json')
assert preview['audit_source_sha256']==sha(loader/'tools/audit_map_recovery_preview.py')
assert preview['serial']=='emulator-5590' and preview['avd']=='DH2_Loader_API37' and preview['restored']=='SWAMP'
retention=receipts['loader-recovery-preview.json'];assert retention['steps'][3]['previous_map_pixels_unchanged']
assert retention['audit_source_sha256']==sha(loader/'tools/audit_source_pipeline_preview.py')
assert retention['logcat_sha256']==sha(reports/'loader-recovery-preview-logcat.txt')
apk=root/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
apk_sha=sha(apk);assert apk_sha==native['apk_sha256']==preview['apk_sha256']==retention['apk_sha256']==json.loads((reports/'preview-apk.json').read_text())['sha256']
packaged={}
with zipfile.ZipFile(apk) as z:
    for abi in ('arm64-v8a','x86_64'):
        name='lib/'+abi+'/libdh2_loader_preview.so';packaged[name]=hashlib.sha256(z.read(name)).hexdigest()
    catalog=json.loads(z.read('assets/loader-map-catalog.json'));assert len(catalog['maps'])==51
    assert catalog['procedural_coverage_sha256']==sha(reports/'loader-recovery-picker-coverage.json')
    with z.open('assets/dh2-original-cache.zip') as stream:cache_sha=hashlib.file_digest(stream,'sha256').hexdigest()
    assert cache_sha==prior['canonical_cache_sha256']==coverage['cache_sha256']
files={relative:root/relative for relative in sources}
for name in list(names)+['map-recovery-verification-status.json','preview-apk.json','loader-recovery-picker-coverage.json','loader-recovery-preview-logcat.txt','repair-target-missing.zip','repair-authored-present.zip','map-recovery-original-load-process.asm','map-recovery-dependency-scan.json','map-recovery-latent-seeds.json']:
    p=reports/name;files[p.relative_to(root).as_posix()]=p
for row in preview['cases']:
    p=reports/row['screenshot'];assert sha(p)==row['screenshot_sha256'];files[p.relative_to(root).as_posix()]=p
for name,digest in retention['screenshots'].items():
    p=reports/name;assert sha(p)==digest;files[p.relative_to(root).as_posix()]=p
checkpoint={'validation':'PASS','scope':__doc__,'source_sha256':sources,'receipt_sha256':{name:sha(reports/name) for name in names},
    'baseline_archives_sha256':baselines,'canonical_cache_sha256':cache_sha,'apk_sha256':apk_sha,'packaged_binary_sha256':packaged,
    'native_assembly_cases_before_static_decor':344,'final_source_native_android_retested':True,'final_source_host_sanitizer_retested':True,'native_assembly_cases':344,'original_mode_final_source_cases':86,'sanitizer_decor_cases':13,'verification_status':status,'android_map_frames':86,'backup_cases':53,'repaired_cases':2,'original_mode_cases':86,
    'final_source_native_decor_cases':86,'recovery_checks_per_build':20,'lifecycle_checks_per_build':14,'normal_sanitizer_semantics_match_before_static_decor':True,'previous_map_retention_verified':True,
    'runtime_factory_provider_available':False,'runtime_objects_verified':False,'mob_and_chest_rendering_verified':False,'campaign_restoration_verified':False,'full_loader_verified':False}
cp=reports/'map-recovery-checkpoint.json';cp.write_text(json.dumps(checkpoint,indent=2)+'\n');files[cp.relative_to(root).as_posix()]=cp
manifest={'scope':__doc__,'checkpoint_sha256':sha(cp),'baseline_archives_sha256':baselines,'files_sha256':{relative:sha(p) for relative,p in sorted(files.items())},'full_loader_verified':False}
archive=reports/('map-recovery-handoff-'+sha(cp)[:16]+'.zip');assert not archive.exists(),'Existing immutable handoff must not be overwritten'
with zipfile.ZipFile(archive,'x',compression=zipfile.ZIP_DEFLATED,compresslevel=6) as z:
    for relative,p in sorted(files.items()):
        info=zipfile.ZipInfo(relative,date_time=(2026,10,5,0,0,0));info.compress_type=zipfile.ZIP_DEFLATED;z.writestr(info,p.read_bytes())
    z.writestr('HANDOFF-MANIFEST.json',json.dumps(manifest,indent=2)+'\n')
with zipfile.ZipFile(archive) as z:
    for relative,digest in manifest['files_sha256'].items():assert hashlib.sha256(z.read(relative)).hexdigest()==digest
result={'archive':str(archive),'archive_sha256':sha(archive),'checkpoint_sha256':sha(cp),'apk_sha256':apk_sha,'files':len(files),'native_cases_before_static_decor':344,'android_frames':86,'full_loader_verified':False}
(reports/(archive.name+'.json')).write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
