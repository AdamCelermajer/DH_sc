"""Bundle original Android Knight Idle/Walk/Run reachable clips, with provenance.

The supplied Android ELF queries AnimStancedAnim/SL__LIST_IPHONE and
AnimStances/COUNT_IPHONE literally. Values are read from the verified cache.
This preparation tool does not select or advance live animations.
"""
import argparse,hashlib,json,struct,sys,zipfile
from pathlib import Path
REPO=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(REPO/'port/level-world/tools'))
from inventory import EXPECTED
from prepare_actors import strings
from prepare_player_combat import animation_tables

def constants(raw):
 offset=0
 def integer():
  nonlocal offset
  if offset+4>len(raw):raise ValueError('Truncated constants')
  value=struct.unpack_from('<i',raw,offset)[0];offset+=4;return value
 def text():
  nonlocal offset
  n=integer()
  if not 0<n<=4096 or offset+n>len(raw):raise ValueError('Invalid constant name')
  value=raw[offset:offset+n].decode('ascii');offset+=n;return value
 count=integer();out={}
 if not 0<=count<=10000:raise ValueError('Invalid constant group count')
 for _ in range(count):
  group=text();count=integer();values={}
  if group in out or not 0<=count<=10000:raise ValueError('Duplicate group or invalid count')
  for _ in range(count):
   name=text()
   if name in values:raise ValueError('Duplicate constant')
   values[name]=integer()
  out[group]=values
 if offset!=len(raw):raise ValueError('Unexpected constants suffix')
 return out

def main():
 p=argparse.ArgumentParser();p.add_argument('--cache',type=Path,required=True)
 p.add_argument('--project',type=Path,action='append');a=p.parse_args()
 projects=a.project or [REPO/'port/android-native']
 with a.cache.open('rb') as stream:digest=hashlib.file_digest(stream,'sha256').hexdigest()
 if digest!=EXPECTED:raise ValueError('Cache differs from verified original archive')
 payloads={};input_rows=[];clip_rows=[];states=[]
 with zipfile.ZipFile(a.cache) as archive:
  entries={}
  for entry in archive.infolist():entries.setdefault(entry.filename.lower(),[]).append(entry)
  def read(path):
   key='com.gameloft.android.gand.gloftd2ss/files/'+path.lower()
   found=entries.get(key,[])
   if len(found)!=1:raise ValueError('Missing or duplicate original entry: '+path)
   raw=archive.read(found[0]);input_rows.append({'entry':found[0].filename,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()});return raw,found[0].filename
  def table(name):return read('data/pydata/'+name)[0]
  names,_=strings(table('character_properties_pyarraynames.bin'))
  fields,_=strings(table('character_properties_pystructnames.bin'))
  props_raw=table('character_properties_pyarray.bin');row=names.index('KnightPlayerBase')
  if len(fields)!=224 or struct.unpack_from('<I',props_raw)[0]!=len(names):raise ValueError('Character table dimensions differ')
  props=struct.unpack_from('<224i',props_raw,4+row*896);table_id=props[fields.index('AnimTable')]
  sequences,characters=animation_tables(table('animations_pyarray.bin'))
  raw=table('animations_pystructnames.bin')
  for _ in range(4):state_names,n=strings(raw);raw=raw[n:]
  paths,_=strings(table('animations_dictionary_pyarray.bin'))
  cst=constants(table('animations_pycst.bin'))
  mask=cst['AnimStancedAnim']['SL__LIST_IPHONE'];stance_count=cst['AnimStances']['COUNT_IPHONE']
  if not 0<stance_count<=32:raise ValueError('Invalid authored stance count')
  clips={}
  def collect(index,depth=0):
   if depth>=3 or not 0<=index<len(sequences):raise ValueError('Sequence reference outside original depth/range')
   for step in sequences[index]['steps']:
    if step['redir']==1:collect(step['anim'],depth+1)
    elif step['redir']==0 and 0<=step['anim']<len(paths):clips[step['anim']]=paths[step['anim']]
    else:raise ValueError('Invalid authored direct clip')
  for state in ('Idle','Walk','Run'):
   base=characters[table_id][state_names.index(state)][0]
   enabled=bool(mask&cst['AnimStancedAnim']['SL_'+state.upper()])
   roots=range(base,base+(stance_count if enabled else 1))
   state_rows=[]
   for root in roots:
    collect(root);state_rows.append({'sequence_id':root,**sequences[root]})
   states.append({'state':state,'base_sequence':base,'stance_enabled':enabled,'sequences':state_rows})
  for clip_id,path in sorted(clips.items()):
   raw,entry=read(path);name=Path(path.replace('\\','/')).name
   asset='animations/'+name
   if asset in payloads and payloads[asset]!=raw:raise ValueError('Colliding clip filenames')
   payloads[asset]=raw;clip_rows.append({'clip_id':clip_id,'asset':asset,'entry':entry,'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()})
 report={'cache_sha256':digest,'character':'KnightPlayerBase','character_row':row,'animation_table':table_id,'platform_binding':'Supplied Android ELF literal queries SL__LIST_IPHONE and COUNT_IPHONE','stance_mask':mask,'stance_count':stance_count,'states':states,'clips':clip_rows,'inputs':input_rows,'scope':'Authored reachable Idle/Walk/Run clip bank only. Existing scheduler owns random/nested selection and completion; original timeline/replay services own clip time/root restart; live touch-heading production remains an explicit input adapter.'}
 encoded=(json.dumps(report,indent=2)+'\n').encode()
 for project in projects:
  assets=project.resolve()/'app/src/main/assets'
  if not assets.is_dir():raise ValueError('Missing project assets directory: '+str(assets))
  (assets/'animations').mkdir(exist_ok=True)
  for name,raw in payloads.items():(assets/name).write_bytes(raw)
  (assets/'player-locomotion-provenance.json').write_bytes(encoded)
 print(json.dumps({'projects':[str(p.resolve()) for p in projects],'clips':clip_rows,'provenance_sha256':hashlib.sha256(encoded).hexdigest()}))
if __name__=='__main__':main()
