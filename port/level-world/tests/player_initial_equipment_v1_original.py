"""Original _InitEquipment276B and nested cached _GetProperty valid branch."""
from __future__ import annotations
import hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'engine-resources/tests'))
from cpu import Cpu
ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
SYMBOLS=['_ZN9Character14_InitEquipmentEv','_ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi','_ZN13ItemInventoryC1Ev','_ZN9Character8InitPostEv']
def provenance(path):
 blob=path.read_bytes();assert hashlib.sha256(blob).hexdigest()==ELF_SHA
 with path.open('rb') as stream:
  elf=ELFFile(stream);sy={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};rows=[]
  def raw(at,n):
   g=next(g for g in elf.iter_segments() if g['p_type']=='PT_LOAD' and g['p_vaddr']<=at and at+n<=g['p_vaddr']+g['p_filesz']);o=g['p_offset']+at-g['p_vaddr'];return blob[o:o+n]
  for name in SYMBOLS:
   s=sy[name];at,n=int(s['st_value']),int(s['st_size']);rows.append({'original_symbol':name,'elf_address':hex(at),'size':n,'sha256':hashlib.sha256(raw(at,n)).hexdigest()})
 return {'original_sha256':ELF_SHA,'functions':rows,'upstream_commit':'791e961b12233100b303038c961666834f4beb9d','upstream_kernel':'port/level-world/player_initial_grants_v2.cpp:dh2_player_initial_equipment_v2','scope':'Whole _InitEquipment caller and real nested _GetProperty valid-id cached sheet read. Online singleton/record, count, AddLoot, IsEquippable, virtual CharacterAutoEquip and Skin bodies are declared callee fixtures. ItemInventory constructor and InitPost are statically pinned, not dynamically executed.'}
def cases():
 # online,recordbyte,initialcount,gold,cachedLoot,newcount,equippableMask
 return [[0,0,0,0,17,0,0],[0,0,0,0,17,1,0],[0,0,0,0,17,1,1],
  [0,0,0,0,17,4,15],[0,0,0,0,17,5,21],[0,0,0,0,-1,0,0],
  [0,0,1,0,17,3,7],[0,0,4,123,17,3,7],[0,0,0,123,17,3,7],
  [1,0,0,0,17,3,7],[1,2,0,0,17,3,7],[1,255,4,123,17,3,7],
  [1,1,0,0,17,4,10],[1,1,1,123,17,3,7],[1,1,0,123,17,3,7],
  [255,1,0,0,0x123456,3,5],[0,0,0,0,2147483647,2,3],
  [0,0,0,0,-2147483648,2,0]]
def capture(path,out):
 manifest=provenance(path);cpu=Cpu(path,False,manifest);records=[];seen=set()
 for inputs in cases():
  online,recordbyte,initialcount,gold,loot,newcount,mask=inputs;cpu.seen.clear()
  char=cpu.data+0x1000;inventory=char+0x37c;system=cpu.data+0x7000;record=cpu.data+0x8000;vtable=cpu.data+0x9000
  cpu.uc.mem_write(char,bytes(0x10000));cpu.pointer(char,vtable);cpu.pointer(vtable+0x130,0x3a9fa8);cpu.pointer(char+0x39c,gold)
  cpu.pointer(char+0xff4+4+9*4,loot&0xffffffff);cpu.uc.mem_write(system+5,bytes([online]));cpu.uc.mem_write(record+0x66c,bytes([recordbyte]))
  # The actual offset array is read by the nested original getter, with its
  # source schema field9 offset36. Base/saved/gear differ and are not read.
  offsets=cpu.symbols['_ZN7Structs19CharacterProperties13m_dataOffsetsE'];cpu.pointer(offsets+9*4,9*4)
  cpu.pointer(char+0x560+8+4+9*4,777);cpu.pointer(char+0x560+0x38c+4+9*4,888);cpu.pointer(char+0x560+0x710+4+9*4,999)
  trace=[];current=initialcount;equipped=[];getter_reads=0
  def done(value=0):cpu.put(0,value&0xffffffff);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
  def hook(uc,address,size,unused):
   nonlocal current,getter_reads
   if address==0x7fd794:trace.append([0,0,online]);done(system)
   elif address==0x36eea8:assert cpu.reg(1)==char and cpu.reg(2)==0;trace.append([1,0,recordbyte]);done(record)
   elif address==0x3fc608:assert cpu.reg(0)==inventory;trace.append([2,0,current]);done(current)
   elif address==0x3dedb4:assert [cpu.reg(i) for i in range(3)]==[char+0x560,char+0xff4,9];getter_reads+=1;trace.append([4,9,loot&0xffffffff]) # Real body executes.
   elif address==0x3dfe60:raise AssertionError('InitEquipment unexpectedly recalculated a cached property')
   elif address==0x40407c:
    assert [cpu.reg(i) for i in range(4)]==[inventory,loot&0xffffffff,0,0];stack=cpu.uc.reg_read(cpu.sp);assert struct.unpack('<II',cpu.uc.mem_read(stack,8))==(0xffffffff,0);trace.append([5,loot&0xffffffff,0]);current=newcount;done()
   elif address==0x3fdeec:
    i=cpu.reg(1);assert cpu.reg(0)==inventory and i<current;value=(mask>>i)&1;trace.append([6,i,value]);done(value)
   elif address==0x3a9fa8:
    i=cpu.reg(1);assert cpu.reg(0)==char;equipped.append(i);trace.append([7,i,0]);done(i&1) # Ignored actual virtual result.
   elif address==0x3a999c:assert cpu.reg(0)==char;trace.append([8,0,0]);done()
  handle=cpu.uc.hook_add(UC_HOOK_CODE,hook);cpu.invoke('_ZN9Character14_InitEquipmentEv',[char]);cpu.uc.hook_del(handle);seen|=cpu.seen
  if online and recordbyte!=1:decision=1
  elif initialcount:decision=2
  elif gold:decision=3
  elif not newcount:decision=4
  else:decision=5
  outputs=[decision,1,int(bool(online)),sum(t[0]==2 for t in trace),int(decision in (3,4,5)),getter_reads,sum(t[0]==5 for t in trace),sum(t[0]==6 for t in trace),len(equipped),sum(t[0]==8 for t in trace),newcount if decision==5 else 0,newcount-1 if decision==5 else 0,loot&0xffffffff if getter_reads else 0xffffffff]
  backend=[t for t in trace if t[0] in (0,1,5)]
  records.append({'inputs':inputs,'output':outputs,'source_trace':trace,'backend_trace':backend,'auto_equip_order':equipped})
 assert not cpu.import_calls
 result={'validation':'PASS','original_sha256':ELF_SHA,'cases':records,'distinct_pinned_words':len(seen),'import_calls':cpu.import_calls,'scope':manifest['scope']}
 out.mkdir(parents=True,exist_ok=True);(out/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n');(out/'original-capture.json').write_text(json.dumps(result,indent=2)+'\n')
 blob=struct.pack('<II',0x31455149,len(records))
 for row in records:
  words=[*(x&0xffffffff for x in row['inputs']),*row['output'],len(row['backend_trace'])];blob+=struct.pack('<'+'I'*len(words),*words)
  for t in row['backend_trace']:blob+=struct.pack('<III',*t)
 (out/'original-cases.bin').write_bytes(blob);return result
if __name__=='__main__':
 import argparse
 p=argparse.ArgumentParser();p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();r=capture(a.original_elf.resolve(),a.output.resolve());print(json.dumps({'validation':r['validation'],'cases':len(r['cases']),'distinct_pinned_words':r['distinct_pinned_words']}))
