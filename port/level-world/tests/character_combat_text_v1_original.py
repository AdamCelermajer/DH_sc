"""Execute whole original Character scrolling text selector versus optimized ARM64.
Explicit actor/property/GameDesign/StringManager/queue callback fixtures.
The original source query order, color selection and text/value output execute.
"""
from pathlib import Path
import sys,struct,itertools,json,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
from unicorn import UC_HOOK_CODE
class Actor:
 def __init__(self,native):
  self.native=native;self.c=Cpu(ROOT/'.local-inputs/libcombat-text-v1-oracle.so'if native else ROOT/'.local-inputs/libDungeonHunter2.so',native,{'functions':[]})
  c=self.c;self.attacker=c.data+0x1000;self.target=c.data+0x4000;self.result=c.data+0x7000;self.services=c.data+0x8000;self.position=c.data+0x9000;self.text=c.data+0xa000
  if native:c.uc.mem_write(self.services,struct.pack('<10Q',self.attacker,*[c.stop+0x100+i*16 for i in range(9)]))
  else:
   c.pointer(self.target,self.target+0x1800);c.pointer(self.target+0x1800+0x28,c.stop+0x200)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def constant(self,group,key):
  self.trace.append(['constant',group,key]);token=(sum(map(ord,key))+len(group)*31)&0x7fffffff;self.constants[token]=key;return token
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native and c.stop+0x100<=at<c.stop+0x190:
   op=(at-c.stop-0x100)//16
   if op==0:self.trace.append(['follower']);uc.mem_write(c.reg(2),bytes([self.follower]));self.ret()
   elif op==1:self.trace.append(['position',c.reg(1)==self.target]);uc.mem_write(c.reg(2),struct.pack('<3f',1,2,3));self.ret()
   elif op==2:uc.mem_write(c.reg(2),struct.pack('<f',4));self.ret()
   elif op==3:self.trace.append(['property',c.reg(2)]);uc.mem_write(c.reg(3),words(self.property));self.ret()
   elif op==4:self.trace.append(['dual']);uc.mem_write(c.reg(2),bytes([self.dual]));self.ret()
   elif op==5:self.trace.append(['player']);uc.mem_write(c.reg(2),bytes([self.player]));self.ret()
   elif op==6:uc.mem_write(c.reg(3),words(self.constant(c.string(c.reg(1)).decode(),c.string(c.reg(2)).decode())));self.ret()
   elif op==7:
    key=self.constants[c.reg(1)];self.trace.append(['localized',key]);uc.mem_write(self.text,key.encode()+b'\0');c.pointer(c.reg(2),self.text);self.ret()
   elif op==8:
    style,x,y,z,text,number,color,numeric=struct.unpack('<Q3f4xQii?',uc.mem_read(c.reg(1),41));self.trace.append(['queue',c.string(style).decode(),[x,y,z],number if numeric else c.string(text).decode(),color]);self.ret()
   return
  if not self.native:
   if at==0x3a307c:self.trace.append(['follower']);self.ret(self.follower)
   elif at==0x413e90:self.ret(self.services)
   elif at==0x3935dc:self.trace.append(['position',c.reg(0)==self.target]);uc.mem_write(self.position,struct.pack('<3f',1,2,3));self.ret(self.position)
   elif at==0x3af76c:self.trace.append(['property',c.reg(1)]);self.ret(self.property)
   elif at==0x40019c:self.trace.append(['dual']);self.ret(self.dual)
   elif at==c.stop+0x200:self.trace.append(['player']);self.ret(self.player)
   elif at==0x4c4bdc:self.ret(self.constant(c.string(c.reg(1)).decode(),c.string(c.reg(2)).decode()))
   elif at==0x508edc:
    key=self.constants[c.reg(1)];self.trace.append(['localized',key]);uc.mem_write(self.text,key.encode()+b'\0');self.ret(self.text)
   elif at==0x414678:self.style=c.string(c.reg(1)).decode();self.ret(0)
   elif at in (0x413dc4,0x413fa0):
    p=list(struct.unpack('<3f',uc.mem_read(c.reg(2),12)));value=signed(c.reg(3))if at==0x413fa0 else c.string(c.reg(3)).decode();color=signed(struct.unpack('<I',uc.mem_read(uc.reg_read(c.sp),4))[0]);self.trace.append(['queue',self.style,p,value,color]);self.ret()
 def run(self,outcomes,mask,amount,follower,player,dual,prop):
  self.trace=[];self.constants={};self.follower=follower;self.player=player;self.dual=dual;self.property=prop;c=self.c
  c.uc.mem_write(self.result,words(amount,-1,-1,-1,0,0,outcomes,mask,-1,-1))
  if self.native:assert signed(c.invoke('dh2_combat_text_v1',[self.result,self.attacker,self.target,self.services]))==1
  else:
   c.uc.mem_write(self.target+0x14c,struct.pack('<f',1));c.uc.mem_write(self.target+0x158,struct.pack('<f',5));c.invoke(0x3af77c,[self.result,self.attacker,self.target])
  return self.trace
def main():
 old,native=Actor(False),Actor(True);cases=0;calls=0
 for args in itertools.product((0,1,2,4,8,12,32,256,288,511),(0,0x10000,0x4000000,0x20000000),(0,1,256,12345),(0,1),(0,1),(0,1),(0,256)):
  expected=old.run(*args);actual=native.run(*args);assert expected==actual,(args,expected,actual);cases+=1;calls+=len(expected)
 report=dict(validation='PASS',cases=cases,ordered_calls=calls,mismatches=0,scope=__doc__,original_sha256=hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),native_sha256=hashlib.sha256((ROOT/'.local-inputs/libcombat-text-v1-oracle.so').read_bytes()).hexdigest())
 (ROOT/'port/android-native/reports/character-combat-text-v1-original.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
