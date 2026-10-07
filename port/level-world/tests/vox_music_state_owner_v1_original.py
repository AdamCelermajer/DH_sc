from pathlib import Path
import sys,struct,json,hashlib,itertools
r=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages');sys.path.insert(0,str(r/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
c=Cpu(r/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});owner=c.data+0x1000;disabled=c.data+0x2000;table=c.data+0x3000;cell=c.data+0x4000;channels=c.data+0x5000;state=c.data+0x6000
def word(a):return int.from_bytes(c.uc.mem_read(a,4),'little')
got=0x369530+8+word(0x3695e4);c.pointer(got+word(0x3695e8),disabled);c.pointer(got+word(0x3695ec),cell);c.pointer(cell,table);c.pointer(table+4,2);c.uc.mem_write(state,b'combat\0')
events=[];case=None;constructor_prefix=False
def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def hook(uc,a,n,p):
 if constructor_prefix and a==0x36c82c:c.uc.reg_write(c.sp,c.stack+0xe000);c.uc.reg_write(c.pc,c.stop) # Explicit prefix observer exit, not complete constructor.
 elif a==0x862548:assert c.reg(1)==0x8888 and c.reg(3)==1;events.append(4);ret(case[3]&0xffffffff)
 elif a==0x861928:assert c.reg(2)==state;events.append(5);ret()
 elif a==0x8683ac:events.append(6);ret()
c.uc.hook_add(UC_HOOK_CODE,hook);records=[]
constructor_prefix=True;c.uc.mem_write(owner,bytes([255])*0x80);c.invoke(0x36c7b0,[owner]);prefix=bytes(c.uc.mem_read(owner+0x24,16));assert struct.unpack('<3I4B',prefix)==(0xffffffff,0xffffffff,0xffffffff,1,1,0,0);constructor_prefix=False
(r/'port/level-world/reference/character-player-aggro-v1/vox-constructor-prefix.bin').write_bytes(prefix)
for music,mute,channel,count in itertools.product((-1,0),(0,1),(0,0x8888),(-1,0,1)):
 case=[music,mute,channel,count];c.uc.mem_write(owner,bytes(0x40));c.pointer(owner+0x24,music&0xffffffff);c.pointer(owner+8,channels);c.pointer(channels+8,channel);c.uc.mem_write(disabled,bytes([mute]));events=[];c.invoke(0x369514,[owner,state]);records.append(struct.pack('<4iI',*case,len(events))+struct.pack('<'+'I'*len(events),*events))
p=r/'port/level-world/reference/character-player-aggro-v1/vox-source-fixture.bin';p.write_bytes(struct.pack('<I',len(records))+b''.join(records));report={'validation':'PASS','original_cases':len(records),'source_sha256':hashlib.sha256((r/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'oracle_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'audio_services':'explicit source call observers'};(r/'port/level-world/reports/vox-music-state-v1-original-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
