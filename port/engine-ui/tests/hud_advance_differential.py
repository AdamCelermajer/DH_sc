"""Full original notify_need_advance weak-parent gate vs native source fields.
Only weak-control deletion is explicit borrowed allocator/lifetime service.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../level-world/tests'))
from visual_timeline_differential import TimelineCpu
from navigation_search_differential import word
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;self.nodes=[c.data+0x1000+i*0x200 for i in range(10)];self.proxies=[c.data+0x4000+i*32 for i in range(10)];self.services=c.data+0x5000;c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def snapshot(self):
  c=self.c;raw=b''
  for i in range(self.n):
   p=self.nodes[i];parent=struct.unpack('<Q',c.uc.mem_read(p+8,8))[0] if self.native else word(c,p+0x40);proxy=struct.unpack('<Q',c.uc.mem_read(p+16,8))[0] if self.native else word(c,p+0x3c);dirty=c.uc.mem_read(p+(24 if self.native else 0x9d),1)[0];raw+=words(dirty,self.nodes.index(parent)+1 if parent else 0,self.proxies.index(proxy)+1 if proxy else 0,word(c,self.proxies[i]),c.uc.mem_read(self.proxies[i]+4,1)[0])
  return raw
 def destroy(self,proxy):
  self.events.append(words(self.proxies.index(proxy))+self.snapshot())
  if self.mutate:
   owner=self.nodes[self.dead];self.c.pointer(owner+(8 if self.native else 0x40),self.nodes[0]);self.c.pointer(owner+(16 if self.native else 0x3c),self.proxies[-1])
 def callback(self,address):assert address==self.c.callback+32;self.destroy(self.c.reg(1));self.c.put(0,1)
 def hook(self,uc,address,size,unused):
  if not self.native and address==0x752b38:self.destroy(self.c.reg(0));self.c.put(0,0);uc.reg_write(self.c.pc,uc.reg_read(self.c.lr))
 def run(self,raw):
  v=struct.unpack('<'+str(len(raw)//4)+'I',raw);self.n,self.dead,ref,self.mutate=v[:4];self.events=[];c=self.c
  for i in range(self.n):
   parent=self.nodes[i+1] if i+1<self.n else 0;proxy=self.proxies[i] if parent else 0;alive=0 if i==self.dead else (255 if i%3==0 else 1);c.uc.mem_write(self.proxies[i],words(ref)+bytes((alive,0,0,0)))
   if self.native:c.uc.mem_write(self.nodes[i],struct.pack('<3QB7x',i+1,parent,proxy,v[4+i]))
   else:c.uc.mem_write(self.nodes[i],bytes(0xa0));c.pointer(self.nodes[i]+0x40,parent);c.pointer(self.nodes[i]+0x3c,proxy);c.uc.mem_write(self.nodes[i]+0x9d,bytes((v[4+i],)))
  if self.native:c.uc.mem_write(self.services,struct.pack('<2Q',0,c.callback+32));assert c.invoke('dh2_ui_hud_notify_v1',[self.nodes[0],self.services])==0
  else:c.invoke(0x7750e8,[self.nodes[0]])
  return self.snapshot(),tuple(self.events)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();manifest=json.loads((ROOT/'reference/hud-sprite-timeline/original-functions.json').read_text());old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});records=[];deletes=0
 for n in range(1,10):
  for dead in list(range(n-1))+[0xffffffff]:
   for ref in (0,1,2,0x80000000,0xffffffff):
    for mutate in (0,1):
     raw=words(n,dead,ref,mutate,*[(i+n)%2 for i in range(n)]);expected,events=old.run(raw);actual,observed=new.run(raw);assert expected==actual,(n,dead,ref,mutate);assert events==observed;deletes+=len(events);records.append(words(len(raw),len(expected),len(events))+raw+expected+b''.join(events))
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(words(0x31414448,len(records))+b''.join(records));report=dict(validation='PASS',comparisons=len(records),deletion_callbacks=deletes,mismatches=0,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),gold_sha256=sha(a.gold),source_sha256={p.name:sha(p) for p in (ROOT/'hud_advance.hpp',ROOT/'hud_advance.cpp')},scope=__doc__);a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
