import sys,struct,json,hashlib,random
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[4];P=R/'.local-inputs/actor-playback-events-discovery'
sys.path.insert(0,str(R/'port/engine-animation/tests'))
from events_differential import Cpu,install,parse,i32,u32
engine=R/'.local-inputs/libDungeonHunter2.so'
manifest=json.loads((R/'port/level-world/reference/actor-playback-events/original-functions.json').read_text())
old=Cpu(engine,False,manifest);new=Cpu(P/'oracle.so',True,{'functions':[]})
captured=[]
def observe(uc,address,size,data):
 if address==0x3a4d5c:
  captured.append((old.reg(1),i32(struct.unpack('<I',uc.mem_read(old.data+0x8000+0x58,4))[0]),old.string(old.reg(2))))
  uc.reg_write(old.pc,uc.reg_read(old.lr))
old.uc.hook_add(UC_HOOK_CODE,observe,begin=0x3a4d5c,end=0x3a4d5c)
rng=random.Random(314026);lags=[-2147483648,-1,0,1,2147483647]+[rng.randint(-2147483648,2147483647) for _ in range(395)]
for lag in lags:
 captured.clear();old.uc.mem_write(old.data+0x8000,bytes(0x80));old.pointer(old.data+0x8004,old.data+0x9000)
 old.uc.mem_write(old.data+0xa000,b'attack_mainhand\0');old.uc.mem_write(old.data+0x9000,struct.pack('<iI',lag,old.data+0xa000))
 old.invoke(0x3c9984,[old.data+0x9000,old.data+0x8000]);assert captured==[(0x28,lag,'attack_mainhand')]
 new.uc.mem_write(new.data+0xa000,b'attack_mainhand\0');new.uc.mem_write(new.data+0x9000,struct.pack('<iIQ',lag,0,new.data+0xa000))
 assert i32(new.invoke('dh2_actor_event_handoff',[new.data+0x8000,new.data+0x8100,new.data+0x9000]))==0
 event,actual,payload=struct.unpack('<IiQ',new.uc.mem_read(new.data+0x8000,16));assert (event,actual,new.string(payload))==captured[0]
 assert struct.unpack('<i',new.uc.mem_read(new.data+0x8100,4))[0]==lag
ids={955:'prince_1hand_combo_01',956:'prince_1hand_combo_01_moving',957:'prince_1hand_combo_01_to_idle',958:'prince_1hand_combo_02',959:'prince_1hand_combo_02_moving',960:'prince_1hand_combo_02_to_idle',961:'prince_1hand_combo_03',962:'prince_1hand_combo_03_moving',963:'prince_1hand_combo_03_to_idle',967:'prince_1hand_pre_combo_01',969:'prince_1hand_pre_combo_02',971:'prince_1hand_pre_combo_03',1023:'prince_died'}
assets=R/'port/android-native/app/src/main/assets/animations';windows=[];authored=[];callbacks=0
for clip,name in ids.items():
 file=assets/(name+'.bdae')
 if not file.exists():
  assert clip==1023;file=assets/'prince_dying_01.bdae'
 parsed=parse(file)
 if not parsed:parsed=(1,[],[])
 kind,keys,groups=parsed;authored.append({'id':clip,'file':file.name,'sha256':hashlib.sha256(file.read_bytes()).hexdigest(),'keys':keys,'names':groups})
 om,ot=install(old,kind,keys,groups);nm,nt=install(new,kind,keys,groups)
 probes=[-1,0,1,100,266,267,500,799,800,1000,2000,4000]
 for key in keys:
  time=int(struct.unpack('<f',struct.pack('<f',key*33.33333206176758))[0]) if kind!=4 else key
  probes.extend([time-1,time,time+1])
 for j in range(100):
  previous,current=rng.choice(probes),rng.choice(probes);last=rng.randrange(-1,len(keys)+1)
  old.uc.mem_write(om+16,struct.pack('<i',last));new.uc.mem_write(nm,struct.pack('<i',last));old.events=[];new.events=[]
  old.invoke(0x60ebe0,[om,previous,current,0,4000]);assert new.invoke('dh2_events_update',[nt,nm,previous,current,0,4000,new.callback,new.data+0x7000])==1
  after=struct.unpack('<i',old.uc.mem_read(om+16,4))[0];assert after==struct.unpack('<i',new.uc.mem_read(nm,4))[0] and old.events==new.events
  row=struct.pack('<8i',clip,previous,current,0,4000,last,after,len(old.events))
  for lag,name in old.events:
   raw=name.encode()+b'\0';row+=struct.pack('<iI',lag,len(raw))+raw
  windows.append(row);callbacks+=len(old.events)
# Real ANIM_Swap executes its metadata loop against a three-level caller table.
swap=Cpu(engine,False,{'functions':[]});calls=[]
def swap_hook(uc,address,size,data):
 if address in (0x3cacb0,0x3c93fc):
  calls.append((address,swap.reg(1)));uc.reg_write(swap.pc,uc.reg_read(swap.lr))
swap.uc.hook_add(UC_HOOK_CODE,swap_hook,begin=0x3c93fc,end=0x3cacb0)
def w(a):return struct.unpack('<I',swap.uc.mem_read(a,4))[0]
base=0x3cacdc+8+w(0x3caf08);offset=w(0x3caf10);ptr=swap.data+0x1000;table=swap.data+0x2000;steps=swap.data+0x4000
swap.pointer(base+offset,ptr);swap.pointer(ptr,table)
for seq in range(9):
 swap.uc.mem_write(table+20*seq,struct.pack('<5I',0,1,3,steps+seq*168,0))
 for step in range(3):
  record=steps+seq*168+step*56;swap.uc.mem_write(record,bytes(56));swap.pointer(record+8,seq+1 if seq%3<2 else 955);swap.pointer(record+0x28,int(seq%3<2))
swaps=[]
for root in (0,3,6):
 for desired in (-1,0,3,6):
  for prior in (-1,0,3,6):
   for depth in range(3):
    anim=swap.data+0x10000;swap.uc.mem_write(anim,bytes(0x80));swap.pointer(anim+0x2c,depth);swap.pointer(anim+0x40,0x3fa66666)
    loops=[-1,2,0];indices=[2,1,0]
    for k in range(depth+1):swap.uc.mem_write(anim+8+12*k,struct.pack('<iiI',root+k,loops[k],indices[k]))
    calls.clear();swap.invoke(0x3caccc,[anim,desired,prior]);out=[i32(w(anim+8+12*k)) for k in range(depth+1)];out+= [-1]*(3-len(out))
    action=2 if calls else (1 if desired!=-1 and(prior==-1 or prior==root) else 0)
    if calls:assert calls==[(0x3cacb0,u32(desired)),(0x3c93fc,0x3fa66666)]
    for k in range(depth+1):assert i32(w(anim+12+12*k))==loops[k] and w(anim+16+12*k)==indices[k]
    swaps.append(struct.pack('<14i',desired,prior,depth,root,*loops,*indices,*out,action))
# Execute actual animator updateTime, timeline, retained event manager and
# Character callback. Scene parsing/pose application are external boundaries.
sys.path.insert(0,str(R/'port/level-world/tests'))
from visual_timeline_differential import Machine,state
m=Machine(engine,False,manifest);cpu=m.c;anim=cpu.data+0x30000;avt=anim+0x100;char=anim+0x200
cpu.data+=0x80000;manager,track=install(cpu,4,[366,466],[['first','second'],['third']]);cpu.data-=0x80000;manager_vt=anim+0x400
cpu.pointer(manager,manager_vt);cpu.pointer(manager_vt+0x10,0x60ebe0);cpu.pointer(manager+8,0x3c9984);cpu.pointer(manager+12,char)
cpu.pointer(anim,avt);cpu.pointer(avt+0x44,cpu.callback+96);cpu.pointer(anim+0x18,manager);cpu.pointer(char+4,anim+0x500)
cpu.pointer(m.vt,0x667104);m.configure(state(start=266,end=600,current=266,last=1,initialized=1),(1500,0,1,0),struct.pack('<iI',0,0),1)
def virtual(address):
 if address==cpu.callback+96:cpu.put(0,m.s)
 else:m.callback(address)
cpu.handler=virtual;order=[]
def frame_hook(uc,address,size,data):
 if address==0x3a4d5c:
  event=cpu.reg(1);name=bytes(uc.mem_read(cpu.reg(2),64)).split(b'\0')[0].decode();lag=struct.unpack('<i',uc.mem_read(char+0x58,4))[0]
  current=struct.unpack('<i',uc.mem_read(m.s+4,4))[0];refs=struct.unpack('<I',uc.mem_read(manager+4,4))[0]
  order.append((event,lag,name,current,refs))
  if len(order)==1:cpu.pointer(anim+0x18,0) # synchronous active-manager replacement
  uc.reg_write(cpu.pc,uc.reg_read(cpu.lr))
cpu.uc.hook_add(UC_HOOK_CODE,frame_hook,begin=0x3a4d5c,end=0x3a4d5c)
try:cpu.invoke(0x667c48,[anim,1500])
except Exception:print('animator PC',hex(cpu.uc.reg_read(cpu.pc)), 'LR',hex(cpu.uc.reg_read(cpu.lr)));raise
assert order==[(40,234,'first',600,2),(40,234,'second',600,2),(40,134,'third',600,2)],order
assert struct.unpack('<i',cpu.uc.mem_read(manager+16,4))[0]==1 and struct.unpack('<I',cpu.uc.mem_read(manager+4,4))[0]==1
# Actual base finite closure sets byte48 before22, then clears49. Logging only
# is a caller service. Closure is distinct from authored event dispatch.
close=Cpu(engine,False,manifest);close_anim=close.data+0x10000;owner=close.data+0x11000;guard=close.data+0x12000;tableptr=close.data+0x13000;table=close.data+0x14000
def cw(a):return struct.unpack('<I',close.uc.mem_read(a,4))[0]
got=0x3caf4c+8+cw(0x3cb2c4);close.pointer(got+cw(0x3cb2c8),guard);close.pointer(guard,123);close.pointer(got+cw(0x3cb2d4),tableptr);close.pointer(tableptr,table)
close.uc.mem_write(table,struct.pack('<5I',0,0,1,0,0));close.pointer(owner+0x520,0x200);close.uc.mem_write(close_anim,bytes(0x80));close.pointer(close_anim+4,owner);close.pointer(close_anim+0x50,0xffffffff);close.uc.mem_write(close_anim+0x5c,b'\1')
closure=[];services={0x3136b4,0x3136b8,0x337888,0x3140ec,0x337a88,0x3139ac}
def closure_hook(uc,address,size,data):
 if address in services:uc.reg_write(close.pc,uc.reg_read(close.lr))
 elif address==0x3a4d5c:
  closure.append((close.reg(1),close.reg(2),uc.mem_read(close_anim+0x48,1)[0],uc.mem_read(close_anim+0x49,1)[0]));uc.reg_write(close.pc,uc.reg_read(close.lr))
close.uc.hook_add(UC_HOOK_CODE,closure_hook)
for repeat in range(2):close.uc.mem_write(close_anim+0x49,b'\1');close.invoke(0x3caf3c,[close_anim]);assert close.uc.mem_read(close_anim+0x49,1)[0]==0
assert [x for x in closure if x[0]==0x22]==[(0x22,0,1,1)],closure
blob=b'AEG1'+struct.pack('<3I',len(windows),len(swaps),len(lags))+b''.join(windows)+b''.join(swaps)+struct.pack('<'+'i'*len(lags),*lags)
out=R/'port/level-world/reference/actor-playback-events';out.mkdir(parents=True,exist_ok=True);(out/'event-fixtures.bin').write_bytes(blob)
report={'original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'arm64_sha256':hashlib.sha256((P/'oracle.so').read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'handoff_comparisons':len(lags),'event_manager_comparisons':len(windows),'authored_events_compared':callbacks,'swap_instruction_fixtures':len(swaps),'authored_tracks':authored,'mismatches':0,'scope':'Actual original EventManager and Char callback vs ARM64 event/handoff kernels; actual original ANIM_Swap metadata and explicit ANIM_Set/SetSpeed service boundary. Host composition separately validates phase/reentry/root behavior.'}
report.update(original_scene_callback_order=order,original_closure_trace=closure,retained_manager_batch_callbacks=3,finite_closure_repeated_updates=2)
(R/'port/level-world/reports/actor-playback-events-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='authored_tracks'}))
