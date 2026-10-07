"""Original ARM ModifiedXP/GetLevelScaledXP instructions; imported IEEE math
and source Debug endpoints are oracle service boundaries, not XP stubs."""
from pathlib import Path
import sys,struct,random,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu as BaseCpu
from unicorn import UC_HOOK_CODE
class Cpu(BaseCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='__aeabi_fcmplt':
   f=lambda v:struct.unpack('<f',struct.pack('<I',v))[0]
   self.put(0,int(f(self.reg(0))<f(self.reg(1))));uc.reg_write(self.pc,uc.reg_read(self.lr));self.import_calls[name]=self.import_calls.get(name,0)+1
  elif name=='__aeabi_fdiv':
   f=lambda v:struct.unpack('<f',struct.pack('<I',v))[0]
   self.put(0,bits(f(self.reg(0))/f(self.reg(1))));uc.reg_write(self.pc,uc.reg_read(self.lr));self.import_calls[name]=self.import_calls.get(name,0)+1
  else:super().external(uc,address,size,unused)
c=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
def u(a):return struct.unpack('<I',c.uc.mem_read(a,4))[0]
def bits(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
properties=c.data+0x1000;settings=c.data+0x3000;player=c.data+0x4000;victim=c.data+0x6000
got=(0x3bd928+8+u(0x3bda60))&0xffffffff
c.pointer(got+u(0x3bda68),c.data+0x9000);c.pointer(c.data+0x9000,settings)
levels=(0,0)
def hook(uc,a,n,unused):
 if a==0x3bd120:ret(levels[0] if c.reg(0)==player else levels[1])
 elif a in (0x337888,0x337a88,0x3140ec,0x318254):ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
rng=random.Random(20261005);modified=[];scaled=[]
for i in range(1600):
 raw=rng.getrandbits(32);bonus=rng.getrandbits(32)
 c.uc.mem_write(properties+0xdb8,struct.pack('<I',bonus))
 result=c.invoke(0x3de7ec,[properties,raw]);modified.append((raw,bonus,result))
for i in range(1600):
 levels=(rng.randrange(-10,101),rng.randrange(-10,101))
 values=[float(rng.randrange(-25,26)),float(rng.randrange(-25,26)),float(rng.randrange(50,251)),float(rng.randrange(0,51)),float(rng.randrange(0,21)),0.,0.]
 c.uc.mem_write(settings,bytes(176));c.uc.mem_write(settings+0x8c,struct.pack('<7f',*values))
 base=bits(float(rng.randrange(0,10000)))
 result=c.invoke(0x3bd918,[base,player,victim]);scaled.append((base,*levels,*map(bits,values[:5]),result))
folder=root/'port/level-world/reference/player-progression-v1';folder.mkdir(exist_ok=True)
gold=folder/'math-fixtures.bin';gold.write_bytes(struct.pack('<II',len(modified),len(scaled))+b''.join(struct.pack('<3I',*x) for x in modified)+b''.join(struct.pack('<Iii6I',*x) for x in scaled))
report={'validation':'PASS','original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'original_cases':len(modified)+len(scaled),'scope':__doc__,'gold_sha256':hashlib.sha256(gold.read_bytes()).hexdigest(),'imports':c.import_calls}
(root/'port/level-world/reports/player-progression-v1-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
