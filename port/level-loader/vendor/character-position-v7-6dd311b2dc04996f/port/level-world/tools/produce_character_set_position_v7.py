from pathlib import Path
import sys,json,struct,importlib.util,random,hashlib
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R1,UC_ARM_REG_R2
root=Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('decor_probe',root/'port/level-world/tests/decor_scene_differential.py');dep=importlib.util.module_from_spec(spec);spec.loader.exec_module(dep)
cpu=dep.Cpu(root/'.local-inputs/libDungeonHunter2.so',False,dep.Dependencies(),json.loads((root/'port/level-world/reference/decor-scene/original-functions.json').read_text()));cpu.symbols['position']=0x393db4
actor=cpu.data+0x1000;attached=cpu.data+0x3000;physical=cpu.data+0x4000;visual=cpu.data+0x5000;input_pointer=cpu.data+0x6000
calls=0;physical_words=b'\0'*8;reentry=False;pointer=input_pointer
def ret():cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))
def hook(uc,address,size,unused):
 global calls,physical_words
 if address==0x46ea80:
  calls=calls*10+2;physical_words=struct.pack('<II',uc.reg_read(UC_ARM_REG_R1),uc.reg_read(UC_ARM_REG_R2))
  if reentry:
   uc.mem_write(actor+0x2d8,b'\0'*4);values=list(struct.unpack('<3f',bytes(uc.mem_read(pointer,12))));values[0]+=7;values[1]-=3;uc.mem_write(pointer,struct.pack('<3f',*values))
  ret()
 elif address==0x470cb8:calls=calls*10+3;ret()
cpu.uc.hook_add(UC_HOOK_CODE,hook)
rng=random.Random(20261006);output=bytearray(struct.pack('<I',128));records=[]
for n in range(128):
 current=[rng.uniform(-1000,1000)for _ in range(3)];relative=[rng.uniform(-100,100)for _ in range(6)];a=[rng.uniform(-2000,2000)for _ in range(3)];destination=[rng.uniform(-1000,1000)for _ in range(3)];p=[rng.uniform(-1000,1000)for _ in range(3)];flags=n%16;alias=n//16%4;reentry=bool(n&64);calls=1 if flags&1 else 0;physical_words=b'\0'*8
 cpu.uc.mem_write(actor,bytes(0x400));cpu.uc.mem_write(actor+0x160,struct.pack('<3f',*current));cpu.uc.mem_write(actor+0x144,struct.pack('<6f',*relative));cpu.uc.mem_write(actor+0x1a8,struct.pack('<3f',*destination));cpu.uc.mem_write(attached+12,struct.pack('<3f',*a));cpu.uc.mem_write(input_pointer,struct.pack('<3f',*p));cpu.uc.mem_write(actor+0x2e0,struct.pack('<I',attached if flags&1 else 0));cpu.uc.mem_write(actor+0x2dc,struct.pack('<I',physical if flags&2 else 0));cpu.uc.mem_write(actor+0x2d8,struct.pack('<I',visual if flags&4 else 0));pointer=[input_pointer,actor+0x160,attached+12,actor+0x1a8][alias]
 cpu.invoke('position',[actor,pointer,1 if flags&8 else 0])
 expected=bytes(cpu.uc.mem_read(actor+0x160,12))+bytes(cpu.uc.mem_read(actor+0x12c,24))+bytes(cpu.uc.mem_read(attached+12,12))+bytes(cpu.uc.mem_read(actor+0x1a8,12))+struct.pack('<I',calls)+physical_words
 output+=struct.pack('<18fIII',*(current+relative+a+destination+p),flags,alias,int(reentry))+expected
 records.append({'flags':flags,'alias':alias,'physical_reentry':reentry,'calls':calls})
out=root/'port/level-world/reference/character-set-position-v7';out.mkdir(exist_ok=True);(out/'original-gold.bin').write_bytes(output)
(out/'original-audit.json').write_text(json.dumps({'original_elf_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'cases':records,'scope':'Whole original SetPosition393db4, UpdateAbsoluteAABB38aac8, SetDestination393600 execute. Actual physical.setPosition and visual.SyncPosition are declared callback fixtures; alias and reentrant pointer reread cases retained.'},indent=2)+'\n')
print('Original128 cases captured')
