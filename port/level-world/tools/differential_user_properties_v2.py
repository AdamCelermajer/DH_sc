from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
root=Path(__file__).resolve().parents[3];elf_path=root/'.local-inputs/libDungeonHunter2.so'
u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0x1100000);u.mem_map(0x2000000,0x40000)
with elf_path.open('rb') as f:
 elf=ELFFile(f)
 for s in elf.iter_segments():
  if s['p_type']=='PT_LOAD':u.mem_write(s['p_vaddr'],s.data())
OBJ=0x2000000;INPUT=0x2010000;BUFFER=0x2020000;CTYPE=0x2030000;STOP=0x203f000
table=bytearray(257)
for i in range(128):table[i+1]=7 if chr(i).isascii() and chr(i).isalnum() else 0
u.mem_write(CTYPE,bytes(table))
def word(p):return struct.unpack('<I',u.mem_read(p,4))[0]
def text(p):
 if not p:return b''
 out=bytearray()
 while True:
  c=u.mem_read(p+len(out),1)[0]
  if not c:return bytes(out)
  out.append(c);assert len(out)<4096
result={};trace=[]
def ret(value=None):
 if value is not None:u.reg_write(UC_ARM_REG_R0,value)
 u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
def hook(uc,a,size,data):
 trace.append(a)
 if a==0x318e8c:uc.reg_write(UC_ARM_REG_R0,CTYPE)
 elif a==0x3140ec:
  p=uc.reg_read(UC_ARM_REG_R0);v=text(uc.reg_read(UC_ARM_REG_R1));uc.mem_write(BUFFER,v+b'\0');uc.mem_write(p+20,struct.pack('<I',BUFFER));ret(p)
 elif a==0x318254:ret()
 elif a==0x30ec28:
  p=uc.reg_read(UC_ARM_REG_R0);value=uc.reg_read(UC_ARM_REG_R1)&255;v=text(p);i=v.find(bytes([value]));ret(p+i if i>=0 else 0)
 elif a==0x30ebd4:
  p=uc.reg_read(UC_ARM_REG_R0);v=text(p);needle=text(uc.reg_read(UC_ARM_REG_R1));i=v.find(needle);ret(p+i if i>=0 else 0)
 elif a==0x318e18:
  result[text(uc.reg_read(UC_ARM_REG_R1)).decode()]=text(uc.reg_read(UC_ARM_REG_R2)).decode();ret()
u.hook_add(UC_HOOK_CODE,hook)
manifest=json.loads((root/'.local-inputs/swamp-module-v2/manifest.json').read_text())
inputs=sorted(set(p for m in manifest['modules'] for p in m['floor_properties']))
inputs+=['',' key = value','a=one\na=two','foo_bar=%22water%22','x=pre%22hole wall%22post','a=%22unclosed','noequal','   = value','99key=%22void%22','a=\nb=%22%22','a=%22one%22%22two%22']
rows=[]
for source in inputs:
 result.clear();u.mem_write(INPUT,source.encode()+b'\0');u.reg_write(UC_ARM_REG_SP,OBJ+0xf000);u.reg_write(UC_ARM_REG_LR,STOP)
 u.reg_write(UC_ARM_REG_R0,OBJ);u.reg_write(UC_ARM_REG_R1,INPUT)
 try:u.emu_start(0x31903c,STOP,count=100000)
 except Exception:
  print('failure',repr(source),hex(u.reg_read(UC_ARM_REG_PC)),[hex(a) for a in trace if a>=0x300000][-30:]);raise
 rows.append(dict(input=source,output=dict(result)))
report=dict(status='CAPTURED',scope='whole original UserProperties._ParseProperties; CString/strchr/strstr/AddProperty and ASCII libc ctype leaf contracts supplied; golden native comparison pending',original_sha256=hashlib.sha256(elf_path.read_bytes()).hexdigest(),cases=rows)
(root/'port/level-world/reference/level-config-module-connection-v2/user-properties-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
