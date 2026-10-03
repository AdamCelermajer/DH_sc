"""Bundle reachable original Idle/Walk/Attack/Died clips for Crypt actors."""
import argparse,hashlib,json,zipfile
from pathlib import Path
from inventory import EXPECTED
from prepare_actors import strings
from inspect_animation_data import parse
from inspect_animation_tables import sections
def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--project',type=Path,default=Path(__file__).resolve().parents[2]/'android-native');a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 assets=a.project/'app/src/main/assets';schema={n:sections((assets/'data'/n).read_bytes()) for n in ('animations_pyarraynames.bin','animations_pystructnames.bin')};table=parse((assets/'data/animations_pyarray.bin').read_bytes(),schema);paths,_=strings((assets/'data/animations_dictionary_pyarray.bin').read_bytes());actors=json.loads((assets/'actor-provenance.json').read_text());states=[];clip_ids=set()
 def visit(id,stack):
  assert 0<=id<len(table['animations']) and id not in stack and len(stack)<3
  sequence=table['animations'][id];assert sequence['Steps']
  for step in sequence['Steps']:
   if step['Redir']==1:visit(step['Anim'],stack+[id])
   else:assert step['Redir']==0 and 0<=step['Anim']<len(paths);clip_ids.add(step['Anim'])
 for character_id in sorted({r['animation_table'] for r in actors['records'] if r['kind']==1}):
  character=table['characters'][character_id]
  for state in ('Idle','Walk','Attack','Died'):
   id=character[state];visit(id,[]);states.append({'character_table':character_id,'character_name':character['name'],'state':state,'sequence_id':id,'sequence_name':table['animations'][id]['name']})
 inputs=[]
 with zipfile.ZipFile(a.cache) as z:
  for id in sorted(clip_ids):
   name=Path(paths[id]).name;matches=[i for i in z.infolist() if Path(i.filename).name==name];assert len(matches)==1,(name,len(matches));raw=z.read(matches[0]);(assets/'actors'/name).write_bytes(raw);inputs.append({'clip_id':id,'path':paths[id],'asset':'actors/'+name,'entry':matches[0].filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()})
 report={'cache_sha256':EXPECTED,'states':states,'inputs':inputs,'runtime_scope':'Independent per-actor native scheduler. Original clip completion decision, loops, random re-selection, sequences and redirect unwind; blend/event/root-motion/AI/combat dispatch pending.'}
 (assets/'actor-state-provenance.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'states':len(states),'clips':len(inputs)}))
if __name__=='__main__':main()
