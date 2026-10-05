from pathlib import Path
import sys,struct,itertools,json,hashlib
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages');sys.path.insert(0,str(REPO/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
old=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});new=Cpu(REPO/'.local-inputs/character_combat_sound_v1_arm64.so',True,{'functions':[]})
row=old.data+0x1000;ids=old.data+0x2000;attacker=old.data+0x3000;target=old.data+0x5000;vt=old.data+0x7000;pos=old.data+0x8000;result=old.data+0x9000;dead_callback=0x3a2ed4
old.uc.mem_write(attacker,bytes(0x1800));old.uc.mem_write(target,bytes(0x1800));old.pointer(attacker,vt);old.pointer(target,vt);old.pointer(vt+0x34,dead_callback);old.uc.mem_write(pos,struct.pack('<3f',1,2,3))
for i in range(4):old.uc.mem_write(ids+i*8,struct.pack('<2i',(i+1)*100,(i+1)*100+1))
current=None;events=[]
def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
def hook(uc,address,size,unused):
 if address==0x3a32d0:ret(row)
 elif address in [0x337888,0x3140ec,0x3139ac]:ret()
 elif address==0x337a88:ret(current[7])
 elif address==dead_callback:ret(current[6])
 elif address==0x3af6d8:ret(old.reg(0)-1)
 elif address==0x3935dc:ret(pos)
 elif address==0x36b5d8:
  sp=uc.reg_read(old.sp);args=struct.unpack('<3I',bytes(uc.mem_read(sp,12)))
  assert old.reg(3)==0 and args==(1,0xbf800000,0xbf800000)
  assert bytes(uc.mem_read(old.reg(2),12))==struct.pack('<3f',1,2,3)
  events.append(old.reg(1));ret()
old.uc.hook_add(UC_HOOK_CODE,hook)
cases=0
for values in itertools.product([0,2],repeat=4):
 for f1,f2,dead,minimal,amount,char in itertools.product([0,1],[0,1],[0,1],[0,1],[-1,0,1],[0,1]):
  current=values+(f1,f2,dead,minimal,amount&0xffffffff,char);events=[]
  raw=bytearray(40)
  for i,count in enumerate(values):struct.pack_into('<2I',raw,4+i*8,count,ids+i*8)
  raw[36]=f1;raw[37]=f2;old.uc.mem_write(row,bytes(raw));old.uc.mem_write(result,struct.pack('<i',amount)+bytes(100))
  try:old.invoke(0x3afee0 if char else 0x3afd38,[result,attacker,target],budget=200000)
  except Exception:print(current,hex(old.uc.reg_read(old.pc)),hex(old.reg(0)),flush=True);raise
  new.uc.mem_write(new.data+0x1000,struct.pack('<10I',*current));new.uc.mem_write(new.data+0x2000,bytes(16));rc=new.invoke('dh2_test_combat_sound_v1',[new.data+0x1000,new.data+0x2000]);actual=struct.unpack('<4I',bytes(new.uc.mem_read(new.data+0x2000,16)));assert rc==0 and list(actual[1:1+actual[0]])==events,(current,events,actual,rc);cases+=1
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report={'validation':'PASS','comparisons':cases,'scope':'Whole original Character F_ApplyCombatSound 3afee0 and GameObject overload3afd38 selection and ordered Play3D requests versus compiled ARM64 kernel. Row/debug/dead/shared Random/GetPosition and actual audio are explicit observer providers; no audio playback parity claimed. All death/hit/impact flags, empty/nonempty lists, positive/zero/negative amounts and MP_MinimalRandoms guards. Play3D positional=false,repeats1,volume/pitch=-1 exact.','original_sha256':sha(REPO/'.local-inputs/libDungeonHunter2.so'),'native_sha256':sha(REPO/'.local-inputs/character_combat_sound_v1_arm64.so'),'source_sha256':sha(ROOT/'character_combat_sound_v1.cpp')}
(ROOT/'reports/character-combat-sound-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
