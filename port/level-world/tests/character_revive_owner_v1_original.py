from pathlib import Path
import sys,struct,json,hashlib,itertools
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
engine=ROOT/'.local-inputs/libDungeonHunter2.so';c=Cpu(engine,False,{'functions':[]})
owner=c.data+0x1000;vt=c.data+0x4000;app=c.data+0x5000
trace=[];online=[0,0];queries=0;player=0;submitted=[0,0,0]
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,address,size,unused):
 global queries,submitted
 if address in (0x3a4d5c,0x3b3a70,0x3b4088,0x3d8894):trace.append(address);ret()
 elif address==0x7fd794:
  trace.append(address);uc.mem_write(app+5,bytes([online[min(queries,1)]]));queries+=1;ret(app)
 elif address==0x3a49f0:trace.append(address);ret(player)
 elif address==0x525508:
  trace.append(address);assert bytes(uc.mem_read(c.reg(1),12))==struct.pack('<3f',1,2,3);uc.mem_write(c.reg(2),struct.pack('<f',9));ret()
 elif address==0x393db4:trace.append(address);submitted=list(struct.unpack('<3I',uc.mem_read(c.reg(1),12)));assert c.reg(2)==1;ret()
c.uc.hook_add(UC_HOOK_CODE,hook);c.pointer(vt+0x28,0x3a49f0)
records=[]
for dead,armed,remote,physical,player,first,second in itertools.product((0,1,255),(0,1),(0,5),(0,1,255),(0,1),(0,1),(0,1)):
 c.uc.mem_write(owner,bytes(0x1800));c.pointer(owner,vt);c.uc.mem_write(owner+0x1448,bytes([armed,dead]));c.uc.mem_write(owner+0x118,bytes([remote]));c.pointer(owner+0x110,17);c.pointer(owner+0x114,23);c.uc.mem_write(owner+0x1474,struct.pack('<3f',1,2,3))
 trace.clear();queries=0;online=[first,second];submitted=[0,0,0]
 c.invoke(0x3a59ac,[owner,0,physical])
 output=[c.uc.mem_read(owner+0x1449,1)[0],c.uc.mem_read(owner+0x1448,1)[0],c.uc.mem_read(owner+0x118,1)[0],*struct.unpack('<2I',c.uc.mem_read(owner+0x110,8))]
 words=[dead,armed,remote,17,23,physical,player,first,second,*output,len(trace),*trace,*([0]*(9-len(trace))),*submitted]
 assert len(words)==27;records.append(struct.pack('<27I',*words))
out=ROOT/'port/level-world/reference/character-revive-owner-v1';out.mkdir(parents=True,exist_ok=True)
gold=out/'original-fixtures.bin';gold.write_bytes(struct.pack('<I',len(records))+b''.join(records))
report={'original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'entry':'0x3a59ac','size':312,'function_sha256':hashlib.sha256(bytes(c.uc.mem_read(0x3a59ac,312))).hexdigest(),'cases':len(records),'gold_sha256':hashlib.sha256(gold.read_bytes()).hexdigest(),'scope':'Whole original Revive control flow/direct cue/dead/remote/network stores and live App query reload. Event/vitals/physical/IsPlayer/floor/position/skills are explicit observer/helper fixtures; their complete bodies are not claimed.'}
(out/'original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
