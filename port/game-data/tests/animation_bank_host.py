"""Prove native PAB1 decode equals source JSON; bind real-bank sanitizer replay."""
import argparse,hashlib,importlib.util,json,shlex,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(*args):
 r=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc',*args],capture_output=True,text=True,timeout=120);assert r.returncode==0,(r.returncode,r.stdout,r.stderr);assert not r.stderr.strip(),r.stderr;return r.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--rebuild',action='store_true');p.add_argument('--executable',default='/home/adampalace/dh2-world-build/animation_bank_isolated_audit');p.add_argument('--output',type=Path,default=ROOT/'reports/animation-bank-host-audit.json');a=p.parse_args()
 scratch=REPO/'.local-inputs/animation-bank-discovery';scratch.mkdir(parents=True,exist_ok=True);assets=REPO/'port/android-native/app/src/main/assets';manifest=assets/'data/prince-animation-bank.json';binary=assets/'data/prince-animation-bank.bin';m=json.loads(manifest.read_text())
 tool=ROOT/'tools/produce_animation_bank.py';spec=importlib.util.spec_from_file_location('bank_producer',tool);producer=importlib.util.module_from_spec(spec);spec.loader.exec_module(producer);expected,preflight=producer.produce(manifest,assets);assert binary.read_bytes()==expected
 probe=REPO/'port/engine-animation/reference/prince-registration/probe.json';assert sha(probe)==m['producer_sha256']
 names=['port/game-data/animation_bank.cpp','port/game-data/animation_bank.hpp','port/game-data/data.hpp','port/game-data/tests/animation_bank.cpp','port/game-data/tests/animation_bank_host.py','port/game-data/tools/produce_animation_bank.py'];hashes={n:sha(REPO/n) for n in names};command=['g++','-std=c++17','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer','-g',names[0],names[3],'-o',a.executable]
 if a.rebuild:run(*command)
 build={'source_sha256':hashes,'executable_sha256':run('sha256sum',a.executable).split()[0],'compiler':run('g++','--version').splitlines()[0],'command':shlex.join(command)};assert all(sha(REPO/n)==h for n,h in hashes.items()),'Source changed'
 snapshot=scratch/'host-build-inputs.json'
 if a.rebuild:snapshot.write_text(json.dumps(build,indent=2)+'\n')
 else:assert json.loads(snapshot.read_text())==build,'Rebuild frozen inputs first'
 host=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1',a.executable,'port/android-native/app/src/main/assets/data/prince-animation-bank.bin','.local-inputs/animation-bank-discovery/native-bank.json'));native=json.loads((scratch/'native-bank.json').read_text())
 for key,value in m.items():
  if key!='scope':assert native[key]==value,('Source/native metadata differs',key)
 assert native['manifest_sha256']==sha(manifest) and native['identity_policy']==1
 # Producer preflight fails before writing on changed manifest or existing asset.
 refused=0
 with tempfile.TemporaryDirectory(dir=scratch) as tmp:
  bad_manifest=Path(tmp)/'manifest.json';bad_manifest.write_bytes(manifest.read_bytes()+b' ')
  try:producer.produce(bad_manifest,assets);raise RuntimeError('Changed source accepted')
  except AssertionError:refused+=1
  bad_output=Path(tmp)/'bank.bin';bad_output.write_bytes(b'wrong existing bytes');before=bad_output.read_bytes()
  import sys
  result=subprocess.run([sys.executable,str(tool),'--output',str(bad_output),'--report',str(Path(tmp)/'report.json')],capture_output=True,text=True,timeout=30);assert result.returncode!=0 and bad_output.read_bytes()==before and not (Path(tmp)/'report.json').exists();refused+=1
  missing=Path(tmp)/'missing-assets';missing.mkdir()
  try:producer.produce(manifest,missing);raise RuntimeError('Missing assets accepted')
  except FileNotFoundError:refused+=1
  first=missing/m['resources'][0]['asset'];first.parent.mkdir(parents=True);first.write_bytes(b'changed resource')
  try:producer.produce(manifest,missing);raise RuntimeError('Changed asset accepted')
  except AssertionError:refused+=1
 assert host['resources']==116 and host['registration_requests']==158 and not host['mismatches'];assert all(sha(REPO/n)==h for n,h in hashes.items()),'Source changed during replay';assert sha(manifest)==producer.EXPECTED and binary.read_bytes()==expected
 report={'validation':'PASS',**build,'build_inputs_sha256':sha(snapshot),'host_audit':host,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'source_manifest_sha256':sha(manifest),'binary_sha256':sha(binary),'source_metadata_fields_compared':len(m)-1,'source_resource_records_compared':116,'source_ordered_requests_compared':158,'producer_preflight_rejections':refused,'producer_preflight':preflight,'original_registration_producer':{'path':str(probe.relative_to(REPO)).replace('\\','/'),'sha256':sha(probe)},'scope':'New documented port PAB1 metadata format decoded natively; exact source JSON/actual asset-byte binding and source registration producer hash. Not an original serialized engine format or original pointer identity reconstruction.'};a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS',**host,'source_resource_records_compared':116,'source_ordered_requests_compared':158,'producer_preflight_rejections':refused}))
if __name__=='__main__':main()
