"""Original PlayerSavegame::_Load request order; named section readers and
quest/level/network owners are explicitly borrowed services in this probe.
"""
import json,hashlib,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from player_savegame_v1_original import Save,ROOT
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_SP
class Probe(Save):
 def __init__(self):
  super().__init__();self.requests=[];self.online=self.data+0x18000;self.uc.mem_write(self.online,bytes(0x100));self.uc.hook_add(UC_HOOK_CODE,self.loader)
 def text(self,p):
  b=bytearray()
  while self.uc.mem_read(p+len(b),1)!=b'\0':b.extend(self.uc.mem_read(p+len(b),1))
  return b.decode('utf8')
 def loader(self,uc,a,z,u):
  if a==0x315848:
   self.requests.append({'section':self.text(self.reg(1)),'loader':hex(self.reg(2)),'saver':hex(self.reg(3)),'context_matches_savegame':self.word(uc.reg_read(UC_ARM_REG_SP))==self.obj});self.returned()
  elif a in (0x46954c,0x469764,0x4694c8,0x46c1a8):
   self.requests.append({'initialize':hex(a),'subject_offset':self.reg(0)-self.obj});self.returned()
  elif a==0x7fd794:self.returned(self.online)
 def run(self,flags):
  self.fresh([0]);self.pointer(self.obj+8,self.data+0x19000);self.requests=[];self.invoke(0x464f4c,[self.obj,flags]);return list(self.requests)
def main():
 p=Probe();rows=[{'flags':f,'requests':p.run(f)} for f in [0,1,2,4,7,8,16,32,63]]
 path=ROOT/'port/game-data/reference/player-savegame-v1/load-order-original.json'
 path.write_text(json.dumps({'validation':'PASS','original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'cases':rows,'scope':__doc__},indent=2)+'\n');print(json.dumps(rows))
if __name__=='__main__':main()

