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
base=old.data;left=base+0x1000;right=base+0x1100;nodes=[base+0x2000,base+0x2100];materials=[base+0x3000,base+0x3100];vt=base+0x4000
equal=less=leftsub=rightsub=0
def returned(v):old.put(0,v&0xffffffff);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
def hook(uc,a,size,unused):
 if a in (0x351e3c,0x310be8):returned(0)
 elif a==0x3537b0:returned(equal)
 elif a==0x3537e4:returned(less)
 elif a==0x708ec0:returned(leftsub if old.reg(0)==nodes[0] else rightsub)
old.uc.hook_add(UC_HOOK_CODE,hook)
old.pointer(vt+0x20,0x708ec0)
for node in nodes:old.pointer(node,vt)
rng=random.Random(20261005);records=[]
floatwords=[0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc00001,0x7f800001]
prioritywords=[0,1,0xffffffff,0x7fffffff,0x80000000]
for i in range(1024):
 pa=rng.choice(prioritywords);pb=pa if i%2 else rng.choice(prioritywords)
 fa=rng.choice(floatwords);fb=fa if i%3 else rng.choice(floatwords)
 ma=rng.choice([0,*materials]);mb=rng.choice([0,*materials]);equal=i%2;less=i%3==0;leftsub=rng.choice(prioritywords);rightsub=rng.choice(prioritywords)
 a=[nodes[0],2,ma,pa,fa];b=[nodes[1],3,mb,pb,fb]
 for material in materials:old.uc.mem_write(material,struct.pack('<I',1))
 old.uc.mem_write(left,struct.pack('<5I',*a));old.uc.mem_write(right,struct.pack('<5I',*b))
 result=old.invoke(0x3538ac,[left,right]);assert result in (0,1)
 records.append(struct.pack('<15I',*a,*b,int(equal),int(less),leftsub,rightsub,result))
out=root/'port/scene-materials/reference/transparent-queue-original-v1.bin';out.write_bytes(struct.pack('<I',len(records))+b''.join(records))
report={'status':'PASS','cases':len(records),'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(out.read_bytes()).hexdigest(),'scope':'Whole original transparent comparator instructions; material equal/order, node suborder and lease release are declared service observers. Not full material comparator, queue registration, vector sort or GPU order.'}
(root/'port/scene-materials/reports/transparent-queue-original-v1.json').write_text(json.dumps(report,indent=2));print(json.dumps(report))
