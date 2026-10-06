from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_R7,UC_ARM_REG_LR,UC_ARM_REG_PC,UC_ARM_REG_SP
root=Path(__file__).resolve().parents[3];original=root/'.local-inputs/libDungeonHunter2.so';ref=root/'port/level-world/reference/canonical-object-factory-v1'
results=[]
for kind,entry in [('ObjectBase',0x33f014),('GameObject',0x38cee8),('Character',0x3a9fe4),('OpenableContainer',0x3a1e8c),('AnimatedDecor',0x389dd4)]:
 u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,0xa00000);u.mem_map(0x1000000,0x300000)
 with original.open('rb') as f:
  elf=ELFFile(f);symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
  for p in elf.iter_segments():
   if p['p_type']=='PT_LOAD':u.mem_write(p['p_vaddr'],p.data())
 actor=0x1100000;stop=0x1000000;heap=[0x1200000];rows=[];strings={}
 # Real AnimatedDecor factory342600 stores static84=1 before registration.
 if kind=='AnimatedDecor':u.mem_write(actor+0x84,b'\x01')
 def text(a):
  out=bytearray()
  while a and u.mem_read(a,1)!=b'\0':out.extend(u.mem_read(a,1));a+=1
  return out.decode('utf-8')
 def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def hook(u,address,size,data):
  if address==stop:u.emu_stop();return
  if address==0x310570:
   u.reg_write(UC_ARM_REG_R0,heap[0]);heap[0]+=0x100;u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  if address==0x3140ec:
   strings[u.reg_read(UC_ARM_REG_R0)]=text(u.reg_read(UC_ARM_REG_R1));u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  if address==0x318254:u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
  if address in (0x33e4ac,0x33ef7c,0x33e404,0x389894,0x3a92b4,0x513ce4):
   field=u.reg_read(UC_ARM_REG_R2);default=u.reg_read(UC_ARM_REG_R3);name=text(u.reg_read(UC_ARM_REG_R1))
   row=dict(name=name,register_leaf=hex(address),offset=hex(field-actor))
   if address==0x33e4ac:row.update(kind='bool',default=default&255)
   elif address==0x33ef7c:row.update(kind='string',default='',default_source='source wrapper33ef7c constructs empty CString')
   elif address==0x33e404:row.update(kind='string',default=strings.get(default,'UNAVAILABLE'))
   elif address==0x389894:
    global_pointer=u.reg_read(UC_ARM_REG_R7)
    row.update(kind='vector3',default_raw=list(struct.unpack('<3I',u.mem_read(default,12))),global_reference=dict(address=hex(global_pointer),symbols=[s.name for s in symbols if s['st_value']==global_pointer]))
   elif address==0x3a92b4:row.update(kind='bool',default=1,default_source='source bool clone21 literal1')
   else:
    row['offset']=hex(word(field+4)+4);row['descriptor_vtable']=hex(word(field));row['descriptor_default_raw']=list(struct.unpack('<2I',u.mem_read(field+0x20,8)))
   rows.append(row);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR));return
 u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,actor);u.reg_write(UC_ARM_REG_SP,0x10ff000);u.reg_write(UC_ARM_REG_LR,stop)
 u.emu_start(entry,stop,count=20000)
 assert rows and all(r['name'] for r in rows)
 results.append(dict(kind=kind,entry=hex(entry),declarations=rows))
report=dict(original_sha256=hashlib.sha256(original.read_bytes()).hexdigest(),classes=results,
 fixture_services=['AddProperty typed registration leaves capture actual arguments','operator new allocation','CString constructor/destructor used only descriptor name/default presentation'],
 excludes=['constructor-backed defaults for implicit fields','vector global dynamic initialization','generic PropertyMap storage execution'],production_property_map_verified=False)
(ref/'property-declarations-original.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(results,indent=2))
