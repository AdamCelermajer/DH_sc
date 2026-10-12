"""Bind the five native procedural stages and original caller/constructor evidence."""
import ast,hashlib,json,pathlib
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader';build=root.parent/'build'
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
receipts={};helpers={}
counts={'blocks':287,'connections':25,'lists':44,'rules':54,'instances':50}
for stage in counts:
    path=loader/'reports'/('procedural-'+stage+'-original.json');original=json.loads(path.read_text())
    assert original['original_execution_complete'] and len(original['cases'])==counts[stage]
    assert sha(loader/'tests'/('procedural_'+stage+'_original.py'))==original['script_sha256']
    assert original['engine_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
    assert original['cache_sha256']=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    for name,want in original['dependency_sources_sha256'].items():
        assert sha(root/name)==want,name;helpers[name]=want
    if stage=='blocks':assert original['authored_blocks_requested']==250
    elif stage=='connections':
        assert original['direction_opposites']==[2,3,0,1,4]
        previous=loader/'reports/procedural-file-lists-original.json'
        assert sha(previous)==original['original_lists_sha256'];receipts[previous.name]=sha(previous)
    elif stage=='lists':assert sha(loader/'reports/procedural-connections-original.json')==original['connections_receipt_sha256']
    elif stage=='rules':
        assert original['authored_rule_files_requested']==35 and original['synthetic_cases_requested']==19
        assert sum('checked_domain_rejection' in row for row in original['cases'])==5
        assert sha(loader/'reports/procedural-lists-original.json')==original['lists_receipt_sha256']
        assert original['original_static_tokens']=={'room_pool':'pool','pool_element':'elem'}
    else:
        assert sha(loader/'reports/procedural-rules-original.json')==original['rules_receipt_sha256']
        assert len(original['caller_cases'])==32
        for row in original['caller_cases']:
            expected=(['destroy_old'] if row['old_generator'] else [])+['construct','load','generate','serialize','destroy_new']
            assert row['events']==expected and row['return']==row['generate_result'] and row['generator_cleared']
        assert len([row for row in original['cases'] if row['name'].startswith('data/')])==35
        assert not original['layout_generation_verified'] and not original['pool_size_allocation_verified']
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
        elif stage=='lists':assert report['authored_rule_files_compared']==35 and report['list_declaration_occurrences_compared']==171 and report['list_element_occurrences_compared']==479 and report['unavailable_block_reference_occurrences']==0
        elif stage=='rules':
            assert report['authored_rule_files_compared']==35 and report['original_reader_results']=={'1':29,'0':6}
            assert report['authored_totals']=={'rules':236,'rules_returning_false':38,'unresolved_list_references':0,
                'unavailable_block_references':0,'pools':0,'pool_elements':0}
            assert len(report['additional_domain_rejections'])==4
        else:
            assert report['authored_definitions_compared']==35
            assert report['authored_totals']=={'rule_nodes':236,'instances_compared':2185,'original_random_calls':230}
            assert report['caller_reader_failure_policy_verified'] and report['caller_boundary_cases']==32
            assert not report['runtime_graph_registration_verified'] and not report['pool_size_allocation_verified']
            assert all(row['ownership_checks'] for row in report['levels'] if row['instances']=='original_compared')
        for source,want in report['sources_sha256'].items():assert sha(loader/source)==want,source
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
for name in ('README.md','PROCEDURAL-INSTANCES-MILESTONE-HANDOFF.md','tools/capture_procedural_instances_checkpoint.py'):
    path=loader/name;sources[str(path.relative_to(root))]=sha(path)
    if path.suffix=='.py':ast.parse(path.read_text())
for name in ('procedural_instances_v1.cpp','procedural_instances_v1.hpp','tests/procedural_instances_probe.cpp',
             'tests/procedural_instances_original.py','tests/procedural_instances_differential.py'):
    text=(loader/name).read_text();assert all(line==line.rstrip() for line in text.splitlines()),name
    if name.endswith('.py'):ast.parse(text)
for name in ('port/asset-payloads/zip_asset_pack_v1.cpp','port/asset-payloads/zip_asset_pack_v1.hpp'):
    sources[name]=sha(root/name)
binaries={};configurations={}
for variant in ('host-xml','host-sanitizers','android-arm64','android-x86_64'):
    for name in ('libdh2_level_loader.a',*[('dh2_loader_procedural_'+stage+'_probe') for stage in counts]):
        binaries[variant+'/'+name]=sha(build/variant/name)
    configurations[variant]=sha(build/variant/'CMakeCache.txt')
report={'validation':'PASS','scope':__doc__,'sources_sha256':sources,'binaries_sha256':binaries,
        'build_configuration_sha256':configurations,'receipts_sha256':receipts,
        'procedural_level_rows_compared':35,'rule_node_occurrences_compared':236,
        'runtime_constructor_instances_compared':2185,'original_constructor_random_calls':230,
        'original_root_reader_results':{'1':29,'0':6},'caller_boundary_cases':32,
        'caller_reader_failure_policy_verified':True,'caller_service_bodies_verified':False,
        'runtime_graph_registration_verified':False,'pool_size_allocation_verified':False,
        'layout_generation_verified':False,'android_execution_verified':False,
        'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
(loader/'reports/procedural-instances-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','sources':len(sources),'binaries':len(binaries),'receipts':len(receipts),
                  'procedural_level_rows_compared':35,'runtime_constructor_instances_compared':2185,'caller_boundary_cases':32}))
