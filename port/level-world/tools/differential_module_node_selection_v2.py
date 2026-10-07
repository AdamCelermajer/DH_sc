from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
root=Path(__file__).resolve().parents[3]
asset=root/'.local-inputs/swamp-module-v2/swamp.bdae';b=asset.read_bytes()
manifest=json.loads((asset.parent/'manifest.json').read_text());elf_path=root/'.local-inputs/libDungeonHunter2.so'
u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0x1100000)
with elf_path.open('rb') as f:
 elf=ELFFile(f)
 for s in elf.iter_segments():
  if s['p_type']=='PT_LOAD':u.mem_write(s['p_vaddr'],s.data())
BASE=0x2000000;STACK=0x3000000;NAME=0x3010000;STOP=0x3020000
u.mem_map(BASE,(len(b)+4095)&~4095);u.mem_map(STACK,0x30000)
w=lambda p:struct.unpack_from('<I',b,p)[0]
relocated=bytearray(b)
for i in range(w(16)):
 p=w(w(24)+4*i);v=w(p)
 if v:struct.pack_into('<I',relocated,p,BASE+v)
u.mem_write(BASE,bytes(relocated));scene=w(w(32)+156)
def cstring(p):
 v=bytearray()
 while True:
  c=u.mem_read(p+len(v),1)[0]
  if not c:return bytes(v)
  v.append(c)
  assert len(v)<4096
def hook(uc,a,size,data):
 if a==0x60e54c:
  assert uc.reg_read(UC_ARM_REG_R1)==0
  uc.reg_write(UC_ARM_REG_R0,BASE+scene);uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
 elif a==0x30e31c:
  lhs=cstring(uc.reg_read(UC_ARM_REG_R0));rhs=cstring(uc.reg_read(UC_ARM_REG_R1))
  uc.reg_write(UC_ARM_REG_R0,0 if lhs==rhs else (1 if lhs>rhs else 0xffffffff));uc.reg_write(UC_ARM_REG_PC,uc.reg_read(UC_ARM_REG_LR))
u.hook_add(UC_HOOK_CODE,hook)
rows=[]
for row in manifest['modules']:
 for suffix,expected in [('-node',BASE+row['node_offset']),('-NODE',0)]:
  name=row['declaration']['xrefobject']+suffix;u.mem_write(NAME,name.encode()+b'\0')
  u.reg_write(UC_ARM_REG_SP,STACK+0xf000);u.reg_write(UC_ARM_REG_LR,STOP)
  u.reg_write(UC_ARM_REG_R0,STACK);u.reg_write(UC_ARM_REG_R1,NAME)
  u.emu_start(0x61c290,STOP,count=1000000);actual=u.reg_read(UC_ARM_REG_R0)
  assert actual==expected,(name,hex(actual),hex(expected));rows.append(dict(name=name,node_offset=actual-BASE if actual else None))
report=dict(status='PASS',cases=len(rows),scope='whole original database.getNode on actual SWAMP BRES; GetVisualScene0 and strcmp are explicit leaf services; not whole Module InitPost',original_sha256=hashlib.sha256(elf_path.read_bytes()).hexdigest(),asset_sha256=hashlib.sha256(b).hexdigest(),results=rows)
out=root/'port/level-world/reports/module-node-selection-original-v2.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
