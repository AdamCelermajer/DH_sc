"""Execute original ctor/InitTrophies/GetData/query instructions on cache rows.
Only allocator and discarded DebugLoad/GetSwitch/CString services are fixture
observers. Original row mapping, vector append/growth/copy and lookup execute.
"""
from pathlib import Path
import sys,struct,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
assets=root/'.local-inputs/trophy-owner-assets-v1';raw=(assets/'trophies_pyarray.bin').read_bytes();count=struct.unpack_from('<I',raw)[0]
owner=c.data+0x1000;table=c.data+0x2000;heap=c.data+0x10000;allocations=0;debug_calls=0
c.uc.mem_write(owner,bytes(28));c.pointer(0x9a66c8,count);c.pointer(0x9a66cc,table)
for i in range(count):c.uc.mem_write(table+i*32,bytes(4)+raw[4+i*28:4+(i+1)*28])
def returned(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,a,z,p):
 global heap,allocations,debug_calls
 if a==0x310570:
  n=c.reg(0);assert n==40;returned(heap);heap+=64;allocations+=1
 elif a==0x37fd80:
  n=c.reg(1);assert n<=256;c.pointer(c.reg(2),n);returned(heap);heap+=max(n*4,256);allocations+=1
 elif a in (0x337888,0x337a88):debug_calls+=1;returned()
 elif a==0x3140ec:
  text=bytes(c.uc.mem_read(c.reg(1),100)).split(b'\0')[0];assert text==b'isTracingTrophies';c.uc.mem_write(c.reg(0),bytes(24));returned(c.reg(0))
 elif a in (0x708f00,0x310440):returned()
c.uc.hook_add(UC_HOOK_CODE,hook)
c.invoke(0x380474,[owner]);first,end=struct.unpack('<II',c.uc.mem_read(owner+4,8));assert (end-first)//4==count
records=[]
for i in range(count):
 ptr=struct.unpack('<I',c.uc.mem_read(first+i*4,4))[0];record=bytes(c.uc.mem_read(ptr+4,36));words=struct.unpack('<9I',record)
 row=struct.unpack_from('<7I',raw,4+i*28)
 assert words==(i,row[5],row[0],0,row[6],row[3],row[4],row[2],row[1]),(i,words,row)
 assert c.invoke(0x37ff60,[owner,i])==ptr
 assert c.invoke(0x380058,[owner,i])==0
 assert c.invoke(0x37f9e4,[owner,i])==0
 records.append(record)
ref=root/'port/level-world/reference/trophy-manager-owner-v1/constructor-source-fixture.bin';ref.write_bytes(struct.pack('<I',count)+b''.join(records))
report={'validation':'PASS','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'authored_rows':count,'source_field_comparisons':count*9,'source_queries':count*3,'allocator_fixture_calls':allocations,'debug_observer_calls':debug_calls,'reference_sha256':hashlib.sha256(ref.read_bytes()).hexdigest(),'full_achievement_callback':False}
(root/'port/level-world/reports/trophy-manager-owner-v1-original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
