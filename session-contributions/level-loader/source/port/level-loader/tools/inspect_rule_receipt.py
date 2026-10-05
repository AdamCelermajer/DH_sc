"""Print bounded rule-reader evidence without dumping the complete receipt."""
import argparse,collections,json,pathlib
ap=argparse.ArgumentParser();ap.add_argument('--receipt',type=pathlib.Path,required=True);ap.add_argument('--case');a=ap.parse_args()
report=json.loads(a.receipt.read_text());cases=report['cases']
if a.case:
    rows=[r for r in cases if r['name']==a.case];assert len(rows)==1,a.case
    r=rows[0];print(json.dumps({k:r[k] for k in ('name','xml_error','root_present','read_result','rule','pools')},indent=2))
else:
    authored=[r for r in cases if r['name'].startswith('data/')]
    print(json.dumps({'cases':len(cases),'authored_cases':len(authored),
        'reader_results':dict(collections.Counter(str(r['read_result']) for r in authored)),
        'authored_false_results':[{'name':r['name'],'xml_error':r['xml_error']} for r in authored if not r['read_result']],
        'guarded_cases':[{'name':r['name'],'reason':r['checked_domain_rejection']} for r in cases if 'checked_domain_rejection' in r]},indent=2))
