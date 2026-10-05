"""Whole relationship branches/order against original instructions; explicit synchronous dependency fixtures."""
from pathlib import Path
import sys,struct,json,hashlib,itertools
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
class NativeCpu(Cpu):
 def external(self,uc,address,size,unused):
  if address==self.callback+32:native_hook(uc,address,size,unused)
  else:super().external(uc,address,size,unused)
old=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
new=NativeCpu(root/'.local-inputs/libcharacter_world_ai_v1_oracle.so',True,{'functions':[]})
word=lambda at:struct.unpack('<I',old.uc.mem_read(at,4))[0]
oc,tc,ai,vt,rows,entries=[old.data+x for x in (0x1000,0x3000,0x5000,0x6000,0x7000,0x8000)]
nc,nt=0x100000001,0x200000002
state,out,nrows,nentries,target_slot,service=[new.data+x for x in (0x1000,0x1100,0x2000,0x3000,0x4000,0x5000)]
for pc,literal,countlit,tablelit in ((0x3d5138,0x3d53a8,0x3d53ac,0x3d53b4),(0x3d5768,0x3d5a54,0x3d5a58,0x3d5a60)):
 got=(pc+word(literal))&0xffffffff
 countptr=old.data+0xa000;tableptr=countptr+4
 old.pointer((got+word(countlit))&0xffffffff,countptr);old.pointer((got+word(tablelit))&0xffffffff,tableptr)
 old.uc.mem_write(countptr,struct.pack('<I',2));old.pointer(tableptr,rows)
for i in range(2):
 old.uc.mem_write(rows+i*12,struct.pack('<III',0,2,entries+i*24))
 old.uc.mem_write(entries+i*24,b''.join(struct.pack('<Iii',0,j,1 if i==j else -1) for j in range(2)))
 new.uc.mem_write(nrows+i*16,struct.pack('<QII',nentries+i*16,2,0))
 new.uc.mem_write(nentries+i*16,b''.join(struct.pack('<ii',j,1 if i==j else -1) for j in range(2)))
old.pointer(oc,vt);old.pointer(tc,vt);old.pointer(ai+4,oc)
old.pointer(vt+0x28,0x3a49f0);old.pointer(vt+0x88,0x3a5000);old.pointer(vt+0x90,0x3a5010)
new.uc.mem_write(service,struct.pack('<QQ',0,new.callback+32))
facts={};ot=[];ntt=[]
def ret(cpu,value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
def old_hook(uc,address,size,unused):
 if address==0x33dd70:
  ot.append((1,tc));old.uc.mem_write(old.reg(0),struct.pack('<III',7,tc,0));ret(old,old.reg(0))
 elif address in (0x33ff8c,0x33fdc0):ot.append((2,tc));ret(old,tc if facts['resolved'] else 0)
 elif address==0x3a3180:
  subject=old.reg(0);ot.append((4,subject));ret(old,facts['owner_faction' if subject==oc else 'target_faction'])
 elif address==0x3a49f0:
  subject=old.reg(0);ot.append((5,subject));ret(old,facts['owner_player' if subject==oc else 'target_player'])
 elif address==0x3a5000:ot.append((6,tc));ret(old,facts['interactive'])
 elif address==0x3a5010:ot.append((7,tc));ret(old,facts['interaction_type'])
def native_hook(uc,address,size,unused):
 if address!=new.callback+32:return
 q=new.reg(1);response=new.reg(2);op,arg,subject,other,handle=struct.unpack('<IIQQQ',new.uc.mem_read(q,32));new.uc.mem_write(response,bytes(32))
 mapped=oc if subject==nc else tc
 if op!=3:ntt.append((op,mapped))
 if op==1:new.uc.mem_write(response+16,struct.pack('<iIQ',7,0,nt))
 elif op==2:new.uc.mem_write(response,struct.pack('<Q',nt if facts['resolved'] else 0))
 elif op==3:new.uc.mem_write(response+8,struct.pack('<i',facts['kind']))
 elif op==4:new.uc.mem_write(response+8,struct.pack('<i',facts['owner_faction' if subject==nc else 'target_faction']))
 elif op==5:new.uc.mem_write(response+8,struct.pack('<i',facts['owner_player' if subject==nc else 'target_player']))
 elif op==6:new.uc.mem_write(response+8,struct.pack('<i',facts['interactive']))
 elif op==7:new.uc.mem_write(response+8,struct.pack('<i',facts['interaction_type']))
 else:raise AssertionError(op)
 ret(new)
old.uc.hook_add(UC_HOOK_CODE,old_hook)
records=[]
for enemy,resolved,kind,of,tf,op,tp,interactive,itype,null,stored in itertools.product((0,1),(0,1),(0,1),(0,1),(0,1),(0,1),(0,1),(0,1),(3,8),(0,1),(0,1)):
 facts.update(resolved=resolved,kind=kind,owner_faction=of,target_faction=tf,owner_player=op,target_player=tp,interactive=interactive,interaction_type=itype)
 old.uc.mem_write(tc+0xf4,struct.pack('<I',kind));old.pointer(ai+0x40,tc if stored else 0)
 new.uc.mem_write(target_slot,struct.pack('<Q',nt if stored else 0));new.uc.mem_write(state,struct.pack('<QQQIIQ',nc,target_slot,nrows,2,0,0))
 ot.clear();ntt.clear();original=old.invoke(0x3d574c if enemy else 0x3d511c,[ai,0 if null else tc]);status=new.invoke('dh2_world_ai_relationship_v1',[out,state,enemy,0 if null else nt,service]);result=struct.unpack('<4I',new.uc.mem_read(out,16))
 assert status==0 and original==result[0],(facts,enemy,null,stored,original,status,result)
 assert ot==ntt,(facts,enemy,null,stored,ot,ntt)
 records.append([enemy,resolved,kind,of,tf,op,tp,interactive,itype,null,stored,original,len(ot)])
report={'validation':'PASS','cases':len(records),'mismatches':0,'whole_original_relationship_instruction_branches':True,'ordered_handle_faction_virtual_dependencies_compared':True,'dependency_scope':'Explicit GetHandle/GetObject/GetFaction and virtual player/interactive fixtures; registry tree/assertion backends remain separate. Valid faction domain only.','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'native_sha256':hashlib.sha256((root/'.local-inputs/libcharacter_world_ai_v1_oracle.so').read_bytes()).hexdigest()}
(root/'port/level-world/reports/character-world-ai-relationship-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
