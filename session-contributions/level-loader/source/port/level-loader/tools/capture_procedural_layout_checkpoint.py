"""Bind original procedural generation evidence; native generation is pending."""
import ast,hashlib,json,pathlib,zipfile
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader'
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
path=loader/'reports/procedural-layout-original.json';report=json.loads(path.read_text())
assert report['original_execution_complete'] and report['selected_execution_complete']
assert not report['native_layout_generation_verified'] and not report['full_loader_verified']
assert report['requested_cases']==39 and len(report['cases'])==39
assert report['engine_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert report['cache_sha256']=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
sources=dict(report['dependency_sources_sha256'])
for name,want in sources.items():assert sha(root/name)==want,name
assert sha(loader/'tests/procedural_layout_original.py')==report['script_sha256']
receipts={path.name:sha(path)}
for key,name in (('file_lists_receipt_sha256','procedural-file-lists-original.json'),
                 ('rules_receipt_sha256','procedural-rules-original.json')):
    receipt=loader/'reports'/name;assert sha(receipt)==report[key];receipts[name]=sha(receipt)
cache=pathlib.Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache)==report['cache_sha256']
totals={'definitions':0,'runs':0,'successful_runs':0,'no_layout_runs':0,'tile_occurrences':0,
        'random_calls':0,'place_calls':0,'unspawn_calls':0,'root_reader_false_definitions':0}
synthetics={};levels=[]
with zipfile.ZipFile(cache) as pack:
    prefix='com.gameloft.android.GAND.GloftD2SS/files/'
    for case in report['cases']:
        assert 'failure' not in case and 'checked_domain_rejection' not in case
        authored=case['name'].startswith('data/')
        raw=pack.read(prefix+case['name']) if authored else bytes.fromhex(case['input_hex'])
        assert hashlib.sha256(raw).hexdigest()==case['input_sha256']
        for block in case['blocks']:
            raw=pack.read(prefix+block['uri']) if authored else bytes.fromhex(block['input_hex'])
            assert hashlib.sha256(raw).hexdigest()==block['input_sha256']
        assert [run['seed'] for run in case['runs']]==[0,1]
        if authored:
            totals['definitions']+=1;totals['root_reader_false_definitions']+=not case['reader_result']
        else:synthetics[case['name']]=[len(run['tiles']) for run in case['runs']]
        for run in case['runs']:
            tiles=run['tiles'];assert bool(tiles)==run['success']
            assert [tile['index'] for tile in tiles]==list(range(len(tiles)))
            children=[child for tile in tiles for child in tile['children']]
            assert sorted(children)==list(range(1,len(tiles))),case['name']
            assert run['events']['place_calls']>=len(tiles)
            for tile in tiles:
                assert tile['block_name']==tile['name']
                assert tile['mgx_uri']==case['blocks'][tile['block_source']]['uri']
                assert all(child>tile['index'] for child in tile['children'])
            if authored:
                totals['runs']+=1;totals['successful_runs']+=run['success'];totals['no_layout_runs']+=not run['success']
                totals['tile_occurrences']+=len(tiles)
                for key in ('random_calls','place_calls','unspawn_calls'):totals[key]+=run['events'][key]
        if authored:levels.append({'name':case['name'],'reader_result':case['reader_result'],
                                  'runs':[{'seed':r['seed'],'success':r['success'],'tiles':len(r['tiles'])} for r in case['runs']]})
assert totals['definitions']==35 and totals['runs']==70 and totals['root_reader_false_definitions']==6
assert synthetics=={'single-root':[1,1],'three-room-force-chain':[3,3],
                   'same-block-path-rejected':[0,0],'list-root-selection':[1,1]}
reference=loader/'reference/procedural-generation';provenance_path=reference/'provenance.json'
provenance=json.loads(provenance_path.read_text());assert provenance['engine_sha256']==report['engine_sha256']
assert sha(reference/'distributions.bin')==provenance['distribution']['sha256']
assert provenance['distribution']['size']==26136 and provenance['distribution']['dimensions']==[6,6,121,6]
assert sha(loader/'tools/capture_procedural_generation_reference.py')==provenance['script_sha256']
for function in provenance['functions']:assert sha(reference/function['file'])==function['disassembly_sha256']
for file in reference.iterdir():
    if file.is_file():sources[str(file.relative_to(root))]=sha(file)
for name in ('tests/procedural_layout_original.py','tools/capture_procedural_generation_reference.py',
             'tools/capture_procedural_layout_checkpoint.py','PROCEDURAL-GENERATION-REFERENCE-HANDOFF.md'):
    source=loader/name;sources[str(source.relative_to(root))]=sha(source)
    if source.suffix=='.py':ast.parse(source.read_text())
    assert all(line==line.rstrip() for line in source.read_text().splitlines()),name
checkpoint={'validation':'PASS','scope':__doc__,'sources_sha256':sources,'receipts_sha256':receipts,
            'totals':totals,'synthetic_tile_counts':synthetics,'levels':levels,
            'original_generate_executed':True,'selection_placement_answers_are_fixtures':False,
            'from_filename_or_full_load_rule_file_executed':False,'pool_compute_executed':False,
            'original_serialization_executed':False,'repeated_load_unload_verified':False,
            'native_layout_generation_verified':False,'android_procedural_rendering_verified':False,
            'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
(loader/'reports/procedural-layout-checkpoint.json').write_text(json.dumps(checkpoint,indent=2)+'\n')
print(json.dumps({'validation':'PASS','sources':len(sources),'receipts':len(receipts),'totals':totals}))
