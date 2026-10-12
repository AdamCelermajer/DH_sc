"""Inventory retained procedural declaration types without activating objects."""
import collections,hashlib,json,pathlib
root=pathlib.Path(__file__).resolve().parents[3];reports=root/'port/level-loader/reports'
source=reports/'procedural-map-host-coverage.json';coverage=json.loads(source.read_text())
types=collections.Counter();rows=[]
for level in coverage['levels']:
    for run in level['runs']:
        row={'identity':level['identity'],'definition':level['definition'],'seed':run['seed'],'preparation':run['status']}
        if run['status']=='assembled':
            counts=run['result']['declaration_types'];types.update(counts)
            row.update(module_count=run['result']['module_count'],source_documents=run['result']['source_documents'],
                       declaration_types=counts,declarations=sum(counts.values()))
        else:row['reason']=run.get('reason','Original generator produced no layout')
        rows.append(row)
result={'scope':__doc__,'source_receipt_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
        'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'levels':rows,'types_across_selected_runs':dict(sorted(types.items())),
        'selected_run_declarations':sum(types.values()),'type_count':len(types),
        'counts_are_declaration_occurrences':True,'counts_are_active_objects':False,
        'runtime_factory_contract_agreed':False,'runtime_objects_verified':False,'full_loader_verified':False}
(reports/'procedural-declaration-types.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({'type_count':len(types),'selected_run_declarations':sum(types.values()),
                  'types':dict(types.most_common())},indent=2))
