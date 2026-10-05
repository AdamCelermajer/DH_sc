"""Verify connected ZIP/file/module source preparation, not live object loading."""
import hashlib,json,pathlib,subprocess
root=pathlib.Path(__file__).resolve().parents[1];worktree=root.parents[1]
build=worktree.parent/'build';reports=root/'reports'
assert worktree==pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
original_path=reports/'level-file-walk-original.json';original=json.loads(original_path.read_text())
assert original['validation']=='PASS' and len(original['fixtures'])==12 and len(original['cache_files'])==1627
assert original['script_sha256']==sha(root/'tests/level_file_walk_original.py')
assert original['parser_helper_sha256']==sha(root/'tests/xml_original_probe.py')
assert original['engine_sha256']==sha(pathlib.Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so'))
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
with cache.open('rb') as stream:assert hashlib.file_digest(stream,'sha256').hexdigest()==original['cache_sha256']
for path,digest in original['capture_sha256'].items():assert sha(root/path)==digest,path
artifacts={};members={};fixture_hashes=[]
for folder,receipt,fixture in [('host-xml','cached-level-file-host.json','cached-file-fixtures-host.zip'),
                              ('host-sanitizers','cached-level-file-sanitizers.json','cached-file-fixtures-sanitizers.zip')]:
    actual=json.loads((reports/receipt).read_text());probe=build/folder/'dh2_loader_cached_level_file_probe'
    assert actual['validation']=='PASS' and actual['fixtures_compared']==12 and actual['cache_files_compared']==1627
    assert actual['adapter_checks']==12 and len(actual['cases'])==1639
    assert actual['original_receipt_sha256']==sha(original_path) and actual['probe_sha256']==sha(probe)
    assert actual['cache_sha256']==original['cache_sha256'] and actual['fixture_archive_sha256']==sha(reports/fixture)
    assert sum(row['polls_compared'] for row in actual['cases'])==21218
    assert sum(row['element_callbacks_compared'] for row in actual['cases'])==8191
    for path,digest in actual['source_sha256'].items():assert sha(root/path)==digest,path
    fixture_hashes.append(sha(reports/fixture));artifacts[str(probe)]=sha(probe)
assert fixture_hashes[0]==fixture_hashes[1]
assert 'CMAKE_CXX_FLAGS:STRING=-fsanitize=address,undefined' in (build/'host-sanitizers/CMakeCache.txt').read_text()
ar=pathlib.Path.home()/'AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-ar.exe'
for folder in ('android-arm64','android-x86_64'):
    library=build/folder/'libdh2_level_loader.a'
    entries=subprocess.check_output([str(ar),'t',str(library)],timeout=15).decode().splitlines()
    for member in ('cached_level_file_v1.cpp.o','module_load_v1.cpp.o','level_file_walk_v1.cpp.o','xml_document_v1.cpp.o'):
        assert member in entries,(folder,member)
        obj=subprocess.check_output([str(ar),'p',str(library),member],timeout=15)
        assert obj.startswith(b'\x7fELF');members[folder+':'+member]=hashlib.sha256(obj).hexdigest()
    artifacts[str(library)]=sha(library)
paths=['CMakeLists.txt','cached_level_file_v1.hpp','cached_level_file_v1.cpp','module_load_v1.hpp','module_load_v1.cpp',
       'level_file_walk_v1.hpp','level_file_walk_v1.cpp','object_entry_v1.hpp','object_entry_v1.cpp',
       'xml_document_v1.hpp','xml_document_v1.cpp','resource_paths_v1.hpp','resource_paths_v1.cpp',
       'tests/cached_level_file_probe.cpp','tests/cached_level_file_host.py','tests/level_file_walk_original.py',
       'tests/xml_original_probe.py','CACHED-LEVEL-FILE-HANDOFF.md','INTERFACE-PROPOSAL.md',
       'tools/capture_cached_level_file_checkpoint.py','../asset-payloads/zip_asset_pack_v1.hpp',
       '../asset-payloads/zip_asset_pack_v1.cpp','vendor/tinyxml/tinyxmlparser.cpp','vendor/tinyxml/tinyxml.cpp']
paths+=['reports/'+name for name in ('level-file-walk-original.json','cached-level-file-host.json',
                                   'cached-level-file-sanitizers.json','cached-file-fixtures-host.zip','cached-file-fixtures-sanitizers.zip')]
report={'validation':'PASS','scope':__doc__,'source_and_receipt_sha256':{path:sha(root/path) for path in paths},
        'built_artifact_sha256':artifacts,'android_member_sha256':members,'cache_files_compared':1627,
        'fixtures_compared':12,'polls_compared_per_build':21218,'element_callbacks_compared_per_build':8191,
        'adapter_checks_per_build':12,'cache_sha256':original['cache_sha256'],'engine_sha256':original['engine_sha256'],
        'host_and_sanitizer_matches':True,'connected_module_declaration_tests':True,
        'original_open_async_state_verified':False,'runtime_factory_contract_agreed':False,
        'runtime_objects_verified':False,'mob_and_chest_rendering_verified':False,
        'new_apk_installed':False,'full_loader_verified':False}
path=reports/'cached-level-file-checkpoint.json';path.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','cache_files_compared','fixtures_compared','adapter_checks_per_build','full_loader_verified')}))
