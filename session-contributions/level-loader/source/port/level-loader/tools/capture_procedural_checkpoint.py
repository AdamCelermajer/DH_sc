"""Bind the procedural preparation milestone, preserving prior checkpoints."""
import hashlib,json,pathlib
root=pathlib.Path(__file__).resolve().parents[3];loader=root/'port/level-loader';build=root.parent/'build'
def sha(path):
    with path.open('rb') as file:return hashlib.file_digest(file,'sha256').hexdigest()
receipts={}
for kind,stem in (('random','procedural-random'),('file_lists','procedural-file-lists'),('sources','procedural-sources')):
    for sanitizer in (False,True):
        suffix='sanitizers' if sanitizer else 'host' if kind=='sources' else 'differential'
        name=stem+'-'+suffix+'.json';path=loader/'reports'/name;report=json.loads(path.read_text())
        assert report['validation']=='PASS' and not report['layout_generation_verified'] and not report['full_loader_verified']
        assert report['sanitizers']==sanitizer
        for source,expected in report['sources_sha256'].items():assert sha(loader/source)==expected,source
        variant='host-sanitizers' if sanitizer else 'host-xml'
        target='dh2_loader_procedural_'+('file_list' if kind=='file_lists' else kind)+'_probe'
        assert sha(build/variant/target)==report['probe_sha256'],target
        if kind=='file_lists':assert sha(loader/'reports/procedural-file-lists-original.json')==report['original_receipt_sha256']
        if kind=='sources':
            assert report['summary']=={'source_compared':35,'fixed_definition':16}
            assert report['block_occurrences_compared']==603
            assert sha(loader/'reports/procedural-file-lists-original.json')==report['original_lists_sha256']
        receipts[name]=sha(path)
original_path=loader/'reports/procedural-file-lists-original.json';original=json.loads(original_path.read_text())
assert original['original_execution_complete'] and len(original['cases'])==32
assert sha(loader/'tests/procedural_file_list_original.py')==original['script_sha256']
receipts[original_path.name]=sha(original_path)
sources={str(p.relative_to(root)):sha(p) for p in loader.iterdir() if p.suffix in ('.cpp','.hpp') or p.name=='CMakeLists.txt'}
for folder in ('tests','reference/procedural-functions'):
    for p in (loader/folder).glob('*'):
        if p.is_file() and (folder.startswith('reference') or p.name.startswith('procedural_')):sources[str(p.relative_to(root))]=sha(p)
binaries={}
for variant in ('host-xml','host-sanitizers','android-arm64','android-x86_64'):
    for name in ('libdh2_level_loader.a','dh2_loader_procedural_random_probe','dh2_loader_procedural_file_list_probe','dh2_loader_procedural_sources_probe'):
        binaries[variant+'/'+name]=sha(build/variant/name)
swamp2=loader/'reports/swamp2-procedural-sources.json';source=json.loads(swamp2.read_text())
assert source['identity']=='SWAMP_02' and source['document_count']==42 and len(source['blocks'])==41
report={'validation':'PASS','scope':__doc__,'sources_sha256':sources,'binaries_sha256':binaries,'receipts_sha256':receipts,
    'swamp2_source_sha256':sha(swamp2),'procedural_source_rows':35,'block_occurrences_compared':603,
    'android_execution_verified':False,'layout_generation_verified':False,'runtime_factory_contract_agreed':False,
    'runtime_objects_verified':False,'full_loader_verified':False}
(loader/'reports/procedural-checkpoint.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','sources':len(sources),'binaries':len(binaries),'receipts':len(receipts),
                  'procedural_source_rows':35,'block_occurrences_compared':603}))
