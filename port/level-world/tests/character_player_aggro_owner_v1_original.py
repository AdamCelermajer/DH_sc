"""Original AISPlayer bodies; world/debug/audio calls are explicit fixtures."""
from pathlib import Path
import sys,struct,json,hashlib,itertools
r=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages');sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]})
ais=c.data+0x1000;other=c.data+0x2000;otherai=c.data+0x3000;online=c.data+0x4000;level=c.data+0x5000;scene=c.data+0x6000;sound=c.data+0x7000;threshold=c.data+0x8000;pending=c.data+0x9000
def word(a):return int.from_bytes(c.uc.mem_read(a,4),'little')
got=0x3ddb80+8+word(0x3dde08)
soundcell=c.data+0xa000;thresholdcell=c.data+0xa100;c.pointer(soundcell,sound);c.pointer(thresholdcell,threshold)
for offset in (word(0x3dde1c),word(0x3de0cc)):c.pointer(got+offset,soundcell)
for offset in (word(0x3dde30),):c.pointer(got+offset,thresholdcell)
c.pointer(otherai+0x14,5)
events=[];case=None
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,a,n,p):
 if a in (0x337888,0x337a88):ret()
 elif a==0x3140ec:c.uc.mem_write(c.reg(0),bytes(24));ret(c.reg(0))
 elif a in (0x708f00,0x310440):ret()
 elif a==0x7fd794:events.append(1);ret(online)
 elif a==0x36effc:events.append(2);ret(case[5])
 elif a==0x31f594:events.append(3);ret(level if case[6] else 0)
 elif a==0x3a3024:assert c.reg(0)==other;events.append(4);ret(otherai)
 elif a==0x369514:
  name=bytes(c.uc.mem_read(c.reg(1),20)).split(b'\0')[0];events.append(70 if name==b'combat' else 71);ret()
 elif a==0x36bd78:events.append(8);assert (c.reg(1),c.reg(2),c.reg(3))==(case[11]&0xffffffff,1,0),(case,[c.reg(i) for i in range(4)]);ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
records=[]
for mode,net,local,haslevel,enabled,ambient,music in itertools.product(range(2),range(2),range(2),range(2),range(2),range(2),range(2)):
 for initial in ((0,0,5),(1,5,5),(2,11,5)):
  # The deaggro music restart source requires genuine nonnull Level on reached branch.
  if mode and not haslevel and not ambient and initial[1]==initial[2] and music:continue
  case=[mode,*initial,net,local,haslevel,enabled,ambient,music,5,7,3]
  c.uc.mem_write(ais,bytes(0x100));c.pointer(ais+0xd0,initial[0]);c.pointer(ais+0xd4,initial[1]);c.pointer(ais+0x98,other+0x100)
  c.pointer(ais+0xc4,pending);c.pointer(ais+0xc8,pending+12);c.pointer(ais+0xcc,pending+24)
  c.pointer(otherai+0x14,initial[2]);c.uc.mem_write(online+5,bytes([net]));c.pointer(level+0x38,scene);c.pointer(level+0x120,7);c.uc.mem_write(scene+0x1c8,bytes([enabled]));c.uc.mem_write(sound+0x31,bytes([ambient,music]));c.pointer(threshold+0x14,5)
  events=[];c.invoke(0x3dde48 if mode else 0x3ddb70,[ais,other])
  out=[word(ais+0xd0),word(ais+0xd4),c.uc.mem_read(sound+0x31,1)[0],(word(ais+0xc8)-word(ais+0xc4))//4]
  records.append(struct.pack('<13i4I I',*case,*out,len(events))+struct.pack('<'+'I'*len(events),*events))
target=r/'port/level-world/reference/character-player-aggro-v1/source-fixture.bin';target.write_bytes(struct.pack('<I',len(records))+b''.join(records))
report={'validation':'PASS','original_instruction_cases':len(records),'original_sha256':hashlib.sha256((r/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(target.read_bytes()).hexdigest(),'world_debug_audio_services':'explicit fixtures','production_music_provider':False}
(r/'port/level-world/reports/character-player-aggro-v1-original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
