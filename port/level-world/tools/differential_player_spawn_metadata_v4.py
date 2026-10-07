from pathlib import Path
import sys,struct,json
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,str(root/'port/game-data/tests'))
from player_savegame_v1_original import Save
from unicorn import UC_HOOK_CODE
class Prefix(Save):
 def __init__(self):
  self.active=False;super().__init__();self.info=self.allocate(0x688);self.manager=self.allocate(0x800)
  self.pointer(self.info+0x380,0);self.pointer(self.info+0x660,0)
  self.rows=[];self.uc.hook_add(UC_HOOK_CODE,self.prefix_hook)
 def text(self,p):
  data=bytes(self.uc.mem_read(p,256));return data.split(b'\0')[0].decode()
 def prefix_hook(self,uc,a,z,u):
  if not self.active:return
  if a==0x36dfb0:
   assert self.reg(1)==self.internal&0xffffffff and self.reg(2)==0
   self.returned(self.info)
  elif a==0x30eae4:
   fmt=self.text(self.reg(1));assert fmt=='PlayerCharacter_%d'
   value=self.reg(2);value=value-(1<<32) if value&(1<<31) else value
   uc.mem_write(self.reg(0),(fmt%value).encode()+b'\0');self.returned()
  elif a==0x34b724:
   self.rows.append({'internal':self.internal,'template':self.text(self.reg(2)),'name':self.text(self.reg(3)),
    'first_bool':self.word(uc.reg_read(self.sp)),'second_bool':self.word(uc.reg_read(self.sp)+4)})
   # Bounded observation stop: restore only the harness frame, never claim the
   # remainder of AddCharacter executed or returned successfully.
   uc.reg_write(self.sp,self.stack+0xe000);uc.reg_write(self.pc,self.stop);uc.emu_stop()
old=Prefix();old.active=True
for value in (0,1,-1,17,-2147483648,2147483647):
 old.internal=value;old.invoke(0x372220,[old.manager,value])
assert len(old.rows)==6
for row in old.rows:
 assert row['name']=='PlayerCharacter_'+str(row['internal']) and row['template']=='Character'
 assert row['first_bool']==1 and row['second_bool']==1
report={'validation':'PASS','comparisons':6,'scope':'actual original AddCharacter prefix until Spawn call; GetPlayerInfo and printf are explicit service/libc projections; no whole AddCharacter success', 'rows':old.rows}
p=root/'port/level-world/reference/player-manager-owner-v1/player-spawn-metadata-original-v4.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
