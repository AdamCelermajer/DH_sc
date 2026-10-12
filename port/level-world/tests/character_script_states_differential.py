"""Actual Register/Change/current calls/External ordering versus optimized ARM64 kernels; original STL and Lua calls are explicit services."""
import argparse,hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();ref=ROOT/'reference/character-script-states';manifest=json.loads((ref/'original-functions.json').read_text());old=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Cpu(a.library,True,{'functions':[]})
 os=old.data+4096;ns=new.data+4096;oa=old.data+8192;ov=old.data+12288;nv=new.data+8192;services=new.data+12288
 records=[[old.data+0x10000+i*0x100,new.data+0x10000+i*0x100] for i in range(3)]
 strings=[old.data+0x20000,new.data+0x20000];names=[[f'{chr(65+i)}_{j}' for j in range(4)] for i in range(3)];traces=[[],[]];mutation=-1;destination=2
 for side,c in enumerate([old,new]):
  for i in range(3):
   for j in range(4):
    address=strings[side]+(i*4+j)*64;c.uc.mem_write(address,names[i][j].encode()+b'\0');c.pointer(records[i][side]+([0x14,0x2c,0x44,0x5c][j] if side==0 else 8*j),address)
 def current(side):return [0,*[x[side] for x in records]].index(struct.unpack('<I' if side==0 else '<Q',[old,new][side].uc.mem_read(os+0xb4 if side==0 else ns,4 if side==0 else 8))[0])-1
 def mutate(side):
  if len(traces[side])-1==mutation:[old,new][side].pointer(os+0xb4 if side==0 else ns,0 if destination<0 else records[destination][side])
 def ret(c,value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def old_hook(uc,address,size,_):
  if address==0x31c49c:ret(old,struct.unpack('<I',uc.mem_read(old.reg(0)+0x20,4))[0])
  elif address==0x3d9fdc:traces[0].append(['lookup',string(old,struct.unpack('<I',uc.mem_read(old.reg(1),4))[0]).decode()]);ret(old,records[0][0])
  elif address==0x3109e0:
   text=string(old,old.reg(1)).decode();index=(old.reg(0)-records[0][0])//24;traces[0].append(['assign',index,text]);ret(old)
  elif address==0x30de54:ret(old,len(string(old,old.reg(0))))
  elif address==0x3d9358:ret(old,os+0x9c if destination<0 else records[destination][0]-0x28)
  elif address==0x37c514:traces[0].append(['call',string(old,old.reg(1)).decode(),current(0)]);mutate(0);ret(old)
  elif address==0x3dc798:
   traces[0].append(['default',current(0)]);mutate(0);ret(old)
 old.uc.hook_add(UC_HOOK_CODE,old_hook)
 def callback(uc,address,size,_):
  r=new.reg(2);kind,index,text,record,reserved=struct.unpack('<IIQQQ',uc.mem_read(r,32));assert reserved==0
  if kind==0:traces[1].append(['lookup',string(new,text).decode()]);new.pointer(new.reg(3),records[0][1])
  elif kind==1:traces[1].append(['assign',index,string(new,text).decode()])
  elif kind==2:traces[1].append(['call',string(new,text).decode(),current(1)]);mutate(1)
  else:assert kind==3;traces[1].append(['default',current(1)]);mutate(1)
  ret(new)
 new.imports[new.callback+32]='body_callback';new.body_callback=callback;new.uc.mem_write(services,struct.pack('<QQ',0,new.callback+32))
 rows=[]
 def reset(prior,flags):
  old.uc.mem_write(os,bytes(0xc4));old.pointer(os+0xb4,0 if prior<0 else records[prior][0]);old.pointer(os+0xb8,flags)
  new.uc.mem_write(ns,struct.pack('<QII',0 if prior<0 else records[prior][1],flags,0));traces[0].clear();traces[1].clear()
 # Original callback argument vector is112-byte Values. Invalid arity/type are
 # source no-effects, not native malformed rejection.
 for count in range(8):
  for invalid in range(-1,count):
   reset(-1,0);old.uc.mem_write(oa,bytes(16));old.pointer(oa+4,oa+32);old.pointer(oa+32,ov);old.pointer(oa+36,ov+112*count)
   for i in range(count):
    for side,c,base,stride in [(0,old,ov,112),(1,new,nv,40)]:
     text=strings[side]+1024+i*64;c.uc.mem_write(text,f'arg{i}'.encode()+b'\0')
     if side==0:c.uc.mem_write(base+i*stride,bytes(stride));c.pointer(base+i*stride+4,0 if i==invalid else 4);c.pointer(base+i*stride+0x20,text)
     else:c.uc.mem_write(base+i*stride,struct.pack('<IIfIQQQ',0 if i==invalid else 4,0,0,0,text,4,0))
   old.invoke(0x3da144,[oa,0,os]);assert new.invoke('dh2_character_script_states_register',[ns,nv,count,services])==1
   assert traces[0]==traces[1],(count,invalid,traces);rows.append(dict(op=0,count=count,invalid=invalid,trace=traces[0].copy()))
 for op,prior,flags,mutation,destination in itertools.product(range(1,7),range(-1,3),(0,1,0xffffffff),range(-1,4),range(-1,3)):
  reset(prior,flags)
  if op==1:
   old.pointer(oa+4,oa+32);old.pointer(oa+32,ov);old.pointer(oa+36,ov+112);old.pointer(ov+0x20,strings[0]);old.invoke(0x3d9710,[oa,0,os]);result=new.invoke('dh2_character_script_states_change',[ns,0 if destination<0 else records[destination][1],services])
  elif op==2:old.invoke(0x3dce64,[os]);result=new.invoke('dh2_character_script_states_update',[ns,services])
  else:
   index=op-3;old.invoke([0x3d8eb4,0x3d8ea0,0x3d8e8c,0x3d8e78][index],[os]);result=new.invoke('dh2_character_script_states_call',[ns,index,services])
  assert result==1 and traces[0]==traces[1] and current(0)==current(1),(op,prior,flags,mutation,destination,traces,current(0),current(1))
  rows.append(dict(op=op,prior=prior,flags=flags,mutation=mutation,destination=destination,trace=traces[0].copy(),final=current(0)))
 corpus=bytearray(b'SST1'+struct.pack('<I',len(rows)))
 for row in rows:
  corpus+=struct.pack('<7i',row['op'],row.get('count',0),row.get('invalid',-1),row.get('prior',-1),row.get('flags',0) if row.get('flags',0)<2**31 else -1,row.get('mutation',-1),row.get('destination',-1));corpus+=struct.pack('<iI',row.get('final',-1),len(row['trace']))
  for call in row['trace']:
   kind=call[0];text=call[1] if kind in ['lookup','call'] else call[2] if kind=='assign' else '';index=call[1] if kind=='assign' else call[2] if kind=='call' else call[1] if kind=='default' else 0;data=text.encode();corpus+=struct.pack('<IiI',['lookup','assign','call','default'].index(kind),index,len(data))+data
 (ref/'state-fixtures.bin').write_bytes(corpus)
 guards=0;new.uc.mem_write(ns,struct.pack('<QII',records[0][1],0,0));before=bytes(new.uc.mem_read(ns,16));traces[1].clear()
 for name,args in [('dh2_character_script_states_call',[0,0,services]),('dh2_character_script_states_call',[ns,4,services]),('dh2_character_script_states_update',[ns,0]),('dh2_character_script_states_register',[ns,0,2,services]),('dh2_character_script_states_change',[ns,records[0][1],0])]:
  assert new.invoke(name,args)&0xffffffff==0xffffffff and bytes(new.uc.mem_read(ns,16))==before and not traces[1];guards+=1
 report=dict(validation='PASS',scope=__doc__,original_sha256=manifest['original_sha256'],original_manifest_sha256=sha(ref/'original-functions.json'),optimized_arm64_library_sha256=sha(a.library),comparisons=len(rows),ordered_services=sum(len(r['trace']) for r in rows),native_atomic_rejections=guards,mismatches=0,gold_sha256=hashlib.sha256(corpus).hexdigest(),original_instructions_executed=True,source_sha256={str((ROOT/x).relative_to(REPO)):sha(ROOT/x) for x in ['character_script_states.hpp','character_script_states.cpp','tests/character_script_states_differential.py','tools/build_character_script_states_oracle.ps1']},full_Lua_VM_executed=False,original_STL_map_service_fixture=True,rows=rows)
 (ROOT/'reports/character-script-states-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',comparisons=len(rows),ordered_services=report['ordered_services'])))
if __name__=='__main__':main()
