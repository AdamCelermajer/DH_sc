"""Read-only original native FSM getter, authored preset and Update-order probes; virtual/set-state/clock services explicit."""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu as Base,string
class Cpu(Base):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='strlen':self.put(0,len(string(self,self.reg(0))));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if self.imports.get(address)=='memcmp':
   a,b,n=[self.reg(i) for i in range(3)];aa,bb=bytes(uc.mem_read(a,n)),bytes(uc.mem_read(b,n));self.put(0,(int(aa>bb)-int(aa<bb))&0xffffffff);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
c=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});char=c.data+4096;machine=char+0x4fc;info=c.data+0x10000;behavior=c.data+0x11000;vt=c.data+0x12000;out=c.data+0x13000;text=c.data+0x14000;virtual=c.data+0x1e000;trace=[];dt=0;stun_result=-1;scare_result=-1
def w(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def ret(value=0):c.put(0,value&0xffffffff);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def current():return w(w(machine+0x20)) if w(machine+0x20) else 0xffffffff
def hook(uc,address,size,unused):
 if address==0x37cb24:trace.append(dict(service='pushInteger',signed=struct.unpack('<i',struct.pack('<I',c.reg(1)))[0],raw=c.reg(1)));ret()
 elif address in [0x3136b4,0x3136b8]:trace.append(dict(service='profile_begin' if address==0x3136b4 else 'profile_end'));ret()
 elif address==0x31f66c:trace.append(dict(service='raw_engine_dt',elapsed_before=w(machine+0x60),result=dt));ret(dt)
 elif address in [0x3c5ffc,0x3c6144]:
  trace.append(dict(service='set_stun' if address==0x3c5ffc else 'set_scare',duration=c.reg(1),mode=c.reg(2),payload=c.reg(3),force=w(c.uc.reg_read(c.sp)),elapsed=w(machine+0x60),current=current()))
  result=stun_result if address==0x3c5ffc else scare_result
  if result>=0:c.pointer(info,result);c.pointer(machine+0x20,info)
  ret()
 elif address==virtual:trace.append(dict(service='actual_current_virtual_update',id=c.reg(1),character=c.reg(2)==char,machine=c.reg(3)==machine,elapsed=w(machine+0x60)));ret()
c.uc.hook_add(UC_HOOK_CODE,hook);c.uc.mem_write(virtual,struct.pack('<I',0xe12fff1e));c.pointer(behavior,vt);c.pointer(vt+0x14,virtual);c.pointer(info+4,behavior)
getters=[]
for state,elapsed in itertools.product([-1,0,3,4,5,8,9,12,17,0x7fffffff],[0,1,500,0x7fffffff,0x80000000,0xffffffff]):
 c.pointer(machine+0x20,0 if state==-1 else info);c.pointer(info,state&0xffffffff);c.pointer(char+0x55c,elapsed);trace.clear();c.invoke(0x3b6d78,[0,out,char]);c.invoke(0x3b6d6c,[0,out,char]);assert len(trace)==2 and trace[0]['raw']==state&0xffffffff and trace[1]['raw']==elapsed;getters.append(dict(state=state,elapsed_bits=elapsed,trace=trace.copy()))
updates=[]
for state,flags,policy,elapsed,dt,stun_result,scare_result in itertools.product([-1,3,8,9],[0,2,4,6],[0,0x400,0x800,0xc00],[0,0xffffffff],[0,16,0xffffffff],[-1,9],[-1,8]):
 c.pointer(machine+4,char);c.pointer(machine+0x20,0 if state==-1 else info);c.pointer(info,state&0xffffffff);c.pointer(machine+0x60,elapsed);c.pointer(machine+0x2c,flags);c.pointer(machine+0x24,policy);trace.clear();c.invoke(0x3c628c,[machine]);assert w(machine+0x60)==(elapsed+dt)&0xffffffff
 assert trace[0]['service']=='profile_begin' and trace[1]['service']=='raw_engine_dt' and trace[-1]['service']=='profile_end'
 assert all(e['duration']==0xffffffff and e['payload']==e['force']==0 and e['mode']==bool(policy&(0x800 if e['service']=='set_stun' else 0x400)) for e in trace if e['service'] in ['set_stun','set_scare'])
 calls=[e for e in trace if e['service']=='actual_current_virtual_update'];assert len(calls)==int(bool(w(machine+0x20))) and all(e['id']==current() and e['character'] and e['machine'] for e in calls)
 updates.append(dict(state=state,mask=flags,policy=policy,elapsed=elapsed,dt=dt,stun_result=stun_result,scare_result=scare_result,trace=trace.copy(),final_state=current(),final_elapsed=w(machine+0x60)))
preset=[]
for key in [b'',b'Limbus',b'PreSpawn',b'Idle',b'limbus',b'PreSpawnX']:
 c.uc.mem_write(text,key+b'\0');c.pointer(char+0x13dc,text+len(key));c.pointer(char+0x13e0,text)
 # Original owned string at+13cc starts with storage bookkeeping, with its
 # end/begin at+10/+14. Imported C string primitives are explicit services.
 expected=0 if key==b'Limbus' else 17 if key==b'PreSpawn' else 3
 # The actual std::string.compare body executes; its libc memcmp is modeled.
 value=c.invoke(0x3a5784,[char]);assert value==expected,(key,value);preset.append(dict(name=key.decode(),state=value))
table_address=(w(0x3c74a4)+0x3c73cc+8)&0xffffffff;raw=bytes(c.uc.mem_read(table_address,160));factory=[]
for i in range(20):
 kind,function=struct.unpack_from('<iI',raw,i*8);factory.append(dict(index=i,state=kind,function=hex(function),symbols=[name for name,address in c.symbols.items() if address==function and name.startswith('_Z11GetNewState')]))
table=dict(original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),table_address=hex(table_address),table_bytes_sha256=hashlib.sha256(raw).hexdigest(),rows=factory);(HERE/'state-factory-table.json').write_text(json.dumps(table,indent=2)+'\n')
report=dict(validation='PASS',scope=__doc__,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),original_instructions_executed=True,probe_sha256=sha(Path(__file__)),manifest_sha256=sha(HERE/'original-functions.json'),factory_table_sha256=sha(HERE/'state-factory-table.json'),getter_cases=len(getters),update_cases=len(updates),preset_cases=len(preset),getters=getters,updates=updates,presets=preset,explicit_services=['ReturnValues.pushInteger service records exact signed32/raw argument; original getters execute','profile begin/end','raw engine GetDt result','SetStun/SetScare complete bodies are provider fixtures; actual predicates and subsequent reloaded current virtual execute','current-state virtual update body','libc strlen/memcmp for original preset std::string.compare'])
(HERE/'native-fsm-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',getter_cases=len(getters),update_cases=len(updates),preset_cases=len(preset))))
