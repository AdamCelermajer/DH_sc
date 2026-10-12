"""Verify module caller ordering and native cleanup, not whole-level object loading."""
import hashlib,json,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[1];worktree=root.parents[1]
build=worktree.parent/'build';reports=root/'reports'
assert worktree==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
original_path=reports/'module-load-original.json';original=json.loads(original_path.read_text())
assert original['validation']=='PASS' and len(original['cases'])==9
assert original['script_sha256']==sha(root/'tests/module_load_original.py')
assert original['parser_helper_sha256']==sha(root/'tests/xml_original_probe.py')
assert original['engine_sha256']==sha(pathlib.Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so'))
for path,digest in original['capture_sha256'].items():assert sha(root/path)==digest,path
artifacts={};members={}
for folder,receipt in [('host-xml','module-load-host.json'),('host-sanitizers','module-load-sanitizers.json')]:
    actual=json.loads((reports/receipt).read_text());probe=build/folder/'dh2_loader_module_load_probe'
    assert actual['validation']=='PASS' and actual['cases_compared']==9 and actual['file_polls_compared']==39
    assert actual['adapter_checks']==12 and actual['original_receipt_sha256']==sha(original_path)
    assert actual['probe_sha256']==sha(probe)
    for path,digest in actual['source_sha256'].items():assert sha(root/path)==digest,path
    artifacts[str(probe)]=sha(probe)
assert 'CMAKE_CXX_FLAGS:STRING=-fsanitize=address,undefined' in (build/'host-sanitizers/CMakeCache.txt').read_text()
ar=pathlib.Path.home()/'AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-ar.exe'
for folder in ('android-arm64','android-x86_64'):
    library=build/folder/'libdh2_level_loader.a'
    entries=subprocess.check_output([str(ar),'t',str(library)],timeout=15).decode().splitlines()
    for member in ('module_load_v1.cpp.o','level_file_walk_v1.cpp.o','object_entry_v1.cpp.o','xml_document_v1.cpp.o'):
        assert member in entries,(folder,member)
        obj=subprocess.check_output([str(ar),'p',str(library),member],timeout=15)
        assert obj.startswith(b'\x7fELF');members[folder+':'+member]=hashlib.sha256(obj).hexdigest()
    artifacts[str(library)]=sha(library)
paths=['CMakeLists.txt','module_load_v1.hpp','module_load_v1.cpp','object_entry_v1.hpp','object_entry_v1.cpp',
       'xml_document_v1.hpp','xml_document_v1.cpp','tests/module_load_original.py','tests/xml_original_probe.py',
       'tests/module_load_probe.cpp','tests/module_load_host.py','MODULE-LOAD-HANDOFF.md','INTERFACE-PROPOSAL.md',
       'tools/capture_module_load_checkpoint.py']
paths+=['reports/'+name for name in ('module-load-original.json','module-load-host.json','module-load-sanitizers.json')]
report={'validation':'PASS','scope':__doc__,'source_and_receipt_sha256':{path:sha(root/path) for path in paths},
        'built_artifact_sha256':artifacts,'android_member_sha256':members,'original_cases_compared':9,
        'file_service_polls_compared_per_build':39,'adapter_checks_per_build':12,
        'engine_sha256':original['engine_sha256'],'host_and_sanitizer_matches':True,
        'original_module_caller_order_verified':True,'original_selection_verified':False,
        'original_file_loading_verified_by_this_comparison':False,'combined_live_module_loading_verified':False,
        'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,
        'mob_and_chest_rendering_verified':False,'new_apk_installed':False,'full_loader_verified':False}
path=reports/'module-load-checkpoint.json';path.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','original_cases_compared':9,'file_polls':39,'adapter_checks_per_build':12,'full_loader_verified':False}))
