"""Verify the source-backed factory/entry milestone without claiming live objects."""
import hashlib,json,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[1]
worktree=root.parents[1];build=worktree.parent/'build';reports=root/'reports'
assert worktree==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
original_path=reports/'object-factory-dispatch-original.json';original=json.loads(original_path.read_text())
factory_path=reports/'original-object-factories.json';factory=json.loads(factory_path.read_text())
assert original['validation']=='PASS' and original['factory_receipt_sha256']==sha(factory_path)
assert original['script_sha256']==sha(root/'tests/object_factory_dispatch_original.py')
assert original['cpu_helper_sha256']==sha(worktree/'port/engine-math/tests/differential.py')
assert original['engine_sha256']==sha(pathlib.Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so'))
assert len(original['factory_dispatch_cases'])==102 and len(original['mgp_link_entry_cases'])==30 and len(original['xml_gate_cases'])==13
for p,digest in original['capture_sha256'].items():assert sha(root/p)==digest,p
assert factory['script_sha256']==sha(root/'tools/inspect_original_object_factories.py')
assert factory['xml_reference_sha256']==sha(reports/'xml-original-comparison.json')
assert len(factory['registry'])==33
for row in original['mgp_link_entry_cases']:
    assert row['returned_without_object'] and row['events'][-1]=={'kind':'resolve_handle','required':False,'null':True}
    assert not any(e['kind']=='factory_callback' for e in row['events'])
table=(root/'original_factory_table_v1.inc').read_text()
assert sha(factory_path) in table
for row in factory['registry']:assert '{"'+row['gametype']+'", '+row['factory_address']+'u},' in table
artifacts={}
for folder,receipt in [('host-xml','object-entry-host.json'),('host-sanitizers','object-entry-sanitizers.json')]:
    actual=json.loads((reports/receipt).read_text());probe=build/folder/'dh2_loader_object_entry_probe'
    assert actual['validation']=='PASS' and actual['registry_entries_compared']==33
    assert actual['original_lookup_cases_compared']==69 and actual['original_xml_entry_cases_compared']==43
    assert actual['unsafe_adapter_case']==1 and len(actual['cases'])==113
    assert actual['original_receipt_sha256']==sha(original_path) and actual['factory_receipt_sha256']==sha(factory_path)
    for p,digest in actual['source_sha256'].items():assert sha(root/p)==digest,p
    assert actual['probe_sha256']==sha(probe)
    artifacts[str(probe)]=sha(probe)
ar=pathlib.Path.home()/'AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-ar.exe'
for folder in ('android-arm64','android-x86_64'):
    library=build/folder/'libdh2_level_loader.a'
    assert 'object_entry_v1.cpp.o' in subprocess.check_output([str(ar),'t',str(library)],timeout=15).decode().splitlines()
    artifacts[str(library)]=sha(library)
paths=['object_entry_v1.hpp','object_entry_v1.cpp','original_factory_table_v1.inc','CMakeLists.txt',
       'tests/object_factory_dispatch_original.py','tests/object_entry_probe.cpp','tests/object_entry_host.py',
       'tools/generate_original_factory_table.py','tools/capture_object_entry_checkpoint.py',
       'OBJECT-ENTRY-HANDOFF.md','OBJECT-INITIALIZATION-HANDOFF.md','INTERFACE-PROPOSAL.md']
paths+=['reports/'+p for p in ['object-factory-dispatch-original.json','object-entry-host.json',
                              'object-entry-sanitizers.json','original-object-factories.json']]
report={'validation':'PASS','scope':__doc__,'source_and_receipt_sha256':{p:sha(root/p) for p in paths},
        'built_artifact_sha256':artifacts,'original_factory_dispatch_cases':102,
        'original_mgp_link_entry_cases':30,'original_xml_gate_cases':13,'native_registry_entries_compared':33,
        'native_lookup_cases_per_build':69,'native_xml_entry_cases_per_build':43,'unsafe_native_adapter_cases_per_build':1,
        'host_and_sanitizer_matches':True,'mgp_link_entry_policy':'original null handle; retained explicit unregistered record',
        'full_file_caller_policy_verified':False,'runtime_factory_contract_agreed':False,
        'runtime_objects_verified':False,'mob_and_chest_rendering_verified':False,'new_apk_installed':False,'full_loader_verified':False}
path=reports/'object-entry-checkpoint.json';path.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','checkpoint':str(path),'full_loader_verified':False}))
