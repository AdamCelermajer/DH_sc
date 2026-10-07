"""Execute the original whole joystick event5 branch through source position
and actual CTRLIsAllowed-false exit. Receiver/local-matrix/cache callbacks are
fixtures; float libc/ABI imports model IEEE binary32. No original GPU/controller
or whole unrelated HUD event dispatch is claimed."""
from pathlib import Path
import sys,struct,math,random,json,hashlib
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/game-data/tests'))
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from aggro_differential import Cpu
from combat_result_differential import bits,floating
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4,UC_ARM_REG_R5,UC_ARM_REG_R7,UC_ARM_REG_R10
class Oracle(Cpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ['atan2f','sqrtf','sinf','cosf','__aeabi_fdiv']:
   a,b=floating(self.reg(0)),floating(self.reg(1))
   v=math.atan2(a,b) if name=='atan2f' else math.sqrt(a) if name=='sqrtf' else math.sin(a) if name=='sinf' else math.cos(a) if name=='cosf' else a/b
   self.put(0,bits(v));uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
c=Oracle(R/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
state,event,ch,matrix=c.data+0x1000,c.data+0x2000,c.data+0x3000,c.data+0x4000
c.uc.mem_write(event,struct.pack('<I',ch));c.uc.mem_write(ch+0x4c,struct.pack('<I',matrix))
position=[]
def hook(uc,address,size,unused):
 if address==0x419764:
  uc.reg_write(UC_ARM_REG_R4,event);uc.reg_write(UC_ARM_REG_R5,state);uc.reg_write(UC_ARM_REG_R7,ch);uc.reg_write(UC_ARM_REG_R10,ch)
 elif address==0x418d8c:uc.reg_write(c.pc,c.stop)
 elif address==0x427d50:c.put(0,ch);uc.reg_write(c.pc,uc.reg_read(c.lr))
 elif address==0x7aa3f0:position.append((c.reg(2),c.reg(3)));uc.reg_write(c.pc,uc.reg_read(c.lr))
 elif address==0x3ad430:c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
c.uc.hook_add(UC_HOOK_CODE,hook)
rng=random.Random(20261005);records=[]
for index in range(1000):
 radii=[rng.randrange(1,101),rng.randrange(1,101)];center=[rng.randrange(-100,101),rng.randrange(-100,101)]
 values=[rng.uniform(-5000,5000) for _ in range(4)];values=[floating(bits(v)) for v in values]
 c.uc.mem_write(state+0xc,struct.pack('<4i',*radii,*center));c.uc.mem_write(event+0xc,struct.pack('<2f',*values[:2]));c.uc.mem_write(matrix+8,struct.pack('<f',values[2]));c.uc.mem_write(matrix+0x14,struct.pack('<f',values[3]));position.clear()
 c.invoke(0x419764,[]);assert len(position)==1
 magnitude=struct.unpack('<I',c.uc.mem_read(state+0x668,4))[0]
 records.append(struct.pack('<4i4f3I',*radii,*center,*values,magnitude,*position[0]))
directory=R/'port/engine-ui/reference/authored-joystick-v1';directory.mkdir(parents=True,exist_ok=True)
(directory/'fixtures.bin').write_bytes(struct.pack('<I',len(records))+b''.join(records))
report={'validation':'PASS','original_cases':len(records),'original_sha256':hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'scope':__doc__,'fixtures_sha256':hashlib.sha256((directory/'fixtures.bin').read_bytes()).hexdigest()}
(R/'port/engine-ui/reports/authored-joystick-v1-original.json').write_text(json.dumps(report,indent=2));print(json.dumps(report))
