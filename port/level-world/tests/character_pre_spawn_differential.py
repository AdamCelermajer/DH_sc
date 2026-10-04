"""Original complete PreSpawn Focus/Blur/Update/Event versus optimized ARM64.
Actual field stores/branches/string compare/table pointer reloads execute;
deeper Character/physics/animation/Spawn/debug dependencies are explicit services.
"""
import argparse,hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sys.path.insert(0,str(R/'tests'))
from character_script_selection_differential import Cpu
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 manifest=json.loads((R/'reference/character-state-methods/inventory.json').read_text());functions=[dict(original_symbol=x['symbol'],elf_address=x['source_function'],size=x['size']) for x in manifest['rows'] if x['state']==17];old=Cpu(a.engine,False,dict(functions=functions));new=Cpu(a.library,True,{'functions':[]})
 char=old.data+0x1000;sm=char+0x4fc;vt=old.data+0x4000;tables=[old.data+0x5000,old.data+0x6000];ai=old.data+0x7000;ostring=old.data+0x8000;target=old.data+0x9000
 state=new.data+0x1000;view=new.data+0x2000;ntables=[new.data+0x3000,new.data+0x4000];nword=new.data+0x5000;nstring=new.data+0x6000;services=new.data+0x7000
 owner=0xa123456789abcdef;identity=0xb123456789abcdef;context=0xc123456789abcdef;trace=[[],[]];P=None;indexes=[0,0];records=[];texts=['','is_interactive','Is_interactive','body','is_interactive_more'];comparisons=0
 def w(c,address):return struct.unpack('<I',c.uc.mem_read(address,4))[0]
 def put(c,address,value):c.uc.mem_write(address,struct.pack('<I',value&0xffffffff))
 def ret(value=0):old.put(0,value&0xffffffff);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def text(c,address):
  v=bytearray()
  for i in range(256):
   b=c.uc.mem_read(address+i,1)[0]
   if not b:return bytes(v)
   v.append(b)
  raise AssertionError('fixture string unterminated')
 base=(0x3c689c+8+w(old,0x3c6a00))&0xffffffff;table_cell=w(old,base+w(old,0x3c6a10));assert_base=(0x3c2828+8+w(old,0x3c292c))&0xffffffff;assert_cell=w(old,assert_base+w(old,0x3c2934))
 def mutate(which,kind):
  c=new if which else old
  if kind==0:
   indexes[which]+=1
   if P[12]==1:
    if which:new.pointer(view+16,ntables[indexes[which]%2])
    else:put(old,table_cell,tables[indexes[which]%2])
  if kind==1 and P[12]==2:
   # Source has already captured fallback base before querying constants.
   for table in ntables if which else tables:put(c,table+32*4,999)
  if kind==5 and P[12]==3:
   if which:put(new,view+28,0)
   else:old.uc.mem_write(char+0x13e4,b'\0')
  if kind==12 and P[12]==4:
   if which:put(new,view+32,3)
   else:put(old,char+0x400,3)
 def record(which,kind,a0=0,a1=0,payload=0):trace[which].append(struct.pack('<4IQQ',kind,a0&0xffffffff,a1&0xffffffff,0,owner,payload));mutate(which,kind)
 def old_hook(uc,address,size,unused):
  if address in (0x337888,0x337a88,0x318254):ret();return
  if address==0x3140ec:uc.mem_write(old.reg(0),bytes(24));ret();return
  if address==0x3a3228:record(0,0);ret(P[10] if indexes[0]==1 else P[11]);return
  if address==0x4c4bdc:assert text(old,old.reg(1))==b'AnimStancedAnim' and text(old,old.reg(2))==b'SL__LIST_IPHONE';record(0,1);ret(P[8]);return
  if address==0x3a53e0:record(0,2);ret(P[9]);return
  if address==0x3cacb0:record(0,3,old.reg(1));put(old,char+0x4e8,old.reg(1));ret();return
  if address==0x3c93fc:record(0,4,old.reg(1));ret();return
  if address==0x394bf8:assert old.reg(1)==old.reg(2)==0;record(0,5);put(old,char+0x2dc,0);ret();return
  if address in (0x3949b0,0x3a59ac,0x394a3c,0x3b4088):
   kind={0x3949b0:7,0x3a59ac:8,0x394a3c:9,0x3b4088:10}[address];record(0,kind);ret();return
  if address==0x30e31c:ret(0 if text(old,old.reg(0))==text(old,old.reg(1)) else 1);return
  if address==0x3ad2e4:assert old.reg(1)==9 and old.reg(2)==target and old.reg(3)==17;record(0,11,9,17,identity);put(old,w(old,uc.reg_read(old.sp)),P[14]);ret(P[13]);return
  if address==0x3c2734:assert old.reg(1)==1 and old.reg(2)==0;record(0,12,1);ret();return
  if address==0x3c28b4:record(0,13);return
  if address==0x30e004:record(0,14,w(old,uc.reg_read(old.sp)));ret();return
 def enable(uc,address,size,unused):assert old.reg(0)==char;record(0,6,old.reg(1));ret()
 old.imports[old.callback+32]='body_callback';old.body_callback=enable;old.pointer(vt+0x40,old.callback+32);old.uc.hook_add(UC_HOOK_CODE,old_hook)
 def native(uc,address,size,unused):
  assert new.reg(0)==context and new.reg(1)==view
  kind,arg0,arg1,arg2,subject,payload=struct.unpack('<4IQQ',uc.mem_read(new.reg(2),32));assert arg2==0 and subject==owner;record(1,kind,arg0,arg1,payload)
  response=0
  if kind==0:response=P[10] if indexes[1]==1 else P[11]
  elif kind==1:response=P[8]
  elif kind==2:response=P[9]
  elif kind==3:put(new,state+52,arg0)
  elif kind==5:put(new,state+44,0)
  elif kind==11:response=P[13];put(new,new.reg(3)+4,P[14])
  elif kind==13:response=P[15]
  put(new,new.reg(3),response);new.put(0,0);uc.reg_write(new.pc,uc.reg_read(new.lr))
 new.imports[new.callback+32]='body_callback';new.body_callback=native;new.uc.mem_write(services,struct.pack('<QQ',context,new.callback+32))
 def compare(params):
  nonlocal P,comparisons
  P=params;indexes[:]=[0,0];trace[0].clear();trace[1].clear();old.uc.mem_write(char,bytes(0x2000));put(old,char,vt);put(old,char+0x520,P[2]);put(old,char+0x4e8,0xffffffff);put(old,char+0x2dc,target if P[16] else 0);old.uc.mem_write(char+0x13e4,bytes([P[3]]));put(old,char+0x400,P[4]);put(old,char+0x3fc,ai);put(old,ai+0x24,P[5]);put(old,sm+4,char);put(old,assert_cell,P[15]);put(old,table_cell,tables[0]);put(new,nword,P[5]);new.uc.mem_write(state,struct.pack('<i13I',17,P[2],0,0,0,0,0,0,0,0,0,P[16],0xffffffff,0xffffffff));new.uc.mem_write(view,struct.pack('<QQQ4IQ',state,owner,ntables[0],2,P[3],P[4],0,nword))
  for c,rows in ((old,tables),(new,ntables)):
   for k,table in enumerate(rows):
    c.uc.mem_write(table,bytes(320))
    for i in range(2):put(c,table+i*160+25*4,P[6]+(k*10+i if P[6]!=-1 else 0));put(c,table+i*160+32*4,P[7]+k*10+i)
  payload=target;npayload=identity
  if P[1]==0x28:raw=texts[P[17]].encode()+b'\0';old.uc.mem_write(ostring,raw);new.uc.mem_write(nstring,raw);payload=ostring;npayload=nstring
  fn=(0x3c688c,0x3c67d4,0x3c0070,0x3c2810)[P[0]];old.invoke(fn,[0x12345678,17,char,sm,P[1],payload]);result=new.invoke('dh2_character_pre_spawn_body',[view,P[0],P[1],npayload,services]);assert result==1
  expected=bytearray(struct.pack('<i13I',17,w(old,char+0x520),0,0,0,0,0,0,0,0,0,int(bool(w(old,char+0x2dc))),0xffffffff,w(old,char+0x4e8)));observed=bytes(new.uc.mem_read(state,56));assert bytes(expected)==observed and w(old,ai+0x24)==w(new,nword) and trace[0]==trace[1],(P,trace,bytes(expected).hex(),observed.hex());comparisons+=1
  records.append(struct.pack('<20I',*(x&0xffffffff for x in P))+expected+struct.pack('<3I',w(old,ai+0x24),result,len(trace[0]))+b''.join(trace[0]))
 def params(op=0,event=0,flags=0xdeadbeef,stay=0,kind=0,word=77,clip=300,baseclip=800,mask=1,stance=4,mode=0,accept=1,next_=1,policy=0,body=1,string=0):return [op,event,flags,stay,kind,word,clip,baseclip,mask,stance,0,1,mode,accept,next_,policy,body,string,0,0]
 for clip,stay,mask,stance,mode in itertools.product((-1,300),(0,1,255),(0,1,0xffffffff),(-2,0,4,0x7fffffff),range(5)):compare(params(clip=clip,stay=stay,mask=mask,stance=stance,mode=mode))
 for op,flag in itertools.product((1,2),(0,0x1300,0x2000,0xffffffff)):compare(params(op=op,flags=flag))
 for string,flag in itertools.product(range(len(texts)),(0,0x1300,0x2000,0xffffffff)):compare(params(op=3,event=0x28,flags=flag,string=string))
 for accept,next_,policy,kind,mode in itertools.product((0,1),(-1,0,1,17),(0,1,3),(0,3,4),(0,4)):compare(params(op=3,event=9,accept=accept,next_=next_,policy=policy,kind=kind,mode=mode))
 for event in (0,1,0x22,0x23,0x2a,0xffffffff):compare(params(op=3,event=event))
 gold=R/'reference/character-pre-spawn/pre-spawn-fixtures.bin';gold.parent.mkdir(parents=True,exist_ok=True);gold.write_bytes(b'PSP1'+struct.pack('<I',len(records))+b''.join(records));paths=('character_pre_spawn.hpp','character_pre_spawn.cpp','character_state_empty.hpp','character_state_empty.cpp','reference/character-state-methods/native-methods.inc','tests/character_pre_spawn_differential.py');report=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),library_sha256=sha(a.library),inventory_sha256=sha(R/'reference/character-state-methods/inventory.json'),reference_sha256=sha(gold),source_sha256={'port/level-world/'+x:sha(R/x) for x in paths},comparisons=comparisons,ordered_requests=sum(struct.unpack('<I',r[144:148])[0] for r in records),mismatches=0,table_pointer_and_stay_kind_reload_mutations=True,full_physics_spawn_revive_backends=False,packaged_APK=False);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
