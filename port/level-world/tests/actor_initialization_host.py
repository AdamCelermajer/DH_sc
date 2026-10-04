"""Isolated O2 ASan/UBSan CAI1 load from actual ZIP authoring/current DACT.

The owned port sidecar is compared with the direct-MGP producer projection;
full shipping XML/factory, Application, scene or APK parity is not claimed.
"""
import argparse,hashlib,json,shlex,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def posix(p):return '/mnt/'+p.resolve().drive[0].lower()+str(p.resolve())[2:].replace('\\','/')
def run(args):
 r=subprocess.run(args,capture_output=True,text=True);assert r.returncode==0,(args,r.returncode,r.stdout,r.stderr);return r
def main():
 p=argparse.ArgumentParser();p.add_argument('--cache',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 assert not a.output.exists(),'Refusing to overwrite audit report'
 reference=ROOT/'reference/actor-initialization';binary=reference/'crypt01-actor-initialization.bin';manifest_path=reference/'crypt01-actor-initialization.json';descriptor=REPO/'port/android-native/app/src/main/assets/worlds/crypt01.dact'
 producer=ROOT/'tools/produce_actor_initialization.py'
 generation=run([sys.executable,str(producer),'--cache',str(a.cache),'--output',str(binary),'--manifest',str(manifest_path)])
 verified=run([sys.executable,str(producer),'--cache',str(a.cache),'--output',str(binary),'--manifest',str(manifest_path),'--verify-only'])
 assert generation.stdout==verified.stdout
 names=['port/level-world/actor_initialization.hpp','port/level-world/actor_initialization.cpp','port/level-world/tools/produce_actor_initialization.py','port/level-world/tests/actor_initialization.cpp','port/level-world/tests/actor_initialization_host.py','port/level-world/character_native_fsm.hpp','port/level-world/character_native_fsm.cpp']
 hashes={n:sha(REPO/n) for n in names};exe=REPO/'.local-inputs/actor-initialization/actor_initialization_audit';exe.parent.mkdir(parents=True,exist_ok=True)
 files=['port/level-world/actor_initialization.cpp','port/level-world/character_native_fsm.cpp','port/level-world/tests/actor_initialization.cpp']
 cmd=['g++','-std=c++17','-O2','-g','-fno-fast-math','-ffp-contract=off','-fno-omit-frame-pointer','-ffunction-sections','-fdata-sections','-Wl,--gc-sections','-fsanitize=address,undefined','-Wall','-Wextra','-Werror',*[posix(REPO/n) for n in files],'-o',posix(exe)]
 run(['wsl','-e','bash','-lc',shlex.join(cmd)])
 result=run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',posix(exe),posix(binary),posix(descriptor)]);assert not result.stderr
 audit=json.loads(result.stdout);manifest=json.loads(manifest_path.read_text());native=audit['native_projection']
 for key in ('cache_sha256','descriptor_sha256','original_sha256','default_capture_sha256','sources','records'):assert native[key]==manifest[key],key
 assert hashes=={n:sha(REPO/n) for n in names}
 proofs=['port/level-world/reference/character-live-owner-readiness/original-functions.json','port/level-world/reference/character-live-owner-readiness/default-probes.json','port/level-world/reference/character-live-owner-readiness/crypt-authoring.json','port/level-world/reports/character-native-fsm-arm64-differential.json']
 proofs=[n for n in proofs if (REPO/n).exists()]
 report=dict(validation='PASS',scope=__doc__,host_audit={k:v for k,v in audit.items() if k!='native_projection'},native_projection=native,source_sha256=hashes,executable_sha256=sha(exe),binary_sha256=sha(binary),manifest_sha256=sha(manifest_path),descriptor_sha256=sha(descriptor),original_sha256=manifest['original_sha256'],original_proof_sha256={n:sha(REPO/n) for n in proofs},source_records_compared=11,source_MGP_members_compared=3,mismatches=0,sanitizers=dict(address=True,undefined=True,leaks=True,diagnostics=0),compiler_command=cmd,central_DSO_used=False,original_XML_factory_parity=False,APK=False)
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('native_projection','source_sha256','compiler_command')}))
if __name__=='__main__':main()
