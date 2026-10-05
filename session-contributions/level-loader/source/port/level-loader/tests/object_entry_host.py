"""Check native retained entry classifications against executed original branches.

This is source preparation and original-registry metadata, not native object
construction, class behavior, event/restoration integration or rendering.
"""
import argparse,hashlib,json,pathlib,subprocess
ap=argparse.ArgumentParser()
for name in ('original','factories','probe','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
a=ap.parse_args();root=pathlib.Path(__file__).resolve().parents[1]
original=json.loads(a.original.read_text());factories=json.loads(a.factories.read_text())
assert original['validation']=='PASS' and original['factory_receipt_sha256']==hashlib.sha256(a.factories.read_bytes()).hexdigest()
assert original['script_sha256']==hashlib.sha256((root/'tests/object_factory_dispatch_original.py').read_bytes()).hexdigest()
expected_registry=[{'gametype':r['gametype'],'original_address':int(r['factory_address'],16)} for r in factories['registry']]
rows=[]
for row in original['factory_dispatch_cases']:
    if row['factory_failure']:continue # Entry preparation does not run a constructor.
    rows.append({'label':'dispatch-'+str(len(rows)),'route':'manager','attributes':{'gametype':row['gametype'],'name':'authored_name'},
                 'events':row['events'],'only_lookup':True})
for row in original['mgp_link_entry_cases']:
    rows.append({'label':row['uri']+':'+row['name']+':'+str(row['debug_return']),
                 'route':'level','attributes':row['attributes'],'events':row['events']})
for row in original['xml_gate_cases']:
    rows.append({'label':'gate-'+str(len(rows)),'route':row['route'],'type_filter':row['type_filter'],
                 'attributes':{k:v for k,v in row['attributes'].items() if k not in ('game_object','type_filter')},
                 'events':row['events']})
unsafe={'label':'level-missing-type','route':'level','attributes':{'name':'retained'},'events':None}
rows.append(unsafe) # Adapter reports unsafe null-strcmp input; not an ARM oracle result.
lines=[str(len(rows))]
for row in rows:
    filt=row.get('type_filter');attrs=row['attributes']
    lines.append(' '.join([json.dumps(row['label']),row['route'],str(int(filt is not None)),json.dumps(filt or ''),str(len(attrs))]))
    for name,value in attrs.items():lines.append(json.dumps(name,ensure_ascii=False)+' '+json.dumps(value,ensure_ascii=False))
payload='\n'.join(lines)+'\n'
native=json.loads(subprocess.check_output([str(a.probe)],input=payload.encode(),timeout=30))
assert native['registry']==expected_registry
assert len(native['cases'])==len(rows)
checked=[]
for source,actual in zip(rows,native['cases']):
    assert actual['label']==source['label'] and actual['attributes']==source['attributes']
    attrs=source['attributes'];events=source['events'];kind=attrs.get('gametype');name=attrs.get('name')
    if events is None:wanted='unsafe_level_missing_type'
    elif source['route']=='level' and kind=='Player':wanted='original_player_exclusion';assert events==[]
    elif kind is None or name is None:wanted='original_missing_attribute';assert events==[]
    elif source.get('type_filter') is not None and source['type_filter']!=kind:wanted='original_type_filter';assert events==[]
    elif any(e['kind']=='add' for e in events):wanted='registered_requires_services'
    else:
        wanted='original_unregistered_type'
        assert any(e['kind']=='debug_switch' for e in events) and not any(e['kind']=='factory_callback' for e in events)
    assert actual['disposition']==wanted,(source,actual,wanted)
    added=next((e for e in events or [] if e['kind']=='add'),None)
    if wanted=='registered_requires_services':
        assert actual['factory_address']==next(r['original_address'] for r in expected_registry if r['gametype']==kind)
        assert actual['template_present']==('template' in attrs)
        # Direct GetNewObject cases verify selection only. The early XML call
        # is independently reached in the LevelConfig entry fixture.
        assert actual['early_init_post']==(kind=='LevelConfig' if source.get('only_lookup') else any(e['kind']=='early_init_post' for e in events))
        assert actual['force_id_minus_one']==bool(added and added['object_id']==-1)
    else:
        assert actual['factory_address']==0 and not actual['template_present'] and not actual['early_init_post'] and not actual['force_id_minus_one']
    assert actual['ownership_and_failure_checks']
    checked.append({'label':source['label'],'disposition':wanted,'original_execution_compared':events is not None})
paths=('object_entry_v1.cpp','object_entry_v1.hpp','original_factory_table_v1.inc','tests/object_entry_probe.cpp','tests/object_entry_host.py')
report={'validation':'PASS','scope':__doc__,'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'source_sha256':{p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in paths},
        'original_receipt_sha256':hashlib.sha256(a.original.read_bytes()).hexdigest(),
        'factory_receipt_sha256':hashlib.sha256(a.factories.read_bytes()).hexdigest(),
        'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),'input_sha256':hashlib.sha256(payload.encode()).hexdigest(),
        'registry_entries_compared':33,'cases':checked,'original_lookup_cases_compared':sum(bool(r.get('only_lookup')) for r in rows),
        'original_xml_entry_cases_compared':sum(r['events'] is not None and not r.get('only_lookup') for r in rows),
        'unsafe_adapter_case':1,'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','registry_entries':33,'original_entry_cases':len(rows)-1,'unsafe_adapter_cases':1,'full_loader_verified':False}))
