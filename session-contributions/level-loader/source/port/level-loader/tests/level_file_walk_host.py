"""Compare native ready-buffer traversal against executed original Level::LoadFile.

No resource-open/async stream path, gameplay construction, condition evaluation,
activation, active-level replacement or campaign restoration is verified here.
The native adapter distinguishes failure from the original terminal true return.
"""
import argparse,hashlib,json,pathlib,struct,subprocess,zipfile
ap=argparse.ArgumentParser()
for name in ('original','cache','probe','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
a=ap.parse_args();root=pathlib.Path(__file__).resolve().parents[1]
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
original=json.loads(a.original.read_text())
assert original['validation']=='PASS'
assert original['script_sha256']==sha(root/'tests/level_file_walk_original.py')
assert original['parser_helper_sha256']==sha(root/'tests/xml_original_probe.py')
for path,wanted in original['capture_sha256'].items():assert sha(root/path)==wanted
with a.cache.open('rb') as stream:assert hashlib.file_digest(stream,'sha256').hexdigest()==original['cache_sha256']
rows=original['fixtures']+original['cache_files'];payload=bytearray(struct.pack('<I',len(rows)))
prefix='com.gameloft.android.GAND.GloftD2SS/files/'
with zipfile.ZipFile(a.cache) as pack:
    for row in rows:
        raw=(original['fixture_source'][row['label']].encode() if row in original['fixtures']
             else pack.read(prefix+row['label']))
        assert hashlib.sha256(raw).hexdigest()==row['raw_sha256'] and len(raw)==row['raw_length']
        for blob in (row['label'].encode(),row['requested'].encode(),raw):
            payload+=struct.pack('<I',len(blob))+blob
native=json.loads(subprocess.check_output([str(a.probe)],input=payload,timeout=60))
assert len(native['cases'])==len(rows)
checked=[]
for wanted,actual in zip(rows,native['cases']):
    label=wanted['label'];assert actual['label']==label
    assert actual['parse_success']==wanted['parse_success'],(label,'parse',wanted['parse_success'],actual)
    assert actual['unsafe']==bool(wanted['unsafe']),(label,'unsafe',wanted,actual)
    assert actual['raw_source_preserved'],label
    expected=[]
    for event in wanted['events']:
        if event['kind']=='unsafe_null_first_child_element':continue
        expected.append({k:v for k,v in event.items() if not(event['kind']=='parse_result' and k=='error')})
    assert actual['events']==expected,(label,'events',expected,actual['events'])
    assert actual['calls']==wanted['calls'],(label,'calls',wanted['calls'],actual['calls'])
    assert actual['failure_cleanup_poll']==wanted.get('failure_cleanup_poll'),(label,'cleanup',wanted,actual)
    checked.append({'label':label,'parse_success':actual['parse_success'],'unsafe_root_reported':actual['unsafe'],
                    'polls_compared':len(actual['calls']),
                    'element_callbacks_compared':sum(e['kind']=='load_element' for e in expected),
                    'raw_source_preserved':True})
assert native['adapter_checks']==6
paths=('CMakeLists.txt','level_file_walk_v1.hpp','level_file_walk_v1.cpp','xml_document_v1.hpp','xml_document_v1.cpp',
       'tests/level_file_walk_probe.cpp','tests/level_file_walk_host.py')
report={'validation':'PASS','scope':__doc__,'original_receipt_sha256':sha(a.original),'probe_sha256':sha(a.probe),
        'script_sha256':sha(pathlib.Path(__file__)),'source_sha256':{path:sha(root/path) for path in paths},
        'input_sha256':hashlib.sha256(payload).hexdigest(),'fixtures_compared':len(original['fixtures']),
        'cache_files_compared':len(original['cache_files']),'adapter_checks':native['adapter_checks'],
        'cases':checked,'initial_open_async_state_verified':False,'runtime_factory_contract_agreed':False,
        'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','fixtures_compared','cache_files_compared','adapter_checks','full_loader_verified')}))
