from pathlib import Path
import sys,struct,random,json,hashlib
r=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
mgr=c.data+0x10000;event=c.data+0x20000;evtvt=event+0x100;recvvt=event+0x200;heap=c.data+0x100000
typefn=c.data+0x40000;recvfn=typefn+4;destroyfn=typefn+8
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def signed(p):return struct.unpack('<i',c.uc.mem_read(p,4))[0]
def put(p,v):c.uc.mem_write(p,struct.pack('<I',v&0xffffffff))
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
put(event,evtvt);put(evtvt+8,typefn);put(evtvt+4,destroyfn);put(recvvt+8,recvfn)
receivers=[c.data+0x30000+i*0x100 for i in range(8)]
for i,p in enumerate(receivers):put(p,recvvt);put(p+4,i)
calls=[];deletes=[];allocations=frees=0
def hook(uc,a,z,u):
 global heap,allocations,frees
 if a==0x708ec0:
  size=word(c.reg(0));assert size in (12,16,20,28),size
  v=heap;heap+=64;c.uc.mem_write(v,b'\xa5'*size);allocations+=1;ret(v)
 elif a==0x708f00:frees+=1;ret()
 elif a==typefn:ret(word(c.reg(0)+4))
 elif a==recvfn:
  assert c.reg(1)==event and c.reg(2)==mgr
  i=word(c.reg(0)+4);calls.append(i);put(event+8,word(event+8)+1);ret(i%4)
 elif a==destroyfn:assert c.reg(0)==event;deletes.append(99);ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
c.uc.mem_write(mgr,b'\xa5'*48);c.invoke(0x3380bc,[mgr])
assert word(mgr+0xc)==0 and word(mgr+0x18)==0 and word(mgr+0x20)==mgr+0x20 and word(mgr+0x28)==mgr+0x28
def state():
 out=[]
 def walk(p):
  if not p:return
  walk(word(p+8));key=signed(p+0x10);head=p+0x14;n=word(head)
  while n!=head:
   out.append((key,word(word(n+8)+4),signed(n+0xc),c.uc.mem_read(n+0x10,1)[0]));n=word(n)
  walk(word(p+0xc))
 walk(word(mgr+0xc));n=word(mgr+0x28);dc=0
 while n!=mgr+0x28:dc+=1;n=word(n)
 return out,dc
rng=random.Random(0x45564d12);ops=[(0,7,i,p) for i,p in enumerate([10,-5,100,0,-2147483648,2147483647,12,4])]+[(4,7,0,0),(5,7,0,0)]
for _ in range(1600):ops.append((rng.choice([0,1,2,3,4,5,6,7]),rng.choice([-2147483648,-1,0,7,2147483647]),rng.randrange(8),rng.randrange(-50,50)))
records=[];scheduled=set()
for op,t,i,priority in ops:
 # Undefined original duplicate delayed-node or detach-before-drop lifetime
 # combinations are excluded; native successor tests exercise modern safety.
 if op==2 and (t,i) in scheduled:continue
 if op==1 and (t,i) in scheduled:continue
 calls.clear();deletes.clear();put(event+4,t);put(event+8,0)
 if op==0:result=c.invoke(0x338da0,[mgr,t,receivers[i],priority])
 elif op==1:result=c.invoke(0x33811c,[mgr,t,receivers[i]])
 elif op==2:
  result=c.invoke(0x3382a4,[mgr,t,receivers[i]])
  if result:scheduled.add((t,i))
 elif op==3:c.invoke(0x3383f4,[mgr]);result=0;scheduled.clear()
 elif op in (4,5):c.invoke(0x338ebc if op==4 else 0x339090,[mgr,event]);result=0
 elif op==6:c.invoke(0x3384ac,[mgr]);result=0;scheduled.clear()
 else:c.invoke(0x33900c,[mgr,0,0,0]);result=0;scheduled.clear()
 rows,dc=state()
 records.append(struct.pack('<IiiiIIIII',op,t,i,priority,result,dc,word(event+8),len(rows),len(calls))+b''.join(struct.pack('<iiii',*row) for row in rows)+b''.join(struct.pack('<I',x) for x in calls))
# Explicit producer-storage fixture injects ONE native list node, exercising
# whole positive Update Raise -> deleting virtual4 -> node free12.
def inject_pending():
 global heap
 node=heap;heap+=64;put(node,mgr+0x20);put(node+4,mgr+0x20);put(node+8,event);put(mgr+0x20,node);put(mgr+0x24,node)
put(event+4,7);inject_pending()
calls.clear();deletes.clear();c.invoke(0x33900c,[mgr,0,0,0]);assert deletes==[99] and word(mgr+0x20)==mgr+0x20
inject_pending();deletes.clear();c.invoke(0x3384ac,[mgr]);assert deletes==[] and word(mgr+0x20)==mgr+0x20
inject_pending();c.invoke(0x3384f8,[mgr]);assert deletes==[] and word(mgr+0xc)==0 and word(mgr+0x20)==mgr+0x20
out=r/'port/level-world/reference/event-manager-v12';blob=b'EM12'+struct.pack('<I',len(records))+b''.join(records);(out/'fixtures.bin').write_bytes(blob)
report={'status':'PASS','cases':len(records),'positive_queue_lifetime_cases':3,'allocations':allocations,'frees':frees,'source_sha256':hashlib.sha256((r/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'fixture_sha256':hashlib.sha256(blob).hexdigest(),'scope':'Whole original C1, Attach/Detach/map tree/native lists, DelayedDetach/Drop, Raise/RaiseAsync receiver snapshots, Flush, empty/positive Update, D1. Node allocator, abstract GetType/handler/delete virtual receivers and positive pending-list storage insertion are explicit fixtures. Legacy dangling/duplicate delayed-node combinations excluded.'}
(out/'original-audit.json').write_text(json.dumps(report,indent=2));print(json.dumps(report))
