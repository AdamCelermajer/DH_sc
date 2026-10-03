"""Bundle original default Knight stationary combo clips from authored tables."""
import argparse,hashlib,json,struct,zipfile
from pathlib import Path
from prepare_actors import strings
from inventory import EXPECTED

def animation_tables(raw):
 offset=0
 def take(fmt):
  nonlocal offset
  value=struct.unpack_from(fmt,raw,offset)[0];offset+=struct.calcsize(fmt);return value
 def integer():return take('<i')
 def count():
  n=integer();assert 0<=n<=10000;return n
 def integers():return [integer() for _ in range(count())]
 sequences=[]
 for _ in range(count()):
  loop=integer();steps=[]
  for _ in range(count()):
   steps.append(dict(zip(('anchor_fx','anim','blend_out','cam','cam_dir','fx','move_go','random_cam','redir','sound','speed','swoosh'),(take('<B'),integer(),integer(),integer(),take('<B'),integer(),take('<B'),integers(),integer(),integer(),take('<f'),take('<B')))))
  sequences.append({'loop':loop,'steps':steps,'type':integer()})
 for _ in range(count()):integers();integer();integer();integer();integer()
 characters=[]
 for _ in range(count()):characters.append([integers() if j in (15,31) else [integer()] for j in range(37)])
 assert offset==len(raw);return sequences,characters

def main():
 p=argparse.ArgumentParser();p.add_argument('cache',type=Path);p.add_argument('--project',type=Path,default=Path(__file__).resolve().parents[2]/'android-native');a=p.parse_args()
 with a.cache.open('rb') as f:assert hashlib.file_digest(f,'sha256').hexdigest()==EXPECTED
 assets=a.project/'app/src/main/assets';data=assets/'data';names,_=strings((data/'character_properties_pyarraynames.bin').read_bytes());fields,_=strings((data/'character_properties_pystructnames.bin').read_bytes());row=names.index('KnightPlayerBase');properties=struct.unpack_from('<224i',(data/'character_properties_pyarray.bin').read_bytes(),4+row*896);table=properties[fields.index('AnimTable')]
 raw_fields=(data/'animations_pystructnames.bin').read_bytes()
 for _ in range(4):states,used=strings(raw_fields);raw_fields=raw_fields[used:]
 sequences,characters=animation_tables((data/'animations_pyarray.bin').read_bytes());paths,_=strings((data/'animations_dictionary_pyarray.bin').read_bytes());root=characters[table][states.index('AttackStatic')][0];clips={}
 def collect(index,depth=0):
  assert depth<3 and 0<=index<len(sequences)
  for step in sequences[index]['steps']:
   if step['redir']==1:collect(step['anim'],depth+1)
   else:assert 0<=step['anim']<len(paths);clips[step['anim']]=paths[step['anim']]
 collect(root);assert len(clips)==9
 inputs=[]
 with zipfile.ZipFile(a.cache) as archive:
  entries={entry.filename.lower():entry for entry in archive.infolist()}
  for index,path in sorted(clips.items()):
   entry=entries['com.gameloft.android.gand.gloftd2ss/files/'+path.lower()];raw=archive.read(entry);name=Path(path).name;destination=assets/'animations'/name;destination.write_bytes(raw)
   inputs.append({'clip_id':index,'asset':'animations/'+name,'entry':entry.filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()})
 report={'cache_sha256':EXPECTED,'character':'KnightPlayerBase','row':row,'class_id':properties[26],'animation_table':table,'state':'AttackStatic','root_sequence':root,'inputs':inputs,'scope':'Original stationary three-combo sequence. Input/target selection, blending/root motion, equipment, FX/audio and full player lifecycle are separate.'}
 (assets/'player-combat-provenance.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
