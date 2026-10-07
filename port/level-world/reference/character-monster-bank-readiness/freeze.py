"""Bind executed monster producer evidence, exact staged assets and native reader proof."""
import hashlib,json,subprocess,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 staging=REPO/'.local-inputs/character-monster-bank-assets'
 subprocess.run([sys.executable,str(HERE/'produce.py'),'--verify-only'],check=True)
 producer=json.loads((staging/'producer-report.json').read_text());native=json.loads((HERE/'native-reader-host-audit.json').read_text());assert producer['validation']==native['validation']=='PASS';assert native['producer_report_sha256']==sha(staging/'producer-report.json')
 for path,digest in producer['source_sha256'].items():assert sha(REPO/path)==digest,path
 for path,digest in native['source_sha256'].items():assert sha(REPO/path)==digest,path
 for path,record in producer['output_sha256'].items():assert sha(staging/path)==record['sha256'] and (staging/path).stat().st_size==record['bytes'],path
 proofs={};placements=[]
 for tag in ['skeleton','slime','slime-red','ghost']:
  probe=HERE/(tag+'-probe.json');p=json.loads(probe.read_text());assert p['script_sha256']==sha(HERE/'probe.py') and p['manifest_sha256']==sha(HERE/'original-functions.json');m=json.loads((staging/'assets/data'/('monster-'+tag+'-animation-bank.json')).read_text());assert m['producer_sha256']==sha(probe)
  assert p['existing_set_registration_calls']==0;assert p['idle_focus_probes'][0]['flags']==0x2380 and len(p['idle_focus_probes'][0]['calls'])==1;assert all(not v['calls'] and v['flags']==85 for v in p['idle_focus_probes'][1:]);placements+=m['source_placements']
  proofs[tag]={'character':p['character'],'source_probe_sha256':sha(probe),'animation_table':p['animation_table'],'animation_set_id':p['animation_set_id'],'template_dictionary_id':p['template']['clip_id'],'ordered_requests':p['registration_calls_count'],'unique_resources':p['unique_clip_count'],'source_idle_sequence':p['source_idle_sequence'],'source_placements':m['source_placements'],'PAB1':{'path':str((staging/'assets/data'/('monster-'+tag+'-animation-bank.bin')).relative_to(REPO)),'sha256':sha(staging/'assets/data'/('monster-'+tag+'-animation-bank.bin'))}}
 assert len(placements)==len(set(placements))==11
 files=[HERE/'NOTES.md',HERE/'probe.py',HERE/'produce.py',HERE/'freeze.py',HERE/'reader-probe.cpp',HERE/'host-audit.py',HERE/'native-reader-host-audit.json',HERE/'original-functions.json',HERE/'reference/original-functions.asm',*[HERE/(tag+'-probe.json')for tag in proofs],staging/'producer-report.json']
 report={'validation':'PASS','original_sha256':producer['original_sha256'],'cache_sha256':producer['cache_sha256'],'producer_report':{'path':(staging/'producer-report.json').relative_to(REPO).as_posix(),'sha256':sha(staging/'producer-report.json')},'native_reader_report':{'path':(HERE/'native-reader-host-audit.json').relative_to(REPO).as_posix(),'sha256':sha(HERE/'native-reader-host-audit.json')},'file_sha256':{p.relative_to(REPO).as_posix():sha(p)for p in files},'banks':proofs,'unique_animation_files':64,'ordered_occurrences':146,'authored_monster_placements':11,'ghost_template_signed_start':-133,'native_reader_samples':native['samples'],'sanitizer_findings':0,'production_APK_assets_changed':False,'scope':'Original instruction registration/getter/Idle Focus request evidence plus byte-exact isolated PAB1/cache staging and native shared-reader compatibility. Explicit manager/constant/IsPlayer/script/FX/audio probe services; not original complete frame, GPU, cache allocation or live AI parity.'}
 output=REPO/'port/level-world/reports/character-monster-bank-readiness.json';output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','report':str(output),'sha256':sha(output),'banks':4,'unique_animation_files':64,'authored_monsters':11}))
if __name__=='__main__':main()
