from pathlib import Path
import sys,struct,random,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def put(p,v):c.uc.mem_write(p,struct.pack('<I',v&0xffffffff))
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
actor=c.data+0x10000;inv=actor+0x37c;head=inv+0x30;obj=c.data+0x14000;data=c.data+0x15000;heap=c.data+0x100000
c.uc.mem_write(actor,bytes(0x2000));put(head,head);put(head+4,head);put(obj+0x10,actor);put(obj+0xc,data)
base=(0x47d614+8+word(0x47d6e4))&0xffffffff
assert_mode=word((base+word(0x47d6e8))&0xffffffff);put(assert_mode,0) # Declared actual assertion-policy input fixture.
allocations=frees=base_calls=0
def hook(uc,a,z,u):
 global heap,allocations,frees,base_calls
 if a in (0x47ae70,0x47adbc):base_calls+=1;ret()
 elif a==0x708ec0:
  assert word(c.reg(0))==16;value=heap;heap+=32;c.uc.mem_write(value,b'\xa5'*16);allocations+=1;ret(value)
 elif a==0x708f00:assert c.reg(1)==16;frees+=1;ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
def entries():
 out=[];p=word(head);last=head
 while p!=head:
  assert word(p+4)==last;out.append((struct.unpack('<i',c.uc.mem_read(p+8,4))[0],c.uc.mem_read(p+12,1)[0]));last=p;p=word(p)
 assert word(head+4)==last;return out
rng=random.Random(0x47415431);ops=[]
ops.extend((0,77,1) for _ in range(256));ops.extend([(1,77,1),(0,-2147483648,1),(0,2147483647,1),(0,-1,1),(1,1234,1),(0,77,0),(1,-1,0)])
ops.extend((rng.randrange(2),rng.choice([-1,0,1,7,77,2147483647,-2147483648]),rng.randrange(5)!=0) for _ in range(1500))
records=[]
for op,id,enabled in ops:
 c.uc.mem_write(obj+8,bytes([int(enabled)]));put(data+0x24,id)
 if op==0:c.invoke(0x47eaec,[obj])
 else:c.invoke(0x47d6fc,[obj])
 current=entries();records.append(struct.pack('<IiII',op,id,int(enabled),len(current))+b''.join(struct.pack('<iI',id,refs) for id,refs in current))
blob=b'IG11'+struct.pack('<I',len(records))+b''.join(records)
p=root/'port/level-world/reference/inventory-gathering-v11';p.mkdir(exist_ok=True);(p/'fixtures.bin').write_bytes(blob)
report={'status':'PASS','cases':len(records),'allocations':allocations,'frees':frees,'base_lifecycle_deliveries':base_calls,'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'fixture_sha256':hashlib.sha256(blob).hexdigest(),'scope':'Whole original Objective_GatherLoot Register/Unregister and inventory unlink/counter bodies; generic Objective lifecycle, source16 allocator and mode0 assertion configuration declared fixtures. Source ordered sentinel/list and byte wrap execute.'}
(p/'original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
