"""Execute original attack-event decisions with observing caller backends.

Original timeline/state getters and name/range dispatch run. The caller's
range-capability query and attack/projectile backends are fixtures. Discarded
debug-switch string/query blocks are skipped, as explicitly reported.
"""
import argparse,hashlib,json,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'))
from cpu import Cpu as BaseCpu,i32,u32
class Cpu(BaseCpu):
 def __init__(self,*args):super().__init__(*args);self.observed=[];self.can_range=0;self.projectile=-1
 def string(self,p):
  data=bytearray()
  while len(data)<4096:
   c=bytes(self.uc.mem_read(p+len(data),1))[0]
   if not c:return bytes(data)
   data.append(c)
  raise AssertionError('Caller string lacks terminator')
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('strcmp','strncmp'):
   a,b=self.string(self.reg(0)),self.string(self.reg(1))
   if name=='strncmp':a=a[:self.reg(2)];b=b[:self.reg(2)]
   self.put(0,(a>b)-(a<b))
  elif name=='range_capability':self.uc.mem_write(self.reg(3),struct.pack('<i',self.projectile));self.put(0,self.can_range)
  elif name=='attack_observer':self.observed.append((1,i32(self.reg(1)),i32(self.reg(2)),i32(self.reg(3))))
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--runtime-sequences',type=int,nargs='+',default=[0,1]);a=p.parse_args();started=time.monotonic();assert 0<len(a.runtime_sequences)<=128 and len(set(a.runtime_sequences))==len(a.runtime_sequences) and all(-2147483648<=v<=2147483647 for v in a.runtime_sequences);runtime_count=len(a.runtime_sequences)*8
 manifest=json.loads((ROOT/'reference/combat-events/original-functions.json').read_text(encoding='utf-8-sig'));assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261002)
 character,ai,cv,av,state,table=[old.data+x for x in (0x1000,0x4000,0x5000,0x6000,0x7000,0x8000)]
 old.uc.mem_write(character,bytes(0x2000));old.uc.mem_write(ai,bytes(0x100));old.pointer(ai+4,character);old.pointer(character,cv);old.pointer(ai,av);old.pointer(character+0x51c,state)
 range_call=old.extern+0xfd00;attack_call=old.extern+0xfd10;old.imports[range_call]='range_capability';old.imports[attack_call]='attack_observer';old.pointer(cv+0x128,range_call);old.pointer(av+0xa8,attack_call)
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 got=u32(0x3c9360+word(0x3c9390));old.pointer(word(got+word(0x3c9394)),table);old.uc.mem_write(table,struct.pack('<5I',0,0,3,0,0))
 skipped=0;projectiles=0
 def hook(uc,address,size,unused):
  nonlocal skipped,projectiles
  if address in (0x3d48cc,0x3d4920,0x3d47d0,0x3d4720):
   skipped+=1;uc.reg_write(old.pc,{0x3d48cc:0x3d48f0,0x3d4920:0x3d4944,0x3d47d0:0x3d47f4,0x3d4720:0x3d4744}[address])
  elif address==0x3e701c:
   old.observed.append((2,i32(old.reg(1)),0,0));projectiles+=1;uc.reg_write(old.pc,uc.reg_read(old.lr))
 old.uc.hook_add(UC_HOOK_CODE,hook,begin=0x3d4434,end=0x3e701c)
 context=new.data+0x1000;out=new.data+0x2000;name_old=old.data+0x9000;name_new=new.data+0x3000;cases=0;attacks=0
 names=('attack_mainhand','attack_offhand','attack_ranged','do_skill','unknown','Attack_mainhand','attack_mainhand_extra','attack','');reference=bytearray();runtime_cases=[]
 for i in range(6000):
  active=rng.randrange(3)!=0;sm=rng.choice((-1,0,1,2,3,4,5,5,5,6,7,8,9,10,11,12,13,14));sequence=rng.choice((-1,0,1,2,2147483647,-2147483648));clip=rng.choice((-1,0,1,2,3,2147483647,-2147483648));old.can_range=rng.randrange(2);old.projectile=rng.randrange(-1,1000);name=rng.choice(names)
  if i<runtime_count:active=True;sm=5;sequence=a.runtime_sequences[i//8];clip=1;old.can_range=(i//4)%2;old.projectile=123;name=names[i%4]
  old.uc.mem_write(state,struct.pack('<i',sm));old.uc.mem_write(ai+0x74,struct.pack('<i',sequence));old.uc.mem_write(character+0x4e4,bytes((int(not active),)));old.uc.mem_write(character+0x4c8,struct.pack('<i',0));old.uc.mem_write(character+0x4ac,struct.pack('<i',clip))
  raw=name.encode()+b'\0';old.uc.mem_write(name_old,raw);new.uc.mem_write(name_new,raw);old.observed.clear()
  try:returned=old.invoke(0x3d4434,[ai,name_old])
  except Exception:
   print('original caller fixture failure',i,name,sm,active,hex(old.uc.reg_read(old.pc)),[hex(old.reg(j)) for j in range(4)]);raise
  assert returned==1
  new.uc.mem_write(context,struct.pack('<iiiIi',sm,sequence,clip if active else -1,old.can_range,old.projectile));new.uc.mem_write(out,b'\xcc'*16);assert new.invoke('dh2_combat_event_route',[out,context,name_new])==0
  action=struct.unpack('<4i',new.uc.mem_read(out,16));expected=old.observed[0] if old.observed else (0,0,0,0);assert len(old.observed)<=1 and action==expected,(i,sm,active,sequence,clip,old.can_range,name,expected,action);attacks+=bool(old.observed);cases+=1
  reference+=struct.pack('<iiiIiI4i',sm,sequence,clip if active else -1,old.can_range,old.projectile,names.index(name),*expected)
  if i<runtime_count:runtime_cases.append({'state':sm,'sequence_step':sequence,'clip_step':clip,'can_range':old.can_range,'projectile':old.projectile,'name':name,'action':expected})
 a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(struct.pack('<I',cases)+reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'attack_event_decisions_compared':cases,'observed_attack_requests':attacks,'observed_projectile_requests':projectiles,'mismatches':0,'discarded_debug_switch_blocks_skipped':skipped,'scope':'State-5 main/off-hand and ranged attack-event decisions, sequence/substep arguments and name gating; original timeline/state getters execute; capability/projectile/attack backends are caller fixtures; noncombat events and full AI/combat lifecycle remain pending','elapsed_seconds':round(time.monotonic()-started,2)}
 report.update({'native_host_reference_sha256':hashlib.sha256(a.reference_output.read_bytes()).hexdigest(),'runtime_cases':runtime_cases,'runtime_sequences':a.runtime_sequences})
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
