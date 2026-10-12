"""Complete original Spawn1 method instructions versus optimized ARM64; genuine empty fade/sync bodies execute, deeper services explicit."""
import argparse,hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sys.path.insert(0,str(R/'tests'))
from character_script_selection_differential import Cpu,string
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser()
 for n in ('engine','library','output'):p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();assert sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 manifest=R/'reference/character-spawn-body/original-functions.json';old=Cpu(a.engine,False,json.loads(manifest.read_text()));new=Cpu(a.library,True,{'functions':[]})
 char=old.data+0x1000;sm=char+0x4fc;tables=[old.data+0x4000,old.data+0x5000];visual=old.data+0x6000;body=old.data+0x7000;ostring=old.data+0x8000;other=old.data+0xa000
 state=new.data+0x1000;view=new.data+0x2000;ntables=[new.data+0x3000,new.data+0x4000];nstring=new.data+0x5000;svc=new.data+0x6000
 owner=0xa123456789abcdef;v_identity=0xb123456789abcdef;context=0xc123456789abcdef;traces=[[],[]];P=None;target=last=0;records=[];texts=['','is_interactive','Is_interactive','body','is_interactive_more']
 def w(c,addr):return struct.unpack('<I',c.uc.mem_read(addr,4))[0]
 def put(c,addr,value):c.uc.mem_write(addr,struct.pack('<I',value&0xffffffff))
 def ret(c,value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 base=(0x3c3604+w(old,0x3c3770))&0xffffffff;table_cell=w(old,base+w(old,0x3c3784))
 def mutate(which,kind,argument):
  c=new if which else old
  if P[6]==1 and kind==2:
   if which:c.pointer(view+16,ntables[1])
   else:put(c,table_cell,tables[1])
  if P[6]==2 and kind==3:
   for table in ntables if which else tables:put(c,table+P[7]*160+128,999)
  if P[6]==3 and kind==5:put(c,state+4 if which else char+0x520,0x12340000)
  if P[6]==4 and kind==8:
   put(c,state+4 if which else char+0x520,0x80004001)
   if which:c.pointer(view+24,0 if P[5] else v_identity);put(c,view+36,0x80000000)
   else:put(c,char+0x2d8,0 if P[5] else visual);put(c,char+0x1440,0x80000000)
  if P[6]==5 and kind==1 and argument==0:
   put(c,state+4 if which else char+0x520,0x2000)
   if which:c.pointer(view+16,ntables[1])
   else:put(c,table_cell,tables[1])
  if P[6]==6 and kind==0:
   if which:c.pointer(view+8,0xd123456789abcdef)
   else:put(c,sm+4,other)
 def record(which,kind,arg=0,payload=0):traces[which].append(struct.pack('<4IQQ',kind,arg&0xffffffff,0,0,owner,payload));mutate(which,kind,arg)
 def original(uc,address,size,unused):
  if address==0x337888:record(0,0);ret(old);return
  if address==0x3140ec:uc.mem_write(old.reg(0),struct.pack('<I',old.reg(1))+bytes(20));ret(old);return
  if address==0x337a88:
   name=string(old,w(old,old.reg(1)));assert name in (b'isTracingCharState',b'isTracingCSSpawn');record(0,1,int(name==b'isTracingCSSpawn'));ret(old,0xffffffff);return
  if address==0x318254:ret(old);return
  if address==0x3a3228:record(0,2);ret(old,P[7]);return
  if address==0x4c4bdc:assert string(old,old.reg(1))==b'AnimStancedAnim' and string(old,old.reg(2))==b'SL__LIST_IPHONE';record(0,3);ret(old,P[3]);return
  if address==0x3a53e0:record(0,4);ret(old,P[4]&0xffffffff);return
  if address==0x3cacb0:record(0,5,old.reg(1));put(old,char+0x4e8,old.reg(1));ret(old);return
  if address==0x3d6890:assert old.reg(0)==char+0x3c8 and old.reg(1)==old.reg(2)==0;record(0,6);put(old,char+0x404,0);put(old,char+0x408,0);ret(old);return
  if address==0x3d49c4:assert old.reg(0)==char+0x3c8;record(0,7);return # Actual source copy body runs.
  if address==0x3bc6b8:record(0,8);ret(old);return
  if address==0x470ce4:assert old.reg(0)==visual;record(0,9,old.reg(1),v_identity);return # Actual original bx-lr.
  if address==0x3b4088:record(0,10);put(old,char+0x2dc,body);ret(old);return
  if address==0x30e31c:ret(old,0 if string(old,old.reg(0))==string(old,old.reg(1)) else 1);return
 old.uc.hook_add(UC_HOOK_CODE,original)
 def native(uc,address,size,unused):
  nonlocal target,last
  assert new.reg(0)==context and new.reg(1)==view
  kind,arg,a1,a2,subject,payload=struct.unpack('<4IQQ',uc.mem_read(new.reg(2),32));assert a1==a2==0 and subject==owner
  record(1,kind,arg,payload);response=0
  if kind==1:response=0xffffffff
  elif kind==2:response=P[7]
  elif kind==3:response=P[3]
  elif kind==4:response=P[4]&0xffffffff
  elif kind==5:put(new,state+52,arg)
  elif kind==6:target=0
  elif kind==7:last=target
  elif kind==10:put(new,state+44,1)
  put(new,new.reg(3),response);ret(new)
 new.imports[new.callback+32]='body_callback';new.body_callback=native;new.uc.mem_write(svc,struct.pack('<QQ',context,new.callback+32))
 def compare(params):
  nonlocal P,target,last
  P=params;traces[0].clear();traces[1].clear();target=P[10];last=P[11]
  old.uc.mem_write(char,bytes(0x2000));put(old,sm+4,char);put(old,char+0x520,P[2]);put(old,char+0x2d8,visual if P[5] else 0);put(old,char+0x1440,P[9]);put(old,char+0x2dc,body if P[12] else 0);put(old,char+0x4e8,0xffffffff);put(old,char+0x408,body if P[10] else 0);put(old,char+0x40c,body if P[11] else 0);put(old,table_cell,tables[0])
  initial=struct.pack('<i13I',1,P[2],0,0,0,0,0,0,0,0,0,P[12],0xffffffff,0xffffffff);new.uc.mem_write(state,initial);new.uc.mem_write(view,struct.pack('<QQQQ4I',state,owner,ntables[0],v_identity if P[5] else 0,2,P[9],0,0))
  for c,rows in ((old,tables),(new,ntables)):
   for k,table in enumerate(rows):
    c.uc.mem_write(table,bytes(320))
    for i in range(2):put(c,table+i*160+128,P[8]+k*100+i)
  old_payload=body;payload=v_identity
  if P[13]==0x28:raw=texts[P[14]].encode()+b'\0';old.uc.mem_write(ostring,raw);new.uc.mem_write(nstring,raw);old_payload=ostring;payload=nstring
  fn=(0x3c35ec,0x3c2f7c,0x3bfff0,0x3c0b04)[P[0]];old.invoke(fn,(0x12345678,1,char,sm,(P[13] if P[0]==3 else P[1])&0xffffffff,old_payload));assert new.invoke('dh2_character_spawn_body',(view,P[0],P[1]&0xffffffff,P[13],payload,svc))==1
  expected=struct.pack('<i13I',1,w(old,char+0x520),0,0,0,0,0,0,0,0,0,int(bool(w(old,char+0x2dc))),0xffffffff,w(old,char+0x4e8));projection=(int(bool(w(old,char+0x2d8))),w(old,char+0x1440),int(bool(w(old,char+0x408))),int(bool(w(old,char+0x40c))))
  got=(int(bool(struct.unpack('<Q',new.uc.mem_read(view+24,8))[0])),w(new,view+36),target,last)
  assert expected==bytes(new.uc.mem_read(state,56)) and projection==got and traces[0]==traces[1],(P,projection,got,traces)
  records.append(struct.pack('<16I',*(x&0xffffffff for x in P))+expected+struct.pack('<5I',*projection,len(traces[0]))+b''.join(traces[0]))
 def params(op=0,previous=17,flags=0x2000,mask=1,stance=4,visual_=1,mode=0,index=0,base_=800,fade=0x3f800000,event=0,string_=0):return [op,previous,flags,mask,stance,visual_,mode,index,base_,fade,1,1,0,event,string_,0]
 for previous,flags,mask,stance,visual_,mode,index in itertools.product((-1,17,1,3),(0,0x2000,0x1300,0xffffffff),(0,1,0xffffffff),(-2,0,4,0x7fffffff),(0,1),range(7),(0,1)):compare(params(previous=previous,flags=flags,mask=mask,stance=stance,visual_=visual_,mode=mode,index=index,fade=(0x7fc12345 if mode==2 else 0x3f800000)))
 for op,flags,mode in itertools.product((1,2),(0,0x2000,0x1300,0xffffffff),range(7)):compare(params(op=op,flags=flags,mode=mode))
 for string_,flags in itertools.product(range(5),(0,0x2000,0x1300,0xffffffff)):compare(params(op=3,event=0x28,string_=string_,flags=flags))
 for event in (0,9,0x22,0x23,0x2d,0xffffffff):compare(params(op=3,event=event))
 gold=R/'reference/character-spawn-body/spawn-body-fixtures.bin';gold.write_bytes(b'SPB1'+struct.pack('<I',len(records))+b''.join(records));paths=('character_spawn_body.hpp','character_spawn_body.cpp','character_state.hpp','character_state_empty.hpp','character_state_empty.cpp','reference/character-state-methods/native-methods.inc','tests/character_spawn_body_differential.py')
 data=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),library_sha256=sha(a.library),reference_sha256=sha(gold),source_sha256={'port/level-world/'+x:sha(R/x) for x in paths},original_capture_sha256=sha(manifest),comparisons=len(records),ordered_requests=sum(struct.unpack_from('<I',r,136)[0] for r in records),mismatches=0,service_mutation_modes=7,all_physics_AI_CancelSneaking_backends=False,APK=False)
 a.output.write_text(json.dumps(data,indent=2)+'\n');print(json.dumps(data))
if __name__=='__main__':main()
