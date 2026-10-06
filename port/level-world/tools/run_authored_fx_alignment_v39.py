from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3]
p=root/'port/level-world/tools/run_authored_fx_v32_host.py';s=p.read_text()
s=s.replace("version=sys.argv[1] if len(sys.argv)>1 else '6'","version='32'")
s=s.replace("mode=sys.argv[2] if len(sys.argv)>2 else 'census'","mode='alignment-v39'")
s=s.replace("snapshot='.local-inputs/character-fx-owner-v1/host-snapshot'","sources[0]='port/level-world/tests/authored_fx_alignment_v39.cpp'\nsources+=['port/level-world/authored_fx_alignment_v39.cpp']\nsnapshot='.local-inputs/character-fx-owner-v1/host-snapshot'")
# Asset identity is checked before compiling; do not infer row IDs from art.
assets=root/'port/level-world/reference/shared-target-facing-v1/cache/general-v5'
rows=json.loads((assets/'manifest.json').read_text())
for name in ('asset_38.bdae','asset_42.bdae','asset_43.bdae'):
 r=next(r for r in rows if r['local']==name);assert hashlib.sha256((assets/name).read_bytes()).hexdigest()==r['sha256'];print(name,r['uri'])
exec(compile(s,str(p),'exec'),{'__file__':str(p),'__name__':'__main__'})
