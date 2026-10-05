"""Bind the supplemental transform evidence without recertifying older frames."""
import datetime, hashlib, json, pathlib

root = pathlib.Path(__file__).resolve().parents[1]
repo = root.parents[1]
def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest()
def read(name): return json.loads((root/'reports'/name).read_text())

host, san = [read(n) for n in ('visual-root-policy-host.json','visual-root-policy-sanitizers.json')]
for report in (host,san):
    assert report['validation'] == 'PASS'
    assert report['script_sha256'] == sha(root/'tests/visual_root_policy_original.py')
    assert len(report['cases']) == 135 and all(r['matches'] for r in report['cases'])
    assert len(report['constructor_prefix_cases']) == 3 and len(report['reset_guard_cases']) == 2
    assert report['maximum_quaternion_error'] == 0
    assert not report['full_collada_construction_verified'] and not report['full_loader_verified']
    for relative, digest in report['source_sha256'].items(): assert sha(root/relative) == digest, relative
    assert report['cpu_helper_sha256'] == sha(repo/'port/engine-math/tests/differential.py')
assert host['cases'] == san['cases'] and host['source_sha256'] == san['source_sha256']
for variant, report in (('host-xml',host),('host-sanitizers',san)):
    assert report['probe_sha256'] == sha(repo.parent/'build'/variant/'dh2_loader_visual_transform_probe')
vtable = read('original-loader-vtables.json')
assert vtable['validation'] == 'PASS'
assert vtable['script_sha256'] == sha(root/'tools/inspect_original_loader_vtables.py')
assert vtable['early_init_gametype_literal']['value'] == 'LevelConfig'
assert host['engine_sha256'] == san['engine_sha256'] == vtable['engine_sha256']
visible = read('visible-emulator-window.json')
assert visible['avd'] == 'DH2_Loader_API37' and visible['serial'] == 'emulator-5590'
assert visible['booted'] and any(w['visible'] for w in visible['windows'])
apk = repo/'port/android-native/app/build/outputs/apk/debug/app-debug.apk'
assert sha(apk) == 'c90521525b105e55d835d62cb5c348e5940a4c097b16343b908e6724dadabe75'
paths = set(host['source_sha256']) | {
    'tests/visual_root_policy_original.py','tools/capture_visual_root_policy_checkpoint.py',
    'tools/check_visible_preview.py','INTERFACE-PROPOSAL.md','README.md',
    'fixed_declarations_v1.cpp','DECLARATIONS-MILESTONE-HANDOFF.md',
    'VISUAL-ROOT-POLICY-HANDOFF.md','reports/visual-root-policy-host.json',
    'reports/visual-root-policy-sanitizers.json','reports/visible-emulator-window.json',
    'reports/procedural-modules-checkpoint.json'}
report = {'validation':'PASS','captured_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),
          'scope':__doc__,'engine_sha256':host['engine_sha256'],
          'source_sha256':{p:sha(root/p) for p in sorted(paths)},
          'probe_sha256':{'host':host['probe_sha256'],'sanitizers':san['probe_sha256']},
          'unchanged_local_apk_sha256':sha(apk),'visible_emulator':visible,
          'placement_cases_per_variant':135,'constructor_prefix_cases_per_variant':3,
          'reset_guard_cases_per_variant':2,'original_type_gate_verified':True,
          'checked_named_node_reset_sync_verified':True,'full_collada_construction_verified':False,
          'new_android_rendering_run':False,'runtime_objects_verified':False,
          'factory_contract_agreed':False,'full_loader_verified':False,
          'historical_checkpoint_note':'Prior module checkpoint retains its original source/receipt hashes; this supplement does not recertify its 82 Android frames.'}
(root/'reports/visual-root-policy-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','placement_cases_per_variant','checked_named_node_reset_sync_verified','new_android_rendering_run','full_loader_verified')}))
