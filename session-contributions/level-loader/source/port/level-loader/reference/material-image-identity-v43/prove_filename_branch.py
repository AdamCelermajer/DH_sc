import sys,struct,json
from pathlib import Path
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original
from unicorn import UC_HOOK_CODE
class Oracle(Original):
 def txt(self,p):
  if not p:return ''
  b=bytearray()
  while True:
   c=bytes(self.uc.mem_read(p+len(b),1))
   if c==b'\0':return b.decode()
   b.extend(c)
 def ss(self,p):return bytes(self.uc.mem_read(self.word(p+20),self.word(p+16)-self.word(p+20)))
 def store(self,p,b):
  if isinstance(b,str):b=b.encode()
  h=self.heap;self.heap+=(len(b)+32)&~15;self.uc.mem_write(h,b+b'\0');self.pointer(p+20,h);self.pointer(p+16,h+len(b));self.pointer(p+12,h+len(b)+1)
 def storage(self,uc,addr,size,unused):
  if addr==self.callback+112:self.returned(self.rootpath)
  elif addr==0x3209a8:self.store(self.reg(0),b'');self.returned()
  elif addr in (0x320b88,0x325ff4):
   self.store(self.reg(0),bytes(uc.mem_read(self.reg(1),self.reg(2)-self.reg(1))));self.returned()
  elif addr==0x320a4c:
   self.store(self.reg(0),self.ss(self.reg(0))+bytes(uc.mem_read(self.reg(1),self.reg(2)-self.reg(1))));self.returned()
  elif addr==0x34e324:self.store(self.reg(0),self.ss(self.reg(1))+self.ss(self.reg(2)));self.returned()
  elif addr==0x56c740:
   # Deliberately explicit basename endpoint; original kernel selects it.
   self.basename_calls+=1;self.store(self.reg(0),self.ss(self.reg(2)).replace(b'\\',b'/').rsplit(b'/',1)[-1]);self.returned()
  elif addr==0x34e090:self.store(self.reg(0),self.ss(self.reg(0)).lower());self.returned()
  elif addr==0x310450:self.returned()
  else:super().storage(uc,addr,size,unused)
 def external(self,uc,a,size,u):
  if a==self.callback+112:self.returned(self.rootpath)
  elif self.imports.get(a)=='strlen':self.returned(len(self.txt(self.reg(0))))
  elif self.imports.get(a)=='strstr':
   x,y=self.txt(self.reg(0)),self.txt(self.reg(1));pos=x.find(y);self.returned(self.reg(0)+pos if pos>=0 else 0)
  else:super().external(uc,a,size,u)
o=Oracle(Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so'),{'functions':[]})
owner=o.data+0x1000;vt=owner+0x100;rootpath=owner+0x200;inp=owner+0x300;out=owner+0x800;o.rootpath=rootpath;o.pointer(owner,vt);o.pointer(vt+44,o.callback+112);rows=[]
for root in ('q:/data/iphone','data',''):
 o.uc.mem_write(rootpath,root.encode()+b'\0')
 for s in ['q:/data/iphone/3d/textures/env_swamp.tga','Q:/data/iphone/3D/Textures/env_swamp.tga','data/3d/environments/textures/test.tga','q:/data/iphone/3d/modules/swamp/swamp.bdae','env_swamp.tga','data/3d/textures/UPPER.TGA']:
  o.basename_calls=0;o.uc.mem_write(inp,s.encode()+b'\0');o.uc.mem_write(out,bytes(32));o.invoke(0x34e96c,[out,owner,inp]);rows.append({'root':root,'input':s,'result':o.ss(out).decode(),'basename_calls':o.basename_calls})
print(json.dumps(rows,indent=2))
Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/material-image-identity-v43/filename-oracle.json').write_text(json.dumps({'scope':'Original branch and literal instructions; string storage/append/lower/basename are explicit endpoints, no real filesystem or texture GPU','cases':rows},indent=2))
