"""Attempt owned native map assembly for every prepared fixed source set.

Keep original preparation blockers visible for all 51 rows. Assembled does not
mean rendered, runtime-object supported, gameplay supported or loader-complete.
"""
import argparse,collections,hashlib,json,pathlib,subprocess
ap=argparse.ArgumentParser();ap.add_argument('--cache',type=pathlib.Path,required=True)
ap.add_argument('--probe',type=pathlib.Path,required=True);ap.add_argument('--source-coverage',type=pathlib.Path,required=True)
ap.add_argument('--out',type=pathlib.Path,required=True);a=ap.parse_args()
sources=json.loads(a.source_coverage.read_text())
with a.cache.open('rb') as f:cache_sha=hashlib.file_digest(f,'sha256').hexdigest()
assert cache_sha==sources['cache_sha256']=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
rows=[]
for source in sources['levels']:
    row={k:source[k] for k in ('name','file','random','source_preparation')}
    row.update({'map_render_verified':False,'gameplay_verified':False})
    if source['source_preparation']!='prepared':row['map_preparation']='source_blocked';row['reason']=source['reason']
    else:
        result=subprocess.run([str(a.probe.resolve()),str(a.cache.resolve()),source['name'],source['file']],text=True,capture_output=True)
        if result.returncode:row['map_preparation']='assembly_blocked';row['reason']=result.stderr.strip()
        else:row['map_preparation']='assembled';row['result']=json.loads(result.stdout)
    rows.append(row)
summary=dict(collections.Counter(r['map_preparation'] for r in rows))
report={'cache_sha256':cache_sha,'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'source_coverage_sha256':hashlib.sha256(a.source_coverage.read_bytes()).hexdigest(),'scope':__doc__,'levels':rows,'summary':summary,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'rows':len(rows),'summary':summary,'assembly_failures':[{'name':r['name'],'reason':r['reason']} for r in rows if r['map_preparation']=='assembly_blocked']},indent=2))
