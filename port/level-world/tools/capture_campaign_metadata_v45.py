"""Execute five complete original metadata writers; stream/CString leaves fixture."""
from pathlib import Path
import sys,struct,json,random,hashlib
r=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages');sys.path.insert(0,str(r/'port/level-world/tests'))
from navigation_differential import Cpu
from unicorn import UC_HOOK_CODE
cpu=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});save=cpu.data+0x1000;names=cpu.data+0x3000;textbase=cpu.data+0x4000;strings={};output=bytearray()
labels=['KnightPlayerBase','RoguePlayerBase','MagePlayerBase']
for i,name in enumerate(labels):cpu.pointer(names+i*4,textbase+i*128);cpu.uc.mem_write(textbase+i*128,name.encode()+b'\0')
cpu.pointer(0x9a6458,len(labels));cpu.pointer(0x9a6460,names)
def ret():cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
def hook(uc,address,n,_):
 if address in (0x38b808,0x461770):output.extend(uc.mem_read(cpu.reg(1),4));ret()
 elif address in (0x33e138,0x39f828):output.extend(uc.mem_read(cpu.reg(1),1));ret()
 elif address==0x30de54:cpu.put(0,len(bytes(uc.mem_read(cpu.reg(0),128)).split(b'\0')[0]));ret()
 elif address==0x3116e8:strings[cpu.reg(0)]=bytes(uc.mem_read(cpu.reg(1),cpu.reg(2)-cpu.reg(1)));ret()
 elif address==0x461668:
  value=strings[cpu.reg(1)]+b'\0';output.extend(struct.pack('<I',len(value))+value);ret()
 elif address==0x3139ac:ret()
cpu.uc.hook_add(UC_HOOK_CODE,hook);rng=random.Random(45);records=[]
addresses={'PCLS':0x4698e4,'PDFL':0x4688f8,'LNAM':0x468b20,'LEPT':0x4688c8,'LUSP':0x4689a8}
for tag,address in addresses.items():
 for case in range(64):
  values=[rng.choice([-1,0,1,2]),rng.randrange(3),rng.randrange(3),rng.getrandbits(32)]+[rng.randrange(-5,100) for _ in range(12)]+[rng.randrange(256) for _ in range(3)]
  values=[v&0xffffffff for v in values]
  cpu.uc.mem_write(save,bytes(0x200));cpu.pointer(save+0x34,values[0]);cpu.pointer(0x9a6060,values[1]);cpu.pointer(save+0x3c,values[2]);cpu.pointer(save+0x38,values[3])
  for i in range(3):
   cpu.pointer(save+0x50+i*4,values[4+i]);cpu.pointer(save+0x5c+i*4,values[7+i]);cpu.pointer(save+0xfc+i*4,values[10+i]);cpu.pointer(save+0x40+i*4,values[13+i]);cpu.uc.mem_write(save+0x4c+i,bytes([values[16+i]]))
  output=bytearray();strings={};cpu.invoke(address,[cpu.data,save]);records.append(tag.encode()+struct.pack('<19I',*(v&0xffffffff for v in values))+struct.pack('<I',len(output))+output)
d=r/'port/level-world/reference/campaign-save-v45';blob=struct.pack('<I',len(records))+b''.join(records);(d/'metadata-original.bin').write_bytes(blob)
report={'cases':len(records),'whole_original_metadata_bodies':True,'stream_and_string_leaves':'explicit fixtures','sha256':hashlib.sha256(blob).hexdigest()};(d/'metadata-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
