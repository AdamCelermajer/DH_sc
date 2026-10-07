"""Bind compiled Crypt authored character names to exact cache AI rows."""
import hashlib,json,struct,zipfile
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1]
DATA=REPO/'port/android-native/app/src/main/assets/data'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def strings(raw):
 at=4;result=[]
 for _ in range(struct.unpack_from('<I',raw)[0]):
  n=struct.unpack_from('<I',raw,at)[0];at+=4;result.append(raw[at:at+n].decode());at+=n
 return result
names=strings((DATA/'character_properties_pyarraynames.bin').read_bytes());fields=strings((DATA/'character_properties_pystructnames.bin').read_bytes())
ai_names=strings((DATA/'ai_pyarraynames.bin').read_bytes());raw=(DATA/'ai_pyarray.bin').read_bytes();at=4;rows=[]
for i,name in enumerate(ai_names):
 prefix=struct.unpack_from('<iiiBIfffi',raw,at);at+=33;n=struct.unpack_from('<I',raw,at)[0];at+=4;script=raw[at:at+n].decode();at+=n;suffix=struct.unpack_from('<iiiff',raw,at);at+=20
 rows.append(dict(row=i,name=name,script_bytes=n,script=script,delayed=prefix[3],type=suffix[2]))
assert at==len(raw)
assert len(names)==448 and len(fields)==224 and fields[1]=='AI'
objects=ROOT/'reports/object-input-provenance.json';world=REPO/'port/android-native/app/src/main/assets/worlds/crypt01-provenance.json'
provenance=json.loads(objects.read_text());authored=json.loads(world.read_text());selected={x['character'] for x in provenance['records'] if x.get('kind')==1}
selected.update(x['charpropsname'] for x in authored['objects'] if 'charpropsname' in x)
properties=(DATA/'character_properties_pyarray.bin').read_bytes();assert len(properties)>=4+448*224*4 and struct.unpack_from('<I',properties)[0]==448
characters=[]
for name in sorted(selected):
 index=names.index(name);sheet=struct.unpack_from('<224i',properties,4+index*224*4);ai=sheet[1];assert 0<=ai<len(rows);characters.append(dict(character=name,character_row=index,authored_AI_index=ai,ai=rows[ai]))
inputs=[DATA/name for name in ['character_properties_pyarray.bin','character_properties_pyarraynames.bin','character_properties_pystructnames.bin','ai_pyarray.bin','ai_pyarraynames.bin','ai_pystructnames.bin']]
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');assert sha(cache)==provenance['cache_sha256']==authored['cache_sha256']
with zipfile.ZipFile(cache) as z:
 for file in inputs:
  entry='com.gameloft.android.GAND.GloftD2SS/files/data/pydata/'+file.name;assert z.read(entry)==file.read_bytes()
 resources=[]
 for name in ['_commons','monster','follower','rene']:
  entry='com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/'+name+'.luac';payload=z.read(entry)
  resources.append(dict(requested=name,resolved_path='data/scripts/ai/'+name+'.luac',entry=entry,size=len(payload),sha256=hashlib.sha256(payload).hexdigest(),header=payload[:12].hex()))
report=dict(validation='PASS',cache_sha256=sha(cache),source_sha256=sha(Path(__file__)),input_sha256={str(x.relative_to(REPO)):sha(x) for x in inputs+[objects,world]},AI_property_index=1,ai_rows=rows,crypt_characters=characters,required_cache_resources=resources,scope='Exact authored property table AI indices and cache rows; dynamic class/property changes and spawning decisions remain runtime producers.')
(HERE/'authored-ai-rows.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(characters=characters,script_kinds=sorted(set(x['script'] for x in rows)))))
