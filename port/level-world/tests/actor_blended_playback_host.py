"""Bind prebuilt sanitized two-slot audits to immutable original service gold.

Runs host binaries only. Static CAnimationSet compile policy is intentional:
live Prince's dynamic compiler producer is separate pending evidence.
"""
import argparse,hashlib,json,struct,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(*args):
 r=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc',*args],capture_output=True,text=True,timeout=120)
 assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
 assert not r.stderr.strip(),r.stderr
 return r.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--rebuild',action='store_true',help='Build host targets between identical compiler-input snapshots.');p.add_argument('--output',type=Path,default=ROOT/'reports/actor-blended-playback-host-audit.json');p.add_argument('--cache',type=Path,default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'));a=p.parse_args()
 ref=ROOT/'reference/actor-blended-playback';scratch=REPO/'.local-inputs/actor-blended-playback-discovery';scratch.mkdir(parents=True,exist_ok=True)
 evidence={name:json.loads((ref/(name+'.json')).read_text()) for name in ('original-composition','original-scheduler','original-selection','original-controls')}
 for name,report in evidence.items():assert report['validation']=='PASS'
 assert sha(ref/'frame-fixtures.bin')==evidence['original-composition']['fixture_sha256']
 for name,filename in (('original-composition','composition-reference.bin'),('original-scheduler','scheduler-fixtures.bin'),('original-selection','selection-fixtures.bin'),('original-controls','control-fixtures.bin')):assert sha(ref/filename)==evidence[name]['corpus_sha256']
 sources=['port/level-world/actor_blended_playback.hpp','port/level-world/actor_blended_playback.cpp','port/game-data/animation_scheduler.hpp','port/game-data/animation_scheduler.cpp','port/engine-animation/animation.hpp','port/engine-animation/animation.cpp','port/engine-animation/animation_blend.hpp','port/engine-animation/animation_blend.cpp','port/level-world/animation_blender.hpp','port/level-world/animation_blender.cpp','port/level-world/visual_timeline.hpp','port/level-world/visual_timeline.cpp','port/level-world/visual_motion.hpp','port/level-world/visual_motion.cpp','port/level-world/tests/actor_blended_playback.cpp','port/level-world/tests/actor_blended_scheduler.cpp','port/level-world/tests/actor_blended_selection.cpp','port/level-world/tests/actor_blended_playback_differential.py','port/level-world/tests/actor_blended_scheduler_reference.py','port/level-world/tests/actor_blended_selection_reference.py','port/level-world/tests/actor_blended_playback_host.py','port/level-world/CMakeLists.txt']
 source_hashes={n:sha(REPO/n) for n in sources}
 source_hashes.update({n:sha(REPO/n) for n in ['port/level-world/tests/actor_blended_controls.cpp','port/level-world/tests/actor_blended_controls_reference.py']})
 # Snapshot the full host dependency source trees, including source compiled
 # into the shared libraries rather than the thin audit executables alone.
 for module in ('level-world','game-data','engine-animation','engine-skinning','engine-resources','engine-textures','engine-math','scene-materials','physics-backend'):
  for path in sorted((REPO/'port'/module).rglob('*')):
   if path.is_file() and (path.suffix in ('.cpp','.hpp','.h','.c','.cc','.inl','.ipp','.inc','.cmake') or path.name=='CMakeLists.txt'):
    source_hashes[path.relative_to(REPO).as_posix()]=sha(path)
 snapshot=scratch/'build-inputs.json'
 targets=['actor_blended_playback_audit','actor_blended_scheduler_audit','actor_blended_selection_audit','actor_blended_controls_audit']
 if a.rebuild:
  run('cmake','--build',a.build,'--target',*targets,'--parallel','8')
  assert all(sha(REPO/n)==v for n,v in source_hashes.items()),'Compiler inputs changed during build'
 exe_hashes={a.build+'/'+n:run('sha256sum',a.build+'/'+n).split()[0] for n in targets}
 dependency_paths=sorted({token for exe in exe_hashes for token in run('ldd',exe).split() if token.startswith(a.build+'/') and token.endswith('.so')})
 dependency_hashes={path:run('sha256sum',path).split()[0] for path in dependency_paths}
 build_inputs={'source_sha256':source_hashes,'executable_sha256':exe_hashes,'dependency_sha256':dependency_hashes}
 if a.rebuild:snapshot.write_text(json.dumps(build_inputs,indent=2)+'\n')
 else:assert snapshot.exists() and json.loads(snapshot.read_text())==build_inputs,'Rebuild with stable inputs required before replay/source binding'
 env=['env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
 playback=a.build+'/actor_blended_playback_audit';scheduler=a.build+'/actor_blended_scheduler_audit';selection=a.build+'/actor_blended_selection_audit'
 audit=json.loads(run(*env,playback,'port/android-native/app/src/main/assets','.local-inputs/actor-blended-playback-discovery/host-frame-fixtures.bin'))
 assert audit['validation']=='PASS' and audit['mismatches']==0 and audit['callback_tail_regressions']==2
 assert sha(scratch/'host-frame-fixtures.bin')==evidence['original-composition']['fixture_sha256'],'Native regenerated frames differ from original-bound capture'
 dynamic_evidence=json.loads((ref/'dynamic/original-composition.json').read_text());assert dynamic_evidence['validation']=='PASS' and dynamic_evidence['mismatches']==0
 assert sha(ref/'dynamic/frame-fixtures.bin')==dynamic_evidence['fixture_sha256'] and sha(ref/'dynamic/composition-reference.bin')==dynamic_evidence['corpus_sha256']
 dynamic_audit=json.loads(run(*env,playback,'port/android-native/app/src/main/assets','.local-inputs/actor-blended-playback-discovery/host-dynamic-frame-fixtures.bin','--dynamic'))
 assert dynamic_audit['validation']=='PASS' and dynamic_audit['mismatches']==0 and dynamic_audit['constructor_slot_checks']==2 and dynamic_audit['forced_stop_checks']==2
 assert sha(scratch/'host-dynamic-frame-fixtures.bin')==dynamic_evidence['fixture_sha256'],'Native dynamic capture differs from original-bound capture'
 sched=json.loads(run(*env,scheduler,'port/level-world/reference/actor-blended-playback/scheduler-fixtures.bin'))
 selected=json.loads(run(*env,selection,'port/level-world/reference/actor-blended-playback/selection-fixtures.bin'))
 controlled=json.loads(run(*env,a.build+'/actor_blended_controls_audit','port/level-world/reference/actor-blended-playback/control-fixtures.bin'))
 assert sched['original_scheduling_cases']==48 and selected['original_selection_cases']==72 and sched['mismatches']==selected['mismatches']==0
 assert controlled['original_control_cases']==2160 and controlled['mismatches']==0
 cache_hash=sha(a.cache);assert cache_hash=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 assets=REPO/'port/android-native/app/src/main/assets';inputs=[]
 names=['models/prince_modular.bdae','data/animations_pyarray.bin','data/animations_pyarraynames.bin','data/animations_pystructnames.bin','data/animations_dictionary_pyarraynames.bin','data/animations_dictionary_pyarray.bin']
 clip_ids={955,956,957,958,959,960,961,962,963,967,969,971,1023,1040,1041,1114,1126}
 # Dictionary names are exact source binary strings, rather than a guessed
 # numbered clip file convention; its table is the same one read by the audit.
 dictionary=(assets/'data/animations_dictionary_pyarray.bin').read_bytes();at=0
 def word():
  nonlocal at
  result=struct.unpack_from('<I',dictionary,at)[0];at+=4;return result
 clips=[]
 for _ in range(word()):
  length=word();clips.append(dictionary[at:at+length].decode('ascii'));at+=length
 assert at==len(dictionary)
 for clip in sorted(clip_ids):names.append('animations/'+clips[clip].replace('\\','/').rsplit('/',1)[-1])
 with zipfile.ZipFile(a.cache) as archive:
  for name in names:
   path=assets/name;raw=path.read_bytes();matching=[n for n in archive.namelist() if n.endswith('/'+path.name)]
   equal=[n for n in matching if archive.read(n)==raw];assert len(equal)==1,(name,equal)
   inputs.append({'asset':name,'cache_entry':equal[0],'sha256':sha(path),'bytes':len(raw)})
 for path,expected in source_hashes.items():assert sha(REPO/path)==expected,path
 manifests={name:sha(ref/(name+'-original-functions.json')) for name in ('composition','scheduler','controls')}
 raw_path=REPO/'port/engine-animation/reports/dynamic-compiled-transforms-host-audit.json';raw_report=json.loads(raw_path.read_text());assert raw_report['validation']=='PASS'
 for name,value in raw_report['source_sha256'].items():assert sha(REPO/name)==value,name
 report={'validation':'PASS','host_audit':audit,'dynamic_host_audit':dynamic_audit,'scheduler_audit':sched,'selection_audit':selected,'controls_audit':controlled,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'executable_sha256':exe_hashes,'dependency_sha256':dependency_hashes,'source_sha256':source_hashes,'build_inputs_sha256':sha(snapshot),'cache_sha256':cache_hash,'cache_inputs':inputs,'original_manifest_sha256':manifests,'original_instruction_evidence':{name:{'report_sha256':sha(ref/(name+'.json')),**{k:v for k,v in row.items() if k not in ('records','import_calls')}} for name,row in evidence.items()},'dynamic_composition_evidence':{'report_sha256':sha(ref/'dynamic/original-composition.json'),**dynamic_evidence},'dynamic_sampler_dependency':{'report_sha256':sha(raw_path),'host_audit':raw_report['host_audit']},'caller_fixture':{'template_bank_key':20000,'library0':'Exact Prince_modular model BRES registered first; fixture key/order, not recovered AddTemplateAnim dictionary identity.'},'scope':'Static and dynamic full-node1/5/10 compiled target domains; actual original two-slot composition with supplied raw/timeline services; actual scheduler/selection callback recursion with leaf audio/equipment services; public metadata/control adapters; real Prince asset host replay and synchronous reentry. Dynamic model/default registration key/order remains caller fixture. No full AIS/audio/material/compressed-channel/GPU/application parity claim.'}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','scene_frames':audit['scene_phase_frames'],'original_composition_records':420,'original_scheduler_cases':48,'original_selection_cases':72,'original_control_cases':2160,'callback_tail_regressions':2,'sanitizer_findings':0}))
if __name__=='__main__':main()
