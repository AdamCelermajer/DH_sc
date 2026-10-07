"""Whole original filename branch; sprintf/CString assignment leaf fixtures."""
from pathlib import Path
import sys,struct,json
r=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages');sys.path.insert(0,str(r/'port/level-world/tests'))
from navigation_differential import Cpu
from unicorn import UC_HOOK_CODE
cpu=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});result=''
def text(at):return bytes(cpu.uc.mem_read(at,128)).split(b'\0')[0].decode()
def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
def ret():cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
def hook(uc,at,n,_):
 global result
 if at==0x30eae4:
  assert text(cpu.reg(1))=='%s%03u%s%s';sp=uc.reg_read(cpu.sp);value=text(cpu.reg(2))+('%03d'%cpu.reg(3))+text(word(sp))+text(word(sp+4));uc.mem_write(cpu.reg(0),value.encode()+b'\0');ret()
 elif at==0x30de54:cpu.put(0,len(text(cpu.reg(0))));ret()
 elif at==0x3109e0:result=bytes(uc.mem_read(cpu.reg(1),cpu.reg(2)-cpu.reg(1))).decode();ret()
cpu.uc.hook_add(UC_HOOK_CODE,hook);rows=[];blob=bytearray()
for slot in (0,9,99,999,1000,0xffffffff):
 for cp in (0,1):
  for multi in (0,1):
   result='';cpu.invoke(0x463c84,[slot,cpu.data+0x1000,cp,multi]);rows.append({'slot':slot,'checkpoint':cp,'multi':multi,'filename':result});v=result.encode();blob.extend(struct.pack('<4I',slot,cp,multi,len(v))+v)
d=r/'port/level-world/reference/campaign-save-v45';(d/'filename-gold.bin').write_bytes(struct.pack('<I',len(rows))+blob);(d/'filename-gold.json').write_text(json.dumps({'cases':rows,'scope':__doc__},indent=2)+'\n');print(len(rows))
