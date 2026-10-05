"""Bind current MGX, connection and typed room-list declaration evidence."""
import ast,hashlib,json,pathlib
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader';build=root.parent/'build'
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
receipts={};helpers={}
for stage in ('blocks','connections','lists'):
    path=loader/'reports'/('procedural-'+stage+'-original.json');original=json.loads(path.read_text())
    assert original['original_execution_complete']
    assert sha(loader/'tests'/('procedural_'+stage+'_original.py'))==original['script_sha256']
    for name,expected in original['dependency_sources_sha256'].items():
        assert sha(root/name)==expected,name;helpers[name]=expected
    if stage=='blocks':assert original['authored_blocks_requested']==250 and len(original['cases'])==287
    elif stage=='connections':
        assert len(original['cases'])==25 and original['direction_opposites']==[2,3,0,1,4]
        lists_path=loader/'reports/procedural-file-lists-original.json'
        assert sha(lists_path)==original['original_lists_sha256'];receipts[lists_path.name]=sha(lists_path)
    else:
        assert len(original['cases'])==44
        assert sha(loader/'reports/procedural-connections-original.json')==original['connections_receipt_sha256']
    receipts[path.name]=sha(path)
    for sanitizer in (False,True):
        name='procedural-'+stage+'-'+('sanitizers' if sanitizer else 'host')+'.json'
        report_path=loader/'reports'/name;report=json.loads(report_path.read_text())
        assert report['validation']=='PASS' and report['sanitizers']==sanitizer
        assert report['summary']=={'original_compared':35,'fixed_definition':16}
        assert not report['layout_generation_verified'] and not report['full_loader_verified']
        assert report['original_receipt_sha256']==sha(path)
        if stage=='blocks':assert report['authored_blocks_compared']==250 and report['block_occurrences_compared']==603 and report['exit_occurrences_compared']==1073
        elif stage=='connections':assert report['authored_block_pools_compared']==21 and report['selected_block_occurrences_compared']==603 and report['connection_reference_occurrences_compared']==10550
        else:
            assert report['authored_rule_files_compared']==35 and report['list_declaration_occurrences_compared']==171
            assert report['list_element_occurrences_compared']==479 and report['unavailable_block_reference_occurrences']==0
            assert len(report['checked_domain_rejections'])==3
        for source,expected in report['sources_sha256'].items():assert sha(loader/source)==expected,source
        variant='host-sanitizers' if sanitizer else 'host-xml'
        assert sha(build/variant/('dh2_loader_procedural_'+stage+'_probe'))==report['probe_sha256']
        receipts[name]=sha(report_path)
sources={}
for path in loader.iterdir():
    if path.suffix in ('.cpp','.hpp') or path.name=='CMakeLists.txt':sources[str(path.relative_to(root))]=sha(path)
for folder in ('tests','reference/procedural-functions','reference/procedural-map','reference/string-conversions'):
    for path in (loader/folder).glob('*'):
        if path.is_file() and (folder.startswith('reference') or path.name.startswith('procedural_')):
            sources[str(path.relative_to(root))]=sha(path)
sources.update(helpers)
for name in ('README.md','PROCEDURAL-LISTS-MILESTONE-HANDOFF.md','tools/capture_procedural_lists_checkpoint.py',
             'tools/inspect_procedural_rules.py','tools/inspect_original_rule_vtables.py'):
    path=loader/name;sources[str(path.relative_to(root))]=sha(path)
    if path.suffix=='.py':ast.parse(path.read_text())
for name in ('procedural_lists_v1.cpp','procedural_lists_v1.hpp','tests/procedural_lists_probe.cpp',
             'tests/procedural_lists_original.py','tests/procedural_lists_differential.py'):
    text=(loader/name).read_text();assert all(line==line.rstrip() for line in text.splitlines()),name
    if name.endswith('.py'):ast.parse(text)
binaries={}
for variant in ('host-xml','host-sanitizers','android-arm64','android-x86_64'):
    for name in ('libdh2_level_loader.a','dh2_loader_procedural_blocks_probe','dh2_loader_procedural_connections_probe','dh2_loader_procedural_lists_probe'):
        binaries[variant+'/'+name]=sha(build/variant/name)
report={'validation':'PASS','scope':__doc__,'sources_sha256':sources,'binaries_sha256':binaries,'receipts_sha256':receipts,
    'authored_block_pools_compared':21,'procedural_level_rows_compared':35,'selected_block_occurrences_compared':603,
    'connection_reference_occurrences_compared':10550,'list_declaration_occurrences_compared':171,
    'list_element_occurrences_compared':479,'unavailable_block_reference_occurrences':0,
    'android_execution_verified':False,'full_load_blocks_verified':False,'layout_generation_verified':False,
    'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
(loader/'reports/procedural-lists-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','sources':len(sources),'binaries':len(binaries),'receipts':len(receipts),
    'procedural_level_rows_compared':35,'list_declaration_occurrences_compared':171,'list_element_occurrences_compared':479}))
