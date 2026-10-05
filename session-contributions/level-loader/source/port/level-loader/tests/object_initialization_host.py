"""Compare portable dispatcher traces and memberships with original ARM receipts.

The services in both probes are explicit fixture implementations. This verifies
dispatch ordering, not real class construction/behavior or a gameplay ABI.
"""
import argparse,hashlib,json,pathlib,subprocess
ap=argparse.ArgumentParser()
for name in ('original','probe','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
a=ap.parse_args()
original=json.loads(a.original.read_text());assert original['validation']=='PASS'
root=pathlib.Path(__file__).resolve().parents[1]
assert original['script_sha256']==hashlib.sha256((root/'tests/object_initialization_original.py').read_bytes()).hexdigest()
lines=[str(len(original['cases']))]
for row in original['cases']:
    f=row['fixture'];objects=f['objects'];ids={o['label']:i+1 for i,o in enumerate(objects)}
    lines.append(f"{f['name']} {len(objects)} {len(f['modules'])} {int(f.get('preseed',False))}")
    for i,o in enumerate(objects):
        assert all(c.isalnum() or c=='_' for c in o['label']+o['type'])
        fields=[o['label'],o['type'],o.get('key',i),int(o.get('updating',False)),int(o.get('deleted',False)),
                int(o.get('a8',0)),int(o.get('ac',0)),int(o.get('cc',0)),int(o.get('d0',0)),
                ids[o['append']] if o.get('append') else 0,o.get('init_a8',-1),o.get('room_cc',-1),
                ids[o['insert']] if o.get('insert') else 0,int(o.get('deferred',False))]
        lines.append(' '.join(map(str,fields)))
    lines.append(' '.join(str(ids[name]) for name in f['modules']))
text='\n'.join(lines)+'\n'
native=json.loads(subprocess.check_output([str(a.probe)],input=text.encode(),timeout=30))
assert len(native['cases'])==len(original['cases'])
checked=[]
for reference,actual in zip(original['cases'],native['cases']):
    name=reference['fixture']['name'];assert actual['name']==name
    for key in ('calls','events','room_list','transient_lists','module_list'):
        assert reference[key]==actual[key],f"{name}: {key} differs\noriginal={reference[key]}\nnative={actual[key]}"
    checked.append({'name':name,'calls':len(actual['calls']),'events':len(actual['events']),
                    'room_list':actual['room_list'],'transient_lists':actual['transient_lists']})
guards=json.loads(subprocess.check_output([str(a.probe),'--adapter-checks'],timeout=30))
assert guards=={'validation':'PASS','adapter_checks':6}
report={'validation':'PASS','scope':__doc__,'original_receipt_sha256':hashlib.sha256(a.original.read_bytes()).hexdigest(),
        'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),'probe_input_sha256':hashlib.sha256(text.encode()).hexdigest(),
        'source_sha256':{p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in
                         ('object_initialization_v1.cpp','object_initialization_v1.hpp','tests/object_initialization_probe.cpp','tests/object_initialization_host.py')},
        'cases':checked,'call_count':sum(c['calls'] for c in checked),'event_count':sum(c['events'] for c in checked),
        'adapter_checks':guards,'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','fixtures':len(checked),'calls':report['call_count'],'events':report['event_count'],
                  'adapter_checks':guards['adapter_checks']}))
