"""Original selected Revived vtables and EnemySpotted/target event routing.

This read-only discovery is original-only: no native routing/body parity claim.
Complete Character and CharAI routers execute; final virtual/FSM providers are
explicit services. The selected Revived body executes its actual single bx lr.
"""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-target-event-route'
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original,words
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 engine=REPO/'.local-inputs/libDungeonHunter2.so';manifest=json.loads((REF/'original-functions.json').read_text());c=Original(engine,manifest);d=c.data;owners=[d+0x10000,d+0x14000];ai=owners[0]+0x3c8;controller=d+0x20000;vt=d+0x21000;callback=d+0x22000;trace=[];records=[];event=payload=blocked=accepted=mutate=0
 def hook(uc,a,size,_):
  if a==0x3cbd18:uc.mem_write(c.reg(3),bytes([blocked]))
  elif a==callback:
   assert c.reg(0)==ai;assert event!=9 or c.reg(1)==payload;trace.append(['handler',event,payload])
   if mutate:c.pointer(ai+4,owners[1])
   c.returned(accepted)
  elif a==0x3c5684:
   assert c.reg(1)==event and c.reg(2)==payload;trace.append(['fsm',owners.index(c.reg(0)-0x4fc)+1,event,payload]);c.returned()
 c.uc.hook_add(UC_HOOK_CODE,hook)
 for event,locked,blocked,forced,accepted,payload,mutate in itertools.product(range(9,18),(0,1,255),(0,1,255),(0,1,255),(0,1),(0,0xf1234567),(0,1)):
  for p in owners:c.uc.mem_write(p,bytes(0x1800));c.pointer(p+0x378,controller)
  c.pointer(ai,vt);c.pointer(ai+4,owners[0]);c.uc.mem_write(controller+8,bytes([locked,forced]));c.pointer(vt+0x34,callback)
  for i in range(8):c.pointer(vt+0x40+4*i,callback)
  trace.clear();c.invoke(0x3a4d5c,[owners[0],event,payload]);gated=not forced and bool(locked or blocked);expected=[] if gated else [['handler',event,payload]]
  if gated or event==9:expected.append(['fsm',2 if mutate and not gated else 1,event,payload])
  assert trace==expected,(event,locked,blocked,forced,accepted,payload,mutate,trace,expected)
  records.append({'event':event,'locked':locked,'blocked':blocked,'forced':forced,'handler_return':accepted,'payload':payload,'replace_owner':mutate,'trace':trace.copy()})
 vtables=[]
 with engine.open('rb') as f:
  elf=ELFFile(f);syms=list(elf.get_section_by_name('.symtab').iter_symbols())
  for name in ('_ZTV10AISDefault','_ZTV11AISExternal','_ZTV15AISPlayerIPhone'):
   sym=next(x for x in syms if x.name==name);addr=sym['st_value']+8+0x44;segment=next(x for x in elf.iter_segments() if x['p_type']=='PT_LOAD' and x['p_vaddr']<=addr<x['p_vaddr']+x['p_filesz']);f.seek(segment['p_offset']+addr-segment['p_vaddr']);raw=f.read(4);assert struct.unpack('<I',raw)[0]==0x3dbeb0;vtables.append({'symbol':name,'address_point':hex(sym['st_value']+8),'slot':68,'entry_bytes':raw.hex(),'entry_sha256':hashlib.sha256(raw).hexdigest(),'callee':'AISDefault::OnTargetRevived','callee_address':'0x3dbeb0'})
 revived=[]
 for receiver in (0,1,2,0x12345678,0xffffffff,d,d+1,d+0x1000):
  sentinel=bytes(range(256));c.uc.mem_write(d,sentinel);before=bytes(c.uc.mem_read(d,256));actual=c.invoke(0x3dbeb0,[receiver]);assert actual==receiver and bytes(c.uc.mem_read(d,256))==before;revived.append({'receiver':receiver,'retained_r0':actual,'memory_unchanged':True})
 assert bytes(c.uc.mem_read(0x3dbeb0,4))==bytes.fromhex('1eff2fe1')
 report={'validation':'PASS','original_only_routing_cases':len(records),'original_only_empty_endpoint_cases':len(revived),'native_comparisons':0,'mismatches':0,'original_sha256':sha(engine),'manifest_sha256':sha(REF/'original-functions.json'),'script_sha256':sha(Path(__file__)),'scope':__doc__,'revived_body_bytes':'1eff2fe1','revived_body_sha256':hashlib.sha256(bytes.fromhex('1eff2fe1')).hexdigest(),'selected_vtables':vtables,'revived_cases':revived,'routing_cases':records};(REF/'endpoints-original-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('routing_cases','revived_cases')}))
if __name__=='__main__':main()
