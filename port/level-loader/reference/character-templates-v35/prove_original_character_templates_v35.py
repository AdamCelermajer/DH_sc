from pathlib import Path
import sys,struct,hashlib,json,zipfile
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from items_differential import Original as Base
class Original(Base):
 def external(self,uc,address,size,unused):
  if address==self.callback+32:self.returned(0) # Explicit non-player virtual service.
  else:super().external(uc,address,size,unused)
W=lambda *v:struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
z=zipfile.ZipFile(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip');prefix='com.gameloft.android.GAND.GloftD2SS/files/data/pydata/'
records=z.read(prefix+'character_templates_pyarray.bin');names=z.read(prefix+'character_templates_pyarraynames.bin');schema=z.read(prefix+'character_templates_pystructnames.bin')
c=Original(root/'.local-inputs/libDungeonHunter2.so',{'functions':[]});c.blob=records;c.cursor=4;count=struct.unpack_from('<I',records)[0];assert count==121;rows=[]
table=c.data+0x10000
for i in range(count):
 row=table+12*i;c.uc.mem_write(row,bytes(12));start=c.cursor;c.invoke(0x4dcd38,[row,c.stream]);n=c.word(row+4);ptr=c.word(row+8);values=[c.word(ptr+8*j+4) for j in range(n)]
 assert records[start:c.cursor]==W(n,*values);rows.append(values)
assert c.cursor==len(records)
at=4;labels=[]
for i in range(count):n=struct.unpack_from('<I',names,at)[0];at+=4;labels.append(names[at:at+n].decode());at+=n
assert at==len(names) and struct.unpack_from('<I',names)[0]==count
lookup=c.data+0x20000;storage=lookup+0x1000
for i,name in enumerate(labels):b=name.encode()+b'\0';c.uc.mem_write(storage,b);c.pointer(lookup+4*i,storage);storage+=len(b)
got_template=0x3b3714+c.word(0x3b3790);c.pointer(c.word(got_template+c.word(0x3b3794)),count);c.pointer(c.word(got_template+c.word(0x3b3798)),lookup)
got_props=0x3b3d58+c.word(0x3b3fe0);c.pointer(c.word(got_props+c.word(0x3b3fe4)),table)
seed=c.word(got_props+c.word(0x3b3fe8));calls=c.word(got_props+c.word(0x3b3fec));obj=c.data+0x30000;vt=obj+0x2000;name_storage=vt+0x1000
c.uc.mem_write(obj,bytes(0x1600));c.pointer(obj,vt);c.pointer(vt+0x28,c.callback+32)
cases=[]
for name in labels+['MissingTemplate','swamp_commontype1']:
 for initial in (0,1,0xffffffff):
  b=name.encode()+b'\0';c.uc.mem_write(name_storage,b);c.pointer(obj+0x13a8,name_storage+len(name));c.pointer(obj+0x13ac,name_storage)
  c.uc.mem_write(obj+0x13c8,struct.pack('<hh',-1,-1));c.pointer(seed,initial);c.pointer(calls,7)
  value=c.invoke(0x3b3d38,[obj]);props,template=struct.unpack('<hh',c.uc.mem_read(obj+0x13c8,4));after_seed=c.word(seed);after_calls=c.word(calls)
  assert value==(props&0xffffffff)
  if name in labels:assert template==labels.index(name) and props in [struct.unpack('<h',struct.pack('<H',v&65535))[0] for v in rows[template]] and after_calls==8
  else:assert (props,template,after_seed,after_calls)==(-1,-1,initial,7)
  cases.append({'name':name,'initial_seed':initial,'initial_calls':7,'initial_props':-1,'initial_template':-1,'props':props,'template':template,'seed':after_seed,'calls':after_calls})
# Whole source cache fast path does not reroll or resolve a supplied name.
for name in ('Swamp_CommonType1','MissingTemplate'):
 b=name.encode()+b'\0';c.uc.mem_write(name_storage,b);c.pointer(obj+0x13a8,name_storage+len(name));c.pointer(obj+0x13ac,name_storage);c.uc.mem_write(obj+0x13c8,struct.pack('<hh',360,99));c.pointer(seed,123);c.pointer(calls,7)
 assert c.invoke(0x3b3d38,[obj])==360 and c.word(seed)==123 and c.word(calls)==7
 cases.append({'name':name,'initial_seed':123,'initial_calls':7,'initial_props':360,'initial_template':99,'props':360,'template':99,'seed':123,'calls':7})
out=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader/reference/character-templates-v35');out.mkdir(exist_ok=True)
for name,data in [('character_templates_pyarray.bin',records),('character_templates_pyarraynames.bin',names),('character_templates_pystructnames.bin',schema)]: (out/name).write_bytes(data)
gold=W(len(cases))
for case in cases:
 b=case['name'].encode();gold+=W(len(b))+b+W(*(case[k] for k in ['initial_seed','initial_calls','initial_props','initial_template','props','template','seed','calls']))
(out/'selection-original-gold-v35.bin').write_bytes(gold)
report={'validation':'PASS','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'actual_original_rows':len(rows),'record_bytes_consumed':c.cursor,'selector_cases':len(cases),'gold_sha256':hashlib.sha256(gold).hexdigest(),'scope':'Whole original CharTemplate/CharInfoName record readers and non-player SafeGetCharPropsId/template lookup/inline RNG branch; stream/storage and IsPlayer=false services explicit. Player/save branch outside this adapter.','swamp_rows':{name:row for name,row in zip(labels,rows) if name.startswith('Swamp')},'inputs_sha256':{name:hashlib.sha256(data).hexdigest() for name,data in [('character_templates_pyarray.bin',records),('character_templates_pyarraynames.bin',names),('character_templates_pystructnames.bin',schema)]}}
(out/'original-character-templates-v35.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','original_rows':len(rows),'selector_cases':len(cases),'gold_sha256':report['gold_sha256']}))
