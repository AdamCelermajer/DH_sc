"""Bind the original-verified MGX block milestone without replacing old proofs."""
import ast,hashlib,json,pathlib
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader';build=root.parent/'build'
for name in ('procedural_blocks_v1.hpp','procedural_blocks_v1.cpp','xml_document_v1.hpp','xml_document_v1.cpp',
             'tests/procedural_blocks_probe.cpp','tests/procedural_blocks_original.py',
             'tests/procedural_blocks_differential.py','tools/capture_procedural_blocks_checkpoint.py'):
    text=(loader/name).read_text()
    assert all(line==line.rstrip() for line in text.splitlines()),name
    if name.endswith('.py'):ast.parse(text)
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
original_path=loader/'reports/procedural-blocks-original.json';original=json.loads(original_path.read_text())
assert original['original_execution_complete'] and len(original['cases'])==287 and original['authored_blocks_requested']==250
assert sha(loader/'tests/procedural_blocks_original.py')==original['script_sha256']
for name,expected in original['dependency_sources_sha256'].items():assert sha(root/name)==expected,name
receipts={original_path.name:sha(original_path)}
for sanitizer in (False,True):
    name='procedural-blocks-'+('sanitizers' if sanitizer else 'host')+'.json'
    path=loader/'reports'/name;report=json.loads(path.read_text())
    assert report['validation']=='PASS' and report['sanitizers']==sanitizer
    assert report['summary']=={'original_compared':35,'fixed_definition':16}
    assert report['authored_blocks_compared']==250 and report['block_occurrences_compared']==603
    assert report['exit_occurrences_compared']==1073 and report['rejected_link_occurrences_compared']==0
    assert not report['connection_graph_verified'] and not report['layout_generation_verified'] and not report['full_loader_verified']
    assert report['original_receipt_sha256']==sha(original_path)
    for source,expected in report['sources_sha256'].items():assert sha(loader/source)==expected,source
    variant='host-sanitizers' if sanitizer else 'host-xml'
    assert sha(build/variant/'dh2_loader_procedural_blocks_probe')==report['probe_sha256']
    receipts[name]=sha(path)
sources={}
for path in loader.iterdir():
    if path.suffix in ('.cpp','.hpp') or path.name=='CMakeLists.txt':sources[str(path.relative_to(root))]=sha(path)
for folder in ('tests','reference/procedural-functions','reference/string-conversions'):
    for path in (loader/folder).glob('*'):
        if path.is_file() and (folder.startswith('reference') or path.name.startswith('procedural_')):
            sources[str(path.relative_to(root))]=sha(path)
for name,expected in original['dependency_sources_sha256'].items():sources[name]=expected
sources[str(pathlib.Path(__file__).resolve().relative_to(root))]=sha(pathlib.Path(__file__))
for name in ('README.md','PROCEDURAL-BLOCKS-MILESTONE-HANDOFF.md'):
    sources[str((loader/name).relative_to(root))]=sha(loader/name)
binaries={}
for variant in ('host-xml','host-sanitizers','android-arm64','android-x86_64'):
    for name in ('libdh2_level_loader.a','dh2_loader_procedural_blocks_probe'):
        binaries[variant+'/'+name]=sha(build/variant/name)
report={'validation':'PASS','scope':__doc__,'sources_sha256':sources,'binaries_sha256':binaries,'receipts_sha256':receipts,
    'cache_sha256':original['cache_sha256'],'engine_sha256':original['engine_sha256'],
    'authored_blocks_compared':250,'procedural_level_rows_compared':35,'block_occurrences_compared':603,
    'exit_occurrences_compared':1073,'historical_bionic_numeric_parsing_verified':False,
    'android_execution_verified':False,'connection_graph_verified':False,'layout_generation_verified':False,
    'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
(loader/'reports/procedural-blocks-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','sources':len(sources),'binaries':len(binaries),'receipts':len(receipts),
    'authored_blocks_compared':250,'procedural_level_rows_compared':35,'block_occurrences_compared':603,'exit_occurrences_compared':1073}))
