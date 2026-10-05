"""Compare real ZIP acquisition plus retained traversal with the original caller.

The connected module tests collect retained declarations only. Class factories,
properties/templates, conditions, rendering, save and transitions remain absent.
ZIP acquisition is a native synchronous adapter, not original async streaming.
"""
import argparse,hashlib,json,pathlib,struct,subprocess,zipfile
ap=argparse.ArgumentParser()
for name in ('original','cache','probe','fixtures','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
a=ap.parse_args();root=pathlib.Path(__file__).resolve().parents[1]
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
original=json.loads(a.original.read_text());assert original['validation']=='PASS'
assert original['script_sha256']==sha(root/'tests/level_file_walk_original.py')
assert original['parser_helper_sha256']==sha(root/'tests/xml_original_probe.py')
for path,digest in original['capture_sha256'].items():assert sha(root/path)==digest,path
with a.cache.open('rb') as stream:assert hashlib.file_digest(stream,'sha256').hexdigest()==original['cache_sha256']
prefix='com.gameloft.android.GAND.GloftD2SS/files/'
fixture_bytes={'fixture/'+name+'.xml':raw.encode() for name,raw in original['fixture_source'].items()}
fixture_bytes['fixture/unsupported_nul.xml']=b'<Module>\0<GameObject name="a" gametype="Decor"/></Module>'
fixture_bytes['data/scene/alias.mgp']=fixture_bytes['fixture/single.xml']
# Deterministic private fixture archive. Canonical cache is never modified.
with zipfile.ZipFile(a.fixtures,'w') as pack:
    for uri,raw in fixture_bytes.items():
        info=zipfile.ZipInfo(prefix+uri,(2000,1,1,0,0,0));info.compress_type=zipfile.ZIP_DEFLATED
        pack.writestr(info,raw)
rows=original['fixtures']+original['cache_files'];payload=bytearray(struct.pack('<I',len(rows)))
with zipfile.ZipFile(a.cache) as pack:
    for index,row in enumerate(rows):
        fixture=index<len(original['fixtures']);uri='fixture/'+row['label']+'.xml' if fixture else row['label']
        raw=fixture_bytes[uri] if fixture else pack.read(prefix+uri)
        assert hashlib.sha256(raw).hexdigest()==row['raw_sha256'] and len(raw)==row['raw_length']
        payload+=struct.pack('<I',int(fixture))
        for blob in (row['label'].encode(),uri.encode(),row['requested'].encode(),raw):payload+=struct.pack('<I',len(blob))+blob
native=json.loads(subprocess.check_output([str(a.probe),str(a.cache),str(a.fixtures)],input=payload,timeout=60))
assert len(native['cases'])==len(rows) and native['adapter_checks']==12
checked=[]
for wanted,actual in zip(rows,native['cases']):
    label=wanted['label'];assert actual['label']==label
    assert actual['parse_success']==wanted['parse_success'],(label,'parse',wanted,actual)
    assert actual['unsafe']==bool(wanted['unsafe']) and actual['raw_source_preserved'],label
    expected=[{k:v for k,v in event.items() if not(event['kind']=='parse_result' and k=='error')}
              for event in wanted['events'] if event['kind']!='unsafe_null_first_child_element']
    assert actual['events']==expected,(label,'events',expected,actual['events'])
    assert actual['calls']==wanted['calls'],(label,'calls',wanted['calls'],actual['calls'])
    assert actual['failure_cleanup_poll']==wanted.get('failure_cleanup_poll'),(label,'cleanup',wanted,actual)
    checked.append({'label':label,'parse_success':actual['parse_success'],'unsafe_root_reported':actual['unsafe'],
                    'polls_compared':len(actual['calls']),
                    'element_callbacks_compared':sum(event['kind']=='load_element' for event in expected),
                    'raw_source_preserved':True})
paths=('CMakeLists.txt','cached_level_file_v1.hpp','cached_level_file_v1.cpp','module_load_v1.hpp','module_load_v1.cpp',
       'level_file_walk_v1.hpp','level_file_walk_v1.cpp','xml_document_v1.hpp','xml_document_v1.cpp',
       'resource_paths_v1.hpp','resource_paths_v1.cpp','tests/cached_level_file_probe.cpp','tests/cached_level_file_host.py')
report={'validation':'PASS','scope':__doc__,'original_receipt_sha256':sha(a.original),'probe_sha256':sha(a.probe),
        'cache_sha256':original['cache_sha256'],'fixture_archive_sha256':sha(a.fixtures),
        'script_sha256':sha(pathlib.Path(__file__)),'source_sha256':{path:sha(root/path) for path in paths},
        'input_sha256':hashlib.sha256(payload).hexdigest(),'fixtures_compared':len(original['fixtures']),
        'cache_files_compared':len(original['cache_files']),'adapter_checks':native['adapter_checks'],
        'cases':checked,'connected_module_declaration_tests':True,'original_open_async_state_verified':False,
        'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','fixtures_compared','cache_files_compared','adapter_checks','full_loader_verified')}))
