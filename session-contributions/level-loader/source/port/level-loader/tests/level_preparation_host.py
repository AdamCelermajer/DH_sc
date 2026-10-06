"""Verify retained staged preparation against real cache identity/failure cases."""
import argparse, hashlib, json, pathlib, subprocess
ap=argparse.ArgumentParser()
for name in ('cache','inventory','out'):ap.add_argument('--'+name,type=pathlib.Path,required=True)
ap.add_argument('--probe',required=True);a=ap.parse_args()
root=pathlib.Path(__file__).resolve().parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
inventory=json.loads(a.inventory.read_text());lookup={r['name']:r['file'] for r in inventory['levels']}
assert a.probe.startswith('/mnt/c/');binary=pathlib.Path('C:/'+a.probe[7:])
cache='/mnt/c/'+str(a.cache.resolve()).replace('\\','/')[3:]
with a.cache.open('rb') as stream:assert hashlib.file_digest(stream,'sha256').hexdigest()==inventory['cache_sha256']
command=['wsl.exe','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
         'UBSAN_OPTIONS=halt_on_error=1',a.probe,cache]+[lookup[key] for key in ('DESERT_CAVE_02','SWAMP_02','VOID_MAZE_03')]
run=subprocess.run(command,capture_output=True,timeout=120)
assert run.returncode==0 and not run.stderr,run.stderr.decode(errors='replace')
actual=json.loads(run.stdout);assert actual['validation']=='PASS' and actual['lifecycle_checks']>=13
paths=['level_preparation_v1.hpp','level_preparation_v1.cpp','CMakeLists.txt','tests/level_preparation_probe.cpp','tests/level_preparation_host.py']
report={'validation':'PASS','scope':__doc__,'source_sha256':{p:sha(root/p) for p in paths},
        'probe_sha256':sha(binary),'inventory_sha256':sha(a.inventory),'cache_sha256':inventory['cache_sha256'],
        'actual':actual,'runtime_objects_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(actual))
