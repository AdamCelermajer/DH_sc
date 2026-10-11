"""Pinned original OnDied/AI_SetDead/SM selection, with explicit callee fixtures."""
from __future__ import annotations
import hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'engine-resources/tests'))
from cpu import Cpu
ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
NAMES=['_ZN6CharAI6OnDiedEP10GameObject','_ZN6CharAI10AI_SetDeadEv','_ZN10AISDefault6OnDiedEP10GameObject',
 '_ZN6CharAI12AI_SetTargetEP10GameObjectb','_ZN6CharAI17AI_SyncLastTargetEv','_ZN16CharStateMachine15SM_SetDeadStateEbPvb',
 '_ZNK9Character18GetCharAnimTableIdEv','_ZNK16CharStateMachine21SM_IsAwaitingToReviveEv','_ZNK16CharStateMachine11SM_GetStateEv',
 '_ZN10CharTimers8TMR_StopEj','_ZN6CharAI16AI_ClearAllAggroEv','_ZN6CharAI24AI_ClearAllAggroTowardMeEb',
 '_ZN6CharAI13_SkillCleanUpEv','_ZN6CharAI13_SpellCleanUpEv','_ZN9CharacterC1EN10ObjectBase6GO_IDSE','_ZN9CharacterC2EN10ObjectBase6GO_IDSE']
def provenance(path):
 blob=path.read_bytes();assert hashlib.sha256(blob).hexdigest()==ELF_SHA
 with path.open('rb') as stream:
  elf=ELFFile(stream);sy={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};rows=[]
  def raw(at,n):
   g=next(g for g in elf.iter_segments() if g['p_type']=='PT_LOAD' and g['p_vaddr']<=at and at+n<=g['p_vaddr']+g['p_filesz']);o=g['p_offset']+at-g['p_vaddr'];return blob[o:o+n]
  for name in NAMES:
   s=sy[name];at,n=int(s['st_value']),int(s['st_size']);rows.append({'original_symbol':name,'elf_address':hex(at),'size':n,'sha256':hashlib.sha256(raw(at,n)).hexdigest()})
  tables=[]
  for name in ['_ZTV9AISPlayer','_ZTV15AISPlayerIPhone']:
   s=sy[name];at,n=int(s['st_value']),int(s['st_size']);word=struct.unpack_from('<I',raw(at,n),8+0x24)[0];assert word==0x3dbe90
   tables.append({'symbol':name,'address':hex(at),'size':n,'sha256':hashlib.sha256(raw(at,n)).hexdigest(),'died_slot':'0x24','target':hex(word)})
 return {'original_sha256':ELF_SHA,'functions':rows,'vtables':tables,'scope':'Whole OnDied and AI_SetDead callers and all SM_SetDeadState forced/false branches; existing target/sync/timer/aggro and skill-vector callers execute. Group, Debug/string, design mask/stance, state transition, STL storage, OnDeAggro and individual OnSkillCleanUp are declared callee fixtures. Character constructors are statically pinned for marker14d0 proof, not dynamically executed.'}
def cases():
 # group,active,target,trace,great,row,count,mask,stance,prior,mutate_great,out,in
 rows=[[0,1,1,0,0,2,20,0,5,s,0,0,0] for s in [-1,0,1,3,4,5,12,16,17]]
 rows += [[0,1,0,0,1,2,20,0,5,3,0,0,0],
  [1,1,1,1,1,2,20,0x78000,5,3,0,0,0],
  [1,0,1,0,0,2,20,0x78000,5,3,0,0,0],
  [0,0,1,0,0,2,20,0x8000,5,3,0,0,0],
  [0,1,1,0,1,2,20,0x78000,5,3,1,0,0],
  [0,1,1,0,0,-1,18,0x10000,5,3,0,0,0],
  [0,1,1,0,0,-1,17,0,5,3,0,0,0],
  [0,1,1,0,0,0,0,0,5,3,0,0,0],
  [0,1,1,0,0,40,20,0,5,3,0,0,0],
  [0,1,1,0,0,2,20,0,5,3,0,1,0],
  [0,1,1,0,0,2,20,0,5,3,0,0,1],
  [1,1,1,0,0,2,20,0,5,3,0,1,1]]
 return rows
def capture(path,out):
 manifest=provenance(path);cpu=Cpu(path,False,manifest);rows=[];seen=set()
 for inputs in cases():
  group,active,target,tracing,great,row,count,mask,stance,prior,mutation,out_count,in_count=inputs
  cpu.seen.clear();char=cpu.data+0x1000;ai=char+0x3c8;sm=char+0x4fc;ais=cpu.data+0x6000;state=cpu.data+0x7000;peer=cpu.data+0x9000
  anim=cpu.data+0x10000;slots=cpu.data+0x13000;out_node=cpu.data+0x14000;in_node=cpu.data+0x15000;heap=cpu.data+0x18000
  cpu.uc.mem_write(char,bytes(0x19000));cpu.pointer(ai, cpu.symbols['_ZTV6CharAI']+8);cpu.pointer(ai+4,char)
  cpu.pointer(ais,cpu.symbols['_ZTV15AISPlayerIPhone']+8);cpu.pointer(ai+0x1c,ais if active else 0);cpu.pointer(ai+0x34,cpu.data+0x8000 if group else 0)
  cpu.pointer(ai+0x40,peer if target else 0);cpu.pointer(ai+0x44,peer);cpu.uc.mem_write(ai+0x48,bytes([3,4,0,0,1]));cpu.uc.mem_write(char+0x14d0,struct.pack('<H',0x1234))
  cpu.pointer(sm+4,char);cpu.pointer(sm+0x20,state);cpu.pointer(state,prior&0xffffffff);cpu.pointer(sm+0x28,0xffffffff);cpu.pointer(sm+0x38,0xffffffff)
  cpu.uc.mem_write(sm+0x3e,bytes([123,great]));cpu.pointer(char+0x1000,row&0xffffffff)
  cpu.pointer(cpu.symbols['_ZN6Arrays13CharAnimTable4sizeE'],count);cpu.pointer(cpu.symbols['_ZN6Arrays13CharAnimTable7membersE'],anim)
  for index in range(20):
   for offset,value in [(0x10,20),(0x14,30),(0x18,40),(0x1c,10)]:cpu.pointer(anim+index*0xa0+offset,value)
  cpu.pointer(char+0x3b4+8,slots);cpu.pointer(char+0x3b4+12,slots+3*32)
  for index in range(3):cpu.uc.mem_write(slots+index*32+0x14,b'\x01')
  cpu.pointer(ai+0x10,0);cpu.pointer(ai+0x14,1)
  for header,node,number in [(ai+0x7c,out_node,out_count),(ai+0x94,in_node,in_count)]:
   cpu.pointer(header+4,node if number else 0);cpu.pointer(header+8,node if number else header);cpu.pointer(header+12,node if number else header);cpu.pointer(header+16,number)
   if number:
    cpu.pointer(node+4,header);cpu.pointer(node+8,0);cpu.pointer(node+12,0);cpu.pointer(node+16,peer)
  cpu.pointer(peer+0x3c8,cpu.symbols['_ZTV6CharAI']+8);cpu.pointer(peer+0x3c8+4,peer)
  trace=[];keys={};direct=[];direction=0;mask_calls=0;lease_end=heap
  def word(a):return struct.unpack('<I',cpu.uc.mem_read(a,4))[0]
  def done(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
  def text(a):return bytes(cpu.uc.mem_read(a,64)).split(b'\0')[0].decode()
  def hook(uc,address,size,unused):
   nonlocal direction,mask_calls,lease_end
   if address==0x3d2628:trace.append([0,0,0]);assert cpu.reg(1)==char;done()
   elif address==0x3dbe90:trace.append([17,0,0]) # Actual inherited bx lr executes.
   elif address==0x337888:trace.append([101,0,0]);done()
   elif address==0x3140ec:keys[cpu.reg(0)]=text(cpu.reg(1));done(cpu.reg(0))
   elif address==0x337a88:
    key=1 if keys[cpu.reg(1)]=='IsTracingCharAITarget' else 2;trace.append([102,key,0]);done(tracing if key==1 else 0)
   elif address in [0x318254,0x310440,0x708f00]:done()
   elif address==0x3a3228:trace.append([2,row if 0<=row<count else 17,count]) # Actual 60-byte getter executes.
   elif address in [0x3c5a2c,0x3c5948,0x3c59f8,0x3c5984]:
    field,offset={0x3c5a2c:(0,0x1c),0x3c5948:(1,0x10),0x3c59f8:(2,0x14),0x3c5984:(3,0x18)}[address];trace.append([3,field,word(cpu.reg(8)+offset)])
   elif address==0x4c4bdc:
    assert text(cpu.reg(1))=='AnimStancedAnim' and text(cpu.reg(2))=='SL__LIST_IPHONE';trace.append([4,0,mask]);mask_calls+=1
    if mutation and mask_calls==1:cpu.uc.mem_write(sm+0x3f,b'\x00')
    done(mask)
   elif address==0x3a53e0:trace.append([5,0,stance]);done(stance)
   elif address==0x3c1938:
    assert [cpu.reg(i) for i in range(1,4)]==[12,50008,0];direct.extend([word(sm+0x28),word(sm+0x38)]);trace.append([16,*direct]);cpu.pointer(state,12);done()
   elif address==0x3db2d8:trace.append([18,cpu.reg(1),0]) # Actual source stop body executes.
   elif address==0x3d5fa8:direction=0;trace.append([6,0,word(ai+0x8c)])
   elif address==0x3d6abc:direction=1;assert cpu.reg(1)==0;trace.append([6,1,word(ai+0xa4)])
   elif address==0x3d5e14:
    if cpu.reg(1):
     vector=cpu.reg(0);cpu.pointer(vector,lease_end);cpu.pointer(vector+4,lease_end);cpu.pointer(vector+8,lease_end+cpu.reg(1)*4);lease_end+=0x100
    done()
   elif address==0x3cd34c:trace.append([10,direction,0]);done()
   elif address==0x3d2014:
    receiver=cpu.reg(0);other=cpu.reg(1);assert (receiver,other)==((peer+0x3c8,char) if direction==0 else (ai,peer));trace.append([11,direction,2]);done()
   elif address==0x3d8ae0:trace.append([13,0,0]) # Real empty source vector loop.
   elif address==0x3d8a98:trace.append([14,0,0])
  handle=cpu.uc.hook_add(UC_HOOK_CODE,hook)
  cpu.invoke('_ZN6CharAI6OnDiedEP10GameObject',[ai,peer]);cpu.uc.hook_del(handle);seen|=cpu.seen
  output=[word(state),direct[0] if direct else 0xffffffff,word(sm+0x38),*bytes(cpu.uc.mem_read(sm+0x3e,2)),
   word(ai+0x3c),word(ai+0x40),word(ai+0x44),*bytes(cpu.uc.mem_read(ai+0x48,2)),bytes(cpu.uc.mem_read(ai+0x4c,1))[0],
   struct.unpack('<H',cpu.uc.mem_read(char+0x14d0,2))[0],word(ai+0x10),word(ai+0x14),
   *(bytes(cpu.uc.mem_read(slots+i*32+0x14,1))[0] for i in range(3)),word(ai+0x8c),word(ai+0xa4)]
  rows.append({'inputs':inputs,'output':output,'trace':trace})
 result={'validation':'PASS','original_sha256':ELF_SHA,'cases':rows,'distinct_pinned_words':len(seen),'import_calls':cpu.import_calls,'scope':manifest['scope']}
 assert not cpu.import_calls
 out.mkdir(parents=True,exist_ok=True);(out/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n');(out/'original-capture.json').write_text(json.dumps(result,indent=2)+'\n')
 binary=struct.pack('<II',0x31444950,len(rows))
 for row in rows:
  values=[*(x&0xffffffff for x in row['inputs']),*row['output'],len(row['trace'])];binary+=struct.pack('<'+'I'*len(values),*values)
  for entry in row['trace']:binary+=struct.pack('<III',*(x&0xffffffff for x in entry))
 (out/'original-cases.bin').write_bytes(binary);return result
if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser();p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();r=capture(a.original_elf.resolve(),a.output.resolve());print(json.dumps({'validation':r['validation'],'cases':len(r['cases']),'distinct_pinned_words':r['distinct_pinned_words']}))
