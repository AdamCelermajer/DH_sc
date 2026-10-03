"""Genuine shared-library occurrence coordinator acceptance and legacy replay."""
import argparse, hashlib, json, subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]; REPO=ROOT.parents[1]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--output',type=Path,default=ROOT/'reports/actor-blended-occurrences-host-audit.json');a=p.parse_args()
 scratch=REPO/'.local-inputs/actor-blended-occurrences-discovery';scratch.mkdir(parents=True,exist_ok=True)
 commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc',*args],capture_output=True,text=True,timeout=180)
  commands.append({'arguments':list(args),'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
  assert r.returncode==0 and not r.stderr.strip(),commands[-1]
  return r.stdout.strip()
 paths=[]
 for module in ('level-world','game-data','engine-animation','engine-skinning','engine-resources','engine-textures','engine-math','scene-materials','physics-backend','asset-payloads'):
  paths += [x for x in (REPO/'port'/module).rglob('*') if x.is_file() and (x.suffix in ('.cpp','.hpp','.h','.c','.cc','.inl','.ipp','.inc','.cmake') or x.name=='CMakeLists.txt')]
 paths += [Path(__file__), ROOT/'tests/actor_blended_occurrences_reference.py']
 before={x.relative_to(REPO).as_posix():sha(x) for x in paths}
 def linux(path): return '/mnt/c/'+str(path.resolve()).replace('\\','/')[3:]
 exe=linux(scratch/'host_audit'); animation=a.build+'/engine-skinning/engine-animation';scene=animation+'/scene-materials';game=a.build+'/game-data'
 flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 run('g++',*flags,linux(ROOT/'tests/actor_blended_occurrences.cpp'),'-L'+a.build,'-L'+animation,'-L'+scene,'-L'+game,'-ldh2_level_world','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_game_data','-Wl,-rpath,'+':'.join((a.build,animation,scene,game)),'-o',exe)
 env=['env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
 audit=json.loads(run(*env,exe,'port/engine-animation/reference/prince-bank-integration/bank-fixture.bin','port/android-native/app/src/main/assets','port/level-world/reference/actor-blended-occurrences/playclip-fixtures.bin','.local-inputs/actor-blended-occurrences-discovery/host-frames.bin'))
 assert audit['validation']=='PASS' and audit['original_playclip_comparisons']==72 and audit['atomic_rejections']==11
 ref=ROOT/'reference/actor-blended-playback';legacy={}
 for tag,dynamic in (('static',False),('dynamic',True)):
  target=scratch/(tag+'-frames.bin');args=[*env,a.build+'/actor_blended_playback_audit','port/android-native/app/src/main/assets',linux(target)]
  if dynamic:args+=['--dynamic']
  legacy[tag]=json.loads(run(*args));evidence=json.loads((ref/('dynamic/' if dynamic else '')/'original-composition.json').read_text())
  assert legacy[tag]['mismatches']==0 and sha(target)==evidence['fixture_sha256']
 for name,fixture in (('scheduler','scheduler-fixtures.bin'),('selection','selection-fixtures.bin'),('controls','control-fixtures.bin')):
  legacy[name]=json.loads(run(*env,a.build+'/actor_blended_'+name+'_audit',linux(ref/fixture)));assert legacy[name]['mismatches']==0
 binaries={exe:run('sha256sum',exe).split()[0]}
 dependencies=run('ldd',exe);assert 'libasan.so' in dependencies and 'libubsan.so' in dependencies
 for token in dependencies.split():
  if token.startswith(a.build+'/') and token.endswith('.so'):binaries[token]=run('sha256sum',token).split()[0]
 assert all(sha(REPO/name)==value for name,value in before.items()),'Source changed during acceptance'
 original=ROOT/'reference/actor-blended-occurrences/original-playclip.json';gold=ROOT/'reference/actor-blended-occurrences/playclip-fixtures.bin';evidence=json.loads(original.read_text());assert evidence['reference_sha256']==sha(gold)
 bank=REPO/'port/engine-animation/reports/prince-bank-current-integration-host.json';bank_report=json.loads(bank.read_text());assert bank_report['validation']=='PASS'
 for name,value in bank_report['source_sha256'].items():assert sha(REPO/name)==value,name
 for name,value in bank_report['input_sha256'].items():assert sha(REPO/name)==value['sha256'],name
 reports={}
 for path in [original,bank,REPO/'port/engine-animation/reports/animation-registration-arm64-differential.json']:
  row=json.loads(path.read_text());assert row['validation']=='PASS' or row.get('differential')=='PASS';reports[path.relative_to(REPO).as_posix()]=sha(path)
 report={'validation':'PASS','host_audit':audit,'legacy_regression':legacy,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'original_sha256':evidence['original_sha256'],'original_instruction_cases':72,'reference_sha256':sha(gold),'source_sha256':before,'binary_sha256':binaries,'linked_dependencies':dependencies,'frame_capture_sha256':sha(scratch/'host-frames.bin'),'evidence_report_sha256':reports,'input_sha256':bank_report['input_sha256'],'sources_unchanged_through_replay':True,'commands':commands,'scope':'Occurrence adapter and genuine production DSO scene/event/root integration, with focused actual original PlayClip engine-index comparison and separately source-derived registration/bank evidence. Legacy original-bound captures remain exact. This is not a full original-frame, gameplay FSM, GPU or APK differential.'}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','report':str(a.output),'sha256':sha(a.output),'checks':audit}))
if __name__=='__main__':main()
