"""Original host globals versus O2 ARM64; manager/session ownership and signed ReturnValues push services explicit."""
import argparse,hashlib,itertools,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu as Base
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def bits(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def signed(u):return struct.unpack('<i',struct.pack('<I',u&0xffffffff))[0]
class Cpu(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='__aeabi_f2iz':
   f=struct.unpack('<f',struct.pack('<I',self.reg(0)))[0];v=0 if math.isnan(f) else -2147483648 if f<=-2147483648 else 2147483647 if f>=2147483648 else int(f)
   self.put(0,v&0xffffffff);self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();ref=ROOT/'reference/character-host-context';engine=REPO/'.local-inputs/libDungeonHunter2.so';manifest=json.loads((ref/'original-functions.json').read_text());assert sha(engine)==manifest['original_sha256'];old=Cpu(engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});trace=[[],[]];setting={};op=0
 def w(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def ret(v=0):old.put(0,v&0xffffffff);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 app=w(((0x37cc0c+8+w(0x37cc34))&0xffffffff)+w(0x37cc38));manager=old.data+0x1000;player=old.data+0x2000;level=old.data+0x3000;rows=[old.data+0x4000,old.data+0x5000];values=old.data+0x6000;args=old.data+0x7000;vec=args+32;returned=old.data+0x8000
 level_slot=w(((0x31f59c+8+w(0x31f5ac))&0xffffffff)+w(0x31f5b0));range_slot=w(((0x37f200+8+w(0x37f348))&0xffffffff)+w(0x37f350));old.pointer(app+0x40,manager)
 ns=new.data+0x1000;np=new.data+0x2000;nl=new.data+0x3000;nr=[new.data+0x4000,new.data+0x5000];na=new.data+0x6000
 table=[[10,47,76,8,45,74],[12,49,78,9,46,75],[-2147483648,2147483647,-1,0,-3,123456789]]
 level_reference=REPO/'port/game-data/reference/level-tables';level_capture=json.loads((level_reference/'original-loader-capture.json').read_text());level_gold=level_reference/'level-table-fixtures.bin';assert level_capture['original_sha256']==sha(engine) and level_capture['gold_sha256']==sha(level_gold)
 data=level_gold.read_bytes();at=0
 def get():
  nonlocal at
  v=struct.unpack_from('<I',data,at)[0];at+=4;return v
 def skip():
  nonlocal at
  n=get();at+=n
 assert get()==0x3144544c;record_count=get()
 for i in range(record_count):
  kind=get();skip();words=list(struct.unpack_from('<'+('18I' if kind else '7I'),data,at));at+=(18 if kind else 7)*4
  for j in range(2 if kind else 1):skip()
  if i<84 and kind:table.append([signed(v) for v in words[12:18]])
 assert at==len(data) and len(table)==54
 for t in range(2):
  for i,row in enumerate(table):
   selected=[signed((x+1000*t)&0xffffffff) for x in row];old.uc.mem_write(rows[t]+i*72+0x30,struct.pack('<6i',*selected));new.uc.mem_write(nr[t]+i*24,struct.pack('<6i',*selected))
 def hook(uc,address,size,unused):
  if address==0x36e09c:assert old.reg(0)==manager;trace[0].append([0,op,0]);ret(player)
  elif address==0x31f594:trace[0].append([1,op,0])
  elif address in (0x37f25c,0x37f26c,0x37f2c8,0x37f2d8,0x37f304,0x37f314):trace[0].append([2,op,setting['index']])
  elif address==0x37cb24:
   assert old.reg(0)==returned;trace[0].append([3,op,signed(old.reg(1))])
   if setting['mutate'] and sum(e[0]==3 for e in trace[0])==1:old.pointer(range_slot,rows[1]);old.pointer(level+0x3c,2);old.pointer(level+0x118,99);old.pointer(player+0x330,99)
   ret()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 def native(uc,address,size,unused):
  service,operation,value,reserved=struct.unpack('<IIiI',uc.mem_read(new.reg(1),16));assert operation==op and not reserved;trace[1].append([service,operation,value]);data=count=0
  if service==0:data=np
  elif service==1:data=nl if setting['present'] else 0
  elif service==2:data=nr[int(bool(setting['mutate'] and any(e[0]==3 for e in trace[1])))];count=len(table)
  elif service==3:
   if setting['mutate'] and sum(e[0]==3 for e in trace[1])==1:uc.mem_write(nl,struct.pack('<ii',2,99));uc.mem_write(np,struct.pack('<iI',99,0))
  else:raise AssertionError(service)
  uc.mem_write(new.reg(2),struct.pack('<QII',data,count,0));new.put(0,0);uc.reg_write(new.pc,uc.reg_read(new.lr))
 new.imports[new.callback+32]='body_callback';new.body_callback=native;new.uc.mem_write(ns,struct.pack('<QQ',0xabcdef0123456789,new.callback+32))
 records=[];gold=bytearray(b'HCT2'+bytes(4)+struct.pack('<I',len(table))+b''.join(struct.pack('<6i',*row) for row in table));ordered=0;rng=random.Random(20261004)
 patterns=[bits(x) for x in [-3,-1,-.999,0,.999,1,1.999,2,2.999,3,2147483648,-2147483648]]+[0x80000000,0x7fc01234,0x7f800000,0xff800000,1,0x7f7fffff]
 cases=[]
 for index,kind,number,count,mutate in itertools.product([-1,0,1,2],range(8),patterns,[0,1,33],range(2)):
  # Non-numeric types do not consult getNumber; compact the repeated payloads.
  if kind!=3 and number!=patterns[0]:continue
  cases.append((2,index,kind,number,count,mutate,1,0,0))
 for operation,present,cached,difficulty,count,mutate in itertools.product(range(2),range(2),[-2147483648,-1,0,1,16777217,2147483647],[-3,0,1,2,2147483647],[0,33],range(2)):
  cases.append((operation,0,3,bits(2),count,mutate,present,cached,difficulty))
 for index,tier in itertools.product(range(3,len(table)),range(3)):cases.append((2,index,3,bits(tier),1,0,1,17,tier))
 for op,index,kind,number,count,mutate,present,cached,difficulty in cases:
  setting=dict(index=index,mutate=mutate,present=present);old.pointer(player+0x330,cached&0xffffffff);old.pointer(level+0x3c,index&0xffffffff);old.pointer(level+0x118,difficulty&0xffffffff);old.pointer(level_slot,level if present else 0);old.pointer(range_slot,rows[0]);old.uc.mem_write(values,bytes(112*max(1,count)));old.pointer(values+4,kind);old.pointer(values+8,number);old.pointer(args+4,vec);old.uc.mem_write(vec,struct.pack('<3I',values,values+112*count,values+112*count));new.uc.mem_write(np,struct.pack('<iI',cached,0));new.uc.mem_write(nl,struct.pack('<ii',index,difficulty));new.uc.mem_write(na,bytes(40*max(1,count)));new.uc.mem_write(na,struct.pack('<III',kind,0,number));trace[0].clear();trace[1].clear()
  old.invoke([0x37cc00,0x37cb8c,0x37f1f0][op],[args,returned,0]);assert new.invoke('dh2_character_host_context_query',[op,na if count else 0,count,ns])==1
  assert trace[0]==trace[1],(op,setting,kind,number,count,trace)
  record=[op,index&0xffffffff,kind,number,count,mutate,present,cached&0xffffffff,difficulty&0xffffffff,len(trace[0])];gold+=struct.pack('<10I',*record)
  for event in trace[0]:gold+=struct.pack('<IIi',*event)
  ordered+=len(trace[0]);records.append(dict(input=record,trace=trace[0].copy()))
 struct.pack_into('<I',gold,4,len(records));(ref/'host-context-fixtures.bin').write_bytes(gold)
 trace[1].clear();guards=0
 for arguments in ([3,0,0,ns],[0,0,0,0],[0,0,1,ns],[0,na,1048577,ns],[0,na+1,1,ns]):assert new.invoke('dh2_character_host_context_query',arguments)&0xffffffff==0xffffffff and not trace[1];guards+=1
 op=2;setting=dict(index=0,mutate=0,present=0);assert new.invoke('dh2_character_host_context_query',[2,0,0,ns])&0xffffffff==0xfffffffe and trace[1]==[[1,2,0]]
 paths=['character_host_context.hpp','character_host_context.cpp','tests/character_host_context_differential.py','tools/build_character_host_context_oracle.ps1'];report=dict(validation='PASS',scope=__doc__,original_sha256=sha(engine),original_manifest_sha256=sha(ref/'original-functions.json'),optimized_arm64_library_sha256=sha(a.library),gold_sha256=sha(ref/'host-context-fixtures.bin'),comparisons=len(records),ordered_services=ordered,actual_decoded_row_tier_cases=153,Level_original_capture_sha256=sha(level_reference/'original-loader-capture.json'),Level_original_gold_sha256=sha(level_gold),native_atomic_rejections=guards,missing_range_level_failure=True,mismatches=0,source_sha256={str((ROOT/x).relative_to(REPO)):sha(ROOT/x) for x in paths},original_instructions_executed=True,compiled_arm64_instructions_executed=True,soft_float_imports=old.import_calls,services=['PlayerManager GetHostingPlayer selected receiver','Application current Level pointer executes','Level range table pointer/count fixture','signed ReturnValues.pushInteger service; first-push table replacement'],packaged_APK=False)
 (ROOT/'reports/character-host-context-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');(ref/'original-probe.json').write_text(json.dumps(dict(validation='PASS',original_sha256=sha(engine),rows=records),indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','comparisons','ordered_services','native_atomic_rejections']}))
if __name__=='__main__':main()
