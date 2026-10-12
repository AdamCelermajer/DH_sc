"""Prepare actual SWAMP sources with native cache reader; no rendered claim."""
import argparse,hashlib,json,pathlib,subprocess
ap=argparse.ArgumentParser();ap.add_argument('--cache',type=pathlib.Path,required=True)
ap.add_argument('--probe',type=pathlib.Path,required=True);ap.add_argument('--out',type=pathlib.Path,required=True)
a=ap.parse_args()
with a.cache.open('rb') as f:cache_sha=hashlib.file_digest(f,'sha256').hexdigest()
assert cache_sha=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
command=[str(a.probe),str(a.cache),'SWAMP','001_swamp.mlx']
p=subprocess.run(command,capture_output=True,text=True)
report={'cache_sha256':cache_sha,'native_probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'command':command,'returncode':p.returncode,'stderr':p.stderr,
    'scope':'Native fixed source preparation, original resource paths, retained raw XML and module links. No factories, transforms, geometry, conditions, quests, loot, saves or rendering executed.',
    'full_loader_verified':False}
if p.returncode:report['validation']='FAIL'
else:
    result=json.loads(p.stdout);report.update(result)
    counts=result['declaration_counts_in_unique_sources']
    assert result['documents']==19 and result['module_links']==9
    assert counts['Module']==9 and counts['Character']==50 and counts['OpenableContainer']==5
    assert result['ownership_checks'] and not result['map_render_verified']
a.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
raise SystemExit(p.returncode)
