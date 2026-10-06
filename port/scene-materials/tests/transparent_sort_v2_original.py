from pathlib import Path
import sys,struct,random,json,hashlib
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu as BaseCpu
from combat_result_differential import floating
from unicorn import UC_HOOK_CODE
class Cpu(BaseCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_fcmpeq':
   self.put(0,int(floating(self.reg(0))==floating(self.reg(1))));uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
old=Cpu(root/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
def hook(uc,a,size,unused):
 if a in (0x351e3c,0x310be8):
  old.put(0,0);uc.reg_write(old.pc,uc.reg_read(old.lr))
old.uc.hook_add(UC_HOOK_CODE,hook)
rng=random.Random(2026100502);records=[]
words=[0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc00001]
for case in range(160):
 count=case if case<34 else rng.randrange(1,65)
 entries=[]
 for i in range(count):
  entries.append([old.data+0x8000+i*64,i,0,rng.choice([0,1,0xffffffff]),rng.choice(words)])
 if case%4==0:
  for entry in entries:entry[3:]=[0,0x3f800000]
 initial=b''.join(struct.pack('<5I',*entry) for entry in entries)
 old.uc.mem_write(old.data+0x1000,initial or bytes(20))
 old.invoke(0x356f08,[old.data+0x1000,count])
 actual=bytes(old.uc.mem_read(old.data+0x1000,count*20)) if count else b''
 records.append(struct.pack('<I',count)+initial+actual)
out=root/'port/scene-materials/reference/transparent-sort-original-v2.bin'
out.write_bytes(struct.pack('<I',len(records))+b''.join(records))
report={'status':'PASS','cases':len(records),'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'scope':'Whole original heapsort, heapsink and transparent comparator instructions; actual null-material domain including IEEE nonfinite inputs and equal-key permutations. Not a whole-scene GPU receipt.'}
(root/'port/scene-materials/reports/transparent-sort-original-v2.json').write_text(json.dumps(report,indent=2));print(json.dumps(report))
