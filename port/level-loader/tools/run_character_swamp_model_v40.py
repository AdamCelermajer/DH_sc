"""Run only after parent confirms the entire changed V5 graph is READY.
The executable must be parent's coherently built V40 target. Never builds,
launches a game, changes source caches, or selects an older target itself.
"""
from pathlib import Path
import argparse,hashlib,json,subprocess,xml.etree.ElementTree as ET
def main():
 p=argparse.ArgumentParser();p.add_argument('--binary',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--design-fixture',type=Path,required=True);p.add_argument('--save-dir',type=Path,required=True);p.add_argument('--coherent-ready',action='store_true',required=True,help='Parent verified full coherent V5 graph after actor/visual/manager changes');p.add_argument('--output',type=Path,required=True);a=p.parse_args();base=Path(__file__).resolve().parents[1];a.output.mkdir(parents=True,exist_ok=True)
 sha=lambda q:hashlib.sha256(q.read_bytes()).hexdigest()
 assert sha(a.cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679','Original cache changed'
 for q in [a.binary,a.design_fixture]:assert q.is_file(),q
 assert a.binary.stat().st_mtime_ns>=max((base/'vendor/character-rng-integration-v5-loading/port/level-world'/n).stat().st_mtime_ns for n in ['retained_character_actor_v1.hpp','retained_character_actor_v1.cpp','character_model_name_v38.cpp']),'Binary predates actual-field adoption'
 def w(q):return '/mnt/c/'+str(q.resolve()).replace('\\','/')[3:]
 xml=a.output/'character-swamp-model-v40.xml';r=subprocess.run(['wsl.exe','-d','Ubuntu','--',w(a.binary),w(a.cache),w(a.design_fixture),w(a.save_dir),w(xml)],capture_output=True,text=True);(a.output/'character-swamp-model-v40.log').write_text(r.stdout+r.stderr);print(r.stdout+r.stderr,flush=True);assert r.returncode==0
 root=ET.parse(xml).getroot();coverage=root.find('Coverage');assert coverage is not None;rows=[dict(node.attrib,tag=node.tag) for node in root if node.tag in ['Entity','Blocked']];actual=[dict(n.attrib) for n in root.findall('ActualState')];characters=[n for n in rows if n['kind']=='Character'];chests=[n for n in rows if n['kind']=='OpenableContainer'];assert len(characters)==50 and len(chests)==5 and len(actual)==50
 assert coverage.get('production_rng_unchanged')=='true' and coverage.get('whole_init_rng_order_verified')=='false' and coverage.get('rendered')=='false'
 default=[n for n in characters if n['charpropsname']=='DefaultFairy'];assert len(default)==1 and default[0]['model_id']=='34' and 'faeries_02_celeste' in default[0]['asset']
 receipt={'schema':'dh2-character-swamp-model-v40-coverage','validation':'PASS bounded authored-reference probe','scope':root.get('scope'),'coverage':dict(coverage.attrib),'entities':rows,'existing_actual_records_observed':actual,'nonvisual':[n for n in rows if n['status']=='nonvisual'],'service_or_asset_gaps':[n for n in rows if n['status'] not in ['asset','nonvisual']],'original_cache_sha256':sha(a.cache),'binary_sha256':sha(a.binary),'xml_sha256':sha(xml),'sources':{str(q.relative_to(base)):sha(q) for q in [base/'tests/character_swamp_model_v40_probe.cpp',base/'tests/character_swamp_model_v40_export.inc',base/'tools/run_character_swamp_model_v40.py']},'coherent_parent_ready_asserted':a.coherent_ready,'binary_mtime_guard_only':'fresh timestamp is not independently a full dependency-coherence proof; parent READY is required','rendered':False,'playable':False,'whole_init_rng_order_verified':False,'emulator_launched':False}
 (a.output/'character-swamp-model-v40-coverage.json').write_text(json.dumps(receipt,indent=2)+'\n')
 lines=['# SWAMP actual-owner model probe V40','',root.get('scope'),'','| Placement | Kind | Row | Model | Status | Asset / service |','|---|---|---:|---:|---|---|']
 for n in rows:lines.append('| '+' | '.join(n.get(k,'').replace('|','\\|') for k in ['name','kind','properties_row','model_id','status'])+' | '+(n['asset'] or n['reason']).replace('|','\\|')+' |')
 lines.extend(['','All 50 Character and 5 chest placements accounted. Existing Character sheets/caches/master and production RNG remained unchanged. Asset references come from explicitly isolated fresh canonical records and original tables. Later nonNULL Faery master requires actual save/table services. These checks do not prove original whole Init RNG order, rendering, activation or playable SWAMP.'])
 (a.output/'character-swamp-model-v40-coverage.md').write_text('\n'.join(lines)+'\n');print(json.dumps(receipt['coverage'],indent=2))
if __name__=='__main__':main()
