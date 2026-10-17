"""Original Character event33/34 routing and RegenTick instruction capture.

Debug/string/cached property/regen/combat calls are explicit service fixtures.
Character RaiseEvent, AI dispatch, remote/aggro/state leaves, GetInCombat,
_UpdateRegen, RegenTick and HandleDots execute their real ARM instructions.
"""
from __future__ import annotations
import hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'engine-resources/tests'))
from cpu import Cpu
ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
NAMES=['_ZN9Character10RaiseEventEiPv','_ZN6CharAI12RaiseAIEventEiPv','_ZN6CharAI12_UpdateRegenEv',
 '_ZNK10ObjectBase17IsRemotelyUpdatedEv','_ZNK6CharAI13AI_IsInCombatEv','_ZNK6CharAI11AI_HasAggroEv','_ZNK6CharAI12AI_IsAggroedEv',
 '_ZNK16CharStateMachine14SM_IsAttackingEv','_ZNK16CharStateMachine15SM_IsUsingSkillEv','_ZNK16CharStateMachine12SM_IsCastingEv','_ZNK16CharStateMachine11SM_GetStateEv',
 '_ZN9Character9RegenTickEb','_ZN14CharProperties10HandleDotsEv','_ZN10AISDefault8OnUpdateEv','_ZN10ObjectBaseC1ENS_6GO_IDSE','_ZN10ObjectBaseC2ENS_6GO_IDSE']
def provenance(path):
 assert hashlib.sha256(path.read_bytes()).hexdigest()==ELF_SHA
 blob=path.read_bytes()
 with path.open('rb') as stream:
  elf=ELFFile(stream);sy={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};rows=[]
  def raw(at,n):
   seg=next(g for g in elf.iter_segments() if g['p_type']=='PT_LOAD' and g['p_vaddr']<=at and at+n<=g['p_vaddr']+g['p_filesz']);off=seg['p_offset']+at-seg['p_vaddr'];return blob[off:off+n]
  for name in NAMES:
   s=sy[name];at,n=int(s['st_value']),int(s['st_size']);rows.append({'original_symbol':name,'elf_address':hex(at),'size':n,'sha256':hashlib.sha256(raw(at,n)).hexdigest()})
  tables=[]
  for name in ['_ZTV9Character','_ZTV9AISPlayer','_ZTV15AISPlayerIPhone']:
   s=sy[name];at,n=int(s['st_value']),int(s['st_size']);r=raw(at,n);slot=0x54 if name=='_ZTV9Character' else 0x18
   target=struct.unpack_from('<I',r,slot+8)[0];assert target==(0x33dd10 if slot==0x54 else 0x3dc798)
   tables.append({'symbol':name,'address':hex(at),'sha256':hashlib.sha256(r).hexdigest(),'slot':hex(slot),'target':hex(target)})
 return {'original_sha256':ELF_SHA,'functions':rows,'vtables':tables,'new_source_callers':['Character::RegenTick(bool)','CharAI::_UpdateRegen','CharAI::AI_IsInCombat'], 'query_leaf_body_credit':0,'attribution':{'upstream_commit':'791e961b12233100b303038c961666834f4beb9d','reuse':'Existing maintained CharAI event route, remote getter, HandleDots, regeneration/Debug/property/DoT attack kernels; aggro count/state leaves use actual typed fields; no Adam private VM/timer owner imported'}}
def cases():
 rows=[]
 for combat_state in [-1,0,3,5,6,7,8]:rows.append([0x33,0xffffffff,0,0,0,combat_state,0,256,512,128,384,0])
 for a,b in [(1,0),(0,1),(0xffffffff,0),(0,0xffffffff)]:rows.append([0x33,0xffffffff,0,a,b,3,0,257,513,129,385,1])
 for remote,byte in [(0xffffffff,1),(0xffffffff,255),(0,0),(0xfffffffe,0)]:rows.append([0x33,remote,byte,1,1,5,0,1,2,3,4,0])
 for amount in [0,0xffffffff,0x80000000,1,256]:
  for dead in [0,7]:rows.append([0x34,0xffffffff,0,0,0,3,dead,amount,0,0,0,0])
 return rows
def capture(path,out):
 manifest=provenance(path);all_seen=set();rows=[];cpu=Cpu(path,False,manifest)
 for inputs in cases():
  event,remote,byte,aggro,aggroed,state_id,dead,hp39,hp40,mp44,mp45,mutation=inputs
  cpu.seen.clear();char=cpu.data+0x1000;cpu.uc.mem_write(char,bytes(0x3000));ai=char+0x3c8;props=char+0x560;sheet=char+0xff4;sm_state=cpu.data+0x5000
  cpu.pointer(char,cpu.symbols['_ZTV9Character']+8);cpu.pointer(char+0x110,remote);cpu.uc.mem_write(char+0x118,bytes([byte]))
  cpu.pointer(ai+4,char);cpu.pointer(props+4,char);cpu.pointer(ai+0x8c,aggro);cpu.pointer(ai+0xa4,aggroed);cpu.pointer(char+0x4fc+0x20,sm_state);cpu.pointer(sm_state,state_id&0xffffffff)
  values={39:hp39,40:hp40,44:mp44,45:mp45,**{p:hp39 for p in range(126,132)}};trace=[];buff=[]
  def done(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
  def hook(uc,address,size,unused):
   if address==0x337888:trace.append([0,0,0]);done(99)
   elif address==0x3140ec:
    text=bytes(cpu.uc.mem_read(cpu.reg(1),32)).split(b'\0')[0];assert text==b'isTracingChar_Stats';trace.append([1,0,0]);done(cpu.reg(0))
   elif address==0x337a88:trace.append([2,0,0]);done(123)
   elif address==0x318254:trace.append([3,0,0]);done()
   elif address==0x3dedb4:
    assert cpu.reg(0)==props and cpu.reg(1)==sheet
    p=cpu.reg(2);trace.append([4,p,values[p]]);done(values[p])
   elif address in [0x3bdca4,0x3bdbb8]:
    assert cpu.reg(0)==char;op=5 if address==0x3bdca4 else 6;trace.append([op,0,cpu.reg(1)])
    if op==5 and mutation:
     values[44]=777;values[45]=778
    done(0xaaaa)
   elif address==0x3a2ed4:
    assert cpu.reg(0)==char;done(dead)
   elif address==0x3e123c:buff.append(cpu.reg(1));done()
  # Dot symbol spelling differs among retained symbol aliases: identify it.
  dot=next(v for k,v in cpu.symbols.items() if k.startswith('_ZN9Character11F_DotAttack'))
  def checked_hook(uc,address,size,unused):
   if address==dot:
    trace.append([7,cpu.reg(3),struct.unpack('<I',cpu.uc.mem_read(cpu.uc.reg_read(cpu.sp),4))[0]]);done();return
   # Exclude the unavailable mangled lookup in the generic hook.
   if address==0x3b10b4:trace.append([8,0,0]);done();return
   if address in [0x337888,0x3140ec,0x337a88,0x318254,0x3dedb4,0x3bdca4,0x3bdbb8,0x3a2ed4,0x3e123c]:hook(uc,address,size,unused)
  handler=cpu.uc.hook_add(UC_HOOK_CODE,checked_hook)
  try:cpu.invoke(0x3a4d5c,[char,event,cpu.data+0x7000])
  except Exception as error:raise RuntimeError(f'Original route inputs={inputs}, pc={hex(cpu.uc.reg_read(cpu.pc))}, trace={trace}') from error
  cpu.uc.hook_del(handler);assert not cpu.import_calls;all_seen.update(cpu.seen)
  skip=int(event==0x33 and (remote!=0xffffffff or byte!=0));combat=int(bool(aggro or aggroed or state_id in [5,6,7]))
  if event==0x33:assert len(trace)==(0 if skip else 8)
  else:assert sum(t[0]==4 for t in trace)==6
  rows.append({'inputs':inputs,'regen_skipped':skip,'in_combat':combat,'trace':trace})
 # Direct buff36 returns from Character RaiseEvent, and never enters CharAI.
 cpu=Cpu(path,False,manifest);buff_calls=[]
 def buff_hook(uc,address,size,unused):
  if address==0x3e123c:buff_calls.append([cpu.reg(0),cpu.reg(1)]);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 cpu.uc.hook_add(UC_HOOK_CODE,buff_hook);cpu.invoke(0x3a4d5c,[cpu.data+0x1000,0x36,cpu.data+0x7000]);assert buff_calls==[[cpu.data+0x1560,cpu.data+0x7000]]
 report={'validation':'PASS','original_sha256':ELF_SHA,'cases':rows,'direct_buff36':True,'distinct_pinned_words':len(all_seen),'import_calls':{},'scope':'Actual Character RaiseEvent/CharAI event special routes, _UpdateRegen, remote/aggro/state leaves, GetInCombat, RegenTick and HandleDots. Debug/string/cached-sheet/HP-MP regeneration/combat applications are declared service fixtures. No frame or AIS update is invoked by these timer events.'}
 out.mkdir(parents=True,exist_ok=True);(out/'original-capture.json').write_text(json.dumps(report,indent=2)+'\n');(out/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n')
 with (out/'original-cases.bin').open('wb') as f:
  f.write(struct.pack('<II',0x31544950,len(rows)))
  for row in rows:
   f.write(struct.pack('<15I',*(v&0xffffffff for v in [*row['inputs'],row['regen_skipped'],row['in_combat'],len(row['trace'])])))
   for trace in row['trace']:f.write(struct.pack('<III',*trace))
 return report
if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser();p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();r=capture(a.original_elf,a.output);print(json.dumps({k:v for k,v in r.items() if k!='cases'}))
