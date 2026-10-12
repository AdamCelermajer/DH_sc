import json,pathlib
root=pathlib.Path(__file__).resolve().parents[1]
for name in ('procedural-sources-host.json','procedural-sources-sanitizers.json'):
    path=root/'reports'/name
    if not path.exists():continue
    report=json.loads(path.read_text())
    print(json.dumps({'receipt':name,**{k:report[k] for k in ('validation','summary','block_occurrences_compared','original_partial_xml_calls','sanitizers')},
        'diagnostic_rules':[{'name':r['name'],'diagnostic':r.get('rule_diagnostic')} for r in report['levels'] if r.get('rule_parsed') is False]}))
path=root/'reports/swamp2-procedural-sources.json'
if path.exists():
    report=json.loads(path.read_text())
    print(json.dumps({'identity':report['identity'],'folder':report['folder'],'documents':report['document_count'],
        'listed_files':len(report['filenames']),'blocks':len(report['blocks']),
        'root_attributes':report['rule']['tree']['attributes'],'layout_generation_verified':False}))
