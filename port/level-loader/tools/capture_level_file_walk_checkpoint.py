"""Verify ready-buffer caller traversal; gameplay construction remains unverified."""
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
artifacts={};ownership={};members={}
for folder,receipt in [('host-xml','level-file-walk-host.json'),('host-sanitizers','level-file-walk-sanitizers.json')]:
    actual=json.loads((reports/receipt).read_text());probe=build/folder/'dh2_loader_level_file_walk_probe'
    assert actual['validation']=='PASS' and actual['fixtures_compared']==12 and actual['cache_files_compared']==1627
    assert actual['adapter_checks']==6 and len(actual['cases'])==1639
    assert actual['original_receipt_sha256']==sha(original_path) and actual['probe_sha256']==sha(probe)
    for path,digest in actual['source_sha256'].items():assert sha(root/path)==digest,path
    artifacts[str(probe)]=sha(probe)
    owner_probe=build/folder/'dh2_loader_xml_ownership'
    linux='/mnt/c/'+owner_probe.as_posix()[3:]
    owner=json.loads(subprocess.check_output(['wsl.exe','-d','Ubuntu','--',linux],timeout=15))
    assert owner['validation']=='PASS';ownership[folder]=owner;artifacts[str(owner_probe)]=sha(owner_probe)
    entry=json.loads((reports/('object-entry-host.json' if folder=='host-xml' else 'object-entry-sanitizers.json')).read_text())
    assert entry['validation']=='PASS' and entry['original_lookup_cases_compared']==69 and entry['original_xml_entry_cases_compared']==43
    assert entry['probe_sha256']==sha(build/folder/'dh2_loader_object_entry_probe')
    for path,digest in entry['source_sha256'].items():assert sha(root/path)==digest,path
san_cache=(build/'host-sanitizers/CMakeCache.txt').read_text()
assert 'CMAKE_CXX_FLAGS:STRING=-fsanitize=address,undefined' in san_cache
ar=pathlib.Path.home()/'AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-ar.exe'
for folder in ('android-arm64','android-x86_64'):
    library=build/folder/'libdh2_level_loader.a'
    entries=subprocess.check_output([str(ar),'t',str(library)],timeout=15).decode().splitlines()
    for member in ('xml_document_v1.cpp.o','level_file_walk_v1.cpp.o','object_entry_v1.cpp.o'):
        assert member in entries,(folder,member)
        obj=subprocess.check_output([str(ar),'p',str(library),member],timeout=15)
        assert obj.startswith(b'\x7fELF');members[folder+':'+member]=hashlib.sha256(obj).hexdigest()
    artifacts[str(library)]=sha(library)
rows=original['fixtures']+original['cache_files']
paths=['CMakeLists.txt','level_file_walk_v1.hpp','level_file_walk_v1.cpp','xml_document_v1.hpp','xml_document_v1.cpp',
       'tests/level_file_walk_original.py','tests/xml_original_probe.py','tests/level_file_walk_probe.cpp',
       'tests/level_file_walk_host.py','tests/xml_document.cpp','LEVEL-FILE-WALK-HANDOFF.md','INTERFACE-PROPOSAL.md',
       'tools/capture_level_file_walk_checkpoint.py']
paths+=['reports/'+name for name in ('level-file-walk-original.json','level-file-walk-host.json',
                                   'level-file-walk-sanitizers.json','object-entry-host.json','object-entry-sanitizers.json')]
report={'validation':'PASS','scope':__doc__,'source_and_receipt_sha256':{path:sha(root/path) for path in paths},
        'built_artifact_sha256':artifacts,'android_member_sha256':members,'xml_ownership':ownership,
        'cache_files_compared':1627,'fixtures_compared':12,'adapter_checks_per_build':6,
        'polls_compared_per_build':sum(len(row['calls']) for row in rows),
        'element_callbacks_compared_per_build':sum(e['kind']=='load_element' for row in rows for e in row['events']),
        'parse_failures':[{'label':row['label'],'error':row['original_xml_error']} for row in rows if not row['parse_success']],
        'unsafe_roots':[row['label'] for row in rows if row['unsafe']],
        'cache_sha256':original['cache_sha256'],'engine_sha256':original['engine_sha256'],
        'host_and_sanitizer_matches':True,'ready_buffer_caller_verified':True,
        'initial_open_async_state_verified':False,'runtime_factory_contract_agreed':False,
        'runtime_objects_verified':False,'mob_and_chest_rendering_verified':False,
        'new_apk_installed':False,'full_loader_verified':False}
path=reports/'level-file-walk-checkpoint.json';path.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','cache_files_compared','fixtures_compared',
                                      'polls_compared_per_build','element_callbacks_compared_per_build','parse_failures','full_loader_verified')}))
