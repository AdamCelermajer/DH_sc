"""Real original Constructor/Split/Equip callers with explicit Item text/power failures."""
from __future__ import annotations
import hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from fresh_inventory_owned_v4_original import OriginalOwned,W
from player_saved_inventory_v1_original import ELF_SHA,metadata
ADDRESSES=[0x400634,0x3fc3e0,0x3fc26c,0x3fbc58,0x3fa17c,0x3f9e58,0x3f9e80,0x3fa038]
class Prefix(Exception):pass
class Original(OriginalOwned):
 def __init__(self):
  self.active=False;self.failure=0;self.pending=0;self.effects=[];self.words_seen=set();self.power_calls=0
  super().__init__();self.uc.hook_add(UC_HOOK_CODE,self.source_hook)
 def source_hook(self,uc,a,n,u):
  if not self.active:return
  if any(lo<=a<hi for lo,hi in [(0x400634,0x400634+956),(0x3fc3e0,0x3fc3e0+180),(0x3fc26c,0x3fc26c+372),(0x3fbc58,0x3fbc60)]):self.words_seen.add(a)
  if a==0x3fc26c:self.pending=self.reg(0)
 def inventory_service(self,uc,a,n,u):
  if not self.loading:return
  if a in [0x3fb754,0x3fb290,0x3facdc]:
   if self.active:
    caller=uc.reg_read(self.lr)-4;self.effects.append([a,caller,self.word(self.reg(0)+0x54),uc.mem_read(self.reg(0)+0x68,1)[0]])
    if (self.failure in [1,2,3] and caller==[0,0x3fc36c,0x3fc374,0x3fc37c][self.failure]) or (self.failure==4 and caller==0x3fc44c):raise Prefix('required original text callee failure')
   self.returned();return
  if a==0x3fbc60:
   item,power=self.reg(0),self.reg(1);assert self.reg(2)==0xffffffff
   start=self.word(item+0x5c);end=self.word(item+0x60)
   if not start:start=self.allocate(1024);end=start;self.pointer(item+0x5c,start)
   uc.mem_write(end,bytes(32));self.pointer(end,power);self.pointer(item+0x60,end+32);self.pointer(item+0x64,start+1024)
   if self.active:
    self.power_calls+=1;self.effects.append([a,uc.reg_read(self.lr)-4,power,self.power_calls])
    if (self.failure==5 and self.power_calls==1) or (self.failure==6 and self.power_calls==2):raise Prefix('required Power text failure after append')
   self.returned();return
  if a==0x3fe330 and self.active and self.failure==7:raise Prefix('required fullness callee failure after storage')
  super().inventory_service(uc,a,n,u)
 def item_words(self,p):
  start=self.word(p+0x5c);count=(self.word(p+0x60)-start)//32
  return [self.word(p+4),int.from_bytes(self.uc.mem_read(p+0x50,2),'little'),self.word(p+0x54),self.uc.mem_read(p+0x68,1)[0],count]+[self.word(start+32*j) for j in range(count)]
 def snapshot(self):
  begin=self.word(self.inv+8);end=self.word(self.inv+0xc);out=[self.uc.mem_read(self.inv+0x2e,1)[0],(end-begin)//4]
  stored=[]
  for j in range(out[1]):
   slot=self.word(begin+4*j);p=self.word(slot);stored.append(p);out+=self.item_words(p)+[self.uc.mem_read(slot+4,1)[0],self.uc.mem_read(slot+5,1)[0]]
  pending=self.pending if self.pending and self.pending not in stored else 0;out+=[int(bool(pending))]
  if pending:out+=self.item_words(pending)
  for set in range(2):
   cells=self.word(self.word(self.inv+0x14)+12*set)
   for j in range(9):
    cell=self.word(cells+j*4);out.append(next((k for k in range(len(stored)) if cell==self.word(begin+4*k)),0xffffffff))
  return out
def capture(elf,cache,out):
 assert hashlib.sha256(elf.read_bytes()).hexdigest()==ELF_SHA
 assert elf.resolve()==(ROOT/'.local-inputs/libDungeonHunter2.so').resolve()
 with elf.open('rb') as f:
  e=ELFFile(f);sy=list(e.get_section_by_name('.symtab').iter_symbols());functions=[]
  for at in ADDRESSES:
   s=next(s for s in sy if int(s['st_value'])==at and int(s['st_size'])>0);n=int(s['st_size']);g=next(g for g in e.iter_segments() if g['p_type']=='PT_LOAD' and g['p_vaddr']<=at and at+n<=g['p_vaddr']+g['p_filesz']);f.seek(g['p_offset']+at-g['p_vaddr']);functions.append({'original_symbol':s.name,'elf_address':hex(at),'size':n,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
 _,descriptions=metadata(cache);power=next(j for j,x in enumerate(descriptions) if x!=0xffffffff)
 source=Original();records=[];seen=set()
 # operation0 Equip; operation1 direct Split. Synthetic only Stackable row664.
 cases=[(0,4,0,1,0,0),(0,4,1,0,0,0),(0,4,1,1,1,0),(0,2,0,0,1,0),
  (1,4,0,1,0,0),(1,4,0,1,1,0),(1,4,0,1,1,8),(1,4,0,1,1,9)]
 cases += [(0,4,1,1,1,f) for f in range(1,8)]
 for operation,qty,selected,forced,identified,failure in cases:
  source.fresh_inventory(-1);source.pointer(source.inv+4,source.character);source.loading=True;source.capture=True;source.requests=[];source.infinite=0;source.toggle_stats=False;source.mutation=0;source.minimal=False
  source.uc.mem_write(source.table+164*664+0x1c,b'\1');source.perform(1,664,qty,1);original=source.word(source.slot(0));source.pointer(original+0x54,137);source.uc.mem_write(original+0x68,bytes([identified]));powers=source.allocate(64);source.uc.mem_write(powers,W(power)+bytes(28)+W(power)+bytes(28));source.pointer(original+0x5c,powers);source.pointer(original+0x60,powers+64);source.pointer(original+0x64,powers+64);source.uc.mem_write(source.inv+0x2e,bytes([selected]));source.pending=0;source.failure=failure;source.power_calls=0;source.effects=[];source.words_seen.clear();source.active=True;failed=False
  amount=0 if failure==8 else qty if failure==9 else qty-1
  try:
   if operation==0:source.invoke(0x400634,[source.inv,1,0,forced])
   else:source.invoke(0x3fc3e0,[original,amount])
  except Prefix:failed=True
  finally:source.active=False
  seen|=source.words_seen
  snapshot=source.snapshot();records.append({'input':[operation,qty,selected,forced,identified,failure,power,amount],'failed':failed,'snapshot':snapshot,'effects':source.effects})
 scope='Whole original Equip956B, Split180B and valid Constructor372B instruction bodies, actual SetValue8B and source metadata/getters. Stock cache has no naturally stackable equippable row; only row664 Stackable is synthetically set1, retaining all other actual fields. Item text, AddPower full32B entry storage/text, Debug, memory/string/libc transports are explicit fixtures. Required callee failures are injected source-prefix boundaries, not original C++ exception behavior. Vitals/property/Skin/VM/profile/startup are outside this proof.'
 manifest={'original_sha256':ELF_SHA,'functions':functions,'scope':scope,'attribution':'Gameloft original source; existing sole V4 inventory and V5 power/presentation/equipment/text contributions preserve Adam791e961 attribution. This repair adds only caller-owned borrowed lifetime boundaries.'}
 out.mkdir(parents=True,exist_ok=True);blob=W(0x314c5249,len(records))
 for r in records:
  blob+=W(*r['input'],int(r['failed']),len(r['snapshot']))+W(*r['snapshot'])+W(len(r['effects']))+b''.join(W(*x) for x in r['effects'])
 (out/'original-cases.bin').write_bytes(blob);(out/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n')
 report={'validation':'PASS','cases':len(records),'required_failure_prefix_cases':sum(r['failed'] for r in records),'distinct_original_instruction_words':len(seen),'scope':scope,'case_snapshots':records,'original_sha256':ELF_SHA}
 (out/'original-capture.json').write_text(json.dumps(report,indent=2)+'\n');return report
