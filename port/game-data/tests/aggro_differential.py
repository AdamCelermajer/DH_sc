"""Original aggression instructions versus compiled ARM64 source.

Original map lookup/insertion/rebalancing/erase, dead queries, Set/Add/Clear
branches and all five queries run. The node allocator is caller fixture storage;
IsPlayer, OnAggro/OnDeAggro, SetTarget and controller Stop are service observers.
Imported IEEE single-precision arithmetic/comparison is modeled as in the
existing combat audits; arithmetic NaN sign/payload is not treated as portable.
The packaged native build's TLS canary and checked memcpy are caller fixtures.
This does not reconstruct acquisition or the AI FSM.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_TPIDR_EL0
from combat_application_differential import Cpu as FloatCpu
from combat_result_differential import bits,floating
ROOT=Path(__file__).resolve().parents[1]
CAPACITY=8

def float_bits(value):
 try:return bits(value)
 except OverflowError:return bits(math.copysign(math.inf,value))

class Cpu(FloatCpu):
 def __init__(self,path,arm64,provenance):
  super().__init__(path,arm64,provenance)
  if arm64:
   # Android's packaged stack protector reads the thread canary via TLS.
   # This caller fixture supplies a stable canary; the protector executes.
   tls=self.data+0x1ff0000;self.uc.reg_write(UC_ARM64_REG_TPIDR_EL0,tls);self.uc.mem_write(tls+0x28,struct.pack('<Q',0xace126d24c47cb95))
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='__aeabi_fcmpgt':self.import_calls[name]=self.import_calls.get(name,0)+1
  if name=='__memcpy_chk':
   dst,src,count,capacity=[self.reg(i) for i in range(4)];assert count<=capacity and count<=0x2000000
   if count:self.uc.mem_write(dst,bytes(self.uc.mem_read(src,count)))
   self.put(0,dst);self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif name in ('__aeabi_fadd','__aeabi_fsub'):
   a,b=floating(self.reg(0)),floating(self.reg(1));self.put(0,float_bits(a+b if name.endswith('fadd') else a-b));self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif address in (self.callback,self.callback+16):
   self.events.append((1 if address==self.callback else 2,self.reg(0),self.reg(1)));self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--cases',type=int,default=5000);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/aggro/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});old.events=[];rng=random.Random(20261005)
 chars=[old.data+0x1000+i*0x2000 for i in range(CAPACITY)];keys=[0x100000000+c for c in chars];vt=old.data+0x20000;avt=vt+0x100;heap=old.data+0x100000
 tables=[[new.data+0x1000+i*0x40+j*0x10 for j in range(2)] for i in range(CAPACITY)];storage=[[new.data+0x10000+i*0x200+j*0x80 for j in range(2)] for i in range(CAPACITY)];request=new.data+0x2000;output=new.data+0x2100;query=new.data+0x2200
 current_owner=0;current_facts=0;allocations=0;deallocations=0;original_dead_queries=0;nan_payload_differences=0;records=[];totals={'set':0,'add':0,'clear':0,'inserted':0,'removed':0,'aggro_notifications':0,'deaggro_notifications':0,'target_clear_requests':0,'gated_set_add':0};specials=[]
 def float_equal(expected,actual):
  nonlocal nan_payload_differences
  if expected==actual:return True
  if math.isnan(floating(expected)) and math.isnan(floating(actual)):nan_payload_differences+=1;return True
  return False
 def entries_equal(expected,actual):return len(expected)==len(actual) and all(e[0]==n[0] and e[2]==n[2] and float_equal(e[1],n[1]) for e,n in zip(expected,actual))
 def word(address):return struct.unpack('<I',old.uc.mem_read(address,4))[0]
 def returned(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,address,size,unused):
  nonlocal heap,allocations,deallocations,original_dead_queries
  if address==0x708ec0:
   assert word(old.reg(0))==24;returned(heap);heap+=32;allocations+=1
  elif address==0x708f00:deallocations+=1;returned()
  elif address==0x3a49f0:
   assert old.reg(0)==chars[current_owner];returned(int(bool(current_facts&1)))
  elif address==0x3a2ed4:original_dead_queries+=1
  elif address==0x3d6890:
   assert old.reg(1)==old.reg(2)==0;old.events.append((4,old.reg(0),0));returned()
  elif address==0x40559c:old.events.append((8,old.reg(0),0));returned()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 old.pointer(vt+0x28,0x3a49f0);old.pointer(vt+0x34,0x3a2ed4);old.pointer(avt+0x38,old.callback);old.pointer(avt+0x3c,old.callback+16)
 def reset():
  nonlocal heap
  heap=old.data+0x100000
  for i,c in enumerate(chars):
   old.uc.mem_write(c,bytes(0x1800));old.pointer(c,vt);old.pointer(c+0x3c8,avt);old.pointer(c+0x3cc,c);old.pointer(c+0x378,c+0x1700)
   for j,offset in enumerate((0x444,0x45c)):
    head=c+offset;old.uc.mem_write(head,struct.pack('<5I',0,0,head,head,0));new.uc.mem_write(storage[i][j],bytes(128));new.uc.mem_write(tables[i][j],struct.pack('<QII',storage[i][j],0,CAPACITY))
 def original_entries(i,j):
  head=chars[i]+(0x444 if j==0 else 0x45c);values=[]
  def walk(n):
   if not n:return
   walk(word(n+8));values.append((0x100000000+word(n+16),word(n+20),0));walk(word(n+12))
  walk(word(head+4));assert len(values)==word(head+16);assert values==sorted(values);return values
 def native_entries(i,j):
  ptr,count,capacity=struct.unpack('<QII',new.uc.mem_read(tables[i][j],16));assert ptr==storage[i][j] and capacity==CAPACITY and count<=capacity
  return [struct.unpack_from('<QII',new.uc.mem_read(ptr,count*16),n*16) for n in range(count)]
 def packed(entries):return b''.join(struct.pack('<QII',*e) for e in entries)+bytes((CAPACITY-len(entries))*16)
 def compare(owner,target,value,facts,operation,label):
  nonlocal current_owner,current_facts
  current_owner=owner;current_facts=facts
  for c in chars:old.uc.mem_write(c+0x1449,b'\0')
  old.uc.mem_write(chars[owner]+0x1449,bytes((int(bool(facts&2)),)));old.uc.mem_write(chars[target]+0x1449,bytes((int(bool(facts&4)),)))
  old.pointer(chars[target]+0x408,chars[owner] if facts&8 else 0);old.events.clear()
  before_out=original_entries(owner,0);before_in=original_entries(target,1);incoming_count=word(chars[owner]+0x46c)
  assert entries_equal(before_out,native_entries(owner,0)) and entries_equal(before_in,native_entries(target,1))
  returned_bits=old.invoke((0x3d79ec,0x3d7c68,0x3d6d68)[operation],[chars[owner]+0x3c8,chars[target],value]) if operation<2 else old.invoke(0x3d6d68,[chars[owner]+0x3c8,chars[target]])
  exists=any(e[0]==keys[target] for e in before_out);expected_requests=sum(e[0] for e in old.events);after_out=original_entries(owner,0);after_in=original_entries(target,1)
  inserted=int(operation<2 and not exists and not(facts&7));removed=int(operation==2 and exists)
  if operation==2:returned_bits=0
  expected_change=struct.pack('<4I',returned_bits,expected_requests,inserted,removed)
  new.uc.mem_write(request,struct.pack('<QQQQII',tables[owner][0],tables[target][1],keys[owner],keys[target],value,facts));assert new.invoke('dh2_aggro_apply',[output,request,operation])==0
  actual_change=struct.unpack('<4I',new.uc.mem_read(output,16));expected_words=struct.unpack('<4I',expected_change)
  assert float_equal(expected_words[0],actual_change[0]) and expected_words[1:]==actual_change[1:],(label,'change',value,facts,expected_words,actual_change)
  if operation==0:assert actual_change[0]==returned_bits,(label,'set preserves every supplied bit')
  for i in range(CAPACITY):
   for j in range(2):assert entries_equal(original_entries(i,j),native_entries(i,j)),(label,'table',i,j,original_entries(i,j),native_entries(i,j))
  if operation==0 and not(facts&7):
   assert next(e[1] for e in native_entries(owner,0) if e[0]==keys[target])==value and next(e[1] for e in native_entries(target,1) if e[0]==keys[owner])==value
  expected_order=([1] if inserted else [2] if removed else [])+([4,8] if operation==2 and facts&8 else [])
  assert [e[0] for e in old.events]==expected_order,(label,'request order',old.events)
  for event,obj,arg in old.events:
   if event in (1,2):assert obj==chars[target]+0x3c8 and arg==chars[owner]
  original_threat=old.invoke(0x3d4ac8,[chars[owner]+0x3c8,chars[target]]);count=old.invoke(0x3d4a10,[chars[owner]+0x3c8]);has=old.invoke(0x3d49f0,[chars[owner]+0x3c8]);aggroed=old.invoke(0x3d4a00,[chars[owner]+0x3c8]);highest=old.invoke(0x3d4a18,[chars[owner]+0x3c8]);highest=highest+0x100000000 if highest else 0
  incoming_count=word(chars[owner]+0x46c);assert new.invoke('dh2_aggro_query',[query,tables[owner][0],keys[target],incoming_count])==0
  expected_query=struct.pack('<4IQ',original_threat,count,has,aggroed,highest);actual_query=struct.unpack('<4IQ',new.uc.mem_read(query,24));expected_fields=struct.unpack('<4IQ',expected_query);assert float_equal(expected_fields[0],actual_query[0]) and expected_fields[1:]==actual_query[1:],(label,'query',expected_fields,actual_query)
  record=struct.pack('<4I2Q2I',operation,value,facts,incoming_count,keys[owner],keys[target],len(before_out),len(before_in))+packed(before_out)+packed(before_in)+struct.pack('<2I',len(after_out),len(after_in))+packed(after_out)+packed(after_in)+expected_change+expected_query
  assert len(record)==600;records.append(record);totals[('set','add','clear')[operation]]+=1;totals['inserted']+=inserted;totals['removed']+=removed;totals['aggro_notifications']+=bool(expected_requests&1);totals['deaggro_notifications']+=bool(expected_requests&2);totals['target_clear_requests']+=bool(expected_requests&4);totals['gated_set_add']+=int(operation<2 and bool(facts&7))
  if isinstance(label,str):specials.append({'case':label,'returned_bits':returned_bits,'requests':expected_requests,'outgoing_count':count,'incoming_count':incoming_count,'highest':highest})
 reset()
 for owner,target,value,facts,op,label in [(0,3,bits(5.),0,0,'first target'),(0,1,bits(5.),0,0,'positive tie chooses lower key'),(0,2,bits(-5.),0,0,'negative retained'),(0,3,0,0,0,'zero retained'),(0,1,bits(2.),1,1,'rejected add returns negative previous'),(0,2,bits(-2.),0,1,'negative add'),(0,1,0,0,2,'remove reciprocal'),(0,1,0,8,2,'absent entry still clears current target'),(0,2,0,8,2,'remove and clear target'),(0,0,bits(1.),0,0,'self relation'),(0,0,0,0,2,'remove self relation'),(1,0,bits(10.),0,0,'incoming aggro count independent'),(0,2,0,0,2,'empty outgoing with incoming relation')]:compare(owner,target,value,facts,op,label)
 boundary_words=(0,0x80000000,bits(1.),bits(-1.),0x00000001,0x80000001,0x007fffff,0x00800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234,0xffc01234,0x7f801234)
 for previous in boundary_words:
  for supplied in boundary_words:
   reset();compare(0,1,previous,0,0,('boundary set',previous));compare(0,1,supplied,0,1,('boundary add',previous,supplied));compare(0,1,supplied,0,0,('boundary replacement',previous,supplied))
 boundary_cases=len(boundary_words)**2*3
 reset()
 for i in range(a.cases):
  if i%128==0:reset()
  owner=rng.randrange(CAPACITY);target=rng.randrange(CAPACITY-1);target+=target>=owner;facts=rng.randrange(16);operation=rng.randrange(3);value=rng.choice(boundary_words) if i%3 else rng.getrandbits(32)
  compare(owner,target,value,facts,operation,('synthetic',i))
  if i%1000==999:print(f'Original aggression cases {i+1}/{a.cases}',flush=True)
 combat_cases=0;combat_sources=[]
 for name in ('combat-application-arm64-differential.json','player-application-arm64-differential.json','player-defender-arm64-differential.json'):
  path=ROOT/'reports'/name;report=json.loads(path.read_text());combat_sources.append({'file':name,'sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
  for sequence in report['live_sequences']:
   reset()
   for index,attack in enumerate(sequence['attacks']):
    # F_ApplyResult adds attacker-derived threat to the defender's AI table.
    # That original call rejects a player defender, even with positive damage.
    compare(0,1,attack['application_words'][14],1 if report.get('player_defender') else 0,1,('captured combat threat',name,sequence['defender'],index));combat_cases+=1
 a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(struct.pack('<I',len(records))+b''.join(records))
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'comparisons':len(records),'synthetic_cases':a.cases,'boundary_cases':boundary_cases,'explicit_behavior_cases':len(specials),'captured_combat_threat_cases':combat_cases,'mismatches':0,'full_outgoing_and_reciprocal_tables_compared':True,'finite_float_bits_and_all_query_fields_compared':True,'set_preserves_all_supplied_bits':True,'arithmetic_nan_comparison':'unordered NaN class; sign/payload implementation dependent','nan_payload_field_differences':nan_payload_differences,'arm64_storage_and_character_keys_above_4gib':True,'original_tree_insertion_rebalancing_and_erase_execute':True,'original_dead_queries_executed':original_dead_queries,'caller_node_allocations':allocations,'caller_node_deallocations':deallocations,'original_import_calls':old.import_calls,'totals':totals,'behavior_snapshots':specials,'combat_reference_sources':combat_sources,'reference_sha256':hashlib.sha256(a.reference_output.read_bytes()).hexdigest(),'scope':'Aggression table Set/Add/Clear/Get/Highest/Count/Has/IsAggroed with reciprocal incoming relations. Original node allocator replaced by fixture storage; IsPlayer and target callbacks/SetTarget/controller Stop are observers. Original IsDead/map traversal/insertion/rebalancing/erase instructions execute. Imported IEEE float add/sub/greater-than modeled; finite bits exact, arithmetic NaNs compared by unordered class without payload/sign parity. Full callback backends, acquisition, range/visibility/decay, target selection and AI/FSM remain pending.','elapsed_seconds':round(time.monotonic()-started,2)}
 report['native_import_calls']=new.import_calls;report['packaged_native_tls_canary_supplied']=bool(new.import_calls.get('__memcpy_chk'))
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}),flush=True)
if __name__=='__main__':main()
