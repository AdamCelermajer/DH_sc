"""Original bounded state routines versus ARM64 source, with explicit ordered services.

Original state focus/blur/update/event dispatch/predicates and animation override
execute. Tables, controller/body/animation/AI/FX/timer services are caller fixtures.
The event maps are extracted by executing original OnInit registrations.
"""
import argparse,hashlib,json,math,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../game-data/tests'))
from aggro_differential import float_bits
from body_transform_differential import Cpu as Base
from combat_result_differential import floating

class Cpu(Base):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='state_callback':return self.state_callback(uc,address,size,unused)
  name=self.imports.get(address)
  if name in ('__aeabi_fcmplt','__aeabi_fcmpgt'):
   a,b=floating(self.reg(0)),floating(self.reg(1));self.put(0,int(a<b if name.endswith('fcmplt') else a>b));self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)

def pack_request(service,a=0,b=0,c=0,scalar=0,identity=0):
 return struct.pack('<4IIIQ',service,a&0xffffffff,b&0xffffffff,c&0xffffffff,scalar,0,identity&0xffffffffffffffff)

def equal(a,b,float_index=5):
 if len(a)!=len(b):return False
 expected=struct.unpack('<'+'I'*(len(a)//4),a);actual=struct.unpack('<'+'I'*(len(b)//4),b)
 return all(x==y or i==float_index and math.isnan(floating(x)) and math.isnan(floating(y)) for i,(x,y) in enumerate(zip(expected,actual)))

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--cases',type=int,default=2500);args=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/character-state/original-functions.json').read_text());assert hashlib.sha256(args.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(args.engine,False,manifest);new=Cpu(args.library,True,{'functions':[]});rng=random.Random(2026100305)
 char=old.data+0x1000;sm=char+0x4fc;vt=old.data+0x4000;controller=old.data+0x5000;body=old.data+0x6000;target=old.data+0x7000;table=old.data+0x8000;design=old.data+0x9000;infos=old.data+0xa000;objects=old.data+0xb000;vts=old.data+0xc000;eventinfo=old.data+0xd000
 ns=new.data+0x1000;nf=new.data+0x2000;nc=new.data+0x3000;np=new.data+0x4000;no=new.data+0x5000
 context=0xabcdef0123456789;identity=0x123456789abcdef0
 events=[[],[]];facts=None;records=[];registrations={};transition_calls=0;dt=0;getter=False;callback_mode=0
 def word(address):return struct.unpack('<I',old.uc.mem_read(address,4))[0]
 def put(address,value):old.pointer(address,value&0xffffffff)
 def byte(address,value):old.uc.mem_write(address,bytes((value,)))
 def signed(value):return value if value<0x80000000 else value-0x100000000
 def returned(value=0):old.put(0,value&0xffffffff);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def request(service,a=0,b=0,c=0,scalar=0,key=0):events[0].append(pack_request(service,a,b,c,scalar,key))
 def logical():
  ptr=word(sm+0x20);current=signed(word(ptr)) if ptr else -1
  return struct.pack('<i13I',current,word(char+0x520),word(char+0x528),word(char+0x53c),word(sm+0x60),word(char+0x52c),old.uc.mem_read(char+0x538,1)[0],old.uc.mem_read(char+0x442,1)[0],old.uc.mem_read(char+0x1b5,1)[0],old.uc.mem_read(controller+8,1)[0],old.uc.mem_read(char+0x53a,1)[0],int(bool(word(char+0x2dc))),word(sm+0x28),word(char+0x4e8))
 def hook(uc,address,size,unused):
  nonlocal transition_calls
  if address==0x3c1938:transition_calls+=1;return
  if address in (0x337888,0x3140ec,0x337a88,0x318254,0x3136b4,0x3136b8):returned();return
  if address==0x3c7b18:
   registrations.setdefault(old.reg(1),{})[old.reg(2)]=(old.reg(3),word(uc.reg_read(old.sp)),word(uc.reg_read(old.sp)+4));returned();return
  if address==0x3a49f0:returned(facts['is_player']);return
  if address==0x33dd10:returned(facts['following_path']);return
  if address==0x39361c:returned(facts['is_at_destination']);return
  if address==0x3fffa4:returned(facts['has_ranged_weapon']);return
  if address==0x3a3228:returned();return
  if address==0x3a53e0:returned(facts['stance']);return
  if address==0x4c4bdc:
   # Source queries STANCE flags or DESPAWN delay. Both are explicit inputs.
   caller=uc.reg_read(old.lr)
   returned(facts['despawn_delay'] if 0x3c4c3c<=caller<0x3c4f68 else facts['stance_mask']);return
  if address==0x3a3438:returned(facts['attack_delay']);return
  if address==0x3de6c4:returned(facts['walk_speed']);return
  if address==0x3de74c and not getter:returned(facts['attack_speed']);return
  if address==0x31f66c:returned(dt);return
  if address==0x3c0084:returned(int(old.reg(1) in (3,4,5,12)));return
  if address==0x3c184c:returned(infos+old.reg(1)*16);return
  if address in (0x3c00e0,0x3c1694):
   value=registrations.get(old.reg(1),{}).get(old.reg(2));value=value if value and value[0] in (3,4,5,12) else None
   if address==0x3c00e0:returned(int(value is not None))
   else:
    assert value is not None;to,predicate,adjust=value;old.uc.mem_write(eventinfo,struct.pack('<3I',predicate,adjust,to));returned(eventinfo)
   return
  service={0x3938f8:1,0x46eb20:2,0x46eae0:3,0x3cacb0:4,0x3c93fc:5,0x3caccc:6,0x3c948c:7,0x3dbe24:8,0x4052bc:9,0x393d48:9,0x393cec:9,0x3a4068:10,0x46ece8:11,0x46ec6c:12,0x3a40e4:13,0x3a40b0:14,0x3e0af8:15,0x3bc6b8:16,0x3a4d5c:17,0x394bf8:18,0x3c0b78:19,0x393be8:20}.get(address)
  if service:
   if service==1:
    request(1);byte(char+0x1b5,0)
    if callback_mode==4:uc.mem_write(char+0x1b8,bytes(12))
   elif service==4:
    request(4,old.reg(1));put(char+0x4e8,old.reg(1))
    if callback_mode==2:put(char+0x2dc,0)
   elif service==5:request(5,scalar=old.reg(1))
   elif service==6:
    request(6,old.reg(1),old.reg(2))
    if callback_mode==1:byte(char+0x1b5,0)
   elif service==8:
    assert word(uc.reg_read(old.sp))==0;request(8,old.reg(1),old.reg(2),old.reg(3))
   elif service==9:
    key=old.reg(1);request(9,1 if address==0x4052bc else 2 if address==0x393cec else 0,key=identity if key else 0)
   elif service==11:
    assert word(uc.reg_read(old.sp))==0;request(11,old.reg(1),old.reg(2),old.reg(3))
   elif service==17:
    value=old.reg(2);is_transition=old.reg(1)==0x1d
    request(17,old.reg(1),value if is_transition else 0,key=signed(value) if is_transition else value)
   elif service==18:request(18);put(char+0x2dc,0)
   elif service==20:request(20,*struct.unpack('<3I',uc.mem_read(old.reg(1),12)),scalar=float_bits(1.0))
   else:request(service)
   returned();return
  if address==0x3935dc:returned(target+0x100);return
 old.uc.hook_add(UC_HOOK_CODE,hook)
 def callback(uc,address,size,unused):
  assert new.reg(0)==context and new.reg(1)==ns
  data=bytes(uc.mem_read(new.reg(2),32));events[1].append(data);kind,a,b,c,value,zero,key=struct.unpack('<6IQ',data);assert zero==0
  if kind==1:
   uc.mem_write(ns+32,bytes(4))
   if callback_mode==4:uc.mem_write(nf+16,bytes(12))
  elif kind==4:
   uc.mem_write(ns+52,struct.pack('<I',a))
   if callback_mode==2:uc.mem_write(ns+44,bytes(4))
  elif kind==6 and callback_mode==1:uc.mem_write(ns+32,bytes(4))
  elif kind==18:uc.mem_write(ns+44,bytes(4))
  uc.reg_write(new.pc,uc.reg_read(new.lr))
 new.imports[new.callback+32]='state_callback';new.state_callback=callback;new.uc.mem_write(nc,struct.pack('<QQ',context,new.callback+32))
 old.pointer(vt+0x28,0x3a49f0);old.pointer(vt+0x54,0x33dd10)
 focus={3:0x3c3020,4:0x3c3bf8,5:0x3c404c,12:0x3c4d50};blur={3:0x3c2d3c,4:0x3c3aa4,5:0x3c3f74,12:0x3c499c};update={3:0x3c0e80,4:0x3c1184,5:0x3c14f8,12:0x3c004c};onevent={3:0x3c0004,4:0x3c000c,5:0x3c1288,12:0x3c4c3c}
 for state in (3,4,5,12):
  old.uc.mem_write(infos+state*16,struct.pack('<II',state,objects+state*16));put(objects+state*16,vts+state*32)
  for off,address in ((12,focus[state]),(16,blur[state]),(20,update[state]),(24,onevent[state])):put(vts+state*32+off,address)
 # Move UpdateType literal pool resolves the actual table/constant GOT cells.
 base=(0x3c0f40+word(0x3c115c))&0xffffffff
 for literal,ptr in ((0x3c1160,design),(0x3c1164,table)):
  variable=word(base+word(literal));put(variable,ptr)
 constants=word(base+word(0x3c1168));put(constants+0x2c,old.data+0x20000)
 def load(state,values):
  nonlocal facts
  facts=values;old.uc.mem_write(char,bytes(0x1800));put(char,vt);put(sm+4,char);put(char+0x378,controller);old.uc.mem_write(controller,bytes(32))
  current,flags,gate,type_,elapsed,cached,idle,allowed,heading,locked,alt,present,override,anim=struct.unpack('<i13I',state)
  put(sm+0x20,infos+current*16 if current!=-1 else 0);put(char+0x520,flags);put(char+0x528,gate);put(char+0x53c,type_);put(sm+0x60,elapsed);put(char+0x52c,cached);put(sm+0x28,override);put(char+0x4e8,anim)
  for off,value in ((0x538,idle),(0x442,allowed),(0x1b5,heading),(0x53a,alt)):byte(char+off,value)
  byte(controller+8,locked);put(char+0x2dc,body if present else 0);put(char+0x408,target if values['target'] else 0);old.uc.mem_write(char+0x1b8,struct.pack('<3I',*values['heading']))
  for off,key in ((0x28,'idle'),(0x94,'walk'),(0x70,'run'),(4,'attack_moving'),(8,'attack_static')):put(table+off,values[key])
  put(design+0x5c,values['walk_threshold']);put(design+0x60,values['run_threshold'])
  native=struct.pack('<IIQ7I7i6I',values['is_player'],values['stance_mask'],identity if values['target'] else 0,*values['heading'],values['walk_threshold'],values['run_threshold'],values['walk_speed'],values['attack_speed'],values['idle'],values['walk'],values['run'],values['attack_static'],values['attack_moving'],values['death'],values['stance'],values['attack_delay'],values['despawn_delay'],0,values['is_at_destination'],values['following_path'],values['has_ranged_weapon'])
  assert len(native)==96;new.uc.mem_write(ns,state);new.uc.mem_write(nf,native);return native
 defaults={'is_player':1,'stance_mask':210,'target':0,'heading':[float_bits(1),0,0],'walk_threshold':float_bits(.25),'run_threshold':float_bits(.75),'walk_speed':float_bits(1.25),'attack_speed':float_bits(1.5),'idle':215,'walk':223,'run':230,'attack_static':243,'attack_moving':248,'death':259,'stance':5,'attack_delay':400,'despawn_delay':2000,'is_at_destination':0,'following_path':0,'has_ranged_weapon':0}
 initial=struct.pack('<i13I',3,0x2380,0,0,123,float_bits(1),0,0,1,0,0,1,259,215)
 load(initial,defaults)
 for state,address in ((3,0x3c7e60),(4,0x3c80ac),(5,0x3c8284),(12,0x3c8920)):old.invoke(address,[objects+state*16,state,char,sm])
 def compare(op,state,values,a=0,b=0,payload=0,mode=0):
  nonlocal dt,transition_calls,callback_mode
  callback_mode=mode
  native=load(state,values);events[0].clear();events[1].clear();transition_calls=0;dt=a
  if op==0:old.invoke(0x3c1938,[sm,a,b,target if payload else 0]);result=1;actual=new.invoke('dh2_character_state_transition',[ns,nf,a,b,identity if payload else 0,nc])
  elif op==1:old.invoke(0x3c5684,[sm,a,target if payload else 0]);result=int(bool(transition_calls));actual=new.invoke('dh2_character_state_event',[ns,nf,a,identity if payload else 0,nc])
  else:old.invoke(0x3c628c,[sm]);result=1;actual=new.invoke('dh2_character_state_update',[ns,nf,a,nc])
  expected=logical();observed=bytes(new.uc.mem_read(ns,56));assert result==actual,(len(records),op,a,'return',result,actual);assert equal(expected,observed),(len(records),op,a,'state',struct.unpack('<i13I',expected),struct.unpack('<i13I',observed));assert len(events[0])==len(events[1]) and all(equal(x,y,4) for x,y in zip(*events)),(len(records),op,a,'requests',[struct.unpack('<6IQ',v) for v in events[0]],[struct.unpack('<6IQ',v) for v in events[1]])
  records.append(struct.pack('<6IQ',op,a&0xffffffff,b&0xffffffff,len(events[0]),result,mode,identity if payload else 0)+state+native+expected+b''.join(events[0]))
 # Complete outgoing/incoming including same-state, player/nonplayer, body gates.
 for prior in (-1,3,4,5,12):
  for following in (3,4,5,12):
   for player in (0,1):
    for present in (0,1):
     values=dict(defaults,is_player=player);state=struct.pack('<i13I',prior,0xdeadbeef,6,2,0xfffffffa,float_bits(2),1,1,1,1,0,present,259,248);compare(0,state,values,following,0xc358,1)
 for current in (3,4,5,12):
  for event in (0xc351,0xc352,0xc354,0xc358,0x22,0x23,0x3f,0x1c,0x1a,0xdead):
   for gate in (0,1):
    for player in (0,1):
     state=struct.pack('<i13I',current,0x2380,gate,1,41,float_bits(1),0,gate,gate,0,0,1,259,243);compare(1,state,dict(defaults,is_player=player,target=gate,has_ranged_weapon=1),event,payload=1)
 # Strict threshold/hysteresis and epsilon/special float boundaries.
 for move_type in (0,1,2):
  for heading in (0,0x80000000,float_bits(.25),float_bits(.75),0x7f7fffff,0x7f800000,0x7fc01234):
   values=dict(defaults,heading=[heading,0,0]);state=struct.pack('<i13I',4,0x23c1,0,move_type,12,float_bits(1.25),0,0,1,0,0,1,259,230);compare(2,state,values,16)
 for cached in (0,0x80000000,0x38d1b716,0x38d1b717,0x38d1b718,0x7f7fffff,0x7f800000,0x7fc01234):
  for value in (0,0x80000000,float_bits(1),0x7f7fffff,0x7f800000,0x7fc01234):
   state=struct.pack('<i13I',5,0x2341,0,0,12,cached,0,0,1,0,0,1,259,248);compare(2,state,dict(defaults,attack_speed=value),16)
 callback_state=struct.pack('<i13I',5,0x2341,0,0,12,float_bits(1.5),0,0,1,0,0,1,259,248);compare(1,callback_state,defaults,0x1c,mode=1)
 for following in (3,4,5,12):compare(0,initial,defaults,following,mode=2)
 stop_state=struct.pack('<i13I',4,0x23c1,0,2,12,float_bits(1.25),0,0,1,0,0,1,259,230)
 for following in (3,4,5,12):compare(0,stop_state,defaults,following,mode=4)
 for i in range(args.cases):
  current=rng.choice((3,4,5,12));op=rng.randrange(3);values=dict(defaults,is_player=rng.randrange(2),stance_mask=rng.randrange(256),stance=rng.randrange(-5,10),attack_delay=rng.choice((0,1,400,0xffffffff)),target=rng.randrange(2),is_at_destination=rng.randrange(2),following_path=rng.randrange(2),has_ranged_weapon=rng.randrange(2),attack_static=rng.choice((-1,243)),heading=[float_bits(rng.uniform(-2,2)) for _ in range(3)],walk_speed=float_bits(rng.uniform(0,4)),attack_speed=float_bits(rng.uniform(0,4)))
  state=struct.pack('<i13I',current,rng.getrandbits(32),rng.randrange(4),rng.randrange(3),rng.getrandbits(32),rng.choice((values['walk_speed'],values['attack_speed'],float_bits(rng.uniform(0,4)))),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.choice((-1,259))&0xffffffff,rng.choice((-1,243))&0xffffffff)
  a=rng.choice((3,4,5,12)) if op==0 else rng.choice((0xc351,0xc352,0xc354,0xc358,0x22,0x23,0x3f,0x1c,0x1a)) if op==1 else rng.choice((0,1,16,33,0xffffffff));compare(op,state,values,a,payload=rng.randrange(2))
 getter=True;getter_cases=0
 for value in (-2147483648,-25601,-25600,-25599,-1,0,1,255,256,2147483647)+tuple(rng.randrange(-2147483648,2147483648) for _ in range(1000)):
  old.pointer(char+0xb58,value&0xffffffff);expected=old.invoke(0x3de74c,[char]);new.uc.mem_write(np,bytes(224*4));new.uc.mem_write(np+48*4,struct.pack('<i',value));assert new.invoke('dh2_character_attack_speed',[no,np])==1;observed=struct.unpack('<I',new.uc.mem_read(no,4))[0];assert expected==observed,(value,expected,observed);getter_cases+=1
  state=struct.pack('<i13I',value,*([0]*13));out=bytearray(state);struct.pack_into('<I',out,20,expected);records.append(struct.pack('<6IQ',3,0,0,0,1,0,0)+state+bytes(96)+out)
 idle_cases=0
 for current in range(-1,40):
  for flag in (0,1):
   old.pointer(sm+0x20,infos);old.uc.mem_write(infos,struct.pack('<i',current));expected=old.invoke(0x3c0260,[sm,flag]);assert new.invoke('dh2_character_state_is_idle',[current,flag])==expected;idle_cases+=1
   state=struct.pack('<i13I',current,*([0]*13));records.append(struct.pack('<6IQ',4,flag,0,0,expected,0,0)+state+bytes(96)+state)
 # Malformed ABI inputs are a native defensive boundary, not original behavior.
 new.uc.mem_write(ns,initial);new.uc.mem_write(nf,bytes(96));before=bytes(new.uc.mem_read(ns,56));assert new.invoke('dh2_character_state_event',[ns,nf,0x22,0,0])&0xffffffff==0xffffffff;assert bytes(new.uc.mem_read(ns,56))==before
 reference=struct.pack('<II',0x31545343,len(records))+b''.join(records);args.reference_output.parent.mkdir(parents=True,exist_ok=True);args.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(args.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'kernel_source_sha256':hashlib.sha256((ROOT/'character_state.cpp').read_bytes()).hexdigest(),'comparisons':len(records),'state_cases':len(records)-getter_cases-idle_cases,'attack_speed_cases':getter_cases,'idle_predicate_cases':idle_cases,'mismatches':0,'original_event_registrations':{str(k):{hex(e):list(v) for e,v in regs.items()} for k,regs in registrations.items()},'original_import_calls':old.import_calls,'scope':__doc__,'imported_ieee_single_precision_arithmetic_and_comparisons_modeled':True,'full_service_backends_executed':False,'callback_context_and_target_identity_above_4gib':True,'coverage':{r['original_symbol']:{'instructions':r['size']//4,'seen':sum(int(r['elf_address'],16)<=pc<int(r['elf_address'],16)+r['size'] for pc in old.seen)} for r in manifest['functions']},'elapsed_seconds':round(time.monotonic()-started,2)};args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('coverage','original_event_registrations')}))
if __name__=='__main__':main()
