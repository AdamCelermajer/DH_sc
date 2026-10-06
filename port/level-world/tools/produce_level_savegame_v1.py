from pathlib import Path
import sys,json,struct,importlib.util,hashlib,random
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
root=Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('oracle',root/'port/level-world/tests/decor_scene_differential.py');dep=importlib.util.module_from_spec(spec);spec.loader.exec_module(dep)
class Dependencies(dep.Dependencies):
 def call(self,cpu,name):
  if name=='strlen':cpu.write_reg(0,len(cstring(cpu.reg(0))));returned();return
  if name=='sprintf':
   fmt=cstring(cpu.reg(1));args=[cpu.reg(2),cpu.reg(3)]+list(struct.unpack('<4I',bytes(cpu.uc.mem_read(cpu.uc.reg_read(cpu.sp_reg),16))))
   # Source format is read from ELF; %s arguments are actual source literals.
   if fmt=='%s%03u_%01u_%03u_%03u%s':vals=(cstring(args[0]),args[1],args[2],args[3],args[4],cstring(args[5]))
   elif fmt=='%s%03u_%01u%s%s':vals=(cstring(args[0]),args[1],args[2],cstring(args[3]),cstring(args[4]))
   else:raise AssertionError(fmt)
   output=(fmt%vals).encode()+b'\0';cpu.uc.mem_write(cpu.reg(0),output);cpu.write_reg(0,len(output)-1);returned();return
  super().call(cpu,name)
cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,Dependencies(),{'functions':[]})
cpu.uc.mem_map(cpu.data+0x10000,0x40000)
def cstring(p):return bytes(cpu.uc.mem_read(p,1024)).split(b'\0')[0].decode()
def returned():cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
heap=cpu.data+0x10000;sections=[];cached='';raw=-1
def hook(uc,a,n,x):
 global heap,cached,raw
 if a==0x31167c:returned()
 elif a==0x3109e0:
  out,start,end=[cpu.reg(i)for i in range(3)];data=bytes(uc.mem_read(start,end-start))+b'\0';p=heap;heap+=2048;uc.mem_write(p,data);uc.mem_write(out+16,struct.pack('<II',p,p));returned()
 elif a==0x310570:cpu.write_reg(0,heap);heap+=2048;returned()
 elif a==0x315ed8:cached=cstring(cpu.reg(1));raw=cpu.reg(2);returned()
 elif a==0x315904:sections.append(cstring(cpu.reg(1)));returned()
 elif a==0x3139ac:returned()
cpu.uc.hook_add(UC_HOOK_CODE,hook)
actor=cpu.data+0x1000;level=cpu.data+0x3000;rng=random.Random(20261006);rows=[]
for i in range(64):
 seed=[0,1,114,0xffffffff][i%4] if i<16 else rng.getrandbits(32);difficulty=[-1,0,1,2147483647][i%4];row=[-1,0,433,-2147483648][i//4%4];mode=[0,1,-1,7][i//16];checkpoint=i%2
 heap=cpu.data+0x10000;sections=[];cached='';raw=-1;cpu.uc.mem_write(actor,bytes([0xa5])*64)
 cpu.symbols['ctor']=0x462934;cpu.invoke('ctor',[actor,level,seed,difficulty&0xffffffff],stack=struct.pack('<III',row&0xffffffff,mode&0xffffffff,checkpoint))
 values=struct.unpack('<5i',bytes(cpu.uc.mem_read(actor+0x28,20)));assert values[:4]==(row,-1,-1,-1)
 assert bytes(cpu.uc.mem_read(actor+0x38,2))==b'\0\0';assert sections==['INFO','OBJS'];assert raw==0
 rows.append({'seed':seed,'difficulty':difficulty,'row':row,'mode':mode,'checkpoint':checkpoint,'filename':cached})
out=root/'port/level-world/reference/level-savegame-v1';out.mkdir(exist_ok=True)
(out/'constructor-gold.json').write_text(json.dumps({'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'cases':rows,'scope':'Whole LevelSavegame C1 and filename functions execute. Allocation/CString/libc and nested Savegame C1/initSectionInfo are declared service fixtures; file/cache body is not oracle-accepted.'},indent=2)+'\n')
lines=['#pragma once','struct LevelSaveGolden { unsigned seed; int difficulty,row,mode;bool checkpoint;const char* filename; };','static const LevelSaveGolden level_save_gold[] = {']
for r in rows:lines.append('{%du,%d,%d,%d,%s,%s},'%(r['seed'],r['difficulty'],r['row'],r['mode'],'true'if r['checkpoint']else'false',json.dumps(r['filename'])))
lines.append('};');(out/'constructor-gold.hpp').write_text('\n'.join(lines)+'\n');print('Original LevelSavegame C1 64 cases PASS')
