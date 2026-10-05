"""Compare module context/selection/file ordering against the executed ARM caller.

Selection/file/class services are explicit modeled boundaries. Pending yielding,
failure latching and explicit unwind are native policies, not original rollback.
"""
import argparse,hashlib,json,pathlib,struct,subprocess
ap=argparse.ArgumentParser()
for name in ('original','probe','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
a=ap.parse_args();root=pathlib.Path(__file__).resolve().parents[1]
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
original=json.loads(a.original.read_text());assert original['validation']=='PASS'
assert original['script_sha256']==sha(root/'tests/module_load_original.py')
assert original['parser_helper_sha256']==sha(root/'tests/xml_original_probe.py')
for path,digest in original['capture_sha256'].items():assert sha(root/path)==digest,path
payload=bytearray(struct.pack('<I',len(original['cases'])))
def blob(text):
    raw=text.encode();return struct.pack('<I',len(raw))+raw
for row in original['cases']:
    payload+=blob(row['label'])+blob(row['gametype'])
    payload+=struct.pack('<4I',row['module_id']&0xffffffff,*row['position_words'])
    payload+=blob(row['gameplay'])+blob(row['visual'])
    payload+=struct.pack('<2I',row['gameplay_pending'],row['visual_pending'])
native=json.loads(subprocess.check_output([str(a.probe)],input=payload,timeout=30))
assert len(native['cases'])==len(original['cases']) and native['adapter_checks']==12
for wanted,actual in zip(original['cases'],native['cases']):
    for key in ('label','events','file_polls','final_context'):assert actual[key]==wanted[key],(wanted['label'],key,wanted[key],actual[key])
paths=('module_load_v1.hpp','module_load_v1.cpp','CMakeLists.txt','object_entry_v1.hpp','object_entry_v1.cpp',
       'xml_document_v1.hpp','xml_document_v1.cpp','tests/module_load_probe.cpp','tests/module_load_host.py')
report={'validation':'PASS','scope':__doc__,'original_receipt_sha256':sha(a.original),'probe_sha256':sha(a.probe),
        'script_sha256':sha(pathlib.Path(__file__)),'source_sha256':{path:sha(root/path) for path in paths},
        'input_sha256':hashlib.sha256(payload).hexdigest(),'cases_compared':len(native['cases']),
        'file_polls_compared':sum(sum(row['file_polls'].values()) for row in original['cases']),
        'adapter_checks':native['adapter_checks'],'cases':native['cases'],
        'original_selection_verified':False,'original_file_loading_verified':False,
        'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ('validation','cases_compared','file_polls_compared','adapter_checks','full_loader_verified')}))
