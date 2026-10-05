"""Bind the isolated phase-dispatch milestone to current sources and artifacts."""
import hashlib,json,pathlib,subprocess,sys
root=pathlib.Path(__file__).resolve().parents[1]
worktree=root.parents[1];build=worktree.parent/'build'
assert worktree==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
reports=root/'reports'
original=json.loads((reports/'object-initialization-original.json').read_text())
assert original['validation']=='PASS' and len(original['cases'])==16
assert sum(len(r['calls']) for r in original['cases'])==182
assert sum(len(r['events']) for r in original['cases'])==269
assert original['script_sha256']==sha(root/'tests/object_initialization_original.py')
helper=worktree/'port/engine-math/tests/differential.py'
assert original['cpu_helper_sha256']==sha(helper)
for p,digest in original['capture_sha256'].items():assert sha(root/p)==digest,p
engine=pathlib.Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so')
assert original['engine_sha256']==sha(engine)
artifact_hashes={}
for folder,receipt in [('host-xml','object-initialization-host.json'),('host-sanitizers','object-initialization-sanitizers.json')]:
    result=json.loads((reports/receipt).read_text());probe=build/folder/'dh2_loader_object_initialization_probe'
    assert result['validation']=='PASS' and len(result['cases'])==16
    assert result['call_count']==182 and result['event_count']==269
    assert result['adapter_checks']=={'validation':'PASS','adapter_checks':6}
    assert result['original_receipt_sha256']==sha(reports/'object-initialization-original.json')
    assert result['probe_sha256']==sha(probe)
    for p,digest in result['source_sha256'].items():assert sha(root/p)==digest,p
    artifact_hashes[str(probe)]=sha(probe)
ar=pathlib.Path.home()/'AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-ar.exe'
for folder in ('android-arm64','android-x86_64'):
    library=build/folder/'libdh2_level_loader.a'
    members=subprocess.check_output([str(ar),'t',str(library)],timeout=15).decode().splitlines()
    assert 'object_initialization_v1.cpp.o' in members
    artifact_hashes[str(library)]=sha(library)
factory=json.loads((reports/'original-object-factories.json').read_text())
assert factory['validation']=='PASS' and len(factory['registry'])==33 and factory['xml_documents_parsed']==2171
assert factory['script_sha256']==sha(root/'tools/inspect_original_object_factories.py')
assert factory['xml_reference_sha256']==sha(reports/'xml-original-comparison.json')
assert factory['engine_sha256']==original['engine_sha256']
links=factory['unregistered_declarations']
assert len(links)==518 and {r['gametype'] for r in links}=={'link'}
assert sum(r['uri'].lower().endswith('.mgp') for r in links)==15
visible=json.loads((reports/'visible-emulator-window.json').read_text())
assert visible['booted'] and visible['serial']=='emulator-5590'
assert any(w['visible'] for w in visible['windows'])
paths=['object_initialization_v1.hpp','object_initialization_v1.cpp','CMakeLists.txt',
       'tests/object_initialization_original.py','tests/object_initialization_probe.cpp','tests/object_initialization_host.py',
       'tools/inspect_original_object_factories.py','tools/summarize_original_object_factories.py',
       'tools/capture_object_initialization_checkpoint.py','OBJECT-INITIALIZATION-HANDOFF.md','INTERFACE-PROPOSAL.md']
paths+=['reports/'+p for p in ['object-initialization-original.json','object-initialization-host.json',
                              'object-initialization-sanitizers.json','original-object-factories.json','visible-emulator-window.json']]
report={'validation':'PASS','scope':__doc__,'source_and_receipt_sha256':{p:sha(root/p) for p in paths},
        'built_artifact_sha256':artifact_hashes,'engine_sha256':original['engine_sha256'],
        'original_dispatch_fixtures':16,'original_dispatch_calls':182,'original_service_events':269,
        'host_and_sanitizer_matches':True,'adapter_guard_checks_per_build':6,
        'android_library_members_verified':['android-arm64','android-x86_64'],
        'original_factory_entries':33,'raw_xml_inventory_documents':2171,
        'unregistered_link_declarations':518,'mgp_link_declarations':15,
        'mgp_link_entry_policy_supplement':'OBJECT-ENTRY-HANDOFF.md; reports/object-entry-checkpoint.json',
        'visible_emulator_receipt':visible,'new_apk_installed':False,
        'runtime_factory_contract_agreed':False,'live_object_initialization_integrated':False,
        'mob_and_chest_rendering_verified':False,'cleanup_and_transitions_verified':False,'full_loader_verified':False}
path=reports/'object-initialization-checkpoint.json';path.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','checkpoint':str(path),'fixtures':16,'calls':182,'events':269,'full_loader_verified':False}))
