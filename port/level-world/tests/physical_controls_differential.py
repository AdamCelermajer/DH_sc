"""Original PhysicalObject control instructions versus compiled native ARM64.

The original getters, velocity setters/caps and complete GameObject::Stop
execute. DropPath and virtual physical-movable query are fixtures. SetXForm
is an argument/state observer; for Stop only, caller applies the emitted XY
to its supplied body snapshot before direct Stop resets. Actual Box2D transform,
shape synchronization, world simulation and contacts are outside this audit.
Imported IEEE float operations are modeled; generated NaN payloads may differ.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_differential import ROOT,Cpu as BaseCpu
from combat_result_differential import floating

OFFSETS=(4,8,0x38,0x40,0x44,0x48,0x4c,0x50,0x54,0x8c)
class Cpu(BaseCpu):
 def external(self,uc,address,size,unused):
  if address==self.callback+128:
   self.put(0,self.can_move);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
def words(*values):return struct.pack('<'+'I'*len(values),*values)
def equal(a,b):
 return len(a)==len(b) and all(x==y or math.isnan(floating(x)) and math.isnan(floating(y)) for x,y in zip(struct.unpack('<'+'I'*(len(a)//4),a),struct.unpack('<'+'I'*(len(b)//4),b)))

def main():
 p=argparse.ArgumentParser()
 for name in ('engine','library','report','reference-output'):p.add_argument('--'+name,type=Path,required=True)
 p.add_argument('--cases',type=int,default=4096);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/physical-controls/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});body=old.data+0x1000;obj=old.data+0x2000;game=old.data+0x3000;vt=old.data+0x4000;const=old.data+0x5000;result=old.data+0x6000
 state=new.data+0x1000;args=new.data+0x2000;out=new.data+0x3000;records=[];rng=random.Random(20261022)
 transform_calls=[];drop_calls=0;movable=0;operation=0;nan_variants=0;totals=[0]*6;wake_cases=0;zero_sleep_retained=0;mid=None;stop_gates=[0]*4
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def snap():return words(struct.unpack('<H',old.uc.mem_read(body,2))[0])+b''.join(bytes(old.uc.mem_read(body+o,4)) for o in OFFSETS)+bytes(old.uc.mem_read(obj+12,4))
 def hook(uc,address,size,unused):
  nonlocal drop_calls,mid
  if address==0x7e164c:
   assert old.reg(0)==body;request=bytes(uc.mem_read(old.reg(1),8))+words(old.reg(2),1);transform_calls.append(request);mid=snap()
   if operation==5:uc.mem_write(body+4,request[:8])
   ret(1)
  elif address==0x52aae4:
   assert old.reg(1)==game+0x1c8;drop_calls+=1;ret()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 old.uc.mem_write(obj,bytes(0x40));old.pointer(obj+0x14,body);old.uc.mem_write(game,bytes(0x400));old.pointer(game,vt);old.pointer(vt+0x64,old.callback+128)
 # Original Stop obtains default heading from its GOT. Supply caller storage
 # for that global constant, while all original copy/reset instructions execute.
 got=(0x393910+word(0x3939e4))&0xffffffff;heading_slot=got+word(0x3939ec);old.pointer(heading_slot,const);old.uc.mem_write(const,bytes(12))
 def compare(op,initial,values,present=1,can_move=1):
  nonlocal operation,movable,mid,nan_variants,wake_cases,zero_sleep_retained
  operation=op;movable=can_move;old.can_move=can_move;mid=None;transform_calls.clear();totals[op]+=1
  old.uc.mem_write(body,b'\xa5'*0x90);old.uc.mem_write(body,struct.pack('<H',struct.unpack_from('<I',initial)[0]));
  for i,o in enumerate(OFFSETS):old.uc.mem_write(body+o,initial[4+i*4:8+i*4])
  old.uc.mem_write(obj+12,initial[44:48]);old.pointer(game+0x2dc,obj if present else 0);old.uc.mem_write(game+0x160,values[:8]+words(0x42cc0000));old.uc.mem_write(game+0x1b4,b'\x01\x01')
  before_body=bytes(old.uc.mem_read(body,0x90));new.uc.mem_write(state,initial);new.uc.mem_write(args,values);new.uc.mem_write(out,bytes(16));old.uc.mem_write(result,bytes(16))
  v=struct.unpack('<4I',values)
  if op==0:old.invoke(0x46e918,[obj,v[0],v[1]]);assert new.invoke('dh2_physical_set_linear',[state,args])==0
  elif op==1:old.invoke(0x46e864,[obj,*v]);assert new.invoke('dh2_physical_add_linear',[state,args])==0
  elif op==2:old.invoke(0x46e978,[obj,v[0]]);assert new.invoke('dh2_physical_set_angular',[state,args])==0
  elif op==3:
   old.invoke(0x46e818,[result,obj]);old.uc.mem_write(result+8,words(old.invoke(0x46e858,[obj]),old.invoke(0x46e750,[obj])));assert new.invoke('dh2_physical_query',[out,state])==0
  elif op==4:old.invoke(0x46ea80,[obj,v[0],v[1]]);assert new.invoke('dh2_physical_request_position',[out,state,args])==0
  else:
   old.invoke(0x3938f8,[game]);stop_gates[present*2+can_move]+=1
   # Logical Stop copies current destination, clears moving/heading-active and
   # assigns supplied default heading, independent of its physical gate.
   assert bytes(old.uc.mem_read(game+0x1a8,12))==values[:8]+words(0x42cc0000)
   assert bytes(old.uc.mem_read(game+0x1b4,2))==bytes(2) and bytes(old.uc.mem_read(game+0x1b8,12))==bytes(12)
   if present and can_move:
    assert new.invoke('dh2_physical_stop_begin',[out,state,args])==0
    assert equal(mid,bytes(new.uc.mem_read(state,48))),('Stop state before transform',len(records))
    request=bytes(new.uc.mem_read(out,16));new.uc.mem_write(state+4,request[:8]);assert new.invoke('dh2_physical_stop_finish',[state])==0
  expected=snap();actual=bytes(new.uc.mem_read(state,48));assert equal(expected,actual),(len(records),op,initial.hex(),values.hex(),expected.hex(),actual.hex())
  expected_output=transform_calls[0] if transform_calls else bytes(old.uc.mem_read(result,16));actual_output=bytes(new.uc.mem_read(out,16));assert equal(expected_output,actual_output),('output',len(records),op,expected_output.hex(),actual_output.hex())
  assert len(transform_calls)==int(op==4 or op==5 and present and can_move)
  before=bytearray(before_body);after=bytes(old.uc.mem_read(body,0x90))
  # Every unmodeled body byte must remain unchanged, including upper flag16.
  for i in range(0x90):
   if i<2 or any(o<=i<o+4 for o in OFFSETS):continue
   assert before[i]==after[i],('unexpected body write',op,i)
  nan_variants+=sum(x!=y for x,y in zip(struct.unpack('<12I',expected),struct.unpack('<12I',actual)))
  if op==0:assert actual[16:24]==values[:8],'setLinear preserves supplied bits'
  if op==2:assert actual[24:28]==values[:4],'setAngular preserves supplied bits'
  if op in (0,1):
   wake_cases+=int(bool(struct.unpack_from('<I',initial)[0]&8) and not(struct.unpack_from('<I',expected)[0]&8))
   zero_sleep_retained+=int(v[0]&0x7fffffff==v[1]&0x7fffffff==0 and bool(struct.unpack_from('<I',expected)[0]&8))
  records.append(words(op,present,can_move,len(transform_calls))+values+initial+expected+expected_output+(mid if mid is not None else expected))
 boundary=(0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234,0xffc01234,0x7f801234)
 def initial(seed=0):return words(0xa55d,*[boundary[(seed+i)%len(boundary)] for i in range(11)])
 for i,x in enumerate(boundary):
  for j,y in enumerate(boundary):
   for op in range(6):compare(op,initial(i+j),words(x,y,boundary[(i+7)%15],boundary[(j+9)%15]),(i+j)%2 if op==5 else 1,i%2 if op==5 else 1)
 boundary_cases=len(records)
 for n in range(a.cases):
  op=n%6;s=words(rng.randrange(65536),*[rng.getrandbits(32) for _ in range(11)]);v=words(*[rng.choice(boundary) if n%2 else rng.getrandbits(32) for _ in range(4)])
  compare(op,s,v,rng.randrange(2) if op==5 else 1,rng.randrange(2) if op==5 else 1)
  if n%1024==1023:print(f'Physical controls original cases {n+1}/{a.cases}',flush=True)
 # Atomic malformed native caller rejection is a separate contract; original
 # C++ methods never validate arbitrary pointers or widened native flags.
 reject_count=0
 new.uc.mem_write(state,words(0x10000)+bytes(44));before=bytes(new.uc.mem_read(state,48));new.uc.mem_write(out,b'\x5a'*16)
 for name,argv in [('dh2_physical_set_linear',[state,args]),('dh2_physical_add_linear',[state,args]),('dh2_physical_set_angular',[state,args]),('dh2_physical_wake',[state]),('dh2_physical_query',[out,state]),('dh2_physical_request_position',[out,state,args]),('dh2_physical_stop_begin',[out,state,args]),('dh2_physical_stop_finish',[state])]:
  assert new.invoke(name,argv)==1 and bytes(new.uc.mem_read(state,48))==before and bytes(new.uc.mem_read(out,16))==b'\x5a'*16;reject_count+=1
 new.uc.mem_write(state,initial());before=bytes(new.uc.mem_read(state,48))
 for name,argv in [('dh2_physical_set_linear',[state,0]),('dh2_physical_add_linear',[state,0]),('dh2_physical_set_angular',[state,0]),('dh2_physical_query',[state,state]),('dh2_physical_request_position',[state,state,args]),('dh2_physical_stop_begin',[state,state,args])]:
  assert new.invoke(name,argv)==1 and bytes(new.uc.mem_read(state,48))==before;reject_count+=1
 reference=words(0x31434850,len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'boundary_cases':boundary_cases,'synthetic_cases':a.cases,'operations':dict(zip(('set_linear','add_linear','set_angular','query','request_position','stop'),totals)),'original_stop_gates':stop_gates,'original_drop_path_observations':drop_calls,'nonzero_linear_wakes':wake_cases,'zero_linear_sleep_retained':zero_sleep_retained,'generated_nan_field_variants':nan_variants,'atomic_rejection_checks':reject_count,'mismatches':0,'original_import_calls':old.import_calls,'arm64_pointers_above_4gib':True,'finite_words_exact':True,'nan_comparison':'classification; setter supplied bits preserved exactly','box2d_transform_backend_executes':False,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
