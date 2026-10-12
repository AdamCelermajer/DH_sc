"""Explicit source-preparation status for every supplied level-table row."""
import argparse,collections,hashlib,json,pathlib,subprocess
ap=argparse.ArgumentParser();ap.add_argument('--cache',type=pathlib.Path,required=True)
ap.add_argument('--probe',type=pathlib.Path,required=True);ap.add_argument('--inventory',type=pathlib.Path,required=True)
ap.add_argument('--out',type=pathlib.Path,required=True);a=ap.parse_args()
inventory=json.loads(a.inventory.read_text());rows=[]
with a.cache.open('rb') as f:digest=hashlib.file_digest(f,'sha256').hexdigest()
assert digest==inventory['cache_sha256']
for level in inventory['levels']:
    p=subprocess.run([str(a.probe),str(a.cache),level['name'],level['file']],capture_output=True,text=True)
    row={'name':level['name'],'file':level['file'],'random':level['random'],'returncode':p.returncode,
         'map_render_verified':False,'gameplay_verified':False}
    if p.returncode:
        reason=p.stderr.strip();row['reason']=reason
        row['source_preparation']=('parser_blocked' if reason.startswith(('XML parse failure','XML parse:')) else
            'read_failure' if reason.startswith('XML read failure') else
            'missing_dependency' if reason.startswith('Missing authored XML') else
            'alternate_selection_unimplemented' if reason.startswith('Alternate module selection') else
            'rule_generation_unimplemented' if 'rule generation is not implemented' in reason else 'error')
    else:row['source_preparation']='prepared';row['result']=json.loads(p.stdout)
    rows.append(row)
summary=dict(collections.Counter(r['source_preparation'] for r in rows))
report={'cache_sha256':digest,'native_probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'inventory_sha256':hashlib.sha256(a.inventory.read_bytes()).hexdigest(),
    'scope':'Native fixed source preparation through retained level-buffer traversal. Prepared is not runtime-object support or rendering. This probe does not run rule generation, map assembly, gameplay or restoration services.',
    'levels':rows,'summary':summary,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'levels':len(rows),'summary':summary}))
