"""Audit staged source banks using an already-built sanitizer reader and actual DSOs."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
KINDS=['skeleton','slime','slime-red','ghost']
LIBS=['/home/adampalace/dh2-world-build/engine-skinning/engine-animation/libdh2_engine_animation.so','/home/adampalace/dh2-world-build/game-data/libdh2_game_data.so','/home/adampalace/dh2-world-build/engine-skinning/engine-animation/scene-materials/libdh2_scene_materials.so']
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wsl(p):
 p=str(p.resolve()).replace('\\','/');return '/mnt/'+p[0].lower()+p[2:]
def command(s):
 r=subprocess.run(['wsl','bash','-lc',s],capture_output=True,text=True);assert r.returncode==0,(s,r.returncode,r.stdout,r.stderr);assert not r.stderr,r.stderr;return r.stdout
def hashes():return {line.split()[1]:line.split()[0]for line in command('sha256sum '+' '.join(shlex.quote(x)for x in LIBS)).splitlines()}
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--staging',type=Path,default=REPO/'.local-inputs/character-monster-bank-assets');p.add_argument('--executable',type=Path,default=REPO/'.local-inputs/character-monster-bank-discovery/host/reader-probe');p.add_argument('--output',type=Path,default=HERE/'native-reader-host-audit.json');a=p.parse_args()
 before=hashes();results={};assets=a.staging/'assets';env='LD_LIBRARY_PATH='+':'.join(sorted({x.rsplit('/',1)[0]for x in LIBS}))+' ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '
 for tag in KINDS:
  meta=assets/'data'/('monster-'+tag+'-animation-bank.bin');m=json.loads(meta.with_suffix('.json').read_text());probe=json.loads((HERE/(tag+'-probe.json')).read_text())
  raw=command(env+' '.join(shlex.quote(wsl(x))for x in [a.executable,meta,assets/m['model_asset'],assets]));r=json.loads(raw);assert r['validation']=='PASS';assert r['scene_nodes']==probe['model_property']['scene_nodes'];assert r['occurrences']==probe['registration_calls_count'] and r['unique_resources']==probe['unique_clip_count'];assert dict(r['lookup'])=={int(k):v for k,v in probe['first_dictionary_engine_index'].items()}
  assert len(r['resources'])==len(probe['resources']);expected={v['clip_id']:v for v in probe['resources']}
  for v in r['resources']:
   e=expected[v['clip_id']];start=e['start'] if e['start']<0x80000000 else e['start']-0x100000000;end=e['end'] if e['end']<0x80000000 else e['end']-0x100000000
   assert (v['start'],v['end'],v['tracks']+v['unbound'],v['skipped'])==(start,end,e['tracks'],0),(tag,v,e)
  template=expected[m['template_clip_id']];signedstart=template['start'] if template['start']<0x80000000 else template['start']-0x100000000
  assert r['constructor_library0_start']==signedstart and r['constructor_library0_end']==template['end'];assert (tag!='ghost')or signedstart==-133
  r['metadata_sha256']=sha(meta);r['model_sha256']=sha(assets/m['model_asset']);r['original_probe_sha256']=sha(HERE/(tag+'-probe.json'));results[tag]=r
 after=hashes();assert before==after,'Shared libraries changed during audit'
 sources=[HERE/'reader-probe.cpp',Path(__file__),REPO/'port/engine-animation/animation.hpp',REPO/'port/engine-animation/animation.cpp',REPO/'port/engine-animation/animation_registration.hpp',REPO/'port/engine-animation/animation_registration.cpp',REPO/'port/game-data/animation_bank.hpp',REPO/'port/game-data/animation_bank.cpp',REPO/'port/scene-materials/scene.hpp',REPO/'port/scene-materials/scene.cpp']
 report={'validation':'PASS','actual_shared_libraries_executed':True,'libraries_sha256':before,'executable_sha256':sha(a.executable),'source_sha256':{x.relative_to(REPO).as_posix():sha(x)for x in sources},'producer_report_sha256':sha(a.staging/'producer-report.json'),'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'banks':results,'samples':sum(r['samples']for r in results.values()),'original_instructions_executed_this_run':False,'scope':'Native PAB1/Player/RegistrationSet/dynamic TransformSet compatibility against exact original-derived resource/occurrence/bounds probes. Finite raw-target samples and signed Ghost bounds. No original full-frame, FSM, resource manager or GPU parity claim.'}
 a.output.relative_to(REPO);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','banks':len(results),'samples':report['samples'],'ghost_raw_start':-133,'report':str(a.output)}))
if __name__=='__main__':main()
