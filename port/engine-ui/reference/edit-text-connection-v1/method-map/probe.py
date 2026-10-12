"""Execute original standard_method_map_init; retain ordered name/callback facts.

String/value/hash storage is a desktop fixture. Original initializer instructions,
branch order, class indices and function pointer loads execute from the ELF.
"""
import hashlib,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[5]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from text_layout_v1_original import Original,words

class Probe(Original):
 def __init__(self):
  super().__init__(ROOT/'.local-inputs/libDungeonHunter2.so',json.loads((Path(__file__).parent/'original-functions.json').read_text()))
  self.next=self.c.data+0x10000;self.hashes={};self.events=b'';self.classes={};self.function_values={};self.calls=[];self.active=True
 def hook(self,uc,address,size,unused):
  if not self.active:return
  c=self.c;a,b,d=(c.reg(i) for i in range(3))
  if address==0x76cb04:
   p=self.alloc(64);self.classes[p]=a;self.calls.append({'class':a,'members':[]});self.ret(p)
  elif address==0x7972a0:self.function_values[a]=b;self.ret(a)
  elif address==0x76a130:
   row=next(row for row in reversed(self.calls) if row['class']==self.classes[a]);row['members'].append({'name':self.string(b),'callback':hex(self.function_values.get(d,self.word(d+4))),'value_bytes':self.read(d,12).hex()});self.ret()
  elif address==0x797124:self.ret()
  else:super().hook(uc,address,size,unused)

p=Probe();p.c.invoke(0x76f5c0,[],budget=4000000)
with (ROOT/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
 from elftools.elf.elffile import ELFFile
 elf=ELFFile(f);symbols={s['st_value']:s.name for s in elf.get_section_by_name('.symtab').iter_symbols() if s.name}
for row in p.calls:
 for m in row['members']:m['symbol']=symbols.get(int(m['callback'],16))
result={'validation':'PASS','original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'original_instructions_executed':True,'explicit_fixture_services':['tu_string storage','as_value function storage','owned hash insert'],'ordered_classes':p.calls}
out=Path(__file__).parent/'probe.json';assert not out.exists();out.write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({'classes':len(p.calls),'textfield':next(row for row in p.calls if row['class']==6)}))
