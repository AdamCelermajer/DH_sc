"""Compare native file-list tokenization with original ARM GetFiles receipts."""
import argparse,hashlib,json,os,pathlib,subprocess
ap=argparse.ArgumentParser()
ap.add_argument('--original',type=pathlib.Path,required=True)
ap.add_argument('--probe',type=pathlib.Path,required=True)
ap.add_argument('--out',type=pathlib.Path,required=True)
ap.add_argument('--sanitizers',action='store_true')
a=ap.parse_args();original=json.loads(a.original.read_text())
assert original['original_execution_complete']
assert original['cache_sha256']=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
assert original['engine_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
root=pathlib.Path(__file__).resolve().parents[1]
assert hashlib.sha256((root/'tests/procedural_file_list_original.py').read_bytes()).hexdigest()==original['script_sha256']
cmd=[str(a.probe.resolve())]
if os.name=='nt':
    absolute=a.probe.resolve();cmd=['wsl','-d','Ubuntu','--','/mnt/'+absolute.drive[0].lower()+absolute.as_posix()[2:]]
if a.sanitizers:
    prefix=['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
    cmd=cmd[:4]+prefix+cmd[4:] if os.name=='nt' else prefix+cmd
inputs=bytearray()
for row in original['cases']:
    raw=bytes.fromhex(row['input_hex']);assert hashlib.sha256(raw).hexdigest()==row['input_sha256']
    inputs.extend(str(len(raw)).encode()+b'\n'+raw)
run=subprocess.run(cmd,input=bytes(inputs),capture_output=True)
if run.returncode:raise RuntimeError(run.stderr.decode())
actual=[json.loads(row) for row in run.stdout.decode().splitlines()]
assert len(actual)==len(original['cases'])
rows=[{'name':expected['name'],'input_sha256':expected['input_sha256'],
       'expected':expected['rows'],'actual':got,'matches':expected['rows']==got}
      for expected,got in zip(original['cases'],actual)]
paths=[root/'procedural_file_list_v1.cpp',root/'procedural_file_list_v1.hpp',
       root/'tests/procedural_file_list_probe.cpp',pathlib.Path(__file__)]
report={'validation':'PASS' if all(r['matches'] for r in rows) else 'FAIL',
    'scope':__doc__,'original_receipt_sha256':hashlib.sha256(a.original.read_bytes()).hexdigest(),
    'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'sources_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
    'sanitizers':a.sanitizers,'cases':rows,'authored_lists_compared':sum(r['name'].startswith('data/') for r in rows),
    'layout_generation_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':report['validation'],'cases':len(rows),
    'authored_lists_compared':report['authored_lists_compared'],'sanitizers':a.sanitizers,
    'mismatches':[r for r in rows if not r['matches']]}))
raise SystemExit(0 if report['validation']=='PASS' else 1)
