"""Actual ARM32 state selection/event walks vs O2 ARM64 owned-registry kernel.
StateInfo storage is a supplied std::map allocation fixture; source tree walks,
blur/focus/event/predicate dispatch and current/owner reloads really execute.
All behavior bodies, predicates and logger construction are explicit services.
"""
import argparse,hashlib,json,itertools,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'tests'))
from character_script_selection_differential import Cpu
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def signed(x):return struct.unpack('<i',struct.pack('<I',x&0xffffffff))[0]
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
 assert sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 plans=json.loads((R/'reference/character-monster-state-ownership/registration-plan.json').read_text())['states']
 old=Cpu(a.engine,False,{'functions':[]});new=Cpu(a.library,True,{'functions':[]})
 ob=old.data+0x1000;om=ob+0x4fc;ob2=old.data+0x4000;nodes=old.data+0x10000;events=old.data+0x20000
 ns=new.data+0x1000;nf=new.data+0x2000;nm=new.data+0x3000;infos=new.data+0x4000;ne=new.data+0x6000;nv=new.data+0x8000
 context=0xabcdef0123456789;owners=(0x123456789abcdef0,0xfedcba9876543210);physical=0xabcdef1234567890;payload=0xa123456789abcdef
 ow=lambda at:struct.unpack('<I',old.uc.mem_read(at,4))[0]
 oi=lambda at:signed(ow(at))
 ret=lambda v=0:(old.put(0,v&0xffffffff),old.uc.reg_write(old.pc,old.uc.reg_read(old.lr)))
 calls=[[],[]];mutation=0;accept=1;override=-1;evt=0;initial=0
 def tree(cpu,header,rows):
  def build(rows,parent):
   if not rows:return 0
   mid=len(rows)//2;ptr,key=rows[mid];cpu.pointer(ptr+4,parent);cpu.pointer(ptr+8,build(rows[:mid],ptr));cpu.pointer(ptr+12,build(rows[mid+1:],ptr));cpu.pointer(ptr+16,key&0xffffffff);return ptr
  cpu.pointer(header+4,build(rows,header));cpu.pointer(header+8,rows[0][0] if rows else header);cpu.pointer(header+12,rows[-1][0] if rows else header)
 old.uc.mem_write(ob,bytes(0x2000));old.uc.mem_write(ob2,bytes(0x2000));old.pointer(om+4,ob)
 erows=[];off=0
 for x in plans:
  i=x['id'];node=nodes+i*128;old.uc.mem_write(node,bytes(128));old.pointer(node+20,i);old.pointer(node+24,x['singleton']);old.pointer(x['singleton'],x['vtable'])
  es=sorted(x['events'],key=lambda e:signed(e['event']));rows=[];neptr=ne+off*16
  for j,e in enumerate(es):
   ptr=events+(off+j)*40;old.uc.mem_write(ptr,bytes(40));old.uc.mem_write(ptr+20,struct.pack('<IIi',e['predicate'],e['adjustment'],signed(e['target'])));rows.append((ptr,signed(e['event'])))
   new.uc.mem_write(ne+(off+j)*16,struct.pack('<iiII',signed(e['event']),signed(e['target']),e['predicate'],e['adjustment']))
  tree(old,node+28,rows);off+=len(es)
  new.uc.mem_write(infos+i*40,struct.pack('<i5IQII',i,x['singleton'],x['on_focus'],x['on_blur'],x['on_update'],x['on_event'],neptr,len(es),0))
 tree(old,om+8,[(nodes+i*128,i) for i in range(20)])
 new.uc.mem_write(nv,struct.pack('<QQ',context,new.callback+32))
 # Actual Level iterator owns one Character fixture; preset provider is explicit.
 app=old.data+0x8000;manager=app+0x400;listnode=manager+0x100
 base=(0x3effa4+8+ow(0x3f0010))&0xffffffff
 old.pointer(base+ow(0x3f0014),app);old.pointer(app+0x38,manager);old.pointer(manager+0x60,listnode);old.pointer(listnode,manager+0x60);old.pointer(listnode+8,ob)
 def old_id():return oi(ow(om+0x20)) if ow(om+0x20) else -1
 def old_owner():return 0 if ow(om+4)==ob else 1
 def set_old(id):old.pointer(om+0x20,nodes+id*128+20 if id>=0 else 0)
 def set_new(id):new.uc.mem_write(nm+20,struct.pack('<i',id));new.uc.mem_write(nf+16,struct.pack('<I',int(id>=0)));new.uc.mem_write(ns,struct.pack('<i',id))
 def alter(which,op):
  if mutation==1 and op==1:
   if which: new.uc.mem_write(nf+8,struct.pack('<Q',owners[1]))
   else:old.pointer(om+4,ob2)
  if mutation in (2,3) and op==(3 if mutation==2 else 4):
   id=5 if initial!=5 else 3
   if which:set_new(id)
   else:set_old(id)
 def record(which,op,fn,state,other,event,adjust,owner,value):
  row=[op,fn,signed(state),signed(other),event&0xffffffff,adjust,owner,value&0xffffffff];calls[which].append(row);alter(which,op)
 methods=set(x[k] for x in plans for k in ('on_focus','on_blur','on_event'))
 predicates={e['predicate'] for x in plans for e in x['events'] if e['predicate']}
 def old_hook(uc,address,size,unused):
  lr=uc.reg_read(old.lr);sp=uc.reg_read(old.sp)
  if address==0x3a5784:ret(initial);return
  if address in methods and lr in (0x3c198c,0x3c19fc,0x3c5730):
   op={0x3c198c:1,0x3c19fc:2,0x3c5730:3}[lr];state=old.reg(1);owner=0 if old.reg(2)==ob else 1
   other=ow(sp) if op in (1,2) else 0;event=ow(sp+4) if op==2 else ow(sp) if op==3 else 0;value=ow(sp+8) if op==2 else ow(sp+4) if op==3 else 0
   record(0,op,address,state,other,event,0,owner,value);ret();return
  if address in predicates:
   target=ow(ow(sp));record(0,4,address,old.reg(3),target,old.reg(1),0,old_owner(),old.reg(2))
   if override!=-1:old.pointer(ow(sp),override)
   ret(accept);return
  if address==0x3a4d5c:
   record(0,5,address,old.reg(2),0,old.reg(1),0,0 if old.reg(0)==ob else 1,old.reg(2));ret();return
  if address==0x46eb20:record(0,6,address,old_id(),0,0x30,0,2,0);ret();return
  if address in (0x337888,0x318254):
   record(0,7 if address==0x337888 else 8,address,0,0,0,0,old_owner(),0);ret();return
  if address==0x3140ec:uc.mem_write(old.reg(0),bytes(24));ret();return
  if address==0x337a88:ret();return
 def native_hook(uc,address,size,unused):
  assert new.reg(0)==context and new.reg(1)==nm
  op,fn,state,other,event,adjust,who,value,z0,z1=struct.unpack('<IIiiIIQQII',uc.mem_read(new.reg(2),48));assert not z0 and not z1
  owner=2 if who==physical else owners.index(who);record(1,op,fn,state,other,event,adjust,owner,value)
  if op==4:uc.mem_write(new.reg(3),struct.pack('<iI',override if override!=-1 else other,accept))
  new.put(0,0);uc.reg_write(new.pc,uc.reg_read(new.lr))
 old.uc.hook_add(UC_HOOK_CODE,old_hook);new.imports[new.callback+32]='body_callback';new.body_callback=native_hook
 records=[]
 def compare(operation,id,next,event,preset=0,change=0,predicate=1,chosen=-1,body=1):
  nonlocal mutation,accept,override,evt,initial
  mutation=change;accept=predicate;override=chosen;evt=event;initial=preset if operation==2 else id
  old.pointer(om+4,ob);set_old(id);old.pointer(om+0x60,0xfffffff0);old.pointer(om+0x2c,7);old.uc.mem_write(om+0x3c,bytes([1]));old.pointer(ob+0x2dc,old.data+0x9000 if body else 0)
  new.uc.mem_write(ns,struct.pack('<i13I',id,0,7,0,0xfffffff0,0,1,0,0,0,0,body,0xffffffff,0xffffffff));new.uc.mem_write(nf,struct.pack('<QQII',ns,owners[0],int(id>=0),0));new.uc.mem_write(nm,struct.pack('<QQIiQQ',nf,infos,20,id,physical if body else 0,0));calls[0].clear();calls[1].clear()
  if operation==0:old.invoke(0x3c1938,[om,next,event,0x89abcdef]);result=new.invoke('dh2_character_state_owner_transition',[nm,next,event,payload,nv])
  elif operation==1:old.invoke(0x3c5684,[om,event,0x89abcdef]);result=new.invoke('dh2_character_state_owner_event',[nm,event,payload,nv])
  else:old.invoke(0x3eff98,[old.data+0xa000]);result=new.invoke('dh2_character_state_owner_initialize_level',[nm,preset,nv])
  expect=[old_id(),ow(om+0x60),ow(om+0x2c),old.uc.mem_read(om+0x3c,1)[0],old_owner()]
  words=struct.unpack('<i13I',new.uc.mem_read(ns,56));actual=[words[0],words[4],words[2],words[6],owners.index(struct.unpack('<Q',new.uc.mem_read(nf+8,8))[0])]
  # payload identities are different representations of the same explicit input.
  for cs in calls:
   for c in cs:
    if c[0] in (2,3,4) and c[7] in (0x89abcdef,payload&0xffffffff):c[7]=0x89abcdef
  assert expect==actual and calls[0]==calls[1],(operation,id,next,event,change,predicate,chosen,expect,actual,calls)
  assert result in (0,1),result
  records.append(dict(input=[operation,id,next,event,preset,change,predicate,chosen,body],final=expect,calls=calls[0].copy(),result=result))
 for id,next in itertools.product(range(-1,20),(-3,-1,0,3,4,5,12,17,19,20)):compare(0,id,next,0xc354,change=int(id%3==0))
 for id in range(20):
  for event in sorted({e['event'] for e in plans[id]['events']}|{0x2a,0x2b,0x2c,0x30,9,0xffffffff}):
   for accepted in (0,1):compare(1,id,0,signed(event),predicate=accepted)
   for change in (2,3):compare(1,id,0,signed(event),change=change,chosen=4)
 for preset,id in itertools.product((-1,0,3,4,17,19),(-1,0,3,5,17)):compare(2,id,0,-1,preset)
 # Actual original registration order compares exported static source plan.
 n=new.invoke('dh2_character_state_owner_registration_count',[]);assert n==109
 rows=[(x['id'],e) for x in plans for e in x['events']]
 for i,(id,e) in enumerate(rows):
  assert new.invoke('dh2_character_state_owner_registration',[i,new.data+0x9000,new.data+0x9010])==1
  assert signed(struct.unpack('<I',new.uc.mem_read(new.data+0x9000,4))[0])==id
  assert bytes(new.uc.mem_read(new.data+0x9010,16))==struct.pack('<iiII',signed(e['event']),signed(e['target']),e['predicate'],e['adjustment'])
 gold=a.output/'state-owner-gold.json';gold.write_text(json.dumps(dict(records=records,registrations=rows),indent=2)+'\n')
 binary=bytearray(b'SBO1'+struct.pack('<I',len(records)))
 for x in records:
  binary+=struct.pack('<9I',*(v&0xffffffff for v in x['input']))+struct.pack('<5I',*(v&0xffffffff for v in x['final']))+struct.pack('<II',x['result'],len(x['calls']))
  for call in x['calls']:binary+=struct.pack('<8I',*(v&0xffffffff for v in call))
 (a.output/'state-owner-fixtures.bin').write_bytes(binary)
 sources=['character_state_owner.hpp','character_state_owner.cpp','character_state_owner_data.inc','tests/character_state_owner_differential.py']
 report=dict(validation='PASS',original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),source_sha256={str((R/x).relative_to(R.parents[1])).replace('\\','/'):sha(R/x) for x in sources},reference_sha256=sha(gold),binary_reference_sha256=hashlib.sha256(binary).hexdigest(),registration_plan_sha256=sha(R/'reference/character-monster-state-ownership/registration-plan.json'),comparisons=len(records),ordered_callbacks=sum(len(x['calls']) for x in records),on_init_registrations_compared=n,original_instructions_executed=True,optimized_arm64_instructions_executed=True,mismatches=0,scope=__doc__,allocation_boundary='Supplied stable StateInfo/std::map node storage, original map walks execute; original factories/OnInit separately execute in plan_probe.py; original singleton vtable initialized from actual ELF vtable metadata.',behavior_bodies_reconstructed=False,full_AI=False)
 (a.output/'differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
