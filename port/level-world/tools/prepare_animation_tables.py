"""Add original complete animation tables and all Crypt idle alternatives.

Only preparation uses Python. Android reads the five original tables natively.
"""
import argparse,hashlib,json,zipfile
from pathlib import Path
from inventory import EXPECTED
from prepare_actors import strings
from inspect_animation_data import parse
from inspect_animation_tables import sections
TABLES=('animations_pyarray.bin','animations_pyarraynames.bin','animations_pystructnames.bin','animations_dictionary_pyarray.bin','animations_dictionary_pyarraynames.bin')
def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--project',type=Path,default=Path(__file__).resolve().parents[2]/'android-native');a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 assets=a.project/'app/src/main/assets';inputs=[];raws={}
 with zipfile.ZipFile(a.cache) as z:
  def extract(name,folder):
   matches=[i for i in z.infolist() if Path(i.filename).name==name];assert len(matches)==1,(name,len(matches));raw=z.read(matches[0]);target=assets/folder/name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(raw)
   inputs.append({'asset':folder+'/'+name,'entry':matches[0].filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()});return raw
  for name in TABLES:raws[name]=extract(name,'data')
  schema={name:sections(raws[name]) for name in TABLES[1:3]};table=parse(raws[TABLES[0]],schema);paths,_=strings(raws[TABLES[3]])
  actors=json.loads((assets/'actor-provenance.json').read_text());states=[];clip_names=set()
  for row in actors['records']:
   if row['kind']!=1:continue
   char=table['characters'][row['animation_table']];sequence=table['animations'][char['Idle']];choices=[]
   for step in sequence['Steps']:
    assert step['Redir']==0 and step['Anim']>=0;path=paths[step['Anim']];clip_names.add(Path(path).name);choices.append({'clip_id':step['Anim'],'path':path,'speed':step['Speed']})
   states.append({'character':row['character'],'animation_table':row['animation_table'],'idle_id':char['Idle'],'idle_name':sequence['name'],'loop':sequence['Loop'],'type':sequence['Type'],'choices':choices})
  for name in sorted(clip_names):extract(name,'actors')
  report={'cache_sha256':EXPECTED,'inputs':inputs,'idle_states':states,'sequence_count':len(table['animations']),'character_count':len(table['characters']),'clip_path_count':len(paths),'serialized_bytes':table['consumed'],'policy':'Native original initial idle selection and speed; shared resource clock and development seed 1. Idle completion re-selection, blending, AI and combat pending.'}
  (assets/'animation-provenance.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'inputs':len(inputs),'idle_clip_names':sorted(clip_names),'serialized_bytes':table['consumed']}))
if __name__=='__main__':main()
